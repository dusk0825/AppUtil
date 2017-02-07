
#ifndef _FIREWALL_H_
#define _FIREWALL_H_

#include <WinSock2.h>
#include <netfw.h>
#include "dtk/DTK_LogModule.h"

class CFireWallMgr
{
public:
	~CFireWallMgr();
	static CFireWallMgr* Instance(){static CFireWallMgr oSingleObj; return &oSingleObj;}

	int Init();

	void Fini();

	/**	@fn	DTK_INT32 IsFirewallOn(bool bIsOn);
	*	@brief 检查防火墙是否开启
	*	@param [out] bIsOn 打开标志
	*	@return	成功返回0，其他为错误码
	*/
	int IsFirewallOn(bool& bIsOn);
	
	/**	@fn	DTK_INT32 TurnOnFirewall();
	*	@brief 打开防火墙
	*	@param [in]  无
	*	@param [out] 无
	*	@return	成功返回0，其他为错误码
	*/
	int TurnOnFirewall();

	/**	@fn	DTK_INT32 TurnOffFirewall();
	*	@brief 关闭防火墙
	*	@param [in]  无
	*	@param [out] 无
	*	@return	成功返回0，其他为错误码
	*/
	int TurnOffFirewall();

	/**	@fn	DTK_INT32 AddApplication( const wchar_t* lpszProcessImageFileName, const wchar_t* lpszRegisterName );
	*	@brief 添加应用程序入站规则
	*	@param [in]  lpszProcessImageFileName 应用程序路径
	*	@param [out] lpszRegisterName 注册规则名
	*	@return	成功返回0，其他为错误码
	*/
	int AddApplication( const wchar_t* lpszProcessImageFileName, const wchar_t* lpszRegisterName );

	/**	@fn	DTK_INT32 RemoveApplication( const wchar_t* lpszProcessImageFileName );
	*	@brief 删除已加入防火墙的应用程序规则
	*	@param [in]  lpszProcessImageFileName 应用程序路径
	*	@return	成功返回0，其他为错误码
	*/
	int RemoveApplication( const wchar_t* lpszProcessImageFileName );

	/**	@fn	DTK_INT32 IsAppEnabled( const wchar_t* lpszProcessImageFileName, DTK_BOOL& bEnable );
	*	@brief 检查应用程序是否已加入防火墙
	*	@param [in]  lpszProcessImageFileName 应用程序路径
	*	@param [out] bEnable 标志
	*	@return	成功返回0，其他为错误码
	*/
	int IsAppEnabled( const wchar_t* lpszProcessImageFileName, DTK_BOOL& bEnable );

	/**	@fn	DTK_INT32 AddPort( LONG lPortNumber, NET_FW_IP_PROTOCOL ipProtocol, const wchar_t* lpszRegisterName );
	*	@brief 添加端口入站规则
	*	@param [in]  lPortNumber 端口号
	*	@param [in]  ipProtocol 协议
	*	@param [in]  lpszRegisterName 防火墙规则名
	*	@return	成功返回0，其他为错误码
	*/
	int AddPortRule( LONG lPortNumber, NET_FW_IP_PROTOCOL ipProtocol, const wchar_t* lpszRegisterName );

	/**	@fn	DTK_INT32 RemovePort( LONG lPortNumber, NET_FW_IP_PROTOCOL ipProtocol );
	*	@brief 删除已加入防火墙的端口规则
	*	@param [in]  lPortNumber 端口号
	*	@param [in]  ipProtocol 协议
	*	@return	成功返回0，其他为错误码
	*/
	int RemovePortRule( LONG lPortNumber, NET_FW_IP_PROTOCOL ipProtocol );

	/**	@fn	DTK_INT32 IsAppEnabled( const wchar_t* lpszProcessImageFileName, DTK_BOOL& bEnable );
	*	@brief 检查端口是否已加入防火墙
	*	@param [in]  lPortNumber 端口号
	*	@param [in]  ipProtocol 协议
	*	@param [out] bEnable 标志
	*	@return	成功返回0，其他为错误码
	*/
	int IsPortEnabled( LONG lPortNumber, NET_FW_IP_PROTOCOL ipProtocol, BOOL& bEnable );
	
private:
	CFireWallMgr();
	INetFwProfile* m_pFireWallProfile;
};

#endif
