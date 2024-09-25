#include "ColorPalette.h" /// colorpalette.h ��� ������ �����Ѵ�

COLORREF ColorPalette::colorArr[] = { RGB(0,0,0),RGB(0,0,0), RGB(0,0,0) };

void ColorPalette::colorSelect(HWND hWnd, int penNum)
{
	ZeroMemory(&cc, sizeof(cc)); /// ���� ���� â ����, ȣ���� �⺻������ ����
	cc.lStructSize = sizeof(cc); /// ����ü�� ũ�� ����
	cc.hwndOwner = hWnd; /// ������ �ڵ� ����
	cc.lpCustColors = (LPDWORD)acrCustClr; /// lpCustColors : ����� ���� ���� �迭�� �ּ�
	cc.rgbResult = RGB(0, 0, 0); /// ���� â�� ó�� ���� �� �������� ���� (�ʱ� ���� : ������)
	cc.Flags = CC_FULLOPEN | CC_RGBINIT; /// ��ȭ ���� �÷��� ����

	if (ChooseColor(&cc))			/// ���� ���� ��ȭ ���� ǥ��
	{ 
		colorArr[penNum] = cc.rgbResult;
	}
}



COLORREF ColorPalette::getColor(int penNum) {
	return ColorPalette::colorArr[penNum];
}