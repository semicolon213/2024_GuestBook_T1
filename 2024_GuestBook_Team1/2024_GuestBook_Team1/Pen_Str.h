/**
@file Pen_Info.h
@brief �� ����ü ���� ���
*/
#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include<Windows.h>

typedef struct Pen_Info
{
    LPARAM      penCoordinate;     // ��ǥ
    int         penWidth;          // �� ����
    COLORREF    penColor;          // �� ����
    DWORD       penTime;           // �ð�
    UINT        penState;          // ���� (LBUTTON)

    bool        stampOn;           // ������ Ȱ��ȭ ����
    int         stampImg;          // ������ �̹���
    int         stampSize;         // ������ ������

} PEN_INFO;





