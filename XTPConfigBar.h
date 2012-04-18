#ifndef XTPCONFIGBAR_H_
#define XTPCONFIGBAR_H_

#include "base/compiler_specific.h"
#include "command_updater.h"

class CXTPRibbonBar;
class OnOffButton;

// this class using for create the Osc Config Controls. change control set for 
// different Osc state. controls the show or hide of professional controls (it
// is only the ui state). respond the command state change and change the UI.
// 
// INFO this method has risk, because the XTP has mechanism the clone the 
// control obj to implement copy command UI and some other effect, that let the
// ptr to control become invalid.
class XTPConfigBar : public CommandUpdater::CommandObserver
{
public:
  enum OSC_STATE {
    SOFTWARE_DIFF,
    HARDWARE_DIFF,
  };

  // create the control ui.
  XTPConfigBar(CXTPRibbonBar* ribbon_bar, CWnd* main_frame,
    CommandUpdater* command_updater);
  ~XTPConfigBar() {};

  // create the controls
  bool Init();

private:  
  // CommandUpdater::CommandObserver
  virtual void EnabledStateChangedForCommand(int id, bool enabled) OVERRIDE;
  virtual void ParamChangedForCommand(int id, const base::Value& param) OVERRIDE;

private:
  // for execute the command 
  CommandUpdater* command_updater_;

  // for create edit, the AfxGetMainWnd() is no available when LoadFrame and
  // main_frame_ OnCreate.
  CWnd* main_frame_;

  // need the ribbon to create the tab, group, and control.
  CXTPRibbonBar* ribbon_bar_;

  // the ui show different between the hardware diff model 
  // and software diff model.
  OSC_STATE osc_state_;

  // the button switch between the start and stop state.
  OnOffButton* start_or_stop_;
  
  CXTPControlButton* auto_scale_;
  
  CXTPControlComboBox* can_h_vertical_div_;

  CXTPControlEdit* can_h_vertical_offset_;

  // TODO need a self-verify textbox for input voltage and ampere.
  DISALLOW_COPY_AND_ASSIGN(XTPConfigBar);
};

#endif // XTPCONFIGBAR_H_