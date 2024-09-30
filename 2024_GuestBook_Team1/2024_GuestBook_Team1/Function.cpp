/**
@author 조재현
@date 2024.09.28
	붓 브러쉬 기능 추가
	draw 함수 내용 조정
	mouseUD 함수 내용 조정	
**/
#include "Function.h"


using namespace std;


int Function::penNum = 0;
LINFO Function::drawLInfo = { };
HWND Function::hWnd = nullptr;
int Function::bShape = BASIC;

void Function::record(PINFO inputPI)
{

	inputPI.bShape = bShape;
	drawLInfo.pInfo.push_back(inputPI);

	/*std::wstring message = L"record() 호출됨, drawLInfo.pInfo 크기: " + std::to_wstring(drawLInfo.pInfo.size()) +
		L"\nFunction 객체 주소: " + std::to_wstring((uintptr_t)this);
	MessageBox(nullptr, message.c_str(), L"디버깅: record", MB_OK); */

	

}

void Function::draw(HWND hWnd, PINFO dInfo, bool isRecord) // 뒤에 브러쉬 추가
{

	hdc = GetDC(hWnd);
	if (isLeftClick)
	{		
		px = LOWORD(dInfo.lParam); // 그리기 시작한 좌표
		py = HIWORD(dInfo.lParam);

		currentTime = std::chrono::steady_clock::now(); // 그리기 시간 저장

		setPenStyle(dInfo, dInfo.pColor);
		
		MoveToEx(hdc, x, y, NULL);
		LineTo(hdc, px, py);
		DeleteObject(nPen);
				
		x = px; 
		y = py;
		
		DrawTime = currentTime; // 마지막 시간 업데이트

		if (isRecord)
			record(dInfo);

	}
	ReleaseDC(hWnd, hdc);

}

void Function::mouseUD(PINFO dInfo, bool isRecord)
{
	if (dInfo.state == WM_LBUTTONDOWN)
	{
		x = LOWORD(dInfo.lParam); //좌클릭 한 좌표
		y = HIWORD(dInfo.lParam);

		DrawTime = std::chrono::steady_clock::now(); // 붓 브러쉬 사용을 위한 그리는 시작 시간 저장.		
		lastThicknessChangeTime = DrawTime; // 두께 변경 시간 초기화
		currentThickness = dInfo.pWidth; // 붓 전용 펜 사이즈.


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
		// std::thread를 사용하여 스레드 시작
		replayThreadHandle = thread(&Function::replay, this, hWnd);

	// 스레드가 종료될 때 자동으로 자원이 반환되도록 함
	replayThreadHandle.detach();
}

// 기본 리플레이 동작 함수
void Function::replay(HWND hWnd)
{
	HDC hdc;

	while (isReplay)
	{
		// 화면 초기화
		InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd);

		hdc = GetDC(hWnd);

		for (size_t i = 0; i < drawLInfo.pInfo.size(); i++)
		{
			if (!isReplay)
			{
				isLeftClick = false;
				break;
			}

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

			// 재생 속도 조절
			if (i < drawLInfo.pInfo.size() - 1)
			{
				Sleep(drawLInfo.pInfo[i + 1].pTime - drawLInfo.pInfo[i].pTime);
			}

			DeleteObject(nPen);
		}


		ReleaseDC(hWnd, hdc);

		// 반복 간격 조절	
		Sleep(500);
	}
}

// RESET 버튼 클릭 시 작동되는 함수 (원래 형태로 복원)
void Function::reDrawing(HWND hWnd)
{
	HDC hdc = GetDC(hWnd);

	for (const auto& replayInfo : drawLInfo.pInfo)
	{
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
	}

	ReleaseDC(hWnd, hdc);
}

void Function::clearDrawing(HWND hWnd)
{
	// 기록 삭제
	drawLInfo.pInfo.clear();

	// 화면 초기화
	InvalidateRect(hWnd, NULL, TRUE);
	UpdateWindow(hWnd);		
}
void Function::setPenStyle(PINFO dinfo, COLORREF col)
{
	// 브러쉬 선택하면 거기에 맞는 펜 제공
	switch (bShape)
	{
	case BASIC: // 기본 그리기
		nPen = CreatePen(PS_SOLID, dinfo.pWidth, col);
		oPen = (HPEN)SelectObject(hdc, nPen);
		break;

	case BRUSH: // 붓 브러쉬
	{
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - DrawTime).count(); //눌렀을 떄 시간부터 그렸을 때 시간 그 사이의 시간(밀리초)
		duration = max(duration, 1); // 최소 duration 값을 설정하여 0으로 나누는 문제 방지

		double distance = sqrt(pow(px - x, 2) + pow(py - y, 2)); // 선 거리
		double speed = (distance / duration) * 1000; // 속도 계산

		int targetThickness = dinfo.pWidth; // 속도가 변경될 때 같이 변경 되는 두께 변수

		// 속도가 빠를 때 두께 줄이기
		if (speed > Threshold_Speed) {
			targetThickness = dinfo.pWidth - (int)((speed - Threshold_Speed) / (Threshold_Speed / (dinfo.pWidth - Min_Thickness)));
			targetThickness = max(targetThickness, Min_Thickness);
		}
		// 속도가 느릴 때 두께 늘리기
		else {
			targetThickness = Min_Thickness + (int)((Threshold_Speed - speed) / (Threshold_Speed / (dinfo.pWidth - Min_Thickness)));
			targetThickness = min(targetThickness, dinfo.pWidth);
		}

		// 두께 변화 간격이 지났는지 확인		
		if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastThicknessChangeTime).count() >= Update_Interval)
		{
			// 붓 전용 사이즈 조절
			if (currentThickness < targetThickness)
				currentThickness += Smoothing_Factor;
			else if (currentThickness > targetThickness)
				currentThickness -= Smoothing_Factor;

			// 두께 변경 시간 업데이트
			lastThicknessChangeTime = currentTime; 		
		}		
		// 두께가 변경된 펜 생성
		nPen = CreatePen(PS_SOLID, currentThickness, col);
		oPen = (HPEN)SelectObject(hdc, nPen);
		break;
	}

	case PENCIL:
	{
		Gdiplus::Graphics graphics(hdc);
		int alpha = 15; // 기본 투명도 설정    		
		Gdiplus::PointF points[80]; // 도형 꼭짓점 갯수
		for (int i = 0; i < 80; ++i)
		{
			INT angle = rand() % 6 * 3.14159f * i / 80; // 꼭짓점 좌표 
			points[i] = Gdiplus::PointF(x + dinfo.pWidth * cos(angle) / 2, y + dinfo.pWidth * sin(angle) / 2); // 꼭짓점 설정
		}
		Gdiplus::SolidBrush brush(Gdiplus::Color(alpha, GetRValue(col), GetGValue(col), GetBValue(col)));	// 색상 설정
		graphics.FillPolygon(&brush, points, 80); // 정형화 되지 않는 도형 그리기	
		ReleaseDC(hWnd, hdc);
		break;
	}

	case SPRAY: // 스프레이 (점을 흩뿌림)
		for (int i = 0; i < 500; ++i)
		{
			int offsetX = (rand() % (dinfo.pWidth * 8)) - (dinfo.pWidth * 4);
			int offsetY = (rand() % (dinfo.pWidth * 8)) - (dinfo.pWidth * 4);
			if (sqrt(offsetX * offsetX + offsetY * offsetY) <= dinfo.pWidth * 2)
			{
				SetPixel(hdc, x + offsetX, y + offsetY, col);
			}
		}
		ReleaseDC(hWnd, hdc);
		break;

	case MARKER:
	{
		Gdiplus::Graphics graphics(hdc);
		Gdiplus::SolidBrush marker(Gdiplus::Color(40, GetRValue(col), GetGValue(col), GetBValue(col)));
		graphics.FillRectangle(&marker, x - dinfo.pWidth, y - dinfo.pWidth, dinfo.pWidth * 2, dinfo.pWidth * 2);
		ReleaseDC(hWnd, hdc);
		break;
	}

	case WATERCOLOR:
	{
		Gdiplus::Graphics graphics(hdc);
		int alpha = 10; // 기본 투명도 설정
		const int numPoints = 20; // 꼭짓점 갯수
		Gdiplus::PointF points[numPoints];

		// 무작위 각도를 사용하여 비정형적인 모양을 만들기
		for (int i = 0; i < numPoints; ++i) {
			float angle = 2 * 3.14159f * i / numPoints; // 원형 좌표
			float radius = dinfo.pWidth + (rand() % 10); // 무작위 반경 변화
			points[i] = Gdiplus::PointF(x + radius * cos(angle), y + radius * sin(angle)); // 꼭짓점 설정
		}

		Gdiplus::SolidBrush brush(Gdiplus::Color(alpha, GetRValue(col), GetGValue(col), GetBValue(col))); // 색상 설정
		graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias); // 부드럽게 그리기
		graphics.FillPolygon(&brush, points, numPoints); // 비정형 도형 그리기

		ReleaseDC(hWnd, hdc);
		break;
	}

	default:
		break;
	}
}

void Function::paint(HWND hWnd, RECT canvasRT)
{
	cHdc = BeginPaint(hWnd, &cPS);
	CanvasPen = (HPEN)SelectObject(cHdc, CreatePen(PS_SOLID, 1, RGB(234, 234, 234)));
	Rectangle(cHdc, canvasRT.left, canvasRT.top, canvasRT.right, canvasRT.bottom);
	SelectObject(cHdc, CanvasPen);
	DeleteObject(CanvasPen);

	if (!getIsReplay())
	{
		for (const auto& record : getDrawLInfo().pInfo)
		{
			setBShape(record.bShape);

			switch (record.state)
			{
			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
				mouseUD(record, FALSE);
				break;

			case WM_MOUSEMOVE:
				draw(hWnd, record, FALSE);
				break;

			default:
				break;
			}
		}
	}

	EndPaint(hWnd, &cPS);
}




void Function::setBShape(int bShape)
{
	this->bShape = bShape;
}

LINFO Function::getDrawLInfo()
{
	/*std::wstring message = L"getDrawLInfo() 호출, drawLInfo.pInfo 크기: " + std::to_wstring(drawLInfo.pInfo.size()) +
		L"\nFunction 객체 주소: " + std::to_wstring((uintptr_t)this);
	MessageBox(nullptr, message.c_str(), L"디버깅: getDrawLInfo", MB_OK); */
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

// RESET 버튼 클릭 시 호출되는 함수
void Function::stopReplay(HWND hWnd)
{
	setIsReplay(false);
	reDrawing(hWnd);
}

// 벡터가 비어있는지 검사
bool Function::getDrawLInfoEmpty()
{
	return drawLInfo.pInfo.empty();
}
void Function::GDIPlusStart()
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}
void Function::GDIPlusEnd() { //gdi 종료
	Gdiplus::GdiplusShutdown(gdiplusToken);
}
