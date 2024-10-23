#ifndef PENTHICKNESS_H
#define PENTHICKNESS_H

#include <Windows.h>
#include <CommCtrl.h>
#include <CommDlg.h>  // ���� ������ ���� �߰�
#include "resource.h"

/// �� ����� ������ �����ϱ� ���� ��ȭ ����
class PenThickness {
public:
    PenThickness();
    ~PenThickness();

    /// �� ���� �� ���� ���� ��ȭ ���ڸ� ǥ��
    static void show(HINSTANCE hInst, HWND hWndParent);

    /// ���� ������ �� ���⸦ ��ȯ
    static int getPenWidth();

    /// ���� ������ �� ������ ��ȯ
    static COLORREF getPenColor();

    /// ��ȭ ���ڿ��� �߻��ϴ� �޽����� ó��
    static INT_PTR CALLBACK dlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

private:
    /// �����̴� ����ϱ� ���� ���̺귯���� �ʱ�ȭ
    static void initializeCommonControls();

    /// �� �̸����⸦ ������Ʈ�ϴ� �Լ�
    static void updatePreview(HWND hDlg);

    /// ���� ���� ��ȭ ���ڸ� ���� �Լ�
    static void chooseColor(HWND hDlg);

    /// �߰� ���� ����
    static int penWidth;
    static COLORREF penColor;
};

#endif
