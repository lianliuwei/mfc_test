#include "StdAfx.h"
#include "resources/resource.h"

#include "ui/ribbon_osc_config_bar.h"

#include "base/logging.h"

#include "ui/osc_command_ids.h"
#include "ui/on_off_button.h"
#include "ui/quantity_edit.h"
#include "ui/command_button.h"

namespace {

static const int kControlWidth = 120;

void SetControlToManualUpdate(CXTPControl* control) {
  control->SetFlags(control->GetFlags()|xtpFlagManualUpdate);
}

void SetControlShowStyle(CXTPControl* control) {
  control->SetWidth(kControlWidth);
  // show the describe text and the control 
  control->SetStyle(xtpButtonIconAndCaption);
}

static const SIZE gOffsetSize = {80, 22};
}

IMPLEMENT_ENUM_COMMBOBOX(CouplingCommboBox)

RibbonOscConfigBar::RibbonOscConfigBar(CXTPRibbonBar* ribbon_bar,
                           CWnd* main_frame,
                           CommandUpdater* command_updater)
  : ribbon_bar_(ribbon_bar)
  , main_frame_(main_frame)
  , command_updater_(command_updater)
  , osc_state_(SOFTWARE_DIFF)
  , start_or_stop_(NULL)
  , auto_scale_(NULL) {}

bool RibbonOscConfigBar::Init() {
  if (ribbon_bar_ == NULL) {
    LOG(INFO) << "no RibbonBar" <<
      "continue without config bar.";
    return false;
  }
  // Osc Tab
  CXTPRibbonTab* pTabOsc = ribbon_bar_->AddTab(ID_TAB_OSC);
  // control Group
  CXTPRibbonGroup* pGroupOscControl = pTabOsc->AddGroup(ID_GROUP_OSC_CONTROL);
  start_or_stop_ = new OnOffButton(ID_START_OSC, ID_STOP_OSC, 
    true, command_updater_);
  pGroupOscControl->Add(start_or_stop_, ID_START_STOP_OSC);
  auto_scale_ = new CommandButton(command_updater_, IDC_AUTOSCALE);
  pGroupOscControl->Add(auto_scale_, ID_AUTOSCALE);
  SetControlToManualUpdate(auto_scale_);
  // CAN-H Group
  CXTPRibbonGroup* pGroupCANH = pTabOsc->AddGroup(ID_GROUP_CAN_HIGH);
  can_h_vertical_div_ = DYNAMIC_DOWNCAST(CXTPControlComboBox,
    pGroupCANH->Add(xtpControlComboBox, ID_COMBOX_CAN_HIGH_VERTICAL_DIV));
  SetControlShowStyle(can_h_vertical_div_);
  can_h_vertical_offset_ = 
    new CXTPQuantityEdit(command_updater_, IDC_CHNL_WAVE_VOLT_OFFSET, _T("V"));
  pGroupCANH->Add(can_h_vertical_offset_, ID_EDIT_CAN_HIGH_VERICAL_OFFSET);
  SetControlShowStyle(can_h_vertical_offset_);
  SetControlToManualUpdate(can_h_vertical_offset_);

  can_h_coupling_ = new CouplingCommboBox(command_updater_, 
    IDC_CHNL_WAVE_COUPLING, ribbon_bar_->GetCommandBars());
  pGroupCANH->Add(can_h_coupling_, ID_COMBOX_CAN_HIGH_VERTICAL_COUPLING);
  SetControlShowStyle(can_h_coupling_);
  SetControlToManualUpdate(can_h_coupling_);

  return true;
}

void RibbonOscConfigBar::EnabledStateChangedForCommand( int id, bool enabled ) {
  CXTPControl* control = NULL;
  switch (id) {
    case IDC_OSC_ON_OFF: 
      control = start_or_stop_;

    case IDC_AUTOSCALE:
      control = auto_scale_;
      break;

    case IDC_CHNL_WAVE_VOLT_OFFSET:
      control = can_h_vertical_offset_;

    case IDC_CHNL_WAVE_COUPLING:
      control = can_h_coupling_;

    default:
      NOTREACHED();
  }
  // set enable of the control
  if (control != NULL) {
    control->SetEnabled(enabled);
  }
}

void RibbonOscConfigBar::ParamChangedForCommand(int id, const base::Value& param) {
  switch (id) {
    case IDC_OSC_ON_OFF: {
      bool on_off;
      CHECK(param.GetAsBoolean(&on_off));
      if (start_or_stop_ != NULL) {
        // when on state need to show the stop UI
        start_or_stop_->UpdateState(on_off);
      }
      break;
     }

    case IDC_AUTOSCALE:
      break;

    case IDC_CHNL_WAVE_VOLT_OFFSET: {
      double value;
      string16 unit;
      const base::DictionaryValue* dict;
      CHECK(param.GetAsDictionary(&dict));
      CHECK(dict->GetDouble(string(kValuePath), &value));
      CHECK(dict->GetString(string(kUnitPath), &unit));
      CHECK(unit == string16(_T("V"))); // no only allow set value, unit no change.
      can_h_vertical_offset_->set_value(value);
      break;
    }
    
    case IDC_CHNL_WAVE_COUPLING: {
      int select;
      CHECK(param.GetAsInteger(&select));
      can_h_coupling_->SetSelect(static_cast<ChnlWaveCoupling>(select), false);
      break;
    }
    default:
      NOTREACHED();
  }
}
