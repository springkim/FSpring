#pragma once
#include "MSpring/View.h"
#include"MSpring/MSpring.h"
#include"repository.h"
#include"resource.h"

class ImageListView :
	public VirtualView {
public:
	ImageListView(CWnd* wnd);
	virtual ~ImageListView();

	void OnPaint(CDC* pDC)override;
	void OnLButtonDown(UINT nFlags, CPoint point)override;
	void OnLButtonUp(UINT nFlags, CPoint point)override;
	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) override;
	void OnMouseMove(UINT nFlags, CPoint point)override;
	void OnMouseLeave()override;
	void OnTimer(UINT_PTR nIDEvent)override;
	void ViewRelease() {
		m_is_drag = false;
	}
private:
	const UINT_PTR TIMER_SCROLLMOVE = 0x65C187C7;
protected:
	//Drag
	bool m_is_drag = false;
	CPoint m_drag_point;
	float m_prev_scroll_pos;

	int m_scrollbar_width = 20;
	int HEIGHT = 20;

	float m_scroll_pos = 0.0F;	//0~1;

	int GetViewHeight() {
		CRect rect;
		m_parent->GetClientRect(&rect);
		return rect.Height();
	}
	int GetPageHeight() {
		std::vector<CString>& files = g_image_files;
		return static_cast<int>(files.size()*HEIGHT);
	}
	int GetThumbHeight() {
		int view_height = GetViewHeight();
		int page_height = GetPageHeight();
		return static_cast<int>(((float)view_height / page_height)*view_height);
	}
	void DrawScroll(CDC* pDC) {

		CRect rect;
		m_parent->GetClientRect(&rect);
		int view_height = GetViewHeight();
		int page_height = GetPageHeight();
		int thumb_height = GetThumbHeight();
		if (page_height > view_height) {
			CRect thumb_rect = rect;
			thumb_rect.top += static_cast<decltype(thumb_rect.top)>((view_height - thumb_height)*m_scroll_pos);

			thumb_rect.left = thumb_rect.right - m_scrollbar_width;
			thumb_rect.bottom = thumb_rect.top + thumb_height;



			CBrush brush;
			brush.CreateSolidBrush(g_color_thumb);
			CPen pen;
			pen.CreatePen(PS_NULL, 1, RGB(0, 0, 0));
			CPen* old_pen = pDC->SelectObject(&pen);
			CBrush* old_brush = pDC->SelectObject(&brush);

			pDC->RoundRect(&thumb_rect, CPoint(5, 5));

			pDC->SelectObject(old_pen);
			pDC->SelectObject(old_brush);
			brush.DeleteObject();
			pen.DeleteObject();
		}
	}
	void DrawProgress(CDC* pDC) {
		CRect rect_window = this->GetViewRect();
		CPen null_pen;
		null_pen.CreatePen(PS_NULL, 0, RGB(0, 0, 0));
		pDC->SelectObject(&null_pen);
		//Draw status window
		CBrush bk_brush;
		bk_brush.CreateSolidBrush(RGB(7, 54, 66));
		CBrush* old_brush = pDC->SelectObject(&bk_brush);
		CRect rect;


		rect.left = (rect_window.left + rect_window.right) / 2 - 250;
		rect.right = rect.left + 500;
		rect.top = (rect_window.top + rect_window.bottom) / 2 - 50;
		rect.bottom = rect.top + 100;



		pDC->RoundRect(rect, CPoint(10, 10));
		pDC->SelectObject(old_brush);
		//Draw Progressbar
		CBrush probk_brush;
		probk_brush.CreateSolidBrush(RGB(88, 110, 117));
		old_brush = pDC->SelectObject(&probk_brush);
		CRect progress_rect = rect;
		progress_rect.top += static_cast<LONG>(rect.Height()*0.1);
		progress_rect.bottom = static_cast<LONG>(progress_rect.top + rect.Height()*0.3);
		progress_rect.left = static_cast<LONG>(rect.left + rect.Width()*0.05);
		progress_rect.right = static_cast<LONG>(rect.right - rect.Width()*0.05);
		pDC->RoundRect(progress_rect, CPoint(10, 10));
		pDC->SelectObject(old_brush);
		//Draw progress thumb
		CBrush proth_brush;
		proth_brush.CreateSolidBrush(RGB(38, 139, 210));
		old_brush = pDC->SelectObject(&proth_brush);
		CRect thumb_rect = progress_rect;
		if (g_succ != 0) {
			thumb_rect.right = static_cast<LONG>(thumb_rect.left + progress_rect.Width()*((double)g_succ / g_image_files.size()));
		} else {
			thumb_rect.right = thumb_rect.left;
		}
		pDC->RoundRect(thumb_rect, CPoint(10, 10));
		pDC->SelectObject(old_brush);
		//Draw status text
		CRect status_rect = rect;
		status_rect.bottom -= static_cast<LONG>(rect.Height()*0.1);
		status_rect.top = static_cast<LONG>(status_rect.bottom - rect.Height()*0.3);
		status_rect.left = static_cast<LONG>(rect.left + rect.Width()*0.05);
		status_rect.right = static_cast<LONG>(rect.right - rect.Width()*0.05);
		CString str;
		str.Format(TEXT("completed frames ... (%lld/%lld)"), g_succ, g_image_files.size());
		int font_height = mspring::Font::GetRealFontHeight(g_font_str, status_rect.Height(), pDC, str, true);

		int font_height2 = mspring::Font::GetRealFontHeight(g_font_str, status_rect.Width(), pDC, str, false);
		font_height = mspring::Min(font_height, font_height2);
		CFont font;
		font.CreatePointFont(font_height, g_font_str);
		CFont* old_font = pDC->SelectObject(&font);
		pDC->SetTextColor(RGB(253, 246, 227));
		pDC->SetBkMode(TRANSPARENT);

		pDC->TextOutW(status_rect.left, status_rect.top, str);
		//dbb.getDC().RoundRect(status_rect, CPoint(10, 10));
		pDC->SelectObject(old_brush);


		pDC->SelectObject(old_font);
		font.DeleteObject();
		bk_brush.DeleteObject();
	}
};

