
#ifndef _WIN_API_WARP_H_
#define _WIN_API_WARP_H_

#include "dtk/DTK_LogModule.h"
#include <map>
#include <vector>
#include <string>

typedef struct SvrInfo
{
	std::string strSvrName;
	int iState;
}SVR_INFO_ST;

typedef enum
{
	SVR_STATUS_UNKNOW = 0,
	SVR_STATUS_RUN,
	SVR_STATUS_STOP
}SVR_STATUS_E;

class CWinApiWrap
{
public:
	~CWinApiWrap();
	static CWinApiWrap* Instance(){static CWinApiWrap oSingleObj; return &oSingleObj;}

	int Init();

	void Fini();

	/**	@fn	int GetProcInfo(map<int, string>& mapProcInfo);
	*	@brief ��ȡ�������н�����Ϣ
	*	@param [in]  ��
	*	@param [out] mapProcInfo <����ID��������>
	*	@return	�ɹ�����0������Ϊ������
	*/
	int GetProcInfo(std::map<int, std::string>& mapProcInfo);

	/**	@fn	int GetProcInfo(map<int, string>& mapProcInfo);
	*	@brief ��ȡ�������з�����Ϣ
	*	@param [in]  ��
	*	@param [out] mapProcInfo <����ID��������>
	*	@return	�ɹ�����0������Ϊ������
	*/
	int GetSvrInfo(std::map<int, std::string>& mapSvrInfo);

	/**	@fn	int GetProcPathByPid(DWORD dwProcessID, char szProcPath[]);
	*	@brief ͨ��PID���ҽ���·��
	*	@param [in]  dwProcessID	����PID
	*	@param [out] szProcPath		����·��
	*	@return	�ɹ�����0������Ϊ������
	*/
	int GetProcPathByPid(DWORD dwProcessID, char szProcPath[]);

	/**	@fn	int GetPidByAppName(string strAppName, int& iPid);
	*	@brief ͨ��Ӧ�ó���������PID
	*	@param [in]  strAppName	Ӧ�ó�����
	*	@param [out] iPid		����PID
	*	@return	�ɹ�����0������Ϊ������
	*/
	int GetPidByAppName(std::string strAppName, int& iPid);

	/**	@fn	int GetAppSetupPath(string& strAppName, string& strPath);
	*	@brief ��ȡӦ�ó���װ·��
	*	@param [in] strAppName	Ӧ�ó�����
	*	@param [out] strPath	��װ·��
	*	@return	�ɹ�����0������Ϊ������
	*/
	int GetAppSetupPath(std::string strAppName, std::string& strPath);

	/**	@fn	int GetSetupPathBySvrName(string strSvrName, string& strPath)
	*	@brief ���ݷ�������ȡӦ�ó���װ·��
	*	@param [in] strAppName	������
	*	@param [out] strPath	��װ·��
	*	@return	�ɹ�����0������Ϊ������
	*/
	int GetSetupPathBySvrName(std::string strSvrName, std::string& strPath);

	/**	@fn	int GetLocalIp(string& strIp);
	*	@brief ��ȡ����IP��ַ
	*	@param [out] strIp		IP��ַ
	*	@return	�ɹ�����0������Ϊ������
	*/
	int GetLocalIp(std::string& strIp);

	/**	@fn	int ServiceControl(const string strSvrName, bool bRun);
	*	@brief �������
	*	@param [in] strSvrName	������
	*	@param [in] bRun		�Ƿ�����
	*	@return	�ɹ�����0������Ϊ������
	*/
	int ServiceControl(const std::string strSvrName, bool bRun = false);

	/**	@fn	int StopService(vector<SVR_INFO_ST>& vctSvrList);
	*	@brief ֹͣ����
	*	@param [in] vctSvrList	�����б�
	*	@return	�ɹ�����0������Ϊ������
	*/
	int StopService(std::vector<SVR_INFO_ST>& vctSvrList);

	/**	@fn	int GetServiceStatus(SVR_INFO_ST& stSvrInfo);
	*	@brief ��ȡ����״̬
	*	@param [in] stSvrInfo	������Ϣ
	*	@return	�ɹ�����0������Ϊ������
	*/
	int GetServiceStatus(SVR_INFO_ST& stSvrInfo);

	/**	@fn	int KillProcessByAppName(const string& strAppName);
	*	@brief ����Ӧ�ó�����ɱ������
	*	@param [out] strAppName	Ӧ�ó�����
	*	@return	�ɹ�����0������Ϊ������
	*/
	int KillProcessByAppName(const std::string& strAppName);
private:
	CWinApiWrap();
};

#endif