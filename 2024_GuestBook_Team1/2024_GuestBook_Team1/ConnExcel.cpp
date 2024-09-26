/**
@file ConnExcel.cpp
@brief 엑셀관련 클래스 외부 정의 파일
*/

#include "ConnExcel.h"
using namespace std;

/**
@fn  ConnExcel::insertExcel(const wchar_t* excelPath, const wchar_t* visitName)
@brief 전달된 파일 이름 엑셀로 저장하는 함수
@param excelPath 엑셀의 저장경로
@param visitName 저장된파일이름(방문자명)
*/

void ConnExcel::insertExcel(const wstring visitName)
{

	ofstream ofile(EXCEL, ios::app);
	ifstream ifile(EXCEL);
	bool hasHeader = false;

	int rowCnt = 0;

	string line;
	if (ifile.is_open())
	{
		if (getline(ifile, line))
		{
			/// 첫 번째 행이 헤더인 경우
			hasHeader = true;
		}

		while (getline(ifile, line))
		{
			rowCnt++;
		}
		ifile.close();
	}
	else
	{
		cerr << "파일 열기 실패!" << endl;
	}


	if (ofile.is_open())
	{
		ofile << wcharToChar(visitName) << ",";
		ofile.close();
	}
	else
	{
		cerr << "파일 열기 실패!" << endl;
	}

}

wstring ConnExcel::getVisitList()
{
	ofstream ofile(EXCEL, ios::app);
	ifstream ifile(EXCEL);
	bool hasHeader = false;

	string line;
	string getVisit = "방문해 주신 분들 :    ";
	if (ifile.is_open())
	{
		if (getline(ifile, line))
		{
			/// 첫 번째 행이 헤더인 경우
			hasHeader = true;
		}

		while (getline(ifile, line))
		{
			replace(line.begin(), line.end(), ',', ' ');
			getVisit = getVisit.append(line);
		}
		ifile.close();
	}
	else
	{
		cerr << "파일 열기 실패!" << endl;
	}

	return charToWchar(getVisit);
}

/**
@fn  ConnExcel::listScroll(HWND hWnd, int clientWidth)
@brief WM_TIMER가 입력될 때마다 방문자 명단의 위치를 왼쪽으로 옮기는 메소드
@param hWnd 윈도우 헨들러를 전달함
@param clientWidth 화면의 크기를 전달
*/

void ConnExcel::listScroll(HWND hWnd, int clientWidth, RECT mainRT)
{
	while (isListRunning)
	{
		textPosX -= TEXTSPEED;

		if (textPosX < -getTextSize(hWnd, getVisitList()))
		{
			textPosX = clientWidth;
		}

		InvalidateRect(hWnd, NULL, FALSE);

		std::this_thread::sleep_for(std::chrono::milliseconds(10));

	}
}

void ConnExcel::listScrollThread(HWND hWnd, int clientWidth, RECT mainRT)
{

	if (listScrollThreadHandle.joinable())
		return;

	else
		//std::thread를 사용하여 스레드를 시작
		listScrollThreadHandle = thread(&ConnExcel::listScroll, this, hWnd, clientWidth, mainRT);

	//스레드가 종료될 때 자동으로 자원이 반환되도록 함
	listScrollThreadHandle.detach();
}

int ConnExcel::getTextSize(HWND hWnd, wstring list)
{
	SIZE textSize;
	wsprintf(text, list.c_str());
	GetTextExtentPoint32(GetDC(hWnd), text, lstrlen(text), &textSize);

	return textSize.cx;
}

string ConnExcel::wcharToChar(const wstring wcharStr)
{

	int strSize = WideCharToMultiByte(CP_ACP, 0, wcharStr.c_str(), -1, NULL, 0, NULL, NULL);
	if (strSize <= 0) {
		throw runtime_error("필요한 버퍼의 크기를 계산하는데 실패했습니다");
	}
	string charStr(strSize, 0);

	int result = WideCharToMultiByte(CP_ACP, 0, wcharStr.c_str(), -1, &charStr[0], strSize, NULL, NULL);
	if (result == 0)
	{
		throw runtime_error("wstring to string 변환 실패...");
	}
	size_t lastDot = charStr.find_last_of('.');
	if (lastDot != string::npos)
	{
		return charStr.substr(0, lastDot);
	}

	return charStr;

}

wstring ConnExcel::charToWchar(const string charStr)
{

	int wStrSize = MultiByteToWideChar(CP_ACP, 0, charStr.c_str(), -1, nullptr, 0);
	if (wStrSize <= 0)
	{
		throw runtime_error("필요한 버퍼의 크기를 계산하는데 실패했습니다.");
	}
	wstring wcharStr(wStrSize, 0);

	int result = MultiByteToWideChar(CP_ACP, 0, charStr.c_str(), -1, &wcharStr[0], wStrSize);
	wcharStr.resize(wStrSize - 1);
	if (result == 0)
	{
		throw runtime_error("string to wstring 변환 실패...");
	}

	return wcharStr;
}

int ConnExcel::getTextPosX()
{
	return textPosX;
}

void ConnExcel::setTextPosX(int textPosX)
{
	this->textPosX = textPosX;
}

