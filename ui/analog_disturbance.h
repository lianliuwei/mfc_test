#pragma once

#include "resources/stress_resource.h"

#include "ui/value_get_controls.h"
#include "ui/component_enable_controls.h"
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
                            , public ComponentEnableListener
                            , public StressDeviceObserver
{
public:
  AnalogDisturbanceView(StressDevice* device);
  virtual ~AnalogDisturbanceView();;

  virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, 
    DWORD dwRequestedStyle, const RECT& rect, CWnd* pParentWnd, 
    UINT nID, CCreateContext* pContext);

private:
  enum {IDD = IDD_ANALOG_DISTURBANCE};

  // implement ValueGetListener
  virtual void OnValueChange(ValueGetControls* value_get, double value);
  
  // implement ComponentEnableListener
  virtual void OnEnableChange( ComponentEnableControls* component, bool enable);

  // implement StressDeviceObserver
  virtual void OnComponentEnableChanged(StressComponent component, bool enable);
  virtual void OnComponentValueChanged(StressComponent component, double value);
  virtual void OnDisturbanceVoltageChanged( CAN_CHNL chnl, DisturbanceVoltage volt);
  virtual void OnStart();
  virtual void OnStop();

  // init the gui there
  void Init();

  void LayoutControls();

  void set_stress_layout(StressLayout layout);
  StressLayout stress_layout(StressLayout layout) const;

  // void set_chnl_volt(CAN_CHNL);
  bool GetComponentEnable(StressComponent component) const;

  void SetComponentEnable(StressComponent component, 
    ComponentEnableControls* enable_controls, bool enable, bool can_change);

  // helper.
  StressComponent ValueGetControlsToStressComponent(ValueGetControls* value_get);
  StressComponent ComponentEnableControlsToStressComponent(ComponentEnableControls* component);

  virtual void DoDataExchange(CDataExchange* pDX);

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
  
  // enable show and user click
  ComponentEnableControls rh_enable_;
  ComponentEnableControls rhl_enable_;
  ComponentEnableControls rl_enable_;
  ComponentEnableControls rsh_enable_;
  ComponentEnableControls chl_enable_;
  ComponentEnableControls rsl_enable_;
  
  // control to show what the CHNL input volt is.
  ComponentEnableControls can_h_volt_;
  ComponentEnableControls can_l_volt_;

  // select specific layout.
  CComboBox layout_;
 
  // init GUI trick
  bool init_;

  // hardware object.
  StressDevice* device_;
};