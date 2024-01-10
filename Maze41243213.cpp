
// Maze41243213.cpp: 定義應用程式的類別表現方式。
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Maze41243213.h"
#include "MainFrm.h"

#include "Maze41243213Doc.h"
#include "Maze41243213View.h"

#include "Maze41243213Eng.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMaze41243213App

BEGIN_MESSAGE_MAP(CMaze41243213App, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CMaze41243213App::OnAppAbout)
	// 依據文件命令的標準檔案
	//ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	//ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
END_MESSAGE_MAP()


// CMaze41243213App 建構

CMaze41243213App::CMaze41243213App() noexcept
{
	m_bHiColorIcons = TRUE;


	// TODO: 以唯一的 ID 字串取代下面的應用程式 ID 字串; 建議的
	// 字串格式為 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("Maze41243213.AppID.NoVersion"));

	// TODO:  在此加入建構程式碼，
	// 將所有重要的初始設定加入 InitInstance 中
}

// 唯一一個 CMaze41243213App 物件

CMaze41243213App theApp;


// CMaze41243213App 初始化

BOOL CMaze41243213App::InitInstance()
{
	AfxOleInit();
	// 假如應用程式資訊清單指定使用 ComCtl32.dll 6 (含) 以後版本，
	// 來啟動視覺化樣式，在 Windows XP 上，則需要 InitCommonControls()。
	// 否則任何視窗的建立都將失敗。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 設定要包含所有您想要用於應用程式中的
	// 通用控制項類別。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	EnableTaskbarInteraction(FALSE);

	// 需要有 AfxInitRichEdit2() 才能使用 RichEdit 控制項
	// AfxInitRichEdit2();

	// 標準初始設定
	// 如果您不使用這些功能並且想減少
	// 最後完成的可執行檔大小，您可以
	// 從下列程式碼移除不需要的初始化常式，
	// 變更儲存設定值的登錄機碼
	// TODO: 您應該適度修改此字串
	// (例如，公司名稱或組織名稱)
	SetRegistryKey(_T("本機 AppWizard 所產生的應用程式"));
	LoadStdProfileSettings(4);  // 載入標準 INI 檔選項 (包含 MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 登錄應用程式的文件範本。文件範本負責在文件、
	// 框架視窗與檢視間進行連接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMaze41243213Doc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架視窗
		RUNTIME_CLASS(CMaze41243213View));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// 剖析標準 Shell 命令、DDE、檔案開啟舊檔的命令列
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// 啟用 DDE 執行開啟
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);



	// 在命令列中指定的分派命令。如果已使用 
	// /RegServer、/Register、/Unregserver 或 /Unregister 啟動應用程式，將傳回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	InitialMazeData(20,20, IDM_WALL); // 初始化地圖資料
	InitializeGrp(); //初始化繪圖元件
	InitialMazeBMPDC(m_pMainWnd); // 建立地圖的虛擬螢幕背景圖

	// 僅初始化一個視窗，所以顯示並更新該視窗
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	ResetDisplayTime();
	// 只有在 MDI 應用程式中有後置字元時，才呼叫 DragAcceptFiles
	// 這會發生於 ProcessShellCommand 之後
	// 啟用拖放開啟
	m_pMainWnd->DragAcceptFiles();

	// 初始化繪圖引擎圖形元件
	return TRUE;
}

// CMaze41243213App 訊息處理常式


// 對 App About 使用 CAboutDlg 對話方塊

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

// 程式碼實作
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// 執行對話方塊的應用程式命令
void CMaze41243213App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
	ResetDisplayTime(); // 因為對話視窗導致顯示暫停，累積一段時間差，所以要重設顯示張數時間
}

// CMaze41243213App 自訂載入/儲存方法

void CMaze41243213App::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CMaze41243213App::LoadCustomState()
{
}

void CMaze41243213App::SaveCustomState()
{
}

// CMaze41243213App 訊息處理常式

int CMaze41243213App::ExitInstance()
{
	// TODO: 在此加入特定的程式碼和 (或) 呼叫基底類別
	ReleaseGrp();
	return CWinAppEx::ExitInstance();
}


BOOL CMaze41243213App::OnIdle(LONG lCount)
{
	// TODO: 在此加入特定的程式碼和 (或) 呼叫基底類別
	ULONGLONG NowTime,CheckTime;

	NowTime = GetTickCount64();
	CheckTime = (ULONGLONG)DisplayTime;
	if (NowTime >= CheckTime) //大於顯示時間
	{
		DisplayTime += FRAMETIMESTEP;
		CMaze41243213View* pView = (CMaze41243213View*)((CMainFrame*)m_pMainWnd)->GetActiveView();
		CDC *pDC = pView->GetDC();
		RenderMazeGraphics(pDC);
		pView->ReleaseDC(pDC);
	}
	return(TRUE);

	//return CWinAppEx::OnIdle(lCount);
}
