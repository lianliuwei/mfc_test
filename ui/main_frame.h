#pragma once

#include "ui/download_list.h"

class MainFrame : public CXTPFrameWnd
{

public:
    MainFrame();
    virtual ~MainFrame();

public:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

private:
    int CreateCommandBars();
    int CreatePanes();

    void LoadLayout();
    void SaveLayout();

    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSetFocus(CWnd *pOldWnd);
    afx_msg void OnClose();
    DECLARE_MESSAGE_MAP()

private:
    CXTPStatusBar  m_wndStatusBar;
    DownloadList* downloadlist_;
    CXTPDockingPaneManager m_paneManager;
    CXTPOfficeBorder<CStatic> m_wndOptions;
    CXTPOfficeBorder<CEdit> m_wndProperties;

    DECLARE_DYNAMIC(MainFrame)
};
