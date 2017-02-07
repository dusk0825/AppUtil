
#ifndef _ZIP_TOOLS_H_
#define _ZIP_TOOLS_H_

#include <string>
#include <vector>
#include "zip/zip.h"
#include "zip/unzip.h"
#include "dtk/DTK_FileSystem.h"

typedef enum
{
    ZIP_FILE_APPEND     = 0,    //如果文件存在追加（压缩）
    ZIP_FILE_REPLACE    = 1,    //如果文件存在替换（压缩解压缩）
    ZIP_FILE_EXIT       = 2,    //如果文件存在退出（压缩）
    ZIP_FILE_SKIP       = 3     //如果文件存在跳过（解压缩）
}ZIP_FILE_CODE_E;

typedef enum
{
    ZIP_STATE_OK              = 0,    //成功
    ZIP_STATE_ERR             = -1,   //错误
    ZIP_STATE_FILE_NOT_FOUND  = -2,   //解压文件不存在
    ZIP_STATE_FILE_IS_EXIST   = -3,   //压缩文件已存在
    ZIP_STATE_IO_ERROR        = -4,   //IO错误
    ZIP_STATE_DIR_IS_NULL     = -5    //压缩目录是空目录
}ZIP_STATE_CODE_E;

class CZipTools
{
public:
    CZipTools();
    ~CZipTools();

    int Compress(const char* strSrcDir, const char* strDstDir, int iFlag = -1);
    int UnCompress(const char* strSrcDir, const char* strDstDir, int iFlag = 1);

private:
    int TraverseFloder(const std::string& strFolderPath, std::vector<std::string>& vctFileList); 

    bool CreatedMultipleDirectory(const std::string& szPath);
    void GetRelativePath(std::string pFullPath,std::string& pSubString);
    
    int CompressFile(zipFile& zFp, std::string strRelPath, std::string strFullPath);

    std::string	m_FolderPath;
    std::string	m_FolderName;
};

#endif
