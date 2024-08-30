/**
@file ConnExcel.cpp
@brief 엑셀관련 클래스 외부 정의 파일
*/

#include "ConnExcel.h"

ConnExcel::ConnExcel()
{
	///---------- 소멸자 --------------

	///OLE(프로그램 연결 기능) 초기화
	OleInitialize(NULL);
}

ConnExcel::~ConnExcel()
{
	///---------- 소멸자 --------------
	
	///사용이 끝난 객체 싹 다 해제
	if (pRange) pRange->Release();
	if (pSheet) pSheet->Release();
	if (pSheets) pSheets->Release();
	if (pExcel) pExcel->Release();
	if (pWorkBooks) pWorkBooks->Release();
	if (appInst) pRange->Release();

	///OLE 해체
	OleUninitialize();
}

/**
@fn  ConnExcel::insertExcel(const wchar_t* excelPath, const wchar_t* visitName)
@brief 전달된 파일 이름 엑셀로 저장하는 함수
@param excelPath 엑셀의 저장경로
@param visitName 저장된파일이름(방문자명)
*/

void ConnExcel::insertExcel(const wchar_t* excelPath, const wchar_t* visitName)
{
	///CLSID(프로그램 참조 레지스트리) 가져오기
	hr = CLSIDFromProgID(L"Excel.Application", &clsid);
	if (FAILED(hr))
	{
		wcout << L"Excel CLSID 가져오기 실패" << endl;
		return;
	}

	///프로그램(엑셀) 인스턴스 생성
	hr = CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER, IID_IDispatch, (void**)&appInst);
	if (FAILED(hr) || appInst == NULL)
	{
		wcout << L"Excel 인스턴스 생성 실패" << endl;
		return;
	}

	///Excel 표시설정 - 보이지 않게
	{
		VARIANT a;			///여러 타입을 가지고 있는 열거체 변수

		a.vt = VT_I4;		///열거체 타입을 long형으로 설정
		a.lVal = 1;			///프로그램 표시를 보이지 않게 설정

		hr = appInst->GetIDsOfNames(IID_NULL, (OLECHAR**)&szVisible, 1, LOCALE_USER_DEFAULT, &dispID);
		if (SUCCEEDED(hr))
		{
			DISPPARAMS dp = { &a, NULL, 1, 0 };
			appInst->Invoke(dispID, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT, &dp, NULL, NULL, NULL);
		}
	}

	///WorkBooks(Excel 베이스 프로그램) 열기
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

	///Excel 파일 열기
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

	///Sheet를 선택 후 첫번째 시트 선택
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
			varIndex.lVal = 1;		///시트 번호

			DISPPARAMS dp = { &varIndex, NULL, 1, 0 };
			VARIANT result;

			pSheets->Invoke(dispID, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dp, &result, NULL, NULL);
			pSheet = result.pdispVal;
		}
	}

	///첫 번째 빈 행 찾기
	{
		hr = pSheet->GetIDsOfNames(IID_NULL, (OLECHAR**)&szCells, 1, LOCALE_USER_DEFAULT, &dispID);
		if (SUCCEEDED(hr))
		{
			VARIANT varRow, varCol;
			varRow.vt = VT_I4;
			varCol.vt = VT_I4;
			varCol.lVal = 1;		///열 번호

			for (int row = 1; row < 1000; row++)
			{
				varRow.lVal = row;
				VARIANT cellRC[2] = { varRow, varCol };			///현재 탐색중인 셀의 열,행(순서 중요!!)

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
						///셀이 비어있다면 탈출
						break;
					}
				}
			}
		}
	}

	///엑셀 파일에 방문자 이름(저장 파일명) 추가
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

	///엑셀 파일 변경내용 저장
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