#pragma once
#include "framework.h"
#include <vector>
#include "Resource.h"

class MakeButton 
{
private:
	int icon;		/// ������ ���� ��� ����

public:
	RECT rectButton;
	bool toggleState;	/// doubleImgButton ��ȯ ��� ����

	/// �⺻ ������
	MakeButton();

	/// ��ǥ ���� ������
	MakeButton(int left, int top, int right, int bottom);

	/// �簢�� ��ư �׸��� (�Ϲ� ��ư)
	void drawRectButton(HDC tHdc, int icon);

	/// ���� ��ư �׸��� (�÷� ��ư)
	void drawEllipseButton(HDC tHdc, COLORREF test);

	/// Ŭ���� �̹����� �ٲ�� ��ư
	void doubleImgButton(HDC tHdc, int icon1, int icon2);
	
	/// ��ư�� ���� �� ��ǥ ���� �޼���
	void setCoordinate(int left, int top, int right, int bottom);

	/// Ŭ���� �Ϲ� ��ư�� ����Ʈ ���� �޼���
	void clickEffectPen(int icon,int clickIcon, HDC tHdc);

	/// Ŭ���� �÷� ��ư�� ����Ʈ ���� �޼���
	void clickEffectPen(int icon, HDC tHdc);
	
	
protected:
	/// ��ư�� �̹��� �ε� �޼��� 
	void buttonLoadImage(int icon, HDC tHdc);
};
