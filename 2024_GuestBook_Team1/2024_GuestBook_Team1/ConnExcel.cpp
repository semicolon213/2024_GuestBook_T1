/**
@file ConnExcel.cpp
@brief 엑셀관련 클래스 외부 정의 파일
*/

#include "ConnExcel.h"


ConnExcel::ConnExcel()	
{
	///생성자 : OLE(프로그램 연결 기능) 초기화
	OleInitialize(NULL);
}

ConnExcel::~ConnExcel()
{
	///소멸자 : OLE 해체
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
	a.vt = VT_I4;		///열거체 타입을 long형으로 설정
	a.lVal = 1;			///프로그램 표시를 보이지 않게 설정

	hr = appInst ->GetIDsOfNames(IID_NULL, (OLECHAR**)&szVisible, 1, LOCALE_USER_DEFAULT, &dispID);
	if (SUCCEEDED(hr))
	{
		DISPPARAMS dp = { &a,NULL,1,0 };
		appInst->Invoke(dispID, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT, &dp, NULL, NULL, NULL);
	}

	///WorkBoos(Excel 베이스 프로그램)
}