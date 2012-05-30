#include "stdafx.h"

#include "ui/config_view.h"

namespace {
  static const TCHAR* kOscListenPortString[] = {
    _T("CAN IN"),
    _T("CAN OUT"),
  };
  static const TCHAR* kDisturbVoltString[] = {
    _T("VDIS+"),
    _T("VDIS-"),
  };
  static const TCHAR* kCANBusTypeString[] = {
    _T("high speed"),
    _T("fault tolerant"),
  };
}

BEGIN_MESSAGE_MAP(ConfigView, CFormView)
  ON_CBN_SELCHANGE(IDC_COMBO_OSC_LISTEN_PORT, OnOscListenPort)
  ON_CBN_SELCHANGE(IDC_COMBO_CANH_DIS_VOLT, OnCANLDisturbVolt)
  ON_CBN_SELCHANGE(IDC_COMBO_CANL_DIS_VOLT, OnCANHDisturbVolt)
  ON_CBN_SELCHANGE(IDC_COMBO_CAN_BUS_TYPE, OnCANBusType)
END_MESSAGE_MAP()

BOOL ConfigView::Create( LPCTSTR lpszClassName, LPCTSTR lpszWindowName, 
                         DWORD dwRequestedStyle, const RECT& rect,
                         CWnd* pParentWnd, UINT nID, CCreateContext* pContext )
{
  return CFormView::Create(lpszClassName, lpszWindowName, 
    dwRequestedStyle, rect, pParentWnd, nID, pContext);
}

void ConfigView::DoDataExchange(CDataExchange* pDX) {
  CFormView::DoDataExchange(pDX);

  DDX_Control(pDX, IDC_COMBO_OSC_LISTEN_PORT, osc_listen_port_);
  DDX_Control(pDX, IDC_COMBO_CANH_DIS_VOLT, can_high_dist_volt_);
  DDX_Control(pDX, IDC_COMBO_CANL_DIS_VOLT, can_low_dist_volt_);
  DDX_Control(pDX, IDC_COMBO_CAN_BUS_TYPE, can_bus_type_);

  if (init_ == false) {
    init_ = true;
    Init();
  }
}

void ConfigView::Init() {
  // TODO using arraysize replace.
  for(int i = 0; i < 2; ++i)
    osc_listen_port_.AddString(kOscListenPortString[i]);
  for(int i = 0; i < 2; ++i)
    can_high_dist_volt_.AddString(kDisturbVoltString[i]);
  for(int i = 0; i < 2; ++i)
    can_low_dist_volt_.AddString(kDisturbVoltString[i]);
  for(int i = 0; i < 2; ++i)
    can_bus_type_.AddString(kCANBusTypeString[i]);
  osc_listen_port_.SetCurSel(device_->GetOscListenPort());
  can_high_dist_volt_.SetCurSel(device_->GetDisturbanceVoltage(CAN_HIGH));
  can_low_dist_volt_.SetCurSel(device_->GetDisturbanceVoltage(CAN_LOW));
  can_bus_type_.SetCurSel(device_->GetCANBusType());
}

void ConfigView::OnOscListenPort() {
   device_->SetOscListenPort(
     static_cast<OscListenPort>(osc_listen_port_.GetCurSel()));
}

void ConfigView::OnCANHDisturbVolt() {
  device_->SetDisturbanceVoltage(CAN_HIGH, 
    static_cast<DisturbanceVoltage>(can_high_dist_volt_.GetCurSel()));
}

void ConfigView::OnCANLDisturbVolt() {
  device_->SetDisturbanceVoltage(CAN_LOW, 
    static_cast<DisturbanceVoltage>(can_low_dist_volt_.GetCurSel()));
}

void ConfigView::OnCANBusType() {
  device_->SetCANBusType(
    static_cast<CANBusType>(can_bus_type_.GetCurSel()));
}

void ConfigView::OnDisturbanceVoltageChanged( CAN_CHNL chnl, 
                                             DisturbanceVoltage volt ){
  switch (chnl) {
  case CAN_HIGH:
    can_high_dist_volt_.SetCurSel(device_->GetDisturbanceVoltage(CAN_HIGH));
    break;
  case CAN_LOW:
    can_low_dist_volt_.SetCurSel(device_->GetDisturbanceVoltage(CAN_LOW));
    break;
  default:
    ASSERT(0);
  }
}
