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
#include "WndFunc.h"
#include "DrowWindow.h"

class ConnExcel
{
public:

    void insertExcel(const std::wstring);
    std::wstring getVisitList();
    void listScroll(HWND, int, RECT);

    int getTextSize(HWND, std::wstring);
    void listScrollThread(HWND, int, RECT);

    int getTextPosX();
    void setTextPosX(int);

    bool getIsScroll();
    void setIsScroll(bool);

    bool getIsStart();
    void setIsStart(bool);

    void resumeScroll();
    void suspendScroll();

    void stopThread();


    static std::wstring list;

private:
    std::wstring uniVisit;
    std::string multiVisit;

    std::string wcharToChar(const std::wstring);
    std::wstring charToWchar(const std::string);

    std::thread listScrollThreadHandle;

    WCHAR text[10000];
    int textPosX;
    int textWidth = 0;
    bool isScroll = false;
    bool isStart = false;

    HANDLE listHandle;

};
