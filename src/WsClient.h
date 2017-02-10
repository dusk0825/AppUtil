
#ifndef __WSCLIENT_H__  
#define __WSCLIENT_H__  

#include <string>
#include "dtk/DTK_Utils.h"
#include "dtk/DTK_LogModule.h"

class CWsClient
{
public:
    CWsClient();
    ~CWsClient();

    void SetWsAddr(const std::string strIp, int iPort);

    int RemoteConfig();

private:
    std::string m_strWsAddr;
};

#endif // __WSCLIENT_H__ 