#pragma once

#include <Windows.h>
#include <memory>
#include "Resource.h"
#include "WndFunc.h"

class DrowWindow {
public:
    DrowWindow(int mode, HINSTANCE hInst);

    //void createWindow(int width, int height, HWND parent); // 창 생성 메서드
    virtual LRESULT handleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // 가상 함수로 메시지 처리
    static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // 정적 윈도우 프로시저

    void createWindowNB(int left, int top, int right, int bottom, HWND parent);
    void createWindowTB(int left, int top, int right, int bottom, HWND parent);
    void createWindowCV(int left, int top, int right, int bottom, HWND parent);
    void createWindowSB(int left, int top, int right, int bottom, HWND parent);

    /// 네임바 정적 윈도우
    static LRESULT CALLBACK WndProcNB(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // 정적 윈도우 프로시저
    /// 네임바 메세지 처리 메서드
    virtual LRESULT handleMessageNB(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // 가상 함수로 메시지 처리


    /// 네임바 정적 윈도우
    static LRESULT CALLBACK WndProcTB(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // 정적 윈도우 프로시저
    /// 네임바 메세지 처리 메서드
    virtual LRESULT handleMessageTB(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // 가상 함수로 메시지 처리


    /// 캔버스 영역 정적 윈도우
    static LRESULT CALLBACK WndProcCV(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // 정적 윈도우 프로시저
    /// 캔버스 영역 처리 메서드
    virtual LRESULT handleMessageCV(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // 가상 함수로 메시지 처리
    
    /// 사이드 메뉴 정적 윈도우
    static LRESULT CALLBACK WndProcSB(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // 정적 윈도우 프로시저
    /// 사이드 메뉴  처리 메서드
    virtual LRESULT handleMessageSB(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // 가상 함수로 메시지 처리

private:
    HWND hwnd;
    HINSTANCE hInst;
    WNDCLASS wc;
    int mode;
    RECT drowRT;
    COLORREF bkColor;
    std::unique_ptr<WndFunc> wndFunc;
 
};
