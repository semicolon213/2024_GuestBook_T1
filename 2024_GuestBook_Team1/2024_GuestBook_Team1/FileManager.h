#pragma once
#include "pch.h"



//#include "Button.h"
namespace fs = std::filesystem;



class FileManager
{
private:
	struct Item   //파일과 폴더의 정보를 저장하기 위한 구조체
	{
		fs::path file_name;
		fs::path file_path;
		uintmax_t file_size;
	};
	HWND hWnd; 

	int x_;
	int y_;
	int width_;
	int height_;

	int list_box_x_;
	int list_box_y_;
	int list_box_width_;
	int list_box_height_;

	RECT list_box_area_;

	std::vector<Item> items_;;

	int list_item_select_; //현재 선택된 목록 아이템의 인덱스. 파일 목록에서 어떤 항목이 선택되었는지 추적. 
	int list_item_height_; // 파일 목록에서 각 아이템의 높이를 나타냄. 이 값을 통해 목록의 스크롤바 위치를 계산함

	WCHAR root_path_[256];
	WCHAR current_path_[256];

	bool is_active_;
	RECT scroll_bar_area_; // 스크롤바가 표시될 영역을 정의하는 사각형 구조체 
	RECT file_manager_area_; // 파일매니저 전체 영역을 나타내는 사각형 구조체

	//void ScrollBarControl(POINT mouse_position); // 마우스가 스크롤바 영역 내에서 이동할 때 스크롤바를 제어하는 함수.
	//사용자가 스크롤바를 드래그하면 해당 위치에 따라 파일 목록이 스크롤 됨.

public:
	FileManager(HWND hWnd); //FileManager 객체의 생성자. 윈도우 핸들을 받아서 객체를 초기화 함.
	~FileManager() = default; // FileManager 객체의 소멸자. =default로 설정되어 있어, 특별한 작업 없이 기본 소멸자가 호출.

	
	void Draw(HDC hdc); // FileManager의 전체 ui를 그리는 함수. hdc를 사용하여 모든 요소를 화면에 그린다 
	void Active(); // 파일매니저를 활성화 

	void FileRefresh(fs::path path); // 현재 디렉토리를 새로고침하는 함수. 주어진 경로를 사용해 파일 목록을 다시 로드한다.

	LPCWSTR GetRootPath(); // FileManager의 루트 경로를 반환함 
	LPCWSTR GetCurrentPath(); // FileManager의 너비와 높이를 반환함 

	// FileManager의 너비와 높이를 반환함
	int GetWidth();
	int GetHeight();
};




