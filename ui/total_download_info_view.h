#pragma once

class TotalDownloadInfoView : public CXTPTaskPanel
{
public:
    TotalDownloadInfoView();
    virtual ~TotalDownloadInfoView();

private:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    DECLARE_MESSAGE_MAP()

private:
    void AddSampleData();
};