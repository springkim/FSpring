/*
*  version.hpp
*  FSpring
*
*  Created by kimbom on 2017. 9. 28...
*  Copyright 2017 kimbom. All rights reserved.
*
*/
#if !defined(FSPRING_7E1_9_1C_VERSION_HPP_INCLUDED)
#define FSPRING_7E1_9_1C_VERSION_HPP_INCLUDED

#include<iostream>
#include<vector>
#include<string>
#include<Windows.h>
#include<WinInet.h>
#include<afxwin.h>
#pragma comment(lib, "wininet.lib")
inline std::string GetHtml(std::string url) {
	std::string html;
	HINTERNET hInternet = InternetOpen(TEXT("HTTP"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);	//인터넷 관련 DLL을 초기화한다.
	if (hInternet) {
		HINTERNET hUrl = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);	//url에 걸린 파일을 연다.
		if (hUrl) {
			DWORD realSize = 0;
			DWORD possibleSize = 0;
			DWORD recv = 0;
			char* buffer = new char[2000000];
			char* tempBuffer = new char[2000000];
			memset(buffer, 0, 2000000 * sizeof(char));
			memset(tempBuffer, 0, 2000000 * sizeof(char));
			do {
				InternetQueryDataAvailable(hUrl, &possibleSize, 0, 0);
				InternetReadFile(hUrl, buffer, possibleSize, &realSize);
				if (possibleSize>0) {
					memcpy(tempBuffer + recv, buffer, possibleSize * sizeof(char));
				}
				recv += possibleSize;
			} while (realSize != 0);
			html.resize(realSize);
			html = tempBuffer;
			delete[] buffer;
			delete[] tempBuffer;
		}
	}
	return html;
}
inline std::string GetNewVersion() {
	std::string url_git = "https://github.com/springkim/FSpring/blob/master/FSpring(windows)/FSpring/version.txt";
	std::string html = GetHtml(url_git);
	std::string tag_begin = "&lt;fspring-version&gt;";
	std::string tag_end = "&lt;/fspring-version&gt;";
	size_t pos_beg = html.find(tag_begin) + tag_begin.length();
	size_t pos_end = html.find(tag_end);
	if (pos_beg == std::string::npos || pos_end == std::string::npos) {
		return "";
	}
	std::string version = html.substr(pos_beg, pos_end - pos_beg);
	if (version.length() == 0) {
		version = "";
	}
	return version;
}
#if 1
const std::string __version__ = "1.3";
inline UINT _VersionCheck(LPVOID param) {
	CWnd* wnd = (CWnd*)param;
	std::string new_version = GetNewVersion();
	if (new_version.length() == 0) {
		return 1;
	}
	if (atof(__version__.c_str()) < atof(new_version.c_str())) {
		if (IDYES == wnd->MessageBox(TEXT("new version is available\nPlease visit https://github.com/springkim/FSpring \n"), TEXT("news"), MB_YESNO)) {
			ShellExecute(NULL, TEXT("open"), TEXT("https://github.com/springkim/FSpring"), TEXT(""), TEXT(""), SW_SHOW);
			wnd->CloseWindow();
		}
	}
	return 1;
}
inline void VersionCheck(CWnd* wnd) {
	::AfxBeginThread(_VersionCheck,(LPVOID)wnd);
}
#endif
#endif  //FSPRING_7E1_9_1C_VERSION_HPP_INCLUDED