/**
@file ConnExcel.h
@brief �������� Ŭ���� ��� ���� ����
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

	VARIANT a;			///���� Ÿ���� ������ �ִ� ����ü ����

	IDispatch* appInst = NULL;
	IDispatch* pWorkBooks = NULL;

	DISPID dispID;
	const OLECHAR* szVisible = L"Visible";
	const OLECHAR* szWorkbooks = L"WorkBooks";

};

