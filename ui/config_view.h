#pragma once

#include "resources/stress_resource.h"

#include "stress/stress_device.h"

class ConfigView : public CFormView
                 , public StressDeviceObserver
{
public:
  ConfigView(StressDevice* device);
  virtual ~ConfigView();

  virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, 
    DWORD dwRequestedStyle, const RECT& rect, CWnd* pParentWnd, 
    UINT nID, CCreateContext* pContext);

private:
  enum { IDD = IDD_CONFIG };

  // implement StressDeviceListener
  virtual void OnComponentEnableChanged(StressComponent component, bool enable) {}
  virtual void OnComponentValueChanged(StressComponent component, double value) {}
  virtual void OnDisturbanceVoltageChanged( CAN_CHNL chnl, DisturbanceVoltage volt);

  virtual void DoDataExchange(CDataExchange* pDX);

  // init the gui there
  void Init();

  afx_msg void OnOscListenPort();
  afx_msg void OnCANHDisturbVolt();
  afx_msg void OnCANLDisturbVolt();
  afx_msg void OnCANBusType();
  DECLARE_MESSAGE_MAP()

private:
  // init GUI trick
  bool init_;

  CComboBox osc_listen_port_;
  CComboBox can_high_dist_volt_;
  CComboBox can_low_dist_volt_;
  CComboBox can_bus_type_;

  // hardware object.
  StressDevice* device_;
};