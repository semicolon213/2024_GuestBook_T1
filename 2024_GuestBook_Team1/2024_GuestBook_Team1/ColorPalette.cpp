#include "ColorPalette.h" /// colorpalette.h ��� ������ �����Ѵ�

void ColorPalette::colorSelect(HWND hWnd, int penNum)
{
	ZeroMemory(&cc, sizeof(cc)); /// ���� ���� â ����, ȣ���� �⺻������ ����
	cc.lStructSize = sizeof(cc); /// ����ü�� ũ�� ����
	cc.hwndOwner = hWnd; /// ������ �ڵ� ����
	cc.lpCustColors = (LPDWORD)acrCustClr; /// lpCustColors : ����� ���� ���� �迭�� �ּ�
	cc.rgbResult = RGB(0, 0, 0); /// ���� â�� ó�� ���� �� �������� ���� (�ʱ� ���� : ������)
	cc.Flags = CC_FULLOPEN | CC_RGBINIT; /// ��ȭ ���� �÷��� ����

	if (ChooseColor(&cc)) { /// ���� ���� ��ȭ ���� ǥ��
		if (hBrush) {
			DeleteObject(hBrush); /// ���� �귯�� ����
		}
		hBrush = CreateSolidBrush(cc.rgbResult); /// ���ο� �귯�� ����
		colorArr[penNum] = cc.rgbResult;
		InvalidateRect(hWnd, NULL, TRUE);
	}
}

void ColorPalette::paint(PAINTSTRUCT ps, HDC hdc)
{
	if (hBrush)
	{
		FillRect(hdc, &ps.rcPaint, hBrush);
	}

}

void ColorPalette::destroy()
{
	if (hBrush)
	{
		DeleteObject(hBrush); /// �귯�� ����
	}
}

COLORREF ColorPalette::getColor(int penNum) {
	return colorArr[penNum];
}