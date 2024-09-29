#pragma once

#include <thread>
#include "Struct.h"

#include "Resource.h"

#include <gdiplus.h> //gdi+ ����� ����ϱ� ���� �������.
#pragma comment (lib, "gdiplus.lib") // GDI+ ���̺귯�� ��ũ

using namespace std;



class Function
{

private:


	HWND hWnd;
	HDC hdc;
	HPEN nPen, oPen;
	HBRUSH hPen;

	int px, py;

	int x, y;

	int bShape = BASIC;

	bool isLeftClick = false;
	bool isReplay = false;

	LINFO drawLInfo;

	thread replayThreadHandle;


	ULONG_PTR gdiplusToken;


	void record(PINFO);

public:
	void replay(HWND);
	void draw(HWND, PINFO, bool);		//�ڿ� �귯�� �߰�
	void mouseUD(PINFO, bool);
	void replayThread(HWND);

	void clearDrawing(HWND);

	void setIsReplay(bool);
	bool getIsReplay();

	bool getDrawLInfoEmpty();

	void setBShape(int);

	void setPenStyle(int, LPARAM, COLORREF);
	void GDIPlusStart(); // gdi+ ���� �Լ� 
	void GDIPlusEnd(); // gdi+ ���� �Լ�
	static Function globalFunction;
	LINFO getDrawLInfo();

};