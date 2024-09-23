#ifndef PENTHICKNESS_H  // 헤더 파일이 여러 번 포함되는 것을 방지하기 위해 정의된 매크로
#define PENTHICKNESS_H  // 헤더 파일의 시작을 정의

#include <Windows.h>  /// Windows API에 대한 정의를 포함하는 헤더 파일
#include <vector>     

class PenThickness  /// PenThickness 클래스를 정의
{
public:
    PenThickness(HINSTANCE hInstance, HWND parentHWnd);  
    void Show(bool show);  
    void SetThickness(int thickness);  /// 펜 두께를 설정하는 메서드
    void SetCurrentThickness(int thickness);  /// 현재 두께를 설정하는 메서드

private:
    HWND hDlg;  
    HINSTANCE hInst;  
    HWND parentHWnd;  
    int currentThickness;  /// 현재 펜 두께

    void InitializeDialog();  /// 대화 상자를 초기화하는 메서드
    static INT_PTR CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);  
    void DrawThicknessPreview(HDC hdc);  
};

#endif 