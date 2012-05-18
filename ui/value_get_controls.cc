#include "stdafx.h"

#include "ui/value_get_controls.h"

BEGIN_MESSAGE_MAP(ValueGetSliderCtrl, CSliderCtrl)
  ON_WM_HSCROLL_REFLECT()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(ValueGetEdit, CXTPEdit)
  ON_WM_CHAR()
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

void ValueGetEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {
  CEdit::OnChar(nChar, nRepCnt, nFlags);
  CString text;
  GetWindowText(text);
  // TODO need to handle wrong
  double value = _tstof(text);
  if (value < min_ || value > max_) {
    value = min_;
  }
  value_get_->ValueChange(value);
}

void ValueGetEdit::SetValue(double value, int precision) {
  ASSERT(min_ <= value && value <= max_);
  CString format;
  format.Format(_T("%%.%df"), precision);
  CString text;
  text.Format(format, value);
  SetWindowText(text);
}

void ValueGetControls::DoDataExchange(CDataExchange* pDX) {
  DDX_Control(pDX, slider_id_, slider_);
  DDX_Control(pDX, edit_id_, edit_);
  DDX_Control(pDX, spin_id_, spin_);
  if (init_ui_ == false) {
    init_ui_ = true;
    // must set Range first, or SetValue will break.
    SetRangeAndStep(init_min_, init_max_, init_step_);
    set_value(init_value_);
  }
}

void ValueGetControls::ValueChange(double value) {
  slider_.SetValue(value);
  edit_.SetValue(value, precision_);
  spin_.SetValue(value);
  if (listener_ != NULL)
    listener_->OnValueChange(this, value);
}
