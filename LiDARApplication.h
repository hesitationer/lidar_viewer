// LiDARApplication.h : main header file for the LiDARApplication application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "LiDARApplication.h"
#include "resource.h"       // main symbols


// CLiDARApplicationApp:
// See LiDARApplication.cpp for the implementation of this class
//

class CLiDARApplicationApp : public CWinApp
{
public:
	CLiDARApplicationApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnFileOpenLiDAR();
	afx_msg void OnSetupTiling();

public:
	CString m_strFileName;
};

extern CLiDARApplicationApp theApp;