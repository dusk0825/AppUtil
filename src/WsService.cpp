
#include "WsService.h"
#include "ws/stdsoap2.h"
#include "ws/serv/testService.h"
#include "ws/serv/testService.nsmap"

using namespace testService ;

DTK_VOIDPTR CALLBACK SubRecvThread(DTK_VOIDPTR pParam)
{
    CWsService* pThis = static_cast<CWsService*> (pParam);
    if (NULL == pParam)
    {
        return NULL;
    }

    testServiceService* pSvr = new (std::nothrow) testServiceService(SOAP_C_UTFSTRING, SOAP_C_UTFSTRING);
    if (NULL == pSvr)
    {
        LOG_ERROR("new memory fail.");
        return NULL;
    }
    pSvr->accept_timeout = 30;
    pSvr->recv_timeout = 5;
    pSvr->send_timeout = 5;

    bool bBind = false;
    while (!pThis->m_bExit)
    {
        if (!bBind)
        {
            pSvr->bind_flags = SO_REUSEADDR;
            bBind = soap_valid_socket(pSvr->bind(NULL, pThis->m_iPort, 100));
            if (!bBind)
            {
                LOG_DEBUG("service bind fail, retry binding!");
                for (int i = 0; i < 30; i++)
                {
                    DTK_Sleep(1000);
                    if(pThis->m_bExit)
                    {
                        return NULL;
                    }
                }
                continue;
            }
            else
            {
                LOG_INFO("webservice bind port: %d suc.", pThis->m_iPort);
            }
        }
        if ( soap_valid_socket(pSvr->accept()) )
        {
            pSvr->serve();
        }
    }

    if (NULL != pSvr)
    {
        delete pSvr;
        pSvr = NULL;
    }

    return NULL;
}

CWsService::CWsService()
: m_hService(NULL)
, m_iPort(9999)
, m_bExit(false)
{

}

CWsService::~CWsService()
{

}

int CWsService::Start( int iPort )
{
    m_iPort = (iPort > 0) ? iPort : m_iPort;
    m_hService = DTK_Thread_Create(SubRecvThread, (void*)this, 0);
    if(DTK_INVALID_THREAD == m_hService)
    {
        LOG_ERROR("Create WsService Thread Fail.");
        return DTK_ERROR;
    }

    return DTK_OK;
}

void CWsService::Stop()
{
    m_bExit = true;
}

int testServiceService::RemoteConfig(test__ConfigReq req, test__ConfigRsp &rsp)
{
    LOG_INFO("Recv config xml (from %s:%d): \r\n%s\r\n", host, port, req.configXml.c_str());
    return DTK_OK;
}