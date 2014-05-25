
// FindFileEndDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "MyDefFile.h"
#include "MyCreateTable.h"
#include "MyQueryFile.h"
#include "MyProgressDlg.h"

typedef struct PARAM_INPUT
{//扫描线程参数表
	CMyCreateTable * m_pCreat;
	unsigned int *m_Progress;
};

typedef struct POG_PARAM
{
	CMyProgressDlg * m_Dlg;
	unsigned int * m_Pog;
};
// CFindFileEndDlg 对话框
class CFindFileEndDlg : public CDialogEx
{
// 构造
public:
	CFindFileEndDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_FINDFILEEND_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	CMyProgressDlg m_ProgressDlg;
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// 输入的文件名字
	CEdit m_InputFileName;
	CListCtrl m_ListCrtl;

	 FILE_TABLE_MAP * FileTableMap;//这个设置成全局的  好让大家实用
	 RES_TABLE_VECTOR *FileRealRes;
	 afx_msg void OnBnClickedButton1();

	 CMyCreateTable *m_CreatTable;
	 CMyQueryFile *m_QueryFile;
	 afx_msg void OnEnChangeEdit1();
	 afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	 virtual BOOL PreTranslateMessage(MSG* pMsg);
	 afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	 afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	 afx_msg void OnRClickOpenFile();
};
