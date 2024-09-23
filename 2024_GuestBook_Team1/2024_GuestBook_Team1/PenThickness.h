#ifndef PENTHICKNESS_H  // ��� ������ ���� �� ���ԵǴ� ���� �����ϱ� ���� ���ǵ� ��ũ��
#define PENTHICKNESS_H  // ��� ������ ������ ����

#include <Windows.h>  /// Windows API�� ���� ���Ǹ� �����ϴ� ��� ����
#include <vector>     

class PenThickness  /// PenThickness Ŭ������ ����
{
public:
    PenThickness(HINSTANCE hInstance, HWND parentHWnd);  
    void Show(bool show);  
    void SetThickness(int thickness);  /// �� �β��� �����ϴ� �޼���
    void SetCurrentThickness(int thickness);  /// ���� �β��� �����ϴ� �޼���

private:
    HWND hDlg;  
    HINSTANCE hInst;  
    HWND parentHWnd;  
    int currentThickness;  /// ���� �� �β�

    void InitializeDialog();  /// ��ȭ ���ڸ� �ʱ�ȭ�ϴ� �޼���
    static INT_PTR CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);  
    void DrawThicknessPreview(HDC hdc);  
};

#endif 