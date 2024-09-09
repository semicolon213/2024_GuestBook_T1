#pragma once

#include <thread>
#include "GB_struct.h"
// �귯�� ���� ��ư (�̸� ���� �ʿ�)
#define DRAW_BRUSH 300
#define DRAW_PENCIL 301 
#define DRAW_SPRAY 302
#define DRAW_MARKER 303
#define DRAW_PEN 304
#define DRAW_FEATHER 305



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

	void draw(HWND, LPARAM, ULONGLONG, UINT, int, COLORREF, int);

	void mouseUD(LPARAM, ULONGLONG, UINT, int, COLORREF);

	LINFO drawLInfo;
	
	void replay(HWND);

	void replayThread(HWND);

	bool isTerminate = false;

	thread replayThreadHandle;
};