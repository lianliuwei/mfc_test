#pragma once

#include "base/memory/scoped_ptr.h"
#include "ui/command_updater.h"
#include "ui/ribbon_osc_config_bar.h"
#include "ui/edit_view.h"

class MainFrame : public CXTPFrameWnd
                , public CommandUpdater::CommandUpdaterDelegate
{
public:
  MainFrame();
  virtual ~MainFrame();

  // Implement CommandUpdaterDelegate
  virtual void ExecuteCommand(int id, const base::Value& param) OVERRIDE;

private:
  // commandUpdater
  void InitCommandState();

  bool osc_on_off_;
  scoped_ptr<CommandUpdater> command_updater_;
  scoped_ptr<RibbonOscConfigBar> config_bar_;

  BOOL CreateRibbonBar();
  void LoadIcons();

  int CreateCommandBars();
  int CreatePanes();

  void LoadLayout();
  void SaveLayout();

  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnSetFocus(CWnd *pOldWnd);
  afx_msg void OnClose();
  DECLARE_MESSAGE_MAP()

private:
  CXTPStatusBar  m_wndStatusBar;

  // this is a view, view destory it self.
  EditView* edit_view_;

  CXTPDockingPaneManager m_paneManager;

  DECLARE_DYNAMIC(MainFrame)
};
