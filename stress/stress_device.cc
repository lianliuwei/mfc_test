#include "stdafx.h"

#include "stress/stress_device.h"

#include "base/logging.h"

// TODO this dup with the View. remove this dup.
namespace {
const static double kRMin = 0.0;
const static double kRMax = 10237.5;
const static double kRStep = 2.5;
const static double kCMin = 0.0;
const static double kCMax = 15750.0;
const static double kCStep = 250.0;
const static double kDoubleMin = 0.000001;
}

StressDevice::StressDevice(StressDeviceObserver* listener) {
  cfg_.portcfg3 = false;
  cfg_.portcfg4 = true;
  cfg_.portcfg5 = true;
  cfg_.portcfg6 = false;
  chl = 0x41;
  rhl = 0x0001;
  rsl = 0x0002;
  rsh = 0x0003;
  rh = 0x0004;
  rl = 0x8005;
  start_ = false; // init as stop.
}

void StressDevice::SetComponentValue(StressComponent component,
                                     const double value) {
  // no enable no set the value.
  if (ComponentEnable(component) == false)
    return;
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
      NOTREACHED();; // no exit
    }
  }
  NotifyValueChanged(component, value);
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
      ASSERT(FALSE); // no exit
    }
  }
  NotifyEnableChanged(component, enable);
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
      ASSERT(FALSE); // no exit
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
      ASSERT(FALSE); // no exit
    }

    return enable;
  }
}

void StressDevice::SetOscListenPort(OscListenPort listen_port) {
  cfg_.portcfg3 = (listen_port != CAN_IN);
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
    ASSERT(FALSE);
  }
  NotifyDisturbanceVoltageChanged(chnl, volt);
}

DisturbanceVoltage StressDevice::GetDisturbanceVoltage(CAN_CHNL chnl) {
  if (chnl == CAN_HIGH) {
    return !cfg_.portcfg4 ? VOLT_PLUS : VOLT_MINUS;
  } else if (chnl == CAN_LOW) {
    return !cfg_.portcfg5 ? VOLT_PLUS : VOLT_MINUS;
  } else {
    ASSERT(FALSE);
    return VOLT_MINUS;
  }
}

void StressDevice::SetCANBusType(CANBusType type) {
  cfg_.portcfg6 = (type != HIGH_SPEED);
}

CANBusType StressDevice::GetCANBusType() {
  return !cfg_.portcfg6 ? HIGH_SPEED : FAULT_TOLERANT;
}
