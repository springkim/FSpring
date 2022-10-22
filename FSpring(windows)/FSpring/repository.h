#pragma once
#include"stdafx.h"
#include<atomic>
#include<vector>
__declspec(selectany) CString g_font_str= TEXT("Arial");
__declspec(selectany) COLORREF g_color_bk = RGB(250, 243, 225);
__declspec(selectany) COLORREF g_color_thumb = RGB(38, 139, 210);
__declspec(selectany) COLORREF g_color_deactivate = RGB(145, 192, 218);
__declspec(selectany) COLORREF g_color_hover = RGB(38, 139, 210);
__declspec(selectany) COLORREF g_color_text = RGB(7, 54, 66);
__declspec(selectany) COLORREF g_color_border = RGB(108, 113, 196);
enum Status {
	RUN, WAIT, INIT
};
__declspec(selectany) Status g_status;

///Video View
__declspec(selectany) std::vector<CString> g_files;
__declspec(selectany) std::vector<std::pair<int,int>> g_files_progress;

__declspec(selectany) __int64 g_total_frame_count;

__declspec(selectany) std::atomic_bool g_prepare;
__declspec(selectany) std::atomic_int64_t g_progress;
///Image View
__declspec(selectany) Status g_status2;
__declspec(selectany) std::vector<CString> g_image_files;
__declspec(selectany) std::atomic_int64_t g_succ;