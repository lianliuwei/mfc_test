#pragma once

#include "ui/analog_disturbance.h"

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

    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSetFocus(CWnd *pOldWnd);
    afx_msg void OnClose();
    DECLARE_MESSAGE_MAP()

private:
    CXTPStatusBar  m_wndStatusBar;

    // add each client in the Tab ctrl.
    CXTPTabCtrl tab_ctrl_;

    // this is a view, view destory it self.
    AnalogDisturbanceView* analog_disturbance_view_;

    CXTPDockingPaneManager m_paneManager;
    
    DECLARE_DYNAMIC(MainFrame)
};
