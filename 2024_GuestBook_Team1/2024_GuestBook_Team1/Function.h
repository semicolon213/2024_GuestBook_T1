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

	LINFO drawLInfo;

	void replay(HWND);
	void record(LPARAM, ULONGLONG, UINT, int, COLORREF);

public:

	void draw(HWND, LPARAM, ULONGLONG, UINT, int , COLORREF); //뒤에 브러쉬 추가
	void mouseUD(LPARAM, ULONGLONG, UINT, int, COLORREF);
	void setIsTerminate(bool);
	void replayThread(HWND);
	void setBShape(int);
	void setPenStyle(int, LPARAM, COLORREF);

	LINFO getDrawLInfo();
	thread replayThreadHandle;

};