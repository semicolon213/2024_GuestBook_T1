#pragma once

#include <windows.h>
#include <commdlg.h> /// ���� ������ ����ϱ����� API 

class ColorPalette {

private:
	COLORREF acrCustClr[16];	/// ����� ���� ���� ���� ����
	CHOOSECOLOR cc;				/// ���� ���� â�� ����, ȣ���ϴ� ����ü
	HBRUSH hBrush = NULL;		/// ������ �����ϴ� �귯�� �ʱⰪ NULL
	COLORREF colorArr[3] = { RGB(0,0,0),RGB(0,0,0), RGB(0,0,0) }; /// �� ��ȣ�� ���� ���� ���� �迭 �ʱⰪ ������

public:
	void colorSelect(HWND, int); /// ���� ���� ��ȭ ���ڸ� ǥ��, ���õ� ���� ����
	void paint(PAINTSTRUCT, HDC); /// ���õ� ������ ����� â�� ����Ʈ
	void destroy(); /// ������ �귯�� ��ü�� ����

	COLORREF getColor(int); /// �� ��ȣ�� ���� ���� ��ȯ

};