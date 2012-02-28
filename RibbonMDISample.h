// RibbonMDISample.h : main header file for the RIBBONMDISAMPLE application
//

#if !defined(AFX_RIBBONMDISAMPLE_H__20064B00_7EA6_449F_A9E1_EA9D707C34CD__INCLUDED_)
#define AFX_RIBBONMDISAMPLE_H__20064B00_7EA6_449F_A9E1_EA9D707C34CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CRibbonMDISampleApp:
// See RibbonMDISample.cpp for the implementation of this class
//

class CRibbonMDISampleApp : public CWinApp
{
public:
	CRibbonMDISampleApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRibbonMDISampleApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RIBBONMDISAMPLE_H__20064B00_7EA6_449F_A9E1_EA9D707C34CD__INCLUDED_)
