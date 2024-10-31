#include "PenThickness.h"
#include "resource.h"
#include <CommCtrl.h>

int PenThickness::penWidth = 1;
std::function<void(int)> PenThickness::thicknessChangedCallback = nullptr;

INT_PTR CALLBACK PenThickness::dlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_INITDIALOG:
        SendDlgItemMessage(hDlg, IDC_THICKNESS_SLIDER, TBM_SETRANGE, TRUE, MAKELPARAM(1, 20));  // 펜 굵기 범위 1 ~ 20까지 수정 시 늘리거나 줄일 수 있음
        SendDlgItemMessage(hDlg, IDC_THICKNESS_SLIDER, TBM_SETPOS, TRUE, penWidth);
        SetDlgItemInt(hDlg, IDC_THICKNESS_EDIT, penWidth, FALSE);
        return TRUE;  // 대화 상자가 성공적으로 초기화되었음을 반환

    case WM_HSCROLL:  // 슬라이더를 조작할 때 발생하는 메시지 처리
        if (LOWORD(wParam) == TB_THUMBTRACK || LOWORD(wParam) == TB_ENDTRACK) {
            penWidth = SendDlgItemMessage(hDlg, IDC_THICKNESS_SLIDER, TBM_GETPOS, 0, 0);
            SetDlgItemInt(hDlg, IDC_THICKNESS_EDIT, penWidth, FALSE);
        }
        return TRUE;

    case WM_COMMAND:  // 버튼 클릭 등 명령을 처리
        switch (LOWORD(wParam)) {
        case IDOK:
            penWidth = GetDlgItemInt(hDlg, IDC_THICKNESS_EDIT, NULL, FALSE);
            if (thicknessChangedCallback) {
                thicknessChangedCallback(penWidth); // 두께 변경 콜백 호출
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
