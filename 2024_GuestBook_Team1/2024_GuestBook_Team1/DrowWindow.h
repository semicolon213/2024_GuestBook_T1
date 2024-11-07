#pragma once

#include <Windows.h>
#include <memory>
#include "ConnExcel.h"
#include "Resource.h"
#include "WndFunc.h"
#include "Function.h"
#include "PenThickness.h"


class DrowWindow {
public:
    // �ν��Ͻ��� �ʱ�ȭ�ϴ� ����
    DrowWindow(int mode, HINSTANCE hInst);

    /**
    * @brief Ŭ���� ��� �� ȣ��Ǵ� CALLBACK �޼���
    * @param HWND hWnd
    * @param UINT message 
    * @param WPARAM wParam 
    * @param LPARAM lParam 
    */
    static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // ���� ������ ���ν���
    
    /**
    * @brief Ŭ���� ��� �� ȣ��Ǵ� CALLBACK �޼��忡�� ȣ���ϴ� ���� �޼���
    * @param HWND hWnd
    * @param UINT message
    * @param WPARAM wParam
    * @param LPARAM lParam
    */
    virtual LRESULT handleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // ���� �Լ��� �޽��� ó��

    /**
    * @brief ������ ���� �޼���
    * @param left �������� ���� ��ġ
    * @param top �������� ���� ��ġ
    * @param width �������� �ʺ�
    * @param height �������� ����
    * @param parent �θ� ������ �ڵ�
    */
    void createWindowNB(int left, int top, int width, int height, HWND parent);
    void createWindowTB(int left, int top, int width, int height, HWND parent);
    void createWindowCV(int left, int top, int width, int height, HWND parent);
    void createWindowSB(int left, int top, int width, int height, HWND parent);
    void createWindowCP(int left, int top, int width, int height, HWND parent);
    void createWindowVL(int left, int top, int width, int height, HWND parent);
    void createWindowFM(int left, int top, int right, int bottom, HWND parent);
    
    /**
    * @brief �� ������ ���� �� ȣ��Ǵ� CALLBACK �޼���
    * @param hWnd ������ �ڵ�
    * @param message �޽��� �ڵ�
    * @param wParam �߰� �޽��� ����
    * @param lParam �߰� �޽��� ����
    */
    static LRESULT CALLBACK WndProcNB(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // ���� ������ ���ν���
    static LRESULT CALLBACK WndProcTB(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK WndProcCV(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK WndProcSB(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK WndProcCP(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    /**
    * @brief �� ������ ���� �� CALLBACK �޼��忡 ���� ȣ��Ǵ� ���� �޼���
    * @param hWnd ������ �ڵ�
    * @param message �޽��� �ڵ�
    * @param wParam �߰� �޽��� ����
    * @param lParam �߰� �޽��� ����
    */
    virtual LRESULT handleMessageNB(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); 
    virtual LRESULT handleMessageTB(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    virtual LRESULT handleMessageCV(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); 
    virtual LRESULT handleMessageSB(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); 
    virtual LRESULT handleMessageCP(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); 


    static LRESULT CALLBACK WndProcFM(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // ���� ������ ���ν���

    virtual LRESULT handleMessageFM(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // ���� �Լ��� �޽��� ó��

    PINFO drawPInfo;

    int x, y, px, py;

protected:
    std::unique_ptr<Function> function;
    std::unique_ptr<PenThickness> penThickness;


    /// ������ ������ ���� ������
    static LRESULT CALLBACK WndProcVL(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // ���� ������ ���ν���
    /// ������ ó�� �޼���
    virtual LRESULT handleMessageVL(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // ���� �Լ��� �޽��� ó��
    

private:
    HWND hwnd;
    HINSTANCE hInst;
    WNDCLASS wc;
    int mode;
    COLORREF bkColor;
    std::unique_ptr<WndFunc> wndFunc;
    bool pCnt = true;

    bool lCnt = true;

    WCHAR text[10000];

   

};