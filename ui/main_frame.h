#pragma once

#include "ui/float_edit.h"
#include "ui/each_download_info_view.h"
#include "ui/total_download_info_view.h"

class MainFrame : public CXTPFrameWnd
{

public:
    MainFrame();
    virtual ~MainFrame();

public:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, 
        AFX_CMDHANDLERINFO* pHandlerInfo);

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

    CFloatEdit floating_edit_;

    // the download info of each item.
    EachDownloadInfoView each_download_info_;

    // total info 
    TotalDownloadInfoView total_download_info_;

    CXTPDockingPaneManager m_paneManager;
    
    DECLARE_DYNAMIC(MainFrame)
};
