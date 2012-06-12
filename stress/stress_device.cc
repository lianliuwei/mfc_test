#include "stress/stress_device.h"

#include "base/logging.h"

#include "common/Ini.h"

// TODO this dup with the View. remove this dup.
namespace {
const static double kRMin = 0.0;
const static double kRMax = 10237.5;
const static double kRStep = 2.5;
const static double kCMin = 0.0;
const static double kCMax = 15750.0;
const static double kCStep = 250.0;
const static double kDoubleMin = 0.000001;

const static int kVersionMain = 2;
const static int kVersionSub = 1;
const static TCHAR* kVersionSection = _T("Verison");
const static TCHAR* kMainKey = _T("Main");
const static TCHAR* kSubKey = _T("Sub");

const static TCHAR* kAnalogDisturbSection = _T("AnalogDistConfig");
const static TCHAR* kRHKey = _T("R_H");
const static TCHAR* kRHLKey = _T("R_HL");
const static TCHAR* kRLKey = _T("R_L");
const static TCHAR* kRSHKey = _T("R_SH");
const static TCHAR* kRSLKey = _T("R_SL");
const static TCHAR* kCHLKey = _T("C_HL");
const static TCHAR* kRHEnableKey = _T("R_H_State");
const static TCHAR* kRSHEnableKey = _T("R_SH_State");
const static TCHAR* kRLEnableKey = _T("R_L_State");
const static TCHAR* kRSLEnableKey = _T("R_SL_State");
const static TCHAR* kRHLEnableKey = _T("R_HL_State");
const static TCHAR* kCHLEnableKey = _T("C_HL_State");
const static TCHAR* kLayoutKey = _T("ResLayoutType");

int EnableToConfigKey(bool enable) {
  return enable ? 3 : 1;
}

int LayoutToConfigKey(StressLayout layout) {
  return layout + 1;
}

}

StressDevice::StressDevice() {
  Reset();
}

void StressDevice::Reset() {
  set_start(false); // init as stop.

  SetOscListenPort(CAN_IN);
  SetDisturbanceVoltage(CAN_HIGH, VOLT_MINUS);
  SetDisturbanceVoltage(CAN_LOW, VOLT_MINUS);
  SetCANBusType(HIGH_SPEED);
  
  SetComponentValue(kRH, 0);
  SetComponentValue(kRHL, 0);
  SetComponentValue(kRL, 0);
  SetComponentValue(kRSH, 0);
  SetComponentValue(kCHL, 0);
  SetComponentValue(kRSL, 0);

  SetComponentEnable(kRH, false);
  SetComponentEnable(kRHL, false);
  SetComponentEnable(kRL, false);
  SetComponentEnable(kRSH, false);
  SetComponentEnable(kCHL, false);
  SetComponentEnable(kRSL, false);

  set_stress_layout(kStandard); // after value and enable

  MarkNoChanged(); // init no need to save.
}

void StressDevice::SetComponentValue(StressComponent component,
                                     const double value) {
  if (component == kCHL) { // capacitor
    unsigned char reg_value = 0U;
    CHECK(kCMin <= value && value <= kCMax);
    reg_value = static_cast<unsigned char>(value / kCStep);
    double remain = value - reg_value * kCStep;
    CHECK((-kDoubleMin) < remain && remain < kDoubleMin);
    // set to the hardware
    chl = reg_value;
  } else { // resistor
    unsigned short reg_value = 0U;
    CHECK(kRMin <= value && value <= kRMax);
    reg_value = static_cast<unsigned short>(value / kRStep);
    double remain = value - reg_value * kRStep;
    CHECK((-kDoubleMin) < remain && remain < kDoubleMin);
    // set to the hardware.
    switch (component) {
    case kRH:
      rh = reg_value; break;
    case kRHL:
      rhl = reg_value; break;
    case kRL:
      rl = reg_value; break;
    case kRSH:
      rsh = reg_value; break;
    case kRSL:
      rsl = reg_value; break;
    default:
      NOTREACHED(); // no exist
    }
  }
  NotifyValueChanged(component, value);
  MarkChanged();
  SetToDevice();
}

void StressDevice::SetComponentEnable(StressComponent component,
                                      const bool enable) {
  if (component == kCHL) {
    // set the hardware.
    enable ? chl &= 0x3F : chl |= 0x40;
  } else {
    // set the hardware.
    switch (component) {
    case kRH:
      enable ? rh &= 0x7FFF : rh |= 0x8000; break;
    case kRHL:
      enable ? rhl &= 0x7FFF : rhl |= 0x8000; break;
    case kRL:
      enable ? rl &= 0x7FFF : rl |= 0x8000; break;
    case kRSH:
      enable ? rsh &= 0x7FFF : rsh |= 0x8000; break;
    case kRSL:
      enable ? rsl &= 0x7FFF : rsl |= 0x8000; break;
    default:
      NOTREACHED(); // no exist
    }
  }
  NotifyEnableChanged(component, enable);
  MarkChanged();
  SetToDevice();
}

double StressDevice::ComponentValue(StressComponent component) {
  if (component == kCHL) {
    // read the hardware.
    unsigned char value = chl & 0x3F;

    return static_cast<double>(value) * kCStep;
  } else {
    unsigned short value = 0U;
    // read the hardware.
    switch (component) {
    case kRH:
      value = rh & 0x7FFF; break;
    case kRHL:
      value = rhl & 0x7FFF; break;
    case kRL:
      value = rl & 0x7FFF; break;
    case kRSH:
      value = rsh & 0x7FFF; break;
    case kRSL:
      value = rsl & 0x7FFF; break;
    default:
      NOTREACHED(); // no exist
    }

    return static_cast<double>(value) * kRStep;
  }
}

bool StressDevice::ComponentEnable(StressComponent component) {
  if (component == kCHL) {
    // read the hardware
    bool enable = !((chl & 0x40) != 0U);

    return enable;
  } else {
    bool enable = false;
    // read the hardware.
    switch (component) {
    case kRH:
      enable = !((rh & 0x8000) != 0U); break;
    case kRHL:
      enable = !((rhl & 0x8000) != 0U); break;
    case kRL:
      enable = !((rl & 0x8000) != 0U); break;
    case kRSH:
      enable = !((rsh & 0x8000) != 0U); break;
    case kRSL:
      enable = !((rsl & 0x8000) != 0U); break;
    default:
      NOTREACHED(); // no exist
    }

    return enable;
  }
}

void StressDevice::SetOscListenPort(OscListenPort listen_port) {
  cfg_.portcfg3 = (listen_port != CAN_IN);
  MarkChanged();
  SetToDevice();
}

OscListenPort StressDevice::GetOscListenPort() {
  return !cfg_.portcfg3 ? CAN_IN : CAN_OUT;
}

void StressDevice::SetDisturbanceVoltage(CAN_CHNL chnl,
                                         DisturbanceVoltage volt) {
  if (chnl == CAN_HIGH) {
    cfg_.portcfg4 = (volt != VOLT_PLUS);
  } else if (chnl == CAN_LOW) {
    cfg_.portcfg5 = (volt != VOLT_PLUS);
  } else {
    NOTREACHED();
  }
  NotifyDisturbanceVoltageChanged(chnl, volt);
  MarkChanged();
  SetToDevice();
}

DisturbanceVoltage StressDevice::GetDisturbanceVoltage(CAN_CHNL chnl) {
  if (chnl == CAN_HIGH) {
    return !cfg_.portcfg4 ? VOLT_PLUS : VOLT_MINUS;
  } else if (chnl == CAN_LOW) {
    return !cfg_.portcfg5 ? VOLT_PLUS : VOLT_MINUS;
  } else {
    NOTREACHED();
    return VOLT_MINUS;
  }
}

void StressDevice::SetCANBusType(CANBusType type) {
  cfg_.portcfg6 = (type != HIGH_SPEED);
  MarkChanged();
  SetToDevice();
}

CANBusType StressDevice::GetCANBusType() {
  return !cfg_.portcfg6 ? HIGH_SPEED : FAULT_TOLERANT;
}

void StressDevice::set_stress_layout(StressLayout layout) {
  CHECK(0 <= layout && layout < kLayoutSize);
  if (layout_ == layout)
    return;
  layout_ = layout;
  MarkChanged();
  NotifyLayoutChanged(layout_);
}

void StressDevice::Save(FilePath& file) {
  CIni ini(file.value().c_str());
  // version section.
  ini.WriteInt(kVersionSection, kMainKey, kVersionMain);
  ini.WriteInt(kVersionSection, kSubKey, kVersionSub);
  // analog disturbance config section.
  ini.WriteDouble(kAnalogDisturbSection, kRHKey, ComponentValue(kRH));
  ini.WriteDouble(kAnalogDisturbSection, kRHLKey, ComponentValue(kRHL));
  ini.WriteDouble(kAnalogDisturbSection, kRLKey, ComponentValue(kRL));
  ini.WriteDouble(kAnalogDisturbSection, kRSHKey, ComponentValue(kRSH));
  ini.WriteDouble(kAnalogDisturbSection, kRSLKey, ComponentValue(kRSL));
  ini.WriteDouble(kAnalogDisturbSection, kCHLKey, ComponentValue(kCHL));

  ini.WriteInt(kAnalogDisturbSection, kRHEnableKey, 
    EnableToConfigKey(ComponentEnable(kRH)));
  ini.WriteInt(kAnalogDisturbSection, kRSHEnableKey, 
    EnableToConfigKey(ComponentEnable(kRSH)));
  ini.WriteInt(kAnalogDisturbSection, kRLEnableKey, 
    EnableToConfigKey(ComponentEnable(kRL)));
  ini.WriteInt(kAnalogDisturbSection, kRSLEnableKey, 
    EnableToConfigKey(ComponentEnable(kRSL)));
  ini.WriteInt(kAnalogDisturbSection, kRHLEnableKey, 
    EnableToConfigKey(ComponentEnable(kRHL)));
  ini.WriteInt(kAnalogDisturbSection, kCHLEnableKey, 
    EnableToConfigKey(ComponentEnable(kCHL)));

  ini.WriteInt(kAnalogDisturbSection, kLayoutKey, LayoutToConfigKey(stress_layout()));

  MarkNoChanged();
}
