
// FSpring.h : FSpring ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CFSpringApp:
// �� Ŭ������ ������ ���ؼ��� FSpring.cpp�� �����Ͻʽÿ�.
//

class CFSpringApp : public CWinApp
{
public:
	CFSpringApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CFSpringApp theApp;
