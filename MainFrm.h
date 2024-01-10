﻿
// MainFrm.h: CMainFrame 類別的介面
//

#pragma once

class CMainFrame : public CFrameWndEx
{
	
protected: // 僅從序列化建立
	CMainFrame() noexcept;
	DECLARE_DYNCREATE(CMainFrame)

// 屬性
public:

// 作業
public:
// 覆寫
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = nullptr, CCreateContext* pContext = nullptr);

// 程式碼實作
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控制列內嵌的成員
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;

// 產生的訊息對應函式
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()

public:
//	afx_msg void OnPaint();
//	afx_msg void OnFileNew();
//	afx_msg void OnFileOpen();
//	afx_msg void OnFileSave();
//	afx_msg void OnFileSaveAs();
};


