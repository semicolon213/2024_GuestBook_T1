#include "pch.h"
#include "FileManager.h"
#include "Window.h"



using namespace std;
using namespace Gdiplus;


FileManager::FileManager(HWND hWnd)
{
	this->hWnd = hWnd;
	wsprintf(root_path_, L"%s\\Guests", fs::current_path().c_str());
	wsprintf(current_path_, L"%s", root_path_);  //파일 경로 

	list_item_height_ = 50;

	/* 초기에는 파일매니저가 보이지 않도록 설정 
	x 위치는 메인 창의 오른쪽 끝에서부터 파일 매니저의 왼쪽 끝까지의 수평 거리
	y 위치는 메인 창의 위쪽 끝부터 파일 매니저의 위쪽 끝까지의 수직 거리.
	너비는 x 위치에서 시작하여 왼쪽으로, 높이는 y위치에서 시작하여 아래로 향하는 크기
	 */
	width_ = 0;
	x_ = Window::GetInstance()->GetWindowArea().right - width_;
	y_ = Window::GetInstance()->GetControl()->GetHeight();
	height_ = (Window::GetInstance()->GetWindowArea().bottom - y_) - Window::GetInstance()->GetTimeline()->GetHeight();

	/*button_minimize_ = make_unique<Button>([=]
		{
			Active();
		}); */


}
void FileManager::Draw(HDC hdc)
{
	if (is_active_) // 활성 상태 확인 
	{
		Graphics graphics(hdc); // 그리기 작업 수행

		//이미지 로드
		Image file_icon(L"Resources/FileIcon.png");
		Image gb_file_icon(L"Resources/ImageFileIcon.png");
		Image folder_icon(L"Resources/FolderIcon.png");
		Image opened_folder_icon(L"Resources/OpenedFolderIcon.png");
		Image trash_icon(L"Resources/TrashIcon.png");

		//브러시와 펜 생성 
		SolidBrush white_brush(Color(255, 255, 255, 255));
		SolidBrush black_brush(Color(255, 0, 0, 0));
		SolidBrush background_brush(Color(255, 219, 219, 219));
		SolidBrush background_brush2(Color(255, 238, 238, 238));
		SolidBrush scroll_bar_brush(Color(255, 230, 230, 230));
		SolidBrush scroll_bar_thumb_brush(Color(255, 192, 192, 192));

		Pen black_pen(Color(255, 0, 0, 0));
		Pen contour_pen(Color(255, 185, 185, 185));

		// 텍스트의 수직 정렬을 중앙으로 설정
		StringFormat string_format_line_center;
		string_format_line_center.SetLineAlignment(StringAlignmentCenter);

		

		// 파일 매니저 위치와 크기 설정
		x_ = Window::GetInstance()->GetWindowArea().right - width_;
		y_ = Window::GetInstance()->GetControl()->GetHeight();
		height_ = (Window::GetInstance()->GetWindowArea().bottom - y_) - Window::GetInstance()->GetTimeline()->GetHeight();

		//파일매니저 전체 영역 설정
		file_manager_area_ = { x_, y_, x_ + width_, y_ + height_ };

		//파일매니저의 배경
		graphics.FillRectangle(&background_brush, x_, y_, width_, height_);

		//메뉴 아이콘을 로드하고 최소화 버튼에 설정한 후 그림
		Image menu_icon(L"Resources/MenuIcon.png");
		button_minimize_->SetBackground(false);
		button_minimize_->SetContour(false);
		button_minimize_->SetImage(&menu_icon, 20, 20);
		button_minimize_->Draw(hdc, L"", x_ + width_ - 30, y_, 30, 30);

		// 상단바. 창의 가장 위쪽에 위치. 제목과 최소화 버튼이 존재. FILE MANAGER라는 텍스트와 폴더 아이콘이 함께 표시
		Point background_points[] = {
			Point(x_, y_),
			Point(x_, y_ + 60),
			Point(x_ + width_, y_ + 60),
			Point(x_ + width_, y_ + 30),
			Point(x_ + 120, y_ + 30),
			Point(x_ + 120, y_) };

		Point contour_points[] = {
			Point(x_, y_),
			Point(x_, y_ + 59),
			Point(x_ + width_ - 1, y_ + 59),
			Point(x_ + width_ - 1, y_ + 30),
			Point(x_ + 120, y_ + 30),
			Point(x_ + 120, y_) };
		
		graphics.FillPolygon(&background_brush2, background_points, 6); 
		graphics.DrawPolygon(&contour_pen, contour_points, 6); 

		graphics.DrawImage(&folder_icon, x_ + 10, y_ + 5, 20, 20); // 상단바에 폴더 아이콘 

		// 상단바에 파일매니저 텍스트
		PointF header_font_position(x_ + 35, y_ + 15);
		graphics.DrawString(L"File Manager", -1, /*&font_style */  header_font_position, &string_format_line_center, &black_brush);

		graphics.FillRectangle(&background_brush2, x_, y_ + 60, width_, height_ - 90);// 나머지 배경을 그림 

		// 리스트 박스 즉 파일목록 영역. 현재 선택된 폴더의 하위 파일 및 폴더가 이 영역에 나열됨. 각 항목은 폴더 아이콘 또는 파일 아이콘과 함께 파일명과 크기를 표시
		// 사용자가 이 영역을 클릭하거나 스크롤 가능. 
		list_box_x_ = x_;
		list_box_y_ = y_ + 60;
		list_box_width_ = width_ - 20;
		list_box_height_ = height_ - 90;

		list_box_area_ = { list_box_x_, list_box_y_, list_box_x_ + list_box_width_, list_box_y_ + list_box_height_ };

		WCHAR path_word[1024] = L"";

		// 리스트 박스가 스크롤 필요가 없는 경우 스크롤 상태를 초기화 
		/*if ((items_.size() * list_item_height_) < list_box_height_ && scroll_bar_thumb_percent_ != 0)
		{
			list_item_select_ = 0;
			scroll_bar_thumb_percent_ = 0;
		}
		*/
		
		Region region(Rect(list_box_x_, list_box_y_, list_box_width_, list_box_height_)); //리스트 박스에 클리핑 설정

		// 클리핑 마스크 시작
		graphics.SetClip(&region, CombineModeReplace);

		FileRefresh(current_path_); // 현재 디렉토리의 파일 목록을 새로고침

		/*
		if (!items_.empty()) // 파일 목록이 비어 있지 않다면, 선택된 파일 항목의 배경을 강조 표시하고 해당 경로를 표시
		{
			graphics.FillRectangle(&background_brush, list_box_x_, list_box_y_ - (((items_.size() * list_item_height_) - list_box_height_) * scroll_bar_thumb_percent_) + (list_item_select_ * list_item_height_), list_box_width_, list_item_height_);

			wsprintf(path_word, L"%s/%s", fs::path(items_[list_item_select_].file_path).parent_path().filename().c_str(), items_[list_item_select_].file_name.c_str());
		}*/
		
		// 파일 이름과 크기를 저장할 문자열을 선언 
		WCHAR file_name_word[1024];
		WCHAR file_size_word[1024];

		// 오른쪽 정렬 및 중앙 정렬을 위한 객체를 생성 
		StringFormat string_format_right_line_center;
		string_format_right_line_center.SetAlignment(StringAlignmentFar);
		string_format_right_line_center.SetLineAlignment(StringAlignmentCenter);

		// 파일 목록을 순회하며 각각의파일이나 폴더를 그림
		for (size_t i = 0; i < items_.size(); i++)
		{
			PointF file_name_font_position(list_box_x_ + 35, list_box_y_ - (((items_.size() * list_item_height_) - list_box_height_) * scroll_bar_thumb_percent_) + (list_item_height_ / 2) + (i * list_item_height_));
			PointF file_size_font_position(list_box_x_ + list_box_width_ - 5, list_box_y_ - (((items_.size() * list_item_height_) - list_box_height_) * scroll_bar_thumb_percent_) + (list_item_height_ / 2) + (i * list_item_height_));
			wsprintf(file_name_word, L"%s", items_[i].file_name.c_str());
			wsprintf(file_size_word, L"%s", ConvertBytes(items_[i].file_size));

			//항목이 디렉토리면, 빈 폴더인지 여부에 따라 폴더 아이콘을 다르게 그림.
			if (fs::is_directory(items_[i].file_path))
			{
				if (fs::is_empty(items_[i].file_path))
				{
					graphics.DrawImage(&opened_folder_icon, list_box_x_ + 2.5, list_box_y_ - (((items_.size() * list_item_height_) - list_box_height_) * scroll_bar_thumb_percent_) + 10 + (i * list_item_height_), 30, 30);
				}
				else
				{
					graphics.DrawImage(&folder_icon, list_box_x_ + 2.5, list_box_y_ - (((items_.size() * list_item_height_) - list_box_height_) * scroll_bar_thumb_percent_) + 10 + (i * list_item_height_), 30, 30);
				}
			}
			// 항목이 파일이면, 파일 확장자에 따라 다른 아이콘을 표시.
			else if (fs::is_regular_file(items_[i].file_path))
			{
				if (fs::path(items_[i].file_path).extension() == ".gb" || fs::path(items_[i].file_path).extension() == ".GB")
				{
					graphics.DrawImage(&gb_file_icon, list_box_x_, list_box_y_ - (((items_.size() * list_item_height_) - list_box_height_) * scroll_bar_thumb_percent_) + 10 + (i * list_item_height_), 30, 30);
				}
				else if (fs::path(items_[i].file_path).extension() == ".bmp" || fs::path(items_[i].file_path).extension() == ".BMP" ||
					fs::path(items_[i].file_path).extension() == ".jpg" || fs::path(items_[i].file_path).extension() == ".JPG" ||
					fs::path(items_[i].file_path).extension() == ".jpeg" || fs::path(items_[i].file_path).extension() == ".JPEG" ||
					fs::path(items_[i].file_path).extension() == ".png" || fs::path(items_[i].file_path).extension() == ".PNG" ||
					fs::path(items_[i].file_path).extension() == ".gif" || fs::path(items_[i].file_path).extension() == ".GIF")
				{
					Image image_file_icon(items_[i].file_path.c_str());
					graphics.DrawImage(&image_file_icon, list_box_x_, list_box_y_ - (((items_.size() * list_item_height_) - list_box_height_) * scroll_bar_thumb_percent_) + 10 + (i * list_item_height_), 30, 30);
				}
				else
				{
					graphics.DrawImage(&file_icon, list_box_x_, list_box_y_ - (((items_.size() * list_item_height_) - list_box_height_) * scroll_bar_thumb_percent_) + 10 + (i * list_item_height_), 30, 30);
				}
				//파일 크기를 텍스트로 그림,font_style 적용하면 인수문제 해결
				graphics.DrawString(file_size_word, -1, /*&font_style*/ file_size_font_position, &string_format_right_line_center, &black_brush);
			}
			//파일 이름을 텍스트로 그림, font_style 적용하면 인수문제 해결
			graphics.DrawString(file_name_word, -1, /*&font_style*/  file_name_font_position, &string_format_line_center, &black_brush);
		}

		// 클리핑 마스크 종료
		graphics.ResetClip();

		/*
		// 스크롤바. 목록의 길이가 화면에 표시되는 영역보다 클 경우 스크롤바가 활성화. 
		scroll_bar_width_ = 20;
		scroll_bar_height_ = list_box_height_;
		scroll_bar_x_ = list_box_x_ + list_box_width_;
		scroll_bar_y_ = list_box_y_;

		scroll_bar_thumb_ratio_ = (double)scroll_bar_height_ / (items_.size() * list_item_height_);

		scroll_bar_area_ = { scroll_bar_x_, scroll_bar_y_, scroll_bar_x_ + scroll_bar_width_, scroll_bar_y_ + scroll_bar_height_ };

		graphics.FillRectangle(&scroll_bar_brush, scroll_bar_x_, scroll_bar_y_, scroll_bar_width_, scroll_bar_height_);

		// Scroll Bar Thumb
		if ((items_.size() * list_item_height_) > list_box_height_)
		{
			scroll_bar_thumb_height_ = scroll_bar_height_ * scroll_bar_thumb_ratio_;
			graphics.FillRectangle(&scroll_bar_thumb_brush, scroll_bar_x_ + 5, scroll_bar_y_ + 5 + (scroll_bar_thumb_percent_ / 1.0f) * (scroll_bar_height_ - scroll_bar_thumb_height_), scroll_bar_width_ - 10, round(scroll_bar_thumb_height_) - 10);
		}
		*/
		graphics.DrawRectangle(&contour_pen, x_, y_, width_ - 1, height_ - 1);// 파일매니저 전체의 테투리 

		// 하단바. 현재 탐색 중인 경로를 표시 
		graphics.FillRectangle(&background_brush2, x_, y_ + height_ - 30, width_, 30);
		graphics.DrawRectangle(&contour_pen, x_, y_ + height_ - 30, width_ - 1, 29);

		Region bottom_region(Rect(x_ + 1, y_ + height_ - 31, width_ - 1, 28));

		// 하단바 영역 클리핑 마스크 시작
		graphics.SetClip(&bottom_region, CombineModeReplace);

		//하단바에 경로 텍스트
		PointF path_font_position(x_ + 5, y_ + height_ - 15);
		graphics.DrawString(path_word, -1, /*&font_style*/ path_font_position, &string_format_line_center, &black_brush);

		// 클리핑 마스크 종료
		graphics.ResetClip();
	}
}

void FileManager::FileRefresh(fs::path path) // 현재 경로에 있는 파일과 폴더 목록을 새로고침하여 업데이트. 폴더 안의 파일들을 읽어와서 item_라는 벡터에 저장하고,
// 이를 ui에 표시
{
	fs::path p(path);

	// 해당 경로가 존재하는지 확인
	if (fs::exists(p) && fs::is_directory(p))
	{
		fs::directory_iterator iter(p);

		items_.clear();

		// 현재 폴더가 Geusts가 아닐 경우 상위 폴더와 최상위 폴더 표시
		if (p != root_path_)
		{
			items_.push_back({ ".", root_path_, NULL });
			items_.push_back({ "..", p.parent_path(), NULL });
		}

		while (iter != fs::end(iter))
		{
			const fs::directory_entry& entry = *iter;
			items_.push_back({ entry.path().filename(), entry.path(), entry.file_size() });
			++iter;
		}
	}
	else
	{
		if (!fs::exists(root_path_) && !fs::is_directory(root_path_))
		{
			fs::create_directory(root_path_);
		}

		FileRefresh(root_path_);
	}
}

void FileManager::Active() // 파일 관리자 창을 활성화하거나 비활성화. 활성화 된 상태에서는 창의 너비가 300픽셀로 설정, 비활성화면 너비가 0이 되어 창이 화면에서 사라짐.
{
	is_active_ = !is_active_;
	width_ = is_active_ ? 300 : 0;
	InvalidateRect(hWnd, NULL, FALSE);
}



LPCWSTR FileManager::GetRootPath()
{
	return root_path_;
}

LPCWSTR FileManager::GetCurrentPath()
{
	return current_path_;
}

int FileManager::GetWidth()
{
	return width_;
}

int FileManager::GetHeight()
{
	return height_;
}