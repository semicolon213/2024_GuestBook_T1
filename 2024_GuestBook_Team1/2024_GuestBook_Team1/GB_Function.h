#pragma once

#include <thread>
#include "GB_struct.h"

using namespace std;

static int px, py;
class GB_Function
{
private :
	HWND hWnd;
	PINFO drawPInfo;
	HDC hdc;
	HPEN nPen, oPen;

	int x, y;

	bool isLeftClick = false;

	
public :
	GB_Function() = default;
	~GB_Function() = default;

	void record(LPARAM, ULONGLONG, UINT, int, COLORREF);

	void draw(HWND, LPARAM, ULONGLONG, UINT, int, COLORREF);

	void mouseUD(LPARAM, ULONGLONG, UINT, int, COLORREF);

	LINFO drawLInfo;
	
	void replay(HWND);

	void replayThread(HWND);

	bool isTerminate = false;

	thread replayThreadHandle;
};