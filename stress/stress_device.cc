#include "stress/stress_device.h"

#include "base/logging.h"

#include "common/Ini.h"

#include "stress/stress_constants.h"

namespace {
static const double kDoubleMin = 0.000001;

static const int kVersionMain = 2;
static const int kVersionSub = 1;
static const TCHAR* kVersionSection = _T("Version");
static const TCHAR* kMainKey = _T("Main");
static const TCHAR* kSubKey = _T("Sub");

static const TCHAR* kAnalogDisturbSection = _T("AnalogDistConfig");
static const TCHAR* kRHKey = _T("R_H");
static const TCHAR* kRHLKey = _T("R_HL");
static const TCHAR* kRLKey = _T("R_L");
static const TCHAR* kRSHKey = _T("R_SH");
static const TCHAR* kRSLKey = _T("R_SL");
static const TCHAR* kCHLKey = _T("C_HL");
static const TCHAR* kRHEnableKey = _T("R_H_State");
static const TCHAR* kRSHEnableKey = _T("R_SH_State");
static const TCHAR* kRLEnableKey = _T("R_L_State");
static const TCHAR* kRSLEnableKey = _T("R_SL_State");
static const TCHAR* kRHLEnableKey = _T("R_HL_State");
static const TCHAR* kCHLEnableKey = _T("C_HL_State");
static const TCHAR* kLayoutKey = _T("ResLayoutType");

static const int kEnableKeyDefault = 1;
static const int kLayoutKeyDefault = 1;

int EnableToConfigKey(bool enable) {
  return enable ? 3 : 1;
}

bool ConfigKeyToEnable(int key) {
   if (key == 3)
     return true;
   LOG_IF(ERROR, key != 1) << "no support key value "<< key <<". treat as 1 (false)";
   return false; // other return 
}

StressLayout ConfigKeyToLayout(int key) {
  if (key < 1 || key > kLayoutSize) {
    LOG(ERROR) << "no support key value " << key << ". treat as 1 (kStandard)";
    return kStandard;
  }
  return static_cast<StressLayout>(key -1);
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

void StressDevice::Load( FilePath& file )
{
  CIni ini(file.value().c_str());
  
  int main = ini.GetInt(kVersionSection, kMainKey, 0);
  int sub = ini.GetInt(kVersionSection, kSubKey, 0);
  if (main != kVersionMain && sub != kVersionSub) {
    LOG(ERROR) << "version of load file is no support.";
    return; 
  }
  SetComponentValue(kRH, ini.GetDouble(kAnalogDisturbSection, kRHKey, 0.0));
  SetComponentValue(kRHL, ini.GetDouble(kAnalogDisturbSection, kRHLKey, 0.0));
  SetComponentValue(kRL, ini.GetDouble(kAnalogDisturbSection, kRLKey, 0.0));
  SetComponentValue(kRSH, ini.GetDouble(kAnalogDisturbSection, kRSHKey, 0.0));
  SetComponentValue(kRSL, ini.GetDouble(kAnalogDisturbSection, kRSLKey, 0.0));
  SetComponentValue(kCHL, ini.GetDouble(kAnalogDisturbSection, kCHLKey, 0.0));

  SetComponentEnable(kRH, 
    ConfigKeyToEnable(ini.GetInt(kAnalogDisturbSection, kRHEnableKey, kEnableKeyDefault)));
  SetComponentEnable(kRHL, 
    ConfigKeyToEnable(ini.GetInt(kAnalogDisturbSection, kRHLEnableKey, kEnableKeyDefault)));
  SetComponentEnable(kRL, 
    ConfigKeyToEnable(ini.GetInt(kAnalogDisturbSection, kRLEnableKey, kEnableKeyDefault)));
  SetComponentEnable(kRSH, 
    ConfigKeyToEnable(ini.GetInt(kAnalogDisturbSection, kRSHEnableKey, kEnableKeyDefault)));
  SetComponentEnable(kRSL, 
    ConfigKeyToEnable(ini.GetInt(kAnalogDisturbSection, kRSLEnableKey, kEnableKeyDefault)));
  SetComponentEnable(kCHL, 
    ConfigKeyToEnable(ini.GetInt(kAnalogDisturbSection, kCHLEnableKey, kEnableKeyDefault)));

  set_stress_layout(
    ConfigKeyToLayout(ini.GetInt(kAnalogDisturbSection, kLayoutKey, kLayoutKeyDefault)));

  MarkNoChanged();
}
