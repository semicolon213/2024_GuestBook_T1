#ifndef DW_SIDEMENU_H
#define DW_SIDEMENU_H
#include "ChildWindow.h"
#include "FileManager.h"
#include "Function.h"


class DW_SideMenu : public ChildWindow
{
public:
    DW_SideMenu(HINSTANCE hInstance);
    void CreatePop(HWND hParentWnd, int x, int y, int width, int height);
    
   std::vector<PINFO>* penMemory; 
protected:
    LRESULT HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
    HINSTANCE sInst;
    HWND sWnd;
    RECT SideRT;


    HWND NewFile;
    HWND SaveFile;
    HWND LoadFile;
    HWND FileManager;
    HWND Credit;
};

#endif