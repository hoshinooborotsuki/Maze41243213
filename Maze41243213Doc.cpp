
// Maze41243213Doc.cpp: CMaze41243213Doc 類別的實作
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以定義在實作預覽、縮圖和搜尋篩選條件處理常式的
// ATL 專案中，並允許與該專案共用文件程式碼。
#ifndef SHARED_HANDLERS
#include "Maze41243213.h"
#endif

#include "Maze41243213Doc.h"

#include <propkey.h>

#include "CNewMazeDialog.h"


#include "MazeDataHeader.h"
extern struct MazeFileHeader MazeFileHeader; // 地圖的表頭資料結構

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMaze41243213Doc

IMPLEMENT_DYNCREATE(CMaze41243213Doc, CDocument)

BEGIN_MESSAGE_MAP(CMaze41243213Doc, CDocument)
//	ON_COMMAND(ID_FILE_NEW, &CMaze41243213Doc::OnFileNew)
//ON_COMMAND(ID_FILE_SAVE, &CMaze41243213Doc::OnFileSave)
ON_COMMAND(ID_FILE_SAVE, &CMaze41243213Doc::OnFileSave)
END_MESSAGE_MAP()


// CMaze41243213Doc 建構/解構

CMaze41243213Doc::CMaze41243213Doc() noexcept
{
	// TODO: 在此加入一次建構程式碼

}

CMaze41243213Doc::~CMaze41243213Doc()
{
}

BOOL CMaze41243213Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此加入重新初始化程式碼
	// (SDI 文件會重用此文件)
	return TRUE;
}




// CMaze41243213Doc 序列化

void CMaze41243213Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此加入儲存程式碼
	}
	else
	{
		// TODO: 在此加入載入程式碼
	}
}

#ifdef SHARED_HANDLERS

// 縮圖的支援
void CMaze41243213Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改這段程式碼以繪製文件的資料
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜尋處理常式的支援
void CMaze41243213Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// 設定來自文件資料的搜尋內容。
	// 內容部分應該以 ";" 隔開

	// 範例:     strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CMaze41243213Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CMaze41243213Doc 診斷

#ifdef _DEBUG
void CMaze41243213Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMaze41243213Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMaze41243213Doc 命令

void CMaze41243213Doc::OnFileSave()
{
	// TODO: 在此加入您的命令處理常式程式碼
}
