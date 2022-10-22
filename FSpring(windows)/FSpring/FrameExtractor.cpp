// FrameExtractor.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FSpring.h"
#include "FrameExtractor.h"
#include "afxdialogex.h"


// FrameExtractor 대화 상자입니다.

IMPLEMENT_DYNAMIC(FrameExtractor, CDialogEx)

FrameExtractor::FrameExtractor(CWnd* pParent,int& _fps)
	: CDialogEx(IDD_DLG_MAKE_FRAME, pParent),fps(_fps)
{

}

FrameExtractor::~FrameExtractor()
{
}

void FrameExtractor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_FPS, m_static_fps);
	DDX_Control(pDX, IDC_EDIT_FPS, m_fps);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
}


BEGIN_MESSAGE_MAP(FrameExtractor, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDCANCEL, &FrameExtractor::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &FrameExtractor::OnBnClickedOk)
END_MESSAGE_MAP()


// FrameExtractor 메시지 처리기입니다.
BOOL FrameExtractor::OnEraseBkgnd(CDC* pDC) {
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, g_color_bk);
	return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}


HBRUSH FrameExtractor::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	static CBrush brush;
	static bool init = true;
	if (init) {
		init = false;
		brush.CreateSolidBrush(m_color_edit);
	}
	if (nCtlColor == CTLCOLOR_EDIT) {
		pDC->SetBkColor(m_color_edit);
		pDC->SetTextColor(RGB(7, 54, 66));
	}
	if (nCtlColor == CTLCOLOR_STATIC) {
		pDC->SetBkColor(g_color_bk);
		pDC->SetTextColor(RGB(7, 54, 66));
	}
	if (nCtlColor == CTLCOLOR_BTN) {
		pDC->SetBkMode(TRANSPARENT);
		pDC->SelectObject(&brush);
	}
	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}


BOOL FrameExtractor::OnInitDialog() {
	CDialogEx::OnInitDialog();
	
	CRect rect;
	CDC* DC = this->GetDC();
	
	int h = 0;

	m_fps.ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_DRAWFRAME | SWP_FRAMECHANGED);
	
	h = mspring::Font::GetRealFontHeight(TEXT("Arial"), rect.Height(), DC);
	m_font_for_fps.CreatePointFont(h, TEXT("Arial"));
	m_fps.SetFont(&m_font_for_fps);



	// Static 컨트롤의 크기 변경
	CSize sz;
	m_static_fps.SetFont(&m_font_for_fps);
	CString str_fps;
	m_static_fps.GetWindowText(str_fps);
	DC->SelectObject(&m_font_for_fps);
	GetTextExtentPoint32(DC->GetSafeHdc(), str_fps, str_fps.GetLength(), &sz);
	m_static_fps.GetWindowRect(rect);
	ScreenToClient(rect);
	rect.right = rect.left + sz.cx;
	rect.bottom = rect.top + sz.cy;
	m_static_fps.MoveWindow(rect);

	GetTextExtentPoint32(DC->GetSafeHdc(), str_fps, str_fps.GetLength(), &sz);
	
	m_fps.ModifyStyle(0, ES_NUMBER);
	m_fps.SetLimitText(3);
	m_ok.ModifyStyle(1, BS_OWNERDRAW);
	m_cancel.ModifyStyle(1, BS_OWNERDRAW);

	CString tmp;
	tmp.Format(TEXT("%d"), fps);
	m_fps.SetWindowText(tmp);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}




void FrameExtractor::OnBnClickedCancel() {


	CString tmp;
	m_fps.GetWindowText(tmp);
	if (tmp.GetLength() == 0) {
		fps = 60;
	} else {
		fps = atoi(mspring::String::ToString(tmp).c_str());
	}
	CDialogEx::OnCancel();
}


void FrameExtractor::OnBnClickedOk() {
	CString tmp;
	m_fps.GetWindowText(tmp);
	if (tmp.GetLength() == 0) {
		fps = 60;
	} else {
		fps = atoi(mspring::String::ToString(tmp).c_str());
	}
	CDialogEx::OnOK();
}