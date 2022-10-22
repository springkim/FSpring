#pragma once

#include "afxwin.h"

#include"MSpring/DrawHelper.h"
#include"MSpring/String.h"
#include"repository.h"
#include"VideoWriter.h"
// FrameExtractor 대화 상자입니다.

class FrameExtractor : public CDialogEx
{
	DECLARE_DYNAMIC(FrameExtractor)

public:
	FrameExtractor(CWnd* pParent,int& _fps);   // 표준 생성자입니다.
	virtual ~FrameExtractor();
	int& fps;
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_MAKE_FRAME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	COLORREF m_color_edit = RGB(238, 232, 213);
	CFont m_font_for_fps;
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
public:
	CStatic m_static_fps;
	CEdit m_fps;
	XButton m_ok;
	XButton m_cancel;
};
