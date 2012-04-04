#include "stdafx.h"

#include "ui/download_list.h"

BEGIN_MESSAGE_MAP(DownloadList, CXTPReportView)
    ON_WM_CREATE()
END_MESSAGE_MAP()

DownloadList::DownloadList() {
}

DownloadList::~DownloadList() {}

int DownloadList::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CXTPReportView::OnCreate(lpCreateStruct) == -1)
        return -1;

    CXTPReportControl& wndReport = GetReportCtrl();
    wndReport.AddColumn(new CXTPReportColumn(kStatus, _T("Status"), 20));
    wndReport.AddColumn(new CXTPReportColumn(kFileName, _T("FileName"), 40));
    wndReport.AddColumn(new CXTPReportColumn(kProgress, _T("Progress"), 60));


    m_wndVScrollBar.Create(WS_CHILD | WS_VISIBLE | SBS_VERT, 
        CRect(0, 0, 0, 0), this, 100);
//    m_wndVScrollBar.SetScrollBarStyle(xtpScrollStyleOffice2007Dark);
    SetScrollBarCtrl(&m_wndVScrollBar, FALSE);

    m_wndHScrollBar.Create(WS_CHILD | WS_VISIBLE | SBS_HORZ,
        CRect(0, 0, 0, 0), this, 100);
//   m_wndHScrollBar.SetScrollBarStyle(xtpScrollStyleOffice2007Dark);
    SetScrollBarCtrl(&m_wndHScrollBar, TRUE);

    AddSampleRecords();

    return 0;
}

void DownloadList::AddSampleRecords() {

}