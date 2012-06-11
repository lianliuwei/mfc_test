#include "stdafx.h"

#include "ui/value_get_controls.h"

namespace {
static const char* kValueFullMatch = "^[-+]?\\d+(\\.\\d+)?$";
static const char* kValuePartMatch = "^[-+]?((\\d*)|(\\d+\\.)|(\\d+\\.\\d+))$";
static const COLORREF kErrorColor = RGB(255, 148, 148);
}

BEGIN_MESSAGE_MAP(ValueGetSliderCtrl, CSliderCtrl)
  ON_WM_HSCROLL_REFLECT()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(ValueGetEdit, ValidatingEdit<CEdit>)
  ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(ValueGetSpinCtrl, CXTPSpinButtonCtrl)
  ON_WM_VSCROLL_REFLECT()
END_MESSAGE_MAP()

void ValueGetSliderCtrl::HScroll(UINT nSBCode, UINT nPos) {
  if (nSBCode == SB_ENDSCROLL)
    return;
  int pos = GetPos();
  value_get_->ValueChange(static_cast<double>(pos) / scale_); // scale back.
}

void ValueGetSliderCtrl::SetRangeAndStep(double min, double max, double step) {
  ASSERT(max > min);
  ASSERT(step > 0);
  int nMin = static_cast<int>(min * scale_); 
  int nMax = static_cast<int>(max * scale_); 
  int nStep = static_cast<int>(step * scale_); 
  SetRange(nMin, nMax);
  SetTicFreq(nStep);
}

// double ValueGetSliderCtrl::GetStep() {
//   GetTic()
// }

void ValueGetSpinCtrl::VScroll(UINT nSBCode, UINT nPos) {
  if (nSBCode == SB_ENDSCROLL)
    return;
  int pos = GetPos32();
  value_get_->ValueChange(static_cast<double>(pos) / scale_); // scale back.
}

void ValueGetSpinCtrl::SetRangeAndStep(double min, double max, double step) {
  ASSERT(max > min);
  ASSERT(step > 0);
  int nMin = static_cast<int>(min * scale_); 
  int nMax = static_cast<int>(max * scale_); 
  UINT nStep = static_cast<UINT>(step * scale_); 
  SetRange32(nMin, nMax);
  UDACCEL accel = { 1, nStep };
  SetAccel(1, &accel);
}

void ValueGetEdit::SetValue(double value, int precision) {
  ASSERT(min_ <= value && value <= max_);
  CString format;
  format.Format(_T("%%.%df"), precision);
  CString text;
  text.Format(format, value);
  SetWindowText(text);
}

bool ValueGetEdit::SemanticCheck(const CString &strText, CString *pstrErrorMsg /*= 0*/)
{
  double value = _tstof(strText);
  if (value < min_) {
    if (pstrErrorMsg) *pstrErrorMsg = _T("value is too small.");
    return false;
  }
  if (value > max_) {
    if (pstrErrorMsg) *pstrErrorMsg = _T("value is too large.");
    return false;
  }
  return true;
}

void ValueGetEdit::OnKillFocus( CWnd* pNewWnd )
{
  RegexValidatingEdit::OnKillFocus(pNewWnd);
  if (IsInputValid (false)) {
    CString text;
    GetWindowText(text);
    double value = _tstof(text);
    value_get_->set_value(value);
  }
}

ValueGetEdit::ValueGetEdit( ValueGetControls* value_get ) 
  : RegexValidatingEdit(kValueFullMatch, kValuePartMatch)
  , value_get_(value_get) {
  ASSERT(value_get != NULL);
  SetBackgroundColourError(kErrorColor);
}



void ValueGetControls::DoDataExchange(CDataExchange* pDX) {
  DDX_Control(pDX, slider_id_, slider_);
  DDX_Control(pDX, edit_id_, edit_);
  DDX_Control(pDX, spin_id_, spin_);
  if (init_ui_ == false) {
    init_ui_ = true;
    // must set Range first, or SetValue will break.
    SetRangeAndStep(init_min_, init_max_, step_);
    set_value(init_value_);
  }
}

void ValueGetControls::ValueChange(double val) {
  set_value(val);
  if (listener_ != NULL)
    listener_->OnValueChange(this, value());
}

void ValueGetControls::SetRangeAndStep(double min, double max, double step) {
  ASSERT(max > min);
  ASSERT(step > 0);
  slider_.SetRangeAndStep(min, max, step);
  edit_.SetRangeAndStep(min, max, step);
  spin_.SetRangeAndStep(min, max, step);
  CString tooltip;
  tooltip.Format(_T("type num in range(%g, %g)"), min, max);
  edit_.CreateToolTip(edit_.GetParent(), tooltip);
}
