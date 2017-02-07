
#ifndef _ZIP_TOOLS_H_
#define _ZIP_TOOLS_H_

#include <string>
#include <vector>
#include "zip/zip.h"
#include "zip/unzip.h"
#include "dtk/DTK_FileSystem.h"

typedef enum
{
    ZIP_FILE_APPEND     = 0,    //����ļ�����׷�ӣ�ѹ����
    ZIP_FILE_REPLACE    = 1,    //����ļ������滻��ѹ����ѹ����
    ZIP_FILE_EXIT       = 2,    //����ļ������˳���ѹ����
    ZIP_FILE_SKIP       = 3     //����ļ�������������ѹ����
}ZIP_FILE_CODE_E;

typedef enum
{
    ZIP_STATE_OK              = 0,    //�ɹ�
    ZIP_STATE_ERR             = -1,   //����
    ZIP_STATE_FILE_NOT_FOUND  = -2,   //��ѹ�ļ�������
    ZIP_STATE_FILE_IS_EXIST   = -3,   //ѹ���ļ��Ѵ���
    ZIP_STATE_IO_ERROR        = -4,   //IO����
    ZIP_STATE_DIR_IS_NULL     = -5    //ѹ��Ŀ¼�ǿ�Ŀ¼
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
