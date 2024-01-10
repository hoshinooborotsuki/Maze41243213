// CNewMazeDialog.cpp: 實作檔案
//

#include "pch.h"
#include "Maze41243213.h"
#include "afxdialogex.h"
#include "CNewMazeDialog.h"

#include "MazeDataHeader.h"

// CNewMazeDialog 對話方塊

IMPLEMENT_DYNAMIC(CNewMazeDialog, CDialogEx)

CNewMazeDialog::CNewMazeDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_NEWMAZE_DIALOG, pParent)
	, m_width(0)
	, m_height(0)
	, m_FillMode(0)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

CNewMazeDialog::~CNewMazeDialog()
{
}

void CNewMazeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_width);
	DDV_MinMaxInt(pDX, m_width, 20, 40);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_height);
	DDV_MinMaxInt(pDX, m_height, 20, 30);
	DDX_CBIndex(pDX, IDC_FILLMODE, m_FillMode);
	DDV_MinMaxInt(pDX, m_FillMode, 0, 1);
}


BEGIN_MESSAGE_MAP(CNewMazeDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CNewMazeDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CNewMazeDialog::OnBnClickedCancel)
END_MESSAGE_MAP()


// CNewMazeDialog 訊息處理常式


void CNewMazeDialog::OnBnClickedOk()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	UpdateData(); // 將UI數值讀取
	CDialogEx::OnOK();
}


void CNewMazeDialog::OnBnClickedCancel()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	CDialogEx::OnCancel();
}

BOOL CNewMazeDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此加入額外的初始化
	m_FillMode = IDM_ROAD;
	UpdateData(FALSE); // 將變數傳入UI

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX 屬性頁應傳回 FALSE
}
