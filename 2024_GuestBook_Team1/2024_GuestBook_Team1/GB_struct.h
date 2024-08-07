#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "GB_Header.h"


typedef struct pointInfo 
{
	LPARAM lParam;		///��ǥ
	int pWidth;			///����
	COLORREF pColor;	///��
	ULONGLONG pTime;	///�׸� �ð�
	UINT state;			

}PINFO;


typedef struct lineInfo
{
	std::vector<PINFO> pInfo;
	int x;
	int y;
	int width;
	int hieght;
}LINFO;