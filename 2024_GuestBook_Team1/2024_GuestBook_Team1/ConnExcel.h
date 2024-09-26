/**
@file ConnExcel.h
@brief ¿¢¼¿°ü·Ã Å¬·¡½º ¸â¹ö Á¤ÀÇ ÆÄÀÏ
*/

#pragma once

#include <Windows.h>
#include <iostream>
#include <codecvt>
#include <locale>
#include <string>
#include <fstream>
#include <comdef.h>
#include <tchar.h>
#include <thread>
#include <algorithm>

#include "Resource.h"

using namespace std;

class ConnExcel
{
public:

	void insertExcel(const wstring);
	wstring getVisitList();
	void listScroll(HWND, int, RECT);

	int getTextSize(HWND, wstring);
	void listScrollThread(HWND, int, RECT);

	int getTextPosX();
	void setTextPosX(int);

private:

	wstring uniVisit;
	string multiVisit;

	string wcharToChar(const wstring);
	wstring charToWchar(const string);

	thread listScrollThreadHandle;


	int textPosX;
	int textWidth = 0;

	WCHAR text[10000];

	bool isListRunning = true;

};