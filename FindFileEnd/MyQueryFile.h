#pragma once
#include "MyDefFile.h"
#include <process.h>
class CMyQueryFile
{
public:
	CMyQueryFile(FILE_TABLE_MAP * FileTableMap);//必须使用这个构造函数
	~CMyQueryFile(void);
public:
	void InitQuery();
	void GetFileFormTableByNameEx(CString InputFileName);
	RES_TABLE_VECTOR * GetPointRealPathTable();
private:
	void GetRealFilePath(
		DWORDLONG CurrFileNumber,
		CString & RealFilePath,
		size_t CountTableMap);//这个有4个重载的版本
	void GetRealFilePath(
		DWORDLONG CurrFileNumber,
		CString & RealFilePath,
		STD_DIV_MAP * StdDivMap);
	void GetRealFilePath(
		DWORDLONG CurrFileNumber,
		CString & RealFilePath,
		size_t CountTableMap,
		STD_DIV_MAP::iterator &ItrFileInfo);//可以让这个重载函数调用前两个重载函数    ma ?

	void GetRealFilePath(
		DWORDLONG CurrFileNumber,
		CString & RealFilePath,
		STD_DIV_MAP::iterator InputItr,size_t CountTableMap);

	bool JudgeInFileName(CString InputFileName,CString FileName);//判断输入的串  是否和某个文件名相关
private:
	FILE_TABLE_MAP * FileTableMap;//整个硬盘的文件索引映射表
	FILE_TABLE_VEC * FileTableVec;//整个硬盘的表
	RES_TABLE m_MaxTempResTable;//单个驱动器对应的中间查询结果表

	RES_TABLE_VECTOR *m_ResTableVector;//这个里面不用区分盘符！
};

