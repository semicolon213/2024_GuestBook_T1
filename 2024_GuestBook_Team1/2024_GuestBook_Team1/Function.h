#pragma once

#include <thread>
#include "Struct.h"

#include "Resource.h"

#include <gdiplus.h> //gdi+ ����� ����ϱ� ���� �������.
#pragma comment (lib, "gdiplus.lib") // GDI+ ���̺귯�� ��ũ
#include <cmath> // sqrt(���� ��) ����� ���� �߰�
#include <chrono> // �� �귯�� ����� ���� �ð� ���� ���̺� ����

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

	// �� �귯�� ���� 
	std::chrono::steady_clock::time_point DrawTime; // �׸��� ������ �ð�
	std::chrono::steady_clock::time_point lastThicknessChangeTime;
	const int Min_Thickness = 4;  // �ּ� �β�
	const int Threshold_Speed = 600; // �ӵ� �Ӱ谪 (�ȼ�/��)
	const int Smoothing_Factor = 2; // �β� ��ȯ �� �ε巯�� ����
	const int Update_Interval = 10; // �β� ������Ʈ ���� (�и���)

	int bShape = BRUSH; // �귯�� ���� ��ư ��� �ش� �ڵ忡�� ���� �־ ���.

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

	void setPenStyle(int, PINFO, COLORREF,int); // LPARAM �μ� ���� PINFO �� �ٲ�
	void GDIPlusStart(); // gdi+ ���� �Լ� 
	void GDIPlusEnd(); // gdi+ ���� �Լ�

	LINFO getDrawLInfo();

};