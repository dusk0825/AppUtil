
#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <Windows.h>
#include <process.h>
#include <iostream>

#define IOCP_QUERY_INTEAL	200

class IWorkData
{
public:
	IWorkData(){}
	virtual ~IWorkData(){}
};

class IWork
{
public:
	IWork(){}
	virtual ~IWork(){}

	virtual void ProcessJob(IWorkData* pWorkData)=0;
};

typedef struct tagThreadInfo
{
	HANDLE		hHandle;
	unsigned	iThreadId;
	bool		bIsBuy;
	bool		bIsExit;

	tagThreadInfo()
	{
		hHandle = NULL;
		iThreadId = 0;
		bIsBuy = false;
		bIsExit = false;
	}
}THREAD_INFO_S;

typedef enum tagThreadPoolStatus
{
	THREAD_POOL_IDLE,
	THREAD_POOL_NORMAL,
	THREAD_POOL_BUSY
}THREADPOOL_STATUS_E;

class CThreadPool
{
public:
	~CThreadPool();

	static CThreadPool* Instance(){static CThreadPool oSingleObj; return &oSingleObj;}

	/**	@fn	int Init(int iInitialNum, int iMaxNum);
	*	@brief ��ʼ���̳߳�
	*	@param [in]  iInitialNum	��ʼ�߳���
	*	@param [in]  iMaxNum		����߳���
	*	@return	�ɹ�����0������Ϊ������
	*/
	int Init(int iInitialNum, int iMaxNum);

	/**	@fn	void Fini();
	*	@brief ����ʼ���̳߳�
	*	@return	��
	*/
	void Fini();

	/**	@fn	void InputWork(IWork* pWork, IWorkData* pWorkData);
	*	@brief Ͷ������
	*	@param [in]  pWork		����
	*	@param [in]  pWorkData	��������
	*	@return	�ɹ�����0������Ϊ������
	*/
	void InputWork(IWork* pWork, IWorkData* pWorkData);

protected:
	static unsigned __stdcall MangerThread(void* pParam);
	static unsigned __stdcall WorkThread(void* pParam);

	void AddThread();
	void DeleteThread();
	int GetIndexByThreadId(unsigned iThreadId);

	bool			m_bMangerIsExit;
	HANDLE			m_hMangerExitEvent;

private:
	CThreadPool();
	THREADPOOL_STATUS_E GetThreadPoolStatus();

	bool			m_bIsInit;
	int				m_iInitialNum;
	int				m_iMaxNum;
	THREAD_INFO_S*	m_pstWorkThdInfo;

	HANDLE			m_hMangerThd;
	HANDLE			m_hMangerIocp;
	HANDLE			m_hWorkIocp;
};

#endif