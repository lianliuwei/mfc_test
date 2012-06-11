#pragma once

#include "ui/analog_disturbance.h"
#include "ui/config_view.h"

#include "stress/stress_device.h"

class MainFrame : public CXTPFrameWnd
{

public:
    MainFrame();
    virtual ~MainFrame();

private:
    int CreateCommandBars();
    int CreatePanes();

    void LoadLayout();
    void SaveLayout();

    // save the changed. return true to continue, false to cancel.
    bool SaveChanged();

    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    // update the title.
    virtual void OnUpdateFrameTitle(BOOL bAddToTitle);
    CFileDialog* CreateFileDialog(bool open);

    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSetFocus(CWnd *pOldWnd);
    afx_msg void OnClose();
    afx_msg void OnAnalogDisturbanceView();
    afx_msg void OnConfigView();

    afx_msg void AlwaysEnable(CCmdUI* cmd) {
      cmd->Enable(TRUE);
    }
    afx_msg void OnDeviceStart();
    afx_msg void OnDeviceStop();
    afx_msg void OnUpdateDeviceStart(CCmdUI* cmd);
    afx_msg void OnUpdateDeviceStop(CCmdUI* cmd);
    afx_msg void OnUpdateDeviceConnect(CCmdUI* cmd);

    afx_msg void OnNew();
    afx_msg void OnOpen();
    afx_msg void OnSave();
    afx_msg void OnSaveAs();

    DECLARE_MESSAGE_MAP()

private:
    CXTPStatusBar  m_wndStatusBar;

    // add each client in the Tab ctrl.
    CXTPTabCtrl tab_ctrl_;

    // this is a view, view destory it self.
    AnalogDisturbanceView* analog_disturbance_view_;
    ConfigView* config_view_;

    CXTPDockingPaneManager m_paneManager;
    
    //
    StressDevice device_;

    // last file path.
    FilePath last_file_path_;

    DECLARE_DYNAMIC(MainFrame)
};
