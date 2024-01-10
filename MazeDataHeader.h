#pragma once

// 這個資料結構用1 Byte來做資料結構成員對齊，因為預設值是8 Bytes
#pragma pack (push, 1)
struct MAZEFILEHEADER
{
	short int width, height;	
	short int BeginX, BeginY;
	short int EndX, EndY;
	int totalanswers;
};

#pragma pack (pop)

#define TEXTUREWIDTH 32
#define TEXTUREHEIGHT 32
#define BRIGHTALPHA 200
enum
{
	IDM_ROAD=0, IDM_WALL, IDM_BEGIN, IDM_END, IDM_UP, IDM_DOWN, IDM_LEFT, IDM_RIGHT, IDM_BRIGHT
};

#define FRAMETIMESTEP (1000.0/60.0)
