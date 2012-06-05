#pragma once

#include <cmath>

#ifndef UDM_SETPOS32
#define CUSTOM_POS32_FUN
#define UDM_SETPOS32            (WM_USER+113)
#define UDM_GETPOS32            (WM_USER+114)
#endif

class ValueGetControls;

class ValueGetListener
{
public:
  virtual void OnValueChange(ValueGetControls* value_get, double value) = 0;
};

class ValueGetSliderCtrl : public CSliderCtrl
{
public:
  ValueGetSliderCtrl(ValueGetControls* value_get, int scale)
    : value_get_(value_get)
    , scale_(scale) {
      ASSERT(value_get != NULL);
      ASSERT(scale != 0);
  }
  void SetValue(double pos) {
    // TODO check if the cast is overflow
    int scale_pos = static_cast<int>(pos * scale_); 
    SetPos(scale_pos);
  }

  double GetValue() {
    int pos = GetPos();
    return static_cast<double>(pos)/scale_;
  }
  void SetRangeAndStep(double min, double max, double step);

private: 
  afx_msg void HScroll(UINT nSBCode, UINT nPos);
  DECLARE_MESSAGE_MAP()

private:
  ValueGetControls* value_get_;

  // CSliderCtrl only accept int pos so need to scale the value up 
  // if value has decimals.
  // TODO using value/step to replace.
  int scale_;
};

// must using the 32 API, the 16 API desgin is total braindamage.
// you set 0, and you get 65535.
class ValueGetSpinCtrl : public CXTPSpinButtonCtrl
{
public:
  ValueGetSpinCtrl(ValueGetControls* value_get, int scale)    
    : value_get_(value_get)
    , scale_(scale) {
      ASSERT(value_get != NULL);
      ASSERT(scale != 0);
  }

  void SetValue(double pos) {
    // TODO check if the cast is overflow
    int scale_pos = static_cast<int>(pos * scale_); 
    SetPos32(scale_pos);
  }
  double GetValue() {
    int pos = GetPos32();
    return pos/scale_;
  }

  void SetRangeAndStep(double min, double max, double step);

private:
  // I HATE so many control macro
#ifdef CUSTOM_POS32_FUN
  // copy from MFC afxcmn.inl
  int GetPos32(LPBOOL lpbError = NULL) const {
    ASSERT(::IsWindow(m_hWnd)); 
    return (int)::SendMessage(m_hWnd, UDM_GETPOS32, 0, (LPARAM)lpbError); 
  }
  int SetPos32(int nPos){ 
    ASSERT(::IsWindow(m_hWnd)); 
    return (int)::SendMessage(m_hWnd, UDM_SETPOS32, 0, (LPARAM)nPos); 
  }
#endif

  afx_msg void VScroll(UINT nSBCode, UINT nPos);
  DECLARE_MESSAGE_MAP()

private:
  ValueGetControls* value_get_;

  // CXTPSpinButtonCtrl only accept int pos so need to scale the value up 
  // if value has decimals.
  int scale_;
};

// TODO using validate edit to limit the using input
class ValueGetEdit : public CXTPEdit
{
public:
  ValueGetEdit(ValueGetControls* value_get)
    : value_get_(value_get) {
      ASSERT(value_get != NULL);
  }

  double GetValue() {
    CString text;
    GetWindowText(text);
    // TODO need to handle wrong
    return _tstof(text);
  }

  void SetValue(double value, int precision);

  void SetRangeAndStep(double min, double max, double step) {
    ASSERT(max > min);
    ASSERT(step > 0);
    min_ = min;
    max_ = max;
    step_ = step;
  }

private:
  afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
  DECLARE_MESSAGE_MAP()

private:
  ValueGetControls* value_get_;
  double max_;
  double min_;
  double step_;
};

// helper class for value get.
// use the class to get and set value quickly
// listener must live longer the the ValueGetControls
class ValueGetControls {
public:
  ValueGetControls(unsigned int slider_id, 
                   unsigned int edit_id, 
                   unsigned int spin_id,
                   double min,
                   double max,
                   double step,
                   double value,
                   int precision,
                   ValueGetListener* listener) 
    : slider_id_(slider_id)
    , edit_id_(edit_id)
    , spin_id_(spin_id)
    , init_min_(min)
    , init_max_(max)
    , step_(step)
    , init_value_(value)
    , precision_(precision)
    , listener_(listener)
    , slider_(this, pow(10.0, static_cast<int>(precision)))
    , edit_(this)
    , spin_(this, pow(10.0, static_cast<int>(precision)))
    , init_ui_(false) {
      ASSERT(init_max_ > init_min_);
      ASSERT(step_);
      ASSERT(init_min_ <= init_value_ && init_value_ <= init_max_);
      ASSERT(precision_ >= 0);
  }


  void DoDataExchange(CDataExchange* pDX);

  double value() {
    return slider_.GetValue();
  }
  void set_value(double value) {
    double round_value = step_ * static_cast<int>(value / step_);
    slider_.SetValue(round_value);
    edit_.SetValue(round_value, precision_);
    spin_.SetValue(round_value);
  }

  void ValueChange(double val);

  void SetRangeAndStep(double min, double max, double step) {
    ASSERT(max > min);
    ASSERT(step > 0);
    slider_.SetRangeAndStep(min, max, step);
    edit_.SetRangeAndStep(min, max, step);
    spin_.SetRangeAndStep(min, max, step);
  }

  void SetEnable(bool enable) {
    BOOL e = enable ? TRUE : FALSE;
    slider_.EnableWindow(e);
    edit_.EnableWindow(e);
    spin_.EnableWindow(e);
  }

public:
  // three control to Get the Value.
  ValueGetSliderCtrl slider_;
  ValueGetEdit edit_;
  ValueGetSpinCtrl spin_;

  // need id to subclass.
  const unsigned int slider_id_;
  const unsigned int edit_id_;
  const unsigned int spin_id_;

  // notify when the value change.
  ValueGetListener* listener_;

  // range and step to limit the value
  const double init_max_;
  const double init_min_;
  const double step_;
  const double init_value_;

  // need to set ui when get HWND
  bool init_ui_;

  // how many decimal need to have
  int precision_;
};

#ifdef CUSTOM_POS32_FUN
#undef CUSTOM_POS32_FUN
#undef UDM_SETPOS32
#undef UDM_GETPOS32
#endif
