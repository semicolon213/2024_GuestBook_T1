#include "ConnExcel.h"

/// ���� �� ���� �޼���
LRESULT CALLBACK DrowWindow::WndProcVL(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    DrowWindow* pThis = nullptr;

    if (message == WM_NCCREATE) {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        pThis = reinterpret_cast<DrowWindow*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    }
    else {
        pThis = reinterpret_cast<DrowWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    if (pThis) {
        return pThis->handleMessageVL(hWnd, message, wParam, lParam); // �ν��Ͻ��� ���� �Լ� ȣ��
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

/// ���� �� �޼��� ó�� �ڵ� �޼���
LRESULT DrowWindow::handleMessageVL(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(WndFunc::visitListWnd, &ps);

        EndPaint(WndFunc::visitListWnd, &ps);
        break;
    }
    case WM_LBUTTONDOWN:
	    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        HDC hdc = GetDC(hWnd);
        LineTo(hdc, x, y);

        MessageBox(hWnd, L"VL", L"VL", MB_OK);
        break;
	    }


    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}





/**
@file ConnExcel.cpp
@brief �������� Ŭ���� �ܺ� ���� ����
*/


using namespace std;

std::wstring ConnExcel::list = L"";
/**
@fn  ConnExcel::insertExcel(const wchar_t* excelPath, const wchar_t* visitName)
@brief ���޵� ���� �̸� ������ �����ϴ� �Լ�
@param excelPath ������ ������
@param visitName ����������̸�(�湮�ڸ�)
*/

void ConnExcel::insertExcel(const wstring visitName)
{

    ofstream ofile(EXCEL, ios::app);
    ifstream ifile(EXCEL);
    bool hasHeader = false;

    int rowCnt = 0;

    string line;
    if (ifile.is_open())
    {
        if (getline(ifile, line))
        {
            /// ù ��° ���� ����� ���
            hasHeader = true;
        }

        while (getline(ifile, line))
        {
            rowCnt++;
        }
        ifile.close();
    }
    else
    {
        cerr << "���� ���� ����!" << endl;
    }


    if (ofile.is_open())
    {
        ofile << wcharToChar(visitName) << ",";
        ofile.close();
    }
    else
    {
        cerr << "���� ���� ����!" << endl;
    }

}

wstring ConnExcel::getVisitList()
{
    ofstream ofile(EXCEL, ios::app);
    ifstream ifile(EXCEL);
    bool hasHeader = false;

    string line;
    string visit = "�湮�� �ֽ� �е� :    ";
    if (ifile.is_open())
    {
        if (getline(ifile, line))
        {
            /// ù ��° ���� ����� ���
            hasHeader = true;
        }

        while (getline(ifile, line))
        {
            replace(line.begin(), line.end(), ',', ' ');
            visit = visit.append(line);
        }
        ifile.close();
    }
    else
    {
        cerr << "���� ���� ����!" << endl;
    }

    return charToWchar(visit);
}

/**
@fn  ConnExcel::listScroll(HWND hWnd, int clientWidth)
@brief WM_TIMER�� �Էµ� ������ �湮�� ����� ��ġ�� �������� �ű�� �޼ҵ�
@param hWnd ������ ��鷯�� ������
@param clientWidth ȭ���� ũ�⸦ ����
*/


void ConnExcel::listScroll(HWND hWnd, int clientWidth, RECT mainRT)
{
    while (isListRunning)
    {
        textPosX -= TEXTSPEED;

        if (textPosX < -getTextSize(hWnd, list))
        {
            textPosX = clientWidth;
        }

        InvalidateRect(hWnd, &visitRect, FALSE);

        std::this_thread::sleep_for(std::chrono::milliseconds(10));

    }
}

void ConnExcel::listScrollThread(HWND hWnd, int clientWidth, RECT mainRT)
{
    
    if (listScrollThreadHandle.joinable())
        return;

    else
        //std::thread�� ����Ͽ� �����带 ����
        listScrollThreadHandle = thread(&ConnExcel::listScroll, this, hWnd, clientWidth, mainRT);

    //�����尡 ����� �� �ڵ����� �ڿ��� ��ȯ�ǵ��� ��
    listScrollThreadHandle.detach();
    
}

int ConnExcel::getTextSize(HWND hWnd, wstring list)
{
    SIZE textSize;
    wsprintf(text, list.c_str());
    GetTextExtentPoint32(GetDC(hWnd), text, lstrlen(text), &textSize);

    return textSize.cx;
}

string ConnExcel::wcharToChar(const wstring wcharStr)
{

    int strSize = WideCharToMultiByte(CP_ACP, 0, wcharStr.c_str(), -1, NULL, 0, NULL, NULL);
    if (strSize <= 0) {
        throw runtime_error("�ʿ��� ������ ũ�⸦ ����ϴµ� �����߽��ϴ�");
    }
    string charStr(strSize, 0);

    int result = WideCharToMultiByte(CP_ACP, 0, wcharStr.c_str(), -1, &charStr[0], strSize, NULL, NULL);
    if (result == 0)
    {
        throw runtime_error("wstring to string ��ȯ ����...");
    }
    size_t lastDot = charStr.find_last_of('.');
    if (lastDot != string::npos)
    {
        return charStr.substr(0, lastDot);
    }

    return charStr;

}

wstring ConnExcel::charToWchar(const string charStr)
{

    int wStrSize = MultiByteToWideChar(CP_ACP, 0, charStr.c_str(), -1, nullptr, 0);
    if (wStrSize <= 0)
    {
        throw runtime_error("�ʿ��� ������ ũ�⸦ ����ϴµ� �����߽��ϴ�.");
    }
    wstring wcharStr(wStrSize, 0);

    int result = MultiByteToWideChar(CP_ACP, 0, charStr.c_str(), -1, &wcharStr[0], wStrSize);
    wcharStr.resize(wStrSize - 1);
    if (result == 0)
    {
        throw runtime_error("string to wstring ��ȯ ����...");
    }

    return wcharStr;
}

int ConnExcel::getTextPosX()
{
    return textPosX;
}

void ConnExcel::setTextPosX(int textPosX)
{
    this->textPosX = textPosX;
}

void ConnExcel::setVisitRect(int left, int top, int right, int bottom)
{
    this->visitRect.left = left;
    this->visitRect.top = top;
    this->visitRect.right = right;
    this->visitRect.bottom = bottom;
}
