
// FindFileEndDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FindFileEnd.h"
#include "FindFileEndDlg.h"
#include "afxdialogex.h"
#include "MyDefFile.h"
#include <process.h>
#include "MyProgressDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//声明一个全局的进程显示对话框

// CFindFileEndDlg 对话框

void ProcThread(void* Param)
{
	CMyCreateTable * pCreate=((PARAM_INPUT*)Param)->m_pCreat;
	unsigned int * Progress=((PARAM_INPUT*)Param)->m_Progress;
	pCreate->CreateMaxFileTable(Progress);
	_endthread();
}

void ProcThreadPog(void * Param)
{
	CMyProgressDlg * Dlg=((POG_PARAM*)Param)->m_Dlg;
	unsigned * Pog=((POG_PARAM*)Param)->m_Pog;
	Dlg->Create(IDD_DIALOG1);
	Dlg->ShowWindow(SW_SHOW);
	Dlg->SetThreadText(_T("读取USN日志中......."));
	int Flg=0;
	while ((*Pog)<=100)
	{
		Dlg->SetPog(Pog);
		if (100==(*Pog))
		{
			++Flg;
		}
		if (Flg)
		{
			_endthread();
		}
	}
}
CFindFileEndDlg::CFindFileEndDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFindFileEndDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	FileTableMap=new FILE_TABLE_MAP;
	FileRealRes=nullptr;
	this->m_CreatTable=new CMyCreateTable(FileTableMap);
	this->m_QueryFile= new CMyQueryFile(FileTableMap);//初始化一下
}

void CFindFileEndDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_InputFileName);
	DDX_Control(pDX, IDC_LIST1, m_ListCrtl);
}

BEGIN_MESSAGE_MAP(CFindFileEndDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CFindFileEndDlg::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT1, &CFindFileEndDlg::OnEnChangeEdit1)
	ON_WM_CHAR()
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CFindFileEndDlg::OnNMRClickList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CFindFileEndDlg::OnNMDblclkList1)
	ON_COMMAND(ID_32771, &CFindFileEndDlg::OnRClickOpenFile)
END_MESSAGE_MAP()


// CFindFileEndDlg 消息处理程序

BOOL CFindFileEndDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	//this->m_CreatTable->CreateMaxFileTable();//初始化查询表
	unsigned int * MyProgress=new unsigned int;
	*MyProgress=1;
	PARAM_INPUT *Param=new PARAM_INPUT;
	Param->m_pCreat=this->m_CreatTable;
	Param->m_Progress=MyProgress;
	_beginthread(ProcThread,0,Param);

	CMyProgressDlg *ProgressDlg=new CMyProgressDlg;
	POG_PARAM *PogParam=new POG_PARAM;
	PogParam->m_Dlg=ProgressDlg;
	PogParam->m_Pog=MyProgress;
	//ProgressDlg->Create()
	//ProgressDlg.DoModal();
	_beginthread(ProcThreadPog,0,PogParam);

	this->m_QueryFile->InitQuery();
	// TODO: 在此添加额外的初始化代码
	InitMyListCtrl(&m_ListCrtl);
	//进程显示对话框
	//ProgressDlg.DoModal();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFindFileEndDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
	RECT EditRec;
	RECT WndRec;
	RECT ListCtrlRec;
	this->GetClientRect(&WndRec);
	this->m_InputFileName.GetClientRect(&EditRec);
	this->m_ListCrtl.GetClientRect(&ListCtrlRec);
	//EditRec.right=WndRec.right;
	//EditRec.left=WndRec.left;
	this->m_InputFileName.MoveWindow(0,0,WndRec.right,25);
	this->m_ListCrtl.MoveWindow(0,25,WndRec.right,WndRec.bottom);
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFindFileEndDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFindFileEndDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	CString InputFileName;
	this->m_InputFileName.GetWindowText(InputFileName);
	if (InputFileName.GetLength()<2)
	{
		MessageBox(_T("文件名字太短了，结果太多了不好显示！"));
	}else
	{
		//MessageBox(InputFileName);
		m_QueryFile->GetFileFormTableByNameEx(InputFileName);
		this->FileRealRes=m_QueryFile->GetPointRealPathTable();
		SetMyListCtrl(&m_ListCrtl,FileRealRes);
		size_t CountFile=FileRealRes->size();
		CString CountStr;
		CountStr.Format(_T("共查找到：%d个文件"),CountFile);
		GetWindowText(InputFileName);
		SetWindowText(InputFileName+=CountStr);
	}
	//GetWindowText(InputFileName);
	//ShellExecute(0,L"open",FileRealRes->at(0).RealFilePath,NULL,NULL,SW_SHOW);//使用系统打开文件
}


void CFindFileEndDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	CString Tmp;
	m_InputFileName.GetWindowText(Tmp);
	SetWindowText(Tmp+=_T(" - Go For It!"));
	// TODO:  在此添加控件通知处理程序代码
}


void CFindFileEndDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{//这个根本不好事
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nChar=='\n')
	{
		OnBnClickedButton1();
	}
	CDialogEx::OnChar(nChar, nRepCnt, nFlags);
}


BOOL CFindFileEndDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (WM_KEYFIRST<=pMsg->message&&pMsg->message<=WM_KEYLAST)
	{
		//MessageBox(_T("键盘按下"));
		if (pMsg->wParam==VK_RETURN)
		{
			OnBnClickedButton1();
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CFindFileEndDlg::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CMenu m_menu;
	m_menu.LoadMenu(IDR_MENU1);
	int x=pNMItemActivate->ptAction.x;
	int y=pNMItemActivate->ptAction.y;
	CPoint pt;

	//获取当前鼠标的位置
	GetCursorPos(&pt);
	//
	//m_List.GetDC()->LPtoDP(&pt);
	m_menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,pt.x,pt.y,this);
	//.......
	*pResult = 0;
}


void CFindFileEndDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//MessageBox(_T("DBClick"));
	POSITION ps=this->m_ListCrtl.GetFirstSelectedItemPosition();
	if (ps)
	{
		int item=this->m_ListCrtl.GetNextSelectedItem(ps);
		CString FileRealPath=this->m_ListCrtl.GetItemText(item,0); //获取文件名
		//MessageBox(ws);
		ShellExecute(0,_T("open"),FileRealPath,NULL,NULL,SW_SHOW); //打开文件
	}
	*pResult = 0;
}


void CFindFileEndDlg::OnRClickOpenFile()
{
	// TODO: 在此添加命令处理程序代码
// 	MessageBox(_T("申请大内存块"));
// 	int * tmp=(int*)malloc(sizeof(char)*1024*1024*50);
}
