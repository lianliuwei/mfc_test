#include "StdAfx.h"
#include "resource.h"

#include "XTPConfigBar.h"

#include "base/logging.h"

#include "osc_command_ids.h"
#include "XTPToggleButton.h"

namespace {

static const int kControlWidth = 80;

class OnOffButton : public CXTPToggleButton
{
public:
  OnOffButton(int set_icon_id, int unset_icon_id, 
    bool on_off, CommandUpdater* command_updater);
  virtual ~OnOffButton() {}

  // using the CommandUpdater to Execute command.
  virtual void OnChangeState(State new_sate) OVERRIDE;
  
  // 
  void UpdateState(bool on_off);
private:
  CommandUpdater* command_updater_;
};

OnOffButton::OnOffButton(int on_icon_id, int off_icon_id, 
  bool on_off, CommandUpdater* command_updater )
  : CXTPToggleButton(on_icon_id, off_icon_id, 
    on_off ? STATE_UNSET : STATE_UNSET)
  , command_updater_(command_updater) {}

void OnOffButton::OnChangeState(State new_sate) {
  // the old state is the command to execute.
  bool on_off = !(new_sate == STATE_SET);
  scoped_ptr<base::Value> value(
    base::Value::CreateBooleanValue(on_off));
  command_updater_->ExecuteCommand(IDC_OSC_ON_OFF, *(value.get()));
}

void OnOffButton::UpdateState( bool on_off ) {
  ChangeState(on_off ? STATE_UNSET : STATE_SET, false);
}

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

XTPConfigBar::XTPConfigBar(CXTPRibbonBar* ribbon_bar,
                           CWnd* main_frame,
                           CommandUpdater* command_updater)
  : ribbon_bar_(ribbon_bar)
  , main_frame_(main_frame)
  , command_updater_(command_updater)
  , osc_state_(SOFTWARE_DIFF)
  , start_or_stop_(NULL)
  , auto_scale_(NULL) {}

bool XTPConfigBar::Init() {
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
  auto_scale_ = DYNAMIC_DOWNCAST(CXTPControlButton,
    pGroupOscControl->Add(xtpControlButton, ID_AUTOSCALE));
  SetControlToManualUpdate(auto_scale_);
  // CAN-H Group
  CXTPRibbonGroup* pGroupCANH = pTabOsc->AddGroup(ID_GROUP_CAN_HIGH);
  can_h_vertical_div_ = DYNAMIC_DOWNCAST(CXTPControlComboBox,
    pGroupCANH->Add(xtpControlComboBox, ID_COMBOX_CAN_HIGH_VERTICAL_DIV));
  SetControlShowStyle(can_h_vertical_div_);
  can_h_vertical_offset_ = DYNAMIC_DOWNCAST(CXTPControlEdit,
    pGroupCANH->Add(xtpControlEdit, ID_EDIT_CAN_HIGH_VERICAL_OFFSET));
  SetControlShowStyle(can_h_vertical_offset_);

  return true;
}

void XTPConfigBar::EnabledStateChangedForCommand( int id, bool enabled ) {
  CXTPControl* control = NULL;
  switch (id) {
    case IDC_OSC_ON_OFF: 
      control = start_or_stop_;

    case IDC_AUTOSCALE:
      control = auto_scale_;
      break;

    default:
      NOTREACHED();
  }
  // set enable of the control
  if (control != NULL) {
    control->SetEnabled(enabled);
  }
}

void XTPConfigBar::ParamChangedForCommand(int id, const base::Value& param) {
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

    default:
      NOTREACHED();
  }
}
