#pragma once

#include <memory>
#include <mutex>

#include "framework.h"
#include "GB_Function.h"
#include "Resource.h"

#define MAX_LOADSTRING 100

using namespace std;

extern LINFO drawLInfo;

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

