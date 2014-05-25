#include "StdAfx.h"
#include "MyQueryFile.h"


CMyQueryFile::CMyQueryFile(FILE_TABLE_MAP * FileTableMap)
{
	this->FileTableMap=FileTableMap;
}

CMyQueryFile::~CMyQueryFile(void)
{
}
inline bool CMyQueryFile::JudgeInFileName(CString InputFileName,CString FileName)
{//找到字串就说明符合条件哦
	if (-1!=FileName.Find(InputFileName))
	{
		return true;
	}else
	{
		return false;
	}
}

void CMyQueryFile::InitQuery()
{//初始化中间查询表
	//初始化最终查询结果表
	this->m_ResTableVector=new RES_TABLE_VECTOR;
	//这个在析构函数中也要释放吧
}

//这支撑inlien  这样这个调用多的时候效率比较高
inline void CMyQueryFile::GetRealFilePath(
	DWORDLONG CurrFileNumber,
	CString & RealFilePath,
	size_t CountTableMap/*这个是表示在某个映射表中 为什么不直接将映射表传进来呢？ 重载一下*/
	)//这个必须要这样的参数才能进行递归
{/*
 递归算法求到完整路径
 */
	//递归算法，讨厌的啦
	/**
	递归的求出文件的绝对路径，将路径存在OutRealPath
	*/
	STD_DIV_MAP::iterator ItrStart=this->FileTableMap->at(CountTableMap).m_StdDivMap->find(CurrFileNumber);
	if (ItrStart==this->FileTableMap->at(CountTableMap).m_StdDivMap->end())
	{
		return;
	}
	if (OUT_ROOT_NUMBER==ItrStart->second.ParentFileNumber)
	{
		RealFilePath=ItrStart->second.CurrFileName+RealFilePath;
		return;
	}
	RealFilePath=ItrStart->second.CurrFileName+RealFilePath;
	CurrFileNumber=ItrStart->second.ParentFileNumber;
	this->GetRealFilePath(CurrFileNumber,RealFilePath,CountTableMap);//递归得到文件的完整路径
}

inline void CMyQueryFile::GetRealFilePath(
	DWORDLONG CurrFileNumber,
	CString & RealFilePath,
	STD_DIV_MAP::iterator InputItr,size_t CountTableMap)
{

	RealFilePath=InputItr->second.CurrFileName+RealFilePath;
	CurrFileNumber=InputItr->second.ParentFileNumber;
	this->GetRealFilePath(CurrFileNumber,RealFilePath,CountTableMap);//递归得到文件的完整路径
}

inline void CMyQueryFile::GetRealFilePath(
	DWORDLONG CurrFileNumber, 
	CString & RealFilePath,
	size_t CountTableMap,
	STD_DIV_MAP::iterator &ItrFileInfo)
{
	STD_DIV_MAP::iterator ItrStart=this->FileTableMap->at(CountTableMap).m_StdDivMap->find(CurrFileNumber);
	ItrFileInfo=ItrStart;//将第一次查询得到的迭代器取出来
	if (ItrStart==this->FileTableMap->at(CountTableMap).m_StdDivMap->end())
	{
		return;
	}
	if (OUT_ROOT_NUMBER==ItrStart->second.ParentFileNumber)
	{
		RealFilePath=ItrStart->second.CurrFileName+RealFilePath;
		return;
	}
	RealFilePath=ItrStart->second.CurrFileName+RealFilePath;
	CurrFileNumber=ItrStart->second.ParentFileNumber;
	this->GetRealFilePath(CurrFileNumber,RealFilePath,CountTableMap);//递归得到文件的完整路径
}

RES_TABLE_VECTOR * CMyQueryFile::GetPointRealPathTable()
{
	return this->m_ResTableVector;
}

void CMyQueryFile::GetFileFormTableByNameEx(CString InputFileName)
{
	this->m_ResTableVector->clear();
	size_t CountDivVec=0;
	STD_DIV_MAP::iterator ItrBegin;
	CString DivName;
	CString RealFilePath;
	while (CountDivVec<(this->FileTableMap->size()))
	{
		DivName=this->FileTableMap->at(CountDivVec).DivName;
		DivName.Delete(0,4);
		DivName+=_T("\\");
		ItrBegin=this->FileTableMap->at(CountDivVec).m_StdDivMap->begin();
		while (ItrBegin!=this->FileTableMap->at(CountDivVec).m_StdDivMap->end())
		{
			if (ItrBegin->second.CurrFileName.Find(InputFileName)!=-1)
			{
				this->GetRealFilePath(ItrBegin->first,RealFilePath,ItrBegin,CountDivVec);
				RealFilePath=DivName+RealFilePath;
				REAL_RES_TABLE TempRealNode(RealFilePath,ItrBegin);
				this->m_ResTableVector->push_back(TempRealNode);
				//	MessageBox(NULL,RealFilePath,_T("文件路径"),MB_OK);
				RealFilePath=_T("");//每次使用完要清空上得到的文件路径
			}
			++ItrBegin;
		}
		++CountDivVec;
	}
}
