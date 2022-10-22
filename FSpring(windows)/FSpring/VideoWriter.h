#pragma once
#include "afxwin.h"

#include"MSpring/DrawHelper.h"
#include"MSpring/String.h"
class XButton : public CButton {
protected:
	DECLARE_DYNAMIC(XButton)
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) {
		CDC dc;
		dc.Attach(lpDrawItemStruct->hDC);     //Get device context object
		CRect rt;
		rt = lpDrawItemStruct->rcItem;        //Get button rect

		dc.FillSolidRect(rt, RGB(238, 232, 213)); //Fill button with blue color

		UINT state = lpDrawItemStruct->itemState; //Get state of the button
		if ((state & ODS_SELECTED))            // If it is pressed
		{
			dc.DrawEdge(rt, EDGE_SUNKEN, BF_RECT);    // Draw a sunken face
		} else {
			dc.DrawEdge(rt, EDGE_RAISED, BF_RECT);    // Draw a raised face
		}

		dc.SetTextColor(RGB(0,0,0));
		// Set the color of the caption to be yellow
		CString strTemp;
		GetWindowText(strTemp);
		// Get the caption which have been set
		dc.DrawText(strTemp, rt, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		// Draw out the caption
		if ((state & ODS_FOCUS))       // If the button is focused
		{
			// Draw a focus rect which indicates the user 
			// that the button is focused
			int iChange = 3;
			rt.top += iChange;
			rt.left += iChange;
			rt.right -= iChange;
			rt.bottom -= iChange;
			dc.DrawFocusRect(rt);
		}
		dc.Detach();
	}
};
class CVideoWriter : public CDialogEx
{
	DECLARE_DYNAMIC(CVideoWriter)

public:
	CVideoWriter(CWnd* pParent ,CString& _path,CString& _file,int& _fps);   // 표준 생성자입니다.
	virtual ~CVideoWriter();
	CString& path;
	CString& file;
	int& fps;
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_MAKE_VIDEO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	COLORREF m_color_edit = RGB(238, 232, 213);
	DECLARE_MESSAGE_MAP()
public:

	CFont m_font_for_path;
	CFont m_font_for_fps;
	CFont m_font_for_filename;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CEdit m_path;
	XButton m_load_folder;
	CStatic m_static_fps;
	CEdit m_fps;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	CEdit m_filename;
	CStatic m_static_filename;
	CStatic m_static_path;
	afx_msg void OnBnClickedOpenFile();
	XButton m_ok;
	XButton m_cancel;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
};
