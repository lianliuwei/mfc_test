#include "stdafx.h"

#include "ui/main_frame.h"

#include "base/logging.h"
#include "resources/Resource.h"
#include "ui/osc_command_ids.h"
#include "ui/enum_define.h"

namespace {
  static const int frame_height = 500;
  static const int frame_width = 640;
}

using namespace std;

IMPLEMENT_DYNAMIC(MainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(MainFrame, CFrameWnd)
  ON_WM_CREATE()
  ON_WM_SETFOCUS()
  ON_WM_CLOSE()
END_MESSAGE_MAP()

MainFrame::MainFrame() {
  command_updater_.reset(new CommandUpdater(this));
  osc_on_off_ = true;
}


MainFrame::~MainFrame() {
  command_updater_->RemoveCommandObserver(config_bar_.get());
}

int MainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
    return -1;

  edit_view_ = new EditView;
  if (!edit_view_->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, 
    CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST)) {
      TRACE0("Failed to create main view\n");
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

  // config UI
  CXTPCommandBars* pCommandBars = GetCommandBars();
  // Set Theme
  pCommandBars->SetTheme(xtpThemeRibbon);
  
  m_wndStatusBar.SetFont(pCommandBars->GetPaintManager()->GetIconFont());
  m_wndStatusBar.SetDrawDisabledText(FALSE);
  m_wndStatusBar.SetCommandBars(pCommandBars);
  m_wndStatusBar.GetStatusBarCtrl().SetMinHeight(22);
  m_wndStatusBar.GetPane(0)->SetMargins(8, 1, 2, 1);

  CXTPToolTipContext* pToolTipContext = GetCommandBars()->GetToolTipContext();
  pToolTipContext->SetStyle(xtpToolTipOffice2007);
  pToolTipContext->ShowTitleAndDescription();
  pToolTipContext->SetMargin(CRect(2, 2, 2, 2));
  pToolTipContext->SetMaxTipWidth(180);
  pToolTipContext->SetFont(pCommandBars->GetPaintManager()->GetIconFont());

  pCommandBars->GetCommandBarsOptions()->ShowKeyboardCues(xtpKeyboardCuesShowWindowsDefault);
  pCommandBars->GetCommandBarsOptions()->bToolBarAccelTips = TRUE;
  // Set "Always Show Full Menus" option to the FALSE
  pCommandBars->GetCommandBarsOptions()->bAlwaysShowFullMenus = FALSE;

  return 0;
}

void MainFrame::LoadIcons()
{
  CXTPCommandBars* pCommandBars = GetCommandBars();

  pCommandBars->GetImageManager()->SetIcons(IDR_SMALLICONS);
  pCommandBars->GetImageManager()->SetIcons(IDR_LARGEICONS);

  UINT uiGroupClipboard[] = {ID_EDIT_PASTE, ID_EDIT_CUT, ID_EDIT_COPY, ID_FORMAT_PAINTER};
  pCommandBars->GetImageManager()->SetIcons(ID_GROUP_CLIPBOARD, uiGroupClipboard, _countof(uiGroupClipboard), CSize(16, 16));

  UINT uiGroupOscControl[] = {ID_START_OSC, ID_STOP_OSC, ID_DUMP_1, ID_AUTOSCALE};
  pCommandBars->GetImageManager()->SetIcons(ID_GROUP_OSC_CONTROL, uiGroupOscControl, _countof(uiGroupOscControl), CSize(32, 32));

  UINT uiGroupFind[] = {ID_EDIT_FIND, ID_EDIT_REPLACE, ID_EDIT_GOTO, ID_EDIT_SELECT};
  pCommandBars->GetImageManager()->SetIcons(ID_GROUP_EDITING, uiGroupFind, _countof(uiGroupFind), CSize(16, 16));

  UINT uCommand = {IDB_GEAR};
  pCommandBars->GetImageManager()->SetIcons(IDB_GEAR, &uCommand, 1, CSize(0, 0), xtpImageNormal);
}

BOOL MainFrame::CreateRibbonBar()
{
  CXTPCommandBars* pCommandBars = GetCommandBars();

  CMenu menu;
  menu.Attach(::GetMenu(m_hWnd));
  SetMenu(NULL);

  CXTPRibbonBar* pRibbonBar = (CXTPRibbonBar*)pCommandBars->Add(_T("The Ribbon"), xtpBarTop, RUNTIME_CLASS(CXTPRibbonBar));
  if (!pRibbonBar)
  {
    return FALSE;
  }

  pRibbonBar->EnableDocking(0);

  CXTPControlPopup* pControlFile = (CXTPControlPopup*)pRibbonBar->AddSystemButton(0);
  pControlFile->SetCommandBar(menu.GetSubMenu(0));

  pControlFile->SetIconId(IDB_GEAR);

  // Osc Tab
  config_bar_.reset(new RibbonOscConfigBar(pRibbonBar, this, command_updater_.get()));
  LOG_ASSERT(config_bar_->Init());
  command_updater_->AddCommandObserver(IDC_OSC_ON_OFF, config_bar_.get());
  command_updater_->AddCommandObserver(IDC_AUTOSCALE, config_bar_.get());
  command_updater_->AddCommandObserver(IDC_CHNL_WAVE_VOLT_OFFSET, config_bar_.get());
  command_updater_->AddCommandObserver(IDC_CHNL_WAVE_COUPLING, config_bar_.get());
  InitCommandState();

  // home Tab
  CXTPRibbonTab* pTabHome = pRibbonBar->AddTab(ID_TAB_HOME);

  // file Group
  CXTPRibbonGroup* pGroupFile = pTabHome->AddGroup(ID_GROUP_FILE);
  pGroupFile->Add(xtpControlButton, ID_FILE_NEW);
  pGroupFile->Add(xtpControlButton, ID_FILE_OPEN);
  pGroupFile->Add(xtpControlButton, ID_FILE_CLOSE);
  // save Popup
  CXTPControlPopup* pControlSave = (CXTPControlPopup*)pGroupFile->Add(xtpControlSplitButtonPopup, ID_FILE_SAVE);
  pControlSave->GetCommandBar()->GetControls()->Add(xtpControlButton, ID_FILE_SAVE);
  pControlSave->GetCommandBar()->GetControls()->Add(xtpControlButton, ID_FILE_SAVE_AS);
  // Print Popup
  CXTPControlPopup* pControlPrint = (CXTPControlPopup*)pGroupFile->Add(xtpControlSplitButtonPopup, ID_FILE_PRINT);
  pControlPrint->GetCommandBar()->GetControls()->Add(xtpControlButton, ID_FILE_PRINT);
  pControlPrint->GetCommandBar()->GetControls()->Add(xtpControlButton, ID_FILE_PRINT_SETUP);
  pControlPrint->SetBeginGroup(TRUE); // in file Group this begin split 

  // edit Tab
  CXTPRibbonTab* pTabEdit = pRibbonBar->AddTab(ID_TAB_EDIT);
  // clipboard Group
  CXTPRibbonGroup* pGroupClipborad = pTabEdit->AddGroup(ID_GROUP_CLIPBOARD);
  pGroupClipborad->ShowOptionButton();
  // paste Popup
  CXTPControlPopup* pControlPaste = (CXTPControlPopup*)pGroupClipborad->Add(xtpControlSplitButtonPopup, ID_EDIT_PASTE);
  pControlPaste->GetCommandBar()->GetControls()->Add(xtpControlButton, ID_EDIT_PASTE);
  pControlPaste->GetCommandBar()->GetControls()->Add(xtpControlButton, ID_EDIT_PASTE_SPECIAL);
  pGroupClipborad->Add(xtpControlButton, ID_EDIT_CUT);
  pGroupClipborad->Add(xtpControlButton, ID_EDIT_COPY);	
  pGroupClipborad->Add(xtpControlButton, ID_FORMAT_PAINTER);
  // edit Group
  CXTPRibbonGroup* pGroupEditing = pTabEdit->AddGroup(ID_GROUP_EDITING);
  pGroupEditing->Add(xtpControlButton, ID_EDIT_FIND);
  pGroupEditing->Add(xtpControlButton, ID_EDIT_REPLACE);
  pGroupEditing->Add(xtpControlButton, ID_EDIT_GOTO);
  // select Popup
  CXTPControlPopup* pControlSelect = (CXTPControlPopup*)pGroupEditing->Add(xtpControlPopup, ID_EDIT_SELECT);
  pControlSelect->GetCommandBar()->GetControls()->Add(xtpControlButton, ID_EDIT_SELECT_ALL);
  pControlSelect->SetFlags(xtpFlagManualUpdate);

  // view Tab
  CXTPRibbonTab* pTabView = pRibbonBar->AddTab(ID_TAB_VIEW);
  // doc Group
  CXTPRibbonGroup* pGroupDocumentView = pTabView->AddGroup(ID_GROUP_DOCUMENTVIEWS);
  pGroupDocumentView->Add(xtpControlButton, ID_VIEW_NORMAL);
  pGroupDocumentView->Add(xtpControlButton, ID_FILE_PRINT_PREVIEW);
  pGroupDocumentView->Add(xtpControlButton, ID_VIEW_FULLSCREEN);
  // showhide Group
  CXTPRibbonGroup* pGroupShowHide = pTabView->AddGroup(ID_GROUP_SHOWHIDE);
  pGroupShowHide->Add(xtpControlCheckBox, ID_VIEW_STATUS_BAR);
  pGroupShowHide->Add(xtpControlCheckBox, ID_VIEW_WORKSPACE);
  // window Group
  CXTPRibbonGroup* pGroupWindow = pTabView->AddGroup(ID_GROUP_WINDOW);
  pGroupWindow->Add(xtpControlButton, ID_WINDOW_NEW);
  pGroupWindow->Add(xtpControlButton, ID_WINDOW_ARRANGE);
  CXTPControlPopup* pControlSwitchWindows = (CXTPControlPopup*)pGroupWindow->Add(xtpControlPopup, ID_WINDOW_SWITCH);
  pControlSwitchWindows->GetCommandBar()->GetControls()->Add(xtpControlButton, XTP_ID_WINDOWLIST);

  // about button on the ribbon bar
  CXTPControl* pControlAbout = pRibbonBar->GetControls()->Add(xtpControlButton, ID_APP_ABOUT);
  pControlAbout->SetFlags(xtpFlagRightAlign);

  // add Control to QuickAccessControls
  pRibbonBar->GetQuickAccessControls()->Add(xtpControlButton, ID_FILE_SAVE);
  pRibbonBar->GetQuickAccessControls()->Add(xtpControlButton, ID_EDIT_UNDO);
  pRibbonBar->GetQuickAccessControls()->Add(xtpControlButton, ID_FILE_PRINT);
  pRibbonBar->GetQuickAccessControls()->CreateOriginalControls();

  pRibbonBar->EnableFrameTheme();

  return TRUE;
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
  
  LoadIcons();

  if (!CreateRibbonBar()) {
    TRACE0("Failed to create Ribbon bar.\n");
    return -1;      // fail to create
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

  cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE | WS_THICKFRAME
    | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU;

  cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
  cs.lpszClass = AfxRegisterWndClass(0);
  cs.style |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
  cs.cx = frame_width;
  cs.cy = frame_height;
  return TRUE;
}

void MainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
  // forward focus to the view window
  edit_view_->SetFocus();
}

void MainFrame::OnClose()
{
  CFrameWnd::OnClose();
}

void MainFrame::ExecuteCommand(int id, const base::Value& param)
{
  switch (id) {
   case IDC_OSC_ON_OFF: {
     bool on_off;
     CHECK(param.GetAsBoolean(&on_off));
     LOG_ASSERT(osc_on_off_ != on_off);
     osc_on_off_ = on_off;
     scoped_ptr<base::Value> value(
       base::Value::CreateBooleanValue(osc_on_off_));
     command_updater_->UpdateCommandParam(IDC_OSC_ON_OFF, *(value.get()));
     break;
     }
   
   case IDC_CHNL_WAVE_VOLT_OFFSET: {
     double value;
     string16 unit;
     const base::DictionaryValue* dict;
     CHECK(param.GetAsDictionary(&dict));
     CHECK(dict->GetDouble(string(kValuePath), &value));
     CHECK(dict->GetString(string(kUnitPath), &unit));
     CHECK(unit == string16(_T("V")));
     value += 1.0;
     scoped_ptr<base::DictionaryValue> quantity(new base::DictionaryValue);
     quantity->SetDouble(string(kValuePath), value);
     quantity->SetString(string(kUnitPath), string16(_T("V")));
     command_updater_->UpdateCommandParam(IDC_CHNL_WAVE_VOLT_OFFSET, *(quantity.get()));
     break;
     }
   case IDC_CHNL_WAVE_COUPLING: {
     int select;
     CHECK(param.GetAsInteger(&select));
     switch (select) {
       case AC:
         select = DC;
         break;
       case DC:
         select = AC;
         break;
       default:
         NOTREACHED();
     };
     scoped_ptr<base::Value> coupling(
       base::Value::CreateIntegerValue(select));
     command_updater_->UpdateCommandParam(IDC_CHNL_WAVE_COUPLING, *(coupling.get()));
     break;
     }
  }
}

void MainFrame::InitCommandState()
{
  command_updater_->UpdateCommandEnabled(
    IDC_OSC_ON_OFF, true);
  command_updater_->UpdateCommandEnabled(
    IDC_AUTOSCALE, true);
  command_updater_->UpdateCommandEnabled(
    IDC_CHNL_WAVE_VOLT_OFFSET, true);
  command_updater_->UpdateCommandEnabled(
    IDC_CHNL_WAVE_COUPLING, true);

  scoped_ptr<base::Value> value(
    base::Value::CreateBooleanValue(osc_on_off_));
  command_updater_->UpdateCommandParam(IDC_OSC_ON_OFF, *(value.get()));
  
  scoped_ptr<base::DictionaryValue> quantity(new base::DictionaryValue);
  quantity->SetDouble(string(kValuePath), 1);
  quantity->SetString(string(kUnitPath), string16(_T("V")));
  command_updater_->UpdateCommandParam(IDC_CHNL_WAVE_VOLT_OFFSET, *(quantity.get()));

  scoped_ptr<base::Value> coupling(
    base::Value::CreateIntegerValue(AC));
  command_updater_->UpdateCommandParam(IDC_CHNL_WAVE_COUPLING, *(coupling.get()));
}