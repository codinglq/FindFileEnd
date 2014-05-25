#include "StdAfx.h"
#include "MyCreateTable.h"
CMyCreateTable::~CMyCreateTable(void)
{
}
CMyCreateTable::CMyCreateTable(FILE_TABLE_MAP *FileTableMap)
{
	this->FileTableMap=FileTableMap;
}

void CMyCreateTable::SelectDiv()
{
	/*
	挑选出NTFS格式的驱动器
	*/
	WCHAR StartDiv=START_SCANF_DIV;
	WCHAR DivFileSystem[MAX_DIV_FILE_SYSTEM_STR_LEN]={};
	short res;
	WCHAR DeviceName[4]=_T("C:\\");
	while (StartDiv<=END_SCANF_DIV)
	{
		DeviceName[0]=StartDiv;
		res=GetVolumeInformationW(DeviceName,
			NULL,
			0,
			NULL,
			NULL,
			NULL,
			DivFileSystem,
			MAX_PATH);//获取磁盘信息
		if (res&&(!wcscmp(DivFileSystem,TRUE_FILE_SYSTEM)))
		{//以后要用到初始化usn日志的驱动器字符串格式是 \\.\C:这样的格式
			CString StrTempDiv(StartDiv);
			this->m_DivList.push_back(_T("\\\\.\\")+StrTempDiv+=_T(":"));
		}
		++StartDiv;
	}
}

void CMyCreateTable::GetDivHandle()
{
	/*
	在这个函数中 一直做到初始化USN日志就可以
	*/
	size_t count=0;
	while (count<this->m_DivList.size())
	{
		HANDLE handDevice/*驱动器句柄就放在这里了*/=CreateFile(this->m_DivList[count],//这噶哒就需要这种格式
			GENERIC_READ|GENERIC_WRITE,
			FILE_SHARE_READ|FILE_SHARE_WRITE,
			NULL,OPEN_EXISTING,
			FILE_ATTRIBUTE_READONLY,
			NULL);
		if (INVALID_HANDLE_VALUE!=handDevice)
		{
			wcout<<"获取驱动器句柄成功"<<endl;
		}
		//初始化USN日志
		DWORD br;
		CREATE_USN_JOURNAL_DATA mycujd;
		mycujd.AllocationDelta=0;
		mycujd.MaximumSize=0;

		unsigned short int st=DeviceIoControl(handDevice,
			FSCTL_CREATE_USN_JOURNAL,
			&mycujd,
			sizeof(mycujd),
			NULL,
			0,
			&br,
			NULL);

		if (false!=st)
		{//初始化USN
			DIV_HANDLE_NODE TempNode(this->m_DivList[count],handDevice);
			this->m_DivHandleTable.push_back(TempNode);
		}
		++count;
	}
	//............初始化完成的USN驱动器句柄就放在m_DivHandleTable中  以供后面扫描文件使用
}

void CMyCreateTable::CreateMaxFileTable(unsigned int *progress)
{
	this->SelectDiv();
	this->GetDivHandle();
	int Tmp=100/(this->m_DivHandleTable.size());
	size_t CountDiv=0;
	while (CountDiv<(this->m_DivHandleTable.size()))
	{
		MAX_TABLE_MAP MaxTableMap;
		this->CreateMinFileTable(&(this->m_DivHandleTable)[CountDiv],MaxTableMap);
		this->FileTableMap->push_back(MaxTableMap);
		CloseHandle(m_DivHandleTable[CountDiv].m_Handle);
		(*progress)=(CountDiv+1)*Tmp;
		++CountDiv;
	}
}

inline void CMyCreateTable::CreateMinFileTable(DIV_HANDLE_NODE *DivHandleNode, MAX_TABLE_MAP &MaxTableMap)
{
	MaxTableMap.DivName=DivHandleNode->DivName;
	STD_DIV_MAP * TempFileMap=new STD_DIV_MAP;
	MaxTableMap.m_StdDivMap=TempFileMap;
	//初始化结束
	HANDLE handDevice=DivHandleNode->m_Handle;
	unsigned short res;
	DWORD dr;
	USN_JOURNAL_DATA myujd;
	res=DeviceIoControl(handDevice,FSCTL_QUERY_USN_JOURNAL,NULL,0,&myujd,sizeof(myujd),&dr,NULL);//在这里获取USN日志信息
	if (res)
	{
		//这些cout可以不要的啦
		//wcout<<"USN日志信息获取成功"<<endl;
	}else
	{
		//wcout<<"USN日志获取失败"<<GetLastError()<<endl;
	}
	MFT_ENUM_DATA med;//用MFT_ENUM_DATA 获取USN中的文件信息
	med.StartFileReferenceNumber=0;
	med.LowUsn=0;
	med.HighUsn=myujd.NextUsn;

	WCHAR buffer[MAX_BUFFER_LEN];
	DWORD usnDataSize;
	PUSN_RECORD UsnRecord;//这个类型就是USN_RECORD的指针类型

	while (DeviceIoControl(handDevice,FSCTL_ENUM_USN_DATA,&med,sizeof(med),buffer,MAX_BUFFER_LEN,&usnDataSize,NULL))
	{
		DWORD dwRetBytes=usnDataSize-sizeof(USN);
		//找到第一个USN记录
		UsnRecord=(PUSN_RECORD)(((PCHAR)buffer)+sizeof(USN));
		while (dwRetBytes>0)
		{
			//输出获取到的信息
			const int strLen=UsnRecord->FileNameLength;
			if (UsnRecord->FileAttributes&FILE_ATTRIBUTE_DIRECTORY)//判断文件类型  若是文件夹，那么就要在后面加上\\表示这是文件夹，以后用来打开文件路径
			{
				CString StrFileName(UsnRecord->FileName, UsnRecord->FileNameLength/2);
				TempInfoMapNode.CurrFileName=StrFileName;
				TempInfoMapNode.CurrFileName+=_T("\\");//加上斜线处理文件夹
				TempInfoMapNode.ParentFileNumber=UsnRecord->ParentFileReferenceNumber;
				TempFileMap->insert(make_pair(UsnRecord->FileReferenceNumber,TempInfoMapNode));
				//(*TempFileMap)[UsnRecord->FileReferenceNumber]=TempInfoMapNode;
			}else
			{
				CString StrFileName(UsnRecord->FileName, UsnRecord->FileNameLength/2);
				TempInfoMapNode.CurrFileName=StrFileName;
				TempInfoMapNode.ParentFileNumber=UsnRecord->ParentFileReferenceNumber;
				TempFileMap->insert(make_pair(UsnRecord->FileReferenceNumber,TempInfoMapNode));
				//(*TempFileMap)[UsnRecord->FileReferenceNumber]=TempInfoMapNode;
			}
			DWORD recordLen=UsnRecord->RecordLength;
			dwRetBytes-=recordLen;
			UsnRecord=(PUSN_RECORD)(((PCHAR)UsnRecord)+recordLen);
		}
		med.StartFileReferenceNumber=*(USN*)&buffer;
	}
}
