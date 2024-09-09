#pragma once

#include <Windows.h>
#include <vector>
#include <string>
#include <fstream>
#include "Resource.h"



#define ID_FILE_LIST 101


class FileManager
{
private:
	
	 HWND hRightPanel;
	 HWND hFileListBox;
	 HINSTANCE hInst;
	 HWND hWnd;
	 


	 
	 

public:
	static FileManager& getInstance() {
		static FileManager instance;
		return instance;
	}
	
	FileManager();
	FileManager(HWND hWnd);
	~FileManager() = default;
	/*�г� �Լ�*/
	void InitializePanels(HWND hWnd);
	void ResizePanels(HWND hWnd, LPARAM lParam);

	/*��ư �Լ�*/
	void AddFileToList(const std::wstring& fileName);
	void UpdateFileListUI();
	void SaveFile();


};






