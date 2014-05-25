#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CMyProgressDlg 对话框

class CMyProgressDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMyProgressDlg)

public:
	CMyProgressDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyProgressDlg();

	// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 进度条
	CProgressCtrl m_ProgRessCtrl;
	void SetPog(unsigned int * Pog);
	void SetThreadText(CString Name);
	virtual BOOL OnInitDialog();
	// 进程名称  fuck
	CStatic m_StaticText;
};
