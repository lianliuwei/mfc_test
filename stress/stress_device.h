#pragma once

// TODO add notify the port change or the value change or enable change.

enum StressComponent {
  kRH,
  kRHL,
  kRL,
  kRSH,
  kCHL,
  kRSL,
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
  WORD portcfg0:1; //1������ʾ������0������ʾ����
  WORD portcfg1:1; //1��	ʹ���ն˵��裻0�������ն˵���
  WORD portcfg2:1; //0����ѧ��֣�1��Ӳ�����
  WORD portcfg3:1;//ʾ�������˿�ѡ��0��CAN_IN��1��CAN_OUT
  WORD portcfg4:1;//CANH���ŵ�ѹѡ��0��VDIS+��1��VDIS-
  WORD portcfg5:1;//CANL���ŵ�ѹѡ��0��VDIS+��1��VDIS-
  WORD portcfg6:1;//ȫ��CAN���ݴ�CANѡ��0��ȫ��CAN��1���ݴ�CAN
  WORD resered:9; //����λ
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

private:
  BYTE chl;//�ն˿ɵ����ݣ����ڷ�Χ��0nF~15.75nF(15750pF)��������250pF
  WORD rhl;//�ն˿ɵ����裬���ڷ�Χ��0��~10.2375k��(10237.5��)��������2.5��
  WORD rsl;//CANL�������裬���ڷ�Χ��0��~10.2375k��(10237.5��)��������2.5��
  WORD rsh;//CANH�������裬���ڷ�Χ��0��~10.2375k��(10237.5��)��������2.5��
  WORD rh;//CANH�������裬���ڷ�Χ��0��~10.2375k��(10237.5��)��������2.5��
  WORD rl;//CANL�������裬���ڷ�Χ��0��~10.2375k��(10237.5��)��������2.5��

  PORTCFG cfg_;
};