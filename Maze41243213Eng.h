#pragma once

#include "afxwin.h"
#include "MazeDataHeader.h"

extern double DisplayTime; // 紀錄更新顯示的時間

extern struct MAZEFILEHEADER MazeFileHeader; // 地圖的表頭資料結構
extern char* MazeArrayBuffer; // 地圖資料的動態配置記憶體

extern CBitmap* BackGroundBMP, TextureBMP; // BackGroundBMP:地圖的虛擬螢幕背景，TextureBMP:地圖的材質的圖形
extern CDC* BackGroundDC, TextureDC; // BackGroundDC:地圖的虛擬螢幕背景繪製物件，TextureDC:地圖的材質的繪製物件
extern int MazePointX, MazePointY; // 滑鼠指到地圖的XY座標
extern int MazeItemMode; // 目前填入地圖的樣式，預設是可通過的路

int GetPathW(wchar_t* wSrcPath, wchar_t* wDestPath = NULL, int upf = 0); // 取檔名的路徑，回傳路徑長度，參數upf=1就是轉大寫
void ResetDisplayTime(void); // 重設顯示每秒張數控制時間
void InitializeGrp(void); // 初始化繪圖元件
void ReleaseGrp(void); // 釋放繪圖元件

void InitialMazeData(int width, int height, char DefaultMazeData);	// 初始化地圖資料
void InitialMazeBMPDC(CWnd* m_pMainWnd); // 建立地圖的虛擬螢幕背景圖
void RenderMazeGraphics(CDC* pDC); // 更新地圖的圖形狀態
void EraseBackGround(CDC* pDC); //純粹恢復地圖的圖形

void SetMazeArrayData(void); // 依目前滑鼠指的位置指定為目前選的地圖元件

int ReadMazeFile(wchar_t* FileName); // 讀取Maze檔案
int WriteMazeFile(wchar_t* FileName); // 儲存Maze檔案