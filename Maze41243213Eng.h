#pragma once

#include "afxwin.h"
#include "MazeDataHeader.h"

extern double DisplayTime; // ������s��ܪ��ɶ�

extern struct MAZEFILEHEADER MazeFileHeader; // �a�Ϫ����Y��Ƶ��c
extern char* MazeArrayBuffer; // �a�ϸ�ƪ��ʺA�t�m�O����

extern CBitmap* BackGroundBMP, TextureBMP; // BackGroundBMP:�a�Ϫ������ù��I���ATextureBMP:�a�Ϫ����誺�ϧ�
extern CDC* BackGroundDC, TextureDC; // BackGroundDC:�a�Ϫ������ù��I��ø�s����ATextureDC:�a�Ϫ����誺ø�s����
extern int MazePointX, MazePointY; // �ƹ�����a�Ϫ�XY�y��
extern int MazeItemMode; // �ثe��J�a�Ϫ��˦��A�w�]�O�i�q�L����

int GetPathW(wchar_t* wSrcPath, wchar_t* wDestPath = NULL, int upf = 0); // ���ɦW�����|�A�^�Ǹ��|���סA�Ѽ�upf=1�N�O��j�g
void ResetDisplayTime(void); // ���]��ܨC��i�Ʊ���ɶ�
void InitializeGrp(void); // ��l��ø�Ϥ���
void ReleaseGrp(void); // ����ø�Ϥ���

void InitialMazeData(int width, int height, char DefaultMazeData);	// ��l�Ʀa�ϸ��
void InitialMazeBMPDC(CWnd* m_pMainWnd); // �إߦa�Ϫ������ù��I����
void RenderMazeGraphics(CDC* pDC); // ��s�a�Ϫ��ϧΪ��A
void EraseBackGround(CDC* pDC); //�º��_�a�Ϫ��ϧ�

void SetMazeArrayData(void); // �̥ثe�ƹ�������m���w���ثe�諸�a�Ϥ���

int ReadMazeFile(wchar_t* FileName); // Ū��Maze�ɮ�
int WriteMazeFile(wchar_t* FileName); // �x�sMaze�ɮ�