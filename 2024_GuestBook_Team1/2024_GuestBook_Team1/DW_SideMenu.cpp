#include "DW_SideMenu.h"
 

DW_SideMenu::DW_SideMenu(HINSTANCE hInstance)
    : ChildWindow(RGB(243, 243, 243)), penMemory(new std::vector<PINFO>)
{   
    sInst = hInstance;
    SideRT = { 0 };
    sWnd = nullptr;

    NewFile = nullptr;
    SaveFile = nullptr;
    LoadFile = nullptr;
    FileManager = nullptr;
    Credit = nullptr;

   
}

void DW_SideMenu::CreatePop(HWND hParentWnd, int x, int y, int width, int height)
{
    ChildWindow::CreatePop(hParentWnd, L"DW_SideMenuClass", L"Side Child Window", x, y, width, height);
    sWnd = cWnd;

    NewFile = CreateWindow(L"BUTTON", L"NewFile", WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, 
        10, 20, 330, 80, sWnd, (HMENU)SD_NEWFILE_BT, sInst, nullptr);
    SaveFile = CreateWindow(L"BUTTON", L"SaveFile", WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, 
        10, 120, 330, 80, sWnd, (HMENU)SD_SAVEFILE_BT, sInst, nullptr);
    LoadFile = CreateWindow(L"BUTTON", L"LoadFile", WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, 
        10, 220, 330, 80, sWnd, (HMENU)SD_LOADFILE_BT, sInst, nullptr);
    FileManager = CreateWindow(L"BUTTON", L"FileManager", WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, 
        10, 320, 330, 80, sWnd, (HMENU)SD_FILEMANAGER_BT, sInst, nullptr);
    Credit = CreateWindow(L"BUTTON", L"Credit", WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, 
        10, 420, 330, 80, sWnd, (HMENU)SD_CREDIT_BT, sInst, nullptr);

}

PAINTSTRUCT s_ps = { 0 };
HBRUSH SideBrush = nullptr;
HPEN SidePen = nullptr;
HDC sHdc = nullptr;

LRESULT DW_SideMenu::HandleMessage(HWND tWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    case WM_COMMAND:
        switch (wParam)
        {
        case SD_NEWFILE_BT:

            break;

        case SD_SAVEFILE_BT:
            
            
            FileManager::fileManager.selectFileMode(SAVE, tWnd, penMemory);
            break;

        case SD_LOADFILE_BT:
            FileManager::fileManager.selectFileMode(LOAD, tWnd, penMemory);
            SendMessage(GetParent(tWnd), WM_COMMAND, TL_PLAY_BT, 0);   
            break;

        case SD_FILEMANAGER_BT:
        
            FileManager::fileManager.selectFileMode(SD_FILEMANAGER_BT, tWnd, penMemory);
            break;

        case SD_CREDIT_BT:

            break;

        default:
            break;
        }
        break;


    case WM_SETFOCUS:
       

        break;

    case WM_KILLFOCUS:

        /// ShowWindow(sWnd, false);            /// ¿©±â ¹Ù²ñ
        
        break;

    case WM_PAINT:
        SideRT = ChildWindow::GetRT();
        sHdc = GetDC(tWnd);
        sHdc = BeginPaint(tWnd, &s_ps);
        SideBrush = (HBRUSH)SelectObject(sHdc, GetStockObject(NULL_BRUSH));
        SidePen = (HPEN)SelectObject(sHdc, CreatePen(PS_SOLID, 1, RGB(234, 234, 234)));
        Rectangle(sHdc, SideRT.left, SideRT.top, SideRT.right, SideRT.bottom);
        EndPaint(tWnd, &s_ps);
        
    default:
        return ChildWindow::HandleMessage(tWnd, message, wParam, lParam);
    }
    return 0;
}