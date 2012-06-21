#include "stdafx.h"

#include "ui/command_button.h"

#include "base/logging.h"

IMPLEMENT_XTP_CONTROL(CommandButton, CXTPControlButton)

CommandButton::CommandButton(CommandUpdater* command_updater, int id)
  : command_updater_(command_updater)
  , id_(id) {
    CHECK(command_updater_ != NULL);
}

void CommandButton::Copy(CXTPControl* pControl, BOOL bRecursive) {
  CommandButton* button = DYNAMIC_DOWNCAST(CommandButton, pControl);
  DCHECK(button);
  CXTPControlButton::Copy(pControl, bRecursive);
  id_ = button->id_;
  command_updater_ = button->command_updater_;
}

void CommandButton::DoPropExchange(CXTPPropExchange* pPX) {
  NOTREACHED() << "no support XML create.";
}

void CommandButton::OnClick(BOOL bKeyboard, CPoint pt) {
  // call the father method
  CXTPControlButton::OnClick(bKeyboard, pt);
  
  scoped_ptr<base::Value> value(
    base::Value::CreateNullValue());
  command_updater_->ExecuteCommand(id_, *(value.get()));
}


