/**
 * @file ChildWindow.h
 * @brief ChildWindow 클래스 정의. 
 * @details 자식 창을 생성하고 관리.
 * @author 윤찬솔
 * @date 2024.09.19
 */

#ifndef CHILDWINDOW_H
#define CHILDWINDOW_H

#include <windows.h>
#include "Resource.h"
#include "FileManager.h"

class ChildWindow 
{
public:
    ChildWindow(COLORREF bgColor);
    virtual ~ChildWindow() = default;

    /**
    * @brief 자식 속성인 윈도우를 만든다.
    * @param HWND hParentWnd 부모 윈도우 핸들.
    * @param LPCWSTR className 클래스 이름.
    * @param LPCWSTR windowName 윈도우 이름.
    * @param int x X 좌표.
    * @param int y Y 좌표.
    * @param int width 창의 너비.
    * @param int height 창의 높이.
    */
    virtual void Create(HWND hParentWnd, LPCWSTR className, LPCWSTR windowName, int x, int y, int width, int height);

    /**
    * @brief 팟업 속성인 윈도우를 만든다.
    * @param HWND hParentWnd 부모 윈도우 핸들.
    * @param LPCWSTR className 클래스 이름.
    * @param LPCWSTR windowName 윈도우 이름.
    * @param int x X 좌표.
    * @param int y Y 좌표.
    * @param int width 창의 너비.
    * @param int height 창의 높이.
    */
    virtual void CreatePop(HWND hParentWnd, LPCWSTR className, LPCWSTR windowName, int x, int y, int width, int height);

    /**
    * @brief 윈도우의 RECT값을 반환한다.
    * @return 윈도우의 RECT값을 반환.
    */
    RECT GetRT();

    /**
    * @brief 윈도우를 보여주거나 숨긴다.
    * @details 
    * 파라미터 값을 받아서 
    * 참일경우 윈도우를 보여주고
    * 거짓일경우 윈도우를 숨긴다.
    * @param bool show 참 또는 거짓값을 받는다.
    */
    void Show(bool show);

    /**
    * @brief 윈도우 핸들을 반환한다.
    * @return HWND 윈도우의 핸들.
    */
    HWND GetHWND() const;

    /**
    * @brief 부모 윈도우 내부에있는 자식 윈도우의 위치를 반환한다.
    * @param HWND hWndParent 부모의 윈도우 핸들.
    * @param HWND hWndChild 자식의 윈도우 핸들.
    * @return RECT 부모 윈도우 내부에있는 자식 윈도우의 위치를 반환.
    */
    RECT GetChildPos(HWND hWndParent, HWND hWndChild);

protected:

    /**
     * @brief 창들의 메시지를 처리.
     * @param hWnd 윈도우 핸들.
     * @param message 메시지 식별자.
     * @param wParam 추가 메시지 정보.
     * @param lParam 추가 메시지 정보.
     * @return 메시지 처리 결과입니다.
     */
    virtual LRESULT HandleMessage(HWND cWnd, UINT message, WPARAM wParam, LPARAM lParam);

    /// 인스턴스 핸들
    HINSTANCE cInst;

    /// 배경 색
    COLORREF bgColor;

    /// 윈도우 핸들
    HWND cWnd;

    /// 배경 브러쉬
    HBRUSH hBrush;

    /// 윈도우 RECT
    RECT ChildRT;

private:
    static LRESULT CALLBACK ChildWndProc(HWND cWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

#endif