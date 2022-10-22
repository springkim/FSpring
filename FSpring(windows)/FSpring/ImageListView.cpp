#include "stdafx.h"
#include "ImageListView.h"
#include"MSpring/MSpring.h"

ImageListView::ImageListView(CWnd* wnd) :VirtualView(wnd) {
	
}


ImageListView::~ImageListView() {
}
void ImageListView::OnPaint(CDC* pDC) {
	CRect rect = this->GetViewRect();
	if (g_status2 == Status::INIT) {
		CBrush brush_bk;
		brush_bk.CreateSolidBrush(g_color_bk);
		pDC->FillRect(&rect, &brush_bk);
		brush_bk.DeleteObject();
		CDC memDC;
		BITMAP bmpinfo;
		memDC.CreateCompatibleDC(pDC);
		CBitmap bmp;
		CBitmap* oldbmp = nullptr;
		bmp.LoadBitmapW(IDB_IMAGE);
		bmp.GetBitmap(&bmpinfo);
		oldbmp = memDC.SelectObject(&bmp);
		int newWidth = 0;
		int newHeight = 0;
		if (bmpinfo.bmWidth != rect.Width() || bmpinfo.bmHeight != rect.Height()) {
			newWidth = bmpinfo.bmWidth*(rect.Height() / (double)bmpinfo.bmHeight);
			newHeight = rect.Height();
			if (newWidth > rect.Width()) {
				newWidth = rect.Width();
				newHeight = bmpinfo.bmHeight*(rect.Width() / (double)bmpinfo.bmWidth);
			}
		}
		pDC->StretchBlt((rect.Width() - newWidth) / 2, (rect.Height() - newHeight) / 2, newWidth, newHeight, &memDC, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, SRCCOPY);
	} else {
		std::vector<CString>& files = g_image_files;

		CBrush brush_bk;
		brush_bk.CreateSolidBrush(g_color_bk);
		pDC->FillRect(&rect, &brush_bk);
		brush_bk.DeleteObject();
		CFont font;
		int h = mspring::Font::GetRealFontHeight(g_font_str, HEIGHT, pDC);
		font.CreatePointFont(h, g_font_str);
		CFont* old_font = pDC->SelectObject(&font);

		int view_height = GetViewHeight();
		int page_height = GetPageHeight();

		int page_jump = 0;	//띄워야 하는 좌표
		if (page_height > view_height) {
			page_jump = static_cast<int>((page_height - view_height)*m_scroll_pos);
		}
		int page_y = 0;		//페이지가 시작될 좌표.
		int page_idx = 0;	//시작될 줄 넘버
		if (page_jump > 0) {
			div_t div = std::div(page_jump, HEIGHT);
			page_y = -div.rem;
			page_idx = div.quot;
		}
		CPen pen_null;
		pen_null.CreatePen(PS_NULL, 0, RGB(0, 0, 0));

		pDC->SetBkMode(TRANSPARENT);
		while (page_idx < (int)files.size() && page_y < view_height) {
			CRect rect_is_tag = {
				0
				,page_y
				,HEIGHT
				,page_y + HEIGHT
			};
			CBrush brush_check;
			CPen pen_check;
			int pen_check_weight = HEIGHT / 10;
			pen_check.CreatePen(PS_SOLID, pen_check_weight, g_color_bk);
			if (page_idx<g_succ) {
				brush_check.CreateSolidBrush(g_color_thumb);
			} else {
				brush_check.CreateSolidBrush(RGB(88, 110, 117));
			}
			CBrush* old_brush = pDC->SelectObject(&brush_check);
			CPen* old_pen = pDC->SelectObject(&pen_null);
			pDC->RoundRect(&rect_is_tag, CPoint(5, 5));
			pDC->SelectObject(old_pen);
			old_pen = pDC->SelectObject(&pen_check);
			int divw = HEIGHT / 5;
			pDC->MoveTo(rect_is_tag.left + divw, static_cast<int>(rect_is_tag.top + divw*2.5));
			pDC->LineTo(static_cast<int>(rect_is_tag.left + divw*1.5), rect_is_tag.top + divw * 4);
			pDC->LineTo(rect_is_tag.right - divw, rect_is_tag.top + divw * 1);
			pDC->SelectObject(old_pen);
			pDC->SelectObject(old_brush);
			brush_check.DeleteObject();
			pen_check.DeleteObject();
			//draw file path
			
			pDC->TextOutW(rect_is_tag.right, page_y, files[page_idx]);
			//pDC->TextOutW(0, page_y, files[page_idx]);
			
			page_idx++;
			page_y += HEIGHT;
		}
		DrawScroll(pDC);
		if (g_status2 == Status::RUN) {
			this->DrawProgress(pDC);
		}
		pen_null.DeleteObject();

		font.DeleteObject();
	}
}
void ImageListView::OnLButtonDown(UINT nFlags, CPoint point) {
	m_is_drag = true;
	m_prev_scroll_pos = m_scroll_pos;
	m_drag_point = point;
}
void ImageListView::OnLButtonUp(UINT nFlags, CPoint point) {
	if (m_is_drag == true) {
		m_parent->KillTimer(TIMER_SCROLLMOVE);
		m_is_drag = false;
	}
}
BOOL ImageListView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
	if ((GetKeyState(VK_CONTROL) & 0x8000) == 0x8000) {
		//Ctrl키를 누르고 휠을 돌리면 확대 옵션.
		HEIGHT += (zDelta > 0) ? 1 : -1;
		mspring::SetRange(HEIGHT, 10, 50);
	} else if ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000) {
		m_scroll_pos += 0.01F*((zDelta > 0) ? -1 : 1);
		mspring::SetRange(m_scroll_pos, 0.0F, 1.0F);

	} else {
		int view_height = GetViewHeight();
		int thumb_height = GetThumbHeight();
		int page_height = GetPageHeight();
		int page_jump = 0;	//띄워야 하는 좌표
		if (page_height > view_height) {
			page_jump = static_cast<int>((page_height - view_height)*m_scroll_pos);
		}
		int page_idx = 0;	//시작될 줄 넘버
		if (page_jump > 0) {
			page_idx = static_cast<int>(std::round((float)page_jump / HEIGHT));
		}
		page_idx += ((zDelta > 0) ? -1 : 1);
		m_scroll_pos = ((float)page_idx*HEIGHT) / (page_height - view_height);
		mspring::SetRange(m_scroll_pos, 0.0F, 1.0F);
	}
	return TRUE;
}
void ImageListView::OnMouseMove(UINT nFlags, CPoint point) {
	if (m_is_drag == true) {
		int view_height = GetViewHeight();
		int page_height = GetPageHeight();
		int thumb_height = GetThumbHeight();
		int drag_height = point.y - m_drag_point.y;
		m_scroll_pos = m_prev_scroll_pos + ((float)drag_height / (view_height - thumb_height));
		mspring::SetRange(m_scroll_pos, 0.0F, 1.0F);
	}

}
void ImageListView::OnMouseLeave() {
	if (m_is_drag == true) {
		m_parent->SetTimer(TIMER_SCROLLMOVE, 10, nullptr);
	}

}
void ImageListView::OnTimer(UINT_PTR nIDEvent) {
	if (nIDEvent == TIMER_SCROLLMOVE) {
		CPoint point = this->GetMousePoint();
		CRect rect = this->GetViewRect();
		//두 조건은 합쳐질수없음
		if (rect.PtInRect(point) == TRUE) {
			m_parent->KillTimer(TIMER_SCROLLMOVE);
			return;
		}
		if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) == false) {
			m_is_drag = false;
			m_parent->KillTimer(TIMER_SCROLLMOVE);
			return;
		}
		int view_height = GetViewHeight();
		int page_height = GetPageHeight();
		int thumb_height = GetThumbHeight();
		int drag_height = point.y - m_drag_point.y;
		m_scroll_pos = m_prev_scroll_pos + ((float)drag_height / (view_height - thumb_height));
		m_scroll_pos = mspring::Max(m_scroll_pos, 0.0F);
		m_scroll_pos = mspring::Min(m_scroll_pos, 1.0F);
	}
}