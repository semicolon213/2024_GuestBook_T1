#pragma once

#include <thread>
#include "Struct.h"

#include "Resource.h"

#include <gdiplus.h> //gdi+ 기능을 사용하기 위한 헤더파일.
#pragma comment (lib, "gdiplus.lib") // GDI+ 라이브러리 링크

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
	void draw(HWND, PINFO, bool);		//뒤에 브러쉬 추가
	void mouseUD(PINFO, bool);
	void replayThread(HWND);

	void clearDrawing(HWND);

	void setIsReplay(bool);
	bool getIsReplay();

	bool getDrawLInfoEmpty();

	void setBShape(int);

	void setPenStyle(int, LPARAM, COLORREF);
	void GDIPlusStart(); // gdi+ 시작 함수 
	void GDIPlusEnd(); // gdi+ 종료 함수
	static Function globalFunction;
	LINFO getDrawLInfo();

};