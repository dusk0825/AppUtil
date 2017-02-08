
#ifndef _ZIP_SDK_H_
#define _ZIP_SDK_H_

#if (defined _WIN32 || defined _WIN64)
#   ifdef ZIPSDK_EXPORTS
#       define ZIPSDK_EXTERN __declspec(dllexport)
#   else
#       define ZIPSDK_EXTERN __declspec(dllimport)
#   endif
#   define ZIPSDK_API __stdcall
#elif defined(__linux__)
#   define ZIPSDK_EXTERN extern "C"
#   define ZIPSDK_API
#else
#   define ZIPSDK_EXTERN
#   define ZIPSDK_API
#endif

typedef int (__stdcall *Update_UncompressProgressCB)(double dTotal, double dNow, void* pUserData);

ZIPSDK_EXTERN int ZIPSDK_API UnCompress7z(const char* pszPak, const char* pszName,Update_UncompressProgressCB pfnUnZipCB,void* pData);

typedef int (__stdcall *Update_CompressProgressCB)(double dTotal, double dNow, void* pUserData);

ZIPSDK_EXTERN int ZIPSDK_API Compress7z(const char* pszPak, const char* pszName,Update_CompressProgressCB pfnZipCB,void* pData);

#endif
