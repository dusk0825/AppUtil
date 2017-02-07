
#ifndef __DTK_ASYNCIOEX_H__  
#define __DTK_ASYNCIOEX_H__  

#include "DTK_Config.h"
#include "DTK_Types.h"
#include "DTK_Addr.h"

#if defined(OS_WINDOWS)
	#define DTK_INVALID_ASYNCIOQUEUE NULL
	#define DTK_INVALID_ASYNCIO NULL
#elif defined(OS_POSIX)
	#define DTK_INVALID_ASYNCIOQUEUE (DTK_VOIDPTR)-1
	#define DTK_INVALID_ASYNCIO (DTK_VOIDPTR)-1
#else
	#error OS Not Implement Yet.
#endif

#if defined OS_WINDOWS

DTK_DECLARE DTK_HANDLE CALLBACK DTK_AsyncIO_CreateQueueEx(DTK_INT32 iThreadNum = 0);

DTK_DECLARE DTK_INT32 CALLBACK DTK_AsyncIO_DestroyQueueEx(DTK_HANDLE hIOCP);

DTK_DECLARE DTK_INT32 CALLBACK DTK_AsyncIO_BindIOHandleToQueueEx(DTK_HANDLE hIOFd, DTK_HANDLE hIOCP);

DTK_DECLARE DTK_INT32 CALLBACK DTK_AsyncIO_UnBindIOHandleEx(DTK_HANDLE hIOFd, DTK_HANDLE hIOCP);

DTK_DECLARE DTK_INT32 CALLBACK DTK_AsyncIO_BindCallBackToIOHandleEx(DTK_HANDLE hIOFd, DTK_VOID (*CallBackFunc)(DTK_ULONG nErrorCode, DTK_ULONG nNumberOfBytes, DTK_VOIDPTR pUsrData));

DTK_DECLARE DTK_INT32 CALLBACK DTK_AsyncIO_PostQueuedCompleteStatusEx(DTK_HANDLE hIOCP, DTK_HANDLE hIOFd,DTK_INT32 iErrorCode,DTK_UINT32 nNumberOfBytesTransfered, DTK_VOIDPTR pUsrData, DTK_VOIDPTR pCallBack = NULL);

DTK_DECLARE DTK_INT32 CALLBACK DTK_AsyncIO_SendEx(DTK_HANDLE hIOFd, DTK_VOIDPTR pBuffer, DTK_ULONG BytesToSend, DTK_VOIDPTR pUsrData, DTK_VOIDPTR pCallBack = NULL);

DTK_DECLARE DTK_INT32 CALLBACK DTK_AsyncIO_RecvEx(DTK_HANDLE hIOFd, DTK_VOIDPTR pBuffer, DTK_ULONG nBufferLen, DTK_VOIDPTR pUsrData, DTK_VOIDPTR pCallBack = NULL);

DTK_DECLARE DTK_INT32 CALLBACK DTK_AsyncIO_AcceptEx(DTK_HANDLE hIOFd, DTK_ADDR_T* pPeerAddr,DTK_VOIDPTR pBuffer,DTK_INT32 iBufLen,
                                                    DTK_VOIDPTR pUsrData, DTK_VOIDPTR pCallBack, const DTK_INT32 iTimeOut);

DTK_DECLARE DTK_INT32 CALLBACK DTK_AsyncIO_ConnectEx(DTK_HANDLE hIOFd, DTK_ADDR_T* pPeerAddr, DTK_VOIDPTR pBuffer,DTK_INT32 iBufLen,
                                                     DTK_VOIDPTR pUsrData,DTK_VOIDPTR pCallBack, const DTK_INT32 iTimeOut = -1);

DTK_DECLARE DTK_INT32 CALLBACK DTK_AsyncIO_Accept(DTK_HANDLE hIOFd,  DTK_VOIDPTR pBuffer, DTK_SOCK_T sock, DTK_VOIDPTR pUsrData, DTK_VOIDPTR pCallBack = NULL);

DTK_DECLARE DTK_INT32 CALLBACK DTK_GetAcceptExSockaddrs(DTK_VOIDPTR pBuffer, struct sockaddr* plocal, struct sockaddr* premote);

DTK_DECLARE DTK_INT32 CALLBACK DTK_AsyncIO_SendToEx(DTK_HANDLE hIOFd, DTK_VOIDPTR pBuffer, DTK_ULONG nBytesToSend, DTK_VOIDPTR pUsrData, DTK_ADDR_T* pDstAddr, DTK_VOIDPTR pCallBack = NULL);

DTK_DECLARE DTK_INT32 CALLBACK DTK_AsyncIO_RecvFromEx(DTK_HANDLE hIOFd, DTK_VOIDPTR pBuffer, DTK_ULONG nBufferLen, DTK_VOIDPTR pUsrData, DTK_ADDR_T* pAddrSrc, DTK_VOIDPTR pCallBack = NULL);

DTK_DECLARE DTK_INT32 CALLBACK DTK_AsyncIO_WriteFileEx(DTK_HANDLE hIOFd, DTK_VOIDPTR pBuffer, DTK_ULONG nBytesToWrite, DTK_VOIDPTR pUsrData, DTK_VOIDPTR pCallBack = NULL);

DTK_DECLARE DTK_INT32 CALLBACK DTK_AsyncIO_ReadFileEx(DTK_HANDLE hIOFd, DTK_VOIDPTR pBuffer, DTK_ULONG nBufferLen, DTK_VOIDPTR pUsrData, DTK_VOIDPTR pCallBack = NULL);

typedef DTK_VOID (*IOCALLBACK)(DTK_ULONG nErrorCode, DTK_ULONG nNumberOfBytes, DTK_VOIDPTR pUsrData);

#endif

#endif // __DTK_ASYNCIOEX_H__ 