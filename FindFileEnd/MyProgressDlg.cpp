// MyProgressDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FindFileEnd.h"
#include "MyProgressDlg.h"
#include "afxdialogex.h"


// CMyProgressDlg 对话框

IMPLEMENT_DYNAMIC(CMyProgressDlg, CDialogEx)

	CMyProgressDlg::CMyProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyProgressDlg::IDD, pParent)
{

}

CMyProgressDlg::~CMyProgressDlg()
{
}

void CMyProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_ProgRessCtrl);
	DDX_Control(pDX, IDC_STATIC1, m_StaticText);
}


BEGIN_MESSAGE_MAP(CMyProgressDlg, CDialogEx)
END_MESSAGE_MAP()


// CMyProgressDlg 消息处理程序
void CMyProgressDlg::SetPog(unsigned int * Pog)
{
	//HWND
	
	if (*Pog<=100&&Pog)
	{
		this->m_ProgRessCtrl.SetPos(*Pog);
		if (100==(*Pog))
		{
			Sleep(500);
			this->ShowWindow(SW_HIDE);
			TRACE("关闭扫描进程对话框\n");
		}
	}
}

BOOL CMyProgressDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	/*AfxGetMainWnd()->*/
	SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);//设置窗口最前
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CMyProgressDlg::SetThreadText(CString Name)
{
	this->m_StaticText.SetWindowText(Name);
}
