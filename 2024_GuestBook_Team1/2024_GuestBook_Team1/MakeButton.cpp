#include "MakeButton.h"

MakeButton::MakeButton(int left, int top, int right, int bottom) 
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

	Rectangle(tHdc, 
		this->rectButton.left, 
		this->rectButton.top, 
		this->rectButton.right, 
		this->rectButton.bottom
	);

	buttonLoadImage(this->icon, tHdc);
}

/// Ÿ�� �׸��� �޼���
void MakeButton::drawEllipseButton(HDC tHdc)
{
	this->icon = icon;

	Ellipse(tHdc,
		rectButton.left, 
		rectButton.top, 
		rectButton.right, 
		rectButton.bottom
	);

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
