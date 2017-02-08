
#include <stdio.h>
#include "AMqWraper.h"

void CALLBACK OnMessage(const char* pMsg, unsigned uMsgLen,void* pUser)
{
    std::string strMsg(pMsg, uMsgLen);
    printf("recv = %s\n", strMsg.c_str());
}

int main(int argc, char* argv[])
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
    return 0;
}