#pragma once


#include "framework.h"
#include "Resource.h"

class MakeButton
{
private:
    int icon;      /// ������ ���� ��� ����

    int x;                /// ��ư�� x ��ġ
    int y;                /// ��ư�� y ��ġ
    int width;            /// ��ư�� ��
    int height;           /// ��ư�� ����
    int func;             /// ��ư�� ID
    LPCWSTR text;         /// ��ư �ؽ�Ʈ
    COLORREF buttonColor; /// ��ư ������
    HWND hButton;


public:
    RECT rectButton;
    bool toggleState;   /// doubleImgButton ��ȯ ��� ����

    void insertIconImg(LPCWSTR text, int path, HINSTANCE hInst);

    void makeMenuButton(int x, int y, int width, int height, int func, LPCWSTR text);
    void showMenuButton(HWND g_Hwnd, int path, HWND& get_hWnd);

    /**
    * @brief MakeButton Ŭ������ ������ ��ġ ��ǥ�� ���߿� ������ �� ���
    */
    MakeButton();

    /**
    * @brief MakeButton Ŭ������ ������ ��ġ ��ǥ �ʱ�ȭ
    * @param int left RECT �ڷ����� left
    * @param int top RECT �ڷ����� top
    * @param int right RECT �ڷ����� right
    * @param int bottom RECT �ڷ����� bottom
    */
    MakeButton(int left, int top, int right, int bottom);

    /**
    * @brief �Ϲ� �̹��� ��ư �׸��� WM_PAINT���� ���
    * @param HDC tHdc WM_PAINT�� hdc
    * @param int icon ��ư�� ������ �̹��� (.rc, Resource�� ����)
    */
    void drawRectButton(HDC tHdc, int icon);

    /**
    * @brief ���� ��ư �׸���  WM_PAINT���� ���
    * @param HDC tHdc WM_PAINT�� hdc
    * @param COLORREF test ������ ���� ColorBox�� static ���� ���ڷ� ���
    */
    void drawEllipseButton(HDC tHdc, COLORREF test);

    /**
    * @brief Ŭ���� �̹����� �ٲ�� ��ư ��� �� ��ư ���� �ڵ忡 ��۰� ���� �ʼ�
    * @param int icon1 Ŭ�� �� ����� �̹���
    * @param int icon2 ���� ��µǴ� �̹���
    */
    void doubleImgButton(HDC tHdc, int icon1, int icon2);

    /**
    * @brief RECT �ڷ��� ��ư���� ��ǥ ���� �޼���
    * @param int left RECT �ڷ����� left
    * @param int top RECT �ڷ����� top
    * @param int right RECT �ڷ����� right
    * @param int bottom RECT �ڷ����� bottom
    */
    void setCoordinate(int left, int top, int right, int bottom);

    /**
    * @brief Ŭ���� �Ϲ� RECT ��ư�� ����Ʈ �����ϴ� �޼���
    * @param int icon ����Ʈ �̹���
    * @param int clickIcon Ŭ���ϴ� ��ư�� ������ �̹��� 
    * @param HDC tHdc 
    */
    void clickEffectPen(int icon, int clickIcon, HDC tHdc);

    /**
    * @brief Ŭ���� �÷� ��ư�� ����Ʈ �����ϴ� �޼���
    * @param int icon ����Ʈ �̹���
    * @param HDC tHdc 
    */
    void clickEffectPen(int icon, HDC tHdc);

protected:
    /**
    * @brief �̹����� �ε��ϴ� �޼���
    * @param int icon ������ �̹���
    * @param HDC tHdc
    */
    void buttonLoadImage(int icon, HDC tHdc);
};


