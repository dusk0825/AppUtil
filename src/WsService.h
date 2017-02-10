
#ifndef __WSSERVICE_H__  
#define __WSSERVICE_H__  

#include "dtk/DTK_LogModule.h"
#include "dtk/DTK_Thread.h"
#include "dtk/DTK_Utils.h"

class CWsService
{
public:
    CWsService();
    ~CWsService();

    int Start(int iPort);
    void Stop();

public:
    int         m_iPort;
    bool        m_bExit;
    DTK_HANDLE  m_hService;
};


#endif // __WSSERVICE_H__ 
