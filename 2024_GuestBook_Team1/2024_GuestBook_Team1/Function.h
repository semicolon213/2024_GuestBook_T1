#pragma once

#include <thread>
#include "Struct.h"

#include "Resource.h"

#include <gdiplus.h> //gdi+ 기능을 사용하기 위한 헤더파일.
#pragma comment (lib, "gdiplus.lib") // GDI+ 라이브러리 링크

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
	void GDIPlusStart(); // gdi+ 시작 함수 
	void GDIPlusEnd(); // gdi+ 종료 함수

	void setIsTerminate(bool);
	LINFO getDrawLInfo();
	thread replayThreadHandle;

};