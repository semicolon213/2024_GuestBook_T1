#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <vector>

typedef struct pointInfo 
{
	LPARAM lParam;		///ÁÂÇ¥
	int pWidth;			///±½±â
	COLORREF pColor;	///»ö
	ULONGLONG pTime;	///±×¸° ½Ã°£
	UINT state;			

}PINFO;


typedef struct lineInfo
{
	std::vector<PINFO> pInfo;
	//int x;
	//int y;
	//int width;
	//int hieght;
}LINFO;