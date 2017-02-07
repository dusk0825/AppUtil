
#ifndef _PORT_DETECT_H_
#define _PORT_DETECT_H_

#include "dtk/DTK_LogModule.h"
#include "dtk/DTK_Utils.h"
#include <map>

#define PMT_PROCESS_NAME_LEN                            256//进程名称长度
#define PMT_SERVICE_NAME_LEN                            128//服务名长度

//端口协议类型
typedef enum tagPORT_PROTOCOL
{
    PMT_PORT_PROTO_UNKNOWN = 0,
    PMT_PORT_PROTO_TCP,
    PMT_PORT_PROTO_UDP
}PORT_PROTOCOL_E;

//端口状态
typedef enum tagPortStatus
{
    PMT_PORT_STATUS_UNKNOWN = 0,
    PMT_PORT_STATUS_OCP_UNNORMAL,                       //端口占用异常
    PMT_PORT_STATUS_OCP_NORMAL,                         //端口占用正常
    PMT_PORT_STATUS_MAP_UNNORMAL,                       //端口映射异常
    PMT_PORT_STATUS_MAP_NORMAL                          //端口映射正常
}PORT_STATUS_E;

//端口占用详细信息
typedef struct tagPortDetailInfo
{
    int iProtocol;                                  //协议
    unsigned short iLocalPort;                      //本地端口
    char szProcessName[PMT_PROCESS_NAME_LEN];
    char szSvrName[PMT_SERVICE_NAME_LEN];           //进程名
}PORT_DETAIL_ST;

class CPortDetect
{
public:
    CPortDetect();
    ~CPortDetect();

	/**	@fn	DTK_INT32 GetTcpPortList(vector<PORT_DETAIL_ST> &vctTcpPort);
	*	@brief 获取本机TCP端口占用信息
	*	@param [out] vctTcpPort 端口列表
	*	@return	成功返回0，其他为错误码
	*/
    DTK_INT32 GetTcpPortList(std::map<int, PORT_DETAIL_ST> &mapTcpPort);

	/**	@fn	DTK_INT32 GetUdpPortList(vector<PORT_DETAIL_ST> &vctUdpPort);
	*	@brief 获取本机UDP端口占用信息
	*	@param [out] vctUdpPort 端口列表
	*	@return	成功返回0，其他为错误码
	*/
    DTK_INT32 GetUdpPortList(std::map<int, PORT_DETAIL_ST> &mapUdpPort);

};

#endif