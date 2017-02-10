
#include "WsClient.h"
#include "ws/client/testClienttestServiceProxy.h"


CWsClient::CWsClient()
: m_strWsAddr("")
{

}

CWsClient::~CWsClient()
{

}

void CWsClient::SetWsAddr( const std::string strIp, int iPort )
{
    char szUrl[512] = {0};
    DTK_Snprintf(szUrl, sizeof(szUrl), "http://%s:%d/ws/testService", strIp.c_str(), iPort);
    m_strWsAddr = szUrl;
}

int CWsClient::RemoteConfig()
{
    testServiceProxy* pSoapRes = new (std::nothrow) testServiceProxy(m_strWsAddr.c_str(), SOAP_C_UTFSTRING);
    if (pSoapRes == NULL)
    {
        return DTK_ERROR;
    }
    pSoapRes->connect_timeout = 5;
    pSoapRes->recv_timeout = 10;
    pSoapRes->send_timeout = 5;

    do 
    {
        ns1__ConfigReq cfgReq;
        cfgReq.configXml = "just for test";
        _ns1__RemoteConfig getResourcesReq;
        getResourcesReq.req = &cfgReq;
        _ns1__ConfigRsp getResourcesRsp;

        int iRet = pSoapRes->RemoteConfig(&getResourcesReq, &getResourcesRsp);
        if (SOAP_OK != iRet)
        {
            break;
        }
        if (NULL == getResourcesRsp.resultInfo)
        {
            break;
        }

        LOG_DEBUG("retCode = %d, result = %s", getResourcesRsp.resultInfo->resultCode, getResourcesRsp.resultInfo->resultString.c_str());
    } while (0);

    if (NULL != pSoapRes)
    {
        delete pSoapRes;
        pSoapRes = NULL;
    }
    return DTK_OK;
}
