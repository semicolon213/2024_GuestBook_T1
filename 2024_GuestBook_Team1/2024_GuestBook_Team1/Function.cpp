/**
@author ������
@date 2024.09.09
	�귯�� ���� ��� �߰� (��ư �� ���� �ڵ忡�� ���� �����ؾ� ��� ��� ����)
	��ư �߰� (��ư �� ����� ��� ����x)
@todo ��ư ��� ����
	  �� �پ��� �귯�� ��� �߰� ����
	  ����,���÷���,�귯�� ũ��,���� �� �ٸ� ��ɵ��̶� ������ ����ϱ�.
**/
#include "Function.h"

using namespace std;

void Function::record(PINFO inputPI)
{
	inputPI.bShape = bShape;
	drawLInfo.pInfo.push_back(inputPI);
}

// �⺻ �׸��� ��ɿ� �귯�� ��� �ڵ� �߰���.
void Function::draw(HWND hWnd, PINFO dInfo, bool isRecord)// �ڿ� �귯�� �߰�
{

	hdc = GetDC(hWnd);
	if (isLeftClick)
	{
		x = LOWORD(dInfo.lParam);
		y = HIWORD(dInfo.lParam);

		setPenStyle(dInfo.pWidth, dInfo.lParam, dInfo.pColor);

		MoveToEx(hdc, x, y, NULL);
		LineTo(hdc, px, py);
		DeleteObject(nPen);

		px = x;
		py = y;

		
		if (isRecord)
			record(dInfo);

	}

}

void Function::mouseUD(PINFO dInfo, bool isRecord)
{
	if (dInfo.state == WM_LBUTTONDOWN)
	{
		x = LOWORD(dInfo.lParam);
		y = HIWORD(dInfo.lParam);

		px = x;
		py = y;

		isLeftClick = true;
	} else
	{
		isLeftClick = false;
	}

	
	if (isRecord)
		record(dInfo);
}

void Function::replayThread(HWND hWnd)
{
	setIsReplay(true);

	if (replayThreadHandle.joinable())
		return;
	else
		//std::thread�� ����Ͽ� �����带 ����
		replayThreadHandle = thread(&Function::replay, this, hWnd);

	//�����尡 ����� �� �ڵ����� �ڿ��� ��ȯ�ǵ��� ��
	replayThreadHandle.detach();
}

void Function::replay(HWND hWnd)
{
	HDC hdc;

	while (isReplay)
	{
		//ȭ�� �ʱ�ȭ
		InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd);

		hdc = GetDC(hWnd);

		for (size_t i = 0; i < drawLInfo.pInfo.size(); i++)
		{
			if (!isReplay)
				break;

			PINFO replayInfo = drawLInfo.pInfo[i];
			x = LOWORD(replayInfo.lParam);
			y = HIWORD(replayInfo.lParam);

			setBShape(replayInfo.bShape);

			switch (replayInfo.state)
			{
			case WM_LBUTTONDOWN:
				mouseUD(replayInfo, false);
				break;

			case WM_MOUSEMOVE:
				draw(hWnd, replayInfo, false);
				break;

			case WM_LBUTTONUP:
				mouseUD(replayInfo, false);
				break;

			default:
				break;

			}

			//��� �ӵ� ����
			if (i < drawLInfo.pInfo.size() - 1)
			{
				Sleep(drawLInfo.pInfo[i + 1].pTime - drawLInfo.pInfo[i].pTime);
			}

			DeleteObject(nPen);
		}

		ReleaseDC(hWnd, hdc);

		//�ݺ� ���� ����
		Sleep(500);
	}
}

void Function::setPenStyle(int size, LPARAM lParam, COLORREF col)
{

	x = LOWORD(lParam);
	y = HIWORD(lParam);

	// �귯�� �����ϸ� �ű⿡ �´� �� ����
	switch (bShape)
	{
	case BASIC: // �⺻ �׸���
		nPen = CreatePen(PS_SOLID, size, col);
		oPen = (HPEN)SelectObject(hdc, nPen);
		break;

	case PENCIL: // ���� (���� ȸ������)
		nPen = CreatePen(PS_SOLID, size, col);
		oPen = (HPEN)SelectObject(hdc, nPen);
		break;

	case SPRAY: // �������� (���� ��Ѹ�)
		for (int i = 0; i < 100; ++i)
		{
			int offsetX = (rand() % (size * 8)) - (size * 4);
			int offsetY = (rand() % (size * 8)) - (size * 4);
			if (sqrt(offsetX * offsetX + offsetY * offsetY) <= size * 4)
			{
				SetPixel(hdc, x + offsetX, y + offsetY, col);
			}
		}
		ReleaseDC(hWnd, hdc);
		break;

	case MARKER:
		for (int i = 0; i < 100; ++i)
		{
			int offsetX = (rand() % (size * 2)) - size;
			int offsetY = (rand() % (size * 2)) - size;
			SetPixel(hdc, x + offsetX, y + offsetY, col);
		}
		ReleaseDC(hWnd, hdc);
		break;

	case RECTANGLE:
		hPen = CreateSolidBrush(RGB(GetRValue(col), GetGValue(col), GetBValue(col)));
		SelectObject(hdc, hPen);
		Rectangle(hdc, x - size, y - size, x + size, y + size);
		ReleaseDC(hWnd, hdc);
		break;

	default:
		break;
	}
}

void Function::setBShape(int bShape)
{
	this->bShape = bShape;
}

LINFO Function::getDrawLInfo()
{
	return drawLInfo;
}

void Function::setIsReplay(bool isReplay)
{
	this->isReplay = isReplay;
}

bool Function::getIsReplay() 
{
	return isReplay;
}