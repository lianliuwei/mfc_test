#include "stdafx.h"

#include "ui/each_download_info_view.h"

#include "resources/Resource.h"

BEGIN_MESSAGE_MAP(EachDownloadInfoView, CXTPTaskPanel)
    ON_WM_CREATE()
END_MESSAGE_MAP()

EachDownloadInfoView::EachDownloadInfoView() {}

EachDownloadInfoView::~EachDownloadInfoView() {}

int EachDownloadInfoView::OnCreate(LPCREATESTRUCT lpCreateStruct) {

    if (CXTPTaskPanel::OnCreate(lpCreateStruct) == -1)
        return -1;

    AddSampleData();

    return 0;
}

void EachDownloadInfoView::AddSampleData()
{
    CXTPTaskPanelGroup* pGroupDetails = AddGroup(ID_TASKGROUP_TIMEINFO);

    pGroupDetails->AddTextItem(_T("used time: 10 min"));
    pGroupDetails->AddTextItem(_T("need time: 1 hour"));
    pGroupDetails->AddTextItem(_T("total time: 1 hour 10 min"));
}
