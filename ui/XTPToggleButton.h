#ifndef XTPTOGGLEBUTTON_H_
#define XTPTOGGLEBUTTON_H_
#pragma once

#include <CommandBars/XTPControlButton.h>

// the CXTPToggleButton has two icon set  when click change the icon.
// when changeState call the OnChangeState()
class CXTPToggleButton : public CXTPControlButton
{
public:
  enum State{STATE_SET, STATE_UNSET};

  CXTPToggleButton();
  CXTPToggleButton(int set_icon_id, int unset_icon_id, State state);

  virtual ~CXTPToggleButton();

  // set the button into special state
  // when the control level set the state it need set notify to false.
  // or it logic lead to nest call.
  void ChangeState(State state, bool notify);

  // toggle the state.
  void ToggleState();

  // do custom action in this function.
  virtual void OnChangeState(State new_sate);

protected:
  // when popup the commandbars will copy controls.
  virtual void Copy(CXTPControl* pControl, BOOL bRecursive);

  // get call when using XML to config
  virtual void DoPropExchange(CXTPPropExchange* pPX);

private:
  // change the CXTPControlButton OnClick function
  virtual void OnClick(BOOL bKeyboard, CPoint pt);

private:
  // the set and unset icon id
  int set_icon_id_;
  int unset_icon_id_;

  // the state of this button.
  State state_;

  DECLARE_XTP_CONTROL(CXTPToggleButton)
};

#endif // XTPTOGGLEBUTTON_H_