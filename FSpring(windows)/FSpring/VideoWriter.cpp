// VideoWriter.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FSpring.h"
#include "VideoWriter.h"
#include "afxdialogex.h"
#include"repository.h"

// CVideoWriter 대화 상자입니다.
IMPLEMENT_DYNAMIC(XButton, CButton)


BEGIN_MESSAGE_MAP(XButton,CButton)
ON_WM_DRAWITEM()
END_MESSAGE_MAP()


IMPLEMENT_DYNAMIC(CVideoWriter, CDialogEx)

CVideoWriter::CVideoWriter(CWnd* pParent, CString& _path, CString& _file, int& _fps)
	: CDialogEx(IDD_DLG_MAKE_VIDEO, pParent),path(_path),file(_file),fps(_fps)
{

}

CVideoWriter::~CVideoWriter()
{
}

void CVideoWriter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PATH, m_path);
	DDX_Control(pDX, IDC_OPEN_FILE, m_load_folder);
	DDX_Control(pDX, IDC_STATIC_FPS, m_static_fps);
	DDX_Control(pDX, IDC_EDIT_FPS, m_fps);
	DDX_Control(pDX, IDC_EDIT_FILENAME, m_filename);
	DDX_Control(pDX, IDC_STATIC_FILENAME, m_static_filename);
	DDX_Control(pDX, IDC_STATIC_PATH, m_static_path);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
}


BEGIN_MESSAGE_MAP(CVideoWriter, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_OPEN_FILE, &CVideoWriter::OnBnClickedOpenFile)
	ON_BN_CLICKED(IDCANCEL, &CVideoWriter::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CVideoWriter::OnBnClickedOk)
END_MESSAGE_MAP()


// CVideoWriter 메시지 처리기입니다.


BOOL CVideoWriter::OnEraseBkgnd(CDC* pDC) {
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, g_color_bk);
	return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}


HBRUSH CVideoWriter::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
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


BOOL CVideoWriter::OnInitDialog() {
	CDialogEx::OnInitDialog();
	m_path.ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_DRAWFRAME | SWP_FRAMECHANGED);
	CRect rect;
	CDC* DC = this->GetDC();
	m_path.GetWindowRect(&rect);
	int h = mspring::Font::GetRealFontHeight(TEXT("Arial"), rect.Height(), DC);
	m_font_for_path.CreatePointFont(h, TEXT("Arial"));
	m_path.SetFont(&m_font_for_path);

	m_fps.ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_DRAWFRAME | SWP_FRAMECHANGED);
	m_path.GetWindowRect(&rect);
	h = mspring::Font::GetRealFontHeight(TEXT("Arial"), rect.Height(), DC);
	m_font_for_fps.CreatePointFont(h, TEXT("Arial"));
	m_fps.SetFont(&m_font_for_fps);

	m_filename.ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_DRAWFRAME | SWP_FRAMECHANGED);
	m_path.GetWindowRect(&rect);
	h = mspring::Font::GetRealFontHeight(TEXT("Arial"), rect.Height(), DC);
	m_font_for_filename.CreatePointFont(h, TEXT("Arial"));
	m_filename.SetFont(&m_font_for_filename);

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
	
	m_static_filename.SetFont(&m_font_for_filename);
	m_static_filename.GetWindowText(str_fps);
	DC->SelectObject(&m_font_for_filename);
	GetTextExtentPoint32(DC->GetSafeHdc(), str_fps, str_fps.GetLength(), &sz);
	m_static_filename.GetWindowRect(rect);
	ScreenToClient(rect);
	rect.right = rect.left + sz.cx;
	rect.bottom = rect.top + sz.cy;
	m_static_filename.MoveWindow(rect);

	m_static_path.SetFont(&m_font_for_path);
	m_static_path.GetWindowText(str_fps);
	DC->SelectObject(&m_font_for_path);
	GetTextExtentPoint32(DC->GetSafeHdc(), str_fps, str_fps.GetLength(), &sz);
	m_static_path.GetWindowRect(rect);
	ScreenToClient(rect);
	rect.right = rect.left + sz.cx;
	rect.bottom = rect.top + sz.cy;
	m_static_path.MoveWindow(rect);
	//
	m_path.SetReadOnly(TRUE);
	m_fps.ModifyStyle(0, ES_NUMBER);
	m_fps.SetLimitText(3);
	m_load_folder.ModifyStyle(1, BS_OWNERDRAW);
	m_ok.ModifyStyle(1, BS_OWNERDRAW);
	m_cancel.ModifyStyle(1, BS_OWNERDRAW);


	m_path.SetWindowText(path);
	m_filename.SetWindowText(file);
	CString tmp;
	tmp.Format(TEXT("%d"), fps);
	m_fps.SetWindowText(tmp);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CVideoWriter::OnBnClickedOpenFile() {
	CDC* DC = this->GetDC();
	CFolderPickerDialog fpd(nullptr, OFN_FILEMUSTEXIST);
	if (IDOK == fpd.DoModal()) {
		CString path=fpd.GetFolderPath();
		m_path.SetWindowTextW(path);
	}
	ReleaseDC(DC);
}


void CVideoWriter::OnBnClickedCancel() {
	m_path.GetWindowText(path);
	m_filename.GetWindowText(file);
	if (file.GetLength() == 0) {
		file = TEXT("output");
	}
	CString tmp;
	m_fps.GetWindowText(tmp);
	if (tmp.GetLength() == 0) {
		fps = 15;
	} else {
		fps = atoi(mspring::String::ToString(tmp).c_str());
	}
	CDialogEx::OnCancel();
}


void CVideoWriter::OnBnClickedOk() {
	m_path.GetWindowText(path);
	m_filename.GetWindowText(file);
	if (file.GetLength() == 0) {
		file = TEXT("output");
	}
	CString tmp;
	m_fps.GetWindowText(tmp);
	if (tmp.GetLength() == 0) {
		fps = 15;
	} else {
		fps = atoi(mspring::String::ToString(tmp).c_str());
	}
	CDialogEx::OnOK();
}
