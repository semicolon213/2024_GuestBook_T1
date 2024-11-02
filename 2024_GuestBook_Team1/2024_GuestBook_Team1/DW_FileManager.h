#pragma once
#include "DrowWindow.h"
#include "FileManager.h"
#include "DW_SideMenu.h"

class DW_FileManager
{
public:
    static HWND hListBox;
    

    static std::wstring filePath;


protected:

    HWND NewFile; 
    HWND SaveFile;
    HWND LoadFile;
    HWND FileManager;
   
};

