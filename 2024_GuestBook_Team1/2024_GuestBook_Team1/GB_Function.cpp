#include "GB_Function.h"

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

void GB_Function::draw(HWND hWnd, LPARAM lParam, ULONGLONG pTime, UINT state, int size, COLORREF col)
{
	hdc = GetDC(hWnd);
	if (isLeftClick)
	{
		x = LOWORD(lParam);
		y = HIWORD(lParam);
	
		nPen = CreatePen(PS_SOLID, size, col);
		oPen = (HPEN)SelectObject(hdc, nPen);
	
		MoveToEx(hdc, x, y, NULL);
		LineTo(hdc, px, py);

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

    //std::thread를 사용하여 스레드를 시작
    replayThreadHandle = thread(&GB_Function::replay, this, hWnd);

    //스레드가 종료될 때 자동으로 자원이 반환되도록 함
    replayThreadHandle.detach();
}

void GB_Function::replay(HWND hWnd)
{
    HDC hdc;
    HPEN nPen;
    int x, y;

    while (!isTerminate)
    {
        //화면 초기화
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

            //재생 속도 조절
            if (i < drawLInfo.pInfo.size() - 1)
            {
                Sleep(drawLInfo.pInfo[i + 1].pTime - drawLInfo.pInfo[i].pTime);
            }

            DeleteObject(nPen);
        }

        ReleaseDC(hWnd, hdc);

        //반복 간격 조절
        Sleep(500);
    }
}

