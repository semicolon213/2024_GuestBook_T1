#include "pch.h"
#include "FileManager.h"
#include "Window.h"



using namespace std;
using namespace Gdiplus;


FileManager::FileManager(HWND hWnd)
{
	this->hWnd = hWnd;
	wsprintf(root_path_, L"%s\\Guests", fs::current_path().c_str());
	wsprintf(current_path_, L"%s", root_path_);  //���� ��� 

	list_item_height_ = 50;

	/* �ʱ⿡�� ���ϸŴ����� ������ �ʵ��� ���� 
	x ��ġ�� ���� â�� ������ ���������� ���� �Ŵ����� ���� �������� ���� �Ÿ�
	y ��ġ�� ���� â�� ���� ������ ���� �Ŵ����� ���� �������� ���� �Ÿ�.
	�ʺ�� x ��ġ���� �����Ͽ� ��������, ���̴� y��ġ���� �����Ͽ� �Ʒ��� ���ϴ� ũ��
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
	if (is_active_) // Ȱ�� ���� Ȯ�� 
	{
		Graphics graphics(hdc); // �׸��� �۾� ����

		//�̹��� �ε�
		Image file_icon(L"Resources/FileIcon.png");
		Image gb_file_icon(L"Resources/ImageFileIcon.png");
		Image folder_icon(L"Resources/FolderIcon.png");
		Image opened_folder_icon(L"Resources/OpenedFolderIcon.png");
		Image trash_icon(L"Resources/TrashIcon.png");

		//�귯�ÿ� �� ���� 
		SolidBrush white_brush(Color(255, 255, 255, 255));
		SolidBrush black_brush(Color(255, 0, 0, 0));
		SolidBrush background_brush(Color(255, 219, 219, 219));
		SolidBrush background_brush2(Color(255, 238, 238, 238));
		SolidBrush scroll_bar_brush(Color(255, 230, 230, 230));
		SolidBrush scroll_bar_thumb_brush(Color(255, 192, 192, 192));

		Pen black_pen(Color(255, 0, 0, 0));
		Pen contour_pen(Color(255, 185, 185, 185));

		// �ؽ�Ʈ�� ���� ������ �߾����� ����
		StringFormat string_format_line_center;
		string_format_line_center.SetLineAlignment(StringAlignmentCenter);

		

		// ���� �Ŵ��� ��ġ�� ũ�� ����
		x_ = Window::GetInstance()->GetWindowArea().right - width_;
		y_ = Window::GetInstance()->GetControl()->GetHeight();
		height_ = (Window::GetInstance()->GetWindowArea().bottom - y_) - Window::GetInstance()->GetTimeline()->GetHeight();

		//���ϸŴ��� ��ü ���� ����
		file_manager_area_ = { x_, y_, x_ + width_, y_ + height_ };

		//���ϸŴ����� ���
		graphics.FillRectangle(&background_brush, x_, y_, width_, height_);

		//�޴� �������� �ε��ϰ� �ּ�ȭ ��ư�� ������ �� �׸�
		Image menu_icon(L"Resources/MenuIcon.png");
		button_minimize_->SetBackground(false);
		button_minimize_->SetContour(false);
		button_minimize_->SetImage(&menu_icon, 20, 20);
		button_minimize_->Draw(hdc, L"", x_ + width_ - 30, y_, 30, 30);

		// ��ܹ�. â�� ���� ���ʿ� ��ġ. ����� �ּ�ȭ ��ư�� ����. FILE MANAGER��� �ؽ�Ʈ�� ���� �������� �Բ� ǥ��
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

		graphics.DrawImage(&folder_icon, x_ + 10, y_ + 5, 20, 20); // ��ܹٿ� ���� ������ 

		// ��ܹٿ� ���ϸŴ��� �ؽ�Ʈ
		PointF header_font_position(x_ + 35, y_ + 15);
		graphics.DrawString(L"File Manager", -1, /*&font_style */  header_font_position, &string_format_line_center, &black_brush);

		graphics.FillRectangle(&background_brush2, x_, y_ + 60, width_, height_ - 90);// ������ ����� �׸� 

		// ����Ʈ �ڽ� �� ���ϸ�� ����. ���� ���õ� ������ ���� ���� �� ������ �� ������ ������. �� �׸��� ���� ������ �Ǵ� ���� �����ܰ� �Բ� ���ϸ�� ũ�⸦ ǥ��
		// ����ڰ� �� ������ Ŭ���ϰų� ��ũ�� ����. 
		list_box_x_ = x_;
		list_box_y_ = y_ + 60;
		list_box_width_ = width_ - 20;
		list_box_height_ = height_ - 90;

		list_box_area_ = { list_box_x_, list_box_y_, list_box_x_ + list_box_width_, list_box_y_ + list_box_height_ };

		WCHAR path_word[1024] = L"";

		// ����Ʈ �ڽ��� ��ũ�� �ʿ䰡 ���� ��� ��ũ�� ���¸� �ʱ�ȭ 
		/*if ((items_.size() * list_item_height_) < list_box_height_ && scroll_bar_thumb_percent_ != 0)
		{
			list_item_select_ = 0;
			scroll_bar_thumb_percent_ = 0;
		}
		*/
		
		Region region(Rect(list_box_x_, list_box_y_, list_box_width_, list_box_height_)); //����Ʈ �ڽ��� Ŭ���� ����

		// Ŭ���� ����ũ ����
		graphics.SetClip(&region, CombineModeReplace);

		FileRefresh(current_path_); // ���� ���丮�� ���� ����� ���ΰ�ħ

		/*
		if (!items_.empty()) // ���� ����� ��� ���� �ʴٸ�, ���õ� ���� �׸��� ����� ���� ǥ���ϰ� �ش� ��θ� ǥ��
		{
			graphics.FillRectangle(&background_brush, list_box_x_, list_box_y_ - (((items_.size() * list_item_height_) - list_box_height_) * scroll_bar_thumb_percent_) + (list_item_select_ * list_item_height_), list_box_width_, list_item_height_);

			wsprintf(path_word, L"%s/%s", fs::path(items_[list_item_select_].file_path).parent_path().filename().c_str(), items_[list_item_select_].file_name.c_str());
		}*/
		
		// ���� �̸��� ũ�⸦ ������ ���ڿ��� ���� 
		WCHAR file_name_word[1024];
		WCHAR file_size_word[1024];

		// ������ ���� �� �߾� ������ ���� ��ü�� ���� 
		StringFormat string_format_right_line_center;
		string_format_right_line_center.SetAlignment(StringAlignmentFar);
		string_format_right_line_center.SetLineAlignment(StringAlignmentCenter);

		// ���� ����� ��ȸ�ϸ� �����������̳� ������ �׸�
		for (size_t i = 0; i < items_.size(); i++)
		{
			PointF file_name_font_position(list_box_x_ + 35, list_box_y_ - (((items_.size() * list_item_height_) - list_box_height_) * scroll_bar_thumb_percent_) + (list_item_height_ / 2) + (i * list_item_height_));
			PointF file_size_font_position(list_box_x_ + list_box_width_ - 5, list_box_y_ - (((items_.size() * list_item_height_) - list_box_height_) * scroll_bar_thumb_percent_) + (list_item_height_ / 2) + (i * list_item_height_));
			wsprintf(file_name_word, L"%s", items_[i].file_name.c_str());
			wsprintf(file_size_word, L"%s", ConvertBytes(items_[i].file_size));

			//�׸��� ���丮��, �� �������� ���ο� ���� ���� �������� �ٸ��� �׸�.
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
			// �׸��� �����̸�, ���� Ȯ���ڿ� ���� �ٸ� �������� ǥ��.
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
				//���� ũ�⸦ �ؽ�Ʈ�� �׸�,font_style �����ϸ� �μ����� �ذ�
				graphics.DrawString(file_size_word, -1, /*&font_style*/ file_size_font_position, &string_format_right_line_center, &black_brush);
			}
			//���� �̸��� �ؽ�Ʈ�� �׸�, font_style �����ϸ� �μ����� �ذ�
			graphics.DrawString(file_name_word, -1, /*&font_style*/  file_name_font_position, &string_format_line_center, &black_brush);
		}

		// Ŭ���� ����ũ ����
		graphics.ResetClip();

		/*
		// ��ũ�ѹ�. ����� ���̰� ȭ�鿡 ǥ�õǴ� �������� Ŭ ��� ��ũ�ѹٰ� Ȱ��ȭ. 
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
		graphics.DrawRectangle(&contour_pen, x_, y_, width_ - 1, height_ - 1);// ���ϸŴ��� ��ü�� ������ 

		// �ϴܹ�. ���� Ž�� ���� ��θ� ǥ�� 
		graphics.FillRectangle(&background_brush2, x_, y_ + height_ - 30, width_, 30);
		graphics.DrawRectangle(&contour_pen, x_, y_ + height_ - 30, width_ - 1, 29);

		Region bottom_region(Rect(x_ + 1, y_ + height_ - 31, width_ - 1, 28));

		// �ϴܹ� ���� Ŭ���� ����ũ ����
		graphics.SetClip(&bottom_region, CombineModeReplace);

		//�ϴܹٿ� ��� �ؽ�Ʈ
		PointF path_font_position(x_ + 5, y_ + height_ - 15);
		graphics.DrawString(path_word, -1, /*&font_style*/ path_font_position, &string_format_line_center, &black_brush);

		// Ŭ���� ����ũ ����
		graphics.ResetClip();
	}
}

void FileManager::FileRefresh(fs::path path) // ���� ��ο� �ִ� ���ϰ� ���� ����� ���ΰ�ħ�Ͽ� ������Ʈ. ���� ���� ���ϵ��� �о�ͼ� item_��� ���Ϳ� �����ϰ�,
// �̸� ui�� ǥ��
{
	fs::path p(path);

	// �ش� ��ΰ� �����ϴ��� Ȯ��
	if (fs::exists(p) && fs::is_directory(p))
	{
		fs::directory_iterator iter(p);

		items_.clear();

		// ���� ������ Geusts�� �ƴ� ��� ���� ������ �ֻ��� ���� ǥ��
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

void FileManager::Active() // ���� ������ â�� Ȱ��ȭ�ϰų� ��Ȱ��ȭ. Ȱ��ȭ �� ���¿����� â�� �ʺ� 300�ȼ��� ����, ��Ȱ��ȭ�� �ʺ� 0�� �Ǿ� â�� ȭ�鿡�� �����.
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