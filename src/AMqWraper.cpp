
#include "AMqWraper.h"

int MqMgr::Init( void )
{
    activemq::library::ActiveMQCPP::initializeLibrary();
    return DTK_OK;
}

void MqMgr::Fini( void )
{
    ReadWriteLock::WriteLock wLock(&m_wrLockMqContainer);
    for (std::map<std::string,MqConnection*>::iterator itr = m_mapConn.begin(); itr != m_mapConn.end(); ++itr)
    {
        itr->second->DisConn();
        delete itr->second;
        itr->second = NULL;
    }

    m_mapProcucer.clear();
    m_mapConn.clear();

    activemq::library::ActiveMQCPP::shutdownLibrary();
}

/** @fn AddProducer
 *  @brief 添加MQ生产者，预留
 *  @param [in]strUrl  MQ  url
 *  @return 非空-生产者指针，空-失败
 */
int MqMgr::AddProducer( const std::string strUrl )
{
    std::string strDst = "";
    std::string strMqSvr = "";
    MqMsgMode nMode = MQ_MODE_QUEUE;
    if (!ParseMqUri(strUrl,strMqSvr,strDst,nMode))
    {
        LOG_ERROR("Parse mq url %s fail",strUrl.c_str());
        return DTK_ERROR;
    }

    ReadWriteLock::WriteLock wLock(&m_wrLockMqContainer);
    if (HasProducer(strUrl))
    {
        return DTK_OK;
    }

    MqConnection* pConn = AddConnection(strMqSvr);
    if (NULL == pConn)
    {
        LOG_ERROR("Add conn %s fail",strMqSvr.c_str());
        return DTK_ERROR;
    }

    MqProducer* pPro = pConn->AddProducer(strDst,nMode);
    if (NULL == pPro)
    {
        LOG_INFO("Add producer %s fail",strUrl.c_str());
        return DTK_ERROR;
    }

    m_mapProcucer.insert(std::map<std::string,MqProducer*>::value_type(strUrl,pPro));
    LOG_INFO("Add producer %s succ",strUrl.c_str());

    return DTK_OK;
}

/** @fn DelProducer
 *  @brief 删除MQ生产者，预留
 *  @param [in]strUrl  MQ  url
 *  @return 成功返回0，否则返回小于0的错误码
 */
int MqMgr::DelProducer( const std::string strUrl )
{
    std::string strDst = "";
    std::string strMqSvr = "";
    MqMsgMode nMode = MQ_MODE_QUEUE;
    if (!ParseMqUri(strUrl,strMqSvr,strDst,nMode))
    {
        LOG_ERROR("Parse mq url fail");
        return DTK_ERROR;
    }

    ReadWriteLock::WriteLock wLock(&m_wrLockMqContainer);
    std::map<std::string,MqConnection*>::iterator itr = m_mapConn.find(strMqSvr);//erase from conn
    if (itr != m_mapConn.end())
    {
        bool bEmptyConn = itr->second->DelProducer(strDst,nMode);
        if (bEmptyConn)//如果连接是没有消费者或者生产者的空连接，删除
        {
            itr->second->DisConn();
            delete itr->second;
            m_mapConn.erase(itr);
        }
    }
    else
    {
        LOG_WARN("producer %d not exist",strUrl.c_str());
    }

    std::map<std::string,MqProducer*>::iterator itrPro = m_mapProcucer.find(strUrl);//erase from cache
    if (itrPro != m_mapProcucer.end())
    {
        m_mapProcucer.erase(itrPro);
    }

    LOG_INFO("delete producer %s succ",strUrl.c_str());
    return DTK_OK;
}

/** @fn SendMsg
 *  @brief 发送消息
 *  @param [in]strUrl  发送目的MQ  url
 *  @param [in]strMsg  消息内容
 *  @return 成功返回0，否则返回小于0的错误码
 */
int MqMgr::SendMsg( const std::string strUrl,const std::string strMsg )
{
    {
        ReadWriteLock::ReadLock rLock(&m_wrLockMqContainer);
        std::map<std::string,MqProducer*>::iterator itr = m_mapProcucer.find(strUrl);
        if (itr != m_mapProcucer.end())
        {
            return itr->second->SendMsg(strMsg);
        }
    }

    {
        LOG_INFO("Send msg dst %s not exist",strUrl.c_str());
        //加锁不能放到此行前面，因为AddProducer需要锁
        if (DTK_OK != AddProducer(strUrl))
        {
            return DTK_ERROR;
        }
        LOG_INFO("Add msg dst %s succ",strUrl.c_str());

        ReadWriteLock::ReadLock rLock(&m_wrLockMqContainer);//发送需要等Mq返回，必须加锁
        return m_mapProcucer[strUrl]->SendMsg(strMsg);
    }
}

int MqMgr::AddConsumer( const std::string strUrl, MqMsgCb pfnMsgCb, void* pUser )
{
    if (NULL == pfnMsgCb)
    {
        LOG_ERROR("Null mq message callback");
        return DTK_ERROR;
    }

    std::string strDst = "";
    std::string strMqSvr = "";
    MqMsgMode nMode = MQ_MODE_QUEUE;
    if (!ParseMqUri(strUrl,strMqSvr,strDst,nMode))
    {
        LOG_ERROR("Parse mq url fail");
        return DTK_ERROR;
    }

    ReadWriteLock::WriteLock wLock(&m_wrLockMqContainer);
    if (HasConsumer(strMqSvr,strDst,nMode))
    {
        LOG_WARN("Consumer %s already exist",strUrl.c_str());
        return DTK_OK;
    }


    MqConnection* pConn = AddConnection(strMqSvr);
    if (pConn && pConn->AddConsumer(strDst,nMode,pfnMsgCb,pUser))
    {
        LOG_INFO("Add consumer %s succ",strUrl.c_str());
        return DTK_OK;
    }

    LOG_ERROR("Add consumer %s fail",strUrl.c_str());
    return DTK_ERROR;
}

int MqMgr::DelConsumer( const std::string strUrl )
{
    std::string strDst = "";
    std::string strMqSvr = "";
    MqMsgMode nMode = MQ_MODE_QUEUE;
    if (!ParseMqUri(strUrl,strMqSvr,strDst,nMode))
    {
        LOG_ERROR("Parse mq url %s fail",strUrl.c_str());
        return DTK_ERROR;
    }

    ReadWriteLock::WriteLock wLock(&m_wrLockMqContainer);
    std::map<std::string,MqConnection*>::iterator itr = m_mapConn.find(strMqSvr);
    if (itr != m_mapConn.end())
    {
        bool bEmptyConn = itr->second->DelConsumer(strDst,nMode);
        if (bEmptyConn)//如果连接是没有消费者或者生产者的空连接，删除
        {
            itr->second->DisConn();
            delete itr->second;
            m_mapConn.erase(itr);
        }
    }
    else
    {
        LOG_WARN("consumer %d not exist",strUrl.c_str());
    }

    LOG_INFO("delete consumer %s succ",strUrl.c_str());
    return DTK_OK;
}

/** @fn ParseMqUri
 *  @brief 解析Mq uri
 *  @param [in]strMqUri   Mq uri，绝对路径，格式形如mq/127.0.0.1:61618/queue/acs.event.queue
 *  @param [in]strMQSvr   Mq 服务地址，格式127.0.0.1:80
 *  @param [in]strDstName Queue或者Queue名称
 *  @param [in]nType      Mq连接类型
 *  @return true-成功，false-失败
 */
bool MqMgr::ParseMqUri( const std::string& strMqUri, std::string& strMQSvr, std::string& strDstName, MqMsgMode& nType )
{
    do 
    {
        int nCurr = 0;
        int nIndex = strMqUri.find("mq/");
        if (nIndex == strMqUri.npos)
        {
            break;
        }
        nCurr = nIndex + 3;

        nIndex = strMqUri.find('/',nCurr);//mq server
        if (nIndex == strMqUri.npos)
        {
            LOG_ERROR("Parse mq uri %s fail, no mq server",strMqUri.c_str());
            break;
        }
        strMQSvr = strMqUri.substr(nCurr,nIndex - nCurr);
        nCurr = nIndex + 1;

        nIndex = strMqUri.find('/',nCurr);//queue or topic
        if (nIndex == strMqUri.npos)
        {
            LOG_ERROR("Parse mq uri %s fail, no mq message mode",strMqUri.c_str());
            break;
        }
        std::string strType = strMqUri.substr(nCurr, nIndex - nCurr);
        if (strType == "queue")
        {
            nType = MQ_MODE_QUEUE;
        }
        else if(strType == "topic")
        {
            nType = MQ_MODE_TOPIC;
        }
        else
        {
            LOG_ERROR("Unkown mq mode");
            break;
        }
        nCurr = nIndex + 1;

        strDstName = strMqUri.substr(nCurr);
        if (strDstName.empty())
        {
            LOG_ERROR("Parse mq uri %s fail, no mq dst",strMqUri.c_str());
            break;
        }

        return true;
    } while (false);

    return false;
}

/** @fn HasProducer
 *  @brief 查找生产者是否存在
 *  @param [in]strMqUri   Mq uri，绝对路径，格式形如mq/127.0.0.1:61618/queue/acs.event.queue
 *  @return true-是，false-否
 */
bool MqMgr::HasProducer(const std::string& strMqUri)
{
    std::map<std::string,MqProducer*>::iterator itr = m_mapProcucer.find(strMqUri);
    return (itr != m_mapProcucer.end());
}

/** @fn HasConsumer
 *  @brief 查找消费者是否存在
 *  @param [in]strMQSvr   Mq 服务地址，格式127.0.0.1:80
 *  @param [in]strDstName Queue或者Queue名称
 *  @param [in]nType      Mq连接类型
 *  @return true-是，false-否
 */
bool MqMgr::HasConsumer(const std::string& strMQSvr, std::string strDstName, MqMsgMode nType)
{
    std::map<std::string,MqConnection*>::iterator itrCon = m_mapConn.find(strMQSvr);
    if (itrCon != m_mapConn.end())
    {
        return itrCon->second->HasConsumer(strDstName,nType);
    }

    return false;
}

/** @fn AddConnection
 *  @brief 添加连接
 *  @param [in]strMQSvr   Mq 服务地址，格式127.0.0.1:80
 *  @return true-是，false-否
 */
MqConnection* MqMgr::AddConnection(const std::string& strMQSvr)
{
    MqConnection* pConn = NULL;
    std::map<std::string,MqConnection*>::iterator itr = m_mapConn.find(strMQSvr);
    if (itr != m_mapConn.end())
    {
        pConn = itr->second;
    }
    else
    {
        pConn = new(std::nothrow) MqConnection(strMQSvr);
        if (pConn)
        {
            if (pConn->Conn())
            {
                LOG_INFO("Add connection %s",strMQSvr.c_str());
                m_mapConn.insert(std::map<std::string,MqConnection*>::value_type(strMQSvr,pConn));
            }
            else
            {
                pConn->DisConn();
                delete pConn;
            }
        }
        else
        {
            LOG_ERROR("New memory fail");
        }
    }
    
    return pConn;
}

MqConnection::MqConnection( const std::string& strBrokerUrl )
: m_strBrokerUrl("")
, m_pConnection(NULL)
, m_pSession(NULL)
{
    char szURI[256] = {0};
    DTK_Snprintf(szURI,sizeof(szURI),"tcp://%s?connectionTimeout=5000&soTimeout=10000&maxReconnectAttempts=1"
        "&wireFormat.maxInactivityDuration=0",strBrokerUrl.c_str());
    m_strBrokerUrl = szURI;
}

MqConnection::~MqConnection()
{

}

bool MqConnection::Conn()
{
    bool bRet = true;
    try
    {
        std::auto_ptr<ConnectionFactory> connectionFactory(ConnectionFactory::createCMSConnectionFactory(m_strBrokerUrl));
        m_pConnection = connectionFactory->createConnection();
        m_pConnection->setExceptionListener(this);
        m_pConnection->start();
        m_pSession = m_pConnection->createSession( Session::AUTO_ACKNOWLEDGE );
        LOG_INFO("conn mq succ %s", m_strBrokerUrl.c_str());
    }
    catch(...)
    {
        LOG_ERROR("conn mq fail %s", m_strBrokerUrl.c_str());
        bRet = false;
    }

    return bRet;
}

/** @fn    DisConn
 *  @brief 断开Mq连接，销毁生产者、消费者
 *  @param void
 *  @return 
*/
bool MqConnection::DisConn()
{
    DestroyConn();

    DTK_Guard  guard(&m_lockProCon);
    for (std::list<MqProducer*>::iterator itr = m_listProducer.begin(); itr != m_listProducer.end(); ++itr)
    {
        delete (*itr);
    }
    m_listProducer.clear();

    for (std::list<MqConsumer*>::iterator itr = m_listConsumer.begin(); itr != m_listConsumer.end(); ++itr)
    {
        delete (*itr);
    }
    m_listConsumer.clear();

    return true;
}

/** @fn    DestroyConn
 *  @brief 销毁Mq生产者、消费者，断开Mq连接
 *  @param  void
 *  @return 
*/
void MqConnection::DestroyConn()
{
    DTK_Guard  guard(&m_lockProCon);
    for (std::list<MqProducer*>::iterator itr = m_listProducer.begin(); itr != m_listProducer.end(); ++itr)
    {
        (*itr)->Destroy();
    }

    for (std::list<MqConsumer*>::iterator itr = m_listConsumer.begin(); itr != m_listConsumer.end(); ++itr)
    {
        (*itr)->Destroy();
    }

    try
    {
        if( m_pSession != NULL )
        {
            delete m_pSession;
        }
    }
    catch ( CMSException& e ) 
    { 
        LOG_ERROR("%s",e.getMessage().c_str()); 
    }
    catch ( ... )
    {
        LOG_ERROR("Exception");
    }
    m_pSession = NULL;

    try
    {

        if( m_pConnection != NULL )
        {
            delete m_pConnection;
        }
    }
    catch ( CMSException& e ) 
    { 
        LOG_ERROR("%s",e.getMessage().c_str()); 
    }
    catch ( ... )
    {
        LOG_ERROR("Exception");
    }
    m_pConnection = NULL;
}

/** @fn    AddProducer
 *  @brief 添加生产者
 *  @param [in]strProducerName 生产者名称 
 *  @param [in]nType 类型@MqMsgMode
 *  @return 生产者指针
*/
MqProducer* MqConnection::AddProducer(const std::string& strProducerName, MqMsgMode nType)
{
    MqProducer* pPro = new(std::nothrow) MqProducer;
    if (pPro)
    {
        pPro->m_strName = strProducerName;
        pPro->m_nMode = nType;
        {
            DTK_Guard guard(&m_lockProCon);
            m_listProducer.push_back(pPro);
        }
        pPro->Create(m_pSession);
    }

    return pPro;
}

/** @fn    DelProducer
 *  @brief 删除生产者
 *  @param [in]strProducerName 生产者名称 
 *  @param [in]nType 类型@MqMsgMode
 *  @return 删除后当前连接是否为空true-是，false-否
*/
bool MqConnection::DelProducer(const std::string& strProducerName, MqMsgMode nType)
{
    DTK_Guard guard(&m_lockProCon);
    for (std::list<MqProducer*>::iterator itr = m_listProducer.begin(); itr != m_listProducer.end(); ++itr)
    {
        if ((*itr)->m_strName == strProducerName && (*itr)->m_nMode == nType)
        {
            (*itr)->Destroy();
            delete (*itr);
            m_listProducer.erase(itr);
            break;
        }
    }
    return IsEmptyConn();
}

/** @fn    IsEmptyConn
 *  @brief 连接是否为空
 *  @param void
 *  @return true-是，false-否
*/
bool MqConnection::IsEmptyConn()
{
    return (m_listConsumer.empty() && m_listProducer.empty());
}

/** @fn    AddConsumer
 *  @brief 添加消费者
 *  @param [in]strConsumerName 消费者名称 
 *  @param [in]nType 类型@MqMsgMode
 *  @param [in]pfnMsgCb 类型@MqMsgMode
 *  @param [in]pUser 用户指针
 *  @return 消费者指针
*/
MqConsumer* MqConnection::AddConsumer(const std::string& strConsumerName, MqMsgMode nType, MqMsgCb pfnMsgCb,void* pUser)
{
    MqConsumer* pConsumer = new(std::nothrow) MqConsumer;
    if (pConsumer)
    {
        pConsumer->m_strName = strConsumerName;
        pConsumer->m_nMode = nType;
        pConsumer->m_pMsgCb = pfnMsgCb;
        pConsumer->m_pUser = pUser;
        {
            DTK_Guard guard(&m_lockProCon);
            m_listConsumer.push_back(pConsumer);
        }
        pConsumer->Create(m_pSession);
    }

    return pConsumer;
}

/** @fn    DelConsumer
 *  @brief 删除消费者
 *  @param [in]strConsumerName 消费者名称 
 *  @param [in]nType 类型@MqMsgMode
 *  @return 删除后当前连接是否为空true-是，false-否
*/
bool MqConnection::DelConsumer(const std::string& strConsumerName, MqMsgMode nType)
{
    DTK_Guard guard(&m_lockProCon);
    for (std::list<MqConsumer*>::iterator itr = m_listConsumer.begin(); itr != m_listConsumer.end(); ++itr)
    {
        if ((*itr)->m_strName == strConsumerName && (*itr)->m_nMode == nType)
        {
            (*itr)->Destroy();
            delete (*itr);
            m_listConsumer.erase(itr);
            break;
        }
    }
    return IsEmptyConn();
}

/** @fn    HasConsumer
 *  @brief 判断消费者是否存在
 *  @param [in]strConsumerName 消费者名称 
 *  @param [in]nType 类型@MqMsgMode
 *  @return true-是，false-否
*/
bool MqConnection::HasConsumer(const std::string& strConsumerName, MqMsgMode nType)
{
    DTK_Guard guard(&m_lockProCon);
    for (std::list<MqConsumer*>::iterator itr = m_listConsumer.begin(); itr != m_listConsumer.end(); ++itr)
    {
        if ((*itr)->m_strName == strConsumerName && (*itr)->m_nMode == nType)
        {
            return true;
        }
    }

    return false;
}

/** @fn    onException
 *  @brief Mq连接异常回调
 *  @param [in]ex 异常消息 
 *  @return
*/
void MqConnection::onException(const CMSException& ex)
{
    LOG_ERROR("Mq connection exception, errmsg %s",ex.getMessage().c_str());

    DisConn();
}




template<typename T,typename Y>
void ReleaseDst(T& pdst,Y& pCp)
{
    try
    {
        if( pdst ) 
        {
            delete pdst;
        }
    }
    catch ( CMSException& e ) 
    { 
        LOG_ERROR("%s",e.getMessage().c_str());
        pdst = NULL;
    }
    catch(...)
    {
        LOG_ERROR("Exception");
        pdst = NULL;
    }
    pdst = NULL;

    try
    {
        if( pCp ) 
        {
            pCp->close();
            {
                delete pCp;
            }
        }
    }
    catch ( CMSException& e ) 
    { 
        LOG_ERROR("%s",e.getMessage().c_str());
        pCp = NULL;
    }
    catch(...)
    {
        LOG_ERROR("Exception");
        pCp = NULL;
    }
    pCp = NULL;
}


/** @fn    Create
 *  @brief 生产者创建
 *  @param [in]pSession Mq连接会话
 *  @return 
*/
void MqProducer::Create(Session* pSession)
{
    if (!pSession)
    {
        return;
    }

    m_pSession = pSession;
    try
    {
        if (m_nMode == MQ_MODE_QUEUE)
        {
            m_pDestination = m_pSession->createQueue( m_strName.c_str() );
        }
        else
        {
            m_pDestination = m_pSession->createTopic( m_strName.c_str() );
        }

        m_pProducer = m_pSession->createProducer( m_pDestination );
        m_pProducer->setDeliveryMode( DeliveryMode::NON_PERSISTENT );
    }
    catch ( CMSException& e ) 
    { 
        LOG_ERROR("%s",e.getMessage().c_str()); 
    }
    catch ( ... )
    {
        LOG_ERROR("Exception");
    }
}

/** @fn    Destroy
 *  @brief 生产者销毁
 *  @param NULL
 *  @return 
*/
void MqProducer::Destroy()
{
    ReleaseDst(m_pDestination,m_pProducer);
    m_pSession = NULL;
}

/** @fn    SendMsg
 *  @brief 发送消息
 *  @param [in]消息内容
 *  @return 成功返回0，否则返回小于0的错误码
*/
int MqProducer::SendMsg(const std::string strMsg)
{
    int bRet = DTK_ERROR;
    BytesMessage* msg = NULL;
    try 
    {
        if (m_pSession)
        {
            msg = m_pSession->createBytesMessage((const unsigned char*)strMsg.c_str(),strMsg.size());
            if (m_pProducer)
            {
                m_pProducer->send(msg);
                bRet = DTK_OK;
            }
        }
    }
    catch ( ... ) 
    {
        LOG_TRACE("Send msg to mq fail");
    }

    if (msg)
    {
        delete msg;
        msg = NULL;
    }
    return bRet;
}




/** @fn    Create
 *  @brief 消费者创建
 *  @param [in]pSession Mq连接会话
 *  @return 
*/
void MqConsumer::Create(Session* pSession)
{
    if (!pSession)
    {
        return;
    }

    m_pSession = pSession;
    try
    {
        if (m_nMode == MQ_MODE_QUEUE)
        {
            m_pDestination = m_pSession->createQueue( m_strName.c_str() );
        }
        else
        {
            m_pDestination = m_pSession->createTopic( m_strName.c_str() );
        }

        m_pConsumer = m_pSession->createConsumer( m_pDestination );
        m_pConsumer->setMessageListener( this );
    }
    catch ( CMSException& e ) 
    { 
        LOG_ERROR("%s",e.getMessage().c_str()); 
    }
    catch ( ... )
    {
        LOG_ERROR("Exception");
    }
}

/** @fn    Destroy
 *  @brief 消费者销毁
 *  @param NULL
 *  @return 
*/
void MqConsumer::Destroy()
{
    ReleaseDst(m_pDestination,m_pConsumer);
    m_pSession = NULL;
}

/** @fn    onMessage
 *  @brief Mq消息回调函数
 *  @param [in]message 回调的消息
 *  @return 
*/
void MqConsumer::onMessage( const cms::Message* message )
{
    try
    {
        const cms::BytesMessage* msg = dynamic_cast< const cms::BytesMessage* >(message);
        if (msg == NULL)
        {
            return;
        }
        char* pBuf = (char*)msg->getBodyBytes();
        if (m_pMsgCb)
        {
            m_pMsgCb(pBuf,msg->getBodyLength(), m_pUser);
        }
        delete[] pBuf;

    } 
    catch (cms::CMSException& e) 
    {
        LOG_ERROR("%s",e.getMessage().c_str());
    }
    catch ( ... )
    {
        LOG_ERROR("Exception");
    }
}
