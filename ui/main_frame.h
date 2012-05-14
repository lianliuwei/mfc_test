#pragma once

#include "ui/download_list.h"
#include "ui/each_download_info_view.h"
#include "ui/total_download_info_view.h"

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
    DownloadList* downloadlist_;

    // the download info of each item.
    EachDownloadInfoView each_download_info_;

    // total info 
    TotalDownloadInfoView total_download_info_;

    CXTPDockingPaneManager m_paneManager;
    
    DECLARE_DYNAMIC(MainFrame)
};
