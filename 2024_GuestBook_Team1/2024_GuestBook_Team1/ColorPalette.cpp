#include "ColorPalette.h"

/// ����ڰ� ���� ���� ��ȭ ���ڸ� ���� ������ �����ϰ�, �̸� ���� �迭�� �����ϴ� �Լ�
void ColorPalette::colorSelect(HWND hWnd, int penNum)
{
    ZeroMemory(&cc, sizeof(cc));
    cc.lStructSize = sizeof(cc); /// cc ����ü�� ũ�� ����
    cc.hwndOwner = hWnd; 
    cc.lpCustColors = (LPDWORD)acrCustClr; 
    cc.rgbResult = RGB(0, 0, 0); 
    cc.Flags = CC_FULLOPEN | CC_RGBINIT; 

    if (ChooseColor(&cc)) { /// ���� ���� ��ȭ ���� ȣ��, ����ڰ� ������ �����ϸ� TRUE ��ȯ
        if (hBrush) { 
            DeleteObject(hBrush); 
        }
        hBrush = CreateSolidBrush(cc.rgbResult); 
        colorArr[penNum] = cc.rgbResult; 
        InvalidateRect(hWnd, NULL, TRUE); 
    }
}

/// ���õ� ������ ����Ͽ� �����츦 ����Ʈ�ϴ� �Լ�
void ColorPalette::paint(PAINTSTRUCT ps, HDC hdc)
{
    if (hBrush)
    {
        FillRect(hdc, &ps.rcPaint, hBrush); 
    }
}

/// �귯�� ��ü�� �����Ͽ� ���ҽ��� ����
void ColorPalette::destroy()
{
    if (hBrush)
    {
        DeleteObject(hBrush); 
    }
}

COLORREF ColorPalette::getColor(int penNum) {
    return colorArr[penNum]; /// �� ��ȣ�� �´� ������ ��ȯ
}
