#pragma once

class EachDownloadInfoView : public CXTPTaskPanel
{
public:
    EachDownloadInfoView();
    virtual ~EachDownloadInfoView();

private:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    DECLARE_MESSAGE_MAP()

private:
    void AddSampleData();
};