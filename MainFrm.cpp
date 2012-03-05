// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"

#include "RibbonMDISample.h"

#include "MainFrm.h"

#include "XTPToggleButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_FULLSCREEN, OnViewFullscreen)
	ON_COMMAND(ID_VIEW_NORMAL, OnViewNormal)
	ON_UPDATE_COMMAND_UI(ID_VIEW_NORMAL, OnUpdateViewNormal)
	ON_COMMAND(ID_WINDOW_ARRANGE, OnWindowArrange)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_SWITCH, OnUpdateWindowSwitch)

	ON_COMMAND(ID_VIEW_WORKSPACE, OnViewWorkspace)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WORKSPACE, OnUpdateViewWorkspace)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(XTP_ID_RIBBONCONTROLTAB, OnUpdateRibbonTab)
  ON_UPDATE_COMMAND_UI(ID_TEST_TOGGLE, OnUpdateToggleTest)

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	
	if (!InitCommandBars())
		return -1;

	CXTPCommandBars* pCommandBars = GetCommandBars();

	pCommandBars->SetTheme(xtpThemeRibbon);
	
	m_wndStatusBar.SetFont(pCommandBars->GetPaintManager()->GetIconFont());
	m_wndStatusBar.SetDrawDisabledText(FALSE);
	m_wndStatusBar.SetCommandBars(pCommandBars);
	m_wndStatusBar.GetStatusBarCtrl().SetMinHeight(22);
	m_wndStatusBar.GetPane(0)->SetMargins(8, 1, 2, 1);

	//////////////////////////////////////////////////////////////////////////	
	// IMPORTANT:
	// Ribbon bitmaps resources by default located in application executable. 
	//
	// You also can specify handle to find ribbon resources:
	//((CXTPOffice2007Theme*)XTPPaintManager())->SetImageHandle(hHandle);

	//HMODULE hModule = LoadLibrary(m_csStylesPath + _T("Office2007Blue.dll"));
	//((CXTPOffice2007Theme*)pCommandBars->GetPaintManager())->SetImageHandle(hModule);

	CXTPToolTipContext* pToolTipContext = GetCommandBars()->GetToolTipContext();
	pToolTipContext->SetStyle(xtpToolTipOffice2007);
	pToolTipContext->ShowTitleAndDescription();
	pToolTipContext->SetMargin(CRect(2, 2, 2, 2));
	pToolTipContext->SetMaxTipWidth(180);
	pToolTipContext->SetFont(pCommandBars->GetPaintManager()->GetIconFont());

	pCommandBars->GetCommandBarsOptions()->ShowKeyboardCues(xtpKeyboardCuesShowWindowsDefault);
	pCommandBars->GetCommandBarsOptions()->bToolBarAccelTips = TRUE;

	pCommandBars->GetShortcutManager()->SetAccelerators(IDR_MAINFRAME);

	LoadIcons();

	if (!CreateRibbonBar())
	{
		TRACE0("Failed to create ribbon\n");
		return -1;
	}

	m_wndClient.Attach(this, FALSE);
	m_wndClient.GetToolTipContext()->SetStyle(xtpToolTipOffice2007);

	m_bShowWorkspace = FALSE;
	m_wndClient.ShowWorkspace(m_bShowWorkspace);


	LoadCommandBars(_T("CommandBars"));


	return 0;
}

void CMainFrame::LoadIcons()
{
	CXTPCommandBars* pCommandBars = GetCommandBars();

	pCommandBars->GetImageManager()->SetIcons(IDR_SMALLICONS);
	pCommandBars->GetImageManager()->SetIcons(IDR_LARGEICONS);

	UINT uiGroupClipboard[] = {ID_EDIT_PASTE, ID_EDIT_CUT, ID_EDIT_COPY, ID_FORMAT_PAINTER};
	pCommandBars->GetImageManager()->SetIcons(ID_GROUP_CLIPBOARD, uiGroupClipboard, _countof(uiGroupClipboard), CSize(16, 16));

	UINT uiGroupFind[] = {ID_EDIT_FIND, ID_EDIT_REPLACE, ID_EDIT_GOTO, ID_EDIT_SELECT};
	pCommandBars->GetImageManager()->SetIcons(ID_GROUP_EDITING, uiGroupFind, _countof(uiGroupFind), CSize(16, 16));

	UINT uCommand = {IDB_GEAR};
	pCommandBars->GetImageManager()->SetIcons(IDB_GEAR, &uCommand, 1, CSize(0, 0), xtpImageNormal);


}


BOOL CMainFrame::CreateRibbonBar()
{
 	CXTPPropExchangeXMLNode px(TRUE, 0, _T("Settings"));
 
 	if (px.LoadFromResource(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_LAYOUT), RT_HTML))
 	{
 		CXTPPropExchangeSection pxCommandBars(px.GetSection(_T("CommandBars")));
 		XTP_COMMANDBARS_PROPEXCHANGE_PARAM param; 
 		param.bSerializeControls = TRUE;
 		GetCommandBars()->DoPropExchange(&pxCommandBars, &param);
 	}
 	else
 	{
 		AfxMessageBox(_T("Can't load Layout"));
 		return FALSE;
 	}
 	CXTPRibbonBar* pRibbonBar = (CXTPRibbonBar*)GetCommandBars()->GetMenuBar();
 	pRibbonBar->EnableCustomization(FALSE);
 
 	pRibbonBar->GetQuickAccessControls()->SetOriginalControls(new CXTPOriginalControls());
 	pRibbonBar->GetQuickAccessControls()->GetOriginalControls()->Add(xtpControlButton, ID_FILE_SAVE);
 	pRibbonBar->GetQuickAccessControls()->GetOriginalControls()->Add(xtpControlButton, ID_EDIT_UNDO);
 	pRibbonBar->GetQuickAccessControls()->GetOriginalControls()->Add(xtpControlButton, ID_FILE_PRINT);


	pRibbonBar->EnableFrameTheme();
	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.lpszClass = _T("XTPMainFrame");
	CXTPDrawHelpers::RegisterWndClass(AfxGetInstanceHandle(), cs.lpszClass, 
		CS_DBLCLKS, AfxGetApp()->LoadIcon(IDR_MAINFRAME));

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnUpdateRibbonTab(CCmdUI* pCmdUI)
{
	CXTPRibbonControlTab* pControl = DYNAMIC_DOWNCAST(CXTPRibbonControlTab, CXTPControl::FromUI(pCmdUI));
	if (!pControl)
		return;

	CXTPRibbonTab* pTab = pControl->FindTab(ID_TAB_EDIT);
	if (!pTab)
		return;

	pTab->SetVisible(MDIGetActive() != NULL);
}

void CMainFrame::OnViewFullscreen() 
{
	// TODO: Add your command handler code here
	
}

void CMainFrame::OnViewNormal() 
{
	// TODO: Add your command handler code here
	
}

void CMainFrame::OnUpdateViewNormal(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(1);	
}

void CMainFrame::OnWindowArrange() 
{
	::SendMessage(m_hWndMDIClient, WM_MDITILE, MDITILE_HORIZONTAL, 0);
}

void CMainFrame::OnUpdateWindowSwitch(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(MDIGetActive() != NULL ? TRUE : FALSE);
	
}

void CMainFrame::OnViewWorkspace()
{
	m_bShowWorkspace ^= 1;
	m_wndClient.ShowWorkspace(m_bShowWorkspace);
}

void CMainFrame::OnUpdateViewWorkspace(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bShowWorkspace);
}

void CMainFrame::OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState)
{
// Toggle CommandBars
	GetCommandBars()->OnSetPreviewMode(bPreview);

// Toggle Tab Client
	m_wndClient.ShowWorkspace(!bPreview);

	CMDIFrameWnd::OnSetPreviewMode(bPreview, pState);
}

void CMainFrame::OnClose() 
{	
/*	CXTPPropExchangeXMLNode px(FALSE, 0, _T("Settings"));

	if (px.OnBeforeExchange()) 
	{
		CXTPPropExchangeSection pxCommandBars(px.GetSection(_T("CommandBars")));
		XTP_COMMANDBARS_PROPEXCHANGE_PARAM param; 
		param.bSerializeControls = TRUE; 
		param.bSaveOriginalControls = FALSE;
		param.bSerializeOptions = TRUE;
		GetCommandBars()->DoPropExchange(&pxCommandBars, &param);

		px.SaveToFile(_T("C:\\save.xml"));
	}*/
	
	CXTPMDIFrameWnd::OnClose();
}

void CMainFrame::OnUpdateToggleTest( CCmdUI* pCmdUI )
{
  pCmdUI->Enable(TRUE);
}
