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
  {false, false, false, false, false, false},
  // kR_H
  {true, false, false, false, false, false},
  // kR_SH
  {false, false, false, true, false, false},
  // kR_HL_without_R_SH
  {false, true, false, false, false, false},
  // kR_HL_without_R_SL
  {false, true, false, false, false, false},
  // kR_SL
  {false, false, false, false, false, true},
  // kR_L
  {false, false, true, false, false, false},
  // kR_H_and_R_SL
  {true, false, false, false, false, true},
  // kR_H_and_R_L
  {true, false, true, false, false, false},
  // kR_SH_and_R_HL
  {false, true, false, true, false, false},
  // kR_SH_and_R_SL
  {false, false, false, true, false, true},
  // kR_SH_and_R_L
  {false, false, true, true, false, false},
  // kR_HL_and_R_SL
  {false, true, false, false, false, true},
  // kR_HL_and_R_H
  {true, true, false, false, false, false},
  // kR_HL_and_R_L
  {false, true, true, false, false, false},
};

static const bool kLayoutCanChange[][kComponentSize] = {
  // kRH kRHL   kRL   kRSH   kCHL    kRSL
  // kStandard
  {true, true, true, true, true, true},
  // kR_H
  {false, true, true, false, true, true},
  // kR_SH
  {false, true, true, false, true, true},
  // kR_HL_without_R_SH
  {true, false, true, false, true, true},
  // kR_HL_without_R_SL
  {true, false, true, true, true, false},
  // kR_SL
  {true, true, false, true, true, false},
  // kR_L
  {true, true, false, true, true, false},
  // kR_H_and_R_SL
  {false, true, false, false, true, false},
  // kR_H_and_R_L
  {false, true, false, false, true, false},
  // kR_SH_and_R_HL
  {false, false, true, false, true, false},
  // kR_SH_and_R_SL
  {false, true, false, false, true, false},
  // kR_SH_and_R_L
  {false, true, false, false, true, false},
  // kR_HL_and_R_SL
  {true, false, false, false, true, false},
  // kR_HL_and_R_H
  {false, false, true, false, true, false},
  // kR_HL_and_R_L
  {true, false, false, false, true, false},
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

// the coord is the layout show contorls local, need to may to the dialog
// coord.
// ClickRect coord is related to the Picture Control.
static const RECT kRHRect = {0, 0, 100, 50};
static const RECT kRHClickRect = {55, 14, 88, 32};
static const RECT kRHLRect = {0 + 10, 50, 50 + 10, 50 + 65};
static const RECT kRHLClickRect = {21, 12, 39, 45};
static const RECT kRLRect = {0, 50 + 65, 100, 50 + 65 + 50};
static const RECT kRLClickRect = {55, 17, 88, 35};
static const RECT kCANHVOLTRect = {100, 0, 100 + 75, 50};
static const RECT kCANHVOLTClickRect = {1, 10, 33 , 34};
static const RECT kCANLVOLTRect = {100, 50 + 65, 100 + 75, 50 + 65 + 50};
static const RECT kCANLVOLTClickRect = {1, 15, 33, 34};
static const RECT kRSHRect = {100 + 75, 0, 100 + 75 + 80, 50};
static const RECT kRSHClickRect = {23, 12, 54, 47};
static const RECT kCHLRect = {100 + 75 + 80 - 50 + 11, 50, 100 + 75 + 80 + 11, 50 + 60};
static const RECT kCHLClickRect = {17, 6, 44, 55};
static const RECT kRSLRect = {100 + 75, 50 + 65 + 50 - 45 - 10, 100 + 75 + 80, 50 + 65 + 50 - 10};
static const RECT kRSLClickRect = {22, 4, 56, 40};


void MoveEnableControls(ComponentEnableControls* controls, 
                        CRect start_rect, CRect picture_rect, CRect button_rect) {
  picture_rect.OffsetRect(start_rect.TopLeft());
  button_rect.OffsetRect(picture_rect.TopLeft()); // related to the picture.
  controls->MoveWindow(picture_rect, button_rect);
}

static const TCHAR* StartStandardEnableError = _T("on Standard layout when \
device is Started the Component can not be ENABLE.");

static const TCHAR* StartStandardValueError = _T("on Standard layout when \
device is Started the Component Value can not be changed.");
}


BEGIN_MESSAGE_MAP(AnalogDisturbanceView, CFormView)
  ON_CBN_SELCHANGE(IDC_COMBOX_LAYOUT, OnLayoutChange)
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
    , rh_enable_(IDC_BUTTON_RH, IDC_PICTURE_RH, 
    ID_PNG_RH_ENABLE, ID_PNG_RH_DISABLE, this)
    , rhl_enable_(IDC_BUTTON_RHL, IDC_PICTURE_RHL, 
    ID_PNG_RHL_ENABLE, ID_PNG_RHL_DISABLE, this)
    , rl_enable_(IDC_BUTTON_RL, IDC_PICTURE_RL,
    ID_PNG_RL_ENABLE, ID_PNG_RL_DISABLE, this)
    , rsh_enable_(IDC_BUTTON_RSH, IDC_PICTURE_RSH, 
    ID_PNG_RSH_ENABLE, ID_PNG_RSH_DISABLE, this)
    , chl_enable_(IDC_BUTTON_CHL, IDC_PICTURE_CHL, 
    ID_PNG_CHL_ENABLE, ID_PNG_CHL_DISABLE, this)
    , rsl_enable_(IDC_BUTTON_RSL, IDC_PICTURE_RSL, 
    ID_PNG_RSL_ENABLE, ID_PNG_RSL_DISABLE, this)
    , can_h_volt_(IDC_BUTTON_CAN_H_VOLT, IDC_PICTURE_CAN_H_VOLT, 
    ID_PNG_CAN_HIGH_VOLT_PLUS, ID_PNG_CAN_HIGH_VOLT_MINUS, this)
    , can_l_volt_(IDC_BUTTON_CAN_L_VOLT, IDC_PICTURE_CAN_L_VOLT, 
    ID_PNG_CAN_LOW_VOLT_PLUS, ID_PNG_CAN_LOW_VOLT_MINUS, this)
    , init_(false)
    , device_(device) {
  ASSERT(device != NULL);

}

AnalogDisturbanceView::~AnalogDisturbanceView() {
  device_->RemoveObserver(this);
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

  rh_enable_.DoDataExchange(pDX);
  rhl_enable_.DoDataExchange(pDX);
  rl_enable_.DoDataExchange(pDX);
  rsh_enable_.DoDataExchange(pDX);
  chl_enable_.DoDataExchange(pDX);
  rsl_enable_.DoDataExchange(pDX);

  can_h_volt_.DoDataExchange(pDX);
  can_l_volt_.DoDataExchange(pDX);

  DDX_Control(pDX, IDC_COMBOX_LAYOUT, layout_);

  if (init_ == false) {
    init_ = true;
    Init();
  }
}

void AnalogDisturbanceView::OnValueChange(ValueGetControls* value_get, double value)
{
  StressComponent comp = ValueGetControlsToStressComponent(value_get);

  // OnStart in Standard layout component can no by change
  if (layout_.GetCurSel() == kStandard && device_->start()) { 
    value_get->set_value(device_->ComponentValue(comp)); // restore.
    AfxMessageBox(StartStandardValueError, MB_OK | MB_ICONINFORMATION | MB_APPLMODAL);
    return;
  }

  device_->SetComponentValue(comp, value);
} 


void AnalogDisturbanceView::OnEnableChange(ComponentEnableControls* component, bool enable)
{
  if (component == &can_h_volt_) {
    device_->SetDisturbanceVoltage(CAN_HIGH, enable ? VOLT_PLUS : VOLT_MINUS);
    return;
  } else if (component == &can_l_volt_) {
    device_->SetDisturbanceVoltage(CAN_LOW, enable ? VOLT_PLUS : VOLT_MINUS);
    return;
  }
  StressComponent comp = ComponentEnableControlsToStressComponent(component);
  // OnStart in Standard layout component can no by change
  if (layout_.GetCurSel() == kStandard && device_->start()) { 
    component->set_enable(device_->ComponentEnable(comp), false); // restore.
    AfxMessageBox(StartStandardEnableError, MB_OK | MB_ICONINFORMATION | MB_APPLMODAL);
    return;
  }
  device_->SetComponentEnable(comp, enable);
} 

void AnalogDisturbanceView::Init()
{
  for (int i = 0; i < kLayoutSize; ++i)
    layout_.AddString(kLayoutString[i]);
  // the ComponentEnableControls is init as can change so no need to set them.
  layout_.SetCurSel(kStandard);

  // place the show layout control in pixel, the DLU is suck.
  LayoutControls();

  OnComponentValueChanged(kRH, device_->ComponentValue(kRH));
  OnComponentValueChanged(kRHL, device_->ComponentValue(kRHL));
  OnComponentValueChanged(kRL, device_->ComponentValue(kRL));
  OnComponentValueChanged(kRSH, device_->ComponentValue(kRSH));
  OnComponentValueChanged(kCHL, device_->ComponentValue(kCHL));
  OnComponentValueChanged(kRSL, device_->ComponentValue(kRSL));

  OnComponentEnableChanged(kRH, device_->ComponentEnable(kRH));
  OnComponentEnableChanged(kRHL, device_->ComponentEnable(kRHL));
  OnComponentEnableChanged(kRL, device_->ComponentEnable(kRL));
  OnComponentEnableChanged(kRSH, device_->ComponentEnable(kRSH));
  OnComponentEnableChanged(kCHL, device_->ComponentEnable(kCHL));
  OnComponentEnableChanged(kRSL, device_->ComponentEnable(kRSL));

  OnDisturbanceVoltageChanged(CAN_HIGH, device_->GetDisturbanceVoltage(CAN_HIGH));
  OnDisturbanceVoltageChanged(CAN_LOW, device_->GetDisturbanceVoltage(CAN_LOW));

  // listener the device_ get notify when device change.
  device_->AddObserver(this);
}

void AnalogDisturbanceView::SetComponentEnable(
    StressComponent component, 
    ComponentEnableControls* enable_controls,
    bool enable,
    bool can_change) {
  // if can_change using the old state.
  if (!can_change)
    device_->SetComponentEnable(component, enable);
  enable_controls->set_can_change(can_change);
}

void AnalogDisturbanceView::OnLayoutChange() {
  // the combobox select and the StressLayout are match.
  StressLayout select = static_cast<StressLayout>(layout_.GetCurSel());
  ASSERT(0 <= select && select < kLayoutSize);
  SetComponentEnable(kRH, &rh_enable_, 
    kLayoutStatus[select][kRH], kLayoutCanChange[select][kRH]);  
  SetComponentEnable(kRHL, &rhl_enable_, 
    kLayoutStatus[select][kRHL], kLayoutCanChange[select][kRHL]);  
  SetComponentEnable(kRL, &rl_enable_, 
    kLayoutStatus[select][kRL], kLayoutCanChange[select][kRL]);  
  SetComponentEnable(kRSH, &rsh_enable_, 
    kLayoutStatus[select][kRSH], kLayoutCanChange[select][kRSH]);  
  SetComponentEnable(kCHL, &chl_enable_, 
    kLayoutStatus[select][kCHL], kLayoutCanChange[select][kCHL]);  
  SetComponentEnable(kRSL, &rsl_enable_, 
    kLayoutStatus[select][kRSL], kLayoutCanChange[select][kRSL]);  
}

void AnalogDisturbanceView::OnComponentEnableChanged( StressComponent component,
                                                     bool enable ) {
  switch (component) {
  case kCHL:
    value_chl_.SetEnable(enable);
    chl_enable_.set_enable(enable, false);
    break;
  case kRH:
    value_rh_.SetEnable(enable);
    rh_enable_.set_enable(enable, false);
    break;
  case kRHL:
    value_rhl_.SetEnable(enable);
    rhl_enable_.set_enable(enable, false);
    break;
  case kRL:
    value_rl_.SetEnable(enable);
    rl_enable_.set_enable(enable, false);
    break;
  case kRSH:
    value_rsh_.SetEnable(enable);
    rsh_enable_.set_enable(enable, false);
    break;
  case kRSL:
    value_rsl_.SetEnable(enable);
    rsl_enable_.set_enable(enable, false);
    break;
  default:
    ASSERT(FALSE); // no exit
  }
}

void AnalogDisturbanceView::OnComponentValueChanged(StressComponent component, double value)
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
    can_h_volt_.set_enable(volt == VOLT_PLUS);
    break;
  case CAN_LOW:
    can_l_volt_.set_enable(volt == VOLT_PLUS);
    break;
  default:
    ASSERT(FALSE);
  }
}

void AnalogDisturbanceView::OnStart()
{
  layout_.EnableWindow(FALSE);
  if (layout_.GetCurSel() == kStandard) {
    value_rh_.SetEnable(false); // value can no change.
    value_rhl_.SetEnable(false);
    value_rl_.SetEnable(false);
    value_rsh_.SetEnable(false);
    value_chl_.SetEnable(false);
    value_rsl_.SetEnable(false);

    rh_enable_.set_can_change(false); // enable state can no change.
    rhl_enable_.set_can_change(false);
    rl_enable_.set_can_change(false);
    rsh_enable_.set_can_change(false);
    chl_enable_.set_can_change(false);
    rsl_enable_.set_can_change(false);
  }
}

void AnalogDisturbanceView::OnStop()
{
  layout_.EnableWindow(TRUE);
  if (layout_.GetCurSel() == kStandard) {
    // restore enable
    OnComponentEnableChanged(kRH, device_->ComponentEnable(kRH));
    OnComponentEnableChanged(kRHL, device_->ComponentEnable(kRHL));
    OnComponentEnableChanged(kRL, device_->ComponentEnable(kRL));
    OnComponentEnableChanged(kRSH, device_->ComponentEnable(kRSH));
    OnComponentEnableChanged(kCHL, device_->ComponentEnable(kCHL));
    OnComponentEnableChanged(kRSL, device_->ComponentEnable(kRSL));
    // restore can_change
    OnLayoutChange();
  }
}

StressComponent AnalogDisturbanceView::ValueGetControlsToStressComponent(ValueGetControls* value_get) {
  if (value_get == &value_rh_) {
    return kRH;
  } else if (value_get == &value_rhl_) {
    return kRHL;
  } else if (value_get == &value_rl_) {
    return kRL;
  } else if (value_get == &value_rsh_) {
    return kRSH;
  } else if (value_get == &value_chl_) {
    return kCHL;
  } else if (value_get == &value_rsl_) {
    return kRSL;
  } else {
    ASSERT(FALSE); // no exist ValueGetControls
  }
}

StressComponent AnalogDisturbanceView::ComponentEnableControlsToStressComponent(ComponentEnableControls* component) {
  if (component == &rh_enable_) {
    return kRH;
  } else if (component == &rhl_enable_) {
    return kRHL;
  } else if (component == &rl_enable_) {
    return kRL;
  } else if (component == &rsh_enable_) {
    return kRSH;
  } else if (component == &chl_enable_) {
    return kCHL;
  } else if (component == &rsl_enable_) {
    return kRSL;
  }else {
    ASSERT(FALSE); // no exist ComponentEnableControls
  }
}

void AnalogDisturbanceView::LayoutControls() {
  CWnd* start_wnd = GetDlgItem(IDC_PICTURE_RH);
  CRect start_rect;
  start_wnd->GetWindowRect(&start_rect);
  ScreenToClient(&start_rect);

  MoveEnableControls(&rh_enable_, start_rect, kRHRect, kRHClickRect);
  MoveEnableControls(&rhl_enable_, start_rect, kRHLRect, kRHLClickRect);
  MoveEnableControls(&rl_enable_, start_rect, kRLRect, kRLClickRect);
  MoveEnableControls(&rsh_enable_, start_rect, kRSHRect, kRSHClickRect);
  MoveEnableControls(&chl_enable_, start_rect, kCHLRect, kCHLClickRect);
  MoveEnableControls(&rsl_enable_, start_rect, kRSLRect, kRSLClickRect);
  MoveEnableControls(&can_h_volt_, start_rect, kCANHVOLTRect, kCANHVOLTClickRect);
  MoveEnableControls(&can_l_volt_, start_rect, kCANLVOLTRect, kCANLVOLTClickRect);
}
