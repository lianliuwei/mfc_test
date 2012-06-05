#pragma once

#include "base/observer_list.h"

enum StressComponent {
  kRH = 0,
  kRHL,
  kRL,
  kRSH,
  kCHL,
  kRSL,
  kComponentSize,
};

enum OscListenPort {
  CAN_IN,
  CAN_OUT,
};

enum CAN_CHNL {
  CAN_HIGH,
  CAN_LOW,
};
enum DisturbanceVoltage {
  VOLT_PLUS,
  VOLT_MINUS,
};

enum CANBusType {
  HIGH_SPEED,
  FAULT_TOLERANT,
};

struct PORTCFG
{
  WORD portcfg0:1; //1—启用示波器；0—禁用示波器
  WORD portcfg1:1; //1—	使用终端电阻；0—禁用终端电阻
  WORD portcfg2:1; //0—数学差分；1—硬件差分
  WORD portcfg3:1;//示波器监测端口选择，0—CAN_IN；1—CAN_OUT
  WORD portcfg4:1;//CANH干扰电压选择，0—VDIS+；1—VDIS-
  WORD portcfg5:1;//CANL干扰电压选择，0—VDIS+；1—VDIS-
  WORD portcfg6:1;//全速CAN和容错CAN选择，0—全速CAN，1—容错CAN
  WORD resered:9; //保留位
};

class StressDeviceObserver {
public:
  virtual void OnComponentEnableChanged(StressComponent component, bool enable) = 0;

  virtual void OnComponentValueChanged(StressComponent component, double value) = 0;

  virtual void OnDisturbanceVoltageChanged(CAN_CHNL chnl, DisturbanceVoltage volt) = 0;
};

// response for set the stress Device.
// stress Device must live long then the View.
// INFO no the input limit and restrict by the AnalogDisturbanceView.
// if has another UI to set the Stress Device, need to move the restrict here.
class StressDevice
{
public:
  StressDevice(StressDeviceObserver* listener);
  ~StressDevice() {};

public:
  // set adjustable Resistor or Capacitor.
  void SetComponentValue(StressComponent component, const double value);

  // set Enable
  // INFO the vector stress exe res and the config file show the 
  // component no only two status.
  void SetComponentEnable(StressComponent component, const bool enable);

  // Get adjustable Resistor or Capacitor value
  // Resistor is ohm, Capacitor is pF
  double ComponentValue(StressComponent component);

  // Get Enable
  bool ComponentEnable(StressComponent component);

  // Stress Port Cfg
  void SetOscListenPort(OscListenPort listen_port);
  OscListenPort GetOscListenPort();

  void SetDisturbanceVoltage(CAN_CHNL chnl, DisturbanceVoltage volt);
  DisturbanceVoltage GetDisturbanceVoltage(CAN_CHNL chnl);

  void SetCANBusType(CANBusType type);
  CANBusType GetCANBusType();

  void AddObserver(StressDeviceObserver* observer) {
    observer_list_.AddObserver(observer);
  }

  void RemoveObserver(StressDeviceObserver* observer) {
    observer_list_.RemoveObserver(observer);
  }
private:
  void NotifyEnableChanged(StressComponent component, bool enable) {
    FOR_EACH_OBSERVER(StressDeviceObserver, observer_list_, 
      OnComponentEnableChanged(component, enable));
  }

  void NotifyValueChanged(StressComponent component, double value) {
    FOR_EACH_OBSERVER(StressDeviceObserver, observer_list_, 
      OnComponentValueChanged(component, value));
  }

  void NotifyDisturbanceVoltageChanged(CAN_CHNL chnl, DisturbanceVoltage volt) {
    FOR_EACH_OBSERVER(StressDeviceObserver, observer_list_,
      OnDisturbanceVoltageChanged(chnl, volt));
  }

private:
  ObserverList<StressDeviceObserver> observer_list_;
  
  BYTE chl;//终端可调电容，调节范围：0nF~15.75nF(15750pF)，步进：250pF
  WORD rhl;//终端可调电阻，调节范围：0Ω~10.2375kΩ(10237.5Ω)，步进：2.5Ω
  WORD rsl;//CANL串联电阻，调节范围：0Ω~10.2375kΩ(10237.5Ω)，步进：2.5Ω
  WORD rsh;//CANH串联电阻，调节范围：0Ω~10.2375kΩ(10237.5Ω)，步进：2.5Ω
  WORD rh;//CANH上拉电阻，调节范围：0Ω~10.2375kΩ(10237.5Ω)，步进：2.5Ω
  WORD rl;//CANL上拉电阻，调节范围：0Ω~10.2375kΩ(10237.5Ω)，步进：2.5Ω

  PORTCFG cfg_;
};