#include "pch.h"

#include <fcntl.h> // _wsopen_s �}���ɮת��ѼƬ����w�q Needed only for _O_RDWR definition 
#include <io.h>
#include <stdlib.h>
#include <stdio.h>
#include <share.h>

#include "Maze41243213.h"

#include "MazeDataHeader.h"

struct MAZEFILEHEADER MazeFileHeader; // �a�Ϫ����Y��Ƶ��c
char* MazeArrayBuffer = NULL; // �a�ϸ�ƪ��ʺA�t�m�O����

// �ۭq�ܼơAø�Ϥ�����
double DisplayTime; // ������s��ܪ��ɶ�

CBitmap* BackGroundBMP = NULL, TextureBMP; // BackGroundBMP:�a�Ϫ������ù��I���ATextureBMP:�a�Ϫ����誺�ϧ�
CDC* BackGroundDC = NULL, TextureDC; // BackGroundDC:�a�Ϫ������ù��I��ø�s����ATextureDC:�a�Ϫ����誺ø�s����
int MazeArrayYCache[50]; // MazeArrayBuffer���}�C��Y�y�Ъ��O�����m�_�l�ƭ�
int MazePointX = 0, MazePointY = 0; // �ƹ�����a�Ϫ�XY�y��
int MazeItemMode = IDM_ROAD; // �ثe��J�a�Ϫ��˦��A�w�]�O�i�q�L����
int MazeBrightAlpha = 0;

int GetPathW(wchar_t * wSrcPath, wchar_t* wDestPath = NULL, int upf = 0) // ���ɦW�����|�A�^�Ǹ��|���סA�Ѽ�upf=1�N�O��j�g
{
	int l, ret;
	// ���o��J�r�ꪺ����
	l = (int)wcslen(wSrcPath);
	do 
	{
		l--;
	} while (l && !(*(wSrcPath + l) == L'\\'));
	ret = l;
	// �p����|����
	if (l)
	{
		ret++;
	}
	// �p�G��J�F�ؼи��|���СA�h�i��������B�z
	if (l > 1 && wDestPath != NULL)
	{
		wDestPath[l] = 0;// �N�����r���]�m��0
		while (l - 1)
		{
			l--;
			*(wDestPath + l) = *(wSrcPath + l);
			// �p�G upf �ѼƬ� 1�A�h�ഫ�r�����j�g
			if (upf)
			{
				if (*(wSrcPath + l) > 0x60 && *(wSrcPath + l) < 0x7b) // ASCII �j�p�g�t�O�bbit5�A��and���k0��j�g
				{
					*(wDestPath + l) &= 0xdf;
				}
			}
		}
		// �ƻs�Ĥ@�Ӧr��
		wDestPath[0] = wSrcPath[0];
	}
	return(ret);//�^�Ǹ��|��
}

void ResetDisplayTime(void) // ���]��ܨC��i�Ʊ���ɶ�
{
	DisplayTime = (double)GetTickCount64();
}
void InitializeGrp(void) // ��l��ø�Ϥ���
{

	CWnd Wnd,*DesktopWnd;
	
	DesktopWnd = Wnd.GetDesktopWindow();// ����ୱ���f
	CDC* pDC = DesktopWnd->GetDC();		// ����ୱ���f��Device Context (DC)
	TextureBMP.LoadBitmap(IDB_TEXTURE);	// ���J��ϸ귽 IDB_TEXTURE
	TextureDC.CreateCompatibleDC(pDC);	// �ЫػP�ୱDC�ݮe��DC
	TextureDC.SelectObject(&TextureBMP);// �N��Ͽ�JTextureDC
	DesktopWnd->ReleaseDC(pDC);			// ����ୱ���f��DC
}

void ReleaseGrp(void) // ����ø�Ϥ���
{
	if (MazeArrayBuffer != NULL)// �ˬd�O�_���t�FMazeArrayBuffer
	{
		free(MazeArrayBuffer);
	}
	TextureBMP.DeleteObject();	// �R��TextureBMP�A�����������ϸ귽
	TextureDC.DeleteDC();		// �R��TextureDC�A���������Device Context
	delete BackGroundBMP;		// �R��BackGroundBMP�A�����������ϸ귽
	delete BackGroundDC;		// �R��BackGroundDC�A���������Device Context
}

void InitialMazeData(int width, int height, char DefaultMazeData)
{
	// ��l�ƹw�]���a�ϸ��
	MazeFileHeader.width = width;             // �]�w�ɮ׼e��
	MazeFileHeader.height = height;           // �]�w�ɮװ���
	MazeFileHeader.BeginX = width - 1;        // �]�w�_�I��X�y��
	MazeFileHeader.BeginY = height - 1;       // �]�w�_�I��Y�y��
	MazeFileHeader.EndX = 0;                  // �]�w���I��X�y��
	MazeFileHeader.EndY = 0;                  // �]�w���I��Y�y��
	//MazeFileHeader.totalanswers = 0;          

	int i, yp = 0;
	// �p��XMazeArrayBuffer���}�C��Y�y�Ъ��O�����m�_�l�ƭȡA�Ω�[�tY�y�Эp��A�٥h���k
	for (i = 0; i < MazeFileHeader.height; i++)
	{
		MazeArrayYCache[i] = yp;
		yp += MazeFileHeader.width;
	}
	// �p�GMazeArrayBuffer�w�g���t�F�O����A�h�i����������귽
	if (MazeArrayBuffer != NULL)
	{
		free(MazeArrayBuffer);
		delete BackGroundBMP;
		delete BackGroundDC;
	}
	// ���tMazeArrayBuffer���O����A�ñN���l�Ƭ�DefaultMazeData
	MazeArrayBuffer = (char*)malloc(MazeFileHeader.width * MazeFileHeader.height);
	if (MazeArrayBuffer != NULL)
	{
		memset(MazeArrayBuffer, DefaultMazeData, MazeFileHeader.width * MazeFileHeader.height);
		// �b�_�I�M���I����m�]�w�S�w������
		MazeArrayBuffer[MazeArrayYCache[MazeFileHeader.BeginY] + MazeFileHeader.BeginX] = IDM_BEGIN;
		MazeArrayBuffer[MazeArrayYCache[MazeFileHeader.EndY] + MazeFileHeader.EndX] = IDM_END;
	}
}

void InitialMazeBMPDC( CWnd* m_pMainWnd) // ��l�Ʀa�ϸ�ơA�إߦa�Ϫ������ù��I����
{
	int i;
	CDC* pDC = m_pMainWnd->GetDC();


	BackGroundBMP = new CBitmap;
	BackGroundDC = new CDC;
	
	BackGroundBMP->CreateCompatibleBitmap(pDC, (MazeFileHeader.width + 2) * TEXTUREWIDTH, (MazeFileHeader.height + 2) * TEXTUREHEIGHT); //+2�O���F��ܥ|�䪺���
	BackGroundDC->CreateCompatibleDC(pDC);
	BackGroundDC->SelectObject(BackGroundBMP);

	// �N�a�ϭI�����~�򹺥X�@�����
	for (i = 0; i < (MazeFileHeader.width + 2); i++) // �e�W�U�����
	{
		BackGroundDC->BitBlt((i << 5), 0, TEXTUREWIDTH, TEXTUREHEIGHT, &TextureDC, 0, TEXTUREHEIGHT, SRCCOPY); // (i<<5)=i*TEXTUREWIDTH
		BackGroundDC->BitBlt((i << 5), ((MazeFileHeader.height + 1) << 5), TEXTUREWIDTH, TEXTUREHEIGHT, &TextureDC, 0, TEXTUREHEIGHT, SRCCOPY); // (MazeFileHeader.height<<5)=MazeFileHeader.height*TEXTUREHEIGHT
	}
	for (i = 0; i < MazeFileHeader.height; i++) // �e���k�ݳ���
	{
		BackGroundDC->BitBlt(0, ((i + 1) << 5), TEXTUREWIDTH, TEXTUREHEIGHT, &TextureDC, 0, TEXTUREHEIGHT, SRCCOPY);
		BackGroundDC->BitBlt(((MazeFileHeader.width + 1) << 5), ((i + 1) << 5), TEXTUREWIDTH, TEXTUREHEIGHT, &TextureDC, 0, TEXTUREHEIGHT, SRCCOPY);
	}
	m_pMainWnd->ReleaseDC(pDC);
}

// TiledMap�A�Q��MazeArrayBuffer�W����ƹ����t��X�a�Ϲϼ�
void RenderMazeGraphics(CDC* pDC)
{
	int x, y, xp, yp = 0;
	for (y = 0; y < MazeFileHeader.height; y++)
	{
		yp += TEXTUREHEIGHT; // ������+TEXTUREHEIGHT�A�]���~�򦳤@�h����]��
		xp = 0;
		for (x = 0; x < MazeFileHeader.width; x++)
		{
			xp += TEXTUREWIDTH;	// ������+TEXTUREWIDTH�A�]���~�򦳤@�h����]��
			BackGroundDC->BitBlt(xp, yp, TEXTUREWIDTH, TEXTUREHEIGHT, &TextureDC, 0, ((int)*(MazeArrayBuffer + MazeArrayYCache[y] + x) << 5), SRCCOPY); // (i<<5)=i*TEXTUREHEIGHT
		}
	}
	//��ܷƹ���Щҫ��a�Ϭ���m�W���a�Ϥ�������MazePointX�AMazePointX�t�~��������

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
		if (MazeItemMode == IDM_BEGIN) // �}�l�u�঳�@��
		{
			*(MazeArrayBuffer + MazeArrayYCache[MazeFileHeader.BeginY] + MazeFileHeader.BeginX) = IDM_ROAD;
			MazeFileHeader.BeginX = (MazePointX - 1);
			MazeFileHeader.BeginY = (MazePointY - 1);
			*(MazeArrayBuffer + MazeArrayYCache[MazeFileHeader.BeginY] + MazeFileHeader.BeginX) = IDM_BEGIN;
		}
		else
		{
			if (MazeItemMode == IDM_END) // �����u�঳�@��
			{
				*(MazeArrayBuffer + MazeArrayYCache[MazeFileHeader.EndY] + MazeFileHeader.EndX) = IDM_ROAD;
				MazeFileHeader.EndX = (MazePointX - 1);
				MazeFileHeader.EndY = (MazePointY - 1);
				*(MazeArrayBuffer + MazeArrayYCache[MazeFileHeader.EndY] + MazeFileHeader.EndX) = IDM_END;
			}
			else
			{
				if (*(MazeArrayBuffer + MazeArrayYCache[(MazePointY - 1)] + (MazePointX - 1)) < IDM_BEGIN) // �u���D��������~��o�˧��ܡA�}�l�ε����I����o���л\�R��
				{
					*(MazeArrayBuffer + MazeArrayYCache[(MazePointY - 1)] + (MazePointX - 1)) = (char)MazeItemMode;
				}
			}
		}
	}
}

int ReadMazeFile(wchar_t * FileName) // Ū��Maze�ɮ�
{
	int filehandle = -1;
	_wsopen_s(&filehandle, FileName, _O_RDONLY | _O_BINARY, _SH_DENYNO, _S_IREAD | _S_IWRITE);
	if (filehandle == -1)
	{
		//�}�ɮץ��ѡA�^��-1
		return -1;
	}
	int i, yp = 0;

	_read(filehandle, &MazeFileHeader, sizeof(struct MAZEFILEHEADER)); // Ū�Xstruct MAZEFILEHEADER�o�Ӫ��Y���

	// �p��XMazeArrayBuffer���}�C��Y�y�Ъ��O�����m�_�l�ƭȡA�Ω�[�tY�y�Эp��A�٥h���k
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
	_read(filehandle, MazeArrayBuffer, MazeFileHeader.width * MazeFileHeader.height); //Ū���a�ϸ��
	//MazeArrayBuffer[MazeArrayYCache[MazeFileHeader.BeginY] + MazeFileHeader.BeginX] = IDM_BEGIN;
	//MazeArrayBuffer[MazeArrayYCache[MazeFileHeader.EndY] + MazeFileHeader.EndX] = IDM_END;
	_close(filehandle); //�����ɮ�
	return(0);
}

int WriteMazeFile(wchar_t* FileName) // �x�sMaze�ɮ�
{
	int filehandle = -1;
	_wsopen_s(&filehandle, FileName, _O_WRONLY |_O_CREAT | _O_TRUNC | _O_BINARY, _SH_DENYNO, _S_IREAD | _S_IWRITE);
	if (filehandle == -1)
	{
		//�}�ɮץ��ѡA�^��-1
		return -1;
	}
	_write(filehandle, &MazeFileHeader, sizeof(struct MAZEFILEHEADER)); // �g�Jstruct MAZEFILEHEADER�o�Ӫ��Y���
	_write(filehandle, MazeArrayBuffer, MazeFileHeader.width * MazeFileHeader.height); //�g�J�a�ϸ��
	_close(filehandle); //�����ɮ�
	return(0);
}