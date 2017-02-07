
#include "FireWallMgr.h"

CFireWallMgr::CFireWallMgr()
{
	m_pFireWallProfile = NULL;
}

CFireWallMgr::~CFireWallMgr()
{
	m_pFireWallProfile = NULL;
}

int CFireWallMgr::Init()
{
	INetFwMgr* fwMgr = NULL;
	INetFwPolicy* fwPolicy = NULL;
	int iRet = DTK_ERROR;

	if (NULL != m_pFireWallProfile)
	{
		LOG_ERROR("Have already init.");
		return DTK_ERROR;
	}

	HRESULT result = CoInitialize(NULL);
	if (FAILED(result))
	{
		LOG_ERROR("CoInitialize fail, %x", result);
		return DTK_ERROR;
	}

	do 
	{
		result = CoCreateInstance(__uuidof(NetFwMgr), NULL, CLSCTX_ALL, __uuidof( INetFwMgr), (void**)&fwMgr);
		if (FAILED(result))
		{
			LOG_ERROR("CoCreateInstance fail, %x", result);
			break;
		}

		result = fwMgr->get_LocalPolicy(&fwPolicy);
		if (FAILED(result))
		{
			LOG_ERROR("get_LocalPolicy fail, %x", result);
			break;
		}

		result = fwPolicy->get_CurrentProfile(&m_pFireWallProfile);
		if (FAILED(result))
		{
			LOG_ERROR("get_CurrentProfile fail, %x", result);
			break;
		}

		iRet = DTK_OK;
		LOG_INFO("Init succ.");
	} while (0);

	if(NULL != fwPolicy)
	{
		fwPolicy->Release();
		fwPolicy = NULL;
	}

	if(NULL != fwMgr)
	{
		fwMgr->Release();
		fwMgr = NULL;
	}

	if (DTK_OK != iRet)
	{
		CoUninitialize();
	}

	return iRet;
}

void CFireWallMgr::Fini()
{
	if(NULL == m_pFireWallProfile)
	{
		LOG_ERROR("Have't init.");
		return;
	}

	m_pFireWallProfile->Release();
	m_pFireWallProfile = NULL;

	CoUninitialize();
	LOG_INFO("Fini Succ.");
}

int CFireWallMgr::IsFirewallOn(bool& bIsOn)
{
	bIsOn = false;
	DTK_INT32 iRet = DTK_ERROR;

	do 
	{
		if (NULL == m_pFireWallProfile)
		{
			LOG_ERROR("have't init module.");
			break;
		}

		VARIANT_BOOL bFWEnabled = VARIANT_FALSE;
		HRESULT result = m_pFireWallProfile->get_FirewallEnabled(&bFWEnabled);
		if (FAILED(result))
		{
			LOG_ERROR("get_FirewallEnabled fail, %x", result);
			break;
		}

		if (VARIANT_FALSE != bFWEnabled)
		{
			bIsOn = true;
		}

		iRet = DTK_OK;
	} while (0);

	return iRet;
}

int CFireWallMgr::TurnOnFirewall()
{
	int iRet = DTK_ERROR;

	do
	{
		if (NULL == m_pFireWallProfile)
		{
			LOG_ERROR("have't init module.");
			break;
		}

		bool bFWOn = false;
		if (DTK_OK != IsFirewallOn(bFWOn))
		{
			break;
		}

		if (!bFWOn)
		{
			HRESULT result = m_pFireWallProfile->put_FirewallEnabled(VARIANT_TRUE);
			if (FAILED(result))
			{
				LOG_ERROR("put_FirewallEnabled fail, %x", result);
				break;
			}
		}

		iRet = DTK_OK;
	} while (0);

	return iRet;
}

int CFireWallMgr::TurnOffFirewall()
{
	int iRet = DTK_ERROR;

	do
	{
		if (NULL == m_pFireWallProfile)
		{
			LOG_ERROR("have't init module.");
			break;
		}

		bool bFWOn = false;
		if (DTK_OK != IsFirewallOn(bFWOn))
		{
			break;
		}

		if (!bFWOn)
		{
			HRESULT result = m_pFireWallProfile->put_FirewallEnabled(VARIANT_FALSE);
			if (FAILED(result))
			{
				LOG_ERROR("put_FirewallEnabled fail, %x", result);
				break;
			}
		}

		iRet = DTK_OK;
	} while (0);

	return iRet;
}

int CFireWallMgr::IsAppEnabled( const wchar_t* lpszProcessImageFileName, DTK_BOOL& bEnable )
{
	int iRet = DTK_ERROR;
	BSTR bstrFWProcessImageFileName = NULL;
	INetFwAuthorizedApplication* pFWApp = NULL;
	INetFwAuthorizedApplications* pFWApps = NULL;

	if (NULL == lpszProcessImageFileName)
	{
		LOG_ERROR("param is null.");
		return iRet;
	}

	bEnable = DTK_FALSE;
	do
	{
		if (NULL == m_pFireWallProfile)
		{
			LOG_ERROR("have't init module.");
			break;
		}

		HRESULT result = m_pFireWallProfile->get_AuthorizedApplications(&pFWApps);
		if (FAILED(result))
		{
			LOG_ERROR("get_AuthorizedApplications fail, %x", result);
			break;
		}

		bstrFWProcessImageFileName = SysAllocString(lpszProcessImageFileName);
		if (NULL == bstrFWProcessImageFileName || 0 == SysStringLen(bstrFWProcessImageFileName))
		{
			LOG_ERROR("SysAllocString errCode:%d", GetLastError());
			break;
		}

		result = pFWApps->Item(bstrFWProcessImageFileName, &pFWApp);
		if (SUCCEEDED(result))
		{
			VARIANT_BOOL bFWEnabled = VARIANT_FALSE;
			result = pFWApp->get_Enabled( &bFWEnabled );
			if (FAILED(result))
			{
				LOG_ERROR("get_Enabled fail, %x", result);
				break;
			}

			if (VARIANT_TRUE == bFWEnabled)
			{
				bEnable = DTK_TRUE;
			}
		}

		iRet = DTK_OK;
	} while (0);

	if (NULL != bstrFWProcessImageFileName)
	{
		SysFreeString(bstrFWProcessImageFileName);
		bstrFWProcessImageFileName = NULL;
	}

	if (NULL != pFWApp)
	{
		pFWApp->Release();
		pFWApp = NULL;
	}

	if (NULL != pFWApps)
	{
		pFWApps->Release();
		pFWApps = NULL;
	}

	return iRet;
}

int CFireWallMgr::AddApplication( const wchar_t* lpszProcessImageFileName, const wchar_t* lpszRegisterName )
{
	int iRet = DTK_ERROR;
	BSTR bstrRegisterName = NULL;
	BSTR bstrProcessImageFileName = NULL;
	INetFwAuthorizedApplication* pFWApp = NULL;
	INetFwAuthorizedApplications* pFWApps = NULL;

	if ((NULL == lpszProcessImageFileName) || (NULL == lpszRegisterName))
	{
		LOG_ERROR("param is null.");
		return iRet;
	}

	do
	{
		if (NULL == m_pFireWallProfile)
		{
			LOG_ERROR("have't init module.");
			break;
		}

		DTK_BOOL bAppEnable = DTK_FALSE;
		if (DTK_OK != IsAppEnabled(lpszProcessImageFileName, bAppEnable))
		{
			break;
		}
		
		//应用不能重复添加
		if (DTK_TRUE == bAppEnable)
		{
			iRet = -1;
			break;
		}

		HRESULT result = m_pFireWallProfile->get_AuthorizedApplications(&pFWApps);
		if (FAILED(result))
		{
			LOG_ERROR("get_AuthorizedApplications fail, %x", result);
			break;
		}

		result = CoCreateInstance(__uuidof(NetFwAuthorizedApplication), NULL, CLSCTX_ALL, __uuidof(INetFwAuthorizedApplication), (void**)&pFWApp);
		if (FAILED(result))
		{
			LOG_ERROR("CoCreateInstance fail, %x", result);
			break;
		}

		bstrProcessImageFileName = SysAllocString(lpszProcessImageFileName);
		if (NULL == bstrProcessImageFileName || 0 == SysStringLen(bstrProcessImageFileName))
		{
			LOG_ERROR("SysAllocString errCode:%d", GetLastError());
			break;
		}

		result = pFWApp->put_ProcessImageFileName(bstrProcessImageFileName);
		if (FAILED(result))
		{
			LOG_ERROR("put_ProcessImageFileName fail, %x", result);
			break;
		}

		bstrRegisterName = SysAllocString(lpszRegisterName);
		if (NULL == bstrRegisterName || 0 == SysStringLen(bstrRegisterName ))
		{
			LOG_ERROR("SysAllocString errCode:%d", result);
			break;
		}

		result = pFWApp->put_Name( bstrRegisterName );
		if (FAILED(result))
		{
			LOG_ERROR("put_Name fail, %x", result);
			break;
		}

		result = pFWApps->Add( pFWApp );
		if (FAILED(result))
		{
			LOG_ERROR("Add fail, %x", result);
			break;
		}

		iRet = DTK_OK;
	} while (0);

	if (NULL != bstrProcessImageFileName)
	{
		SysFreeString(bstrProcessImageFileName);
		bstrProcessImageFileName = NULL;
	}
	if (NULL != bstrRegisterName)
	{
		SysFreeString(bstrRegisterName);
		bstrRegisterName = NULL;
	}
	if (NULL != pFWApp)
	{
		pFWApp->Release();
		pFWApp = NULL;
	}
	if (NULL != pFWApps)
	{
		pFWApps->Release();
		pFWApps = NULL;
	}

	return iRet;
}

int CFireWallMgr::RemoveApplication( const wchar_t* lpszProcessImageFileName )
{
	DTK_INT32 iRet = DTK_ERROR;
	BSTR bstrProcessImageFileName = NULL;
	INetFwAuthorizedApplications* pFWApps = NULL;

	if (NULL == lpszProcessImageFileName)
	{
		LOG_ERROR("param is null.");
		return iRet;
	}

	do 
	{
		if (NULL == m_pFireWallProfile)
		{
			LOG_ERROR("have't init module.");
			break;
		}

		DTK_BOOL bAppEnable = DTK_FALSE;
		if (DTK_OK != IsAppEnabled(lpszProcessImageFileName, bAppEnable))
		{
			break;
		}

		if (DTK_TRUE == bAppEnable)
		{
			HRESULT result = m_pFireWallProfile->get_AuthorizedApplications(&pFWApps);
			if (FAILED(result))
			{
				LOG_ERROR("get_AuthorizedApplications fail, %x", result);
				break;
			}

			bstrProcessImageFileName = SysAllocString(lpszProcessImageFileName);
			if (NULL == bstrProcessImageFileName || 0 == SysStringLen(bstrProcessImageFileName))
			{
				LOG_ERROR("SysAllocString errCode:%d", GetLastError());
				break;
			}

			result = pFWApps->Remove( bstrProcessImageFileName );
			if (FAILED(result))
			{
				LOG_ERROR("Remove fail, %x", result);
				break;
			}

			iRet = DTK_OK;
		}
	} while (0);

	if (NULL != bstrProcessImageFileName)
	{
		SysFreeString(bstrProcessImageFileName);
		bstrProcessImageFileName = NULL;
	}

	if (NULL != pFWApps)
	{
		pFWApps->Release();
		pFWApps = NULL;
	}

	return iRet;
}

int CFireWallMgr::AddPortRule(LONG lPortNumber, NET_FW_IP_PROTOCOL ipProtocol, const wchar_t* lpszRegisterName)
{
	int iRet = DTK_ERROR;
	INetFwOpenPort* pFWOpenPort = NULL;
	INetFwOpenPorts* pFWOpenPorts = NULL;
	BSTR bstrRegisterName = NULL;

	do 
	{
		if (NULL == m_pFireWallProfile)
		{
			break;
		}

		BOOL bEnablePort = FALSE;
		if (DTK_OK != IsPortEnabled(lPortNumber, ipProtocol, bEnablePort))
		{
			break;
		}

		if (FALSE == bEnablePort)
		{
			HRESULT hResult = m_pFireWallProfile->get_GloballyOpenPorts(&pFWOpenPorts);
			if (FAILED(hResult))
			{
				LOG_ERROR("get_GloballyOpenPorts fail, %x", hResult);
				break;
			}

			hResult = CoCreateInstance(__uuidof(NetFwOpenPort), NULL, CLSCTX_ALL, __uuidof(INetFwOpenPort), (void**)&pFWOpenPort);
			if (FAILED(hResult))
			{
				LOG_ERROR("CoCreateInstance fail, %x", hResult);
				break;
			}

			hResult = pFWOpenPort->put_Port(lPortNumber);
			if (FAILED(hResult))
			{
				LOG_ERROR("put_Port fail, %x", hResult);
				break;
			}

			hResult = pFWOpenPort->put_Protocol(ipProtocol);
			if (FAILED(hResult))
			{
				LOG_ERROR("put_Protocol fail, %x", hResult);
				break;
			}

			bstrRegisterName = SysAllocString(lpszRegisterName);
			if (NULL == bstrRegisterName || 0 == SysStringLen(bstrRegisterName))
			{
				LOG_ERROR("SysAllocString errCode:%d", GetLastError());
				break;
			}

			hResult = pFWOpenPort->put_Name(bstrRegisterName);
			if (FAILED(hResult))
			{
				LOG_ERROR("put_Name fail, %x", hResult);
				break;
			}

			hResult = pFWOpenPorts->Add(pFWOpenPort);
			if (FAILED(hResult))
			{
				LOG_ERROR("Add fail, %x", hResult);
				break;
			}

			iRet= DTK_OK;
		}
	} while (0);

	if (NULL != bstrRegisterName)
	{
		SysFreeString(bstrRegisterName);
		bstrRegisterName = NULL;
	}

	if (NULL != pFWOpenPort)
	{
		pFWOpenPort->Release();
		pFWOpenPort = NULL;
	}

	if (NULL != pFWOpenPorts)
	{
		pFWOpenPorts->Release();
		pFWOpenPorts = NULL;
	}

	return iRet;
}

int CFireWallMgr::RemovePortRule( LONG lPortNumber, NET_FW_IP_PROTOCOL ipProtocol )
{
	int iRet = DTK_ERROR;
	INetFwOpenPorts* pFWOpenPorts = NULL;

	do
	{
		if (NULL == m_pFireWallProfile)
		{
			break;
		}

		BOOL bEnablePort = FALSE;
		if (DTK_OK != IsPortEnabled(lPortNumber, ipProtocol, bEnablePort))
		{
			break;
		}

		if (bEnablePort == TRUE)
		{
			HRESULT result = m_pFireWallProfile->get_GloballyOpenPorts(&pFWOpenPorts);
			if (FAILED(result))
			{
				LOG_ERROR("get_GloballyOpenPorts fail, %x", result);
				break;
			}

			result = pFWOpenPorts->Remove(lPortNumber, ipProtocol);
			if (FAILED(result))
			{
				LOG_ERROR("Remove fail, %x", result);
				break;
			}
		}

		iRet = DTK_OK;
	} while (0);

	if (NULL != pFWOpenPorts)
	{
		pFWOpenPorts->Release();
		pFWOpenPorts = NULL;
	}

	return iRet;
}

int CFireWallMgr::IsPortEnabled( LONG lPortNumber, NET_FW_IP_PROTOCOL ipProtocol, BOOL& bEnable )
{
	int iRet = DTK_ERROR;
	INetFwOpenPort* pFWOpenPort = NULL;
	INetFwOpenPorts* pFWOpenPorts = NULL;

	bEnable = FALSE;

	do 
	{
		if (NULL == m_pFireWallProfile)
		{
			break;
		}

		HRESULT hResult = m_pFireWallProfile->get_GloballyOpenPorts(&pFWOpenPorts);
		if (FAILED(hResult))
		{
			LOG_ERROR("get_GloballyOpenPorts fail, %x", hResult);
			break;
		}

		hResult = pFWOpenPorts->Item(lPortNumber, ipProtocol, &pFWOpenPort);
		if (FAILED(hResult))
		{
			LOG_ERROR("Item fail, %x", hResult);
			break;
		}

		VARIANT_BOOL bFWEnabled;
		hResult = pFWOpenPort->get_Enabled(&bFWEnabled);
		if (FAILED(hResult))
		{
			LOG_ERROR("get_Enabled fail, %x", hResult);
			break;
		}

		if (VARIANT_TRUE == bFWEnabled)
		{
			bEnable = TRUE;
		}

		iRet = DTK_OK;
	} while (0);

	if (NULL != pFWOpenPort)
	{
		pFWOpenPort->Release();
		pFWOpenPort = NULL;
	}

	if (NULL != pFWOpenPorts)
	{
		pFWOpenPorts->Release();
		pFWOpenPorts = NULL;
	}

	return iRet;
}