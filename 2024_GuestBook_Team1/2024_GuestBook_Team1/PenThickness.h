#ifndef PENTHICKNESS_H
#define PENTHICKNESS_H

#include <Windows.h>
#include <CommCtrl.h>  
#include <functional>
class PenThickness
{
public:

    PenThickness();
    ~PenThickness();

    void show(HINSTANCE hInst, HWND hWndParent);
    /// ���� ������ �� ���⸦ ��ȯ
    static int getPenWidth();
    /// ���⸦ ���� �����ϵ��� �޼��� �߰�
    static void setPenWidth(int width);
    // �β� ���� �� ȣ��� �ݹ� �Լ� ���
    void setThicknessChangedCallback(std::function<void(int)> callback);


private:
    static INT_PTR CALLBACK dlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

    void initializeCommonControls();

    static int penWidth;

    static std::function<void(int)> thicknessChangedCallback;
};
#endif
