#pragma once

class DownloadList : public CXTPReportView
{
public:
    DownloadList();
    virtual ~DownloadList();


private:
    enum Column {
        kStatus = 0,
        kFileName = 1,
        kProgress = 2,
        kSpeed = 3,
    };

    void AddSampleRecords();

    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    DECLARE_MESSAGE_MAP()

private:
    CXTPCommandBarScrollBarCtrl m_wndVScrollBar;
    CXTPCommandBarScrollBarCtrl m_wndHScrollBar;
};
