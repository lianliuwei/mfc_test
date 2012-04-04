#include "stdafx.h"

#include "ui/main_frame.h"

#include "resources/Resource.h"

namespace {

}

IMPLEMENT_DYNAMIC(MainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(MainFrame, CFrameWnd)
    ON_WM_CREATE()
    ON_WM_SETFOCUS()
    ON_WM_CLOSE()
END_MESSAGE_MAP()

MainFrame::MainFrame()
    : downloadlist_(NULL) {
}


MainFrame::~MainFrame() {}


int MainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    downloadlist_ = new DownloadList();
    // create a view to occupy the client area of the frame
    if (!downloadlist_->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
        CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL)) {
        TRACE0("Failed to create download list\n");
        return -1;
    }

    if (CreateCommandBars()) {
        TRACE0("Failed to create command bars\n");
        return -1;
    }

    if (CreatePanes()) {
        TRACE0("Failed to create panes\n");
        return -1;
    }

    CXTPCommandBars* pCommandBars = GetCommandBars();
    // Set Windows XP Theme
    CXTPPaintManager::SetTheme(xtpThemeVisualStudio2010);
    // Set Windows XP Theme
    m_paneManager.SetTheme(xtpPaneThemeVisualStudio2010);
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
        _T("Menu Bar"), IDR_MAINFRAME);
    if(pMenuBar == NULL)
    {
        TRACE0("Failed to create menu bar.\n");
        return -1;      // fail to create
    }

    // Create ToolBar
    CXTPToolBar* pToolBar = (CXTPToolBar*)
        pCommandBars->Add(_T("Standard"), xtpBarTop);
    if (!pToolBar || !pToolBar->LoadToolBar(IDR_MAINFRAME))
    {
        TRACE0("Failed to create toolbar\n");
        return -1;
    }

    pCommandBars->GetShortcutManager()->SetAccelerators(IDR_MAINFRAME);

    return 0;
}

int MainFrame::CreatePanes()
{
    // Initialize the docking pane manager and set the
    // initial them for the docking panes.  Do this only after all
    // control bars objects have been created and docked.
    m_paneManager.InstallDockingPanes(this);

    // Create docking panes.
    CXTPDockingPane* pPane = m_paneManager.CreatePane(
        IDR_PANE_TOTAL_DOWNLOAD_INFO, CRect(0, 0,200, 120), xtpPaneDockBottom);
    if (total_download_info_.GetSafeHwnd() == 0) {
        total_download_info_.Create(
            WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_TABSTOP,
            CRect(0, 0, 0, 0), this, 0);
    }
    pPane->Attach(&total_download_info_);

    pPane = m_paneManager.CreatePane(
        IDR_PANE_EACH_DOWNLOAD_INFO, CRect(0, 0,200, 120), xtpPaneDockRight);
    if (each_download_info_.GetSafeHwnd() == 0) {
        each_download_info_.Create(
            WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_TABSTOP,
            CRect(0, 0, 0, 0), this, 0);
    }
    pPane->Attach(&each_download_info_);

    // Set the icons for the docking pane tabs.
    int nIDIcons[] = {IDR_PANE_TOTAL_DOWNLOAD_INFO, IDR_PANE_EACH_DOWNLOAD_INFO};
    m_paneManager.SetIcons(IDB_BITMAP_ICONS, nIDIcons,
        _countof(nIDIcons), RGB(0, 255, 0));

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
        | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE | WS_SYSMENU;

    cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
    cs.lpszClass = AfxRegisterWndClass(0);
    cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
    return TRUE;
}


void MainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
    // forward focus to the view window
    downloadlist_->SetFocus();
}

BOOL MainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
    // let the view have first crack at the command
    if (downloadlist_->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
        return TRUE;

    // otherwise, do default handling
    return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void MainFrame::OnClose()
{
    CFrameWnd::OnClose();
}