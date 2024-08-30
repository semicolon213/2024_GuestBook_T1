#pragma once
#include "pch.h"



//#include "Button.h"
namespace fs = std::filesystem;



class FileManager
{
private:
	struct Item   //���ϰ� ������ ������ �����ϱ� ���� ����ü
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

	int list_item_select_; //���� ���õ� ��� �������� �ε���. ���� ��Ͽ��� � �׸��� ���õǾ����� ����. 
	int list_item_height_; // ���� ��Ͽ��� �� �������� ���̸� ��Ÿ��. �� ���� ���� ����� ��ũ�ѹ� ��ġ�� �����

	WCHAR root_path_[256];
	WCHAR current_path_[256];

	bool is_active_;
	RECT scroll_bar_area_; // ��ũ�ѹٰ� ǥ�õ� ������ �����ϴ� �簢�� ����ü 
	RECT file_manager_area_; // ���ϸŴ��� ��ü ������ ��Ÿ���� �簢�� ����ü

	//void ScrollBarControl(POINT mouse_position); // ���콺�� ��ũ�ѹ� ���� ������ �̵��� �� ��ũ�ѹٸ� �����ϴ� �Լ�.
	//����ڰ� ��ũ�ѹٸ� �巡���ϸ� �ش� ��ġ�� ���� ���� ����� ��ũ�� ��.

public:
	FileManager(HWND hWnd); //FileManager ��ü�� ������. ������ �ڵ��� �޾Ƽ� ��ü�� �ʱ�ȭ ��.
	~FileManager() = default; // FileManager ��ü�� �Ҹ���. =default�� �����Ǿ� �־�, Ư���� �۾� ���� �⺻ �Ҹ��ڰ� ȣ��.

	
	void Draw(HDC hdc); // FileManager�� ��ü ui�� �׸��� �Լ�. hdc�� ����Ͽ� ��� ��Ҹ� ȭ�鿡 �׸��� 
	void Active(); // ���ϸŴ����� Ȱ��ȭ 

	void FileRefresh(fs::path path); // ���� ���丮�� ���ΰ�ħ�ϴ� �Լ�. �־��� ��θ� ����� ���� ����� �ٽ� �ε��Ѵ�.

	LPCWSTR GetRootPath(); // FileManager�� ��Ʈ ��θ� ��ȯ�� 
	LPCWSTR GetCurrentPath(); // FileManager�� �ʺ�� ���̸� ��ȯ�� 

	// FileManager�� �ʺ�� ���̸� ��ȯ��
	int GetWidth();
	int GetHeight();
};




