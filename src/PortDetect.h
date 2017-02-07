
#ifndef _PORT_DETECT_H_
#define _PORT_DETECT_H_

#include "dtk/DTK_LogModule.h"
#include "dtk/DTK_Utils.h"
#include <map>

#define PMT_PROCESS_NAME_LEN                            256//�������Ƴ���
#define PMT_SERVICE_NAME_LEN                            128//����������

//�˿�Э������
typedef enum tagPORT_PROTOCOL
{
    PMT_PORT_PROTO_UNKNOWN = 0,
    PMT_PORT_PROTO_TCP,
    PMT_PORT_PROTO_UDP
}PORT_PROTOCOL_E;

//�˿�״̬
typedef enum tagPortStatus
{
    PMT_PORT_STATUS_UNKNOWN = 0,
    PMT_PORT_STATUS_OCP_UNNORMAL,                       //�˿�ռ���쳣
    PMT_PORT_STATUS_OCP_NORMAL,                         //�˿�ռ������
    PMT_PORT_STATUS_MAP_UNNORMAL,                       //�˿�ӳ���쳣
    PMT_PORT_STATUS_MAP_NORMAL                          //�˿�ӳ������
}PORT_STATUS_E;

//�˿�ռ����ϸ��Ϣ
typedef struct tagPortDetailInfo
{
    int iProtocol;                                  //Э��
    unsigned short iLocalPort;                      //���ض˿�
    char szProcessName[PMT_PROCESS_NAME_LEN];
    char szSvrName[PMT_SERVICE_NAME_LEN];           //������
}PORT_DETAIL_ST;

class CPortDetect
{
public:
    CPortDetect();
    ~CPortDetect();

	/**	@fn	DTK_INT32 GetTcpPortList(vector<PORT_DETAIL_ST> &vctTcpPort);
	*	@brief ��ȡ����TCP�˿�ռ����Ϣ
	*	@param [out] vctTcpPort �˿��б�
	*	@return	�ɹ�����0������Ϊ������
	*/
    DTK_INT32 GetTcpPortList(std::map<int, PORT_DETAIL_ST> &mapTcpPort);

	/**	@fn	DTK_INT32 GetUdpPortList(vector<PORT_DETAIL_ST> &vctUdpPort);
	*	@brief ��ȡ����UDP�˿�ռ����Ϣ
	*	@param [out] vctUdpPort �˿��б�
	*	@return	�ɹ�����0������Ϊ������
	*/
    DTK_INT32 GetUdpPortList(std::map<int, PORT_DETAIL_ST> &mapUdpPort);

};

#endif