
#include "ThreadPool.h"


CThreadPool::CThreadPool()
:m_bIsInit(false)
,m_iInitialNum(0)
,m_iMaxNum(0)
,m_hMangerThd(NULL)
,m_bMangerIsExit(false)
,m_hMangerIocp(NULL)
,m_hWorkIocp(NULL)
,m_pstWorkThdInfo(NULL)
,m_hMangerExitEvent(NULL)
{
	
}

CThreadPool::~CThreadPool()
{
	
}

int CThreadPool::Init( int iInitialNum, int iMaxNum )
{
	if (m_bIsInit)
	{
		return -1;
	}

	do 
	{
		m_iInitialNum = iInitialNum;
		m_iMaxNum = iMaxNum;

		m_pstWorkThdInfo = new (std::nothrow) THREAD_INFO_S [iMaxNum];
		if (NULL == m_pstWorkThdInfo)
		{
			break;
		}

		m_hMangerIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
		if (NULL == m_hMangerIocp)
		{
			break;
		}

		m_hWorkIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
		if (NULL == m_hWorkIocp)
		{
			break;
		}
		
		m_hMangerExitEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		if (NULL == m_hMangerExitEvent)
		{
			break;
		}

		unsigned iThreadId = 0;
		m_bMangerIsExit = false;
		m_hMangerThd = (HANDLE)_beginthreadex(NULL, 0, MangerThread, this, 0, &iThreadId);
		if (NULL == m_hMangerThd)
		{
			break;
		}

		for (int i = 0; i < iInitialNum; i++)
		{
			m_pstWorkThdInfo[i].hHandle = (HANDLE)_beginthreadex(NULL, 0, WorkThread, this, 0, &m_pstWorkThdInfo[i].iThreadId);
		}

		m_bIsInit = true;

		return 0;
	} while (0);

	if (NULL != m_pstWorkThdInfo)
	{
		delete m_pstWorkThdInfo;
		m_pstWorkThdInfo = NULL;
	}

	if (NULL != m_hMangerIocp)
	{
		CloseHandle(m_hMangerIocp);
		m_hMangerIocp = NULL;
	}

	if (NULL != m_hWorkIocp)
	{
		CloseHandle(m_hWorkIocp);
		m_hWorkIocp = NULL;
	}

	if (NULL != m_hMangerExitEvent)
	{
		CloseHandle(m_hMangerExitEvent);
		m_hMangerExitEvent = NULL;
	}
	
	return -1;
}

void CThreadPool::Fini()
{
	if (!m_bIsInit)
	{
		return;
	}

	//退出工作线程
	for (int i = 0; i < m_iMaxNum; i++)
	{
		if (m_pstWorkThdInfo[i].iThreadId > 0)
		{
			m_pstWorkThdInfo[i].bIsExit = true;
			while (1)
			{
				if (m_pstWorkThdInfo[i].hHandle == NULL)
				{
					break;
				}
				else
				{
					Sleep(10);
				}
			}
		}
	}

	//退出管理线程
	m_bMangerIsExit = true;
	if (NULL != m_hMangerThd)
	{
		SetEvent(m_hMangerExitEvent);
		WaitForSingleObject(m_hMangerThd, INFINITE);

		CloseHandle(m_hMangerExitEvent);
		m_hMangerExitEvent = NULL;
	}

	//清除待处理数据
	unsigned long lNumberOfByte = 0;
	unsigned long lCompletionKey = 0;
	OVERLAPPED*	pOverLapped = NULL;
	while (GetQueuedCompletionStatus(m_hWorkIocp, &lNumberOfByte, &lCompletionKey, &pOverLapped, IOCP_QUERY_INTEAL))
	{
		IWorkData* pWorkData = reinterpret_cast<IWorkData*> (lCompletionKey);

		if (NULL != pWorkData)
		{
			delete pWorkData;
			pWorkData = NULL;
		}
	}

	if (NULL != m_hMangerIocp)
	{
		CloseHandle(m_hMangerIocp);
		m_hMangerIocp = NULL;
	}

	if (NULL != m_hWorkIocp)
	{
		CloseHandle(m_hWorkIocp);
		m_hWorkIocp = NULL;
	}

	if (NULL != m_pstWorkThdInfo)
	{
		delete m_pstWorkThdInfo;
		m_pstWorkThdInfo = NULL;
	}

	m_bIsInit = false;
}

unsigned __stdcall CThreadPool::MangerThread(void* pParam)
{
	printf("begin manager thd, id:%d\n", GetCurrentThreadId());

	CThreadPool* pThis = static_cast<CThreadPool*> (pParam);
	if (NULL == pParam)
	{
		return -1;
	}

	unsigned long lNumberOfByte = 0;
	unsigned long lCompletionKey = 0;
	OVERLAPPED*	pOverLapped = NULL;

	while (!pThis->m_bMangerIsExit)
	{
		if (WaitForSingleObject(pThis->m_hMangerExitEvent, 0) == WAIT_OBJECT_0)
		{
			printf("break manager thd, id:%d\n", GetCurrentThreadId());
			break;
		}

		if (TRUE == GetQueuedCompletionStatus(pThis->m_hMangerIocp, &lNumberOfByte, &lCompletionKey, &pOverLapped, IOCP_QUERY_INTEAL))
		{
			if (pOverLapped == (OVERLAPPED*) 0xFFFFFFFF)
			{
				break;
			}
		}
		else
		{
			THREADPOOL_STATUS_E ePoolStatu = pThis->GetThreadPoolStatus();
			if (THREAD_POOL_BUSY == ePoolStatu)
			{
				pThis->AddThread();
			}
			else if (THREAD_POOL_IDLE == ePoolStatu)
			{
				pThis->DeleteThread();
			}
		}
	}

	printf("end manager thd, id:%d\n", GetCurrentThreadId());
	return 0;
}

unsigned __stdcall CThreadPool::WorkThread(void* pParam)
{
	printf("begin work thd, id:%d\n", GetCurrentThreadId());
	CThreadPool* pThis = static_cast<CThreadPool*> (pParam);
	if (NULL == pParam)
	{
		return -1;
	}

	int iIndex = pThis->GetIndexByThreadId(GetCurrentThreadId());
	if (iIndex < 0)
	{
		return -1;
	}

	unsigned long lNumberOfByte = 0;
	unsigned long lCompletionKey = 0;
	OVERLAPPED*	pOverLapped = NULL;

	while (!pThis->m_pstWorkThdInfo[iIndex].bIsExit)
	{
		if (TRUE == GetQueuedCompletionStatus(pThis->m_hWorkIocp, &lNumberOfByte, &lCompletionKey, &pOverLapped, IOCP_QUERY_INTEAL))
		{
			pThis->m_pstWorkThdInfo[iIndex].bIsBuy = true;
			IWork* pWork = reinterpret_cast<IWork*> (lNumberOfByte);
			IWorkData* pWorkData = reinterpret_cast<IWorkData*> (lCompletionKey);
			if (NULL != pWork)
			{
				pWork->ProcessJob(pWorkData);
			}
			if (NULL != pWorkData)
			{
				delete pWorkData;
				pWorkData = NULL;
			}
			pThis->m_pstWorkThdInfo[iIndex].bIsBuy = false;
		}
	}

	CloseHandle(pThis->m_pstWorkThdInfo[iIndex].hHandle);
	pThis->m_pstWorkThdInfo[iIndex].hHandle = NULL;
	pThis->m_pstWorkThdInfo[iIndex].iThreadId = 0;

	printf("end work thd, id:%d\n", GetCurrentThreadId());
	return 0;
}

THREADPOOL_STATUS_E CThreadPool::GetThreadPoolStatus()
{
	int iTotalNum = 0;
	int iBusyNum = 0;
	for (int i = 0; i < m_iMaxNum; i++)
	{
		if (m_pstWorkThdInfo[i].iThreadId > 0)
		{
			iTotalNum++;

			if (m_pstWorkThdInfo[i].bIsBuy)
			{
				iBusyNum++;
			}
		}
	}

	printf("total:%d, busy:%d\n", iTotalNum, iBusyNum);

	double dRate = (1.0 * iBusyNum) / iTotalNum;
	if (dRate > 0.8)
	{
		return THREAD_POOL_BUSY;
	}
	if (dRate < 0.2)
	{
		return THREAD_POOL_IDLE;
	}

	return THREAD_POOL_NORMAL;
}

void CThreadPool::AddThread()
{
	for (int i = 0; i < m_iMaxNum; i++)
	{
		if (m_pstWorkThdInfo[i].iThreadId == 0)
		{
			m_pstWorkThdInfo[i].hHandle = (HANDLE)_beginthreadex(NULL, 0, WorkThread, this, 0, &m_pstWorkThdInfo[i].iThreadId);
			break;
		}
	}
}

void CThreadPool::DeleteThread()
{
	for (int i = m_iMaxNum - 1; i >= m_iInitialNum; i--)
	{
		if (m_pstWorkThdInfo[i].iThreadId > 0)
		{
			m_pstWorkThdInfo[i].bIsExit = true;
		}
	}
}

int CThreadPool::GetIndexByThreadId(unsigned iThreadId)
{
	for (int i = 0; i < m_iMaxNum; i++)
	{
		if (m_pstWorkThdInfo[i].iThreadId == iThreadId)
		{
			return i;
		}
	}

	return -1;
}

void CThreadPool::InputWork(IWork* pWork, IWorkData* pWorkData)
{
	PostQueuedCompletionStatus(m_hWorkIocp, reinterpret_cast<DWORD>(pWork), reinterpret_cast<DWORD>(pWorkData), NULL);
}
