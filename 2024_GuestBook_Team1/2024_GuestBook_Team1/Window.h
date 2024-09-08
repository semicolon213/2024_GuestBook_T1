#pragma once

#include <Windows.h>
#include <mutex>
#include <memory>

#include "Resource.h"
#include "GB_Function.h"

#define MAX_LOADSTRING 100

using namespace std;

enum DrawType { BASIC, BRUSH, PENCIL, SPRAY, MARKER, RECTANGLE, TEST };  //브러쉬 종류

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

	unique_ptr<GB_Function> function;


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

