#pragma once

#include "resources/stress_resource.h"

#include "ui/value_get_controls.h"

class AnalogDisturbanceView : public CFormView, public ValueGetListener
{
  DECLARE_DYNCREATE(AnalogDisturbanceView)

public:
  AnalogDisturbanceView();
  virtual ~AnalogDisturbanceView() {};

  virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, 
    DWORD dwRequestedStyle, const RECT& rect, CWnd* pParentWnd, 
    UINT nID, CCreateContext* pContext);

private:
  enum {IDD = IDD_ANALOG_DISTURBANCE};

  // implement ValueGetListener
  virtual void OnValueChange(ValueGetControls* value_get, double value);

  // init the gui there
  void Init();
  
  virtual void DoDataExchange(CDataExchange* pDX);
  
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnEnableCheckBoxClick();
  DECLARE_MESSAGE_MAP()

private:
  // controls group
  ValueGetControls value_rh_;
  ValueGetControls value_rhl_;
  ValueGetControls value_rl_;
  ValueGetControls value_rsh_;
  ValueGetControls value_chl_;
  ValueGetControls value_rsl_;

  // for temp test.
  CXTPButton rh_enable_;
  CXTPButton rhl_enable_;
  CXTPButton rl_enable_;
  CXTPButton rsh_enable_;
  CXTPButton chl_enable_;
  CXTPButton rsl_enable_;
  
  // init GUI trick
  bool init_;
};