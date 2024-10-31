#include "PenThickness.h"
#include "resource.h"
#include <CommCtrl.h>

int PenThickness::penWidth = 1;
std::function<void(int)> PenThickness::thicknessChangedCallback = nullptr;

INT_PTR CALLBACK PenThickness::dlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_INITDIALOG:
        SendDlgItemMessage(hDlg, IDC_THICKNESS_SLIDER, TBM_SETRANGE, TRUE, MAKELPARAM(1, 20));  // �� ���� ���� 1 ~ 20���� ���� �� �ø��ų� ���� �� ����
        SendDlgItemMessage(hDlg, IDC_THICKNESS_SLIDER, TBM_SETPOS, TRUE, penWidth);
        SetDlgItemInt(hDlg, IDC_THICKNESS_EDIT, penWidth, FALSE);
        return TRUE;  // ��ȭ ���ڰ� ���������� �ʱ�ȭ�Ǿ����� ��ȯ

    case WM_HSCROLL:  // �����̴��� ������ �� �߻��ϴ� �޽��� ó��
        if (LOWORD(wParam) == TB_THUMBTRACK || LOWORD(wParam) == TB_ENDTRACK) {
            penWidth = SendDlgItemMessage(hDlg, IDC_THICKNESS_SLIDER, TBM_GETPOS, 0, 0);
            SetDlgItemInt(hDlg, IDC_THICKNESS_EDIT, penWidth, FALSE);
        }
        return TRUE;

    case WM_COMMAND:  // ��ư Ŭ�� �� ����� ó��
        switch (LOWORD(wParam)) {
        case IDOK:
            penWidth = GetDlgItemInt(hDlg, IDC_THICKNESS_EDIT, NULL, FALSE);
            if (thicknessChangedCallback) {
                thicknessChangedCallback(penWidth); // �β� ���� �ݹ� ȣ��
            }
            EndDialog(hDlg, IDOK);
            return TRUE;
        case IDC_CLOSE_BUTTON:
            EndDialog(hDlg, IDC_CLOSE_BUTTON);
            return TRUE;
        }
        break;
    }
    return FALSE;
}
