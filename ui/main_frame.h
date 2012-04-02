#pragma once

#include "ui/main_view.h"

class MainFrame : public CXTPFrameWnd
{

public:
    MainFrame();
    virtual ~MainFrame();

public:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

private:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSetFocus(CWnd *pOldWnd);
    afx_msg void OnClose();
    afx_msg void OnCustomize();
    afx_msg LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()

private:
    CXTPStatusBar  m_wndStatusBar;
    MainView    m_wndView;
    CXTPDockingPaneManager m_paneManager;
    CXTPOfficeBorder<CStatic> m_wndOptions;
    CXTPOfficeBorder<CEdit> m_wndProperties;

    DECLARE_DYNAMIC(MainFrame)
};
