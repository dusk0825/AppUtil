
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
 *  @brief ���MQ�����ߣ�Ԥ��
 *  @param [in]strUrl  MQ  url
 *  @return �ǿ�-������ָ�룬��-ʧ��
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
 *  @brief ɾ��MQ�����ߣ�Ԥ��
 *  @param [in]strUrl  MQ  url
 *  @return �ɹ�����0�����򷵻�С��0�Ĵ�����
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
        if (bEmptyConn)//���������û�������߻��������ߵĿ����ӣ�ɾ��
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
 *  @brief ������Ϣ
 *  @param [in]strUrl  ����Ŀ��MQ  url
 *  @param [in]strMsg  ��Ϣ����
 *  @return �ɹ�����0�����򷵻�С��0�Ĵ�����
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
        //�������ܷŵ�����ǰ�棬��ΪAddProducer��Ҫ��
        if (DTK_OK != AddProducer(strUrl))
        {
            return DTK_ERROR;
        }
        LOG_INFO("Add msg dst %s succ",strUrl.c_str());

        ReadWriteLock::ReadLock rLock(&m_wrLockMqContainer);//������Ҫ��Mq���أ��������
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
        if (bEmptyConn)//���������û�������߻��������ߵĿ����ӣ�ɾ��
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
 *  @brief ����Mq uri
 *  @param [in]strMqUri   Mq uri������·������ʽ����mq/127.0.0.1:61618/queue/acs.event.queue
 *  @param [in]strMQSvr   Mq �����ַ����ʽ127.0.0.1:80
 *  @param [in]strDstName Queue����Queue����
 *  @param [in]nType      Mq��������
 *  @return true-�ɹ���false-ʧ��
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
 *  @brief �����������Ƿ����
 *  @param [in]strMqUri   Mq uri������·������ʽ����mq/127.0.0.1:61618/queue/acs.event.queue
 *  @return true-�ǣ�false-��
 */
bool MqMgr::HasProducer(const std::string& strMqUri)
{
    std::map<std::string,MqProducer*>::iterator itr = m_mapProcucer.find(strMqUri);
    return (itr != m_mapProcucer.end());
}

/** @fn HasConsumer
 *  @brief �����������Ƿ����
 *  @param [in]strMQSvr   Mq �����ַ����ʽ127.0.0.1:80
 *  @param [in]strDstName Queue����Queue����
 *  @param [in]nType      Mq��������
 *  @return true-�ǣ�false-��
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
 *  @brief �������
 *  @param [in]strMQSvr   Mq �����ַ����ʽ127.0.0.1:80
 *  @return true-�ǣ�false-��
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
 *  @brief �Ͽ�Mq���ӣ����������ߡ�������
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
 *  @brief ����Mq�����ߡ������ߣ��Ͽ�Mq����
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
 *  @brief ���������
 *  @param [in]strProducerName ���������� 
 *  @param [in]nType ����@MqMsgMode
 *  @return ������ָ��
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
 *  @brief ɾ��������
 *  @param [in]strProducerName ���������� 
 *  @param [in]nType ����@MqMsgMode
 *  @return ɾ����ǰ�����Ƿ�Ϊ��true-�ǣ�false-��
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
 *  @brief �����Ƿ�Ϊ��
 *  @param void
 *  @return true-�ǣ�false-��
*/
bool MqConnection::IsEmptyConn()
{
    return (m_listConsumer.empty() && m_listProducer.empty());
}

/** @fn    AddConsumer
 *  @brief ���������
 *  @param [in]strConsumerName ���������� 
 *  @param [in]nType ����@MqMsgMode
 *  @param [in]pfnMsgCb ����@MqMsgMode
 *  @param [in]pUser �û�ָ��
 *  @return ������ָ��
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
 *  @brief ɾ��������
 *  @param [in]strConsumerName ���������� 
 *  @param [in]nType ����@MqMsgMode
 *  @return ɾ����ǰ�����Ƿ�Ϊ��true-�ǣ�false-��
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
 *  @brief �ж��������Ƿ����
 *  @param [in]strConsumerName ���������� 
 *  @param [in]nType ����@MqMsgMode
 *  @return true-�ǣ�false-��
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
 *  @brief Mq�����쳣�ص�
 *  @param [in]ex �쳣��Ϣ 
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
 *  @brief �����ߴ���
 *  @param [in]pSession Mq���ӻỰ
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
 *  @brief ����������
 *  @param NULL
 *  @return 
*/
void MqProducer::Destroy()
{
    ReleaseDst(m_pDestination,m_pProducer);
    m_pSession = NULL;
}

/** @fn    SendMsg
 *  @brief ������Ϣ
 *  @param [in]��Ϣ����
 *  @return �ɹ�����0�����򷵻�С��0�Ĵ�����
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
 *  @brief �����ߴ���
 *  @param [in]pSession Mq���ӻỰ
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
 *  @brief ����������
 *  @param NULL
 *  @return 
*/
void MqConsumer::Destroy()
{
    ReleaseDst(m_pDestination,m_pConsumer);
    m_pSession = NULL;
}

/** @fn    onMessage
 *  @brief Mq��Ϣ�ص�����
 *  @param [in]message �ص�����Ϣ
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
