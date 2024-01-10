
// MainFrm.cpp: CMainFrame 類別的實作
//

#include "pch.h"
#include "framework.h"
#include "Maze41243213.h"

#include "Maze41243213Doc.h"
#include "Maze41243213View.h"


#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
//	ON_WM_PAINT()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 狀態列指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 建構/解構

CMainFrame::CMainFrame() noexcept
{
	// TODO: 在此加入成員初始化程式碼
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	BOOL bNameValid;

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("無法建立功能表列\n");
		return -1;      // 無法建立
	}
	
	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// 防止功能表列在啟用時取得焦點
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_GRIPPER | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("無法建立工具列\n");
		return -1;      // 無法建立
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	//m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("無法建立狀態列\n");
		return -1;      // 無法建立
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: 如果不希望工具列和功能表列為可停駐，請刪除這 5 行
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);


	// 啟用 Visual Studio 2005 樣式停駐視窗行為
	CDockingManager::SetDockingMode(DT_SMART);
	// 啟用 Visual Studio 2005 樣式停駐視窗自動隱藏行為
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// 設定用來繪製所有使用者介面項目的視覺化管理員
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));

	// 啟用工具列和停駐視窗功能表取代
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// 啟用快速 (Alt+拖曳) 工具列自訂
	CMFCToolBar::EnableQuickCustomization();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此經由修改 CREATESTRUCT cs 
	// 達到修改視窗類別或樣式的目的

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_THICKFRAME | WS_MINIMIZEBOX | WS_SYSMENU;

	return TRUE;
}

// CMainFrame 診斷

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 訊息處理常式

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* 掃描功能表 */);
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}


BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext)
{
	// 基底類別從事實際作業

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	return TRUE;
}

//void CMainFrame::OnFileOpen()
//{
	// TODO: 在此加入您的命令處理常式程式碼
//	wchar_t filefilter[] = L"Maze41243213 檔案(*.Maze)\0*.Maze\0\0";
//	wchar_t filetype[] = L"Maze";
//	CFileDialog dlgFile(TRUE, filetype, FileName, OFN_HIDEREADONLY | OFN_NODEREFERENCELINKS, filefilter, this);
//
//	memset(FileName, 0, sizeof(FileName));
//	if (dlgFile.DoModal() != IDOK)
//	{
//		return;
//	}
//	if (ReadMazeFile(FileName) != -1)
//	{
//		InitialMazeBMPDC(this); // 建立地圖的虛擬螢幕背景圖
//		RedrawWindow(); //更新視窗
//		CString FileTitle;
//		FileTitle.Format(IDS_FILETITLE, FileName);
//		SetWindowText(FileTitle);
//	}
//	ResetDisplayTime(); // 因為對話視窗導致顯示暫停，累積一段時間差，所以要重設顯示張數時間
//
//}

//void CMainFrame::OnFileSave()
//{
	// TODO: 在此加入您的命令處理常式程式碼
//	int sl = (int)wcslen(FileName);
//	if (sl == 0)
//	{
//		OnFileSaveAs();
//	}
//}

//void CMainFrame::OnFileSaveAs()
//{
	// TODO: 在此加入您的命令處理常式程式碼
//	wchar_t filefilter[] = L"Maze41243213 檔案(*.Maze)\0*.Maze\0\0";
//	wchar_t filetype[] = L"*.Maze";
//
//	CFileDialog dlgFile(FALSE, filetype, FileName, OFN_HIDEREADONLY | OFN_NODEREFERENCELINKS, filefilter, this);
//	wcscpy_s(FileName, L"Maze41243213.Maze");
//	/*
//	OPENFILENAME& ofn = dlgFile.GetOFN();
//	ofn.Flags |= OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NODEREFERENCELINKS;
//	ofn.lpstrFilter = filetype;
//	ofn.lpstrFile = FileName;
//	ofn.nMaxFile = MAX_PATH;
//	dlgFile.SetDefExt(filetype);
//	*/
//	if (dlgFile.DoModal() != IDOK)
//	{
//		return;
//	}
//}
