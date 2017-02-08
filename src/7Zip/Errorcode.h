
#ifndef __ERRORCODE_H__
#define __ERRORCODE_H__

///错误码定义
#define ERROR_CODE_BASE 0x10000

#define ERROR_OK                    0
#define ERROR_LOAD_7Z_FIALD         (ERROR_CODE_BASE + 1)      //Can not load 7-zip library
#define ERROR_CREATEOBJECT_FIALD    (ERROR_CODE_BASE + 2)      //Can not get CreateObject
#define ERROR_GETOBJECT_FIALD       (ERROR_CODE_BASE + 3)      //Can not get class object
#define ERROR_OPENARCHIVE_FAILD     (ERROR_CODE_BASE + 4)      //Can not open archive
#define ERROR_EXTRACT_FAILD         (ERROR_CODE_BASE + 5)      //Extract Error
#define ERROR_COMPRESS_FAILD        (ERROR_CODE_BASE + 6)      //Compress Error
#define ERROR_FOLEDER_FAILD         (ERROR_CODE_BASE + 7)      //文件夹递归失败
#define ERROR_FINDFILE_FAILD        (ERROR_CODE_BASE + 8)      //找不到指定文件
#define ERROR_CREATEARCHIVE_FAILD   (ERROR_CODE_BASE + 9)      //创建压缩文件失败
#define ERROR_HASFILES_FAILD        (ERROR_CODE_BASE + 10)     //有文件压缩失败
#define ERROR_NOT_SUPPORT           (ERROR_CODE_BASE + 11)     //格式、属性等暂不支持



#define ERROR_EXPARAM_WARN          (ERROR_CODE_BASE + 30)      //解压回调函数指针为空，可继续执行



#endif // __ERRORCODE_H__

