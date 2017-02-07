
#include "WinApiWrap.h"
#include <winsock2.h>
#include <windows.h>
#include <tlhelp32.h>
#include <Psapi.h>

#pragma comment(lib,"Psapi.lib")

CWinApiWrap::CWinApiWrap()
{

}

CWinApiWrap::~CWinApiWrap()
{

}

int CWinApiWrap::Init()
{
	WORD wVersionRequested = MAKEWORD(2, 2);  

	WSADATA wsaData;  
	if (WSAStartup(wVersionRequested, &wsaData) != 0)
	{
		LOG_ERROR("WSAStartup errCode:%d", WSAGetLastError());
		return DTK_ERROR;
	}

	LOG_INFO("Init succ.");
	return DTK_OK;
}

void CWinApiWrap::Fini()
{
	WSACleanup();
	LOG_INFO("Fini succ.");
}

int CWinApiWrap::GetProcInfo(std::map<int, std::string>& mapProcInfo)
{
	int iRet = DTK_ERROR;
	HANDLE DTKocessSnap = INVALID_HANDLE_VALUE;

	do 
	{
		PROCESSENTRY32 stEntry;
		stEntry.dwSize = sizeof(stEntry);

		DTKocessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (INVALID_HANDLE_VALUE == DTKocessSnap)
		{  
			LOG_ERROR("CreateToolhelp32Snapshot errCode:%d", GetLastError());
			break;  
		}

		BOOL bMore = Process32First(DTKocessSnap, &stEntry);
		while (TRUE == bMore)
		{
			mapProcInfo.insert(std::make_pair(stEntry.th32ProcessID, stEntry.szExeFile));
			bMore = Process32Next(DTKocessSnap, &stEntry);
		}

		iRet = DTK_OK;
	} while (0);

	if (DTKocessSnap != INVALID_HANDLE_VALUE)
	{
		CloseHandle(DTKocessSnap);
		DTKocessSnap = INVALID_HANDLE_VALUE;
	}

	return iRet;
}

int CWinApiWrap::GetSvrInfo(std::map<int, std::string>& mapSvrInfo)
{
	char *pBuf = NULL;
	SC_HANDLE hSCM = NULL;
	int iRet = DTK_ERROR;

	do
	{
		//建立了一个到服务控制管理器的连接
		hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE | SC_MANAGER_CONNECT);
		if(NULL == hSCM)
		{
			LOG_ERROR("OpenSCManager errCode:%d", GetLastError());
			break;
		}

		//获取需要的缓冲区大小
		DWORD dwBufSize = 0; 
		DWORD dwBufNeed = 0;
		DWORD dwNumberOfService = 0;

		EnumServicesStatusEx(hSCM,SC_ENUM_PROCESS_INFO,SERVICE_WIN32,SERVICE_STATE_ALL,NULL,dwBufSize,&dwBufNeed,&dwNumberOfService,NULL,NULL);

		dwBufSize = dwBufNeed + sizeof(ENUM_SERVICE_STATUS_PROCESS);
		pBuf = new (std::nothrow) char[dwBufSize];
		if(NULL == pBuf)  
		{  
			LOG_ERROR("new memory error, %d", GetLastError());  
			break;
		}  
		memset(pBuf, 0, dwBufSize);  

		//获取服务信息  
		BOOL bRet = EnumServicesStatusEx(hSCM, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL,(LPBYTE)pBuf, dwBufSize, &dwBufNeed, &dwNumberOfService, NULL, NULL);  
		if(bRet == FALSE)  
		{  
			LOG_ERROR("EnumServicesStatusEx errCode:%d", GetLastError());   
			break;
		}

		ENUM_SERVICE_STATUS_PROCESS* pServiceInfo = (LPENUM_SERVICE_STATUS_PROCESS) pBuf;
		for (unsigned int i = 0; i < dwNumberOfService; i++)
		{
			mapSvrInfo.insert(std::make_pair(pServiceInfo[i].ServiceStatusProcess.dwProcessId, pServiceInfo[i].lpServiceName));
		}

		iRet = DTK_OK;
	} while (0);

	if (NULL != hSCM)
	{
		CloseServiceHandle(hSCM);
		hSCM = NULL;
	}

	if (NULL != pBuf)
	{
		delete[] pBuf;
		pBuf = NULL;
	}

	return iRet;
}

int CWinApiWrap::GetProcPathByPid(DWORD dwProcessID, char szProcPath[])
{
	int iRet = DTK_ERROR;
	HANDLE hToken = NULL;

	do 
	{
		//提升程序权限
		if (FALSE == OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
		{
			LOG_ERROR("OpenProcessToken %d", GetLastError());
			break;
		}

		TOKEN_PRIVILEGES stTokenPrivi;
		if (FALSE == LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &stTokenPrivi.Privileges[0].Luid))//修改进程权限
		{
			LOG_ERROR("LookupPrivilegeValue %d\n", GetLastError());
			break;
		}

		stTokenPrivi.PrivilegeCount = 1;
		stTokenPrivi.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		if (FALSE == AdjustTokenPrivileges(hToken, FALSE, &stTokenPrivi, sizeof(TOKEN_PRIVILEGES), NULL, NULL))//通知系统修改进程权限
		{
			LOG_ERROR("AdjustTokenPrivileges %d\n", GetLastError());
			break;
		}

		HANDLE DTKocess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessID);
		if (NULL == DTKocess)  
		{   
			break;
		}

		DWORD dwPathLen = 256;
		if(FALSE == QueryFullProcessImageName(DTKocess, 0, szProcPath, &dwPathLen))
		{
			LOG_ERROR("QueryFullProcessImageName errCode: %d\n", GetLastError());
			break;
		}

// 		char szDrive[PMT_MAX_DRIVE] = {0};
// 		char szDir[PMT_MAX_DIR] = {0};
// 		char szFname[PMT_MAX_FNAME] = {0};
// 		char szExt[PMT_MAX_EXT] = {0};
// 		_splitpath_s(szTempPath, szDrive, PMT_MAX_DRIVE, szDir, PMT_MAX_DIR, szFname, PMT_MAX_FNAME, szExt, PMT_MAX_EXT);
// 		DTK_Snprintf(szProcPath, PMT_MAX_PATH, "%s%s", szDrive, szDir);

		iRet = DTK_OK;
	} while (0);

	if (NULL != hToken)
	{
		CloseHandle(hToken);
		hToken = NULL;
	}

	return iRet;
}

int CWinApiWrap::GetPidByAppName(std::string strAppName, int& iPid)
{
	int iRet = DTK_ERROR;

	std::map<int, std::string> mapProcInfo;
	if (DTK_OK != CWinApiWrap::Instance()->GetProcInfo(mapProcInfo))
	{
		LOG_ERROR("GetProcInfo error.");
		return iRet;
	}

	iPid = -1;
	for (std::map<int, std::string>::iterator it = mapProcInfo.begin(); it != mapProcInfo.end(); ++it)
	{
		if (0 == it->second.compare(strAppName))
		{
			iPid = it->first;
			break;
		}
	}

	return DTK_OK;
}

int CWinApiWrap::GetLocalIp(std::string& strIp)
{
	strIp = "";
	char szHostname[255] = {0};  
	if (gethostname(szHostname, sizeof(szHostname)) != 0)
	{
		LOG_ERROR("gethostname errCode:%d", WSAGetLastError());
		return DTK_ERROR;
	}

	hostent* pHost = gethostbyname(szHostname);  
	if (NULL == pHost)  
	{
		LOG_ERROR("gethostbyname errCode:%d", WSAGetLastError());
		return DTK_ERROR;
	}

	in_addr szAddr;  
	memcpy(&szAddr, pHost->h_addr_list[0], sizeof(in_addr));//仅获取第一个ip  
	strIp.assign(inet_ntoa(szAddr));

	return DTK_OK;
}

int CWinApiWrap::ServiceControl(const std::string strSvrName, bool bRun)
{
	int iRet = DTK_ERROR;
	SC_HANDLE hSC = NULL;
	SC_HANDLE hSvc = NULL;

	do 
	{
		hSC = OpenSCManager(NULL, NULL, GENERIC_EXECUTE);
		if (NULL == hSC)
		{
			LOG_ERROR("OpenSCManager errCode:%d", GetLastError());
			break;
		}

		hSvc = OpenService( hSC, strSvrName.c_str(), SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);
		if (hSvc == NULL)
		{
			LOG_ERROR("OpenService %s errCode:%d", strSvrName.c_str(),GetLastError());
			break;
		}

		SERVICE_STATUS status;
		if (FALSE == QueryServiceStatus(hSvc, &status))
		{
			LOG_ERROR("QueryServiceStatus errCode:%d", GetLastError());
			break;
		}

		if (SERVICE_RUNNING == status.dwCurrentState && !bRun)
		{
			// 停止服务
			if (FALSE  == ControlService(hSvc,SERVICE_CONTROL_STOP, &status))
			{
				LOG_ERROR("ControlService errCode:%d", GetLastError());
				break;
			}
			// 等待服务停止
			while (TRUE == QueryServiceStatus(hSvc, &status))
			{
				Sleep(status.dwWaitHint);
				if (SERVICE_STOPPED  == status.dwCurrentState)
				{
					break;
				}
			}
		}
		else if (SERVICE_STOPPED  == status.dwCurrentState && bRun)
		{
			// 启动服务
			if (FALSE == StartService(hSvc, NULL, NULL))
			{
				LOG_ERROR("StartService errCode:%d", GetLastError());
				break;
			}
			// 等待服务启动
			while (TRUE == QueryServiceStatus(hSvc, &status))
			{
				Sleep(status.dwWaitHint);
				if (SERVICE_RUNNING == status.dwCurrentState)
				{
					break;
				}
			}
		}

		iRet = DTK_OK;
	} while (0);
	
	if (NULL == hSvc)
	{
		CloseServiceHandle(hSvc);
		hSvc = NULL;
	}

	if (NULL == hSC)
	{
		CloseServiceHandle(hSC);
		hSC = NULL;
	}

	return iRet;
}

int CWinApiWrap::GetServiceStatus(SVR_INFO_ST& stSvrInfo)
{
	int iRet = DTK_ERROR;
	SC_HANDLE hSC = NULL;
	SC_HANDLE hSvc = NULL;

	do 
	{
		hSC = OpenSCManager(NULL, NULL, GENERIC_EXECUTE);
		if (NULL == hSC)
		{
			LOG_ERROR("OpenSCManager errCode:%d", GetLastError());
			break;
		}

		hSvc = OpenService( hSC, stSvrInfo.strSvrName.c_str(), SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);
		if (hSvc == NULL)
		{
			LOG_ERROR("OpenService %s errCode:%d", stSvrInfo.strSvrName.c_str(),GetLastError());
			break;
		}

		SERVICE_STATUS status;
		if (FALSE == QueryServiceStatus(hSvc, &status))
		{
			LOG_ERROR("QueryServiceStatus errCode:%d", GetLastError());
			break;
		}

		if (status.dwCurrentState == SERVICE_STOPPED)
		{
			stSvrInfo.iState = SVR_STATUS_STOP;
			LOG_INFO("Stop Service of %s Suc.", stSvrInfo.strSvrName.c_str());
		}

		iRet = DTK_OK;
	} while (0);

	if (NULL == hSvc)
	{
		CloseServiceHandle(hSvc);
		hSvc = NULL;
	}

	if (NULL == hSC)
	{
		CloseServiceHandle(hSC);
		hSC = NULL;
	}

	return iRet;
}

int CWinApiWrap::StopService(std::vector<SVR_INFO_ST>& vctSvrList)
{
	int iRet = DTK_OK;
	SC_HANDLE hSC = NULL;
	SC_HANDLE hSvc = NULL;

	do 
	{
		hSC = OpenSCManager(NULL, NULL, GENERIC_EXECUTE);
		if (NULL == hSC)
		{
			LOG_ERROR("OpenSCManager errCode:%d", GetLastError());
			iRet = DTK_ERROR;
			break;
		}

		for (std::vector<SVR_INFO_ST>::iterator it = vctSvrList.begin(); it != vctSvrList.end();)
		{
			hSvc = OpenService( hSC, (*it).strSvrName.c_str(), SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);
			if (hSvc == NULL)
			{
				LOG_ERROR("OpenService %s not exist", (*it).strSvrName.c_str());
				it = vctSvrList.erase(it);
				continue;
			}

			SERVICE_STATUS status;
			if (FALSE == QueryServiceStatus(hSvc, &status))
			{
				LOG_ERROR("QueryServiceStatus errCode:%d", GetLastError());
				iRet = DTK_ERROR;
				break;
			}

			if (SERVICE_RUNNING == status.dwCurrentState)
			{
				// 停止服务
				LOG_INFO("Begin Stop Service of %s", (*it).strSvrName.c_str());
				if (FALSE  == ControlService(hSvc,SERVICE_CONTROL_STOP, &status))
				{
					LOG_ERROR("ControlService errCode:%d", GetLastError());
					iRet = DTK_ERROR;
					break;
				}
				(*it).iState = SVR_STATUS_RUN;
			}
			else
			{
				LOG_INFO("Service of %s is not running.", (*it).strSvrName.c_str());
				(*it).iState = SVR_STATUS_STOP;
			}
			

			if (NULL == hSvc)
			{
				CloseServiceHandle(hSvc);
				hSvc = NULL;
			}

			++it;
		}
	} while (0);

	if (NULL == hSvc)
	{
		CloseServiceHandle(hSvc);
		hSvc = NULL;
	}

	if (NULL == hSC)
	{
		CloseServiceHandle(hSC);
		hSC = NULL;
	}

	return iRet;
}

int CWinApiWrap::GetAppSetupPath(std::string strAppName, std::string& strPath)
{
	int iPid = -1;
	GetPidByAppName(strAppName.c_str(), iPid);
	if (iPid < 0)
	{
		LOG_ERROR("can't find %s", strAppName.c_str());
		return DTK_ERROR;
	}

	char szPath[256] = {0};
	if (DTK_OK != GetProcPathByPid(iPid, szPath))
	{
		LOG_ERROR("can't find pid:%d path.", iPid);
		return DTK_ERROR;
	}
	strPath = std::string(szPath);

	return DTK_OK;
}

int CWinApiWrap::GetSetupPathBySvrName(std::string strSvrName, std::string& strPath)
{
	std::map<int, std::string> mapSvrInfo;
	if (DTK_OK != CWinApiWrap::Instance()->GetSvrInfo(mapSvrInfo))
	{
		LOG_ERROR("get localhost service infomation error.");
		return DTK_ERROR;
	}

	int iPid = -1;
	for (std::map<int, std::string>::iterator it = mapSvrInfo.begin(); it != mapSvrInfo.end(); ++it)
	{
		if (strSvrName == it->second)
		{
			iPid = it->first;
			break;
		}
	}
	if (iPid < 0)
	{
		LOG_ERROR("can't find service: %s", strSvrName.c_str());
		return DTK_ERROR;
	}
	else if (iPid == 0)
	{
		LOG_WARN("service %s have't running.", strSvrName.c_str());
		return DTK_ERROR;
	}

	char szPath[256] = {0};
	if (DTK_OK != GetProcPathByPid(iPid, szPath))
	{
		LOG_ERROR("can't find pid:%d path.", iPid);
		return DTK_ERROR;
	}
	strPath = szPath;

	return DTK_OK;
}

int CWinApiWrap::KillProcessByAppName(const std::string& strAppName)
{
	int iPid = 0;
	if (DTK_OK != GetPidByAppName(strAppName, iPid))
	{
		return DTK_ERROR;
	}

	HANDLE hHandle = OpenProcess(PROCESS_TERMINATE, FALSE, iPid);
	if (NULL == hHandle)
	{
		LOG_ERROR("OpenProcess errCode:%d", GetLastError());
		return DTK_ERROR;
	}

	if(FALSE == TerminateProcess(hHandle,0))
	{
		LOG_ERROR("TerminateProcess errCode:%d", GetLastError());
		return DTK_ERROR;
	}

	return DTK_OK;
}
