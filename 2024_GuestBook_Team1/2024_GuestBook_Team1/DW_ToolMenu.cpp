#include "DW_ToolMenu.h"
#include "DW_NameBar.h"
#include "PenThickness.h"

std::unique_ptr<ConnExcel> connExcel = std::make_unique<ConnExcel>();

/// ���� �� ���� �޼���
LRESULT CALLBACK DrowWindow::WndProcTB(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
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
        return pThis->handleMessageTB(hWnd, message, wParam, lParam); // �ν��Ͻ��� ���� �Լ� ȣ��
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

const int xOffset = 400; // �÷�â x ��ǥ
const int yOffset = 100; // �÷�â y ��ǥ

/// �� ���� ��ư ���� (â ũ�� �����̳� ���÷��̽ÿ��� ��ġ ���� ���� �����ڷ� ��ǥ �ʱ�ȭ)
MakeButton basicPenButton(10, 10, 40, 40);
MakeButton pencilButton(50, 10, 80, 40);
MakeButton brushButton(90, 10, 120, 40);
MakeButton sprayButton(130, 10, 160, 40);
//MakeButton rectpenButton(170, 10, 200, 40);
MakeButton waterpenButton(170, 10, 200, 40);
MakeButton visitListButton(1200, 10, 1230, 40);

/// ������ ���� ��ư ����
MakeButton heartButton(250, 10, 280, 40);
MakeButton uhButton(290, 10, 320, 40);
MakeButton yuhanButton(330, 10, 360, 40);
MakeButton pfButton(370, 10, 400, 40);
MakeButton guButton(410, 10, 440, 40);

/// ���� ���� ��ư ���� (�׸���� WM_PAINT�� ó��)
MakeButton colorButton1;
MakeButton colorButton2;
MakeButton colorButton3;

/// �Ʒ� 3���� ��ư�� ���÷��̽� or â ũ�� ���� �� ��ġ �̵��� �־� 
/// DW_ToolMenu::Create���� �ʱ� ��ǥ ����
/// DW_ToolMenu::HandleMessage�� WM_SIZE�� WM_LBUTTONDOWN�� ����, ���÷��� ��ư�� ��ǥ �̵�

MakeButton eraseButton;      /// ���찳 ��ư ���� 
MakeButton playButton;      /// �÷��� ��ư ����
MakeButton stopButton;      /// ���� ��ư ����
//MakeButton saveButton;      /// ���̺� ��ư ����

//ColorPalette colorGet;      /// ���� �޾ƿ���� �ν��Ͻ� 

PAINTSTRUCT t_ps = { 0 };
HBRUSH ToolBrush = nullptr;
HPEN ToolPen = nullptr;
HDC tHdc = nullptr;

RECT a;         /// IntersectRect ��ȯ�� RECT
RECT mouse;      /// ���콺 ��ǥ ���� RECT ����

/// LBUTTONDOWN�� ���� selectedBrushButton�� selectedIcon�� ������Ʈ
/// ����Ʈ ó���� WM_PAINT���� ó��
/// Ŭ���� selectedBrushButton ������ �ش� RECT ������ ����
/// buttonLoadImage �޼���� �ش� Rect�� ����Ʈ ����
MakeButton* selectedBrushButton = nullptr;  /// ���� ���õ� ��ư
int selectedIcon = 0;                  /// ���� ���õ� ������ ID

/// Ŭ���� ���� ������ ����
MakeButton* selectedColorButton = nullptr;  // ���� ���õ� ���� ��ư

bool replayStay = false;   /// ���÷��� ���� �� ȭ�� ��ȿȭ �߻��ص� ���� ��ǥ ���� ���� ����

/// ���� �� �޼��� ó�� �ڵ� �޼���
LRESULT DrowWindow::handleMessageTB(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    case WM_COMMAND:
    {
        switch (wParam)
        {
        case TL_PLAY_BT:
            if (function->getDrawLInfoEmpty())
            {
                break;
            }

            replayStay = true;   /// replay�� true�� �����Ͽ� WM_SIZE ������ �����
            playButton.toggleState = !playButton.toggleState;   /// ��ư ���������� �̹��� ��ü ���� �� ����


            if (pCnt)
            {
                if (!function->getIsReplay())
                {
                    int midPoint = WndFunc::wndSize.right / 2; //�߽� ��ǥ ���

                    SendMessage(WndFunc::canvasWnd, WM_COMMAND, TL_PLAY_BT, 0);

                    /// ���÷��� �� �귯�� ��ư ������ �ٸ� ��ư�� �Ⱥ��̰� ����
                    //saveButton.setCoordinate(-50, -50, -50, -50);
                    eraseButton.setCoordinate(-50, -50, -50, -50);
                    colorButton1.setCoordinate(-50, -50, -50, -50);
                    colorButton2.setCoordinate(-50, -50, -50, -50);
                    colorButton3.setCoordinate(-50, -50, -50, -50);

                    /// ���, ���� ��ư ��ġ ����
                    playButton.setCoordinate(midPoint - 50, 10, midPoint - 20, 40);
                    stopButton.setCoordinate(midPoint + 20, 10, midPoint + 50, 40);

                    function->setIsReplay(true);
                    pCnt = false;
                    InvalidateRect(WndFunc::toolWnd, NULL, true);
                }
                else
                {
                    SendMessage(WndFunc::canvasWnd, WM_COMMAND, TL_PLAY_BT, 0);
                    pCnt = false;
                }
            }
            else
            {
                SendMessage(WndFunc::canvasWnd, WM_COMMAND, TL_PLAY_BT, 1);
                pCnt = true;
            }

            InvalidateRect(WndFunc::toolWnd, NULL, true);
            UpdateWindow(WndFunc::toolWnd);
        }
        break;
    }
    case WM_CREATE:
    {
        function = std::make_unique<Function>();

        //colorPalette = make_unique<ColorPalette>();

        int midPoint = WndFunc::wndSize.right / 2;

        /// ���α׷� ���� �� ����Ʈ ������ ������ ����
        selectedBrushButton = &basicPenButton;   /// �⺻ �� ��ư ����
        selectedIcon = IDI_PEN_ICON;         /// �⺻ �� ������ ����
        selectedColorButton = &colorButton1;   /// ���α׷� ���� �� �÷� ��ư1 �⺻��

        eraseButton.setCoordinate(midPoint + 50, 10, midPoint + 80, 40);
        playButton.setCoordinate(midPoint + 115, 10, midPoint + 145, 40);
        stopButton.setCoordinate(midPoint + 160, 10, midPoint + 190, 40);

        break;
    }
    case WM_LBUTTONDOWN:
    {
        /// ĵ�������� �׸� �� ���� â ���������� ����
        if (IsWindowVisible(WndFunc::colorWnd))
        {
            //function->setisLeftClick(false);
            ShowWindow(WndFunc::colorWnd, SW_HIDE);
            break;
        }
        HDC hdc = GetDC(hWnd);

        /// ���� ���콺 ��ǥ�� �簢�� ����
        mouse.left = LOWORD(lParam);
        mouse.top = HIWORD(lParam);
        mouse.right = mouse.left + 1;
        mouse.bottom = mouse.top + 1;

        /// �湮�� ������ on/off
        if (IntersectRect(&a, &mouse, &visitListButton.rectButton)) 
        {
            visitListButton.toggleState = !visitListButton.toggleState;   /// ��ư ���������� �̹��� ��ü ���� �� ����

            if (lCnt)
            {
                if (!connExcel->getIsStart())
                {
                    SendMessage(WndFunc::visitListWnd, WM_COMMAND, TL_PLAY_BT, 0);

                    connExcel->setIsStart(true);
                    lCnt = false;
                }
                else
                {
                    SendMessage(WndFunc::visitListWnd, WM_COMMAND, TL_PLAY_BT, 0);
                    lCnt = false;
                }
            }
            else {
                SendMessage(WndFunc::visitListWnd, WM_COMMAND, TL_PLAY_BT, 1);
                lCnt = true;
            }
        }


        // ��: �⺻ �� ���� �ڵ� ����
        if (IntersectRect(&a, &mouse, &basicPenButton.rectButton)) {
            function->setBShape(BASIC);
            selectedBrushButton = &basicPenButton;
            selectedIcon = IDI_PEN_ICON;

            // ĵ������ ���� ���õ� �� ��� �ݿ�
            SendMessage(WndFunc::canvasWnd, WM_LBUTTONUP, 0, MAKELPARAM(0, 0));
        }
        else if (IntersectRect(&a, &mouse, &pencilButton.rectButton)) {
            function->setBShape(PENCIL);
            selectedBrushButton = &pencilButton;
            selectedIcon = IDI_PENCIL_ICON;

            // ĵ������ ���� ���õ� �� ��� �ݿ�
            SendMessage(WndFunc::canvasWnd, WM_LBUTTONUP, 0, MAKELPARAM(0, 0));
        }
        /// �귯��
        else if (IntersectRect(&a, &mouse, &brushButton.rectButton)) {
            function->setBShape(BRUSH);

            selectedBrushButton = &brushButton;
            selectedIcon = IDI_BRUSH_ICON;
            // ĵ������ ���� ���õ� �� ��� �ݿ�
            SendMessage(WndFunc::canvasWnd, WM_LBUTTONUP, 0, MAKELPARAM(0, 0));
        }
        /// ��������
        else if (IntersectRect(&a, &mouse, &sprayButton.rectButton)) {
            function->setBShape(SPRAY);


            selectedBrushButton = &sprayButton;
            selectedIcon = IDI_SPRAY_ICON;
            // ĵ������ ���� ���õ� �� ��� �ݿ�
            SendMessage(WndFunc::canvasWnd, WM_LBUTTONUP, 0, MAKELPARAM(0, 0));
        }
        /// ����
        else if (IntersectRect(&a, &mouse, &waterpenButton.rectButton)) {
            function->setBShape(WATERCOLOR);

            selectedBrushButton = &waterpenButton;
            selectedIcon = IDI_WATERPEN_ICON;
            // ĵ������ ���� ���õ� �� ��� �ݿ�
            SendMessage(WndFunc::canvasWnd, WM_LBUTTONUP, 0, MAKELPARAM(0, 0));
        }

        /// ��Ʈ ������
        else if (IntersectRect(&a, &mouse, &heartButton.rectButton)) {
            function->setBShape(STAMP);
            function->stampIcon(IDI_HEART_ICON);

            selectedBrushButton = &heartButton;
            selectedIcon = IDI_HEART_ICON;
        }
        /// ��? ������
        else if (IntersectRect(&a, &mouse, &uhButton.rectButton)) {
            function->setBShape(STAMP);
            function->stampIcon(IDI_UH_ICON);

            selectedBrushButton = &uhButton;
            selectedIcon = IDI_UH_ICON;
        }
        /// ���Ѵ� ��ũ ������
        else if (IntersectRect(&a, &mouse, &yuhanButton.rectButton)) {
            function->setBShape(STAMP);
            function->stampIcon(IDI_YUHAN_ICON);

            selectedBrushButton = &yuhanButton;
            selectedIcon = IDI_YUHAN_ICON;
        }
        /// �������� ������
        else if (IntersectRect(&a, &mouse, &pfButton.rectButton)) {
            function->setBShape(STAMP);
            function->stampIcon(IDI_PF_ICON);

            selectedBrushButton = &pfButton;
            selectedIcon = IDI_PF_ICON;
        }
        /// ��ѱ� ������
        else if (IntersectRect(&a, &mouse, &guButton.rectButton)) {
            function->setBShape(STAMP);
            function->stampIcon(IDI_GU_ICON);

            selectedBrushButton = &guButton;
            selectedIcon = IDI_GU_ICON;
        }

        // colorButton1�� ���� ó��
        else if (IntersectRect(&a, &mouse, &colorButton1.rectButton))
        {
            if (function->getIsReplay()) {
                // ���÷��� ���� ���� ���� ���� �ڽ��� ���� ����
                break;
            }

            // ���� ���� �ڽ��� ���� ��ġ ����
            int fixedX = 320; // ���� x ��ǥ
            int fixedY = 10;  // ���� y ��ǥ

            // ���� ���� �ڽ��� ��ġ�� ȭ�� ��ǥ �������� ����
            RECT screenRect = colorButton1.rectButton;
            ClientToScreen(hWnd, reinterpret_cast<POINT*>(&screenRect.left));
            ClientToScreen(hWnd, reinterpret_cast<POINT*>(&screenRect.right));

            // ���� ��ư�� ���� �ڽ� �����
            if (selectedColorButton != &colorButton1) {
                ShowWindow(WndFunc::colorWnd, SW_HIDE);
            }

            // ���� ���� �ڽ��� ��ġ�� ������ ��ġ�� ����
            SetWindowPos(WndFunc::colorWnd, HWND_TOP, fixedX, fixedY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
            InvalidateRect(WndFunc::colorWnd, NULL, TRUE);

            // ���� ���� �ڽ� ǥ��
            if (DW_ColorBox::colorSelect == 0) {
                ShowWindow(WndFunc::colorWnd, SW_SHOW);
            }
            else {
                DW_ColorBox::colorSelect = 0;
            }

            selectedColorButton = &colorButton1; // ������ ��ư ����
        }

        // colorButton2�� ���� ó��
        else if (IntersectRect(&a, &mouse, &colorButton2.rectButton))
        {
            if (function->getIsReplay()) {
                break;
            }

            // ���� ���� �ڽ��� ���� ��ġ ����
            int fixedX = 370; // ���� x ��ǥ
            int fixedY = 10;  // ���� y ��ǥ

            // ���� ���� �ڽ��� ��ġ�� ȭ�� ��ǥ �������� ����
            RECT screenRect = colorButton2.rectButton;
            ClientToScreen(hWnd, reinterpret_cast<POINT*>(&screenRect.left));
            ClientToScreen(hWnd, reinterpret_cast<POINT*>(&screenRect.right));

            // ���� ��ư�� ���� �ڽ� �����
            if (selectedColorButton != &colorButton2) {
                ShowWindow(WndFunc::colorWnd, SW_HIDE);
            }

            // ���� ���� �ڽ��� ��ġ�� ������ ��ġ�� ����
            SetWindowPos(WndFunc::colorWnd, HWND_TOP, fixedX, fixedY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
            InvalidateRect(WndFunc::colorWnd, NULL, TRUE);
            if (DW_ColorBox::colorSelect == 1) {
                ShowWindow(WndFunc::colorWnd, SW_SHOW);
            }
            else {
                DW_ColorBox::colorSelect = 1;
            }
            selectedColorButton = &colorButton2; // ������ ��ư ����
        }

        // colorButton3�� ���� ó��
        else if (IntersectRect(&a, &mouse, &colorButton3.rectButton))
        {
            if (function->getIsReplay()) {
                break;
            }

            // ���� ���� �ڽ��� ���� ��ġ ����
            int fixedX = 420; // ���� x ��ǥ
            int fixedY = 10;  // ���� y ��ǥ

            // ���� ���� �ڽ��� ��ġ�� ȭ�� ��ǥ �������� ����
            RECT screenRect = colorButton3.rectButton;
            ClientToScreen(hWnd, reinterpret_cast<POINT*>(&screenRect.left));
            ClientToScreen(hWnd, reinterpret_cast<POINT*>(&screenRect.right));

            // ���� ��ư�� ���� �ڽ� �����
            if (selectedColorButton != &colorButton3) {
                ShowWindow(WndFunc::colorWnd, SW_HIDE);
            }

            // ���� ���� �ڽ��� ��ġ�� ������ ��ġ�� ����
            SetWindowPos(WndFunc::colorWnd, HWND_TOP, fixedX, fixedY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
            InvalidateRect(WndFunc::colorWnd, NULL, TRUE);

            if (DW_ColorBox::colorSelect == 2) {
                ShowWindow(WndFunc::colorWnd, SW_SHOW);
            }
            else {
                DW_ColorBox::colorSelect = 2;
            }
            selectedColorButton = &colorButton3; // ������ ��ư ����
        }
        
       


        /// ���찳 ��ư 
        else if (IntersectRect(&a, &mouse, &eraseButton.rectButton)) {

            if (function->getDrawLInfoEmpty()) { break; }
            if (!function->getIsReplay()) {
                SendMessage(WndFunc::canvasWnd, WM_COMMAND, TL_CLEAR_BT, 0);
                SendMessage(WndFunc::nameWnd, WM_SETTEXT, 0, (LPARAM)L"�̸� ����");
            }

        }

        /// ���÷��� ��ư
        else if (IntersectRect(&a, &mouse, &playButton.rectButton)) {

            if (function->getDrawLInfoEmpty()) { break; }

            replayStay = true;   /// replay�� true�� �����Ͽ� WM_SIZE ������ �����
            playButton.toggleState = !playButton.toggleState;   /// ��ư ���������� �̹��� ��ü ���� �� ����

            if (pCnt)
            {
                if (!function->getIsReplay())
                {
                    int midPoint = WndFunc::wndSize.right / 2; //�߽� ��ǥ ���

                    SendMessage(WndFunc::canvasWnd, WM_COMMAND, TL_PLAY_BT, 0);

                    /// ���÷��� �� �귯�� ��ư ������ �ٸ� ��ư�� �Ⱥ��̰� ����
                    //saveButton.setCoordinate(-50, -50, -50, -50);
                    eraseButton.setCoordinate(-50, -50, -50, -50);
                    colorButton1.setCoordinate(-50, -50, -50, -50);
                    colorButton2.setCoordinate(-50, -50, -50, -50);
                    colorButton3.setCoordinate(-50, -50, -50, -50);

                    /// ���, ���� ��ư ��ġ ����
                    playButton.setCoordinate(midPoint - 50, 10, midPoint - 20, 40);
                    stopButton.setCoordinate(midPoint + 20, 10, midPoint + 50, 40);

                    function->setIsReplay(true);
                    pCnt = false;
                    InvalidateRect(WndFunc::toolWnd, NULL, true);
                }
                else
                {
                    SendMessage(WndFunc::canvasWnd, WM_COMMAND, TL_PLAY_BT, 0);
                    pCnt = false;
                }
            }
            else {
                SendMessage(WndFunc::canvasWnd, WM_COMMAND, TL_PLAY_BT, 1);
                pCnt = true;
            }
            

            InvalidateRect(WndFunc::toolWnd, NULL, true);
            UpdateWindow(WndFunc::toolWnd);

        }

        /// ���� ��ư
        else if (IntersectRect(&a, &mouse, &stopButton.rectButton)) {

            InvalidateRect(WndFunc::toolWnd, nullptr, true);

            if (!function->getIsReplay()) {
                SendMessage(WndFunc::canvasWnd, WM_COMMAND, TL_RESET_BT, 0);
            }
            else
            {
                SendMessage(WndFunc::canvasWnd, WM_COMMAND, TL_RESET_BT, 0);
                SendMessage(WndFunc::canvasWnd, WM_PAINT, TL_RESET_BT, 0);
                int midPoint = WndFunc::wndSize.right / 2;   /// ������ �߽� ��ǥ ���

                /// ������ư Ŭ�� ��, ���÷��� ������ ���찳, ���� ��ư ���� �ڸ��� ����
                eraseButton.setCoordinate(midPoint + 50, 10, midPoint + 80, 40);
                playButton.setCoordinate(midPoint + 115, 10, midPoint + 145, 40);
                stopButton.setCoordinate(midPoint + 160, 10, midPoint + 190, 40);

                //saveButton.setCoordinate((WndFunc::wndSize.right - 50, 10, toolRT.right - 20, 40);
                playButton.toggleState = false;      /// ��� �� �����ص� �ʱ� ���������� ����

            }
            replayStay = false;      /// ���÷��� ���� ����
            pCnt = true;
            function->setisLeftClick(false);
            InvalidateRect(WndFunc::toolWnd, NULL, true);
        }
        ///// ���� ��ư

        InvalidateRect(WndFunc::toolWnd, NULL, true); //ȭ�� ����
        ReleaseDC(hWnd, hdc);

        break;
    }

    /// ���� ����� �� WM_PAINT ����
/*
 case WM_PAINT:
 {
     PAINTSTRUCT ps;
     HDC hdc = BeginPaint(hWnd, &ps);
     int midPoint = WndFunc::wndSize.right / 2;

     playButton.setCoordinate(midPoint + 115, 10, midPoint + 145, 40);
     stopButton.setCoordinate(midPoint + 160, 10, midPoint + 190, 40);

     playButton.doubleImgButton(hdc, IDI_PAUSE_ICON, IDI_PLAY_ICON);
     stopButton.drawRectButton(hdc, IDI_STOP_ICON);
     visitListButton.doubleImgButton(hdc, IDI_PAUSE_ICON, IDI_PLAY_ICON);

     if (WndFunc::buttonOn)
     {

         eraseButton.setCoordinate(midPoint + 50, 10, midPoint + 80, 40);
         /// ��ư�� �̹��� �׸���
         basicPenButton.drawRectButton(hdc, IDI_PEN_ICON);
         pencilButton.drawRectButton(hdc, IDI_PENCIL_ICON);
         brushButton.drawRectButton(hdc, IDI_BRUSH_ICON);
         sprayButton.drawRectButton(hdc, IDI_SPRAY_ICON);
         rectpenButton.drawRectButton(hdc, IDI_RECTPEN_ICON);
         waterpenButton.drawRectButton(hdc, IDI_WATERPEN_ICON);
         eraseButton.drawRectButton(hdc, IDI_ERASE_ICON);



         //saveButton.drawRectButton(memDC, IDI_SAVE_ICON);

         /// ���õ� �귯�� ��ư�� ����Ʈ ����
         if (selectedBrushButton != nullptr) {
             selectedBrushButton->clickEffectPen(IDI_PENEFFECT_ICON, selectedIcon, hdc);
         }

         /// �÷� ��ư �׸��� (Ellipse�� ����� ���� ���� WM_PAINT���� �׸��� + ��ǥó��)
         colorButton1.setCoordinate(midPoint - 140, 10, midPoint - 110, 40);
         colorButton2.setCoordinate(midPoint - 90, 10, midPoint - 60, 40);
         colorButton3.setCoordinate(midPoint - 40, 10, midPoint - 10, 40);

         /// ���õ� �÷� ��ư�� ����Ʈ ����
         /// ����Ʈ ���� �׸� �� ���� ��ư�� �׸�(drawEllipseButton)
         if (selectedColorButton != nullptr) {
             selectedColorButton->clickEffectPen(IDI_COLOREFFECT_ICON, hdc);
         }

         colorButton1.drawEllipseButton(hdc, DW_ColorBox::getColorNum(0));   /// ���� ��ư 1 �̸�����
         colorButton2.drawEllipseButton(hdc, DW_ColorBox::getColorNum(1));   /// ���� ��ư 2 �̸�����
         colorButton3.drawEllipseButton(hdc, DW_ColorBox::getColorNum(2));   /// ���� ��ư 3 �̸�����
     }
     EndPaint(hWnd, &ps);
     break;
 }
 */
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // ���� ���۸��� ���� �޸� DC ����
        HDC memDC = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, WndFunc::wndSize.right, WndFunc::wndSize.bottom);
        HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);

        // ����� ������� ä���
        HBRUSH hWhiteBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
        RECT rect;
        GetClientRect(hWnd, &rect);
        FillRect(memDC, &rect, hWhiteBrush);

        int midPoint = WndFunc::wndSize.right / 2;

        

        playButton.setCoordinate(midPoint + 115, 10, midPoint + 145, 40);
        stopButton.setCoordinate(midPoint + 160, 10, midPoint + 190, 40);

        // �޸� DC�� ��ư�� �׸���
        playButton.doubleImgButton(memDC, IDI_PAUSE_ICON, IDI_PLAY_ICON);
        stopButton.drawRectButton(memDC, IDI_STOP_ICON);
        visitListButton.doubleImgButton(memDC, IDI_PAUSE_ICON, IDI_PLAY_ICON);

        if (WndFunc::buttonOn)
        {
            MoveToEx(memDC, 225, 10, NULL);
            LineTo(memDC, 225, 40);

            eraseButton.setCoordinate(midPoint + 50, 10, midPoint + 80, 40);

            // ��ư�� �̹��� �׸���
            basicPenButton.drawRectButton(memDC, IDI_PEN_ICON);
            pencilButton.drawRectButton(memDC, IDI_PENCIL_ICON);
            brushButton.drawRectButton(memDC, IDI_BRUSH_ICON);
            sprayButton.drawRectButton(memDC, IDI_SPRAY_ICON);
            //rectpenButton.drawRectButton(memDC, IDI_RECTPEN_ICON);
            waterpenButton.drawRectButton(memDC, IDI_WATERPEN_ICON);
            eraseButton.drawRectButton(memDC, IDI_ERASE_ICON);

            // ������ ��ư �߰�
            heartButton.drawRectButton(memDC, IDI_HEART_ICON);
            uhButton.drawRectButton(memDC, IDI_UH_ICON);
            yuhanButton.drawRectButton(memDC, IDI_YUHAN_ICON);
            pfButton.drawRectButton(memDC, IDI_PF_ICON);
            guButton.drawRectButton(memDC, IDI_GU_ICON);


            // ���õ� �귯�� ��ư�� ����Ʈ ����
            if (selectedBrushButton != nullptr) {
                selectedBrushButton->clickEffectPen(IDI_PENEFFECT_ICON, selectedIcon, memDC);
            }

            // �÷� ��ư ��ǥ ���� �� ����Ʈ ����
            colorButton1.setCoordinate(midPoint - 140, 10, midPoint - 110, 40);
            colorButton2.setCoordinate(midPoint - 90, 10, midPoint - 60, 40);
            colorButton3.setCoordinate(midPoint - 40, 10, midPoint - 10, 40);

            if (selectedColorButton != nullptr) {
                selectedColorButton->clickEffectPen(IDI_COLOREFFECT_ICON, memDC);
            }

            // ���� ��ư �׸���
            colorButton1.drawEllipseButton(memDC, DW_ColorBox::getColorNum(0));
            colorButton2.drawEllipseButton(memDC, DW_ColorBox::getColorNum(1));
            colorButton3.drawEllipseButton(memDC, DW_ColorBox::getColorNum(2));
        }

        // �޸� DC�� ������ ȭ�� DC�� �����Ͽ� ���
        BitBlt(hdc, 0, 0, WndFunc::wndSize.right, WndFunc::wndSize.bottom, memDC, 0, 0, SRCCOPY);

        // ���ҽ� ����
        SelectObject(memDC, oldBitmap);
        DeleteObject(hBitmap);
        DeleteDC(memDC);

        EndPaint(hWnd, &ps);
        break;
    }

    case WM_ERASEBKGND:
    return 1; // �⺻ ��� ����� ����
     default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}



/// ���� �� �޼��� ó�� �ڵ� �޼���
LRESULT DrowWindow::handleMessageVL(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    case WM_COMMAND:
    {
        if (wParam == TL_PLAY_BT && lParam == 0)
        {
            if (!connExcel->getIsStart())
            {
                connExcel->listScrollThread(hWnd, WndFunc::wndSize.right, WndFunc::wndSize);

                ConnExcel::list = connExcel->getVisitList().c_str();

                connExcel->setTextPosX(WndFunc::wndSize.right);
            }
            else
            {
                connExcel->resumeScroll();
            }
        }


        if (wParam == TL_PLAY_BT && lParam == 1)
        {
            connExcel->suspendScroll();
        }
        break;
    }
    case WM_CREATE:
    {


        break;
    }
    case WM_DESTROY:
    {
        connExcel->stopThread();
        break;
    }
    case WM_PAINT:
    {

        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(WndFunc::visitListWnd, &ps);

        SIZE textSize = { 0 };
        wsprintf(text, ConnExcel::list.c_str());
        SetBkColor(hdc, RGB(249, 243, 240));

        HFONT hFont = CreateFont(17, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
            DEFAULT_PITCH | FF_SWISS, TEXT("�������"));
        HFONT holdFont = (HFONT)SelectObject(hdc, hFont);
        TextOut(hdc, connExcel->getTextPosX(), 5, text, lstrlen(text));
        SelectObject(hdc, holdFont);
        DeleteObject(hFont);


        EndPaint(hWnd, &ps);

        // EndPaint(WndFunc::visitListWnd, &ps);
        break;
    }


    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}