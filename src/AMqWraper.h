
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

//�����߽ṹ��ָ
class MqProducer 
{
public:
    MqProducer():m_pDestination(NULL),m_pProducer(NULL),m_pSession(NULL),m_nMode(MQ_MODE_TOPIC){}
    Destination* m_pDestination;//������Ŀ��ָ��
    MessageProducer* m_pProducer;//������ָ��
    Session*    m_pSession;//Mq�Ự
    MqMsgMode m_nMode;
    std::string m_strName;//������Mq���ƣ����磺usp.all.user.topic

    int SendMsg(const std::string strMsg);
    void Create(Session* pSession);
    void Destroy();

};

/** @fn MqMsgCb
*  @brief MQ��������Ϣ�ص�������
*  @param [in]pMsg  ��Ϣ����
*  @param [in]uMsgLen ��Ϣ����
*  @param [in]pUser �û�ָ��
*  @return null
*/
typedef void ( CALLBACK* MqMsgCb)(const char* pMsg, unsigned uMsgLen,void* pUser);

//�����߽ṹ��
class MqConsumer:public MessageListener 
{
public:
    MqConsumer():m_pDestination(NULL),m_pConsumer(NULL),m_pSession(NULL),m_nMode(MQ_MODE_TOPIC),m_pMsgCb(NULL){}

    Destination* m_pDestination;//������Ŀ��ָ��
    MessageConsumer* m_pConsumer;//������ָ��
    Session*    m_pSession;//Mq�Ự
    MqMsgMode m_nMode;
    std::string m_strName;//������Mq����
    MqMsgCb m_pMsgCb;//Mq��Ϣ�ص�
    void* m_pUser;//�û�ָ��

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

    bool Conn();//����
    bool DisConn();//�Ͽ�����

    MqProducer* AddProducer(const std::string& strProducerName, MqMsgMode nType);//���������
    bool DelProducer(const std::string& strProducerName, MqMsgMode nType);//ɾ�������ߣ����������Ƿ�Ϊ��

    MqConsumer* AddConsumer(const std::string& strConsumerName, MqMsgMode nType, MqMsgCb pfnMsgCb,void* pUser);//���������
    bool DelConsumer(const std::string& strConsumerName, MqMsgMode nType);//ɾ�������ߣ������Ƿ�����Ϊ��
    bool HasConsumer(const std::string& strConsumerName, MqMsgMode nType);//�ж��������Ƿ����

private:
    void DestroyConn();
    bool IsEmptyConn();
    void onException(const CMSException& ex);

    std::string             m_strBrokerUrl;//Mq����url
    Connection*             m_pConnection;//����
    Session*                m_pSession;//Mq�Ự

    DTK_Mutex               m_lockProCon;  //���������������������߻��������޸ģ�����һ����
    std::list<MqProducer*>  m_listProducer;//�����ϵ������߶���
    std::list<MqConsumer*>  m_listConsumer;//�����ϵ������߶���
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

    ReadWriteLock m_wrLockMqContainer;//ͨ��Mq����ģ��������
    std::map<std::string,MqConnection*> m_mapConn;//<MqIp:Port,Conn>ͨ��ģ��Mq����
    std::map<std::string,MqProducer*> m_mapProcucer;//<ProducerURI,Producer>ͨ��ģ��Mq����������
};

#endif // __AMQWRAPER_H__ 