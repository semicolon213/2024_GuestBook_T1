#include "PenThickness.h"
#include <windowsx.h> // for easier message handling macros

int PenThickness::penWidth = 3;  // �⺻ �� ����
COLORREF PenThickness::penColor = RGB(0, 0, 0);  // �⺻ ������ ������

PenThickness::PenThickness() {}
PenThickness::~PenThickness() {}

// �� ���� �� ���� ���� ��ȭ ���ڸ� ǥ���ϴ� �Լ�
void PenThickness::show(HINSTANCE hInst, HWND hWndParent) {
    initializeCommonControls();
    DialogBox(hInst, MAKEINTRESOURCE(IDD_PEN_THICKNESS), hWndParent, dlgProc);
}

// �����̴��� ���� ���� ��Ʈ���� ����� �� �ֵ��� �ʱ�ȭ�ϴ� �Լ�
void PenThickness::initializeCommonControls() {
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_BAR_CLASSES;
    InitCommonControlsEx(&icex);
}

// ��ȭ ���� ���ν���, ��ȭ ���ڿ��� �߻��ϴ� �޽����� ó��
INT_PTR CALLBACK PenThickness::dlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_INITDIALOG:
        // �����̴� �ʱ�ȭ
        SendDlgItemMessage(hDlg, IDC_THICKNESS_SLIDER, TBM_SETRANGE, TRUE, MAKELPARAM(1, 20));  // ���� 1 ~ 20
        SendDlgItemMessage(hDlg, IDC_THICKNESS_SLIDER, TBM_SETPOS, TRUE, penWidth);
        SetDlgItemInt(hDlg, IDC_THICKNESS_EDIT, penWidth, FALSE);

        // �ʱ� �̸����� ����
        updatePreview(hDlg);
        return TRUE;

    case WM_HSCROLL:  // �����̴� ���� ó��
        if (LOWORD(wParam) == TB_THUMBTRACK || LOWORD(wParam) == TB_ENDTRACK) {
            penWidth = SendDlgItemMessage(hDlg, IDC_THICKNESS_SLIDER, TBM_GETPOS, 0, 0);
            SetDlgItemInt(hDlg, IDC_THICKNESS_EDIT, penWidth, FALSE);
            updatePreview(hDlg);  // �̸����� ������Ʈ
        }
        return TRUE;

    case WM_COMMAND:  // ��ư Ŭ�� �� ��� ó��
        switch (LOWORD(wParam)) {
        case IDC_COLOR_BUTTON:  // ���� ���� ��ư Ŭ��
            chooseColor(hDlg);
            updatePreview(hDlg);  // ���� ���� �� �̸����� ������Ʈ
            return TRUE;

        case IDOK:  // OK ��ư Ŭ��
            penWidth = GetDlgItemInt(hDlg, IDC_THICKNESS_EDIT, NULL, FALSE);
            // ������ �� ����� ������ ���� ���α׷��� ������ �� �ֵ��� �߰� �ڵ� �ۼ�
            EndDialog(hDlg, IDOK);  // ��ȭ ���ڸ� ����
            return TRUE;

        case IDCANCEL:  // ��� ��ư Ŭ��
            EndDialog(hDlg, IDCANCEL);  // ��ȭ ���ڸ� ����ϰ� ����
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// �� ���⸦ ��ȯ�ϴ� �Լ�
int PenThickness::getPenWidth() {
    return penWidth;  // ���� ������ �� ���� �� ��ȯ
}

// �� ������ ��ȯ�ϴ� �Լ�
COLORREF PenThickness::getPenColor() {
    return penColor;  // ���� ������ �� ���� ��ȯ
}

// �̸����⸦ ������Ʈ�ϴ� �Լ�
void PenThickness::updatePreview(HWND hDlg) {
    HWND hPreview = GetDlgItem(hDlg, IDC_PREVIEW);
    HDC hdc = GetDC(hPreview);

    RECT rect;
    GetClientRect(hPreview, &rect);
    FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));  // �̸����� ��� ������� ����

    HPEN hPen = CreatePen(PS_SOLID, penWidth, penColor);  // ���õ� ����� ����� �� ����
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    // �̸����� â�� ���� �׸��� ��ġ ����
    MoveToEx(hdc, rect.left + 10, (rect.bottom - rect.top) / 2, NULL);
    LineTo(hdc, rect.right - 10, (rect.bottom - rect.top) / 2);

    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
    ReleaseDC(hPreview, hdc);

    // �̸����� â�� ������ �ٽ� �׸���
    InvalidateRect(hPreview, NULL, TRUE);
    UpdateWindow(hPreview);
}

// ���� ���� ��ȭ ���ڸ� ���� �Լ�
void PenThickness::chooseColor(HWND hDlg) {
    CHOOSECOLOR cc;
    static COLORREF acrCustClr[16];  // ����� ���� ���� �迭

    ZeroMemory(&cc, sizeof(cc));
    cc.lStructSize = sizeof(cc);
    cc.hwndOwner = hDlg;
    cc.lpCustColors = acrCustClr;
    cc.rgbResult = penColor;
    cc.Flags = CC_FULLOPEN | CC_RGBINIT;

    if (ChooseColor(&cc)) {
        penColor = cc.rgbResult;  // ������ ���� ����
    }
}
