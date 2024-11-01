#include "MakeButton.h"

MakeButton::MakeButton()
{
    /// ������ �ʱ�ȭ
    this->rectButton.left = 0;
    this->rectButton.top = 0;
    this->rectButton.right = 0;
    this->rectButton.bottom = 0;
    this->icon = 0;
    this->toggleState = false;
}


MakeButton::MakeButton(int left, int top, int right, int bottom)
{
    /// ������ �ʱ�ȭ
    this->rectButton.left = left;
    this->rectButton.top = top;
    this->rectButton.right = right;
    this->rectButton.bottom = bottom;
    this->icon = 0;
    this->toggleState = false;
}

void MakeButton::setCoordinate(int left, int top, int right, int bottom)
{
    this->rectButton.left = left;           /// RECT �ڷ��� left
    this->rectButton.top = top;             /// RECT �ڷ��� top
    this->rectButton.right = right;         /// RECT �ڷ��� right
    this->rectButton.bottom = bottom;       /// RECT �ڷ��� bottom
    this->icon = 0;                         /// icon 0���� �ʱ�ȭ
}

void MakeButton::drawRectButton(HDC tHdc, int icon)
{
    this->icon = icon;  /// ��� ������ icon �� ����
    
    
    /// �簢�� �׸� �� �ܰ��� ���� ó�� (NULL_BRUSH)
    /// ���� ��ư �߰� �� Ŭ�� ���� Ȯ�� �ʿ�� �̺κ� �ּ� ó��
    HBRUSH hOldBrush = (HBRUSH)SelectObject(tHdc, GetStockObject(NULL_BRUSH));
    HPEN hOldPen = (HPEN)SelectObject(tHdc, GetStockObject(NULL_PEN));
    
    /// Rectangle �׸��� 
    Rectangle(tHdc,
        this->rectButton.left,
        this->rectButton.top,
        this->rectButton.right,
        this->rectButton.bottom
    );

    /// ������ �׸���
    buttonLoadImage(this->icon, tHdc);

    
    /// ���� �귯�ÿ� �� ����
    SelectObject(tHdc, hOldBrush);
    SelectObject(tHdc, hOldPen);

    /// ���ҽ� ����
    DeleteObject(hOldBrush);
    DeleteObject(hOldPen);
}

/// Ÿ�� �׸��� �޼���
void MakeButton::drawEllipseButton(HDC tHdc, COLORREF test)
{
    this->icon = icon;  /// ��� ������ icon �� ����

    /// ���� ColorBox�� ���ǵ� �������� �� ����
    HBRUSH tb1 = CreateSolidBrush(test);
    HBRUSH tb2 = (HBRUSH)SelectObject(tHdc, tb1);

    /// ���� ��ư �׸���
    Ellipse(tHdc,
        rectButton.left,
        rectButton.top,
        rectButton.right,
        rectButton.bottom
    );

    /// ���� �� ���� �� ���ҽ� ����
    SelectObject(tHdc, tb2);
    DeleteObject(tb1);


    buttonLoadImage(this->icon, tHdc);
}

void MakeButton::doubleImgButton(HDC tHdc, int icon1, int icon2)
{
    /// ���� ��� ���¿� ���� �׸� ������ ����
    /// toggleState �⺻�� false �̹Ƿ� icon2 ���� ���
    int iconToDraw = toggleState ? icon1 : icon2;

    /// ����� �����ϰ� ���� (NULL_BRUSH ���)
    HBRUSH hOldBrush = (HBRUSH)SelectObject(tHdc, GetStockObject(NULL_BRUSH));
    HPEN hOldPen = (HPEN)SelectObject(tHdc, GetStockObject(NULL_PEN));

    /// Rectangle �׸��� 
    Rectangle(tHdc,
        this->rectButton.left,
        this->rectButton.top,
        this->rectButton.right,
        this->rectButton.bottom
    );

    /// ������ ����
    buttonLoadImage(iconToDraw, tHdc);

    /// ���� �귯�ÿ� �� ����
    SelectObject(tHdc, hOldBrush);
    SelectObject(tHdc, hOldPen);
}


void MakeButton::buttonLoadImage(int icon, HDC tHdc)
{
    /// ������ �ε�
    HICON hIcon = (HICON)LoadImage(GetModuleHandle(NULL),
        MAKEINTRESOURCE(icon),
        IMAGE_ICON,
        30,
        30,
        0
    );

    /// ������ �׸���
    DrawIconEx(tHdc,
        this->rectButton.left,
        this->rectButton.top,
        hIcon, 30, 30, 0, NULL, DI_NORMAL);

    /// ������ �ڿ� ����
    DestroyIcon(hIcon);
}


void MakeButton::clickEffectPen(int icon, int clickIcon, HDC tHdc)
{
    /// ������ �ε�
    HICON hIcon = (HICON)LoadImage(GetModuleHandle(NULL),
        MAKEINTRESOURCE(icon),
        IMAGE_ICON,
        30,
        30,
        0
    );

    DrawIconEx(tHdc,
        this->rectButton.left - 5,
        this->rectButton.top - 5,
        hIcon, 40, 40, 0, NULL, DI_NORMAL);

    // ������ �ڿ� ����
    DestroyIcon(hIcon);

    /// ����Ʈ �׸� �� ���� ������(clickIcon) ������
    buttonLoadImage(clickIcon, tHdc);
}

void MakeButton::clickEffectPen(int icon, HDC tHdc)
{
    /// ����Ʈ ������
    HICON hIcon = (HICON)LoadImage(GetModuleHandle(NULL),
        MAKEINTRESOURCE(icon),
        IMAGE_ICON,
        38,
        38,
        0
    );

    /// ����Ʈ �׸���
    DrawIconEx(tHdc,
        this->rectButton.left - 4,
        this->rectButton.top - 4,
        hIcon, 38, 38, 0, NULL, DI_NORMAL);

    // ������ �ڿ� ����
    DestroyIcon(hIcon);
}


