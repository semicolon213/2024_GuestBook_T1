#include "PenThickness.h"
#include "resource.h"
<<<<<<< Updated upstream
=======
#include <CommCtrl.h>  
>>>>>>> Stashed changes

int PenThickness::penWidth = 1;

PenThickness::PenThickness() {}

PenThickness::~PenThickness() {}

// �� ���� ���� ��ȭ ���ڸ� ǥ���ϴ� �Լ�
void PenThickness::Show(HINSTANCE hInst, HWND hWndParent) {
<<<<<<< Updated upstream
    InitializeCommonControls(); 
    DialogBox(hInst, MAKEINTRESOURCE(IDD_PEN_THICKNESS), hWndParent, DlgProc);  
=======
    InitializeCommonControls();
    DialogBox(hInst, MAKEINTRESOURCE(IDD_PEN_THICKNESS), hWndParent, DlgProc);
>>>>>>> Stashed changes
}

// �����̴��� ���� ���� ��Ʈ���� ����� �� �ֵ��� �ʱ�ȭ�ϴ� �Լ�
void PenThickness::InitializeCommonControls() {
<<<<<<< Updated upstream
    INITCOMMONCONTROLSEX icex; 
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);  
    icex.dwICC = ICC_BAR_CLASSES;  
    InitCommonControlsEx(&icex);  
=======
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_BAR_CLASSES;
    InitCommonControlsEx(&icex);
>>>>>>> Stashed changes
}

// ��ȭ ���� ���ν���, ��ȭ ���ڿ��� �߻��ϴ� �޽����� ó��
INT_PTR CALLBACK PenThickness::DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
<<<<<<< Updated upstream
    case WM_INITDIALOG:  
        SendDlgItemMessage(hDlg, IDC_THICKNESS_SLIDER, TBM_SETRANGE, TRUE, MAKELPARAM(1, 20));  // �� ���� ���� 1 ~ 20���� ���� �� �ø��ų� ���� �� ����
        SendDlgItemMessage(hDlg, IDC_THICKNESS_SLIDER, TBM_SETPOS, TRUE, penWidth);  
        SetDlgItemInt(hDlg, IDC_THICKNESS_EDIT, penWidth, FALSE);  
        return TRUE;  // ��ȭ ���ڰ� ���������� �ʱ�ȭ�Ǿ����� ��ȯ

    case WM_HSCROLL:  // �����̴��� ������ �� �߻��ϴ� �޽��� ó��
        if (LOWORD(wParam) == TB_THUMBTRACK || LOWORD(wParam) == TB_ENDTRACK) {  
            penWidth = SendDlgItemMessage(hDlg, IDC_THICKNESS_SLIDER, TBM_GETPOS, 0, 0);  
            SetDlgItemInt(hDlg, IDC_THICKNESS_EDIT, penWidth, FALSE);  
=======
    case WM_INITDIALOG:
        SendDlgItemMessage(hDlg, IDC_THICKNESS_SLIDER, TBM_SETRANGE, TRUE, MAKELPARAM(1, 20));  // �� ���� ���� 1 ~ 20���� ���� �� �ø��ų� ���� �� ����
        SendDlgItemMessage(hDlg, IDC_THICKNESS_SLIDER, TBM_SETPOS, TRUE, penWidth);
        SetDlgItemInt(hDlg, IDC_THICKNESS_EDIT, penWidth, FALSE);
        return TRUE;  // ��ȭ ���ڰ� ���������� �ʱ�ȭ�Ǿ����� ��ȯ

    case WM_HSCROLL:  // �����̴��� ������ �� �߻��ϴ� �޽��� ó��
        if (LOWORD(wParam) == TB_THUMBTRACK || LOWORD(wParam) == TB_ENDTRACK) {
            penWidth = SendDlgItemMessage(hDlg, IDC_THICKNESS_SLIDER, TBM_GETPOS, 0, 0);
            SetDlgItemInt(hDlg, IDC_THICKNESS_EDIT, penWidth, FALSE);
>>>>>>> Stashed changes
        }
        return TRUE;

    case WM_COMMAND:  // ��ư Ŭ�� �� ����� ó��
        switch (LOWORD(wParam)) {
<<<<<<< Updated upstream
        case IDOK: 
            penWidth = GetDlgItemInt(hDlg, IDC_THICKNESS_EDIT, NULL, FALSE); 
            EndDialog(hDlg, IDOK);  
            return TRUE;

        case IDCANCEL:  
            EndDialog(hDlg, IDCANCEL); 
=======
        case IDOK:
            penWidth = GetDlgItemInt(hDlg, IDC_THICKNESS_EDIT, NULL, FALSE);
            EndDialog(hDlg, IDOK);
            return TRUE;

        case IDCANCEL:
            EndDialog(hDlg, IDCANCEL);
>>>>>>> Stashed changes
            return TRUE;
        }
        break;
    }
<<<<<<< Updated upstream
    return FALSE;  
=======
    return FALSE;
>>>>>>> Stashed changes
}

// �� ���⸦ ��ȯ�ϴ� �Լ�
int PenThickness::GetPenWidth() {
    return penWidth;  // ���� ������ �� ���� �� ��ȯ
}
