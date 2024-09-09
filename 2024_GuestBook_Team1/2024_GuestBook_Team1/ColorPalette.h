#pragma once

#ifndef COLORPALETTE_H
#define COLORPALETTE_H

#include <windows.h> 
#include <commdlg.h> /// 색상 선택을 사용하기위한 API 

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam); 
/// WindowProc : 모든 이벤트를 처리하는 함수
#endif // COLORPALETTE_H
