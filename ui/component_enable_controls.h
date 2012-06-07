#pragma once

#include "ui/views/picture_ctrl.h"

// controls to show and get the enable state of the component,
// user can click the invisible button, and the picture_ctrl show the relate
// picture. the click range and the picture range is no the same, that is why
// no use only one control.

class ComponentEnableControls;

class ComponentEnableListener
{
public:
  virtual void OnEnableChange(ComponentEnableControls* component, bool enable) = 0;
};

class ComponentEnableButton : public CButton
{
public:
  ComponentEnableButton(ComponentEnableControls* component_enable);

private:
  afx_msg void OnClick();
  DECLARE_MESSAGE_MAP();
  
private:
  ComponentEnableControls* component_enable_;
};

// listener must exit when ComponentEnableControls exit.
class ComponentEnableControls
{
public:
  ComponentEnableControls(unsigned int button_id, 
                          unsigned int picture_id, 
                          unsigned int enable_picture_id, 
                          unsigned int disable_picture_id,
                          ComponentEnableListener* listener);

  bool enable() {
    return enable_;
  }
  // GUI input set and notify, 
  // in listener no need to notify
  void set_enable(bool enable, bool notify=true) {
    if (enable == enable_)
      return;
    enable_ = enable;
    picture_.Load(enable_ ? enable_picture_id_ : disable_picture_id_);
    if (notify && listener_ != NULL)
      listener_->OnEnableChange(this, enable_);
  }

  void toggle_enable(bool notify=true) {
    set_enable(!enable(), notify);
  }

  bool can_change() {
    return button_.IsWindowEnabled() ? true : false;
  }

  void set_can_change(bool enable) {
    // no enable windows so no click.
    button_.EnableWindow(enable ? TRUE : FALSE);
  }

  void DoDataExchange(CDataExchange* pDX);

private:
  CPictureCtrl picture_;
  ComponentEnableButton button_;

  const unsigned int button_id_;
  const unsigned int picture_id_;
  const unsigned int enable_picture_id_;
  const unsigned int disable_picture_id_;

  // the enable of the component
  bool enable_;

  // need to set ui when get HWND
  bool init_ui_;

  // listener
  ComponentEnableListener* listener_;
};