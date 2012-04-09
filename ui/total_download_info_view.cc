#include "stdafx.h"

#include "ui/total_download_info_view.h"

#include "resources/Resource.h"

BEGIN_MESSAGE_MAP(TotalDownloadInfoView, CXTPTaskPanel)
    ON_WM_CREATE()
END_MESSAGE_MAP()

TotalDownloadInfoView::TotalDownloadInfoView() {}

TotalDownloadInfoView::~TotalDownloadInfoView() {}

int TotalDownloadInfoView::OnCreate(LPCREATESTRUCT lpCreateStruct) {

    if (CXTPTaskPanel::OnCreate(lpCreateStruct) == -1)
        return -1;

    AddSampleData();

    return 0;
}

void TotalDownloadInfoView::AddSampleData()
{
    CXTPTaskPanelGroup* pGroupDetails = AddGroup(ID_TASKGROUP_TOTAL_DOWNLOAD);

    pGroupDetails->AddTextItem(_T("total size: 10 GB"));
    pGroupDetails->AddTextItem(_T("last hour size: 1 GB"));
}
