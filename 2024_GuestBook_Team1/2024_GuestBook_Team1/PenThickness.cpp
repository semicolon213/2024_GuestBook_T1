#include "PenThickness.h"  /// PenThickness Ŭ������ ��� ������ ����
#include "Resource.h"      /// ���ҽ� ���Ǹ� �����ϴ� ��� ������ ����
#include <CommCtrl.h>      /// ���� ��Ʈ�ѿ� ���� ���Ǹ� �����ϴ� ��� ������ ����

PenThickness::PenThickness(HINSTANCE hInstance, HWND parentHWnd)
    : hInst(hInstance), parentHWnd(parentHWnd), currentThickness(1)  /// ������, �ν��Ͻ� �ڵ�� �θ� ������ �ڵ��� �ʱ�ȭ, �⺻ �β��� 1�� ����
{
    InitializeDialog();  /// ��ȭ ���ڸ� �ʱ�ȭ
}

void PenThickness::InitializeDialog()
{
    hDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_PEN_THICKNESS_DIALOG), parentHWnd, DialogProc);  /// ��ȭ ���ڸ� ����
    SetWindowLongPtr(hDlg, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));  /// ��ȭ ���ڿ� ����� �����͸� ����
    ShowWindow(hDlg, SW_SHOW);  /// ��ȭ ���ڸ� ȭ�鿡 ǥ��
}

void PenThickness::Show(bool show)
{
    if (show)
        ShowWindow(hDlg, SW_SHOW);  /// show�� true�̸� ��ȭ ���ڸ� ǥ��
    else
        ShowWindow(hDlg, SW_HIDE);  /// show�� false�̸� ��ȭ ���ڸ� ����
}

void PenThickness::SetThickness(int thickness)
{
    currentThickness = thickness;  /// ���� �β��� ����
    InvalidateRect(hDlg, NULL, TRUE);  /// ��ȭ ������ Ŭ���̾�Ʈ ������ ��ȿȭ�Ͽ� �ٽ� �׷������� ��û
}

void PenThickness::SetCurrentThickness(int thickness)
{
    currentThickness = thickness;  /// ���� �β��� ����
    /// �̰��� ���� �β��� �����ϴ� ������ �߰�
    /// ��: ���� ����� �������� �β��� �����ϴ� �Լ� ȣ��
}

void PenThickness::DrawThicknessPreview(HDC hdc)
{
    /// ���� �� �β��� �̸����⸦ �׸��ϴ�.
    HPEN hPen = CreatePen(PS_SOLID, currentThickness, RGB(0, 0, 0));  /// ���� �β��� ���� ����
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));  /// ��� �귯�ø� ����
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);  /// ���� �귯�ø� �����ϰ� �� �귯�ø� ����
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);  /// ���� ���� �����ϰ� �� ���� ����

    // �̸����� �簢���� �׸��ϴ�.
    Rectangle(hdc, 10, 10, 60, 60);  /// (10, 10)���� (60, 60)������ �簢���� �׸�

    SelectObject(hdc, hOldPen);  /// ���� ���� ����
    SelectObject(hdc, hOldBrush);  /// ���� �귯�ø� ����
    DeleteObject(hPen);  /// ������ ���� �����Ͽ� ���ҽ��� ����
    DeleteObject(hBrush);  /// ������ �귯�ø� �����Ͽ� ���ҽ��� ����
}

INT_PTR CALLBACK PenThickness::DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    PenThickness* pThis = reinterpret_cast<PenThickness*>(GetWindowLongPtr(hDlg, GWLP_USERDATA));  /// ��ȭ ������ ����� �����͸� ������ PenThickness ��ü�� ����

    switch (message)
    {
    case WM_INITDIALOG:
        return TRUE;  /// ��ȭ ���ڰ� �ʱ�ȭ�Ǹ� TRUE�� ��ȯ�Ͽ� �⺻ ó���� ����

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
            /// Ȯ�� ��ư Ŭ�� ��
            pThis->SetCurrentThickness(GetDlgItemInt(hDlg, IDC_THICKNESS_EDIT, NULL, TRUE));  /// �β��� �����ͼ� ����
            EndDialog(hDlg, IDOK);  /// ��ȭ ���ڸ� �����ϰ� IDOK�� ��ȯ�մϴ�.
            return TRUE;

        case IDCANCEL:
            EndDialog(hDlg, IDCANCEL);  /// ��� ��ư Ŭ�� �� ��ȭ ���ڸ� �����ϰ� IDCANCEL�� ��ȯ
            return TRUE;

        case IDC_THICKNESS_SLIDER:
            /// �β� ���� �����̴� ���� ����� ��
            int thickness = SendMessage(GetDlgItem(hDlg, IDC_THICKNESS_SLIDER), TBM_GETPOS, 0, 0);  /// �����̴��� ���� ���� ������
            pThis->SetThickness(thickness);  /// �β��� ����
            return TRUE;
        }
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hDlg, &ps);  /// ��ȭ ������ Ŭ���̾�Ʈ ������ �׸��� ����
        pThis->DrawThicknessPreview(hdc);  /// ���� �β��� �̸����⸦ �׸�
        EndPaint(hDlg, &ps);  /// �׸��⸦ ����
        return TRUE;
    }

    }
    return FALSE;  /// ó������ ���� �޽����� FALSE�� ��ȯ
}
