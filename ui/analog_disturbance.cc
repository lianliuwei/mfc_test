#include "stdafx.h"

#include "ui/analog_disturbance.h"

namespace {
const static double kRMin = 0.0;
const static double kRMax = 10237.5;
const static double kRStep = 2.5;
const static int kRPrecision = 1;
const static double kCMin = 0.0;
const static double kCMax = 15750.0;
const static double kCStep = 250.0;
const static int kCPrecision = 0;

static const bool kLayoutStatus[][kComponentSize] = {
  // kRH kRHL   kRL   kRSH   kCHL    kRSL
  // kStandard
  {true, true, false, false, false, false},
  // kR_H
  {true, true, false, false, false, false},
  // kR_SH
  {false, true, false, false, false, false},
  // kR_HL_without_R_SH
  {false, false, false, true, false, false},
  // kR_HL_without_R_SL
  {false, true, false, false, false, false},
  // kR_SL
  {false, true, false, false, false, true},
  // kR_L
  {false, true, true, false, false, false},
  // kR_H_and_R_SL
  {true, true, false, false, false, true},
  // kR_H_and_R_L
  {true, false, true, false, false, false},
  // kR_SH_and_R_HL
  {false, true, true, true, false, false},
  // kR_SH_and_R_SL
  {false, true, false, true, false, true},
  // kR_SH_and_R_L
  {false, true, true, true, false, false},
  // kR_HL_and_R_SL
  {false, true, false, false, false, true},
  // kR_HL_and_R_H
  {true, true, false, false, false, false},
  // kR_HL_and_R_L
  {false, true, true, false, false, false},
};

static const TCHAR* kLayoutString[] = {
  _T("Standard Layout"),
  _T("R_H"),
  _T("R_SH"),
  _T("R_HL (without R_SH)"),
  _T("R_HL (without_R_SL)"),
  _T("R_SL"),
  _T("R_L"),
  _T("R_H and R_SL"),
  _T("R_H and R_L"),
  _T("R_SH and R_HL"),
  _T("R_SH and R_SL"),
  _T("R_SH and R_L"),
  _T("R_HL and R_SL"),
  _T("R_HL and R_H"),
  _T("R_HL and R_L"),
};

}


BEGIN_MESSAGE_MAP(AnalogDisturbanceView, CFormView)
  ON_COMMAND(IDC_CHECKBOX_RH, OnRHEnable)
  ON_COMMAND(IDC_CHECKBOX_RHL, OnRHLEnable)
  ON_COMMAND(IDC_CHECKBOX_RL, OnRLEnable)
  ON_COMMAND(IDC_CHECKBOX_RSH, OnRSHEnable)
  ON_COMMAND(IDC_CHECKBOX_CHL, OnCHLEnable)
  ON_COMMAND(IDC_CHECKBOX_RSL, OnRSLEnable)
  ON_CBN_SELCHANGE(IDC_COMBOX_LAYOUT, OnLayoutChange)
END_MESSAGE_MAP()

AnalogDisturbanceView::AnalogDisturbanceView(StressDevice* device)
    : CFormView(AnalogDisturbanceView::IDD)
    , value_rh_(IDC_SLIDER_RH, IDC_EDIT_RH, IDC_SPIN_RH, IDC_PICTURE_RH, ID_PNG_RH_ENABLE, ID_PNG_RH_DISABLE,
    kRMin, kRMax, kRStep, 0.0, kRPrecision, this)
    , value_rhl_(IDC_SLIDER_RHL, IDC_EDIT_RHL, IDC_SPIN_RHL, IDC_PICTURE_RHL, ID_PNG_RHL_ENABLE, ID_PNG_RHL_DISABLE,
    kRMin, kRMax, kRStep, 0.0, kRPrecision, this)
    , value_rl_(IDC_SLIDER_RL, IDC_EDIT_RL, IDC_SPIN_RL, IDC_PICTURE_RL, ID_PNG_RL_ENABLE, ID_PNG_RL_DISABLE,
    kRMin, kRMax, kRStep, 0.0, kRPrecision, this)
    , value_rsh_(IDC_SLIDER_RSH, IDC_EDIT_RSH, IDC_SPIN_RSH, IDC_PICTURE_RSH, ID_PNG_RSH_ENABLE, ID_PNG_RSH_DISABLE,
    kRMin, kRMax, kRStep, 0.0, kRPrecision, this)
    , value_chl_(IDC_SLIDER_CHL, IDC_EDIT_CHL, IDC_SPIN_CHL, IDC_PICTURE_CHL, ID_PNG_CHL_ENABLE, ID_PNG_CHL_DISABLE,
    kCMin, kCMax, kCStep, 0.0, kCPrecision, this)
    , value_rsl_(IDC_SLIDER_RSL, IDC_EDIT_RSL, IDC_SPIN_RSL, IDC_PICTURE_RSL, ID_PNG_RSL_ENABLE, ID_PNG_RSL_DISABLE,
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

  DDX_Control(pDX, IDC_COMBOX_LAYOUT, layout_);

  DDX_Control(pDX, IDC_PICTURE_CAN_H_VOLT, can_h_volt_);
  DDX_Control(pDX, IDC_PICTURE_CAN_L_VOLT, can_l_volt_);

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
  device_->SetComponentEnable(kRH, enable);
}

void AnalogDisturbanceView::OnRHLEnable() {
  bool enable = (rhl_enable_.GetChecked() == TRUE);
  device_->SetComponentEnable(kRHL, enable);
}

void AnalogDisturbanceView::OnRLEnable() {
  bool enable = (rl_enable_.GetChecked() == TRUE);
  device_->SetComponentEnable(kRL, enable);
}

void AnalogDisturbanceView::OnRSHEnable() {
  bool enable = (rsh_enable_.GetChecked() == TRUE);
  device_->SetComponentEnable(kRSH, enable);
}

void AnalogDisturbanceView::OnCHLEnable() {
  bool enable = (chl_enable_.GetChecked() == TRUE);
  device_->SetComponentEnable(kCHL, enable);
}

void AnalogDisturbanceView::OnRSLEnable() {
  bool enable = (rsl_enable_.GetChecked() == TRUE);
  device_->SetComponentEnable(kRSL, enable);
}

void AnalogDisturbanceView::Init()
{
  for (int i = 0; i < kLayoutSize; ++i)
    layout_.AddString(kLayoutString[i]);

  value_rh_.set_value(device_->ComponentValue(kRH));
  value_rhl_.set_value(device_->ComponentValue(kRHL));
  value_rl_.set_value(device_->ComponentValue(kRL));
  value_rsh_.set_value(device_->ComponentValue(kRSH));
  value_chl_.set_value(device_->ComponentValue(kCHL));
  value_rsl_.set_value(device_->ComponentValue(kRSL));

  OnComponendEnableChanged(kRH, device_->ComponentEnable(kRH));
  OnComponendEnableChanged(kRHL, device_->ComponentEnable(kRHL));
  OnComponendEnableChanged(kRL, device_->ComponentEnable(kRL));
  OnComponendEnableChanged(kRSH, device_->ComponentEnable(kRSH));
  OnComponendEnableChanged(kCHL, device_->ComponentEnable(kCHL));
  OnComponendEnableChanged(kRSL, device_->ComponentEnable(kRSL));

  OnDisturbanceVoltageChanged(CAN_HIGH, device_->GetDisturbanceVoltage(CAN_HIGH));
  OnDisturbanceVoltageChanged(CAN_LOW, device_->GetDisturbanceVoltage(CAN_LOW));

  // listener the device_ get notify when device change.
  device_->set_listener(this);
}


void AnalogDisturbanceView::OnLayoutChange() {
  // the combobox select and the StressLayout are match.
  StressLayout select = static_cast<StressLayout>(layout_.GetCurSel());
  ASSERT(0 <= select && select < kLayoutSize);
  device_->SetComponentEnable(kRH, kLayoutStatus[select][kRH]);
  device_->SetComponentEnable(kRHL, kLayoutStatus[select][kRHL]);
  device_->SetComponentEnable(kRL, kLayoutStatus[select][kRL]);
  device_->SetComponentEnable(kRSH, kLayoutStatus[select][kRSH]);
  device_->SetComponentEnable(kCHL, kLayoutStatus[select][kCHL]);
  device_->SetComponentEnable(kRSL, kLayoutStatus[select][kRSL]);
}

void AnalogDisturbanceView::OnComponendEnableChanged( StressComponent component,
                                                     bool enable ) {
  switch (component) {
  case kCHL:
    value_chl_.SetEnable(enable);
    chl_enable_.SetChecked(enable);
    break;
  case kRH:
    value_rh_.SetEnable(enable);
    rh_enable_.SetChecked(enable);
    break;
  case kRHL:
    value_rhl_.SetEnable(enable);
    rhl_enable_.SetChecked(enable);
    break;
  case kRL:
    value_rl_.SetEnable(enable);
    rl_enable_.SetChecked(enable);
    break;
  case kRSH:
    value_rsh_.SetEnable(enable);
    rsh_enable_.SetChecked(enable);
    break;
  case kRSL:
    value_rsl_.SetEnable(enable);
    rsl_enable_.SetChecked(enable);
    break;
  default:
    ASSERT(FALSE); // no exit
  }
}

void AnalogDisturbanceView::OnComponendValueChanged(StressComponent component, double value)
{
  switch (component) {
  case kCHL:
    value_chl_.set_value(value); break;
  case kRH:
    value_rh_.set_value(value); break;
  case kRHL:
    value_rhl_.set_value(value); break;
  case kRL:
    value_rl_.set_value(value); break;
  case kRSH:
    value_rsh_.set_value(value); break;
  case kRSL:
    value_rsl_.set_value(value); break;
  default:
    ASSERT(FALSE); // no exit
  }
}

void AnalogDisturbanceView::OnDisturbanceVoltageChanged(CAN_CHNL chnl,
                                                        DisturbanceVoltage volt) {
  switch(chnl) {
  case CAN_HIGH:
    can_h_volt_.Load( volt== VOLT_PLUS ? ID_PNG_CAN_HIGH_VOLT_PLUS : ID_PNG_CAN_HIGH_VOLT_MINUS); 
    break;
  case CAN_LOW:
    can_l_volt_.Load( volt== VOLT_PLUS ? ID_PNG_CAN_LOW_VOLT_PLUS : ID_PNG_CAN_LOW_VOLT_MINUS); 
    break;
  default:
    ASSERT(FALSE);
  }
}
