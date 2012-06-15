#include "StdAfx.h"

#include "ui/on_off_button.h"

#include "base/memory/scoped_ptr.h"
#include "base/values.h"
#include "ui/command_updater.h"
#include "ui/osc_command_ids.h"

IMPLEMENT_XTP_CONTROL(OnOffButton, CXTPToggleButton)

OnOffButton::OnOffButton(int on_icon_id, int off_icon_id, 
                         bool on_off, CommandUpdater* command_updater )
                         : CXTPToggleButton(on_icon_id, off_icon_id, 
                         on_off ? STATE_UNSET : STATE_UNSET)
                         , command_updater_(command_updater) {}

OnOffButton::OnOffButton()
    : command_updater_(NULL) {}


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