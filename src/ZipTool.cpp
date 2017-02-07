
#include "ZipTools.h"
#include <io.h>
#include <winsock2.h>
#if (defined _WIN32 || defined _WIN64)
    #include <tchar.h>
    #include <direct.h> 
    #include <winsock2.h>
    #include <windows.h> 
#else
    #include <dirent.h>
    #include <sys/types.h>
    #include <string.h>
#endif

#define BUF_LEN     16384
#define PATH_LEN    1024    //默认文件名长度

CZipTools::CZipTools()
: m_FolderPath("")
, m_FolderName("")
{
}

CZipTools::~CZipTools()
{
}

int CZipTools::CompressFile(zipFile& zFp, std::string strRelPath, std::string strFullPath)
{
    char szbuf[BUF_LEN];
    unsigned int iReadLen;
    int iRet = ZIP_OK;
    zip_fileinfo zFileInfo;

    zipOpenNewFileInZip64(zFp, strRelPath.c_str(), &zFileInfo, NULL, 0, NULL, 0, NULL, Z_DEFLATED, Z_DEFAULT_COMPRESSION, 1);
    FILE* fp = fopen(strFullPath.c_str(), "rb");
    do
    {
        iReadLen = fread(szbuf, 1, BUF_LEN, fp);
        if (ferror(fp))
        {
            zipCloseFileInZip(zFp);
            iRet = ZIP_STATE_IO_ERROR;
            break;
        }

        iRet = zipWriteInFileInZip(zFp, szbuf, iReadLen);
        if (iRet != Z_OK)
        {
            zipCloseFileInZip(zFp);
            iRet = ZIP_STATE_IO_ERROR;
            break;
        }
    } while (!feof(fp));

    fclose(fp);
    zipCloseFileInZip(zFp);
    return iRet;
}

int CZipTools::Compress(const char* strSrcDir, const char* strDstDir, int iFlag /*= 0*/ )
{
    if (NULL == strSrcDir || NULL == strDstDir)
    {
        return ZIP_STATE_ERR;
    }

    //遍历压缩文件目录
    int iRet = ZIP_OK;
    std::vector<std::string> vctFileList;
    TraverseFloder(strSrcDir, vctFileList);
    if (vctFileList.empty())
    {
        return ZIP_STATE_DIR_IS_NULL;
    }

    //处理压缩选项
    int iOpenMode = APPEND_STATUS_CREATE;
    if (DTK_OK == DTK_AccessFile(strDstDir, DTK_F_OK))
    {
        if (iFlag = ZIP_FILE_REPLACE)
        {
            DTK_DeleteFile(strDstDir);
        }
        else if(iFlag = ZIP_FILE_APPEND)
        {
            iOpenMode = APPEND_STATUS_ADDINZIP;
        }
        else
        {
            //文件存在，并且不允许替换或追加
            return ZIP_STATE_FILE_IS_EXIST; 
        }
    }

    //处理压缩文件路径
    std::string strZipFilePath = strDstDir;
#if (defined _WIN32 || defined _WIN64)
    strZipFilePath = strZipFilePath.substr(0, strZipFilePath.find_last_of("\\") + 1);
#else
    strZipFilePath = strZipFilePath.substr(0, strZipFilePath.find_last_of("/") + 1);
#endif
    if(DTK_OK != DTK_AccessFile(strZipFilePath.c_str(), DTK_F_OK)) 
    { 
        if (false == CreatedMultipleDirectory(strZipFilePath)) 
        { 
            return ZIP_STATE_ERR; 
        } 
    }

    std::string strPackPath = strSrcDir;
#if (defined _WIN32 || defined _WIN64)
    if(strPackPath.at(strPackPath.size() - 1) == '\\') 
    { 
        m_FolderPath = strPackPath.substr(0, strPackPath.size() - 1); 
        m_FolderName = m_FolderPath.substr(m_FolderPath.find_last_of("\\") + 1);
    } 
    else 
    { 
        m_FolderPath = strPackPath; 
        m_FolderName = strPackPath.substr(strPackPath.find_last_of("\\") + 1); 
    }
#else
    if(strPackPath.at(strPackPath.size() - 1) == '/') 
    { 
        m_FolderPath = strPackPath.substr(0, strPackPath.size() - 1); 
        m_FolderName = m_FolderPath.substr(m_FolderPath.find_last_of("/") + 1);
    } 
    else 
    { 
        m_FolderPath = strPackPath; 
        m_FolderName = strPackPath.substr(strPackPath.find_last_of("//") + 1); 
    }
#endif

    try
    {
        zipFile zFp = zipOpen64(strDstDir, iOpenMode);
        for (unsigned int i = 0; i < vctFileList.size(); i ++)
        {
            std::string strRelPath;
            GetRelativePath(vctFileList[i],strRelPath);
            iRet = CompressFile(zFp, strRelPath, vctFileList[i]) ;
            if (iRet != ZIP_OK)
            {
                zipClose(zFp, NULL);
                DTK_DeleteFile(strDstDir);
                break;
            }
        }
        zipClose(zFp, NULL);
    }
    catch (...)
    {
    	iRet = ZIP_STATE_ERR;
    }
   
    return iRet;
}

void CZipTools::GetRelativePath(std::string strFullPath,std::string& strSubString) 
{ 
    strSubString = strFullPath.substr(m_FolderPath.size() - m_FolderName.size());
}

int NetPathToWinPath(std::string strSrc, std::string& strDst)
{
    size_t i;
    for (i = 0; i < strSrc.size(); i++)
    {
        if (strSrc[i] == '/')
        {
            strDst.append("\\");
        }
        else
        {
            char szChar[2] = {0};
            sprintf_s(szChar, 2, "%c", strSrc[i]);
            strDst.append(szChar);
        }
    }
    return 0;
}

int CZipTools::UnCompress(const char* strSrcDir, const char* strDstDir, int iFlag /*= 1*/ )
{
    if (NULL == strSrcDir || NULL == strDstDir)
    {
        return ZIP_STATE_ERR;
    }

    int iRet = ZIP_OK;
    if (DTK_OK == DTK_AccessFile(strSrcDir, DTK_F_OK))
    {
        return ZIP_STATE_FILE_NOT_FOUND;
    }

    std::string strZipFilePath = strDstDir; 
    if(DTK_OK == DTK_AccessFile(strZipFilePath.c_str(), DTK_F_OK))
    { 
        if (false == CreatedMultipleDirectory(strZipFilePath)) 
        { 
            return ZIP_STATE_ERR; 
        } 
    } 

    try
    {
        char szBuf[BUF_LEN] = {0};
        unsigned int iReadLen = 0;
        unzFile unzFp;

        do 
        {
            unzFp = unzOpen64(strSrcDir);
            if (NULL == unzFp)
            { 
                iRet = ZIP_STATE_IO_ERROR;//打开压缩文件失败
                break;
            }

            unz_global_info64 unzInfo;
            if (unzGetGlobalInfo64(unzFp, &unzInfo) != UNZ_OK)
            {
                iRet = ZIP_STATE_IO_ERROR;//读取压缩文件头信息失败
                break;
            }

            for (int i = 0; i < unzInfo.number_entry && iRet == ZIP_OK; i++)
            {
                char szFilePath[MAX_PATH] = {0};
                unz_file_info64 unzfileinfo;
                unzGetCurrentFileInfo64(unzFp, &unzfileinfo, szFilePath, sizeof(szFilePath), NULL, 0, NULL, 0);//获取文件
                
                std::string strOutFilePath;
                std::string strTempFilePath;
#if (defined _WIN32 || defined _WIN64)
                strTempFilePath = strZipFilePath + "\\" + std::string(szFilePath);
                NetPathToWinPath(strTempFilePath, strOutFilePath);
                strTempFilePath = strOutFilePath.substr(0, strOutFilePath.find_last_of("\\"));
#else
                strTempFilePath = strZipFilePath + "/" + std::string(szFilePath);
                strTempFilePath = strOutFilePath.substr(0, strOutFilePath.find_last_of("/"));                
#endif
                if (DTK_OK == DTK_AccessFile(strTempFilePath.c_str(), DTK_F_OK))
                {
                    if (false == CreatedMultipleDirectory(strTempFilePath)) 
                    {
                        break; 
                    } 
                }
                
                // 判断文件是否存在，如果存在，根据设置状态进行处理
                if (DTK_OK == DTK_AccessFile(strOutFilePath.c_str(), DTK_F_OK))
                {
                    if (iFlag == ZIP_FILE_SKIP)
                    {
                        unzGoToNextFile(unzFp);
                        continue;
                    }
                    DTK_DeleteFile(strOutFilePath.c_str());// 替换，先删除
                }
                // 读取压缩文件块
                unzOpenCurrentFile(unzFp);
                FILE* fp = fopen(strOutFilePath.c_str(), "wb");
                while(1)
                {
                    iReadLen = unzReadCurrentFile(unzFp, szBuf, BUF_LEN);
                    if (iReadLen < 0)
                    {
                        iRet = ZIP_STATE_IO_ERROR;
                        // 读文件错误
                        break;
                    }
                    if (iReadLen == 0)
                    {
                        break;
                    }
                    fwrite(szBuf, 1, iReadLen, fp);
                    if (ferror(fp))
                    {
                        iRet = ZIP_STATE_IO_ERROR;
                        break;
                    }
                }
                fclose(fp);
                unzCloseCurrentFile(unzFp);
                unzGoToNextFile(unzFp);
            }
        } while (0);
 
        unzClose(unzFp);
    }
    catch(...)
    {
        iRet = ZIP_STATE_ERR;
    }

    return iRet;
}

int CZipTools::TraverseFloder(const std::string& strFolderPath, std::vector<std::string>& vctFileList)
{
#if (defined _WIN32 || defined _WIN64)
    _finddata_t stFileInfo;
    std::string strFindPath = strFolderPath + "\\*";
    long iHandle = _findfirst(strFindPath.c_str(), &stFileInfo);

    if (iHandle == -1L)
    {
        return ZIP_STATE_ERR;
    }

    do
    {
        if (stFileInfo.attrib & _A_SUBDIR)    
        {
            if( (strcmp(stFileInfo.name, ".") != 0 ) && (strcmp(stFileInfo.name, "..") != 0))  
            {
                std::string strNewPath = strFolderPath + "\\" + stFileInfo.name;
                TraverseFloder(strNewPath, vctFileList);
            }
        }
        else  
        {
            std::string strFilePath = strFolderPath + "\\" + std::string(stFileInfo.name);
            vctFileList.push_back(strFilePath);
        }
    }while (_findnext(iHandle, &stFileInfo) == 0);

    _findclose(iHandle);
#else
    DIR* pDir = NULL;
    struct dirent* pStDirent = NULL;
    char szChildPath[PATH_LEN] = {0};

    pDir = opendir(strFolderPath);
    while ((NULL != pStDirent = readdir(pDir)))
    {
        memset(szChildPath, 0, PATH_LEN);
        sprintf(szChildPath, "%s/%s", strPath, pStDirent->d_name);

        if (pStDirent->d_type & DT_DIR)
        {
            if((strcmp(pStDirent->d_name,".") != 0) && (strcmp(pStDirent->d_name,"..")) == 0)
            {
                TraverseFloder(szChildPath, vctFileList);
            }
        }
        else
        {
            vctFileList.push_back(szChildPath);
        }
    }

#endif

    return ZIP_OK;
}

bool CZipTools::CreatedMultipleDirectory(const std::string& szPath)
{
    std::string strTemp;
    std::string strDir(szPath);
    std::vector<std::string> vPath;

#if (defined _WIN32 || defined _WIN64)
    if (strDir.at(strDir.length()-1) != '\\')
    {
        strDir.append("\\");
    }

    for (size_t i=0; i < strDir.length(); ++i)
    {
        if (strDir.at(i) != '\\') 
        {
            char szTempChar[2] = {0};
            sprintf_s(szTempChar, 2, "%c", strDir.at(i));
            strTemp.append(szTempChar);
        }
        else 
        {
            vPath.push_back(strTemp);
            strTemp.append("\\");
        }
    }
#else
    if (strDir.at(strDir.length()-1) != '/')
    {
        strDir.append("/");
    }

    for (size_t i=0; i < strDir.length(); ++i)
    {
        if (strDir.at(i) != '/') 
        {
            char szTempChar[2] = {0};
            sprintf_s(szTempChar, 2, "%c", strDir.at(i));
            strTemp.append(szTempChar);
        }
        else 
        {
            vPath.push_back(strTemp);
            strTemp.append("/");
        }
    }
#endif

    bool bSuccess = false;
    std::vector<std::string>::const_iterator vIter;
    for (vIter = vPath.begin(); vIter != vPath.end(); vIter++) 
    {
        if (DTK_OK == DTK_AccessFile((*vIter).c_str(), DTK_F_OK))
        {
            bSuccess = false;
            continue;
        }
        if (DTK_MakeDir((*vIter).c_str()) < 0)
        {
            bSuccess = false;
            break;
        }
        bSuccess = true;
    }

    return bSuccess;
}

