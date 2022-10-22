#ifndef _M_AMD64
#error This code does not support x86
#endif
// MainFrm.h : CMainFrame 클래스의 인터페이스
//

#pragma once

#include<direct.h>
#include<atomic>

#include<opencv2/opencv.hpp>
#include"MSpring/Frame/MSpringFrame.h"
#include"MSpring/Frame/MSpringMenuFrame.h"
#include"MSpring/frame/MSpringTabFrame.h"
#include"MSpring/view/MSpringView.h"
#include"MSpring/view/VirtualView.h"
#include"MSpring/MSpring.h"
#include"ispring/File.h"
#include"MSpring/String.h"
#include"VideoListView.h"
#include"ImageListView.h"
#include"VideoWriter.h"
#include"FrameExtractor.h"
#include"repository.h"
extern "C" {
#include <libavformat/avformat.h>
}
#ifdef _OPENMP
#include<omp.h>
#endif
#include"version.h"
class ProgressExpansion :public MSpringFrameExpansion {
public:
	ProgressExpansion(CWnd* wnd) :MSpringFrameExpansion(wnd) {
	}
	virtual void OnCreate(LPCREATESTRUCT lpCreateStruct) {
	}
	virtual int OnNcPaint(CDC* pDC, CRect rect) {
		//쓴만큼 돌려줘야함.
		int W = 300;
		if (g_status == Status::RUN) {
			rect.right = rect.left + W;
			rect.top += 5;
			rect.bottom -= 5;
			CPen null_pen;
			null_pen.CreatePen(PS_NULL, 0, RGB(0, 0, 0));
			CPen* old_pen = pDC->SelectObject(&null_pen);
			CBrush probk_brush;
			probk_brush.CreateSolidBrush(RGB(88, 110, 117));
			CBrush* old_brush = pDC->SelectObject(&probk_brush);
			pDC->RoundRect(rect, CPoint(7, 7));
			pDC->SelectObject(old_brush);
			CBrush proth_brush;
			proth_brush.CreateSolidBrush(RGB(38, 139, 210));
			old_brush = pDC->SelectObject(&proth_brush);
			CRect thumb_rect = rect;
			if (g_progress != 0) {
				thumb_rect.right = static_cast<LONG>(thumb_rect.left + rect.Width()*((float)g_progress / g_total_frame_count));
			} else {
				thumb_rect.right = thumb_rect.left;
			}
			pDC->RoundRect(thumb_rect, CPoint(7, 7));
			pDC->SelectObject(old_brush);
			CFont font;
			int h = mspring::Font::GetRealFontHeight(g_font_str, rect.Height(), pDC);
			font.CreatePointFont(h, g_font_str);
			CFont* old_font = pDC->SelectObject(&font);
			CString progress_str;
			progress_str.Format(TEXT("extract frames ... (%d/%d)"), g_progress, g_total_frame_count);
			pDC->TextOutW(rect.right, rect.top, progress_str);
			font.DeleteObject();
		}
		return W;
	}
	virtual void OnSize(UINT nType, int cx, int cy) {

	}
	virtual bool OnNcLButtonDown(UINT nHitTest, CPoint point) {
		return false;
	}
	virtual bool OnNcMouseMove(UINT nHitTest, CPoint point) {
		return false;
	}
	virtual bool OnNcLButtonUp(UINT nHitTest, CPoint point) {
		return false;
	}
	virtual bool OnNcLButtonDblClk(UINT nHitTest, CPoint point) {
		return false;
	}
	virtual void OnNcMouseLeave() {
	}
	virtual void OnDestroy() {
	}
};
class CMainFrame : public MSpringFrame {

public:
	CMainFrame();
protected:
	DECLARE_DYNAMIC(CMainFrame)

	// 특성입니다.
public:

	// 작업입니다.
public:

	// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

	// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	//ProgressExpansion* m_progress_frame = nullptr;
	MSpringMenuFrame* m_menu_frame_video = nullptr;
	MSpringMenuFrame* m_menu_frame_image = nullptr;

	MSpringView    m_wndView;
	MSpringTabFrame* m_tab_frame = nullptr;
	// 생성된 메시지 맵 함수
	VideoListView* m_video_view = nullptr;
	ImageListView* m_image_view = nullptr;
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnFileOpenvideo();
	afx_msg void OnExtractAsJPG();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnExtractAsPng();
	afx_msg void OnFileClear();
	afx_msg void OnDestroy();
	afx_msg void OnFileAddvideofolder();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnFileExit32781();
	afx_msg void OnFileClearimages();
	afx_msg void OnFileAddimages();
	afx_msg void OnMakeSaveasavi();
};


