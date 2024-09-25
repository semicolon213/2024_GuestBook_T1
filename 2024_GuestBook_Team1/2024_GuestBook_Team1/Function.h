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

	int bShape = MARKER; // �귯�� ���� ��ư ��� �ش� �ڵ忡�� ���� �־ ���.

	bool isLeftClick = false;
	bool isReplay = false;

	LINFO drawLInfo;

	thread replayThreadHandle;


	ULONG_PTR gdiplusToken;

	void replay(HWND);
	void record(PINFO);

public:

	void draw(HWND, PINFO, bool);		//�ڿ� �귯�� �߰�
	void mouseUD(PINFO, bool);
	void replayThread(HWND);

	void clearDrawing(HWND);

	void setIsReplay(bool);
	bool getIsReplay();

	bool getDrawLInfoEmpty();

	void setBShape(int);

	void setPenStyle(int, PINFO, COLORREF); // LPARAM �μ� ���� PINFO �� �ٲ�
	void GDIPlusStart(); // gdi+ ���� �Լ� 
	void GDIPlusEnd(); // gdi+ ���� �Լ�

	LINFO getDrawLInfo();

};