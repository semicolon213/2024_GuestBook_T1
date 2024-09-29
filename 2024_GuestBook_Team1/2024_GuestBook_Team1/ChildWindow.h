/**
 * @file ChildWindow.h
 * @brief ChildWindow Ŭ���� ����. 
 * @details �ڽ� â�� �����ϰ� ����.
 * @author ������
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
    * @brief �ڽ� �Ӽ��� �����츦 �����.
    * @param HWND hParentWnd �θ� ������ �ڵ�.
    * @param LPCWSTR className Ŭ���� �̸�.
    * @param LPCWSTR windowName ������ �̸�.
    * @param int x X ��ǥ.
    * @param int y Y ��ǥ.
    * @param int width â�� �ʺ�.
    * @param int height â�� ����.
    */
    virtual void Create(HWND hParentWnd, LPCWSTR className, LPCWSTR windowName, int x, int y, int width, int height);

    /**
    * @brief �̾� �Ӽ��� �����츦 �����.
    * @param HWND hParentWnd �θ� ������ �ڵ�.
    * @param LPCWSTR className Ŭ���� �̸�.
    * @param LPCWSTR windowName ������ �̸�.
    * @param int x X ��ǥ.
    * @param int y Y ��ǥ.
    * @param int width â�� �ʺ�.
    * @param int height â�� ����.
    */
    virtual void CreatePop(HWND hParentWnd, LPCWSTR className, LPCWSTR windowName, int x, int y, int width, int height);

    /**
    * @brief �������� RECT���� ��ȯ�Ѵ�.
    * @return �������� RECT���� ��ȯ.
    */
    RECT GetRT();

    /**
    * @brief �����츦 �����ְų� �����.
    * @details 
    * �Ķ���� ���� �޾Ƽ� 
    * ���ϰ�� �����츦 �����ְ�
    * �����ϰ�� �����츦 �����.
    * @param bool show �� �Ǵ� �������� �޴´�.
    */
    void Show(bool show);

    /**
    * @brief ������ �ڵ��� ��ȯ�Ѵ�.
    * @return HWND �������� �ڵ�.
    */
    HWND GetHWND() const;

    /**
    * @brief �θ� ������ ���ο��ִ� �ڽ� �������� ��ġ�� ��ȯ�Ѵ�.
    * @param HWND hWndParent �θ��� ������ �ڵ�.
    * @param HWND hWndChild �ڽ��� ������ �ڵ�.
    * @return RECT �θ� ������ ���ο��ִ� �ڽ� �������� ��ġ�� ��ȯ.
    */
    RECT GetChildPos(HWND hWndParent, HWND hWndChild);

protected:

    /**
     * @brief â���� �޽����� ó��.
     * @param hWnd ������ �ڵ�.
     * @param message �޽��� �ĺ���.
     * @param wParam �߰� �޽��� ����.
     * @param lParam �߰� �޽��� ����.
     * @return �޽��� ó�� ����Դϴ�.
     */
    virtual LRESULT HandleMessage(HWND cWnd, UINT message, WPARAM wParam, LPARAM lParam);

    /// �ν��Ͻ� �ڵ�
    HINSTANCE cInst;

    /// ��� ��
    COLORREF bgColor;

    /// ������ �ڵ�
    HWND cWnd;

    /// ��� �귯��
    HBRUSH hBrush;

    /// ������ RECT
    RECT ChildRT;

private:
    static LRESULT CALLBACK ChildWndProc(HWND cWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

#endif