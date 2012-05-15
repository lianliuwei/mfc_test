#include "stdafx.h"

#include "ui/analog_disturbance.h"

IMPLEMENT_DYNCREATE(AnalogDisturbanceView, CFormView)

BEGIN_MESSAGE_MAP(AnalogDisturbanceView, CFormView)
END_MESSAGE_MAP()

AnalogDisturbanceView::AnalogDisturbanceView()
: CFormView(AnalogDisturbanceView::IDD) {}

BOOL AnalogDisturbanceView::Create( LPCTSTR /*lpszClassName*/, LPCTSTR /*lpszWindowName*/, 
                                   DWORD dwRequestedStyle, const RECT& rect,
                                   CWnd* pParentWnd, UINT nID, CCreateContext* pContext )
{
  return CFormView::Create(NULL, NULL, dwRequestedStyle, rect, pParentWnd, nID, pContext);
}

void AnalogDisturbanceView::DoDataExchange( CDataExchange* pDX )
{

}
