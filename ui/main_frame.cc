#include "stdafx.h"

#include "ui/main_frame.h"

#include "resources/Resource.h"

namespace {
static const XTPPaintTheme kTheme = xtpThemeVisualStudio2010;
static const XTPDockingPanePaintTheme kPaneTheme = xtpPaneThemeVisualStudio2010;
static const int frame_height = 500;
static const int frame_width = 640;
}

IMPLEMENT_DYNAMIC(MainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(MainFrame, CFrameWnd)
    ON_WM_CREATE()
    ON_WM_SETFOCUS()
    ON_WM_CLOSE()
END_MESSAGE_MAP()

MainFrame::MainFrame()
    : analog_disturbance_view_(NULL)
    , device_(NULL) {
}


MainFrame::~MainFrame() {}


int MainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    if (!tab_ctrl_.Create(AFX_WS_DEFAULT_VIEW,
      CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST)) {
        TRACE0("Failed to create main frame tab ctrl\n");
        return -1;
    }

    analog_disturbance_view_ = new AnalogDisturbanceView(&device_);
    // create a view to occupy the client area of the frame
    if (!analog_disturbance_view_->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
        CRect(0, 0, 0, 0), &tab_ctrl_, AFX_IDW_PANE_FIRST, NULL)) {
        TRACE0("Failed to create analog disturbance view tab page\n");
        return -1;
    }

    tab_ctrl_.AddControl(_T("Analog disturbance"), analog_disturbance_view_);

    if (CreateCommandBars()) {
        TRACE0("Failed to create command bars\n");
        return -1;
    }

    if (CreatePanes()) {
        TRACE0("Failed to create panes\n");
        return -1;
    }
    // config UI
    CXTPCommandBars* pCommandBars = GetCommandBars();
    // Set Theme
    CXTPPaintManager::SetTheme(kTheme);
    m_paneManager.SetTheme(kPaneTheme);
    // Set "Always Show Full Menus" option to the FALSE
    pCommandBars->GetCommandBarsOptions()->bAlwaysShowFullMenus = FALSE;

    return 0;
}

int MainFrame::CreateCommandBars()
{
     UINT indicators[] = {
        ID_SEPARATOR,           // status line indicator
        ID_INDICATOR_CAPS,
        ID_INDICATOR_NUM,
        ID_INDICATOR_SCRL,
    };

    if (!m_wndStatusBar.Create(this) ||
        !m_wndStatusBar.SetIndicators(indicators,
        sizeof(indicators)/sizeof(UINT)))
    {
        TRACE0("Failed to create status bar\n");
        return -1;      // fail to create
    }

    // Initialize the command bars
    if (!InitCommandBars())
        return -1;

    // Get a pointer to the command bars object.
    CXTPCommandBars* pCommandBars = GetCommandBars();
    if(pCommandBars == NULL)
    {
        TRACE0("Failed to create command bars object.\n");
        return -1;      // fail to create
    }

    // Add the menu bar
    CXTPCommandBar* pMenuBar = pCommandBars->SetMenu(
        _T("Menu Bar"), IDR_MAINFRAME_STRESS);
    if(pMenuBar == NULL)
    {
        TRACE0("Failed to create menu bar.\n");
        return -1;      // fail to create
    }

    // Create ToolBar
    CXTPToolBar* pToolBar = (CXTPToolBar*)
        pCommandBars->Add(_T("Standard"), xtpBarTop);
    if (!pToolBar || !pToolBar->LoadToolBar(IDR_MAINFRAME_STRESS))
    {
        TRACE0("Failed to create toolbar\n");
        return -1;
    }

    pCommandBars->GetShortcutManager()->SetAccelerators(IDR_MAINFRAME_STRESS);

    return 0;
}

int MainFrame::CreatePanes()
{
    // Initialize the docking pane manager and set the
    // initial them for the docking panes.  Do this only after all
    // control bars objects have been created and docked.
    m_paneManager.InstallDockingPanes(this);

    return 0;
}

void MainFrame::LoadLayout()
{
    // Load the previous state for toolbars and menus.
    LoadCommandBars(_T("CommandBars"));

    // Load the previous state for docking panes.
    CXTPDockingPaneLayout layoutNormal(&m_paneManager);
    if (layoutNormal.Load(_T("NormalLayout")))
    {
        m_paneManager.SetLayout(&layoutNormal);
    }
}

void MainFrame::SaveLayout()
{
    // Save the current state for toolbars and menus.
    SaveCommandBars(_T("CommandBars"));

    // Save the current state for docking panes.
    CXTPDockingPaneLayout layoutNormal(&m_paneManager);
    m_paneManager.GetLayout(&layoutNormal);
    layoutNormal.Save(_T("NormalLayout"));
}

BOOL MainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    if( !CFrameWnd::PreCreateWindow(cs) )
        return FALSE;

    cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
         | WS_MINIMIZEBOX | WS_SYSMENU;

    cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
    cs.lpszClass = AfxRegisterWndClass(0);
    cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
    cs.cx = frame_width;
    cs.cy = frame_height;
    return TRUE;
}

void MainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
    // forward focus to the view window
    tab_ctrl_.SetFocus();
}

void MainFrame::OnClose()
{
    CFrameWnd::OnClose();
}