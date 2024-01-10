
// Maze41243213View.h: CMaze41243213View 類別的介面
//

#pragma once

#define TOTALBITMAPS 8

class CMaze41243213View : public CView
{
protected: // 僅從序列化建立
	CMaze41243213View() noexcept;
	DECLARE_DYNCREATE(CMaze41243213View)

private:
	wchar_t FileName[MAX_PATH];
	int MouseLButtonActive;
		// 屬性
public:
	CMaze41243213Doc* GetDocument() const;
// 作業
public:

// 覆寫
public:
	virtual void OnDraw(CDC* pDC);  // 覆寫以描繪此檢視
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 程式碼實作
public:
	virtual ~CMaze41243213View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 產生的訊息對應函式
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSetRoad();
	afx_msg void OnSetWall();
	afx_msg void OnSetBegin();
	afx_msg void OnSetEnd();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // 對 Maze41243213View.cpp 中的版本進行偵錯
inline CMaze41243213Doc* CMaze41243213View::GetDocument() const
   { return reinterpret_cast<CMaze41243213Doc*>(m_pDocument); }
#endif

