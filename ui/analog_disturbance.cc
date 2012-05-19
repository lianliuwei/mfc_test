#include "stdafx.h"

#include "ui/analog_disturbance.h"
#include "stress/stress_device.h"

namespace {
const static double kRMin = 0.0;
const static double kRMax = 10237.5;
const static double kRStep = 2.5;
const static int kRPrecision = 1;
const static double kCMin = 0.0;
const static double kCMax = 15750.0;
const static double kCStep = 250.0;
const static int kCPrecision = 0;
}


BEGIN_MESSAGE_MAP(AnalogDisturbanceView, CFormView)
  ON_COMMAND(IDC_CHECKBOX_RH, OnRHEnable)
  ON_COMMAND(IDC_CHECKBOX_RHL, OnRHLEnable)
  ON_COMMAND(IDC_CHECKBOX_RL, OnRLEnable)
  ON_COMMAND(IDC_CHECKBOX_RSH, OnRSHEnable)
  ON_COMMAND(IDC_CHECKBOX_CHL, OnCHLEnable)
  ON_COMMAND(IDC_CHECKBOX_RSL, OnRSLEnable)
  ON_COMMAND(IDC_CHECKBOX_OSC_LISTEN_PORT, OnOscListenPort)
  ON_COMMAND(IDC_CHECKBOX_CANH_DIS_VOLT, OnCANHDisturbVolt)
  ON_COMMAND(IDC_CHECKBOX_CANL_DIS_VOLT, OnCANLDisturbVolt)
  ON_COMMAND(IDC_CHECKBOX_CAN_BUS_TYPE, OnBusType)
END_MESSAGE_MAP()

AnalogDisturbanceView::AnalogDisturbanceView(StressDevice* device)
    : CFormView(AnalogDisturbanceView::IDD)
    , value_rh_(IDC_SLIDER_RH, IDC_EDIT_RH, IDC_SPIN_RH,
    kRMin, kRMax, kRStep, 0.0, kRPrecision, this)
    , value_rhl_(IDC_SLIDER_RHL, IDC_EDIT_RHL, IDC_SPIN_RHL,
    kRMin, kRMax, kRStep, 0.0, kRPrecision, this)
    , value_rl_(IDC_SLIDER_RL, IDC_EDIT_RL, IDC_SPIN_RL,
    kRMin, kRMax, kRStep, 0.0, kRPrecision, this)
    , value_rsh_(IDC_SLIDER_RSH, IDC_EDIT_RSH, IDC_SPIN_RSH,
    kRMin, kRMax, kRStep, 0.0, kRPrecision, this)
    , value_chl_(IDC_SLIDER_CHL, IDC_EDIT_CHL, IDC_SPIN_CHL,
    kCMin, kCMax, kCStep, 0.0, kCPrecision, this)
    , value_rsl_(IDC_SLIDER_RSL, IDC_EDIT_RSL, IDC_SPIN_RSL,
    kRMin, kRMax, kRStep, 0.0, kRPrecision, this)
    , init_(false)
    , device_(device) {
  ASSERT(device != NULL);
}

BOOL AnalogDisturbanceView::Create( LPCTSTR lpszClassName, LPCTSTR lpszWindowName, 
                                   DWORD dwRequestedStyle, const RECT& rect,
                                   CWnd* pParentWnd, UINT nID, CCreateContext* pContext )
{
  return CFormView::Create(lpszClassName, lpszWindowName, 
    dwRequestedStyle, rect, pParentWnd, nID, pContext);
}

void AnalogDisturbanceView::DoDataExchange( CDataExchange* pDX )
{
  CFormView::DoDataExchange(pDX);

  value_rh_.DoDataExchange(pDX);
  value_rhl_.DoDataExchange(pDX);
  value_rl_.DoDataExchange(pDX);
  value_rsh_.DoDataExchange(pDX);
  value_chl_.DoDataExchange(pDX);
  value_rsl_.DoDataExchange(pDX);

  DDX_Control(pDX, IDC_CHECKBOX_RH, rh_enable_);
  DDX_Control(pDX, IDC_CHECKBOX_RHL, rhl_enable_);
  DDX_Control(pDX, IDC_CHECKBOX_RL, rl_enable_);
  DDX_Control(pDX, IDC_CHECKBOX_RSH, rsh_enable_);
  DDX_Control(pDX, IDC_CHECKBOX_CHL, chl_enable_);
  DDX_Control(pDX, IDC_CHECKBOX_RSL, rsl_enable_);

  DDX_Control(pDX, IDC_CHECKBOX_OSC_LISTEN_PORT, osc_listen_port_);
  DDX_Control(pDX, IDC_CHECKBOX_CANH_DIS_VOLT, can_high_dist_vlot_);
  DDX_Control(pDX, IDC_CHECKBOX_CANL_DIS_VOLT, can_low_dist_vlot_);
  DDX_Control(pDX, IDC_CHECKBOX_CAN_BUS_TYPE, bus_type_);

  if (init_ == false) {
    init_ = true;
    Init();
  }
}

void AnalogDisturbanceView::OnValueChange(ValueGetControls* value_get, double value)
{
  if (value_get == &value_rh_) {
    device_->SetComponentValue(kRH, value);
  } else if (value_get == &value_rhl_) {
    device_->SetComponentValue(kRHL, value);
  } else if (value_get == &value_rl_) {
    device_->SetComponentValue(kRL, value);
  } else if (value_get == &value_rsh_) {
    device_->SetComponentValue(kRSH, value);
  } else if (value_get == &value_chl_) {
    device_->SetComponentValue(kCHL, value);
  } else if (value_get == &value_rsl_) {
    device_->SetComponentValue(kRSL, value);
  } else {
    ASSERT(FALSE); // no exist ValueGetControls
  }
} 

void AnalogDisturbanceView::OnRHEnable() {
  bool enable = (rh_enable_.GetChecked() == TRUE);
  value_rh_.SetEnable(enable);
  device_->SetComponentEnable(kRH, enable);
}

void AnalogDisturbanceView::OnRHLEnable() {
  bool enable = (rhl_enable_.GetChecked() == TRUE);
  value_rhl_.SetEnable(enable);
  device_->SetComponentEnable(kRHL, enable);
}

void AnalogDisturbanceView::OnRLEnable() {
  bool enable = (rl_enable_.GetChecked() == TRUE);
  value_rl_.SetEnable(enable);
  device_->SetComponentEnable(kRL, enable);
}

void AnalogDisturbanceView::OnRSHEnable() {
  bool enable = (rsh_enable_.GetChecked() == TRUE);
  value_rsh_.SetEnable(enable);
  device_->SetComponentEnable(kRSH, enable);
}

void AnalogDisturbanceView::OnCHLEnable() {
  bool enable = (chl_enable_.GetChecked() == TRUE);
  value_chl_.SetEnable(enable);
  device_->SetComponentEnable(kCHL, enable);
}

void AnalogDisturbanceView::OnRSLEnable() {
  bool enable = (rsl_enable_.GetChecked() == TRUE);
  value_rsl_.SetEnable(enable);
  device_->SetComponentEnable(kRSL, enable);
}

void AnalogDisturbanceView::Init()
{
  value_rh_.set_value(device_->ComponentValue(kRH));
  value_rhl_.set_value(device_->ComponentValue(kRHL));
  value_rl_.set_value(device_->ComponentValue(kRL));
  value_rsh_.set_value(device_->ComponentValue(kRSH));
  value_chl_.set_value(device_->ComponentValue(kCHL));
  value_rsl_.set_value(device_->ComponentValue(kRSL));

  rh_enable_.SetChecked(device_->ComponentEnable(kRH));
  rhl_enable_.SetChecked(device_->ComponentEnable(kRHL));
  rl_enable_.SetChecked(device_->ComponentEnable(kRL));
  rsh_enable_.SetChecked(device_->ComponentEnable(kRSH));
  chl_enable_.SetChecked(device_->ComponentEnable(kCHL));
  rsl_enable_.SetChecked(device_->ComponentEnable(kRSL));

  osc_listen_port_.SetChecked(device_->GetOscListenPort() != CAN_IN);
  can_high_dist_vlot_.SetChecked(device_->GetDisturbanceVoltage(CAN_HIGH) != VOLT_PLUS);
  can_low_dist_vlot_.SetChecked(device_->GetDisturbanceVoltage(CAN_LOW) != VOLT_PLUS);
  bus_type_.SetChecked(device_->GetCANBusType() != HIGH_SPEED);

  value_rh_.SetEnable(device_->ComponentEnable(kRH));
  value_rhl_.SetEnable(device_->ComponentEnable(kRHL));
  value_rl_.SetEnable(device_->ComponentEnable(kRL));
  value_rsh_.SetEnable(device_->ComponentEnable(kRSH));
  value_chl_.SetEnable(device_->ComponentEnable(kCHL));
  value_rsl_.SetEnable(device_->ComponentEnable(kRSL));
}

void AnalogDisturbanceView::OnOscListenPort()
{
  device_->SetOscListenPort(!osc_listen_port_.GetChecked() ? CAN_IN : CAN_OUT);
}

void AnalogDisturbanceView::OnCANHDisturbVolt()
{
  device_->SetDisturbanceVoltage(CAN_HIGH, !can_high_dist_vlot_.GetChecked() ? VOLT_PLUS : VOLT_MINUS);
}

void AnalogDisturbanceView::OnCANLDisturbVolt()
{
  device_->SetDisturbanceVoltage(CAN_LOW, !can_low_dist_vlot_.GetChecked() ? VOLT_PLUS : VOLT_MINUS);
}

void AnalogDisturbanceView::OnBusType()
{
  device_->SetCANBusType(!bus_type_.GetChecked() ?  HIGH_SPEED : FAULT_TOLERANT);
}

