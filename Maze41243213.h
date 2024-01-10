
// Maze41243213.h: Maze41243213 應用程式的主要標頭檔
//
#pragma once

#ifndef __AFXWIN_H__
	#error "對 PCH 在包含此檔案前先包含 'pch.h'"
#endif

#include "resource.h"       // 主要符號


// CMaze41243213App:
// 查看 Maze41243213.cpp 以了解此類別的實作
//

class CMaze41243213App : public CWinAppEx
{
public:
	CMaze41243213App() noexcept;


// 覆寫
public:
	virtual BOOL InitInstance();

// 程式碼實作
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);
//	afx_msg void OnFileSave();
//	afx_msg void OnFileSave();
};

extern CMaze41243213App theApp;
