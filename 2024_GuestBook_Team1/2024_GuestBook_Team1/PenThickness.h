#ifndef PENTHICKNESS_H
#define PENTHICKNESS_H

#include <Windows.h>
#include <CommCtrl.h>
#include <CommDlg.h>  // 색상 선택을 위해 추가
#include "resource.h"

/// 펜 굵기와 색상을 설정하기 위한 대화 상자
class PenThickness {
public:
    PenThickness();
    ~PenThickness();

    /// 펜 굵기 및 색상 설정 대화 상자를 표시
    static void show(HINSTANCE hInst, HWND hWndParent);

    /// 현재 설정된 펜 굵기를 반환
    static int getPenWidth();

    /// 현재 설정된 펜 색상을 반환
    static COLORREF getPenColor();

    /// 대화 상자에서 발생하는 메시지를 처리
    static INT_PTR CALLBACK dlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

private:
    /// 슬라이더 사용하기 위해 라이브러리를 초기화
    static void initializeCommonControls();

    /// 펜 미리보기를 업데이트하는 함수
    static void updatePreview(HWND hDlg);

    /// 색상 선택 대화 상자를 여는 함수
    static void chooseColor(HWND hDlg);

    /// 추가 정적 변수
    static int penWidth;
    static COLORREF penColor;
};

#endif
