#ifndef PENTHICKNESS_H
#define PENTHICKNESS_H

#include <Windows.h>
#include <CommCtrl.h>  
/// �� ���⸦ �����ϱ� ���� ��ȭ ����
class PenThickness {
public:
    PenThickness(); 
    ~PenThickness();

    /// �� ���� ���� ��ȭ ���ڸ� ǥ��
    void Show(HINSTANCE hInst, HWND hWndParent);

    /// ���� ������ �� ���⸦ ��ȯ
    static int GetPenWidth();

private:
    /// ��ȭ ���ڿ��� �߻��ϴ� �޽����� ó��
    static INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

    /// (�����̴� ����ϱ� ���� ���̺귯���� �ʱ�ȭ
    void InitializeCommonControls();

    /// �߰� ���� ����
    static int penWidth;
};

#endif
