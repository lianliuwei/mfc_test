#ifndef OSC_COMMAND_IDS_H_
#define OSC_COMMAND_IDS_H_
#pragma once

// base Osc operate

// the ON_OFF has run and stop status, when the device is no online the command
// is no enable, so this button has three state. using START and STOP two
// command will face the notify order problem.
// command param is a bool mean the run and stop status.
#define IDC_OSC_ON_OFF  11

// wave control
#define IDC_CHNL_WAVE_VOLT_RANGE 12
#define IDC_CHNL_WAVE_VOLT_OFFSET 13
#define IDC_CHNL_WAVE_COUPLING 16
#define IDC_TIME_RANGE 18
#define IDC_TIME_OFFSET 19

// show operate
#define IDC_WNDS_MODEL 47
#define IDC_SHOW_WAVE 31
#define IDC_SHOW_FFT 44

// advanced operate
#define IDC_AUTOSCALE 41
#define IDC_CALIBRATE 34 //校准 外部和内部两种
#define IDC_FACTORY_TEST 40
#define IDC_FFT_WAVE 48
#define IDC_FFT_SCALETYPE 49
#define IDC_TIME_MEASURE 42
#define IDC_VOLT_MEASURE 43
#define IDC_EXPORT_WAVE_DATA 

// trig operate
#define IDC_TRIG_SOURCE 20  //触发源
#define IDC_TRIG_TYPE 21 //触发类型
#define IDC_TRIG_MODE 22 //触发方式
#define IDC_TRIG_SENS 23 //触发灵敏度
#define IDC_TRIG_AUTO_TIME 24 //自动时间
#define IDC_TRIG_VOL 25 //触发电平
#define IDC_TRIG_CMP 26 //比较符
#define IDC_TRIG_TIME_PARAM 27 //时间参数

// port operate
#define IDC_PORT_OSC 36
#define IDC_PORT_RESIST 37
#define IDC_PORT_DIFF 38

#endif // OSC_COMMAND_IDS_H_