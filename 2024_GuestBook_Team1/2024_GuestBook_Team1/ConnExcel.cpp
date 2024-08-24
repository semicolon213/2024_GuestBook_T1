/**
@file ConnExcel.cpp
@brief �������� Ŭ���� �ܺ� ���� ����
*/

#include "ConnExcel.h"


ConnExcel::ConnExcel()	
{
	///������ : OLE(���α׷� ���� ���) �ʱ�ȭ
	OleInitialize(NULL);
}

ConnExcel::~ConnExcel()
{
	///�Ҹ��� : OLE ��ü
	OleUninitialize();
}

/**
@fn  ConnExcel::insertExcel(const wchar_t* excelPath, const wchar_t* visitName)
@brief ���޵� ���� �̸� ������ �����ϴ� �Լ�
@param excelPath ������ ������
@param visitName ����������̸�(�湮�ڸ�)
*/
void ConnExcel::insertExcel(const wchar_t* excelPath, const wchar_t* visitName)
{
	///CLSID(���α׷� ���� ������Ʈ��) ��������
	hr = CLSIDFromProgID(L"Excel.Application", &clsid);
	if (FAILED(hr))
	{
		wcout << L"Excel CLSID �������� ����" << endl;
		return;
	}

	///���α׷�(����) �ν��Ͻ� ����
	hr = CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER, IID_IDispatch, (void**)&appInst);
	if (FAILED(hr) || appInst == NULL)
	{
		wcout << L"Excel �ν��Ͻ� ���� ����" << endl;
		return;
	}

	///Excel ǥ�ü��� - ������ �ʰ�
	a.vt = VT_I4;		///����ü Ÿ���� long������ ����
	a.lVal = 1;			///���α׷� ǥ�ø� ������ �ʰ� ����

	hr = appInst ->GetIDsOfNames(IID_NULL, (OLECHAR**)&szVisible, 1, LOCALE_USER_DEFAULT, &dispID);
	if (SUCCEEDED(hr))
	{
		DISPPARAMS dp = { &a,NULL,1,0 };
		appInst->Invoke(dispID, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT, &dp, NULL, NULL, NULL);
	}

	///WorkBoos(Excel ���̽� ���α׷�)
}