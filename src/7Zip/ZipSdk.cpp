
#include "ZipSdk.h"
#include "ZipLib.h"


ZIPSDK_EXTERN int ZIPSDK_API UnCompress7z(const char* pszPak, const char* pszName,Update_UncompressProgressCB pfnUnZipCB,void* pData)
{
    int iRet = UnCompress(pszPak, pszName, pfnUnZipCB, pData);
    return iRet;
}

ZIPSDK_EXTERN int ZIPSDK_API Compress7z(const char* pszPak, const char* pszName,Update_CompressProgressCB pfnZipCB,void* pData)
{
    int iRet = Compress(pszPak, pszName, pfnZipCB, pData);
    return iRet;
}
