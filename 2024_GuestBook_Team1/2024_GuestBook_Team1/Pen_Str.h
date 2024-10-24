/**
@file Pen_Info.h
@brief 펜 구조체 관련 헤더
*/
#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include<Windows.h>

typedef struct Pen_Info
{
    LPARAM      penCoordinate;     // 좌표
    int         penWidth;          // 펜 굵기
    COLORREF    penColor;          // 펜 색깔
    DWORD       penTime;           // 시간
    UINT        penState;          // 상태 (LBUTTON)

    bool        stampOn;           // 스탬프 활성화 여부
    int         stampImg;          // 스탬프 이미지
    int         stampSize;         // 스탬프 사이즈

} PEN_INFO;





