#include "WndFunc.h"


HWND WndFunc::drowWnd = nullptr;
HWND WndFunc::nameWnd = nullptr;
HWND WndFunc::toolWnd = nullptr;
HWND WndFunc::canvasWnd = nullptr;
//HWND WndFunc::menuWnd = nullptr;
HWND WndFunc::sideWnd = nullptr;
HWND WndFunc::visitListWnd = nullptr;
HWND WndFunc::fileManager = nullptr;
HWND WndFunc::colorWnd = nullptr;

HWND WndFunc::DrowBT = nullptr;
HWND WndFunc::LoadBT = nullptr;
HWND WndFunc::CreditBT = nullptr;

bool WndFunc::buttonOn = true;
bool WndFunc::creditOn = false;
HWND WndFunc::fileNameW = nullptr;

HINSTANCE WndFunc::hInst = nullptr;
RECT WndFunc::wndSize = { 0, 0, 0, 0 };



RECT WndFunc::GetRT(HWND hWnd)
{
    if (!IsWindow(hWnd)) {  // hWnd�� ��ȿ���� Ȯ��
        //MessageBox(NULL, L"��ȿ���� ���� â �ڵ��Դϴ�.", L"����", MB_OK);
        return RECT{ 0, 0, 0, 0 };  // �ʱ�ȭ�� RECT ��ȯ
    }

    GetClientRect(hWnd, &this->rect);
    return this->rect;
}

void WndFunc::RegisterNewClass(HINSTANCE hInst, LPCWSTR className)
{
    WNDCLASS wc = {};
    wc.lpfnWndProc = DefWindowProc;  // �⺻ �޽��� ó����
    wc.hInstance = hInst;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = className;

    RegisterClass(&wc);
}