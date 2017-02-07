
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
	*	@brief ������ǽ�Ƿ���
	*	@param [out] bIsOn �򿪱�־
	*	@return	�ɹ�����0������Ϊ������
	*/
	int IsFirewallOn(bool& bIsOn);
	
	/**	@fn	DTK_INT32 TurnOnFirewall();
	*	@brief �򿪷���ǽ
	*	@param [in]  ��
	*	@param [out] ��
	*	@return	�ɹ�����0������Ϊ������
	*/
	int TurnOnFirewall();

	/**	@fn	DTK_INT32 TurnOffFirewall();
	*	@brief �رշ���ǽ
	*	@param [in]  ��
	*	@param [out] ��
	*	@return	�ɹ�����0������Ϊ������
	*/
	int TurnOffFirewall();

	/**	@fn	DTK_INT32 AddApplication( const wchar_t* lpszProcessImageFileName, const wchar_t* lpszRegisterName );
	*	@brief ���Ӧ�ó�����վ����
	*	@param [in]  lpszProcessImageFileName Ӧ�ó���·��
	*	@param [out] lpszRegisterName ע�������
	*	@return	�ɹ�����0������Ϊ������
	*/
	int AddApplication( const wchar_t* lpszProcessImageFileName, const wchar_t* lpszRegisterName );

	/**	@fn	DTK_INT32 RemoveApplication( const wchar_t* lpszProcessImageFileName );
	*	@brief ɾ���Ѽ������ǽ��Ӧ�ó������
	*	@param [in]  lpszProcessImageFileName Ӧ�ó���·��
	*	@return	�ɹ�����0������Ϊ������
	*/
	int RemoveApplication( const wchar_t* lpszProcessImageFileName );

	/**	@fn	DTK_INT32 IsAppEnabled( const wchar_t* lpszProcessImageFileName, DTK_BOOL& bEnable );
	*	@brief ���Ӧ�ó����Ƿ��Ѽ������ǽ
	*	@param [in]  lpszProcessImageFileName Ӧ�ó���·��
	*	@param [out] bEnable ��־
	*	@return	�ɹ�����0������Ϊ������
	*/
	int IsAppEnabled( const wchar_t* lpszProcessImageFileName, DTK_BOOL& bEnable );

	/**	@fn	DTK_INT32 AddPort( LONG lPortNumber, NET_FW_IP_PROTOCOL ipProtocol, const wchar_t* lpszRegisterName );
	*	@brief ��Ӷ˿���վ����
	*	@param [in]  lPortNumber �˿ں�
	*	@param [in]  ipProtocol Э��
	*	@param [in]  lpszRegisterName ����ǽ������
	*	@return	�ɹ�����0������Ϊ������
	*/
	int AddPortRule( LONG lPortNumber, NET_FW_IP_PROTOCOL ipProtocol, const wchar_t* lpszRegisterName );

	/**	@fn	DTK_INT32 RemovePort( LONG lPortNumber, NET_FW_IP_PROTOCOL ipProtocol );
	*	@brief ɾ���Ѽ������ǽ�Ķ˿ڹ���
	*	@param [in]  lPortNumber �˿ں�
	*	@param [in]  ipProtocol Э��
	*	@return	�ɹ�����0������Ϊ������
	*/
	int RemovePortRule( LONG lPortNumber, NET_FW_IP_PROTOCOL ipProtocol );

	/**	@fn	DTK_INT32 IsAppEnabled( const wchar_t* lpszProcessImageFileName, DTK_BOOL& bEnable );
	*	@brief ���˿��Ƿ��Ѽ������ǽ
	*	@param [in]  lPortNumber �˿ں�
	*	@param [in]  ipProtocol Э��
	*	@param [out] bEnable ��־
	*	@return	�ɹ�����0������Ϊ������
	*/
	int IsPortEnabled( LONG lPortNumber, NET_FW_IP_PROTOCOL ipProtocol, BOOL& bEnable );
	
private:
	CFireWallMgr();
	INetFwProfile* m_pFireWallProfile;
};

#endif
