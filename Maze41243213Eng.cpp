#include "pch.h"

#include <fcntl.h> // _wsopen_s 開啟檔案的參數相關定義 Needed only for _O_RDWR definition 
#include <io.h>
#include <stdlib.h>
#include <stdio.h>
#include <share.h>

#include "Maze41243213.h"

#include "MazeDataHeader.h"

struct MAZEFILEHEADER MazeFileHeader; // 地圖的表頭資料結構
char* MazeArrayBuffer = NULL; // 地圖資料的動態配置記憶體

// 自訂變數，繪圖引擎用
double DisplayTime; // 紀錄更新顯示的時間

CBitmap* BackGroundBMP = NULL, TextureBMP; // BackGroundBMP:地圖的虛擬螢幕背景，TextureBMP:地圖的材質的圖形
CDC* BackGroundDC = NULL, TextureDC; // BackGroundDC:地圖的虛擬螢幕背景繪製物件，TextureDC:地圖的材質的繪製物件
int MazeArrayYCache[50]; // MazeArrayBuffer的陣列的Y座標的記憶體位置起始數值
int MazePointX = 0, MazePointY = 0; // 滑鼠指到地圖的XY座標
int MazeItemMode = IDM_ROAD; // 目前填入地圖的樣式，預設是可通過的路
int MazeBrightAlpha = 0;

int GetPathW(wchar_t * wSrcPath, wchar_t* wDestPath = NULL, int upf = 0) // 取檔名的路徑，回傳路徑長度，參數upf=1就是轉大寫
{
	int l, ret;
	// 取得輸入字串的長度
	l = (int)wcslen(wSrcPath);
	do 
	{
		l--;
	} while (l && !(*(wSrcPath + l) == L'\\'));
	ret = l;
	// 計算路徑長度
	if (l)
	{
		ret++;
	}
	// 如果輸入了目標路徑指標，則進行相應的處理
	if (l > 1 && wDestPath != NULL)
	{
		wDestPath[l] = 0;// 將結尾字元設置為0
		while (l - 1)
		{
			l--;
			*(wDestPath + l) = *(wSrcPath + l);
			// 如果 upf 參數為 1，則轉換字母為大寫
			if (upf)
			{
				if (*(wSrcPath + l) > 0x60 && *(wSrcPath + l) < 0x7b) // ASCII 大小寫差別在bit5，做and的歸0轉大寫
				{
					*(wDestPath + l) &= 0xdf;
				}
			}
		}
		// 複製第一個字元
		wDestPath[0] = wSrcPath[0];
	}
	return(ret);//回傳路徑長
}

void ResetDisplayTime(void) // 重設顯示每秒張數控制時間
{
	DisplayTime = (double)GetTickCount64();
}
void InitializeGrp(void) // 初始化繪圖元件
{

	CWnd Wnd,*DesktopWnd;
	
	DesktopWnd = Wnd.GetDesktopWindow();// 獲取桌面窗口
	CDC* pDC = DesktopWnd->GetDC();		// 獲取桌面窗口的Device Context (DC)
	TextureBMP.LoadBitmap(IDB_TEXTURE);	// 載入位圖資源 IDB_TEXTURE
	TextureDC.CreateCompatibleDC(pDC);	// 創建與桌面DC兼容的DC
	TextureDC.SelectObject(&TextureBMP);// 將位圖選入TextureDC
	DesktopWnd->ReleaseDC(pDC);			// 釋放桌面窗口的DC
}

void ReleaseGrp(void) // 釋放繪圖元件
{
	if (MazeArrayBuffer != NULL)// 檢查是否分配了MazeArrayBuffer
	{
		free(MazeArrayBuffer);
	}
	TextureBMP.DeleteObject();	// 刪除TextureBMP，釋放相關的位圖資源
	TextureDC.DeleteDC();		// 刪除TextureDC，釋放相關的Device Context
	delete BackGroundBMP;		// 刪除BackGroundBMP，釋放相關的位圖資源
	delete BackGroundDC;		// 刪除BackGroundDC，釋放相關的Device Context
}

void InitialMazeData(int width, int height, char DefaultMazeData)
{
	// 初始化預設的地圖資料
	MazeFileHeader.width = width;             // 設定檔案寬度
	MazeFileHeader.height = height;           // 設定檔案高度
	MazeFileHeader.BeginX = width - 1;        // 設定起點的X座標
	MazeFileHeader.BeginY = height - 1;       // 設定起點的Y座標
	MazeFileHeader.EndX = 0;                  // 設定終點的X座標
	MazeFileHeader.EndY = 0;                  // 設定終點的Y座標
	//MazeFileHeader.totalanswers = 0;          

	int i, yp = 0;
	// 計算出MazeArrayBuffer的陣列的Y座標的記憶體位置起始數值，用於加速Y座標計算，省去乘法
	for (i = 0; i < MazeFileHeader.height; i++)
	{
		MazeArrayYCache[i] = yp;
		yp += MazeFileHeader.width;
	}
	// 如果MazeArrayBuffer已經分配了記憶體，則進行釋放相關資源
	if (MazeArrayBuffer != NULL)
	{
		free(MazeArrayBuffer);
		delete BackGroundBMP;
		delete BackGroundDC;
	}
	// 分配MazeArrayBuffer的記憶體，並將其初始化為DefaultMazeData
	MazeArrayBuffer = (char*)malloc(MazeFileHeader.width * MazeFileHeader.height);
	if (MazeArrayBuffer != NULL)
	{
		memset(MazeArrayBuffer, DefaultMazeData, MazeFileHeader.width * MazeFileHeader.height);
		// 在起點和終點的位置設定特定的標識
		MazeArrayBuffer[MazeArrayYCache[MazeFileHeader.BeginY] + MazeFileHeader.BeginX] = IDM_BEGIN;
		MazeArrayBuffer[MazeArrayYCache[MazeFileHeader.EndY] + MazeFileHeader.EndX] = IDM_END;
	}
}

void InitialMazeBMPDC( CWnd* m_pMainWnd) // 初始化地圖資料，建立地圖的虛擬螢幕背景圖
{
	int i;
	CDC* pDC = m_pMainWnd->GetDC();


	BackGroundBMP = new CBitmap;
	BackGroundDC = new CDC;
	
	BackGroundBMP->CreateCompatibleBitmap(pDC, (MazeFileHeader.width + 2) * TEXTUREWIDTH, (MazeFileHeader.height + 2) * TEXTUREHEIGHT); //+2是為了顯示四邊的牆壁
	BackGroundDC->CreateCompatibleDC(pDC);
	BackGroundDC->SelectObject(BackGroundBMP);

	// 將地圖背景的外圍劃出一圈圍牆
	for (i = 0; i < (MazeFileHeader.width + 2); i++) // 畫上下橫圍牆
	{
		BackGroundDC->BitBlt((i << 5), 0, TEXTUREWIDTH, TEXTUREHEIGHT, &TextureDC, 0, TEXTUREHEIGHT, SRCCOPY); // (i<<5)=i*TEXTUREWIDTH
		BackGroundDC->BitBlt((i << 5), ((MazeFileHeader.height + 1) << 5), TEXTUREWIDTH, TEXTUREHEIGHT, &TextureDC, 0, TEXTUREHEIGHT, SRCCOPY); // (MazeFileHeader.height<<5)=MazeFileHeader.height*TEXTUREHEIGHT
	}
	for (i = 0; i < MazeFileHeader.height; i++) // 畫左右豎圍牆
	{
		BackGroundDC->BitBlt(0, ((i + 1) << 5), TEXTUREWIDTH, TEXTUREHEIGHT, &TextureDC, 0, TEXTUREHEIGHT, SRCCOPY);
		BackGroundDC->BitBlt(((MazeFileHeader.width + 1) << 5), ((i + 1) << 5), TEXTUREWIDTH, TEXTUREHEIGHT, &TextureDC, 0, TEXTUREHEIGHT, SRCCOPY);
	}
	m_pMainWnd->ReleaseDC(pDC);
}

// TiledMap，利用MazeArrayBuffer上的資料對應演算出地圖圖樣
void RenderMazeGraphics(CDC* pDC)
{
	int x, y, xp, yp = 0;
	for (y = 0; y < MazeFileHeader.height; y++)
	{
		yp += TEXTUREHEIGHT; // 提早先+TEXTUREHEIGHT，因為外圍有一層牆壁包圍
		xp = 0;
		for (x = 0; x < MazeFileHeader.width; x++)
		{
			xp += TEXTUREWIDTH;	// 提早先+TEXTUREWIDTH，因為外圍有一層牆壁包圍
			BackGroundDC->BitBlt(xp, yp, TEXTUREWIDTH, TEXTUREHEIGHT, &TextureDC, 0, ((int)*(MazeArrayBuffer + MazeArrayYCache[y] + x) << 5), SRCCOPY); // (i<<5)=i*TEXTUREHEIGHT
		}
	}
	//顯示滑鼠游標所指地圖為位置上的地圖元件類型MazePointX，MazePointX含外圍虛擬牆壁

	if (MazePointX > 0 && MazePointX <= MazeFileHeader.width && MazePointY>0 && MazePointY <= MazeFileHeader.height)
	{
		int newAlpha;
		BLENDFUNCTION Blend;
		Blend.BlendOp = 0;
		Blend.BlendFlags = 0;
		if (MazeBrightAlpha >= BRIGHTALPHA)
		{
			newAlpha = MazeBrightAlpha - BRIGHTALPHA;
			newAlpha = BRIGHTALPHA - newAlpha;
		}
		else
		{
			newAlpha = MazeBrightAlpha;
		}
		Blend.SourceConstantAlpha = newAlpha;
		Blend.AlphaFormat = 0;
		
		MazeBrightAlpha += 10;
		if (MazeBrightAlpha == (BRIGHTALPHA << 1))
		{
			MazeBrightAlpha = 0;
		}

		xp = MazePointX * TEXTUREWIDTH;
		yp = MazePointY * TEXTUREHEIGHT;

		BackGroundDC->BitBlt(xp, yp, TEXTUREWIDTH, TEXTUREHEIGHT, &TextureDC, 0, (MazeItemMode << 5), SRCCOPY); // (MazeItemMode<<5)=MazeItemMode*TEXTUREHEIGHT
		BackGroundDC->AlphaBlend(xp, yp, TEXTUREWIDTH, TEXTUREHEIGHT, &TextureDC, 0, (IDM_BRIGHT << 5), TEXTUREWIDTH, TEXTUREHEIGHT, Blend);
	}
	int MapWidth, MapHeight;
	MapWidth = ((MazeFileHeader.width + 2) << 5);// ((MazeFileHeader.width+2)<<5) = ((MazeFileHeader.width+2)*TEXTUREWIDTH)
	MapHeight = ((MazeFileHeader.height + 2) << 5); // ((MazeFileHeader.height+2)<<5) = ((MazeFileHeader.height+2)*TEXTUREHEIGHT)
	pDC->BitBlt(0, 0, MapWidth, MapHeight, BackGroundDC, 0, 0, SRCCOPY);
}

void EraseBackGround(CDC* pDC)
{
	int MapWidth, MapHeight;
	MapWidth = ((MazeFileHeader.width + 2) << 5);// ((MazeFileHeader.width+2)<<5) = ((MazeFileHeader.width+2)*TEXTUREWIDTH)
	MapHeight = ((MazeFileHeader.height + 2) << 5); // ((MazeFileHeader.height+2)<<5) = ((MazeFileHeader.height+2)*TEXTUREHEIGHT)
	pDC->BitBlt(0, 0, MapWidth, MapHeight, BackGroundDC, 0, 0, SRCCOPY);
}

void SetMazeArrayData(void)
{
	if (MazePointX > 0 && MazePointX <= MazeFileHeader.width && MazePointY > 0 && MazePointY <= MazeFileHeader.height)
	{
		if (MazeItemMode == IDM_BEGIN) // 開始只能有一個
		{
			*(MazeArrayBuffer + MazeArrayYCache[MazeFileHeader.BeginY] + MazeFileHeader.BeginX) = IDM_ROAD;
			MazeFileHeader.BeginX = (MazePointX - 1);
			MazeFileHeader.BeginY = (MazePointY - 1);
			*(MazeArrayBuffer + MazeArrayYCache[MazeFileHeader.BeginY] + MazeFileHeader.BeginX) = IDM_BEGIN;
		}
		else
		{
			if (MazeItemMode == IDM_END) // 結束只能有一個
			{
				*(MazeArrayBuffer + MazeArrayYCache[MazeFileHeader.EndY] + MazeFileHeader.EndX) = IDM_ROAD;
				MazeFileHeader.EndX = (MazePointX - 1);
				MazeFileHeader.EndY = (MazePointY - 1);
				*(MazeArrayBuffer + MazeArrayYCache[MazeFileHeader.EndY] + MazeFileHeader.EndX) = IDM_END;
			}
			else
			{
				if (*(MazeArrayBuffer + MazeArrayYCache[(MazePointY - 1)] + (MazePointX - 1)) < IDM_BEGIN) // 只有道路或牆壁才能這樣改變，開始或結束點不行這樣覆蓋刪掉
				{
					*(MazeArrayBuffer + MazeArrayYCache[(MazePointY - 1)] + (MazePointX - 1)) = (char)MazeItemMode;
				}
			}
		}
	}
}

int ReadMazeFile(wchar_t * FileName) // 讀取Maze檔案
{
	int filehandle = -1;
	_wsopen_s(&filehandle, FileName, _O_RDONLY | _O_BINARY, _SH_DENYNO, _S_IREAD | _S_IWRITE);
	if (filehandle == -1)
	{
		//開檔案失敗，回傳-1
		return -1;
	}
	int i, yp = 0;

	_read(filehandle, &MazeFileHeader, sizeof(struct MAZEFILEHEADER)); // 讀出struct MAZEFILEHEADER這個表頭資料

	// 計算出MazeArrayBuffer的陣列的Y座標的記憶體位置起始數值，用於加速Y座標計算，省去乘法
	for (i = 0; i < MazeFileHeader.height; i++)
	{
		MazeArrayYCache[i] = yp;
		yp += MazeFileHeader.width;
	}
	if (MazeArrayBuffer != NULL)
	{
		free(MazeArrayBuffer);
		delete BackGroundBMP;
		delete BackGroundDC;
	}

	MazeArrayBuffer = (char*)malloc(MazeFileHeader.width * MazeFileHeader.height);
	_read(filehandle, MazeArrayBuffer, MazeFileHeader.width * MazeFileHeader.height); //讀取地圖資料
	//MazeArrayBuffer[MazeArrayYCache[MazeFileHeader.BeginY] + MazeFileHeader.BeginX] = IDM_BEGIN;
	//MazeArrayBuffer[MazeArrayYCache[MazeFileHeader.EndY] + MazeFileHeader.EndX] = IDM_END;
	_close(filehandle); //關閉檔案
	return(0);
}

int WriteMazeFile(wchar_t* FileName) // 儲存Maze檔案
{
	int filehandle = -1;
	_wsopen_s(&filehandle, FileName, _O_WRONLY |_O_CREAT | _O_TRUNC | _O_BINARY, _SH_DENYNO, _S_IREAD | _S_IWRITE);
	if (filehandle == -1)
	{
		//開檔案失敗，回傳-1
		return -1;
	}
	_write(filehandle, &MazeFileHeader, sizeof(struct MAZEFILEHEADER)); // 寫入struct MAZEFILEHEADER這個表頭資料
	_write(filehandle, MazeArrayBuffer, MazeFileHeader.width * MazeFileHeader.height); //寫入地圖資料
	_close(filehandle); //關閉檔案
	return(0);
}