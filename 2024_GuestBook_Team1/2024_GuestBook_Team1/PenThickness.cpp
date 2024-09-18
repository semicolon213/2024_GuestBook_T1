#include "PenThickness.h"  /// PenThickness 클래스의 헤더 파일을 포함
#include "Resource.h"      /// 리소스 정의를 포함하는 헤더 파일을 포함
#include <CommCtrl.h>      /// 공용 컨트롤에 대한 정의를 포함하는 헤더 파일을 포함

PenThickness::PenThickness(HINSTANCE hInstance, HWND parentHWnd)
    : hInst(hInstance), parentHWnd(parentHWnd), currentThickness(1)  /// 생성자, 인스턴스 핸들과 부모 윈도우 핸들을 초기화, 기본 두께를 1로 설정
{
    InitializeDialog();  /// 대화 상자를 초기화
}

void PenThickness::InitializeDialog()
{
    hDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_PEN_THICKNESS_DIALOG), parentHWnd, DialogProc);  /// 대화 상자를 생성
    SetWindowLongPtr(hDlg, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));  /// 대화 상자에 사용자 데이터를 설정
    ShowWindow(hDlg, SW_SHOW);  /// 대화 상자를 화면에 표시
}

void PenThickness::Show(bool show)
{
    if (show)
        ShowWindow(hDlg, SW_SHOW);  /// show가 true이면 대화 상자를 표시
    else
        ShowWindow(hDlg, SW_HIDE);  /// show가 false이면 대화 상자를 숨김
}

void PenThickness::SetThickness(int thickness)
{
    currentThickness = thickness;  /// 현재 두께를 설정
    InvalidateRect(hDlg, NULL, TRUE);  /// 대화 상자의 클라이언트 영역을 무효화하여 다시 그려지도록 요청
}

void PenThickness::SetCurrentThickness(int thickness)
{
    currentThickness = thickness;  /// 현재 두께를 설정
    /// 이곳에 현재 두께를 적용하는 로직을 추가
    /// 예: 메인 드로잉 로직에서 두께를 설정하는 함수 호출
}

void PenThickness::DrawThicknessPreview(HDC hdc)
{
    /// 현재 펜 두께의 미리보기를 그립니다.
    HPEN hPen = CreatePen(PS_SOLID, currentThickness, RGB(0, 0, 0));  /// 현재 두께로 펜을 생성
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));  /// 흰색 브러시를 생성
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);  /// 이전 브러시를 저장하고 새 브러시를 선택
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);  /// 이전 펜을 저장하고 새 펜을 선택

    // 미리보기 사각형을 그립니다.
    Rectangle(hdc, 10, 10, 60, 60);  /// (10, 10)에서 (60, 60)까지의 사각형을 그림

    SelectObject(hdc, hOldPen);  /// 이전 펜을 복원
    SelectObject(hdc, hOldBrush);  /// 이전 브러시를 복원
    DeleteObject(hPen);  /// 생성한 펜을 삭제하여 리소스를 해제
    DeleteObject(hBrush);  /// 생성한 브러시를 삭제하여 리소스를 해제
}

INT_PTR CALLBACK PenThickness::DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    PenThickness* pThis = reinterpret_cast<PenThickness*>(GetWindowLongPtr(hDlg, GWLP_USERDATA));  /// 대화 상자의 사용자 데이터를 가져와 PenThickness 객체를 참조

    switch (message)
    {
    case WM_INITDIALOG:
        return TRUE;  /// 대화 상자가 초기화되면 TRUE를 반환하여 기본 처리를 수행

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
            /// 확인 버튼 클릭 시
            pThis->SetCurrentThickness(GetDlgItemInt(hDlg, IDC_THICKNESS_EDIT, NULL, TRUE));  /// 두께를 가져와서 설정
            EndDialog(hDlg, IDOK);  /// 대화 상자를 종료하고 IDOK를 반환합니다.
            return TRUE;

        case IDCANCEL:
            EndDialog(hDlg, IDCANCEL);  /// 취소 버튼 클릭 시 대화 상자를 종료하고 IDCANCEL을 반환
            return TRUE;

        case IDC_THICKNESS_SLIDER:
            /// 두께 조절 슬라이더 값이 변경될 때
            int thickness = SendMessage(GetDlgItem(hDlg, IDC_THICKNESS_SLIDER), TBM_GETPOS, 0, 0);  /// 슬라이더의 현재 값을 가져옴
            pThis->SetThickness(thickness);  /// 두께를 설정
            return TRUE;
        }
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hDlg, &ps);  /// 대화 상자의 클라이언트 영역을 그리기 시작
        pThis->DrawThicknessPreview(hdc);  /// 현재 두께의 미리보기를 그림
        EndPaint(hDlg, &ps);  /// 그리기를 끝냄
        return TRUE;
    }

    }
    return FALSE;  /// 처리되지 않은 메시지는 FALSE를 반환
}
