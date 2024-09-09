/**
@author ������
@date 2024.09.09
    �귯�� ���� ��� �߰� (��ư �� ���� �ڵ忡�� ���� �����ؾ� ��� ��� ����)
    ��ư �߰� (��ư �� ����� ��� ����x)
@todo ��ư ��� ����
      �� �پ��� �귯�� ��� �߰� ����
      ����,���÷���,�귯�� ũ��,���� �� �ٸ� ��ɵ��̶� ������ ����ϱ�.
**/
#include "GB_Function.h"
#include "Window.h"
using namespace std;


void GB_Function::record(LPARAM lParam, ULONGLONG pTime, UINT state, int size, COLORREF col)
{
	drawPInfo.lParam = lParam;
	drawPInfo.pTime = pTime;
	drawPInfo.state = state;
	drawPInfo.pWidth = size;
	drawPInfo.pColor = col;
	drawLInfo.pInfo.push_back(drawPInfo);
}

// �⺻ �׸��� ��ɿ� �귯�� ��� �ڵ� �߰���.
void GB_Function::draw(HWND hWnd, LPARAM lParam, ULONGLONG pTime, UINT state, int size, COLORREF col, int b_type)
{
    HBRUSH hpen;
    HPEN npen,open;
    
	hdc = GetDC(hWnd);
	if (isLeftClick)
	{
		x = LOWORD(lParam);
		y = HIWORD(lParam);
        // �귯�� �����ϸ� �ű⿡ �´� �� ����
        switch (b_type) {
        case BASIC: // �⺻ �׸���
            nPen = CreatePen(PS_SOLID, size, col);
            oPen = (HPEN)SelectObject(hdc, nPen);
            break;
        case PENCIL: // ���� (���� ȸ������)
            nPen = CreatePen(PS_SOLID, size, RGB(150, 150, 150));
            oPen = (HPEN)SelectObject(hdc, nPen);
            break;
        case SPRAY: // �������� (���� ��Ѹ�)
            for (int i = 0; i < 100; ++i) {
                int offsetX = (rand() % (size * 8)) - (size * 4);
                int offsetY = (rand() % (size * 8)) - (size * 4);
                if (sqrt(offsetX * offsetX + offsetY * offsetY) <= size * 4) {
                    SetPixel(hdc, x + offsetX, y + offsetY, col);
                }
            }
            ReleaseDC(hWnd, hdc);
            break;
        case MARKER: 
            for (int i = 0; i < 100; ++i) {
                int offsetX = (rand() % (size * 2)) - size;
                int offsetY = (rand() % (size * 2)) - size;
                SetPixel(hdc, x + offsetX, y + offsetY, col);                                  
            }
            ReleaseDC(hWnd, hdc);
            break;
        case RECTANGLE:
        {
            HBRUSH hpen = CreateSolidBrush(RGB(GetRValue(col), GetGValue(col), GetBValue(col)));
            SelectObject(hdc, hpen);
            Rectangle(hdc, x - size, y - size, x + size, y + size);
            ReleaseDC(hWnd, hdc);
            return;
        }
        case TEST: break;
        default: break;
        }        
                
            MoveToEx(hdc, x, y, NULL);
            LineTo(hdc, px, py);            
            DeleteObject(nPen);
        

		px = x;
		py = y;

		record(lParam, pTime, state, size, col);
	    
	}

}

void GB_Function::mouseUD(LPARAM lParam, ULONGLONG pTime, UINT state, int size, COLORREF col)
{
    if (state == WM_LBUTTONDOWN) 
    {
        isLeftClick = true;
    }
    else 
    {
        isLeftClick = false;
    }
	
	x = LOWORD(lParam);
	y = HIWORD(lParam);

	if(isLeftClick) 
	{

		MoveToEx(hdc, x, y, NULL);
		LineTo(hdc, x, y);

		px = x;
		py = y;

	}

	record(lParam, pTime, state, size, col);
}

void GB_Function::replayThread(HWND hWnd)
{
    isTerminate = false;
    bool status = false;

    if (replayThreadHandle.joinable())
        return;
        
    else

    //std::thread�� ����Ͽ� �����带 ����
    replayThreadHandle = thread(&GB_Function::replay, this, hWnd);

    //�����尡 ����� �� �ڵ����� �ڿ��� ��ȯ�ǵ��� ��
    replayThreadHandle.detach();
}

void GB_Function::replay(HWND hWnd)
{
    HDC hdc;
    HPEN nPen;
    int x, y;

    while (!isTerminate)
    {
        //ȭ�� �ʱ�ȭ
        InvalidateRect(hWnd, NULL, TRUE);
        UpdateWindow(hWnd);

        hdc = GetDC(hWnd);

        for (size_t i = 0; i < drawLInfo.pInfo.size(); i++)
        {
            if (isTerminate)
                break;

            nPen = CreatePen(PS_SOLID, drawLInfo.pInfo[i].pWidth, drawLInfo.pInfo[i].pColor);
            SelectObject(hdc, nPen);

            x = LOWORD(drawLInfo.pInfo[i].lParam);
            y = HIWORD(drawLInfo.pInfo[i].lParam);

            switch (drawLInfo.pInfo[i].state)
            {
            case WM_LBUTTONDOWN:
                MoveToEx(hdc, x, y, NULL);
                LineTo(hdc, x, y + 1);
                break;

            case WM_MOUSEMOVE:
                LineTo(hdc, x, y);
                break;

            case WM_LBUTTONUP:
                LineTo(hdc, x, y);
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

