
#include "PortDetect.h"
#include "WinApiWrap.h"
#include <winsock2.h>
#include <iphlpapi.h>
#include <windows.h>

#pragma comment(lib,"Iphlpapi.Lib")
#pragma comment(lib,"ws2_32.Lib")

CPortDetect::CPortDetect()
{

}

CPortDetect::~CPortDetect()
{

}

DTK_INT32 CPortDetect::GetTcpPortList(std::map<int, PORT_DETAIL_ST> &mapTcpPort)
{
    DTK_INT32 iRet = DTK_ERROR;
    PMIB_TCPTABLE_OWNER_PID pTcpTable = NULL;

    do 
    {
        DWORD dwSize = sizeof(MIB_TCPTABLE_OWNER_PID);
        pTcpTable = (MIB_TCPTABLE_OWNER_PID *) new (std::nothrow) MIB_TCPTABLE_OWNER_PID [dwSize];
        if (NULL == pTcpTable) 
        {
            break;
        }

        DWORD dwRetVal = GetExtendedTcpTable(pTcpTable, &dwSize, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0);
        if (ERROR_INSUFFICIENT_BUFFER == dwRetVal)
        {
            if (NULL != pTcpTable)
            {
                delete []pTcpTable;
                pTcpTable = NULL;
            }

            pTcpTable = (MIB_TCPTABLE_OWNER_PID *) new (std::nothrow) MIB_TCPTABLE_OWNER_PID [dwSize];
            if (NULL == pTcpTable) 
            {
                break;
            }
        }

        dwRetVal = GetExtendedTcpTable(pTcpTable, &dwSize, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0);
        if (NO_ERROR != dwRetVal)
        {
            LOG_ERROR("GetExtendedUdpTable failed with %d", dwRetVal);
            break;
        }

		std::map<int, std::string> mapProcInfo;
		if (DTK_OK != CWinApiWrap::Instance()->GetProcInfo(mapProcInfo))
		{
			LOG_ERROR("get localhost process infomation error.");
			break;
		}

		std::map<int, std::string> mapSvrInfo;
		if (DTK_OK != CWinApiWrap::Instance()->GetSvrInfo(mapSvrInfo))
		{
			LOG_ERROR("get localhost service infomation error.");
			break;
		}

        for (int i = 0; i < (int) pTcpTable->dwNumEntries; i++) 
        {
			if (pTcpTable->table[i].dwState != MIB_TCP_STATE_LISTEN /*&& pTcpTable->table[i].dwState != MIB_TCP_STATE_ESTAB*/)
			{
				continue;//只获取Listen和Establish状态的端口
			}

            PORT_DETAIL_ST stPort;
            memset(&stPort, 0, sizeof(PORT_DETAIL_ST));

            stPort.iProtocol = PMT_PORT_PROTO_TCP;
            stPort.iLocalPort = ntohs((u_short)pTcpTable->table[i].dwLocalPort);
            int iPid = pTcpTable->table[i].dwOwningPid;

			//获取占用端口的服务名
			std::map<int, std::string>::iterator it = mapSvrInfo.find(iPid);
			if (it != mapSvrInfo.end())
			{
				DTK_Snprintf(stPort.szSvrName, PMT_SERVICE_NAME_LEN, "%s", it->second.c_str());
			}
			else
			{
				//没有被服务占用，获取占用进程名
				it = mapProcInfo.find(iPid);
				if (it != mapProcInfo.end())
				{
					DTK_Snprintf(stPort.szProcessName, PMT_PROCESS_NAME_LEN, "%s", it->second.c_str());
				}
				else
				{
					LOG_ERROR("why can't find name of this pid:%d", iPid);
				}
			}

			mapTcpPort.insert(std::make_pair(stPort.iLocalPort, stPort));
        }

        iRet = DTK_OK;
    } while (0);
    
    if (NULL != pTcpTable)
    {
        delete []pTcpTable;
        pTcpTable = NULL;
    }
    return iRet;
}

DTK_INT32 CPortDetect::GetUdpPortList(std::map<int, PORT_DETAIL_ST> &mapUdpPort)
{
    DTK_INT32 iRet = DTK_ERROR;
    PMIB_UDPTABLE_OWNER_PID pUdpTable = NULL;

    do 
    {
        DWORD dwSize = sizeof(MIB_UDPTABLE_OWNER_PID);
        pUdpTable = (MIB_UDPTABLE_OWNER_PID *) new (std::nothrow) MIB_UDPTABLE_OWNER_PID [dwSize];
        if (NULL == pUdpTable) 
        {
            break;
        }

        DWORD dwRetVal = GetExtendedUdpTable(pUdpTable, &dwSize, TRUE, AF_INET, UDP_TABLE_OWNER_PID, 0);
        if (ERROR_INSUFFICIENT_BUFFER == dwRetVal) 
        {
            if (NULL != pUdpTable)
            {
                delete []pUdpTable;
                pUdpTable = NULL;
            }

            pUdpTable = (MIB_UDPTABLE_OWNER_PID *) new (std::nothrow) MIB_UDPTABLE_OWNER_PID[dwSize];
            if (pUdpTable == NULL) 
            {
                break;
            }
        }

        dwRetVal = GetExtendedUdpTable(pUdpTable, &dwSize, TRUE, AF_INET, UDP_TABLE_OWNER_PID, 0);
        if (NO_ERROR != dwRetVal)
        {
            LOG_ERROR("GetExtendedUdpTable failed with %d", dwRetVal);
            break;
        }

		std::map<int, std::string> mapProcInfo;
		if (DTK_OK != CWinApiWrap::Instance()->GetProcInfo(mapProcInfo))
		{
			LOG_ERROR("get localhost process infomation error.");
			break;
		}

		std::map<int, std::string> mapSvrInfo;
		if (DTK_OK != CWinApiWrap::Instance()->GetSvrInfo(mapSvrInfo))
		{
			LOG_ERROR("get localhost service infomation error.");
			break;
		}

        for (int i = 0; i < (int) pUdpTable->dwNumEntries; i++) 
        {
            PORT_DETAIL_ST stPort;
            memset(&stPort, 0, sizeof(PORT_DETAIL_ST));

            stPort.iProtocol = PMT_PORT_PROTO_UDP;
            stPort.iLocalPort = ntohs((u_short)pUdpTable->table[i].dwLocalPort);
            int iPid = pUdpTable->table[i].dwOwningPid;

			//获取占用端口的服务名
			std::map<int, std::string>::iterator it = mapSvrInfo.find(iPid);
			if (it != mapSvrInfo.end())
			{
				DTK_Snprintf(stPort.szSvrName, PMT_SERVICE_NAME_LEN, "%s", it->second.c_str());
			}
			else
			{
				//没有被服务占用，获取占用进程名
				it = mapProcInfo.find(iPid);
				if (it != mapProcInfo.end())
				{
					DTK_Snprintf(stPort.szProcessName, PMT_PROCESS_NAME_LEN, "%s", it->second.c_str());
				}
				else
				{
					LOG_ERROR("why can't find name of this pid:%d", iPid);
				}
			}

			mapUdpPort.insert(std::make_pair(stPort.iLocalPort, stPort));
        }

        iRet = DTK_OK;
    } while (0);

    if (NULL != pUdpTable)
    {
        delete []pUdpTable;
        pUdpTable = NULL;
    }

    return iRet;
}
