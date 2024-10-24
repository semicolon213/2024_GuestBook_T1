#pragma once
#include "framework.h"
#include <vector>
#include "Resource.h"

class MakeButton 
{
private:
	int icon;		/// 아이콘 지정 멤버 변수

public:
	RECT rectButton;
	bool toggleState;	/// doubleImgButton 전환 멤버 변수

	/// 기본 생성자
	MakeButton();

	/// 좌표 지정 생성자
	MakeButton(int left, int top, int right, int bottom);

	/// 사각형 버튼 그리기 (일반 버튼)
	void drawRectButton(HDC tHdc, int icon);

	/// 원형 버튼 그리기 (컬러 버튼)
	void drawEllipseButton(HDC tHdc, COLORREF test);

	/// 클릭시 이미지가 바뀌는 버튼
	void doubleImgButton(HDC tHdc, int icon1, int icon2);
	
	/// 버튼들 생성 후 좌표 설정 메서드
	void setCoordinate(int left, int top, int right, int bottom);

	/// 클릭시 일반 버튼에 이펙트 설정 메서드
	void clickEffectPen(int icon,int clickIcon, HDC tHdc);

	/// 클릭시 컬러 버튼에 이펙트 설정 메서드
	void clickEffectPen(int icon, HDC tHdc);
	
	
protected:
	/// 버튼에 이미지 로드 메서드 
	void buttonLoadImage(int icon, HDC tHdc);
};
