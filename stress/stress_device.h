#pragma once

#include "base/observer_list.h"
#include "base/file_path.h"

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

struct PORTCFG
{
  WORD portcfg0:1; //1������ʾ������0������ʾ����
  WORD portcfg1:1; //1��	ʹ���ն˵��裻0�������ն˵���
  WORD portcfg2:1; //0����ѧ��֣�1��Ӳ�����
  WORD portcfg3:1;//ʾ�������˿�ѡ��0��CAN_IN��1��CAN_OUT
  WORD portcfg4:1;//CANH���ŵ�ѹѡ��0��VDIS+��1��VDIS-
  WORD portcfg5:1;//CANL���ŵ�ѹѡ��0��VDIS+��1��VDIS-
  WORD portcfg6:1;//ȫ��CAN���ݴ�CANѡ��0��ȫ��CAN��1���ݴ�CAN
  WORD resered:9; //����λ
};

class StressDeviceObserver {
public:
  // one on OnStart one OnStop
  virtual void OnStart() = 0;
  
  virtual void OnStop() = 0;

  virtual void OnComponentEnableChanged(StressComponent component, bool enable) = 0;

  virtual void OnComponentValueChanged(StressComponent component, double value) = 0;

  virtual void OnDisturbanceVoltageChanged(CAN_CHNL chnl, DisturbanceVoltage volt) = 0;

  virtual void OnStressLayoutChanged(StressLayout layout) = 0;
};

// response for set the stress Device.
// stress Device must live long then the View.
// INFO no the input limit and restrict by the AnalogDisturbanceView.
// if has another UI to set the Stress Device, need to move the restrict here.
class StressDevice
{
public:
  StressDevice();
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


  void set_stress_layout(StressLayout layout);
  StressLayout stress_layout() const {
    return layout_;
  }

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

  bool start() {
    return start_;
  }

  bool connect() {
    return false;
  };

  void set_start(bool start) {
    if (start == start_)
      return;
    start_ = start;
    NotifyStartChanged(start_);
    SetToDevice();
  }

  void Save(FilePath& file);
  void Load(FilePath& file) {}
  void Reset();
  // changed for last save.
  bool Changed() { return changed_; }

private:
  void SetToDevice() {
    if (!start_) // only in start state set to the device.
      return;
    // TODO add set to Device
  }
  
  void MarkChanged() { changed_ = true; }
  void MarkNoChanged() { changed_ = false;}
  void NotifyEnableChanged(StressComponent component, bool enable) {
    FOR_EACH_OBSERVER(StressDeviceObserver, observer_list_, 
      OnComponentEnableChanged(component, enable));
  }

  void NotifyValueChanged(StressComponent component, double value) {
    FOR_EACH_OBSERVER(StressDeviceObserver, observer_list_, 
      OnComponentValueChanged(component, value));
  }

  void NotifyLayoutChanged(StressLayout layout) {
    FOR_EACH_OBSERVER(StressDeviceObserver, observer_list_,
      OnStressLayoutChanged(layout));
  }
  void NotifyDisturbanceVoltageChanged(CAN_CHNL chnl, DisturbanceVoltage volt) {
    FOR_EACH_OBSERVER(StressDeviceObserver, observer_list_,
      OnDisturbanceVoltageChanged(chnl, volt));
  }

  void NotifyStartChanged(bool start) {
    if (start) {
      FOR_EACH_OBSERVER(StressDeviceObserver, observer_list_, OnStart());
    } else {
      FOR_EACH_OBSERVER(StressDeviceObserver, observer_list_, OnStop());
    }
  }

private:
  ObserverList<StressDeviceObserver> observer_list_;
  
  BYTE chl;//�ն˿ɵ����ݣ����ڷ�Χ��0nF~15.75nF(15750pF)��������250pF
  WORD rhl;//�ն˿ɵ����裬���ڷ�Χ��0��~10.2375k��(10237.5��)��������2.5��
  WORD rsl;//CANL�������裬���ڷ�Χ��0��~10.2375k��(10237.5��)��������2.5��
  WORD rsh;//CANH�������裬���ڷ�Χ��0��~10.2375k��(10237.5��)��������2.5��
  WORD rh;//CANH�������裬���ڷ�Χ��0��~10.2375k��(10237.5��)��������2.5��
  WORD rl;//CANL�������裬���ڷ�Χ��0��~10.2375k��(10237.5��)��������2.5��

  PORTCFG cfg_;

  StressLayout layout_;
  bool start_;
  bool changed_;
};