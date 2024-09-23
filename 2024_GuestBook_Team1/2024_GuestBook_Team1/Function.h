#pragma once

#include <thread>
#include "Struct.h"

#include "Resource.h"


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

	void replay(HWND);
	void record(PINFO);

public:

	void draw(HWND, PINFO, bool);		//뒤에 브러쉬 추가
	void mouseUD(PINFO, bool);
	void replayThread(HWND);

	void clearDrawing(HWND);

	void setIsReplay(bool);
	bool getIsReplay();

	bool getDrawLInfoEmpty();

	void setBShape(int);

	void setPenStyle(int, LPARAM, COLORREF);

	LINFO getDrawLInfo();

};