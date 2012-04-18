// RibbonMDISample.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "RibbonMDISample.h"

#include "base/command_line.h"
#include "base/logging.h"
#include "base/at_exit.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "RibbonMDISampleDoc.h"
#include "RibbonMDISampleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRibbonMDISampleApp

BEGIN_MESSAGE_MAP(CRibbonMDISampleApp, CWinApp)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRibbonMDISampleApp construction

CRibbonMDISampleApp::CRibbonMDISampleApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CRibbonMDISampleApp object

CRibbonMDISampleApp theApp;

base::AtExitManager g_atExitMamager;

/////////////////////////////////////////////////////////////////////////////
// CRibbonMDISampleApp initialization

BOOL CRibbonMDISampleApp::InitInstance()
{
	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();
	CXTPWinDwmWrapper().SetProcessDPIAware();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.


#if _MSC_VER <= 1200 //MFC 6.0 or earlier
#ifdef _AFXDLL
	Enable3dControls();         // Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();   // Call this when linking to MFC statically
#endif
#endif


	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Codejock Software Sample Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_RIBBONTYPE,
		RUNTIME_CLASS(CRibbonMDISampleDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CRibbonMDISampleView));
	pDocTemplate->SetContainerInfo(IDR_RIBBONTYPE_CNTR_IP);
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

  using namespace logging;

  CommandLine::Init(0, NULL);
  InitLogging(L"xtp.log", 
    LOG_TO_BOTH_FILE_AND_SYSTEM_DEBUG_LOG,
    LOCK_LOG_FILE,
    DELETE_OLD_LOG_FILE,
    DISABLE_DCHECK_FOR_NON_OFFICIAL_RELEASE_BUILDS);

  VLOG(1) << "log init";

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CRibbonMDISampleApp message handlers

