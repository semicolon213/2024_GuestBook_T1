#include "MakeButton.h"



MakeButton::MakeButton()
{
	this->rectButton.left = 0;
	this->rectButton.top = 0;
	this->rectButton.right = 0;
	this->rectButton.bottom = 0;
	this->icon = 0;
	this->toggleState = false; // ��� ���¸� �����ϴ� ����
}


MakeButton::MakeButton(int left, int top, int right, int bottom) 
{
	this->rectButton.left = left;
	this->rectButton.top = top;
	this->rectButton.right = right;
	this->rectButton.bottom = bottom;
	this->icon = 0;
	this->toggleState = false; // ��� ���¸� �����ϴ� ����
}

void MakeButton::setCoordinate(int left, int top, int right, int bottom)
{
	this->rectButton.left = left;
	this->rectButton.top = top;
	this->rectButton.right = right;
	this->rectButton.bottom = bottom;

	this->icon = 0;
}

/// �簢�� �׸��� �޼���
void MakeButton::drawRectButton(HDC tHdc, int icon) 
{
	this->icon = icon;
	
	/// ����� �����ϰ� ���� (NULL_BRUSH ���)
	HBRUSH hOldBrush = (HBRUSH)SelectObject(tHdc, GetStockObject(NULL_BRUSH));
	HPEN hOldPen = (HPEN)SelectObject(tHdc, GetStockObject(NULL_PEN));
	
	Rectangle(tHdc, 
		this->rectButton.left, 
		this->rectButton.top, 
		this->rectButton.right, 
		this->rectButton.bottom
	);

	/// �������� �簢�� �߾ӿ� �׸���
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
	this->icon = icon;

	HBRUSH tb1 = CreateSolidBrush(test);
	HBRUSH tb2 = (HBRUSH)SelectObject(tHdc, tb1);

	Ellipse(tHdc,
		rectButton.left, 
		rectButton.top, 
		rectButton.right, 
		rectButton.bottom
	);

	SelectObject(tHdc, tb2);
	DeleteObject(tb1);

	buttonLoadImage(this->icon, tHdc);
}

/// ��ư�� �̹��� ���� �޼���
void MakeButton::buttonLoadImage(int icon, HDC tHdc)
{
	HICON hIcon = (HICON)LoadImage(GetModuleHandle(NULL),
		MAKEINTRESOURCE(icon),
		IMAGE_ICON,
		30,
		30,
		0
	);

	DrawIconEx(tHdc,
		this->rectButton.left,
		this->rectButton.top,
		hIcon, 30, 30, 0, NULL, DI_NORMAL);

	// ������ �ڿ� ����
	DestroyIcon(hIcon);
}

/// �� ��ư Ŭ���� ����Ʈ
void MakeButton::clickEffectPen(int icon, int clickIcon, HDC tHdc) 
{
	HICON hIcon = (HICON)LoadImage(GetModuleHandle(NULL),
		MAKEINTRESOURCE(icon),
		IMAGE_ICON,
		30,
		30,
		0
	);

	DrawIconEx(tHdc,
		this->rectButton.left-5,
		this->rectButton.top-5,
		hIcon, 40, 40, 0, NULL, DI_NORMAL);

	// ������ �ڿ� ����
	DestroyIcon(hIcon);

	/// ����Ʈ �׸� �� ���� ������(clickIcon) ������
	buttonLoadImage(clickIcon, tHdc);
}

void MakeButton::clickEffectPen(int icon, HDC tHdc)
{
	HICON hIcon = (HICON)LoadImage(GetModuleHandle(NULL),
		MAKEINTRESOURCE(icon),
		IMAGE_ICON,
		38,
		38,
		0
	);

	DrawIconEx(tHdc,
		this->rectButton.left-4,
		this->rectButton.top-4,
		hIcon, 38, 38, 0, NULL, DI_NORMAL);

	// ������ �ڿ� ����
	DestroyIcon(hIcon);
}

void MakeButton::doubleImgButton(HDC tHdc, int icon1, int icon2)
{
	/// ���� ��� ���¿� ���� �׸� ������ ����
	int iconToDraw = toggleState ? icon1 : icon2;

	// ����� �����ϰ� ���� (NULL_BRUSH ���)
	HBRUSH hOldBrush = (HBRUSH)SelectObject(tHdc, GetStockObject(NULL_BRUSH));
	HPEN hOldPen = (HPEN)SelectObject(tHdc, GetStockObject(NULL_PEN));

	Rectangle(tHdc,
		this->rectButton.left,
		this->rectButton.top,
		this->rectButton.right,
		this->rectButton.bottom
	);

	/// �������� �簢�� �߾ӿ� �׸���
	buttonLoadImage(iconToDraw, tHdc);

	// ���� �귯�ÿ� �� ����
	SelectObject(tHdc, hOldBrush);
	SelectObject(tHdc, hOldPen);
}
