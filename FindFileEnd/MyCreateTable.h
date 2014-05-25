#pragma once
#include "MyDefFile.h"
typedef vector<CString> DIV_VEC;
class CMyCreateTable
{
public:
	CMyCreateTable(FILE_TABLE_MAP *FileTableMap);
	~CMyCreateTable(void);
public:
	void CreateMaxFileTable(unsigned int *progress);//建立总表

private:
	void SelectDiv();
	void GetDivHandle();
	void CreateMinFileTable(DIV_HANDLE_NODE *DivHandleNode,//驱动器句柄队列
	MAX_TABLE_MAP &MaxTableMap);//单个盘的快速查找表
	FILE_INFO_MAP_NODE TempInfoMapNode;
private:
	FILE_TABLE_MAP * FileTableMap;
	DIV_VEC m_DivList;
	DIV_HANDLE_TABLE m_DivHandleTable;//驱动器句柄表
};

