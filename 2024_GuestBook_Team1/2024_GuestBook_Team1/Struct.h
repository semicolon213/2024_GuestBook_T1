#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <vector>
#include "Resource.h"

typedef struct pointInfo
{
	LPARAM lParam;		///��ǥ
	int pWidth;			///����
	COLORREF pColor;	///��
	ULONGLONG pTime;	///�׸� �ð�
	UINT state;			///���콺 ����
	int bShape;		///�귯�� ���

}PINFO;


typedef struct lineInfo
{
	std::vector<PINFO> pInfo;
}LINFO;
