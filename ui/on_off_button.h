#ifndef ONOFFBUTTON_
#define ONOFFBUTTON_
#pragma once

#include "ui/XTPToggleButton.h"
#include "base/compiler_specific.h"

class CommandUpdater;

class OnOffButton : public CXTPToggleButton
{
public:
  OnOffButton();
  OnOffButton(int set_icon_id, int unset_icon_id, 
    bool on_off, CommandUpdater* command_updater);
  virtual ~OnOffButton() {}

  // using the CommandUpdater to Execute command.
  virtual void OnChangeState(State new_sate) OVERRIDE;

  // 
  void UpdateState(bool on_off);

protected:
  virtual void Copy(CXTPControl* pControl, BOOL bRecursive);

private:
  virtual void DoPropExchange(CXTPPropExchange* pPX);

private:
  CommandUpdater* command_updater_;

  DECLARE_XTP_CONTROL(OnOffButton)
};

#endif // ONOFFBUTTON_