#include "MakeButton.h"



MakeButton::MakeButton()
{
	this->rectButton.left = 0;
	this->rectButton.top = 0;
	this->rectButton.right = 0;
	this->rectButton.bottom = 0;
	this->icon = 0;
	this->toggleState = false; // 토글 상태를 추적하는 변수
}


MakeButton::MakeButton(int left, int top, int right, int bottom) 
{
	this->rectButton.left = left;
	this->rectButton.top = top;
	this->rectButton.right = right;
	this->rectButton.bottom = bottom;
	this->icon = 0;
	this->toggleState = false; // 토글 상태를 추적하는 변수
}

void MakeButton::setCoordinate(int left, int top, int right, int bottom)
{
	this->rectButton.left = left;
	this->rectButton.top = top;
	this->rectButton.right = right;
	this->rectButton.bottom = bottom;

	this->icon = 0;
}

/// 사각형 그리기 메서드
void MakeButton::drawRectButton(HDC tHdc, int icon) 
{
	this->icon = icon;
	
	/// 배경을 투명하게 설정 (NULL_BRUSH 사용)
	HBRUSH hOldBrush = (HBRUSH)SelectObject(tHdc, GetStockObject(NULL_BRUSH));
	HPEN hOldPen = (HPEN)SelectObject(tHdc, GetStockObject(NULL_PEN));
	
	Rectangle(tHdc, 
		this->rectButton.left, 
		this->rectButton.top, 
		this->rectButton.right, 
		this->rectButton.bottom
	);

	/// 아이콘을 사각형 중앙에 그리기
	buttonLoadImage(this->icon, tHdc);

	/// 이전 브러시와 펜 복원
	SelectObject(tHdc, hOldBrush);
	SelectObject(tHdc, hOldPen);

	/// 리소스 해제
	DeleteObject(hOldBrush);
	DeleteObject(hOldPen);
}

/// 타원 그리기 메서드
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

/// 버튼에 이미지 삽입 메서드
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

	// 아이콘 자원 해제
	DestroyIcon(hIcon);
}

/// 펜 버튼 클릭시 이펙트
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

	// 아이콘 자원 해제
	DestroyIcon(hIcon);

	/// 이펙트 그린 후 기존 아이콘(clickIcon) 덮어씌우기
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

	// 아이콘 자원 해제
	DestroyIcon(hIcon);
}

void MakeButton::doubleImgButton(HDC tHdc, int icon1, int icon2)
{
	/// 현재 토글 상태에 따라 그릴 아이콘 선택
	int iconToDraw = toggleState ? icon1 : icon2;

	// 배경을 투명하게 설정 (NULL_BRUSH 사용)
	HBRUSH hOldBrush = (HBRUSH)SelectObject(tHdc, GetStockObject(NULL_BRUSH));
	HPEN hOldPen = (HPEN)SelectObject(tHdc, GetStockObject(NULL_PEN));

	Rectangle(tHdc,
		this->rectButton.left,
		this->rectButton.top,
		this->rectButton.right,
		this->rectButton.bottom
	);

	/// 아이콘을 사각형 중앙에 그리기
	buttonLoadImage(iconToDraw, tHdc);

	// 이전 브러시와 펜 복원
	SelectObject(tHdc, hOldBrush);
	SelectObject(tHdc, hOldPen);
}
