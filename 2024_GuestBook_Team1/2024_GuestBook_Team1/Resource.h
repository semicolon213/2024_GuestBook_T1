//{{NO_DEPENDENCIES}}
// Microsoft Visual C++에서 생성한 포함 파일입니다.
// 다음에서 사용 2024_GuestBook_Team1.rc
#pragma once

#define ID_RIGHT_PANEL			1002
#define IDS_APP_TITLE			103
#define ID_FILE_LIST			101
#define IDR_MAINFRAME			128
#define IDD_MY2024GUESTBOOKTEAM1_DIALOG	102
#define IDD_ABOUTBOX			103
#define IDM_ABOUT				104
#define IDM_EXIT				105

#define IDI_MY2024GUESTBOOKTEAM1			107
#define IDI_SMALL				108
#define IDC_MY2024GUESTBOOKTEAM1			109
#define IDI_YUHAN256			256
#define IDI_YUHAN				7065
#define IDC_MYICON				2

///사용자 정의 상수

// 브러쉬 종류 버튼 
#define BASIC 300
#define BRUSH 301
#define PENCIL 302 
#define SPRAY 303
#define MARKER 304
#define WATERCOLOR 305

// 버튼 호출 상수
#define COLOR1 1001
#define COLOR2 1002
#define COLOR3 1003
#define P_WIDTH 1004
#define CLEAR 1005
#define NEW_FILE 1006
#define SAVE	 1007
#define LOAD 1008
#define FILE_MANAGER 1009
#define CREDIT 1010


#ifndef IDC_STATIC
#define IDC_STATIC				-1


#define DEF_ID						65000
#define BUTTON_ID (DEF_ID + 1)
#define DEF_DROW_BT					(DEF_ID + 1)
#define DEF_LOAD_BT					(DEF_ID + 2)
#define DEF_CREDIT_BT				(IDM_ABOUT)



//DrowWindow
#define DW_DEF						(DEF_ID + 100)




#define CANVAS_DEF					(DEF_ID + 200)




#define NAMEBAR_DEF					(DEF_ID + 300)

#define NB_BACK_BT					(NAMEBAR_DEF + 1)
#define NB_SIDE_BT					(NAMEBAR_DEF + 2)
#define NB_FILE_NAME				(NAMEBAR_DEF + 3)


#define SIDE_DEF					(DEF_ID + 400)

#define SD_NEWFILE_BT				(SIDE_DEF + 1)
#define SD_SAVEFILE_BT				(SIDE_DEF + 2)
#define SD_LOADFILE_BT				(DEF_LOAD_BT)
#define SD_FILEMANAGER_BT			(SIDE_DEF + 4)
#define SD_CREDIT_BT				(DEF_CREDIT_BT)



#define TOOL_DEF					(DEF_ID + 500)

#define TL_COLOR1_BT				(TOOL_DEF + 1)
#define TL_COLOR2_BT				(TOOL_DEF + 2)
#define TL_COLOR3_BT				(TOOL_DEF + 3)
#define TL_PEN_WIDTH_BT				(TOOL_DEF + 4)
#define TL_CLEAR_BT					(TOOL_DEF + 5)
#define TL_SAVE_BT					(SD_SAVEFILE_BT)
#define TL_PLAY_BT					(TOOL_DEF + 7)
#define TL_RESET_BT					(TOOL_DEF + 8)




#endif
// 다음은 새 개체에 사용할 기본값입니다.
//
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS

#define _APS_NO_MFC					130
#define _APS_NEXT_RESOURCE_VALUE	129
#define _APS_NEXT_COMMAND_VALUE		32771
#define _APS_NEXT_CONTROL_VALUE		1000
#define _APS_NEXT_SYMED_VALUE		110
#endif
#endif

