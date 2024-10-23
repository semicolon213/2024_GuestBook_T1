#include "PenThickness.h"
#include <windowsx.h> // for easier message handling macros

int PenThickness::penWidth = 3;  // 기본 펜 굵기
COLORREF PenThickness::penColor = RGB(0, 0, 0);  // 기본 색상은 검정색

PenThickness::PenThickness() {}
PenThickness::~PenThickness() {}

// 펜 굵기 및 색상 설정 대화 상자를 표시하는 함수
void PenThickness::show(HINSTANCE hInst, HWND hWndParent) {
    initializeCommonControls();
    DialogBox(hInst, MAKEINTRESOURCE(IDD_PEN_THICKNESS), hWndParent, dlgProc);
}

// 슬라이더와 같은 공용 컨트롤을 사용할 수 있도록 초기화하는 함수
void PenThickness::initializeCommonControls() {
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_BAR_CLASSES;
    InitCommonControlsEx(&icex);
}

// 대화 상자 프로시저, 대화 상자에서 발생하는 메시지를 처리
INT_PTR CALLBACK PenThickness::dlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_INITDIALOG:
        // 슬라이더 초기화
        SendDlgItemMessage(hDlg, IDC_THICKNESS_SLIDER, TBM_SETRANGE, TRUE, MAKELPARAM(1, 20));  // 범위 1 ~ 20
        SendDlgItemMessage(hDlg, IDC_THICKNESS_SLIDER, TBM_SETPOS, TRUE, penWidth);
        SetDlgItemInt(hDlg, IDC_THICKNESS_EDIT, penWidth, FALSE);

        // 초기 미리보기 설정
        updatePreview(hDlg);
        return TRUE;

    case WM_HSCROLL:  // 슬라이더 조작 처리
        if (LOWORD(wParam) == TB_THUMBTRACK || LOWORD(wParam) == TB_ENDTRACK) {
            penWidth = SendDlgItemMessage(hDlg, IDC_THICKNESS_SLIDER, TBM_GETPOS, 0, 0);
            SetDlgItemInt(hDlg, IDC_THICKNESS_EDIT, penWidth, FALSE);
            updatePreview(hDlg);  // 미리보기 업데이트
        }
        return TRUE;

    case WM_COMMAND:  // 버튼 클릭 등 명령 처리
        switch (LOWORD(wParam)) {
        case IDC_COLOR_BUTTON:  // 색상 선택 버튼 클릭
            chooseColor(hDlg);
            updatePreview(hDlg);  // 색상 변경 후 미리보기 업데이트
            return TRUE;

        case IDOK:  // OK 버튼 클릭
            penWidth = GetDlgItemInt(hDlg, IDC_THICKNESS_EDIT, NULL, FALSE);
            // 설정된 펜 굵기와 색상을 메인 프로그램에 적용할 수 있도록 추가 코드 작성
            EndDialog(hDlg, IDOK);  // 대화 상자를 닫음
            return TRUE;

        case IDCANCEL:  // 취소 버튼 클릭
            EndDialog(hDlg, IDCANCEL);  // 대화 상자를 취소하고 닫음
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// 펜 굵기를 반환하는 함수
int PenThickness::getPenWidth() {
    return penWidth;  // 현재 설정된 펜 굵기 값 반환
}

// 펜 색상을 반환하는 함수
COLORREF PenThickness::getPenColor() {
    return penColor;  // 현재 설정된 펜 색상 반환
}

// 미리보기를 업데이트하는 함수
void PenThickness::updatePreview(HWND hDlg) {
    HWND hPreview = GetDlgItem(hDlg, IDC_PREVIEW);
    HDC hdc = GetDC(hPreview);

    RECT rect;
    GetClientRect(hPreview, &rect);
    FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));  // 미리보기 배경 흰색으로 설정

    HPEN hPen = CreatePen(PS_SOLID, penWidth, penColor);  // 선택된 색상과 굵기로 펜 생성
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    // 미리보기 창에 펜을 그리는 위치 설정
    MoveToEx(hdc, rect.left + 10, (rect.bottom - rect.top) / 2, NULL);
    LineTo(hdc, rect.right - 10, (rect.bottom - rect.top) / 2);

    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
    ReleaseDC(hPreview, hdc);

    // 미리보기 창을 강제로 다시 그리기
    InvalidateRect(hPreview, NULL, TRUE);
    UpdateWindow(hPreview);
}

// 색상 선택 대화 상자를 여는 함수
void PenThickness::chooseColor(HWND hDlg) {
    CHOOSECOLOR cc;
    static COLORREF acrCustClr[16];  // 사용자 정의 색상 배열

    ZeroMemory(&cc, sizeof(cc));
    cc.lStructSize = sizeof(cc);
    cc.hwndOwner = hDlg;
    cc.lpCustColors = acrCustClr;
    cc.rgbResult = penColor;
    cc.Flags = CC_FULLOPEN | CC_RGBINIT;

    if (ChooseColor(&cc)) {
        penColor = cc.rgbResult;  // 선택한 색상 저장
    }
}
