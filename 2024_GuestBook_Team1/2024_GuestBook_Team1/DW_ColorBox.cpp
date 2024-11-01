#include "DW_ColorBox.h"
#include <Windows.h>

int DW_ColorBox::colorSelect = 0;
POINT currentMousePos = { -1, -1 };  // 마우스 위치 저장
bool isMouseInWheel = false;


std::unique_ptr<DW_ColorBox> colorbox = std::make_unique<DW_ColorBox>();
// 전역 변수
double hue = 0.0, saturation = 1.0, value = 1.0;
int red = 255, green = 0, blue = 0;
RECT wheelRect, barRect, selectedRect;
RECT redSliderRect, greenSliderRect, blueSliderRect;
RECT thicknessSliderRect;

COLORREF DW_ColorBox::colorP[3] = { RGB(0,0,0),RGB(0,0,0),RGB(0,0,0) };
int DW_ColorBox::thicknessP[3] = { 3, 3, 3 }; // 각 버튼의 초기 굵기를 3으로 설정

COLORREF DW_ColorBox::getColorNum(int colorSelect) {
    return DW_ColorBox::colorP[colorSelect];
}

void DW_ColorBox::setColorNum(int num, COLORREF color) {
    DW_ColorBox::colorP[num] = color;

}void DW_ColorBox::setThicknessNum(int num, int thickness) {
    DW_ColorBox::thicknessP[num] = thickness;
}

int DW_ColorBox::getThicknessNum(int colorSelect) {
    return DW_ColorBox::thicknessP[colorSelect];
}


/// 컬러팔레트 정적 메서드
LRESULT CALLBACK DrowWindow::WndProcCP(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

    DrowWindow* pThis = nullptr;

    if (message == WM_NCCREATE) {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        pThis = reinterpret_cast<DrowWindow*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    }
    else {
        pThis = reinterpret_cast<DrowWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    if (pThis) {
        return pThis->handleMessageCP(hWnd, message, wParam, lParam); // 인스턴스의 가상 함수 호출
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

RECT redB = { 5, 5, 40, 40 };
RECT blueB = { 45, 5, 80, 40 };
RECT aaa;
RECT m;

DW_ColorBox test;

/// 컬러팔레트 메세지 처리 핸들 메서드
LRESULT DrowWindow::handleMessageCP(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

    switch (message)
    {
    case WM_CREATE:
    {
        wheelRect = { 50, 50, 250, 250 };
        barRect = { 280, 50, 310, 250 };
        selectedRect = { 50, 280, 310, 330 };
        redSliderRect = { 50, 350, 310, 370 };
        greenSliderRect = { 50, 390, 310, 410 };
        blueSliderRect = { 50, 430, 310, 450 };
        thicknessSliderRect = { 50, 470, 310, 490 };
        break;
    }
    case WM_COMMAND:
    {
        if (LOWORD(wParam) == IDC_CLOSE_BUTTON) {
            ShowWindow(hWnd, SW_HIDE);
            return 0;
        }
        break;
    }
    // WM_ERASEBKGND 메시지 처리 추가
    case WM_ERASEBKGND:
        return 1; // 기본 배경 지우기를 방지하여 깜빡임을 줄임

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // 더블 버퍼링을 위한 메모리 DC 생성
        HDC memDC = CreateCompatibleDC(hdc);
        HBITMAP memBitmap = CreateCompatibleBitmap(hdc, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top);
        HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

        // 배경을 RGB(224, 232, 234)로 채움
        HBRUSH bgBrush = CreateSolidBrush(RGB(224, 232, 234));
        FillRect(memDC, &ps.rcPaint, bgBrush);
        DeleteObject(bgBrush);

        // 메모리 DC에 그림 그리기
        colorbox->DrawColorWheel(memDC, (wheelRect.left + wheelRect.right) / 2, (wheelRect.top + wheelRect.bottom) / 2, (wheelRect.right - wheelRect.left) / 2);
        colorbox->DrawColorBar(memDC, barRect);
        colorbox->DrawSelectedColor(memDC);

        colorbox->DrawSlider(memDC, redSliderRect, red, 255);
        colorbox->DrawSlider(memDC, greenSliderRect, green, 255);
        colorbox->DrawSlider(memDC, blueSliderRect, blue, 255);

        int thicknessValue = DW_ColorBox::getThicknessNum(DW_ColorBox::colorSelect);
        colorbox->DrawThicknessSlider(memDC, thicknessSliderRect, thicknessValue, 20);

        // 메모리 DC의 내용을 화면 DC에 복사
        BitBlt(hdc, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top, memDC, 0, 0, SRCCOPY);

        // 사용한 GDI 객체 정리
        SelectObject(memDC, oldBitmap);
        DeleteObject(memBitmap);
        DeleteDC(memDC);

        EndPaint(hWnd, &ps);
        break;
    }

    case WM_LBUTTONDOWN:
        InvalidateRect(WndFunc::toolWnd, NULL, TRUE);
    case WM_MOUSEMOVE:
        if (wParam & MK_LBUTTON)
        {
            int xPos = LOWORD(lParam);
            int yPos = HIWORD(lParam);

            if (PtInRect(&wheelRect, { xPos, yPos })) {
                DW_ColorBox::colorP[DW_ColorBox::colorSelect] = RGB(red, green, blue);
                DW_ColorBox::setThicknessNum(DW_ColorBox::colorSelect, PenThickness::getPenWidth());
                PenThickness::setPenWidth(DW_ColorBox::getThicknessNum(DW_ColorBox::colorSelect));
                InvalidateRect(hWnd, &selectedRect, TRUE);

                int centerX = (wheelRect.left + wheelRect.right) / 2;
                int centerY = (wheelRect.top + wheelRect.bottom) / 2;
                int radius = (wheelRect.right - wheelRect.left) / 2;

                double dx = xPos - centerX;
                double dy = yPos - centerY;
                double distance = sqrt(dx * dx + dy * dy);

                if (distance <= radius)
                {
                    currentMousePos.x = xPos;
                    currentMousePos.y = yPos;
                    isMouseInWheel = true;

                    hue = atan2(dy, dx) * 180 / 3.14159265358979323846 + 180;
                    saturation = min(distance / radius, 1.0);
                    COLORREF color = colorbox->HSVtoRGB(hue, saturation, value);
                    red = GetRValue(color);
                    green = GetGValue(color);
                    blue = GetBValue(color);
                    InvalidateRect(hWnd, NULL, TRUE);
                }
            }
            if (PtInRect(&thicknessSliderRect, { xPos, yPos })) {
                int newThickness = max(1, min(20, (xPos - thicknessSliderRect.left) * 20 / (thicknessSliderRect.right - thicknessSliderRect.left)));
                DW_ColorBox::setThicknessNum(DW_ColorBox::colorSelect, newThickness);
                PenThickness::setPenWidth(newThickness);
                InvalidateRect(hWnd, NULL, TRUE);
            }
            else if (PtInRect(&barRect, { xPos, yPos }))
            {
                value = 1.0 - (double)(yPos - barRect.top) / (barRect.bottom - barRect.top);
                COLORREF color = colorbox->HSVtoRGB(hue, saturation, value);
                red = GetRValue(color);
                green = GetGValue(color);
                blue = GetBValue(color);
                InvalidateRect(hWnd, NULL, TRUE);
            }
            else if (PtInRect(&redSliderRect, { xPos, yPos }))
            {
                red = 255 * (xPos - redSliderRect.left) / (redSliderRect.right - redSliderRect.left);
                colorbox->RGBtoHSV(RGB(red, green, blue), hue, saturation, value);
                InvalidateRect(hWnd, NULL, TRUE);
            }
            else if (PtInRect(&greenSliderRect, { xPos, yPos }))
            {
                green = 255 * (xPos - greenSliderRect.left) / (greenSliderRect.right - greenSliderRect.left);
                colorbox->RGBtoHSV(RGB(red, green, blue), hue, saturation, value);
                InvalidateRect(hWnd, NULL, TRUE);
            }
            else if (PtInRect(&blueSliderRect, { xPos, yPos }))
            {
                blue = 255 * (xPos - blueSliderRect.left) / (blueSliderRect.right - blueSliderRect.left);
                colorbox->RGBtoHSV(RGB(red, green, blue), hue, saturation, value);
                InvalidateRect(hWnd, NULL, TRUE);
            }
            DW_ColorBox::colorP[DW_ColorBox::colorSelect] = RGB(red, green, blue);
            break;
        }

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


void DW_ColorBox::DrawColorWheel(HDC hdc, int centerX, int centerY, int radius)
{
    for (int y = centerY - radius; y <= centerY + radius; y++)
    {
        for (int x = centerX - radius; x <= centerX + radius; x++)
        {
            int dx = x - centerX;
            int dy = y - centerY;
            double distance = sqrt(dx * dx + dy * dy);

            if (distance <= radius)
            {
                double hue = atan2(dy, dx) * 180 / 3.14159265358979323846 + 180;
                double saturation = distance / radius;
                COLORREF color = HSVtoRGB(hue, saturation, value);
                SetPixel(hdc, x, y, color);
            }
        }
    }
    if (isMouseInWheel && currentMousePos.x != -1 && currentMousePos.y != -1)
    {
        HPEN whitePen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
        HPEN blackPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

        // 흰색 원 (바깥쪽)
        SelectObject(hdc, whitePen);
        HBRUSH nullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
        SelectObject(hdc, nullBrush);
        Ellipse(hdc, currentMousePos.x - 5, currentMousePos.y - 5,
            currentMousePos.x + 5, currentMousePos.y + 5);

        // 검은색 원 (안쪽)
        SelectObject(hdc, blackPen);
        Ellipse(hdc, currentMousePos.x - 4, currentMousePos.y - 4,
            currentMousePos.x + 4, currentMousePos.y + 4);

        DeleteObject(whitePen);
        DeleteObject(blackPen);
    }
}

void DW_ColorBox::DrawColorBar(HDC hdc, RECT rect)
{
    for (int y = rect.top; y < rect.bottom; y++)
    {
        double v = 1.0 - (double)(y - rect.top) / (rect.bottom - rect.top);
        COLORREF color = HSVtoRGB(hue, saturation, v);
        for (int x = rect.left; x < rect.right; x++)
        {
            SetPixel(hdc, x, y, color);
        }
    }
}

void DW_ColorBox::DrawSelectedColor(HDC hdc)
{
   
    COLORREF selectedColor = colorP[colorSelect]; 

    // 선택된 영역을 흰색으로 채웁니다.
    FillRect(hdc, &selectedRect, (HBRUSH)GetStockObject(WHITE_BRUSH));

    
    HPEN pen = CreatePen(PS_SOLID, thicknessP[colorSelect], selectedColor); // thicknessP에서 두께 가져오기
    HGDIOBJ oldPen = SelectObject(hdc, pen);

    
    int centerY = (selectedRect.top + selectedRect.bottom) / 2;
    MoveToEx(hdc, selectedRect.left + 10, centerY, NULL);
    LineTo(hdc, selectedRect.right - 10, centerY);

    // 펜 리소스 정리
    SelectObject(hdc, oldPen);
    DeleteObject(pen);
}
void DW_ColorBox::DrawSlider(HDC hdc, RECT rect, int value, int max)
{

    // 배경 그리기
    HBRUSH hBackgroundBrush = CreateSolidBrush(RGB(240, 240, 240));
    FillRect(hdc, &rect, hBackgroundBrush);
    DeleteObject(hBackgroundBrush);

    // 슬라이더 바 그리기
    int barWidth = (rect.right - rect.left) * value / max;
    RECT barRect = { rect.left, rect.top, rect.left + barWidth, rect.bottom };
    HBRUSH hBarBrush = CreateSolidBrush(RGB(0, 122, 204));
    FillRect(hdc, &barRect, hBarBrush);
    DeleteObject(hBarBrush);

    // 테두리 그리기
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    SelectObject(hdc, hPen);
    Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
    DeleteObject(hPen);

    // 슬라이더 색상 채우기
    for (int x = rect.left; x < rect.right; x++)
    {
        int gradient = 255 * (x - rect.left) / (rect.right - rect.left); // x 위치에 따른 색상 값
        COLORREF color;

        // 슬라이더 색상 결정
        if (EqualRect(&rect, &redSliderRect)) {
            color = RGB(gradient, 0, 0);  // 빨간색 그라데이션
        }
        else if (EqualRect(&rect, &greenSliderRect)) {
            color = RGB(0, gradient, 0);  // 초록색 그라데이션
        }
        else if (EqualRect(&rect, &blueSliderRect)) {
            color = RGB(0, 0, gradient);  // 파란색 그라데이션
        }
        else {
            color = RGB(0, 0, 0); // 기본값 (검정색)
        }

        for (int y = rect.top; y < rect.bottom; y++)
        {
            SetPixel(hdc, x, y, color); // 슬라이더 색상 채우기
        }
    }
    int outlinePosition = rect.left + barWidth;
    HPEN outlinePen = CreatePen(PS_SOLID, 0, RGB(0, 0, 0)); // 윤곽선 색상 및 두께
    SelectObject(hdc, outlinePen);

    Rectangle(hdc, outlinePosition - 2, rect.top - 2, outlinePosition + 2, rect.bottom + 2);
    DeleteObject(outlinePen);
}



void DW_ColorBox::DrawThicknessSlider(HDC hdc, RECT rect, int thicknessValue, int maxThickness) {
    // 슬라이더 배경
    HBRUSH hBackgroundBrush = CreateSolidBrush(RGB(240, 240, 240));
    FillRect(hdc, &rect, hBackgroundBrush);
    DeleteObject(hBackgroundBrush);

    // 굵기 슬라이더 바 그리기
    int barWidth = (rect.right - rect.left) * thicknessValue / maxThickness;
    RECT barRect = { rect.left, rect.top, rect.left + barWidth, rect.bottom };
    HBRUSH hBarBrush = CreateSolidBrush(RGB(255, 255, 255));
    FillRect(hdc, &barRect, hBarBrush);
    DeleteObject(hBarBrush);

    // 슬라이더 테두리
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    SelectObject(hdc, hPen);
    Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
    DeleteObject(hPen);

    // 윤곽선 그리기
    int outlinePosition = rect.left + barWidth;
    HPEN outlinePen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0)); // 윤곽선 색상 및 두께
    SelectObject(hdc, outlinePen);
    Rectangle(hdc, outlinePosition - 2, rect.top - 2, outlinePosition + 2, rect.bottom + 2);
    DeleteObject(outlinePen);
}



COLORREF DW_ColorBox::HSVtoRGB(double h, double s, double v)
{
    double r, g, b;

    h = fmod(h, 360.0);  // h를 0-360 범위로 제한
    if (h < 0) h += 360.0;

    int i = static_cast<int>(h / 60.0);
    double f = h / 60.0 - i;
    double p = v * (1 - s);
    double q = v * (1 - f * s);
    double t = v * (1 - (1 - f) * s);

    switch (i)
    {
    case 0: r = v; g = t; b = p; break;
    case 1: r = q; g = v; b = p; break;
    case 2: r = p; g = v; b = t; break;
    case 3: r = p; g = q; b = v; break;
    case 4: r = t; g = p; b = v; break;
    case 5: r = v; g = p; b = q; break;
    default: r = g = b = v; break;  // 예외 처리
    }

    return RGB(static_cast<BYTE>(r * 255),
        static_cast<BYTE>(g * 255),
        static_cast<BYTE>(b * 255));
}


void DW_ColorBox::RGBtoHSV(COLORREF rgb, double& h, double& s, double& v)
{
    double r = GetRValue(rgb) / 255.0;
    double g = GetGValue(rgb) / 255.0;
    double b = GetBValue(rgb) / 255.0;

    double max = fmax(r, fmax(g, b));
    double min = fmin(r, fmin(g, b));
    double delta = max - min;

    v = max;

    if (max == 0)
    {
        s = 0;
        h = 0;
    }
    else
    {
        s = delta / max;

        if (r == max)
            h = (g - b) / delta;
        else if (g == max)
            h = 2 + (b - r) / delta;
        else
            h = 4 + (r - g) / delta;

        h *= 60;
        if (h < 0)
            h += 360;
    }
}


