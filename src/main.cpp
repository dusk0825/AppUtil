
#include <stdio.h>
#include "AMqWraper.h"
#include "WsService.h"
#include "WsClient.h"

void CALLBACK OnMessage(const char* pMsg, unsigned uMsgLen,void* pUser)
{
    std::string strMsg(pMsg, uMsgLen);
    printf("recv = %s\n", strMsg.c_str());
}

void TestMq()
{
    MqMgr mqMgr;
    mqMgr.Init();

    std::string strMqUrl = "mq/10.33.31.190:61618/queue/mq.test.queue";
    mqMgr.AddProducer(strMqUrl);

    mqMgr.SendMsg(strMqUrl, "hello world");
    mqMgr.SendMsg(strMqUrl, "hello world");
    mqMgr.SendMsg(strMqUrl, "hello world");

    getchar();

    mqMgr.AddConsumer(strMqUrl, OnMessage, NULL);

    getchar();

    mqMgr.Fini();
}

void TestWsServ()
{
    CWsService wsSvr;
    wsSvr.Start(9999);

    getchar();

    wsSvr.Stop();
}

void TestWsClient()
{
    CWsClient wsClient;
    wsClient.SetWsAddr("127.0.0.1", 9999);
    wsClient.RemoteConfig();
}

int main(int argc, char* argv[])
{
    DTK_Init();

    //TestMq();
    //TestWsServ();
    //TestWsClient();

    DTK_Fini();
    return 0;
}