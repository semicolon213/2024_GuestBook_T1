#pragma once

#include <windows.h>
#include <commdlg.h> 

/// ���� ���� �� ������ ����� �����ϴ� Ŭ����
class ColorPalette {

private:
    COLORREF acrCustClr[16];  /// ����� ���� ������ �����ϴ� �迭 (16���� ����� ���� ���� ����)
    CHOOSECOLOR cc;           /// ���� ���� ��ȭ ���ڸ� �����ϰ� ȣ��
    HBRUSH hBrush = NULL;     /// ���� ���õ� �������� ������ �귯��, �ʱⰪ�� NULL
    COLORREF colorArr[3] = { RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0) }; /// �� ��ȣ�� ���� ������ ����(3���� ����, �ʱⰪ�� ������)

public:
    /// ���� ���� ��ȭ ���ڸ� ǥ���ϰ� ���õ� ������ �����ϴ� �Լ�
    void colorSelect(HWND, int);

    /// ���õ� ������ ����� â�� ����Ʈ�ϴ� �Լ�
    void paint(PAINTSTRUCT, HDC);

    /// ������ �귯�� ��ü�� �����ϴ� �Լ�
    void destroy();

    /// �� ��ȣ�� ���� ������ ��ȯ�ϴ� �Լ�
    COLORREF getColor(int);
};
