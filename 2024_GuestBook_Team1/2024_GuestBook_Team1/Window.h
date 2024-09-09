#pragma once

#include <Windows.h>
#include <mutex>
#include <memory>
#include "FileManager.h"
#include "Resource.h"
#include "GB_Function.h"
#include "BlueWindow.h"
#include "DrowWindow.h"

#define MAX_LOADSTRING 100


using namespace std;

class Window
{
private :
	HINSTANCE hInst;
	HWND hWnd;

	bool replay = false;
	
	static LRESULT CALLBACK StaticWndProc(HWND, UINT, WPARAM, LPARAM);
	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	static INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);


	static unique_ptr<Window> sinTonIns;
	static once_flag flag;

	unique_ptr<GB_Function> function;
	std::unique_ptr<FileManager> fileManager;


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

