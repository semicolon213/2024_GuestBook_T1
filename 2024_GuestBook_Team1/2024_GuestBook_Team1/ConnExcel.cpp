/**
@file ConnExcel.cpp
@brief �������� Ŭ���� �ܺ� ���� ����
*/

#include "ConnExcel.h"

ConnExcel::ConnExcel()
{
	///---------- �Ҹ��� --------------

	///OLE(���α׷� ���� ���) �ʱ�ȭ
	OleInitialize(NULL);
}

ConnExcel::~ConnExcel()
{
	///---------- �Ҹ��� --------------
	
	///����� ���� ��ü �� �� ����
	if (pRange) pRange->Release();
	if (pSheet) pSheet->Release();
	if (pSheets) pSheets->Release();
	if (pExcel) pExcel->Release();
	if (pWorkBooks) pWorkBooks->Release();
	if (appInst) pRange->Release();

	///OLE ��ü
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
	{
		VARIANT a;			///���� Ÿ���� ������ �ִ� ����ü ����

		a.vt = VT_I4;		///����ü Ÿ���� long������ ����
		a.lVal = 1;			///���α׷� ǥ�ø� ������ �ʰ� ����

		hr = appInst->GetIDsOfNames(IID_NULL, (OLECHAR**)&szVisible, 1, LOCALE_USER_DEFAULT, &dispID);
		if (SUCCEEDED(hr))
		{
			DISPPARAMS dp = { &a, NULL, 1, 0 };
			appInst->Invoke(dispID, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT, &dp, NULL, NULL, NULL);
		}
	}

	///WorkBooks(Excel ���̽� ���α׷�) ����
	{
		hr = appInst->GetIDsOfNames(IID_NULL, (OLECHAR**)&szWorkbooks, 1, LOCALE_USER_DEFAULT, &dispID);
		if (SUCCEEDED(hr))
		{
			VARIANT result;
			DISPPARAMS dpNoArgs = { NULL, NULL, 0,0 };
			appInst->Invoke(dispID, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dpNoArgs, &result, NULL, NULL);
			pWorkBooks = result.pdispVal;
		}
	}

	///Excel ���� ����
	{
		VARIANT varFileName;
		varFileName.vt = VT_BSTR;
		varFileName.bstrVal = SysAllocString(excelPath);

		VARIANT result;
		DISPPARAMS dp = { &varFileName, NULL, 1,0 };
		hr = pWorkBooks->GetIDsOfNames(IID_NULL, (OLECHAR**)&szOpen, 1, LOCALE_USER_DEFAULT, &dispID);
		if (SUCCEEDED(hr))
		{
			pWorkBooks->Invoke(dispID, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &result, NULL, NULL);
			pExcel = result.pdispVal;
		}
		SysFreeString(varFileName.bstrVal);
	}

	///Sheet�� ���� �� ù��° ��Ʈ ����
	{
		hr = pExcel->GetIDsOfNames(IID_NULL, (OLECHAR**)&szSheets, 1, LOCALE_USER_DEFAULT, &dispID);

		if (SUCCEEDED(hr))
		{
			VARIANT result;
			DISPPARAMS dpNoArgs = { NULL,NULL, 0,0 };

			pExcel->Invoke(dispID, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dpNoArgs, &result, NULL, NULL);
			pSheets = result.pdispVal;
		}

		hr = pSheets->GetIDsOfNames(IID_NULL, (OLECHAR**)&szItem, 1, LOCALE_USER_DEFAULT, &dispID);
		if (SUCCEEDED(hr))
		{
			VARIANT varIndex;
			varIndex.vt = VT_I4;
			varIndex.lVal = 1;		///��Ʈ ��ȣ

			DISPPARAMS dp = { &varIndex, NULL, 1, 0 };
			VARIANT result;

			pSheets->Invoke(dispID, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &result, NULL, NULL);
			pSheet = result.pdispVal;
		}
	}

	///ù ��° �� �� ã��
	{
		hr = pSheet->GetIDsOfNames(IID_NULL, (OLECHAR**)&szCells, 1, LOCALE_USER_DEFAULT, &dispID);
		if (SUCCEEDED(hr))
		{
			VARIANT varRow, varCol;
			varRow.vt = VT_I4;
			varCol.vt = VT_I4;
			varCol.lVal = 1;		///�� ��ȣ

			for (int row = 1; row < 1000; row++)
			{
				varRow.lVal = row;
				VARIANT cellRC[2] = { varRow, varCol };			///���� Ž������ ���� ��,��(���� �߿�!!)

				DISPPARAMS dp = { cellRC, NULL, 2,0 };

				VARIANT result;
				hr = pSheet->Invoke(dispID, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dp, &result, NULL, NULL);
				if (SUCCEEDED(hr))
				{
					pRange = result.pdispVal;

					VARIANT varValue;
					DISPPARAMS dpNoArgs = { NULL, NULL, 0,0 };

					hr = pRange->Invoke(DISPID_VALUE, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dpNoArgs, &varValue, NULL, NULL);
					if (varValue.vt == VT_EMPTY)
					{
						///���� ����ִٸ� Ż��
						break;
					}
				}
			}
		}
	}

	///���� ���Ͽ� �湮�� �̸�(���� ���ϸ�) �߰�
	{
		if (pRange)
		{
			VARIANT varVisitName;
			varVisitName.vt = VT_BSTR;
			varVisitName.bstrVal = SysAllocString(visitName);

			DISPPARAMS dp = { &varVisitName, NULL, 1, 0 };
			pRange->Invoke(DISPID_VALUE, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dp, NULL, NULL, NULL);

			SysFreeString(varVisitName.bstrVal);
		}
	}

	///���� ���� ���泻�� ����
	{
		DISPID dispIDSave;
		hr = pExcel->GetIDsOfNames(IID_NULL, (OLECHAR**)&szSave, 1, LOCALE_USER_DEFAULT, &dispIDSave);

		if (SUCCEEDED(hr))
		{
			DISPPARAMS dpNoArgs = { NULL, NULL, 0, 0 };
			pExcel->Invoke(dispIDSave, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dpNoArgs, NULL, NULL, NULL);
		}
	}
}