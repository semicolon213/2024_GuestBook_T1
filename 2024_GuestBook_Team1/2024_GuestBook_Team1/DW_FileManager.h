#pragma once
#include "DrowWindow.h"
#include "FileManager.h"
class DW_FileManager
{
public: //
    std::vector<PINFO>* penMemory; /// Ãß°¡

    static HWND hListBox2;//
    

    static std::wstring filePath;//

   


protected://
   

    HWND NewFile; //
    HWND SaveFile;// 
    HWND LoadFile;//
    HWND FileManager;//
   
};

