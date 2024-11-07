#include "Function.h"

int Function::penNum = 0;
LINFO Function::drawLInfo = { };
HWND Function::hWnd = nullptr;
int Function::bShape = BASIC;
int Function::stampID = BASIC;
HICON hIcon;
HICON replayhIcon;

void Function::record(PINFO inputPI)
{
    //if (isReplay) return;

	inputPI.bShape = bShape;
	inputPI.pWidth = currentThickness;
	inputPI.stampValue = stampID;

	drawLInfo.pInfo.push_back(inputPI);

    /*std::wstring message = L"record() ȣ���, drawLInfo.pInfo ũ��: " + std::to_wstring(drawLInfo.pInfo.size()) +
       L"\nFunction ��ü �ּ�: " + std::to_wstring((uintptr_t)this);
    MessageBox(nullptr, message.c_str(), L"�����: record", MB_OK); */



}

void Function::draw(HWND hWnd, PINFO dInfo, bool isRecord) // �ڿ� �귯�� �߰�
{
    hdc = GetDC(hWnd);

    if (isLeftClick)
    {
        px = LOWORD(dInfo.lParam); // �׸��� ������ ��ǥ
        py = HIWORD(dInfo.lParam);

        currentTime = std::chrono::steady_clock::now(); // �׸��� �ð� ����

		setPenStyle(dInfo, dInfo.pColor);

		if (isReplay) {
			if (dInfo.bShape != STAMP)
			{
				MoveToEx(hdc, x, y, NULL);
				LineTo(hdc, px, py);
				DeleteObject(nPen);
			}
			else if (dInfo.bShape == STAMP && dInfo.state == WM_LBUTTONDOWN) {
				replayhIcon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(dInfo.stampValue), IMAGE_ICON, 124, 124, 0); // ������ ������ ����
				DrawIconEx(hdc, x - 60, y - 60, replayhIcon, 120, 120, 0, NULL, DI_NORMAL);
				DestroyIcon(replayhIcon);
			}
		}
		else if (stampOn && dInfo.state == WM_LBUTTONDOWN) {
			DrawIconEx(hdc, x - 60, y - 60, hIcon, 120, 120, 0, NULL, DI_NORMAL);
			DestroyIcon(hIcon);
			DeleteObject(nPen);
		}
		else if (stampOn == false && dInfo.state == WM_MOUSEMOVE) {
			MoveToEx(hdc, x, y, NULL);
			LineTo(hdc, px, py);
		}

        x = px;
        y = py;

        DrawTime = currentTime; // ������ �ð� ������Ʈ

        if (isRecord)
            record(dInfo);

    }
    ReleaseDC(hWnd, hdc);

}

void Function::re_draw(HDC phdc, PINFO dInfo, HWND hd) // �ڿ� �귯�� �߰�
{
    hdc = phdc;

    if (isLeftClick)
    {
        px = LOWORD(dInfo.lParam); // �׸��� ������ ��ǥ
        py = HIWORD(dInfo.lParam);

        setPenStyle(dInfo, dInfo.pColor);

		if (dInfo.bShape == STAMP && dInfo.state == WM_LBUTTONDOWN) {
			replayhIcon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(dInfo.stampValue), IMAGE_ICON, 124, 124, 0); // ������ ������ ����
			DrawIconEx(hdc, x - 60, y - 60, replayhIcon, 120, 120, 0, NULL, DI_NORMAL);
			DestroyIcon(replayhIcon);
			DestroyIcon(hIcon);
		}
		else if (stampOn == false && dInfo.state == WM_MOUSEMOVE) {
			if (dInfo.bShape == SPRAY || dInfo.bShape == WATERCOLOR || dInfo.bShape == PENCIL)
			{
				//SetPixel(hdc, px, py, RGB(255, 255, 255));
			}
			else
			{
				MoveToEx(hdc, x, y, NULL);
				if (dInfo.bShape == BASIC || dInfo.bShape == BRUSH) {
					LineTo(hdc, px, py);
				}
				DeleteObject(nPen);
			}
		}

        x = px;
        y = py;


    }

}


void Function::mouseUD(PINFO dInfo, bool isRecord)
{
    if (dInfo.state == WM_LBUTTONDOWN)
    {

        x = LOWORD(dInfo.lParam); //��Ŭ�� �� ��ǥ
        y = HIWORD(dInfo.lParam);

        DrawTime = std::chrono::steady_clock::now(); // �� �귯�� ����� ���� �׸��� ���� �ð� ����.      
        lastThicknessChangeTime = DrawTime; // �β� ���� �ð� �ʱ�ȭ
        currentThickness = dInfo.pWidth; // �� ���� �� ������.


        isLeftClick = true;
    }
    else
    {
        isLeftClick = false;
    }


    if (isRecord)
        record(dInfo);

}

void Function::replayThread(HWND hWnd)
{
    setIsReplay(true);
    setIsReset(false);

    // std::thread�� ����Ͽ� ������ ����
    replayThreadHandle = std::thread(&Function::replay, this, hWnd);

    threadHandle = replayThreadHandle.native_handle();
}


//�⺻ ���÷��� ���� �Լ�
void Function::replay(HWND hWnd)
{

    GetClientRect(hWnd, &clientRect);  // Ŭ���̾�Ʈ ���� ũ�� ���

    while (isReplay)
    {
        // ȭ�� �ʱ�ȭ
        InvalidateRect(hWnd, NULL, TRUE);
        UpdateWindow(hWnd);

        // ȭ�� DC ��������
        hdc = GetDC(hWnd);

        for (size_t i = 0; i < drawLInfo.pInfo.size(); i++)
        {
            if (!isReplay)
            {
                isLeftClick = false;
                break;
            }
            else
                isLeftClick = true;

            PINFO replayInfo = drawLInfo.pInfo[i];

            setBShape(replayInfo.bShape);

			switch (replayInfo.state)
			{
			case WM_LBUTTONDOWN:
				mouseUD(replayInfo, false);
				if (replayInfo.bShape == STAMP) {
					draw(hWnd, replayInfo, false);
				}
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

            // ��� �ӵ� ����
            if (i + 1 < drawLInfo.pInfo.size() && drawLInfo.pInfo[i + 1].state == WM_MOUSEMOVE)
            {
                Sleep((int)((drawLInfo.pInfo[i + 1].pTime - drawLInfo.pInfo[i].pTime) / 10));
            }

            DeleteObject(nPen);
        }
        ReleaseDC(hWnd, hdc);
        // �ݺ� ���� ����
        Sleep(500);
    }

}


// RESET ��ư Ŭ�� �� �۵��Ǵ� �Լ� (���� ���·� ����)
void Function::reDrawing(HWND hWnd)
{
    if (replayThreadHandle.joinable())
    {
        isReplay = false;
        ResumeThread(threadHandle);
        stopReplay(WndFunc::canvasWnd);
    }

    InvalidateRect(WndFunc::canvasWnd, NULL, TRUE);
    UpdateWindow(WndFunc::canvasWnd);

    //MessageBox(hWnd, L"reDrawing", L"dd", MB_OK);
}

void Function::clearDrawing(HWND hWnd)
{
    if (replayThreadHandle.joinable())
    {
        isReplay = false;
        ResumeThread(threadHandle);
        stopReplay(WndFunc::canvasWnd);
    }

    // ��� ����
    drawLInfo.pInfo.clear();

    // ȭ�� �ʱ�ȭ
    InvalidateRect(hWnd, NULL, TRUE);
    UpdateWindow(hWnd);
}

void Function::setPenStyle(PINFO dinfo, COLORREF col)
{
	// �귯�� �����ϸ� �ű⿡ �´� �� ����
	switch (bShape)
	{
	case BASIC: // �⺻ �׸���
		nPen = CreatePen(PS_SOLID, dinfo.pWidth, col);
		oPen = (HPEN)SelectObject(hdc, nPen);
		stampOn = false; // ������ ��� ��Ȱ��ȭ
		break;

    case BRUSH: // �� �귯��
    {
        if (!isReplay || isReset)
        {
            int pwidth = dinfo.pWidth;
            if (dinfo.pWidth < 8) { pwidth = 7; }
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - DrawTime).count(); //������ �� �ð����� �׷��� �� �ð� �� ������ �ð�(�и���)
            duration = max(duration, 1); // �ּ� duration ���� �����Ͽ� 0���� ������ ���� ����

            double distance = sqrt(pow(px - x, 2) + pow(py - y, 2)); // �� �Ÿ�
            double speed = (distance / duration) * 1000; // �ӵ� ���

            int targetThickness = pwidth; // �ӵ��� ����� �� ���� ���� �Ǵ� �β� ����

            // �ӵ��� ���� �� �β� ���̱�
            if (speed > Threshold_Speed) {
                targetThickness = pwidth - (int)((speed - Threshold_Speed) / (Threshold_Speed / (pwidth - Min_Thickness + 1)));
                targetThickness = max(targetThickness, Min_Thickness);
            }
            // �ӵ��� ���� �� �β� �ø���
            else {
                targetThickness = Min_Thickness + (int)((Threshold_Speed - speed) / (Threshold_Speed / (pwidth - Min_Thickness + 1)));
                targetThickness = min(targetThickness, pwidth);
            }

            // �β� ��ȭ ������ �������� Ȯ��      
            if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastThicknessChangeTime).count() >= Update_Interval)
            {
                // �� ���� ������ ����
                if (currentThickness < targetThickness)
                    currentThickness += Smoothing_Factor;
                else if (currentThickness > targetThickness)
                    currentThickness -= Smoothing_Factor;

				// �β� ���� �ð� ������Ʈ
				lastThicknessChangeTime = currentTime;
			}
			// �β��� ����� �� ����
			nPen = CreatePen(PS_SOLID, currentThickness, col);
		}
		else  // ���÷��� �߿��� �β��� ����
		{
			nPen = CreatePen(PS_SOLID, dinfo.pWidth, col);  // �׸� �� ����� �β� ���
		}
		oPen = (HPEN)SelectObject(hdc, nPen);
		stampOn = false; // ������ ��� ��Ȱ��ȭ
		break;
	}

	case PENCIL:
	{
		Gdiplus::Graphics graphics(hdc);
		int alpha = 35; // �⺻ ���� ����    		
		Gdiplus::PointF points[60]; // ���� ������ ����
		for (int i = 0; i < 60; ++i)
		{
			INT angle = rand() % 6 * 3.14159f * i / 80; // ������ ��ǥ 
			points[i] = Gdiplus::PointF(x + dinfo.pWidth * cos(angle) / 1.2, y + dinfo.pWidth * sin(angle) / 1.2); // ������ ����
		}
		Gdiplus::SolidBrush brush(Gdiplus::Color(alpha, GetRValue(col), GetGValue(col), GetBValue(col)));	// ���� ����
		graphics.FillPolygon(&brush, points, 60); // ����ȭ ���� �ʴ� ���� �׸���	
		ReleaseDC(hWnd, hdc);
		stampOn = false; // ������ ��� ��Ȱ��ȭ
		break;
	}

	case SPRAY: // �������� (���� ��Ѹ�)
		int spray_pixel;
		if (dinfo.pWidth <= 3) { spray_pixel = 70; } // �� ���⿡ ���� �� �Ѹ��� �е�
		else if (dinfo.pWidth >= 4 && dinfo.pWidth <= 6) { spray_pixel = 180; }
		else if (dinfo.pWidth >= 7 && dinfo.pWidth <= 10) { spray_pixel = 290; }
		else if (dinfo.pWidth >= 11 && dinfo.pWidth <= 14) { spray_pixel = 400; }
		else if (dinfo.pWidth >= 15 && dinfo.pWidth <= 20) { spray_pixel = 510; }
		else if (dinfo.pWidth >= 21 && dinfo.pWidth <= 25) { spray_pixel = 620; }
		for (int i = 0; i < spray_pixel; ++i)
		{
			int offsetX = (rand() % (dinfo.pWidth * 8)) - (dinfo.pWidth * 4);
			int offsetY = (rand() % (dinfo.pWidth * 8)) - (dinfo.pWidth * 4);
			if (sqrt(offsetX * offsetX + offsetY * offsetY) <= dinfo.pWidth * 4)
			{
				SetPixel(hdc, x + offsetX, y + offsetY, col);
			}
		}
		ReleaseDC(hWnd, hdc);
		stampOn = false; // ������ ��� ��Ȱ��ȭ
		break;

	case MARKER: //���� 
	{
		Gdiplus::Graphics graphics(hdc);
		Gdiplus::SolidBrush marker(Gdiplus::Color(40, GetRValue(col), GetGValue(col), GetBValue(col)));
		graphics.FillRectangle(&marker, x - dinfo.pWidth, y - dinfo.pWidth, dinfo.pWidth * 2, dinfo.pWidth * 2);
		ReleaseDC(hWnd, hdc);
		stampOn = false; // ������ ��� ��Ȱ��ȭ
		break;
	}

    case WATERCOLOR:
    {
        Gdiplus::Graphics graphics(hdc);
        int alpha = 10; // �⺻ ���� ����
        const int numPoints = 20; // ������ ����
        Gdiplus::PointF points[numPoints];

        // ������ ������ ����Ͽ� ���������� ����� �����
        for (int i = 0; i < numPoints; ++i) {
            float angle = 2 * 3.14159f * i / numPoints; // ���� ��ǥ
            float radius = dinfo.pWidth + (rand() % 10); // ������ �ݰ� ��ȭ
            points[i] = Gdiplus::PointF(x + radius * cos(angle), y + radius * sin(angle)); // ������ ����
        }

        Gdiplus::SolidBrush brush(Gdiplus::Color(alpha, GetRValue(col), GetGValue(col), GetBValue(col))); // ���� ����
        graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias); // �ε巴�� �׸���
        graphics.FillPolygon(&brush, points, numPoints); // ������ ���� �׸���

		ReleaseDC(hWnd, hdc);
		stampOn = false; // ������ ��� ��Ȱ��ȭ
		break;
	}
	case STAMP:
	{
		hIcon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(stampID), IMAGE_ICON, 124, 124, 0); // ������ ������ ����
		stampOn = true; // ������ ��� Ȱ��ȭ
		break;
	}
	default:
		break;
	}
}


void Function::paint(HDC hdc, RECT canvasRT, PAINTSTRUCT ps)
{
    cHdc = hdc;
    CanvasPen = (HPEN)SelectObject(cHdc, CreatePen(PS_SOLID, 1, RGB(234, 234, 234)));
    Rectangle(cHdc, canvasRT.left, canvasRT.top, canvasRT.right, canvasRT.bottom);
    SelectObject(cHdc, CanvasPen);
    DeleteObject(CanvasPen);
    FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1)); // ��� ��ĥ


    if (!getIsReplay())
    {
        for (const auto& record : getDrawLInfo().pInfo)
        {
            if (record.bShape != BRUSH)
                setBShape(record.bShape);
            else
                setBShape(BASIC);

			switch (record.state)
			{
			case WM_LBUTTONDOWN:
				mouseUD(record, false);
				if (record.bShape == STAMP) {
					re_draw(cHdc, record, WndFunc::canvasWnd);
				}
				break;

			case WM_LBUTTONUP:
				if (record.bShape == BRUSH) {
					mouseUD(record, FALSE);
					bShape = BRUSH;
				}
				break;

            case WM_MOUSEMOVE:
                re_draw(cHdc, record, WndFunc::canvasWnd);
                break;



            default:
                break;
            }
        }

        DeleteObject(nPen);
    }
}

void Function::stampIcon(int stampID)
{
	this->stampID = stampID;
}

void Function::setBShape(int bShape)
{
    this->bShape = bShape;
}

LINFO Function::getDrawLInfo()
{
    /*std::wstring message = L"getDrawLInfo() ȣ��, drawLInfo.pInfo ũ��: " + std::to_wstring(drawLInfo.pInfo.size()) +
       L"\nFunction ��ü �ּ�: " + std::to_wstring((uintptr_t)this);
    MessageBox(nullptr, message.c_str(), L"�����: getDrawLInfo", MB_OK); */
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

void Function::setIsReset(bool isReset)
{
    this->isReset = isReset;
}

bool Function::getIsReset()
{
    return isReset;
}

void Function::suspendReplay()

{
    setIsReplay(true);
    setIsReset(true);
    isLeftClick = false;
    SuspendThread(threadHandle);
    px2 = px;
    py2 = py;
    //MessageBox(hWnd, L"suspend", L"dd", MB_OK);
}

void Function::resumeReplay()
{
    setIsReset(false);
    setIsReplay(true);
    ResumeThread(threadHandle);
    isLeftClick = true;
    x = px2;
    y = py2;
    //MessageBox(hWnd, L"resume", L"dd", MB_OK);
}

void Function::stopReplay(HWND hWnd)
{
    setIsReplay(false);
    setIsReset(true);

    if (replayThreadHandle.joinable())
    {
        replayThreadHandle.join();
    }
    //MessageBox(hWnd, L"stop", L"dd", MB_OK);
}

void Function::setisLeftClick(bool click) {
    this->isLeftClick = click;
}

bool Function::getisLeftClick() {
    return isLeftClick;
}

// ���Ͱ� ����ִ��� �˻�
bool Function::getDrawLInfoEmpty()
{
    return drawLInfo.pInfo.empty();
}
void Function::GDIPlusStart()
{
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}
void Function::GDIPlusEnd() { //gdi ����
    Gdiplus::GdiplusShutdown(gdiplusToken);
}

//���콺�� �Ҷ� L��ư ����ä ĵ���� ������ �ܺη� ������ ���� ��� �׷������� ��ġ�� �� �� �����۵�
void Function::setisCanvas(bool cnt, LPARAM lParam)
{
    {
        if (!isReplay)
            if (cnt)
            {

                if (!isCanvas)
                {
                    SendMessage(WndFunc::canvasWnd, WM_LBUTTONDOWN, NULL, lParam);
                }

                isCanvas = cnt;
            }
            else
            {
                if (isCanvas)
                {
                    SendMessage(WndFunc::canvasWnd, WM_LBUTTONUP, NULL, lParam);
                }

                POINT pt = { LOWORD(lParam), HIWORD(lParam) };
                MapWindowPoints(WndFunc::drowWnd, WndFunc::canvasWnd, &pt, 1);

                x = pt.x;
                y = pt.y;

                isCanvas = cnt;
            }
    }
}