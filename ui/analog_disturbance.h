#pragma once

#include "resources/stress_resource.h"

#include "ui/value_get_controls.h"
#include "ui/views/picture_ctrl.h"
#include "stress/stress_device.h"

enum StressLayout {
  kStandard = 0,
  kR_H,
  kR_SH,
  kR_HL_without_R_SH,
  kR_HL_without_R_SL,
  kR_SL,
  kR_L,
  kR_H_and_R_SL,
  kR_H_and_R_L,
  kR_SH_and_R_HL,
  kR_SH_and_R_SL,
  kR_SH_and_R_L,
  kR_HL_and_R_SL,
  kR_HL_and_R_H,
  kR_HL_and_R_L,
  kLayoutSize,
};

class AnalogDisturbanceView : public CFormView
                            , public ValueGetListener
                            , public StressDeviceListener
{
public:
  AnalogDisturbanceView(StressDevice* device);
  virtual ~AnalogDisturbanceView() {};

  virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, 
    DWORD dwRequestedStyle, const RECT& rect, CWnd* pParentWnd, 
    UINT nID, CCreateContext* pContext);

private:
  enum {IDD = IDD_ANALOG_DISTURBANCE};

  // implement ValueGetListener
  virtual void OnValueChange(ValueGetControls* value_get, double value);

  // implement StressDeviceListener
  virtual void OnComponendEnableChanged(StressComponent component, bool enable);
  virtual void OnComponendValueChanged(StressComponent component, double value);
  virtual void OnDisturbanceVoltageChanged( CAN_CHNL chnl, DisturbanceVoltage volt);

  // init the gui there
  void Init();

  void set_stress_layout(StressLayout layout);
  StressLayout stress_layout(StressLayout layout) const;

  // void set_chnl_volt(CAN_CHNL);
  bool GetComponentEnable(StressComponent component) const;

  virtual void DoDataExchange(CDataExchange* pDX);
  
  afx_msg void OnRHEnable();
  afx_msg void OnRHLEnable();
  afx_msg void OnRLEnable();
  afx_msg void OnRSHEnable();
  afx_msg void OnCHLEnable();
  afx_msg void OnRSLEnable();

  afx_msg void OnOscListenPort();
  afx_msg void OnCANHDisturbVolt();
  afx_msg void OnCANLDisturbVolt();
  afx_msg void OnBusType();

  afx_msg void OnLayoutChange();
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
  
  // control to show what the CHNL input volt is.
  CPictureCtrl can_h_volt_;
  CPictureCtrl can_l_volt_;

  // select specific layout.
  CComboBox layout_;

  // Port Option
  CXTPButton osc_listen_port_;
  CXTPButton can_high_dist_volt_;
  CXTPButton can_low_dist_volt_;
  CXTPButton bus_type_;

  // init GUI trick
  bool init_;

  // hardware object.
  StressDevice* device_;
};