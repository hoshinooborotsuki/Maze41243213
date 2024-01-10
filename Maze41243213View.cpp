
// Maze41243213View.cpp: CMaze41243213View 類別的實作
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以定義在實作預覽、縮圖和搜尋篩選條件處理常式的
// ATL 專案中，並允許與該專案共用文件程式碼。
#ifndef SHARED_HANDLERS
#include "Maze41243213.h"
#endif

#include "MazeDataHeader.h"

#include "MainFrm.h"
#include "Maze41243213Doc.h"
#include "Maze41243213View.h"

#include "Maze41243213Eng.h"

#include "CNewMazeDialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMaze41243213View

IMPLEMENT_DYNCREATE(CMaze41243213View, CView)

BEGIN_MESSAGE_MAP(CMaze41243213View, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	//	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_SET_ROAD, &CMaze41243213View::OnSetRoad)
	ON_COMMAND(ID_SET_WALL, &CMaze41243213View::OnSetWall)
	ON_COMMAND(ID_SET_BEGIN, &CMaze41243213View::OnSetBegin)
	ON_COMMAND(ID_SET_END, &CMaze41243213View::OnSetEnd)
	ON_COMMAND(ID_FILE_NEW, &CMaze41243213View::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CMaze41243213View::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CMaze41243213View::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, &CMaze41243213View::OnFileSaveAs)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CMaze41243213View 建構/解構

CMaze41243213View::CMaze41243213View() noexcept
{
	// TODO: 在此加入建構程式碼
	MouseLButtonActive = 0;
	memset(FileName, 0, sizeof(FileName)); // 將檔名清除為0

}

CMaze41243213View::~CMaze41243213View()
{
}

BOOL CMaze41243213View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此經由修改 CREATESTRUCT cs 
	// 達到修改視窗類別或樣式的目的

	return CView::PreCreateWindow(cs);
}

// CMaze41243213View 繪圖

void CMaze41243213View::OnDraw(CDC* pDC)
{
	CMaze41243213Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此加入原生資料的描繪程式碼
	RenderMazeGraphics(pDC);
}

void CMaze41243213View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMaze41243213View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, FALSE);
#endif
}


// CMaze41243213View 診斷

#ifdef _DEBUG
void CMaze41243213View::AssertValid() const
{
	CView::AssertValid();
}

void CMaze41243213View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMaze41243213Doc* CMaze41243213View::GetDocument() const // 內嵌非偵錯版本
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMaze41243213Doc)));
	return (CMaze41243213Doc*)m_pDocument;
}
#endif //_DEBUG


// CMaze41243213View 訊息處理常式


int CMaze41243213View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此加入特別建立的程式碼

	return 0;
}


void CMaze41243213View::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	// 更新滑鼠指到地圖的XY座標
	// 座標需減掉外圍的虛擬圍牆TEXTUREWIDTH,TEXTUREHEIGHT
	MazePointX = point.x / TEXTUREWIDTH;
	MazePointY = point.y / TEXTUREHEIGHT;
	if (MouseLButtonActive)
	{
		SetMazeArrayData();
	}
	CView::OnMouseMove(nFlags, point);
}

void CMaze41243213View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	MazePointX = point.x / TEXTUREWIDTH;
	MazePointY = point.y / TEXTUREHEIGHT;
	MouseLButtonActive = 1;
	SetMazeArrayData();

	CView::OnLButtonDown(nFlags, point);
}


void CMaze41243213View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	MouseLButtonActive = 0;
	CView::OnLButtonUp(nFlags, point);
}


void CMaze41243213View::OnSetRoad()
{
	// TODO: 在此加入您的命令處理常式程式碼
	MazeItemMode = IDM_ROAD;
}


void CMaze41243213View::OnSetWall()
{
	// TODO: 在此加入您的命令處理常式程式碼
	MazeItemMode = IDM_WALL;
}


void CMaze41243213View::OnSetBegin()
{
	// TODO: 在此加入您的命令處理常式程式碼
	MazeItemMode = IDM_BEGIN;
}


void CMaze41243213View::OnSetEnd()
{
	// TODO: 在此加入您的命令處理常式程式碼
	MazeItemMode = IDM_END;
}


void CMaze41243213View::OnFileNew()
{
	// TODO: 在此加入您的命令處理常式程式碼
	CNewMazeDialog NewDlg;
	NewDlg.m_width = MazeFileHeader.width;
	NewDlg.m_height = MazeFileHeader.height;

	if (NewDlg.DoModal() == IDOK)
	{
		InitialMazeData(NewDlg.m_width, NewDlg.m_height, NewDlg.m_FillMode); // 初始化地圖資料
		InitialMazeBMPDC(this); // 建立地圖的虛擬螢幕背景圖
		RedrawWindow(); //更新視窗
		memset(FileName, 0, sizeof(FileName)); // 將檔名清除為0
		CString Untitled;
		Untitled.LoadString(IDS_UNTITLED);
		((CMainFrame*)(AfxGetApp()->m_pMainWnd))->SetWindowText(Untitled);
	}
	ResetDisplayTime(); // 因為對話視窗導致顯示暫停，累積一段時間差，所以要重設顯示張數時間
}


void CMaze41243213View::OnFileOpen()
{
	// TODO: 在此加入您的命令處理常式程式碼
	wchar_t filefilter[] = L"Maze41243213 檔案(*.Maze)\0*.Maze\0\0";
	wchar_t filetype[] = L"Maze";
	CFileDialog dlgFile(TRUE);
	OPENFILENAME& ofn = dlgFile.GetOFN();
	ofn.Flags |= OFN_HIDEREADONLY | OFN_NODEREFERENCELINKS;
	ofn.lpstrFilter = filefilter;
	ofn.lpstrFile = FileName;
	ofn.nMaxFile = MAX_PATH;

	memset(FileName, 0, sizeof(FileName));
	if (dlgFile.DoModal() != IDOK)
	{
		return;
	}
	if (ReadMazeFile(FileName) != -1)
	{
		InitialMazeBMPDC(this); // 建立地圖的虛擬螢幕背景圖

		((CMainFrame*)(AfxGetApp()->m_pMainWnd))->RedrawWindow(); //更新視窗

		CString FileTitle;
		int pathl;
		pathl = GetPathW(FileName); // 取得檔案名稱前面路徑長度，FileName[pathl]為檔名開頭
		FileTitle.Format(IDS_FILETITLE, &FileName[pathl]);
		((CMainFrame*)(AfxGetApp()->m_pMainWnd))->SetWindowText(FileTitle);
	}
	ResetDisplayTime(); // 因為對話視窗導致顯示暫停，累積一段時間差，所以要重設顯示張數時間
}


void CMaze41243213View::OnFileSave()
{
	// TODO: 在此加入您的命令處理常式程式碼
	int sl = (int)wcslen(FileName);
	if (sl == 0)
	{
		OnFileSaveAs();
	}
	else
	{
		WriteMazeFile(FileName);
		CString SaveOKS;
		int pathl;
		pathl = GetPathW(FileName); // 取得檔案名稱前面路徑長度，FileName[pathl]為檔名開頭
		SaveOKS.Format(IDS_SAVEOK, &FileName[pathl]);
		AfxMessageBox(SaveOKS);
		ResetDisplayTime(); // 因為對話視窗導致顯示暫停，累積一段時間差，所以要重設顯示張數時間
	}
}


void CMaze41243213View::OnFileSaveAs()
{
	// TODO: 在此加入您的命令處理常式程式碼
	wchar_t filefilter[] = L"Maze41243213 檔案(*.Maze)\0*.Maze\0\0";
	wchar_t filetype[] = L"Maze";

	CFileDialog dlgFile(FALSE);
	
	OPENFILENAME& ofn = dlgFile.GetOFN();
	ofn.Flags |= OFN_HIDEREADONLY | OFN_NODEREFERENCELINKS;
	ofn.lpstrFilter = filefilter;
	ofn.lpstrFile = FileName;
	ofn.nMaxFile = MAX_PATH;
	
	if (dlgFile.DoModal() != IDOK)
	{
		return;
	}
	WriteMazeFile(FileName);

	CString FileTitle;
	int pathl;
	pathl = GetPathW(FileName); // 取得檔案名稱前面路徑長度，FileName[pathl]為檔名開頭
	FileTitle.Format(IDS_FILETITLE, &FileName[pathl]);
	((CMainFrame*)(AfxGetApp()->m_pMainWnd))->SetWindowText(FileTitle);
	CString SaveOKS;
	pathl = GetPathW(FileName); // 取得檔案名稱前面路徑長度，FileName[pathl]為檔名開頭
	SaveOKS.Format(IDS_SAVEOK, &FileName[pathl]);
	AfxMessageBox(SaveOKS);
	ResetDisplayTime(); // 因為對話視窗導致顯示暫停，累積一段時間差，所以要重設顯示張數時間
}


void CMaze41243213View::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此加入您的訊息處理常式程式碼
	// 不要呼叫圖片訊息的 CView::OnPaint()
	RECT s, s1;
	int MapWidth, MapHeight;
	MapWidth = ((MazeFileHeader.width + 2) << 5);// ((MazeFileHeader.width+2)<<5) = ((MazeFileHeader.width+2)*TEXTUREWIDTH)
	MapHeight = ((MazeFileHeader.height + 2) << 5); // ((MazeFileHeader.height+2)<<5) = ((MazeFileHeader.height+2)*TEXTUREHEIGHT)

	((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetWindowRect(&s);
	GetClientRect(&s1);
	::OffsetRect(&s, -s.left, -s.top);
	s.right -= s1.right;
	s.bottom -= s1.bottom;
	s.right += MapWidth; 
	s.bottom += MapHeight;
	::OffsetRect(&s, -s.left, -s.top);
	//AdjustWindowRect(&s, GetWindowLong(m_hWnd, GWL_STYLE), TRUE);
	((CMainFrame*)(AfxGetApp()->m_pMainWnd))->SetWindowPos(&wndTop, 0, 0, s.right, s.bottom, SWP_NOMOVE);
}


BOOL CMaze41243213View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值

	//return CView::OnEraseBkgnd(pDC);
	EraseBackGround(pDC); //純粹恢復地圖的圖形
	return(TRUE);
}
