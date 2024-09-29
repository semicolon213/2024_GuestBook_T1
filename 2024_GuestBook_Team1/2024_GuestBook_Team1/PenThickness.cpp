#include "PenThickness.h"
#include "resource.h"

int PenThickness::penWidth = 1;

PenThickness::PenThickness() {}

PenThickness::~PenThickness() {}

// �� ���� ���� ��ȭ ���ڸ� ǥ���ϴ� �Լ�
void PenThickness::Show(HINSTANCE hInst, HWND hWndParent) {
    InitializeCommonControls(); 
    DialogBox(hInst, MAKEINTRESOURCE(IDD_PEN_THICKNESS), hWndParent, DlgProc);  
}

// �����̴��� ���� ���� ��Ʈ���� ����� �� �ֵ��� �ʱ�ȭ�ϴ� �Լ�
void PenThickness::InitializeCommonControls() {
    INITCOMMONCONTROLSEX icex; 
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);  
    icex.dwICC = ICC_BAR_CLASSES;  
    InitCommonControlsEx(&icex);  
}

// ��ȭ ���� ���ν���, ��ȭ ���ڿ��� �߻��ϴ� �޽����� ó��
INT_PTR CALLBACK PenThickness::DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
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
            EndDialog(hDlg, IDOK);  
            return TRUE;

        case IDCANCEL:  
            EndDialog(hDlg, IDCANCEL); 
            return TRUE;
        }
        break;
    }
    return FALSE;  
}

// �� ���⸦ ��ȯ�ϴ� �Լ�
int PenThickness::GetPenWidth() {
    return penWidth;  // ���� ������ �� ���� �� ��ȯ
}
