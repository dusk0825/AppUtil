

#include "ZipLib.h"
#include "Errorcode.h"

// use another CLSIDs, if you want to support other formats (zip, rar, ...).
// {23170F69-40C1-278A-1000-000110070000}
DEFINE_GUID(CLSID_CFormat7z,
            0x23170F69, 0x40C1, 0x278A, 0x10, 0x00, 0x00, 0x01, 0x10, 0x07, 0x00, 0x00);

using namespace NWindows;

#define kDllName "7z.dll"

typedef UINT32 (WINAPI * CreateObjectFunc)(
    const GUID *clsID,
    const GUID *interfaceID,
    void **outObject);

static HRESULT IsArchiveItemProp(IInArchive *archive, UInt32 index, PROPID propID, bool &result)
{
    NCOM::CPropVariant prop;
    RINOK(archive->GetProperty(index, propID, &prop));
    if (prop.vt == VT_BOOL)
        result = VARIANT_BOOLToBool(prop.boolVal);
    else if (prop.vt == VT_EMPTY)
        result = false;
    else
        return E_FAIL;
    return S_OK;
}

static HRESULT IsArchiveItemFolder(IInArchive *archive, UInt32 index, bool &result)
{
    return IsArchiveItemProp(archive, index, kpidIsDir, result);
}

static const wchar_t *kEmptyFileAlias = L"[Content]";

//////////////////////////////////////////////////////////////
// Archive Open callback class



STDMETHODIMP CArchiveOpenCallback::SetTotal(const UInt64 *  files , const UInt64 *  bytes)
{
    m_uiFiles = *files;
    m_uiTotal = *bytes;
    return S_OK;
}

STDMETHODIMP CArchiveOpenCallback::SetCompleted(const UInt64 *  files , const UInt64 *  bytes)
{
    //printf("complete files = %d, bytes = %d", *files, *bytes);
    return S_OK;
}

STDMETHODIMP CArchiveOpenCallback::CryptoGetTextPassword(BSTR *password)
{
    if (!PasswordIsDefined)
    {
        // You can ask real password here from user
        // Password = GetPassword(OutStream);
        // PasswordIsDefined = true;
        //PrintError("Password is not defined");
        return E_ABORT;
    }
    return StringToBstr(Password, password);
}

//////////////////////////////////////////////////////////////
// Archive Extracting callback class

//static const wchar_t *kCantDeleteOutputFile = L"ERROR: Can not delete output file ";
//
//static const char *kTestingString    =  "Testing     ";
//static const char *kExtractingString =  "Extracting  ";
//static const char *kSkippingString   =  "Skipping    ";
//
//static const char *kUnsupportedMethod = "Unsupported Method";
//static const char *kCRCFailed = "CRC Failed";
//static const char *kDataError = "Data Error";
//static const char *kUnknownError = "Unknown Error";


void CArchiveExtractCallback::Init(IInArchive *archiveHandler, const UString &directoryPath)
{
    NumErrors = 0;
    _archiveHandler = archiveHandler;
    _directoryPath = directoryPath;
    NFile::NName::NormalizeDirPathPrefix(_directoryPath);
}

STDMETHODIMP CArchiveExtractCallback::SetTotal(UInt64  size)
{
    m_uiTotal = size;
    return S_OK;
}

STDMETHODIMP CArchiveExtractCallback::SetCompleted(const UInt64 * completeValue )
{
    /*printf("Total: %I64d Completed: %I64d \n", m_uiTotal, *completeValue);
    printf("Completed: %d\n", *completeValue);*/
    if (NULL != m_pfnCB)
    {
        int iRet = m_pfnCB(m_uiTotal,*completeValue,m_pData);
        if (0 > iRet)
        {
            return S_FALSE;
        }
    }
    return S_OK;
}

STDMETHODIMP CArchiveExtractCallback::GetStream(UInt32 index,
                                                ISequentialOutStream **outStream, Int32 askExtractMode)
{
    *outStream = 0;
    _outFileStream.Release();

    {
        // Get Name
        NCOM::CPropVariant prop;
        RINOK(_archiveHandler->GetProperty(index, kpidPath, &prop));

        UString fullPath;
        if (prop.vt == VT_EMPTY)
            fullPath = kEmptyFileAlias;
        else
        {
            if (prop.vt != VT_BSTR)
                return E_FAIL;
            fullPath = prop.bstrVal;
        }
        _filePath = fullPath;
    }

    if (askExtractMode != NArchive::NExtract::NAskMode::kExtract)
        return S_OK;

    {
        // Get Attrib
        NCOM::CPropVariant prop;
        RINOK(_archiveHandler->GetProperty(index, kpidAttrib, &prop));
        if (prop.vt == VT_EMPTY)
        {
            _processedFileInfo.Attrib = 0;
            _processedFileInfo.AttribDefined = false;
        }
        else
        {
            if (prop.vt != VT_UI4)
                return E_FAIL;
            _processedFileInfo.Attrib = prop.ulVal;
            _processedFileInfo.AttribDefined = true;
        }
    }

    RINOK(IsArchiveItemFolder(_archiveHandler, index, _processedFileInfo.isDir));

    {
        // Get Modified Time
        NCOM::CPropVariant prop;
        RINOK(_archiveHandler->GetProperty(index, kpidMTime, &prop));
        _processedFileInfo.MTimeDefined = false;
        switch(prop.vt)
        {
        case VT_EMPTY:
            // _processedFileInfo.MTime = _utcMTimeDefault;
            break;
        case VT_FILETIME:
            _processedFileInfo.MTime = prop.filetime;
            _processedFileInfo.MTimeDefined = true;
            break;
        default:
            return E_FAIL;
        }

    }
    /*lint -e550*/
    {
        // Get Size
        NCOM::CPropVariant prop;
        RINOK(_archiveHandler->GetProperty(index, kpidSize, &prop));
        bool newFileSizeDefined = (prop.vt != VT_EMPTY);
        UInt64 newFileSize;
        if (newFileSizeDefined)
            newFileSize = ConvertPropVariantToUInt64(prop);
    }
    /*lint +e550*/

    {
        // Create folders for file
        int slashPos = _filePath.ReverseFind(WCHAR_PATH_SEPARATOR);
        if (slashPos >= 0)
            NFile::NDirectory::CreateComplexDirectory(_directoryPath + _filePath.Left(slashPos));
    }

    UString fullProcessedPath = _directoryPath + _filePath;
    _diskFilePath = fullProcessedPath;

    if (_processedFileInfo.isDir)
    {
        NFile::NDirectory::CreateComplexDirectory(fullProcessedPath);
    }
    else
    {
        NFile::NFind::CFileInfoW fi;
        if (fi.Find(fullProcessedPath))
        {
            if (!NFile::NDirectory::DeleteFileAlways(fullProcessedPath))
            {
                //PrintString(UString(kCantDeleteOutputFile) + fullProcessedPath);
                return E_ABORT;
            }
        }

        _outFileStreamSpec = new COutFileStream;
        CMyComPtr<ISequentialOutStream> outStreamLoc(_outFileStreamSpec);
        if (!_outFileStreamSpec->Open(fullProcessedPath, CREATE_ALWAYS))
        {
            //PrintString((UString)L"can not open output file " + fullProcessedPath);
            return E_ABORT;
        }
        _outFileStream = outStreamLoc;
        *outStream = outStreamLoc.Detach();
    }
    return S_OK;
}

STDMETHODIMP CArchiveExtractCallback::PrepareOperation(Int32 askExtractMode)
{
    _extractMode = false;
    switch (askExtractMode)
    {
    case NArchive::NExtract::NAskMode::kExtract:  _extractMode = true; break;
    };
    switch (askExtractMode)
    {
    case NArchive::NExtract::NAskMode::kExtract:  /*printf("%s",kExtractingString);*/ break;
    case NArchive::NExtract::NAskMode::kTest:  /*PrintString(kTestingString);*/ break;
    case NArchive::NExtract::NAskMode::kSkip:  /*PrintString(kSkippingString);*/ break;
    };
    //PrintString(_filePath);
    //printf("%s\n",(LPCSTR)GetOemString(_filePath));
    return S_OK;
}

STDMETHODIMP CArchiveExtractCallback::SetOperationResult(Int32 operationResult)
{
    switch(operationResult)
    {
    case NArchive::NExtract::NOperationResult::kOK:
        break;
    default:
        {
            NumErrors++;
            //PrintString("     ");
            switch(operationResult)
            {
            case NArchive::NExtract::NOperationResult::kUnSupportedMethod:
                //PrintString(kUnsupportedMethod);
                break;
            case NArchive::NExtract::NOperationResult::kCRCError:
                //PrintString(kCRCFailed);
                break;
            case NArchive::NExtract::NOperationResult::kDataError:
                //PrintString(kDataError);
                break;
            default:
                break;//PrintString(kUnknownError);
            }
        }
    }

    if ((NULL != _outFileStream) && (NULL != _outFileStreamSpec))
    {
        if (_processedFileInfo.MTimeDefined )
            _outFileStreamSpec->SetMTime(&_processedFileInfo.MTime);
        RINOK(_outFileStreamSpec->Close());
    }
    _outFileStream.Release();
    if (_extractMode && _processedFileInfo.AttribDefined)
        NFile::NDirectory::MySetFileAttributes(_diskFilePath, _processedFileInfo.Attrib);
    //PrintNewLine();
    return S_OK;
}


STDMETHODIMP CArchiveExtractCallback::CryptoGetTextPassword(BSTR *password)
{
    if (!PasswordIsDefined)
    {
        // You can ask real password here from user
        // Password = GetPassword(OutStream);
        // PasswordIsDefined = true;
        //PrintError("Password is not defined");
        return E_ABORT;
    }
    return StringToBstr(Password, password);
}

//////////////////////////////////////////////////////////////
// Archive Creating callback class

STDMETHODIMP CArchiveUpdateCallback::SetTotal(UInt64  size )
{
    m_uiTotal = size;
    return S_OK;
}

STDMETHODIMP CArchiveUpdateCallback::SetCompleted(const UInt64 *  completeValue )
{
    //printf("total = %I64d, complete = %I64d", m_uiTotal, *completeValue);
    if (NULL != m_pfnZipCB)
    {
        m_pfnZipCB(m_uiTotal,*completeValue,m_pData);
    }
    return S_OK;
}


STDMETHODIMP CArchiveUpdateCallback::EnumProperties(IEnumSTATPROPSTG ** /* enumerator */)
{
    return E_NOTIMPL;
}

STDMETHODIMP CArchiveUpdateCallback::GetUpdateItemInfo(UInt32 /* index */,
                                                       Int32 *newData, Int32 *newProperties, UInt32 *indexInArchive)
{
    if (newData != NULL)
        *newData = BoolToInt(true);
    if (newProperties != NULL)
        *newProperties = BoolToInt(true);
    if (indexInArchive != NULL)
        *indexInArchive = (UInt32)-1;
    return S_OK;
}

STDMETHODIMP CArchiveUpdateCallback::GetProperty(UInt32 index, PROPID propID, PROPVARIANT *value)
{
    NWindows::NCOM::CPropVariant prop;

    if (propID == kpidIsAnti)
    {
        prop = false;
        prop.Detach(value);
        return S_OK;
    }

    if(NULL != DirItems)
    {
        const CDirItem &dirItem = (*DirItems)[index];
        switch(propID)
        {
        case kpidPath:  prop = dirItem.Name; break;
        case kpidIsDir:  prop = dirItem.isDir(); break;
        case kpidSize:  prop = dirItem.Size; break;
        case kpidAttrib:  prop = dirItem.Attrib; break;
        case kpidCTime:  prop = dirItem.CTime; break;
        case kpidATime:  prop = dirItem.ATime; break;
        case kpidMTime:  prop = dirItem.MTime; break;
        }
    }
    prop.Detach(value);
    return S_OK;
}

HRESULT CArchiveUpdateCallback::Finilize()
{
    if (m_NeedBeClosed)
    {
        //PrintNewLine();
        m_NeedBeClosed = false;
    }
    return S_OK;
}

static void GetStream2(const wchar_t *name)
{
    //PrintString("Compressing  ");
    //if (name[0] == 0)
    //    name = kEmptyFileAlias;
    //PrintString(name);
    PCLINT_UNUSED_ARG(name);
}
/*lint -save -e429*/
STDMETHODIMP CArchiveUpdateCallback::GetStream(UInt32 index, ISequentialInStream **inStream)
{
    RINOK(Finilize());

    const CDirItem &dirItem = (*DirItems)[index];
    GetStream2(dirItem.Name);

    if (dirItem.isDir())
        return S_OK;

    {
        CInFileStream *inStreamSpec = new CInFileStream;
        CMyComPtr<ISequentialInStream> inStreamLoc(inStreamSpec);
        UString path = DirPrefix + dirItem.FullPath;
        if (!inStreamSpec->Open(path))
        {
            DWORD sysError = ::GetLastError();
            FailedCodes.Add(sysError);
            FailedFiles.Add(path);
            // if (systemError == ERROR_SHARING_VIOLATION)
            {
                //PrintNewLine();
                //PrintError("WARNING: can't open file");
                // PrintString(NError::MyFormatMessageW(systemError));
                return S_FALSE;
            }
            // return sysError;
        }
        *inStream = inStreamLoc.Detach();
    }
    return S_OK;
}
/*lint -restore*/

STDMETHODIMP CArchiveUpdateCallback::SetOperationResult(Int32 /* operationResult */)
{
    m_NeedBeClosed = true;
    return S_OK;
}

STDMETHODIMP CArchiveUpdateCallback::GetVolumeSize(UInt32 index, UInt64 *size)
{
    if (VolumesSizes.Size() == 0)
        return S_FALSE;
    if (index >= (UInt32)VolumesSizes.Size())
        index = VolumesSizes.Size() - 1;
    *size = VolumesSizes[index];
    return S_OK;
}

/*lint -e429*/
STDMETHODIMP CArchiveUpdateCallback::GetVolumeStream(UInt32 index, ISequentialOutStream **volumeStream)
{
    wchar_t temp[16];
    ConvertUInt32ToString(index + 1, temp);
    UString res = temp;
    while (res.Length() < 2)
        res = UString(L'0') + res;
    UString fileName = VolName;
    fileName += L'.';
    fileName += res;
    fileName += VolExt;
    COutFileStream *streamSpec = new COutFileStream;
    CMyComPtr<ISequentialOutStream> streamLoc(streamSpec);
    if (!streamSpec->Create(fileName, false))
        return ::GetLastError();
    *volumeStream = streamLoc.Detach();
    return S_OK;
}
/*lint +e429*/

STDMETHODIMP CArchiveUpdateCallback::CryptoGetTextPassword2(Int32 *passwordIsDefined, BSTR *password)
{
    if (!PasswordIsDefined)
    {
        if (AskPassword)
        {
            // You can ask real password here from user
            // Password = GetPassword(OutStream);
            // PasswordIsDefined = true;
            //PrintError("Password is not defined");
            return E_ABORT;
        }
    }
    *passwordIsDefined = BoolToInt(PasswordIsDefined);
    return StringToBstr(Password, password);
}

//#define NT_CHECK_FAIL_ACTION printf("Unsupported Windows version"); return 1;
/*lint -e429*/
int UnCompress(AString strPak, AString strName,Update_UncompressProgressCB pfnUnZipCB,void* pData)
{
    int iRet = ERROR_OK;
    NWindows::NDLL::CLibrary lib;
    if (!lib.Load(TEXT(kDllName)))
    {
        return ERROR_LOAD_7Z_FIALD;
    }
    CreateObjectFunc createObjectFunc = (CreateObjectFunc)lib.GetProc("CreateObject");
    if (createObjectFunc == 0)
    {
        return ERROR_CREATEOBJECT_FIALD;
    }

    UString archiveName = GetUnicodeString(strPak);
    UString uncompressName = GetUnicodeString(strName);

    NFile::NFind::CFileInfoW fi;
    if (!fi.Find(uncompressName))
    {
        NFile::NDirectory::CreateComplexDirectory(uncompressName);
    }

    CMyComPtr<IInArchive> archive;
    if (createObjectFunc(&CLSID_CFormat7z, &IID_IInArchive, (void **)&archive) != S_OK)
    {
        return ERROR_GETOBJECT_FIALD;
    }

    CInFileStream *fileSpec = new CInFileStream;
    CMyComPtr<IInStream> file = fileSpec;

    if (!fileSpec->Open(archiveName))
    {
        return ERROR_OPENARCHIVE_FAILD;
    }

    {
        CArchiveOpenCallback *openCallbackSpec = new CArchiveOpenCallback;
        CMyComPtr<IArchiveOpenCallback> openCallback(openCallbackSpec);
        openCallbackSpec->PasswordIsDefined = false;
        // openCallbackSpec->PasswordIsDefined = true;
        // openCallbackSpec->Password = L"1";

        if (archive->Open(file, 0, openCallback) != S_OK)
        {
            return ERROR_OPENARCHIVE_FAILD;
        }
    }

    // Extract command
    CArchiveExtractCallback *extractCallbackSpec = new CArchiveExtractCallback;
    CMyComPtr<IArchiveExtractCallback> extractCallback(extractCallbackSpec);
    extractCallbackSpec->Init(archive, uncompressName); // second parameter is output folder path
    extractCallbackSpec->PasswordIsDefined = false;

    if (NULL == pfnUnZipCB)
    {
        iRet = ERROR_EXPARAM_WARN;
    }
    extractCallbackSpec->m_pfnCB = pfnUnZipCB;
    extractCallbackSpec->m_pData = pData;
    // extractCallbackSpec->PasswordIsDefined = true;
    // extractCallbackSpec->Password = L"1";
    HRESULT result = archive->Extract(NULL, (UInt32)(Int32)(-1), false, extractCallback);
    if (result != S_OK)
    {
        return ERROR_EXTRACT_FAILD;
    }

    return iRet;
}
/*lint +e429*/


int TraverseFloder(AString strFileName, AString strFolderPath, CObjectVector<AString>& fileList)
{
    //_finddata_t stFileInfo;
    NFile::NFind::CFileInfo fi;
    NFile::NFind::CFindFile TEST;

    AString strFindPath;
    if (strFolderPath.IsEmpty())
    {
        strFindPath = strFileName + "\\*";
    }
    else
    {
        strFindPath = strFolderPath + "\\" + strFileName +"\\*";
    }
    UString ustrFindPath = GetUnicodeString(strFindPath);
    bool bRet = TEST.FindFirst(ustrFindPath, fi);
    if (!bRet)
    {
        return -1;
    }

    while (TEST.FindNext(fi))
    {
        if (fi.IsDots() || (0 == (fi.Name.Compare(L".svn"))))
        {
            continue;
        }
        if (fi.IsDir())
        {
            AString strNewPath = strFileName + "\\" + GetAnsiString(fi.Name);
            TraverseFloder(strNewPath, strFolderPath, fileList);
        }
        else
        {
            AString strFilePath = strFileName + "\\" + GetAnsiString(fi.Name);
            fileList.Add(strFilePath);
        }
    }

    TEST.Close();

    return 0;
}

/*lint -e429*/
int Compress(AString strPak, AString strName,Update_CompressProgressCB pfnZipCB,void* pData)
{
    int iRet = ERROR_OK;
    NWindows::NDLL::CLibrary lib;
    if (!lib.Load(TEXT(kDllName)))
    {
        return ERROR_LOAD_7Z_FIALD;
    }
    CreateObjectFunc createObjectFunc = (CreateObjectFunc)lib.GetProc("CreateObject");
    if (createObjectFunc == 0)
    {
        return ERROR_CREATEOBJECT_FIALD;
    }

    UString archiveName = GetUnicodeString(strName);

    AStringVector vecFilelist;
    CObjectVector<CDirItem> dirItems;

    AString strFolderPath;
    AString strFileName;
    int iRight = strPak.ReverseFind('\\');
    
    if (-1 == iRight)
    {
        strFolderPath.Empty();
        strFileName = strPak;
    }
    else
    {
        strFolderPath = strPak.Left(iRight);
        strFileName = strPak.Right(strPak.Length() - iRight -1);
    }

    iRet = TraverseFloder(strFileName, strFolderPath, vecFilelist);
    if (0 != iRet)
    {
        return ERROR_FOLEDER_FAILD;
    }
    
    for (int i = 0; i < vecFilelist.Size();i++)
    {
        CDirItem di;
        UString name;
        if (strFolderPath.IsEmpty())
        {
            name = GetUnicodeString(vecFilelist[i]);
        }
        else
        {
            name = GetUnicodeString(strFolderPath + "\\" + vecFilelist[i]);
        }
        

        NFile::NFind::CFileInfoW fi;
        if (!fi.Find(name))
        {
            return ERROR_FINDFILE_FAILD;
        }

        di.Attrib = fi.Attrib;
        di.Size = fi.Size;
        di.CTime = fi.CTime;
        di.ATime = fi.ATime;
        di.MTime = fi.MTime;
        di.Name = GetUnicodeString(vecFilelist[i]);
        di.FullPath = name;
        dirItems.Add(di);
    }
    vecFilelist.Clear();

    COutFileStream *outFileStreamSpec = new COutFileStream;
    CMyComPtr<IOutStream> outFileStream = outFileStreamSpec;
    if (!outFileStreamSpec->Create(archiveName, true))
    {
        //PrintError("can't create archive file");
        return ERROR_CREATEARCHIVE_FAILD;
    }

    CMyComPtr<IOutArchive> outArchive;
    if (createObjectFunc(&CLSID_CFormat7z, &IID_IOutArchive, (void **)&outArchive) != S_OK)
    {
        //PrintError("Can not get class object");
        return ERROR_GETOBJECT_FIALD;
    }

    CArchiveUpdateCallback *updateCallbackSpec = new CArchiveUpdateCallback;
    CMyComPtr<IArchiveUpdateCallback2> updateCallback(updateCallbackSpec);
    updateCallbackSpec->Init(&dirItems);
    // updateCallbackSpec->PasswordIsDefined = true;
    // updateCallbackSpec->Password = L"1";
    updateCallbackSpec->m_pfnZipCB = pfnZipCB;
    updateCallbackSpec->m_pData = pData;

    /**/
    {
        const wchar_t *names[] =
        {
            L"s",
            L"x"
        };
        const int kNumProps = sizeof(names) / sizeof(names[0]);
        NWindows::NCOM::CPropVariant values[kNumProps] =
        {
            false,    // solid mode OFF
            (UInt32)9 // compression level = 9 - ultra
        };
        CMyComPtr<ISetProperties> setProperties;
        outArchive->QueryInterface(IID_ISetProperties, (void **)&setProperties);
        if (!setProperties)
        {
            //PrintError("ISetProperties unsupported");
            return ERROR_NOT_SUPPORT;
        }
        /*lint -e1547 -e1552*/
        RINOK(setProperties->SetProperties(names, values, kNumProps));
        /*lint +e1547 +e1552*/
    }

    HRESULT result = outArchive->UpdateItems(outFileStream, dirItems.Size(), updateCallback);
    updateCallbackSpec->Finilize();
    if (result != S_OK)
    {
        //PrintError("Update Error");
        return ERROR_COMPRESS_FAILD;
    }
    
    if (updateCallbackSpec->FailedFiles.Size() != 0)
        return ERROR_HASFILES_FAILD;

    return ERROR_OK;
}
/*lint +e429*/

