/**
@file ConnExcel.h
@brief 엑셀관련 클래스 멤버 정의 파일
*/

#pragma once
#pragma comment(lib, "Ole32.lib")

#include <Windows.h>
#include <ole2.h>
#include <comdef.h>
#include <tchar.h>
#include <iostream>

using namespace std;

class ConnExcel
{
public:
	ConnExcel();
	~ConnExcel();

	void insertExcel(const wchar_t*, const wchar_t*);

private:
	CLSID clsid;
	HRESULT hr;

	VARIANT a;			///여러 타입을 가지고 있는 열거체 변수

	IDispatch* appInst = NULL;
	IDispatch* pWorkBooks = NULL;

	DISPID dispID;
	const OLECHAR* szVisible = L"Visible";
	const OLECHAR* szWorkbooks = L"WorkBooks";

};

