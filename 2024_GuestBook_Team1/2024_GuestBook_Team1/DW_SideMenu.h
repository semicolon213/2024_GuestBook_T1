#pragma once
#include<Windows.h>
#include "MakeButton.h"
#include "DrowWindow.h"
#include "FileManager.h"
#include "Function.h"
#include "Struct.h"

class DW_SideMenu
{
public:
    std::vector<PINFO>* penMemory; /// �߰�

    static HWND hListBox;
protected:
    HINSTANCE sInst;
    HWND sWnd;
    RECT SideRT;


    HWND NewFile;
    HWND SaveFile;
    HWND LoadFile;
    HWND FileManager;
    HWND Credit;
};