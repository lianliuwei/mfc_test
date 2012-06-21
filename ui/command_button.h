#pragma once

#include <CommandBars/XTPControlButton.h>

#include "ui/command_updater.h"

class CommandButton : public CXTPControlButton
{
public:
  CommandButton() {}
  CommandButton(CommandUpdater* command_updater, int id);
  virtual ~CommandButton() {}

protected:
  // when popup the commandbars will copy controls.
  virtual void Copy(CXTPControl* pControl, BOOL bRecursive);

private:
  // get call when using XML to config
  virtual void DoPropExchange(CXTPPropExchange* pPX);

  // change the CXTPControlButton OnClick function
  virtual void OnClick(BOOL bKeyboard, CPoint pt);

private:
  int id_;
  CommandUpdater* command_updater_;

  DECLARE_XTP_CONTROL(CommandButton)
};

