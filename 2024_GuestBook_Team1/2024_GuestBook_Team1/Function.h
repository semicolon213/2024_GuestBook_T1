#pragma once

#include <thread>
#include "Struct.h"

#include "Resource.h"


using namespace std;

static int px, py;

class Function
{

private:

	HWND hWnd;
	PINFO drawPInfo;
	HDC hdc;
	HPEN nPen, oPen;
	HBRUSH hPen;

	int x, y;

	int bShape = BASIC;

	bool isLeftClick = false;
	bool isTerminate = false;
	bool isReplay = false;

	LINFO drawLInfo;

	void replay(HWND);
	void record(LPARAM, ULONGLONG, UINT, int, COLORREF);

public:

	void draw(HWND, LPARAM, ULONGLONG, UINT, int , COLORREF); //뒤에 브러쉬 추가
	void mouseUD(LPARAM, ULONGLONG, UINT, int, COLORREF);
	void replayThread(HWND);

	void setIsReplay(bool);
	bool getIsReplay();

	void setBShape(int);

	void setPenStyle(int, LPARAM, COLORREF);

	void setIsTerminate(bool);
	LINFO getDrawLInfo();
	thread replayThreadHandle;

};