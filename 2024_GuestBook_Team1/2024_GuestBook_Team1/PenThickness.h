#ifndef PENTHICKNESS_H
#define PENTHICKNESS_H

#include <Windows.h>
#include <CommCtrl.h>  
/// 펜 굵기를 설정하기 위한 대화 상자
class PenThickness {
public:
    PenThickness(); 
    ~PenThickness();

    /// 펜 굵기 설정 대화 상자를 표시
    void Show(HINSTANCE hInst, HWND hWndParent);

    /// 현재 설정된 펜 굵기를 반환
    static int GetPenWidth();

private:
    /// 대화 상자에서 발생하는 메시지를 처리
    static INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

    /// (슬라이더 사용하기 위해 라이브러리를 초기화
    void InitializeCommonControls();

    /// 추가 정적 변수
    static int penWidth;
};

#endif
