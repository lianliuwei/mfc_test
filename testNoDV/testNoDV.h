// testNoDV.h : main header file for the testNoDV application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CtestNoDVApp:
// See testNoDV.cpp for the implementation of this class
//

class CtestNoDVApp : public CWinApp
{
public:
	CtestNoDVApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CtestNoDVApp theApp;