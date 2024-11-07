#pragma once

#include <thread>
#include "Struct.h"
#include "WndFunc.h"
#include "Resource.h"
#include <gdiplus.h> //gdi+ ����� ����ϱ� ���� �������.
#pragma comment (lib, "gdiplus.lib") // GDI+ ���̺귯�� ��ũ
#include <cmath> // sqrt(���� ��) ����� ���� �߰�
#include <chrono> // �� �귯�� ����� ���� �ð� ���� ���̺� ����

class Function
{

private:

	HDC hdc;
	HPEN nPen, oPen;
	HBRUSH hPen;
	RECT clientRect;
	

	int px, py;
	int px2, py2;
	int x, y, x2, y2;
	int px3, py3;

	bool stampOn = false;	// ������ ��� Ȱ��ȭ ����
	static int stampID;

	// �� �귯�� ���� 
	std::chrono::steady_clock::time_point DrawTime; // ��Ŭ�� �� �ð�
	std::chrono::steady_clock::time_point currentTime; // �׸��� ������ �ð�
	std::chrono::steady_clock::time_point lastThicknessChangeTime; // �β� ���ϴ� �ð�
	const int Min_Thickness = 2;  // �ּ� �β�
	const int Threshold_Speed = 450; // �ӵ� �Ӱ谪 (�� ���� ���� ������ �׷��� �F����)
	const int Smoothing_Factor = 1; // �β� ��ȯ �� �ε巯�� ���� (�� Ŭ���� Ȯ �پ��)
	const int Update_Interval = 30; // �β� ������Ʈ ���� (�и���) (�� Ŭ���� ũ�� ���ϴ� �ֱⰡ �پ��)
	int currentThickness = 0; // �� �귯�� ũ�Ⱑ �ٲ�� ���� (�ʱ�ȭ �ҷ��� 0 �������)
	//������� �귯�� ���� �߰�


	bool isLeftClick = false;
	bool isReplay = false;
	bool isReset = true;
	bool isCanvas = true;

	ULONG_PTR gdiplusToken;

	void replay(HWND);
	void record(PINFO);

	HDC cHdc = nullptr;
	HPEN CanvasPen = nullptr;
	PAINTSTRUCT cPS = { 0 };

	std::thread replayThreadHandle;
	HANDLE threadHandle;
public:
	static int bShape; // �귯�� ���� ��ư ��� �ش� �ڵ忡�� ���� �־ ���.
	void setisLeftClick(bool);
	bool getisLeftClick();

	void setisCanvas(bool, LPARAM);

	void draw(HWND, PINFO, bool);		//�ڿ� �귯�� �߰�
	void mouseUD(PINFO, bool);
	void replayThread(HWND);

	void clearDrawing(HWND);
	void reDrawing(HWND);

	void setIsReplay(bool);
	bool getIsReplay();
	void setIsReset(bool);
	bool getIsReset();

	void suspendReplay();
	void resumeReplay();

	void stopReplay(HWND);

	bool getDrawLInfoEmpty();

	void setBShape(int);

	void setPenStyle(PINFO, COLORREF); // LPARAM �μ� ���� PINFO �� �ٲ�
	void GDIPlusStart(); // gdi+ ���� �Լ� 
	void GDIPlusEnd(); // gdi+ ���� �Լ�

	void paint(HDC, RECT, PAINTSTRUCT);

	void stampIcon(int);

	void re_draw(HDC phdc, PINFO pino, HWND hd);

	LINFO getDrawLInfo();

	static HWND hWnd;
	static LINFO drawLInfo;

	static int penNum;


};