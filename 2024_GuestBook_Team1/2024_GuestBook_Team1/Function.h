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

	HDC hdc;
	HPEN nPen, oPen;
	HBRUSH hPen;

	int px, py;

	int x, y;

	int bShape = BASIC;

	bool isLeftClick = false;
	bool isReplay = false;
	bool reset;

	ULONG_PTR gdiplusToken;

	void replay(HWND);
	void record(PINFO);

public:
	thread replayThreadHandle;

	void draw(HWND, PINFO, bool);		//�ڿ� �귯�� �߰�
	void mouseUD(PINFO, bool);
	void replayThread(HWND);

	void clearDrawing(HWND);
	void reDrawing(HWND);

	void setIsReplay(bool);
	bool getIsReplay();
	void setIsReset(bool);
	bool getIsReset();

	void stopReplay(HWND);

	bool getDrawLInfoEmpty();

	void setBShape(int);

	void setPenStyle(int, LPARAM, COLORREF);
	void GDIPlusStart(); // gdi+ ���� �Լ� 
	void GDIPlusEnd(); // gdi+ ���� �Լ�

	LINFO getDrawLInfo();

	static HWND hWnd;
	static LINFO drawLInfo;
};