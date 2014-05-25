#pragma once
#include "stdafx.h"
#include <afxwin.h>//这个头文件特别的讨厌必须放在下面的某个头文件的前面才行  FUCK
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <WinIoCtl.h>
#include <map>
#include <vector>
//#include "MyCreateTable.h"

#define LQMAIN int main()
#define  START {
#define  END }

#define OUT_ROOT_NUMBER 1407374883553285 //这个是驱动器根目录的索引号
#define MAX_DIV_FILE_SYSTEM_STR_LEN 6
#define START_SCANF_DIV 'A'
#define END_SCANF_DIV 'Z'
#define TRUE_FILE_SYSTEM _T("NTFS")
#define  MAX_BUFFER_LEN 65536
using namespace std;
//文件信息节点
typedef struct FILE_INFO_VEC_NODE 
{
	CString CurrFileName;
	DWORDLONG CurrFileNumber;
};
//每个盘对应的表
typedef vector<FILE_INFO_VEC_NODE> STD_TABLE_VEC;
//封装成结构体好吗？
typedef struct MAX_TABLE_VEC
{
	CString DivName;
	STD_TABLE_VEC *m_StaTable;
};
//整个驱动器队列对应的表
typedef vector<MAX_TABLE_VEC> FILE_TABLE_VEC;
//.....

//文件信息节点
typedef struct FILE_INFO_MAP_NODE
{
	CString CurrFileName;
	DWORDLONG ParentFileNumber;//当前文件的父及目录
	//unsigned short FileAttribute;//表示着文件类型 文件夹什么的
	// 	FILE_INFO_MAP_NODE()
	// 	{
	// 	//	this->FileAttribute=0;
	// 	}
};
//随意的定义一下
typedef DWORDLONG FILE_INDEX;
typedef DWORDLONG FILE_NUMBER;
//每个盘对应的文件映射表
typedef map<FILE_NUMBER,FILE_INFO_MAP_NODE> STD_DIV_MAP;
//整个驱动器队列对应的文件映射表
typedef struct MAX_TABLE_MAP
{
	CString DivName;
	STD_DIV_MAP * m_StdDivMap;
};

typedef vector<MAX_TABLE_MAP> FILE_TABLE_MAP;
//....
//中间查询结果表  中间查询结果表！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
typedef vector<DWORDLONG> TEMP_RES_TABLE;
//每个盘对应的查询结果表
typedef struct MAX_TEMP_RES_TABLE
{
	CString DivName;
	TEMP_RES_TABLE * m_TempResTable;
	//强制使用构造函数
	MAX_TEMP_RES_TABLE(CString DivName,TEMP_RES_TABLE * TempResTable)//强制使用构造函数
	{
		this->DivName=DivName;
		this->m_TempResTable=TempResTable;
	}
	~MAX_TEMP_RES_TABLE()
	{
		//暂时先不用了这个析构函数
		//	delete this->m_TempResTable;
		//这里不能delete 这个变量 因为是只有初始化的时候才给分配内存 要是现在delete了  那么下次操作就完蛋了
	}
};
//整个驱动器队列对应的中间查询表
typedef vector<MAX_TEMP_RES_TABLE> RES_TABLE;
//......................


//驱动器句柄表
//typedef vector<HANDLE> DIV_HANDLE_TABLE;

typedef struct DIV_HANDLE_NODE//来个构造函数尝尝
{
	CString DivName;//驱动器名字
	HANDLE m_Handle;//驱动器句柄

	DIV_HANDLE_NODE(CString DivName,HANDLE DivHandle)
	{
		this->DivName=DivName;
		this->m_Handle=DivHandle;
	}
};

typedef vector<DIV_HANDLE_NODE> DIV_HANDLE_TABLE;// 句柄队列（表）

//关于查询结果表的相关定义  最后查询结果表！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！

typedef struct REAL_RES_TABLE 
{
	CString RealFilePath;
	STD_DIV_MAP::iterator ItrFileInfo;

	REAL_RES_TABLE(CString RealPath,STD_DIV_MAP::iterator ItrFileInfo)
	{//强制使用构造函数
		this->ItrFileInfo=ItrFileInfo;
		this->RealFilePath=RealPath;
	}
};
typedef vector<REAL_RES_TABLE> RES_TABLE_VECTOR;//最后的查询表

//定义多线程函数使用的参数

typedef struct THREAD_PARAM 
{
	size_t *m_CtrlThread;
	STD_TABLE_VEC *m_pStdTableVec;//单个驱动器的文件映射表
	TEMP_RES_TABLE *m_pTempResTable;//单个驱动器的中间查询表
	CString m_InputName;//输入的文件名字
	THREAD_PARAM(size_t *CtrlThread,STD_TABLE_VEC *pStdTableVec,TEMP_RES_TABLE *pTempResTable,CString InputName)//强制构造函数
	{
		this->m_CtrlThread=CtrlThread;
		this->m_InputName=InputName;
		this->m_pStdTableVec=pStdTableVec;
		this->m_pTempResTable=pTempResTable;
	}
};

typedef vector<THREAD_PARAM*> THREAD_PARAM_VEC;

void InitMyListCtrl(CListCtrl * m_ListCtrl);

void SetMyListCtrl(CListCtrl * m_ListCtrl, RES_TABLE_VECTOR *FileRealRes);

