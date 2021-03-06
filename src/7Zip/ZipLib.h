
//#include <Windows.h>
//#include <windows.h>
#include <stdio.h>
//#include <string.h>
//#include <iostream>
//#include <vector>

#include "Common/IntToString.h"
#include "Common/MyInitGuid.h"
#include "Common/StringConvert.h"

#include "Windows/DLL.h"
#include "Windows/FileDir.h"
#include "Windows/FileFind.h"
#include "Windows/FileName.h"
#include "Windows/NtCheck.h"
#include "Windows/PropVariant.h"
#include "Windows/PropVariantConversions.h"

#include "7zip/Common/FileStreams.h"

#include "7zip/Archive/IArchive.h"

#include "7zip/IPassword.h"
#include "7zip/MyVersion.h"

typedef int (__stdcall *Update_UncompressProgressCB)(double dTotal, double dNow, void* pUserData);
typedef int (__stdcall *Update_CompressProgressCB)(double dTotal, double dNow, void* pUserData);

#define     PCLINT_UNUSED_ARG(x)  ((void)x)             /* 参数、变量未使用处理 */

//////////////////////////////////////////////////////////////
// Archive Open callback class

/*lint -save -e1510*/
class CArchiveOpenCallback:
    public IArchiveOpenCallback,
    public ICryptoGetTextPassword,
    public CMyUnknownImp
{
public:
    MY_UNKNOWN_IMP1(ICryptoGetTextPassword)

        STDMETHOD(SetTotal)(const UInt64 *files, const UInt64 *bytes);
    STDMETHOD(SetCompleted)(const UInt64 *files, const UInt64 *bytes);

    STDMETHOD(CryptoGetTextPassword)(BSTR *password);

    bool PasswordIsDefined;
    UString Password;

    UInt64 m_uiFiles;
    UInt64 m_uiTotal;

    CArchiveOpenCallback() 
        : PasswordIsDefined(false)
        ,m_uiFiles(0)
        ,m_uiTotal(0)
    {}
};


//////////////////////////////////////////////////////////////
// Archive Extracting callback class

class CArchiveExtractCallback:
    public IArchiveExtractCallback,
    public ICryptoGetTextPassword,
    public CMyUnknownImp
{
public:
    MY_UNKNOWN_IMP1(ICryptoGetTextPassword)

        // IProgress
        STDMETHOD(SetTotal)(UInt64 size);
    STDMETHOD(SetCompleted)(const UInt64 *completeValue);

    // IArchiveExtractCallback
    STDMETHOD(GetStream)(UInt32 index, ISequentialOutStream **outStream, Int32 askExtractMode);
    STDMETHOD(PrepareOperation)(Int32 askExtractMode);
    STDMETHOD(SetOperationResult)(Int32 resultEOperationResult);

    // ICryptoGetTextPassword
    STDMETHOD(CryptoGetTextPassword)(BSTR *aPassword);

private:
    CMyComPtr<IInArchive> _archiveHandler;
    UString _directoryPath;  // Output directory
    UString _filePath;       // name inside arcvhive
    UString _diskFilePath;   // full path to file on disk
    bool _extractMode;
    struct CProcessedFileInfo
    {
        FILETIME MTime;
        UInt32 Attrib;
        bool isDir;
        bool AttribDefined;
        bool MTimeDefined;
    } _processedFileInfo;

    COutFileStream *_outFileStreamSpec;
    CMyComPtr<ISequentialOutStream> _outFileStream;

public:
    void Init(IInArchive *archiveHandler, const UString &directoryPath);

    UInt64 NumErrors;
    bool PasswordIsDefined;
    UString Password;

    UInt64 m_uiTotal;

    CArchiveExtractCallback() 
        : _extractMode(false)
        ,_outFileStreamSpec(NULL)
        ,NumErrors(0)
        ,PasswordIsDefined(false)
        ,m_uiTotal(0)
        ,m_pfnCB(NULL)
        ,m_pData(NULL) 
    {
        memset(&_processedFileInfo,0,sizeof(_processedFileInfo));
    }

    Update_UncompressProgressCB m_pfnCB;
    void* m_pData;
};

//////////////////////////////////////////////////////////////
// Archive Creating callback class

struct CDirItem
{
    UInt64 Size;
    FILETIME CTime;
    FILETIME ATime;
    FILETIME MTime;
    UString Name;
    UString FullPath;
    UInt32 Attrib;

    bool isDir() const { return (Attrib & FILE_ATTRIBUTE_DIRECTORY) != 0 ; }
};

class CArchiveUpdateCallback:
    public IArchiveUpdateCallback2,
    public ICryptoGetTextPassword2,
    public CMyUnknownImp
{
public:
    MY_UNKNOWN_IMP2(IArchiveUpdateCallback2, ICryptoGetTextPassword2)

        // IProgress
        STDMETHOD(SetTotal)(UInt64 size);
    STDMETHOD(SetCompleted)(const UInt64 *completeValue);

    // IUpdateCallback2
    STDMETHOD(EnumProperties)(IEnumSTATPROPSTG **enumerator);
    STDMETHOD(GetUpdateItemInfo)(UInt32 index,
        Int32 *newData, Int32 *newProperties, UInt32 *indexInArchive);
    STDMETHOD(GetProperty)(UInt32 index, PROPID propID, PROPVARIANT *value);
    STDMETHOD(GetStream)(UInt32 index, ISequentialInStream **inStream);
    STDMETHOD(SetOperationResult)(Int32 operationResult);
    STDMETHOD(GetVolumeSize)(UInt32 index, UInt64 *size);
    STDMETHOD(GetVolumeStream)(UInt32 index, ISequentialOutStream **volumeStream);

    STDMETHOD(CryptoGetTextPassword2)(Int32 *passwordIsDefined, BSTR *password);

public:
    CRecordVector<UInt64> VolumesSizes;
    UString VolName;
    UString VolExt;

    UString DirPrefix;
    const CObjectVector<CDirItem> *DirItems;

    bool PasswordIsDefined;
    UString Password;
    bool AskPassword;

    bool m_NeedBeClosed;

    UInt64 m_uiTotal;

    UStringVector FailedFiles;
    CRecordVector<HRESULT> FailedCodes;

    Update_CompressProgressCB m_pfnZipCB;
    void* m_pData;

    CArchiveUpdateCallback()
        : DirItems(0)
        , PasswordIsDefined(false)
        , AskPassword(false)
        , m_NeedBeClosed(false)
        ,m_uiTotal(0)
        ,m_pfnZipCB(NULL)
        ,m_pData(NULL)
    {};

    ~CArchiveUpdateCallback() 
    { 
        try
        {
            Finilize(); 
        }
        catch (...)
        {
        	
        }
        
        
    }
    HRESULT Finilize();

    void Init(const CObjectVector<CDirItem> *dirItems)
    {
        DirItems = dirItems;
        m_NeedBeClosed = false;
        FailedFiles.Clear();
        FailedCodes.Clear();
        m_pfnZipCB = NULL;
        m_pData = NULL;
    }
    
};

/*lint -restore*/
int UnCompress(AString strPak, AString strName,Update_UncompressProgressCB pfnUnZipCB,void* pData);

int Compress(AString strPak, AString strName,Update_CompressProgressCB pfnZipCB,void* pData);

int TraverseFloder(AString strFileName, AString strFolderPath, CObjectVector<AString>& fileList);

