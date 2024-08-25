/**
@file ConnExcel.h
@brief ¿¢¼¿°ü·Ã Å¬·¡½º ¸â¹ö Á¤ÀÇ ÆÄÀÏ
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

	DISPID dispID;

	IDispatch* appInst = NULL;
	IDispatch* pWorkBooks = NULL;
	IDispatch* pExcel = NULL;
	IDispatch* pSheets = NULL;
	IDispatch* pSheet = NULL;
	IDispatch* pCells = NULL;
	IDispatch* pRange = NULL;

	const OLECHAR* szVisible = L"Visible";
	const OLECHAR* szWorkbooks = L"WorkBooks";
	const OLECHAR* szOpen = L"Open";
	const OLECHAR* szSheets = L"Sheets";
	const OLECHAR* szItem = L"Item";
	const OLECHAR* szCells = L"Cells";
	const OLECHAR* szSave = L"SAVE";
};

