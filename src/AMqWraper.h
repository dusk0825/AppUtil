
#ifndef __AMQWRAPER_H__  
#define __AMQWRAPER_H__  

#include <map>
#include <list>
#include <string>

#include "dtk/DTK_LogModule.h"
#include "dtk/DTK_RWLock.h"
#include "dtk/DTK_Guard.h"
#include "dtk/DTK_Utils.h"

#include <activemq/library/ActiveMQCPP.h>
#include <decaf/lang/Thread.h>
#include <decaf/lang/Runnable.h>
#include <decaf/util/concurrent/CountDownLatch.h>
#include <decaf/lang/Integer.h>
#include <decaf/util/Date.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/util/Config.h>
#include <cms/Connection.h>
#include <cms/Session.h>
#include <cms/TextMessage.h>
#include <cms/BytesMessage.h>
#include <cms/MapMessage.h>
#include <cms/ExceptionListener.h>
#include <cms/MessageListener.h>
#include "activemq/commands/ActiveMQTempQueue.h"
#include "activemq/commands/ActiveMQTempTopic.h"


using namespace activemq::commands;
using namespace activemq::core;
using namespace decaf::util::concurrent;
using namespace decaf::util;
using namespace decaf::lang;
using namespace cms;

enum MqMsgMode
{
    MQ_MODE_TOPIC,/*Topic*/
    MQ_MODE_QUEUE/*Queue*/
};

//生产者结构体指
class MqProducer 
{
public:
    MqProducer():m_pDestination(NULL),m_pProducer(NULL),m_pSession(NULL),m_nMode(MQ_MODE_TOPIC){}
    Destination* m_pDestination;//生产者目的指针
    MessageProducer* m_pProducer;//生产者指针
    Session*    m_pSession;//Mq会话
    MqMsgMode m_nMode;
    std::string m_strName;//生产者Mq名称，例如：usp.all.user.topic

    int SendMsg(const std::string strMsg);
    void Create(Session* pSession);
    void Destroy();

};

/** @fn MqMsgCb
*  @brief MQ消费者消息回调函数。
*  @param [in]pMsg  消息内容
*  @param [in]uMsgLen 消息长度
*  @param [in]pUser 用户指针
*  @return null
*/
typedef void ( CALLBACK* MqMsgCb)(const char* pMsg, unsigned uMsgLen,void* pUser);

//消费者结构体
class MqConsumer:public MessageListener 
{
public:
    MqConsumer():m_pDestination(NULL),m_pConsumer(NULL),m_pSession(NULL),m_nMode(MQ_MODE_TOPIC),m_pMsgCb(NULL){}

    Destination* m_pDestination;//消费者目的指针
    MessageConsumer* m_pConsumer;//消费者指针
    Session*    m_pSession;//Mq会话
    MqMsgMode m_nMode;
    std::string m_strName;//消费者Mq名称
    MqMsgCb m_pMsgCb;//Mq消息回调
    void* m_pUser;//用户指针

    void Create(Session* pSession);
    void Destroy();
    void onMessage( const cms::Message* message );

};

class MqConnection 
    : public ExceptionListener
{
public:
    MqConnection(const std::string& strBrokerUrl);
    ~MqConnection();

    bool Conn();//连接
    bool DisConn();//断开连接

    MqProducer* AddProducer(const std::string& strProducerName, MqMsgMode nType);//添加生产者
    bool DelProducer(const std::string& strProducerName, MqMsgMode nType);//删除生产者，返回连接是否为空

    MqConsumer* AddConsumer(const std::string& strConsumerName, MqMsgMode nType, MqMsgCb pfnMsgCb,void* pUser);//添加消费者
    bool DelConsumer(const std::string& strConsumerName, MqMsgMode nType);//删除消费者，返回是否连接为空
    bool HasConsumer(const std::string& strConsumerName, MqMsgMode nType);//判断消费者是否存在

private:
    void DestroyConn();
    bool IsEmptyConn();
    void onException(const CMSException& ex);

    std::string             m_strBrokerUrl;//Mq连接url
    Connection*             m_pConnection;//连接
    Session*                m_pSession;//Mq会话

    DTK_Mutex               m_lockProCon;  //队列锁，生产者与消费者基本不会修改，共用一个锁
    std::list<MqProducer*>  m_listProducer;//连接上的生产者队列
    std::list<MqConsumer*>  m_listConsumer;//连接上的消费者队列
};

class MqMgr
{
public:
    MqMgr(){}
    ~MqMgr(){}

    int Init(void);

    void Fini(void);

    int AddProducer(const std::string strUrl);

    int DelProducer(const std::string strUrl);

    int SendMsg(const std::string strUrl,const std::string strMsg);

    int AddConsumer(const std::string strUrl, MqMsgCb pfnMsgCb, void* pUser);

    int DelConsumer(const std::string strUrl);

private:
    bool ParseMqUri(const std::string& strMqUri, std::string& strMQSvr, std::string& strDstName, MqMsgMode& nType);

    bool HasProducer(const std::string& strMqUri);

    MqConnection* AddConnection(const std::string& strMQSvr);

    bool HasConsumer(const std::string& strMQSvr, std::string strDstName, MqMsgMode nType);

    ReadWriteLock m_wrLockMqContainer;//通信Mq管理模块容器锁
    std::map<std::string,MqConnection*> m_mapConn;//<MqIp:Port,Conn>通信模块Mq连接
    std::map<std::string,MqProducer*> m_mapProcucer;//<ProducerURI,Producer>通信模块Mq所有生产者
};

#endif // __AMQWRAPER_H__ 