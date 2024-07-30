#pragma once

#include "framework.h"

#define MAX_LOADSTRING 100

class Window
{
private :
	HINSTANCE hInst;
	HWND hWnd;

	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	static INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

public :
	Window() = default;
	~Window() = default;
};

