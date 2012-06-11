#include "stdafx.h"

#include "ui/main_frame.h"

#include "base/memory/scoped_ptr.h"
#include "resources/Resource.h"

namespace {
static const XTPPaintTheme kTheme = xtpThemeVisualStudio2010;
static const XTPDockingPanePaintTheme kPaneTheme = xtpPaneThemeVisualStudio2010;
static const int frame_height = 500;
static const int frame_width = 640;
static const TCHAR* kSaveChanges = _T("Save Changs to %s?");
static const TCHAR* kDefaultExt = _T(".cst");
static const TCHAR* kDefaultFileName = _T("stress.cst");
static const TCHAR* kFilter = _T("CANStress (*.cst)|*.cst|All Files (*.*)|*.*||");
}

IMPLEMENT_DYNAMIC(MainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(MainFrame, CFrameWnd)
    ON_WM_CREATE()
    ON_WM_SETFOCUS()
    ON_WM_CLOSE()
  
    ON_COMMAND(ID_FILE_NEW, OnNew)
    ON_COMMAND(ID_FILE_OPEN, OnOpen)
    ON_COMMAND(ID_FILE_SAVE, OnSave)
    ON_COMMAND(ID_FILE_SAVE_AS, OnSaveAs)
    ON_UPDATE_COMMAND_UI(ID_FILE_NEW, AlwaysEnable)
    ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, AlwaysEnable)
    ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, AlwaysEnable)
    ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, AlwaysEnable)

    ON_COMMAND(ID_VIEW_ANALOG_DISTURBANCE, OnAnalogDisturbanceView)
    ON_COMMAND(ID_VIEW_CONFIG, OnConfigView)
    ON_UPDATE_COMMAND_UI(ID_VIEW_ANALOG_DISTURBANCE, AlwaysEnable)
    ON_UPDATE_COMMAND_UI(ID_VIEW_CONFIG, AlwaysEnable)
    ON_COMMAND(ID_DEVICE_START, OnDeviceStart)
    ON_COMMAND(ID_DEVICE_STOP, OnDeviceStop)
    ON_UPDATE_COMMAND_UI(ID_DEVICE_START, OnUpdateDeviceStart)
    ON_UPDATE_COMMAND_UI(ID_DEVICE_STOP, OnUpdateDeviceStop)
    ON_UPDATE_COMMAND_UI(ID_INDICATOR_DEVICE_CONNECT, OnUpdateDeviceConnect)
END_MESSAGE_MAP()

MainFrame::MainFrame()
    : analog_disturbance_view_(NULL)
    , config_view_(NULL)
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
        CRect(0, 0, 0, 0), &tab_ctrl_, ID_VIEW_ANALOG_DISTURBANCE, NULL)) {
        TRACE0("Failed to create analog disturbance view tab page\n");
        return -1;
    }

    config_view_ = new ConfigView(&device_);
    // create a view to occupy the client area of the frame
    if (!config_view_->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
      CRect(0, 0, 0, 0), &tab_ctrl_, ID_VIEW_CONFIG, NULL)) {
        TRACE0("Failed to create analog disturbance view tab page\n");
        return -1;
    }

    tab_ctrl_.AddControl(_T("Analog disturbance"), analog_disturbance_view_);
    tab_ctrl_.AddControl(_T("Config"), config_view_);
    tab_ctrl_.SetCurFocus(0);

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
        ID_INDICATOR_DEVICE_CONNECT,
        ID_INDICATOR_NUM,
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

void MainFrame::OnAnalogDisturbanceView() {
    tab_ctrl_.SetCurFocus(0);
}

void MainFrame::OnConfigView() {
    tab_ctrl_.SetCurFocus(1);
}

void MainFrame::OnDeviceStart() {
  device_.set_start(true);
}

void MainFrame::OnDeviceStop() {
  device_.set_start(false);
}

void MainFrame::OnUpdateDeviceStart( CCmdUI* cmd ) {
  cmd->Enable(device_.start() ? FALSE : TRUE);
}

void MainFrame::OnUpdateDeviceStop( CCmdUI* cmd )
{
  cmd->Enable(device_.start() ? TRUE : FALSE);
}

void MainFrame::OnUpdateDeviceConnect( CCmdUI* cmd ) {
  cmd->Enable(TRUE);
  cmd->SetText(device_.connect() ? _T("Connected") : _T("Disconnected"));
}

void MainFrame::OnNew() {
  bool reset = SaveChanged();
  if (reset) {
    device_.Reset();
    last_file_path_.clear();
    OnUpdateFrameTitle(TRUE);
  }
}

bool MainFrame::SaveChanged() {
  if(!device_.Changed())
    return true; // no need to save.
 
  CString text;
  text.Format(kSaveChanges, 
    last_file_path_.empty() ? _T("Untitled") : last_file_path_.BaseName().value());
  int id = AfxMessageBox(text, 
    MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON1);
  switch (id) {
    case IDOK: {
      CFileDialog dlg(FALSE, _T("ini"), _T("stress.ini"), 
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER, 
        _T("Ini Files (*.ini)|*.ini|All Files (*.*)|*.*||"), this);
      if (dlg.DoModal() == IDOK) {
        FilePath path = FilePath(string16(dlg.GetPathName().GetString())); 
        device_.Save(path);
        return true; // save success.
      } else {
        return false; // no when to save.
      }
    }
    case IDCANCEL:
      return false; // no when to close this file
    case IDNO:
      return true; // no save
    default:
      NOTREACHED();
      return false;
  }
}

void MainFrame::OnOpen() {
   scoped_ptr<CFileDialog> dlg(CreateFileDialog(true));
  if (dlg->DoModal() == IDOK && SaveChanged()) {
    last_file_path_ = FilePath(string16(dlg->GetPathName().GetString()));
    device_.Load(last_file_path_);
    OnUpdateFrameTitle(TRUE);
  }
}

void MainFrame::OnSave() {
  if (last_file_path_.empty())
    OnSaveAs();
  else
    device_.Save(last_file_path_);
}

void MainFrame::OnSaveAs() {
  scoped_ptr<CFileDialog> dlg(CreateFileDialog(false));
  if (dlg->DoModal() == IDOK && SaveChanged()) {
    last_file_path_ = FilePath(string16(dlg->GetPathName().GetString()));
    device_.Save(last_file_path_);
    OnUpdateFrameTitle(TRUE);
  }
}

CFileDialog* MainFrame::CreateFileDialog(bool open) {
  if (open) {
    return new CFileDialog(TRUE, kDefaultExt, NULL, // no default file. 
      OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, kFilter, this);
  } else {
    return new CFileDialog(FALSE, kDefaultExt, kDefaultFileName, 
      OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER, kFilter, this);
  }
}
void MainFrame::OnUpdateFrameTitle( BOOL bAddToTitle )
{
  CString title;
  title.LoadString(IDR_MAINFRAME_STRESS);
  title += _T(" - ");
  if (last_file_path_.empty())
    title += _T("(New)");
  else
    title += CString(last_file_path_.value().c_str());
  SetWindowText(title);
}