#pragma once
#include "afxdialogex.h"


// CNewMazeDialog 對話方塊

class CNewMazeDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CNewMazeDialog)

public:
	CNewMazeDialog(CWnd* pParent = nullptr);   // 標準建構函式
	virtual ~CNewMazeDialog();

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NEWMAZE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

	DECLARE_MESSAGE_MAP()
public:
	int m_width;
	int m_height;
	int m_FillMode;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
};
