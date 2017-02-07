
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
	*	@brief 获取本机所有进程信息
	*	@param [in]  无
	*	@param [out] mapProcInfo <进程ID，进程名>
	*	@return	成功返回0，其他为错误码
	*/
	int GetProcInfo(std::map<int, std::string>& mapProcInfo);

	/**	@fn	int GetProcInfo(map<int, string>& mapProcInfo);
	*	@brief 获取本机所有服务信息
	*	@param [in]  无
	*	@param [out] mapProcInfo <进程ID，服务名>
	*	@return	成功返回0，其他为错误码
	*/
	int GetSvrInfo(std::map<int, std::string>& mapSvrInfo);

	/**	@fn	int GetProcPathByPid(DWORD dwProcessID, char szProcPath[]);
	*	@brief 通过PID查找进程路径
	*	@param [in]  dwProcessID	进程PID
	*	@param [out] szProcPath		进程路径
	*	@return	成功返回0，其他为错误码
	*/
	int GetProcPathByPid(DWORD dwProcessID, char szProcPath[]);

	/**	@fn	int GetPidByAppName(string strAppName, int& iPid);
	*	@brief 通过应用程序名查找PID
	*	@param [in]  strAppName	应用程序名
	*	@param [out] iPid		进程PID
	*	@return	成功返回0，其他为错误码
	*/
	int GetPidByAppName(std::string strAppName, int& iPid);

	/**	@fn	int GetAppSetupPath(string& strAppName, string& strPath);
	*	@brief 获取应用程序安装路径
	*	@param [in] strAppName	应用程序名
	*	@param [out] strPath	安装路径
	*	@return	成功返回0，其他为错误码
	*/
	int GetAppSetupPath(std::string strAppName, std::string& strPath);

	/**	@fn	int GetSetupPathBySvrName(string strSvrName, string& strPath)
	*	@brief 根据服务名获取应用程序安装路径
	*	@param [in] strAppName	服务名
	*	@param [out] strPath	安装路径
	*	@return	成功返回0，其他为错误码
	*/
	int GetSetupPathBySvrName(std::string strSvrName, std::string& strPath);

	/**	@fn	int GetLocalIp(string& strIp);
	*	@brief 获取本机IP地址
	*	@param [out] strIp		IP地址
	*	@return	成功返回0，其他为错误码
	*/
	int GetLocalIp(std::string& strIp);

	/**	@fn	int ServiceControl(const string strSvrName, bool bRun);
	*	@brief 服务控制
	*	@param [in] strSvrName	服务名
	*	@param [in] bRun		是否运行
	*	@return	成功返回0，其他为错误码
	*/
	int ServiceControl(const std::string strSvrName, bool bRun = false);

	/**	@fn	int StopService(vector<SVR_INFO_ST>& vctSvrList);
	*	@brief 停止服务
	*	@param [in] vctSvrList	服务列表
	*	@return	成功返回0，其他为错误码
	*/
	int StopService(std::vector<SVR_INFO_ST>& vctSvrList);

	/**	@fn	int GetServiceStatus(SVR_INFO_ST& stSvrInfo);
	*	@brief 获取服务状态
	*	@param [in] stSvrInfo	服务信息
	*	@return	成功返回0，其他为错误码
	*/
	int GetServiceStatus(SVR_INFO_ST& stSvrInfo);

	/**	@fn	int KillProcessByAppName(const string& strAppName);
	*	@brief 根据应用程序名杀死进程
	*	@param [out] strAppName	应用程序名
	*	@return	成功返回0，其他为错误码
	*/
	int KillProcessByAppName(const std::string& strAppName);
private:
	CWinApiWrap();
};

#endif