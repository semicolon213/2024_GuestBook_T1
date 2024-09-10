#pragma once

#include <Windows.h>
#include <mutex>
#include <memory>
#include <thread>

#include "framework.h"

#include "BlueWindow.h"
#include "DrowWindow.h"
#include "FileManager.h"
#include "Function.h"
#include "ColorPalette.h"

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

	int penNum = 0;


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

