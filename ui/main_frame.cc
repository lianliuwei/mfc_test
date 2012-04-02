#include "stdafx.h"

#include "ui/main_frame.h"

#include "resources/Resource.h"

namespace {

static UINT indicators[] =
{
    ID_SEPARATOR,           // status line indicator
    ID_INDICATOR_CAPS,
    ID_INDICATOR_NUM,
    ID_INDICATOR_SCRL,
};

static UINT uHideCmds[] =
{
    ID_FILE_PRINT,
    ID_FILE_PRINT_PREVIEW,
};

}

IMPLEMENT_DYNAMIC(MainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(MainFrame, CFrameWnd)
    ON_WM_CREATE()
    ON_WM_SETFOCUS()
    ON_WM_CLOSE()
    ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)
    ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)
END_MESSAGE_MAP()


MainFrame::MainFrame() {}

MainFrame::~MainFrame() {}


int MainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    // create a view to occupy the client area of the frame
    if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
        CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL)) {
        TRACE0("Failed to create view window\n");
        return -1;
    }


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

    // Set Windows XP Theme
    CXTPPaintManager::SetTheme(xtpThemeNativeWinXP);

    // Hide array of commands
   // pCommandBars->HideCommands(uHideCmds, _countof(uHideCmds));

    // Set "Always Show Full Menus" option to the FALSE
    pCommandBars->GetCommandBarsOptions()->bAlwaysShowFullMenus = FALSE;

    pCommandBars->GetShortcutManager()->SetAccelerators(IDR_MAINFRAME);

    // Load the previous state for toolbars and menus.
    LoadCommandBars(_T("CommandBars"));

    // Initialize the docking pane manager and set the
    // initial them for the docking panes.  Do this only after all
    // control bars objects have been created and docked.
    m_paneManager.InstallDockingPanes(this);
    // Set Windows XP Theme
    m_paneManager.SetTheme(xtpPaneThemeOffice2007Visio);

    // Create docking panes.
    CXTPDockingPane* pwndPane2 = m_paneManager.CreatePane(
        IDR_PANE_PROPERTIES, CRect(0, 0,200, 120), xtpPaneDockBottom);

    CXTPDockingPane* pwndPane1 = m_paneManager.CreatePane(
        IDR_PANE_OPTIONS, CRect(0, 0,200, 120), xtpPaneDockRight);

    // Set the icons for the docking pane tabs.
    int nIDIcons[] = {IDR_PANE_OPTIONS, IDR_PANE_PROPERTIES};
    m_paneManager.SetIcons(IDB_BITMAP_ICONS, nIDIcons,
        _countof(nIDIcons), RGB(0, 255, 0));

    // Load the previous state for docking panes.
    CXTPDockingPaneLayout layoutNormal(&m_paneManager);
    if (layoutNormal.Load(_T("NormalLayout")))
    {
        m_paneManager.SetLayout(&layoutNormal);
    }

    return 0;
}

BOOL MainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    if( !CFrameWnd::PreCreateWindow(cs) )
        return FALSE;
    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs

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
    m_wndView.SetFocus();
}

BOOL MainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
    // let the view have first crack at the command
    if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
        return TRUE;

    // otherwise, do default handling
    return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void MainFrame::OnClose()
{

    // Save the current state for toolbars and menus.
    SaveCommandBars(_T("CommandBars"));

    // Save the current state for docking panes.
    CXTPDockingPaneLayout layoutNormal(&m_paneManager);
    m_paneManager.GetLayout(&layoutNormal);
    layoutNormal.Save(_T("NormalLayout"));

    CFrameWnd::OnClose();
}


void MainFrame::OnCustomize()
{
    // Get a pointer to the command bars object.
    CXTPCommandBars* pCommandBars = GetCommandBars();
    if(pCommandBars != NULL)
    {
        // Instanciate the customize dialog object.
        CXTPCustomizeSheet dlg(pCommandBars);

        // Add the keyboard page to the customize dialog.
        CXTPCustomizeKeyboardPage pageKeyboard(&dlg);
        dlg.AddPage(&pageKeyboard);
        pageKeyboard.AddCategories(IDR_MAINFRAME);

        // Add the options page to the customize dialog.
        CXTPCustomizeOptionsPage pageOptions(&dlg);
        dlg.AddPage(&pageOptions);

        // Add the commands page to the customize dialog.
        CXTPCustomizeCommandsPage* pCommands = dlg.GetCommandsPage();
        pCommands->AddCategories(IDR_MAINFRAME);

        // Use the command bar manager to initialize the
        // customize dialog.
        pCommands->InsertAllCommandsCategory();
        pCommands->InsertBuiltInMenus(IDR_MAINFRAME);
        pCommands->InsertNewMenuCategory();

        // Dispaly the dialog.
        dlg.DoModal();
    }
}

LRESULT MainFrame::OnDockingPaneNotify(WPARAM wParam, LPARAM lParam)
{
    if (wParam == XTP_DPN_SHOWWINDOW)
    {
        CXTPDockingPane* pPane = (CXTPDockingPane*)lParam;

        if (!pPane->IsValid())
        {
            switch (pPane->GetID())
            {
            case IDR_PANE_PROPERTIES:
                {
                    if (m_wndProperties.GetSafeHwnd() == 0)
                    {
                        m_wndProperties.Create(WS_CHILD|
                            ES_AUTOVSCROLL|ES_MULTILINE,
                            CRect(0, 0, 0, 0), this, 0);
                    }
                    pPane->Attach(&m_wndProperties);
                    break;
                }
            case IDR_PANE_OPTIONS:
                {
                    if (m_wndOptions.GetSafeHwnd() == 0)
                    {
                        m_wndOptions.Create(_T("\n\nOptions"),
                            WS_CHILD|WS_CLIPCHILDREN|
                            WS_CLIPSIBLINGS|SS_CENTER,
                            CRect(0, 0, 0, 0), this, 0);
                    }
                    pPane->Attach(&m_wndOptions);
                    break;
                }
            }
        }
        return TRUE;
    }
    return FALSE;
}
