#ifndef WINDOW_H
#define WINDOW_H

#include <Windows.h>
#include <mutex>
#include <memory>
#include <thread>
<<<<<<< HEAD

#include "framework.h"

#include "BlueWindow.h"
=======

#include "framework.h"
#include "Resource.h"

>>>>>>> feature/button
#include "DrowWindow.h"
#include "FileManager.h"
#include "Function.h"
#include "ColorPalette.h"
#include "Struct.h"

#define MAX_LOADSTRING 100

#define PLAY 1
#define STOP 2

using namespace std;

class Window
{
private :
	HINSTANCE hInst;
	HWND hWnd;
	
	static LRESULT CALLBACK StaticWndProc(HWND, UINT, WPARAM, LPARAM);
	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	static INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);


	static unique_ptr<Window> sinTonIns;
	static once_flag flag;

	unique_ptr<Function> function;
	unique_ptr<FileManager> fileManager;
	unique_ptr<ColorPalette> colorPalette;

	PINFO drawPInfo;

	int penNum = 0;

	int px, py;

	bool LBState = false;
<<<<<<< HEAD
=======

	HWND DrowBT = nullptr;
	HWND LoadBT = nullptr;
	HWND CreditBT = nullptr;
>>>>>>> feature/button

public :
	Window() = default;
	~Window() = default;

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	WCHAR szTitle[MAX_LOADSTRING];
	WCHAR szWindowClass[MAX_LOADSTRING];

	ATOM MyRegisterClass(HINSTANCE);
	BOOL InitInstance(HINSTANCE, int);

	static Window* GetInstance();

};

#endif