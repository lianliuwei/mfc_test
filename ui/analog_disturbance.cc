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
}

IMPLEMENT_DYNCREATE(AnalogDisturbanceView, CFormView)

BEGIN_MESSAGE_MAP(AnalogDisturbanceView, CFormView)
  ON_COMMAND(IDC_CHECKBOX_RH, OnEnableCheckBoxClick)
  ON_COMMAND(IDC_CHECKBOX_RHL, OnEnableCheckBoxClick)
  ON_COMMAND(IDC_CHECKBOX_RL, OnEnableCheckBoxClick)
  ON_COMMAND(IDC_CHECKBOX_RSH, OnEnableCheckBoxClick)
  ON_COMMAND(IDC_CHECKBOX_CHL, OnEnableCheckBoxClick)
  ON_COMMAND(IDC_CHECKBOX_RSL, OnEnableCheckBoxClick)
END_MESSAGE_MAP()

AnalogDisturbanceView::AnalogDisturbanceView()
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
    , init_(false) {
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
  
  if (init_ == false) {
    init_ = true;
    Init();
  }
}

void AnalogDisturbanceView::OnValueChange(ValueGetControls* value_get, double value)
{

} 

void AnalogDisturbanceView::OnEnableCheckBoxClick()
{
   value_rh_.SetEnable(rh_enable_.GetChecked() == TRUE);
   value_rhl_.SetEnable(rhl_enable_.GetChecked() == TRUE);
   value_rl_.SetEnable(rl_enable_.GetChecked() == TRUE);
   value_rsh_.SetEnable(rsh_enable_.GetChecked() == TRUE);
   value_chl_.SetEnable(chl_enable_.GetChecked() == TRUE);
   value_rsl_.SetEnable(rsl_enable_.GetChecked() == TRUE);
}

void AnalogDisturbanceView::Init()
{
    rh_enable_.SetChecked(TRUE);
    rhl_enable_.SetChecked(TRUE);
    rl_enable_.SetChecked(TRUE);
    rsh_enable_.SetChecked(TRUE);
    chl_enable_.SetChecked(TRUE);
    rsl_enable_.SetChecked(TRUE);
}
