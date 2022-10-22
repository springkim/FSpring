// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "FSpring.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

void GetVideoInfo(std::string video_file, int64_t* pframe_count = nullptr, int64_t* pduration_msec = nullptr, double* pfps = nullptr) {
	bool ret = true;
	av_register_all();
	AVFormatContext* pFormatCtx = nullptr;
	avformat_open_input(&pFormatCtx, video_file.c_str(), NULL, NULL);
	avformat_find_stream_info(pFormatCtx, NULL);
	int videoStream = -1;	//Find video Stream
	for (unsigned int i = 0; i < pFormatCtx->nb_streams; i++) {
		if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
			videoStream = i;
			break;
		}
	}
	int64_t frame_count = pFormatCtx->streams[videoStream]->nb_frames;

	if (frame_count <= 0 || frame_count > INT_MAX) {
		//frame_count is incorrect
		frame_count = 0;
		AVPacket packet;
		while (av_read_frame(pFormatCtx, &packet) >= 0) {
			if (packet.stream_index == videoStream) {
				frame_count++;
			}
			av_packet_unref(&packet);
		}
	}
	int64_t duration_msec = static_cast<int64_t>(pFormatCtx->duration*1000.0 / AV_TIME_BASE);
	double fps = frame_count * 1000.0 / duration_msec;
	if (frame_count) {
		*pframe_count = frame_count;
	}
	if (pduration_msec) {
		*pduration_msec = duration_msec;
	}
	if (pfps) {
		*pfps = fps;
	}
	avformat_close_input(&pFormatCtx);
	avformat_free_context(pFormatCtx);
}
IMPLEMENT_DYNAMIC(CMainFrame, MSpringFrame)

BEGIN_MESSAGE_MAP(CMainFrame, MSpringFrame)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_FILE_ADDVIDEO, &CMainFrame::OnFileOpenvideo)
	ON_COMMAND(ID_EXTRACT_SAVEASJPG, &CMainFrame::OnExtractAsJPG)
	ON_WM_GETMINMAXINFO()
	ON_COMMAND(ID_EXTRACT_SAVEASPNG, &CMainFrame::OnExtractAsPng)
	ON_COMMAND(ID_FILE_CLEARVIDEO, &CMainFrame::OnFileClear)
	ON_WM_DESTROY()
	ON_COMMAND(ID_FILE_ADDVIDEOFOLDER, &CMainFrame::OnFileAddvideofolder)
	ON_WM_LBUTTONDOWN()
	ON_WM_NCLBUTTONDOWN()
	ON_COMMAND(ID_FILE_EXIT32781, &CMainFrame::OnFileExit32781)
	ON_COMMAND(ID_FILE_CLEARIMAGES, &CMainFrame::OnFileClearimages)
	ON_COMMAND(ID_FILE_ADDIMAGES, &CMainFrame::OnFileAddimages)
	ON_COMMAND(ID_MAKE_SAVEASAVI, &CMainFrame::OnMakeSaveasavi)
END_MESSAGE_MAP()

// CMainFrame 생성/소멸

CMainFrame::CMainFrame() {
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame() {
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (MSpringFrame::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 프레임의 클라이언트 영역을 차지하는 뷰를 만듭니다.
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
						  CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL)) {
		TRACE0("뷰 창을 만들지 못했습니다.\n");
		return -1;
	}

	this->AddSysBtn(IDB_FRAME_CLOSE, MSpringFrame::ButtonEvent_Close);
	this->AddSysBtn(IDB_FRAME_MAXIMIZE, MSpringFrame::ButtonEvent_MaximizeWindow);
	this->AddSysBtn(IDB_FRAME_MINIMIZE, MSpringFrame::ButtonEvent_MinimizeWindow);
	this->SetIcon(IDR_MAINFRAME);

	VersionCheck(this);
	this->SetTitle(mspring::String::ToCString("FSpring "+ __version__));
	this->SetStyle(g_font_str, g_color_bk, g_color_text, g_color_border);

	m_menu_frame_video = new MSpringMenuFrame(this);
	m_menu_frame_video->SetStyle(g_font_str, g_color_bk, g_color_text, g_color_hover, g_color_bk);
	m_menu_frame_video->SetMenu(IDR_MAINFRAME);
	m_menu_frame_video->SetPosition(0);

	m_menu_frame_image = new MSpringMenuFrame(this);
	m_menu_frame_image->SetStyle(g_font_str, g_color_bk, g_color_text, g_color_hover, g_color_bk);
	m_menu_frame_image->SetMenu(IDR_MENU1);
	m_menu_frame_image->SetPosition(0);
	this->AddExpansionClass(m_menu_frame_video);

	m_tab_frame = new MSpringTabFrame(this);
	m_tab_frame->SetStyle(g_font_str, g_color_thumb, g_color_deactivate, g_color_bk, g_color_text);
	m_tab_frame->SetPosition(1);
	m_tab_frame->AddTab(TEXT("Frame Extract"));
	m_tab_frame->AddTab(TEXT("Video Maker"));
	this->AddExpansionClass(m_tab_frame);
	//m_progress_frame = new ProgressExpansion(this);
	//this->AddExpansionClass(m_progress_frame);

	m_wndView.SetStyle(g_color_bk);
	m_video_view = new VideoListView(&m_wndView);
	m_image_view = new ImageListView(&m_wndView);
	m_wndView.m_view = m_video_view;
	this->SetWindowPos(nullptr, 100, 0, 1200, 600, SWP_NOMOVE);
#ifdef _DEBUG
	if (::AllocConsole() == TRUE) {
		FILE* nfp[3];
		freopen_s(nfp + 0, "CONOUT$", "rb", stdin);
		freopen_s(nfp + 1, "CONOUT$", "wb", stdout);
		freopen_s(nfp + 2, "CONOUT$", "wb", stderr);
		std::ios::sync_with_stdio();
	}
#endif
	g_status = Status::INIT;
	g_status2 = Status::INIT;
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs) {
	if (!MSpringFrame::PreCreateWindow(cs))
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const {
	MSpringFrame::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const {
	MSpringFrame::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/) {
	// 뷰 창으로 포커스를 이동합니다.
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) {
	// 뷰에서 첫째 크랙이 해당 명령에 나타나도록 합니다.
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// 그렇지 않으면 기본 처리합니다.
	return MSpringFrame::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}



void CMainFrame::OnFileOpenvideo() {
	if (g_status == Status::RUN) {
		this->MessageBox(TEXT("Extractor is already running"), TEXT("warning"), MB_OK);
		return;
	}
	std::vector<CString>& files = g_files;
	const TCHAR* IMAGE_FILTER = TEXT("Video File(*.avi;*.mp4;*.mkv;*.mov)|*.avi;*.mp4;*.mkv;*.mov|AVI File(*.avi)|*.avi|MP4 File(*.mp4)|*.mp4|MKV File(*.mkv)|*.mkv|MOV File(*.mov)|*.mov|");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT, IMAGE_FILTER);
	CString strFileList;
	const unsigned int c_cbBuffSize = (1000 * (MAX_PATH + 1)) + 1;
	dlg.GetOFN().lpstrFile = strFileList.GetBuffer(c_cbBuffSize);
	dlg.GetOFN().nMaxFile = c_cbBuffSize;
	if (IDOK == dlg.DoModal()) {
		std::set<CString> overlab_set;
		//make set
		for (auto&e : files) {
			overlab_set.insert(e);
		}
		for (POSITION pos = dlg.GetStartPosition(); pos != NULL;) {
			CString path = dlg.GetNextPathName(pos);
			if (overlab_set.find(path) == overlab_set.end()) {
				files.push_back(path);
			}
		}
		if (files.size() > 0) {
			g_status = Status::WAIT;
		}
		this->Invalidate();
	}
}
void CMainFrame::OnFileAddvideofolder() {
	if (g_status == Status::RUN) {
		this->MessageBox(TEXT("Extractor is already running"), TEXT("warning"), MB_OK);
		return;
	}
	std::vector<CString>& files = g_files;

	CFolderPickerDialog fpd(nullptr, OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST);
	if (IDOK == fpd.DoModal()) {
		std::set<CString> overlab_set;
		for (auto&e : files) {
			overlab_set.insert(e);
		}
		for (POSITION pos = fpd.GetStartPosition(); pos != NULL;) {
			CString path = fpd.GetNextPathName(pos);
			std::string cpath = mspring::String::ToString(path);
			std::vector<std::string> cfiles = ispring::File::FileList(cpath, "*.avi;*.mp4;*.mkv;*.mov",true);
			for (auto& cfile : cfiles) {
				if (overlab_set.find(mspring::String::ToCString(cfile)) == overlab_set.end()) {
					files.push_back(mspring::String::ToCString(cfile));
				}
			}
		}
		if (files.size() > 0) {
			g_status = Status::WAIT;
		}
		this->Invalidate();
	}
}
void CMainFrame::OnFileClear() {
	if (g_status == Status::RUN) {
		this->MessageBox(TEXT("Extractor is already running"), TEXT("warning"), MB_OK);
		return;
	}
	std::vector<CString>& files = g_files;
	files.clear();
	g_status = Status::INIT;
	this->Invalidate();
}

UINT Run(LPVOID param) {
	int myfps = 60;
	FrameExtractor dlg(nullptr, myfps);
	if (dlg.DoModal() != IDOK) {
		return 0;
	}
	g_progress.store(0);
	g_files_progress.assign(g_files.size(), std::pair<int, int>(0, 0));
	g_status = Status::RUN;
	void** params = (void**)param;
	CMainFrame* wnd = dynamic_cast<CMainFrame*>((CWnd*)params[0]);
	std::string extension = (char*)params[1];
	
	

	
	//Prepare
	g_total_frame_count = 0;
	g_prepare.store(false);
	std::vector<CString>& files = g_files;
	std::vector<double> fps_vec;
	wnd->m_wndView.Invalidate();
	for (int i = 0; i < files.size(); i++) {
		CString cfile = files[i];
		int64_t frame_count;
		std::string file = mspring::String::ToString(cfile);
		double _fps = 0;
		GetVideoInfo(file, &frame_count,nullptr,&_fps);
		fps_vec.push_back(_fps);
		g_files_progress[i].second = frame_count;
		g_total_frame_count += frame_count;
	}
	g_prepare.store(true);
	//Run
	wnd->m_wndView.Invalidate();
	for (int i = 0; i < files.size(); i++) {
		CString cfile = files[i];
		std::string file = mspring::String::ToString(cfile);
		std::string folder = file.substr(0, file.find_last_of(".")) + ".frames";
		ispring::File::DirectoryErase(folder);
	}
#pragma omp parallel for schedule(dynamic) shared(extension)
	for (int i = 0; i < files.size(); i++) {
		try {
			double fps = fps_vec[i];
			std::string file = mspring::String::ToString(files[i]);
			std::string folder = file.substr(0, file.find_last_of(".")) + ".frames";
			ispring::File::DirectoryErase(folder);
			do {
				//_mkdir(folder.c_str());
				CreateDirectoryA(folder.c_str(), 0);
			} while (ispring::File::DirectoryExist(folder)==false);
			cv::VideoCapture vc(file);
			std::string::size_type slash = file.find_last_of("/\\") + 1;
			std::string::size_type dot = file.find_last_of(".");
			std::string pure = file.substr(slash, dot - slash);
			vc.set(CV_CAP_PROP_FPS, 1);
			if (vc.isOpened() == false) {
				continue;
			}
			cv::Mat frame;
			int n = 0;
			std::ostringstream oss;
			double fps_ratio = fps / myfps;
			double fcnt = 0.0;
			while (vc.read(frame) == true) {
				fcnt += 1.0;
				if (myfps<fps && fcnt < fps_ratio*n) {
					continue;
				}
				oss.str("");
				oss.width(8);
				oss.fill('0');
				oss << n;
				n++;
				std::string dst = folder + "\\" + pure+"_"+oss.str() + "." + extension;
				cv::imwrite(dst, frame);	//imwrite has unknown error in windows7

				g_files_progress[i].first=fcnt;
				g_progress = fcnt;
				wnd->m_wndView.Invalidate();
				//wnd->SendMessage(WM_NCPAINT);

			}
		} catch (cv::Exception& e) {
			::MessageBoxA(NULL, e.what(), "exception", MB_OK);
		} catch (std::exception& e) {
			::MessageBoxA(NULL, e.what(), "exception", MB_OK);
		} catch (...) {
			::MessageBoxA(NULL, "unknown exception", "exception", MB_OK);
		}
	}
	g_status = Status::WAIT;
	wnd->m_wndView.Invalidate();
	wnd->MessageBox(TEXT("Complete!!"), TEXT("message"), MB_OK);
	return 0;
}
void CMainFrame::OnExtractAsJPG() {
	if (g_status == Status::INIT) {
		this->MessageBox(TEXT("No videos"), TEXT("warning"), MB_OK);
		return;
	}
	if (g_status == Status::RUN) {
		this->MessageBox(TEXT("Extractor is already running"), TEXT("warning"), MB_OK);
		return;
	}
	static char extension[256];
	strcpy_s(extension, 256, "jpg");
	static void* params[2];
	params[0] = this;
	params[1] = extension;

	::AfxBeginThread(Run, (void*)params);

}
void CMainFrame::OnExtractAsPng() {
	if (g_status == Status::RUN) {
		this->MessageBox(TEXT("Extractor is already running"), TEXT("warning"), MB_OK);
		return;
	}
	if (g_status == Status::INIT) {
		this->MessageBox(TEXT("No videos"), TEXT("warning"), MB_OK);
		return;
	}
	static char extension[256];
	strcpy_s(extension, 256, "png");
	static void* params[2];
	params[0] = this;
	params[1] = extension;

	::AfxBeginThread(Run, (void*)params);

}

void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI) {
	lpMMI->ptMinTrackSize.x = 600;
	lpMMI->ptMinTrackSize.y = 300;
	MSpringFrame::OnGetMinMaxInfo(lpMMI);
}


void CMainFrame::OnDestroy() {
	MSpringFrame::OnDestroy();
#ifdef _DEBUG
	FreeConsole();
#endif
}





void CMainFrame::OnLButtonDown(UINT nFlags, CPoint point) {
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	MSpringFrame::OnLButtonDown(nFlags, point);
}


void CMainFrame::OnNcLButtonDown(UINT nHitTest, CPoint point) {
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	MSpringFrame::OnNcLButtonDown(nHitTest, point);
	printf("%d\n", m_tab_frame->GetCurrentTab());
	if (m_tab_frame->GetCurrentTab() == 0) {
		this->m_expansion[0] = m_menu_frame_video;
		this->m_wndView.m_view = m_video_view;
		m_image_view->ViewRelease();
	} else {
		this->m_expansion[0] = m_menu_frame_image;
		this->m_wndView.m_view = m_image_view;
		m_video_view->ViewRelease();
	}
	this->m_wndView.Invalidate();
	MSpringFrame::OnNcPaint();
}


void CMainFrame::OnFileExit32781() {
	this->CloseWindow();
}


void CMainFrame::OnFileClearimages() {
	if (g_status2 == Status::RUN) {
		this->MessageBox(TEXT("Extractor is already running"), TEXT("warning"), MB_OK);
		return;
	}
	std::vector<CString>& files = g_image_files;
	files.clear();
	g_status2 = Status::INIT;
	g_succ = 0;
	this->Invalidate();
}


void CMainFrame::OnFileAddimages() {
	if (g_status2 == Status::RUN) {
		this->MessageBox(TEXT("Extractor is already running"), TEXT("warning"), MB_OK);
		return;
	}
	std::vector<CString>& files = g_image_files;
	const TCHAR* IMAGE_FILTER = TEXT("Image File(*bmp;*.jpg;*.jpeg;*.png)|*.bmp;*.jpg;*.jpeg;*.png|BMP File(*.bmp)|*.bmp|JPG File(*.jpg)|*.jpg|JPEG File(*.jpeg)|*.jpeg|PNG File(*.png)|*.png|");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT, IMAGE_FILTER);
	CString strFileList;
	const unsigned int c_cbBuffSize = (1000 * (MAX_PATH + 1)) + 1;
	dlg.GetOFN().lpstrFile = strFileList.GetBuffer(c_cbBuffSize);
	dlg.GetOFN().nMaxFile = c_cbBuffSize;
	if (IDOK == dlg.DoModal()) {
		std::set<CString> overlab_set;
		//make set
		for (auto&e : files) {
			overlab_set.insert(e);
		}
		for (POSITION pos = dlg.GetStartPosition(); pos != NULL;) {
			CString path = dlg.GetNextPathName(pos);
			if (overlab_set.find(path) == overlab_set.end()) {
				files.push_back(path);
			}
		}
		if (files.size() > 0) {
			g_status2 = Status::WAIT;
		}
		g_succ = 0;
		this->Invalidate();
	}
}

UINT MakeVideo(LPVOID param) {
	g_status2 = Status::RUN;
	void** params = (void**)param;
	CMainFrame* wnd = dynamic_cast<CMainFrame*>((CWnd*)params[0]);
	std::string save = (char*)params[1];
	int fps = *(int*)params[2];
	g_succ = 0;

	cv::Mat img = cv::imread(mspring::String::ToString(g_image_files[0]));
	{
		cv::VideoWriter vw;
		vw.open(save, cv::VideoWriter::fourcc('X', 'V', 'I', 'D'), fps, img.size(), true);
		for (int i = 0; i < g_image_files.size(); i++) {
			img = cv::imread(mspring::String::ToString(g_image_files[i]));
			g_succ++;
			vw << img;
			wnd->m_wndView.Invalidate();
		}
	}
	g_status2 = Status::WAIT;
	wnd->MessageBox(TEXT("Complete!!"), TEXT("message"), MB_OK);
	return 1;
}
void CMainFrame::OnMakeSaveasavi() {
	if (g_status2 == Status::INIT) {
		this->MessageBox(TEXT("No images"), TEXT("warning"), MB_OK);
		return;
	}
	if (g_status2 == Status::RUN) {
		this->MessageBox(TEXT("Extractor is already running"), TEXT("warning"), MB_OK);
		return;
	}
	if (g_image_files.size() <= 1) {
		this->MessageBox(TEXT("Number of images are too small"), TEXT("warning"), MB_OK);
		return;
	}
	static CString path;
	static CString file = TEXT("output");
	static int fps = 15;
	static char _save[MAX_PATH];
	CVideoWriter dlg(nullptr, path, file, fps);
	if (dlg.DoModal() == IDOK) {
		std::string path2 = mspring::String::ToString(path);
		std::string file2 = mspring::String::ToString(file);
		if (path2.back() != '\\' && path2.back() != '/') {
			path2.push_back('/');
		}
		std::string save = path2 + file2 + ".avi";
		
		strcpy_s(_save, MAX_PATH, save.c_str());
		static void* params[3];
		params[0] = this;
		params[1] = _save;
		params[2] = &fps;
		::AfxBeginThread(MakeVideo, (void*)params);
	}
}
