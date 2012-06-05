#include "stdafx.h"

#include "ui/component_enable_controls.h"


BEGIN_MESSAGE_MAP(ComponentEnableButton, CButton)
  ON_CONTROL_REFLECT(BN_CLICKED, OnClick)
END_MESSAGE_MAP()


ComponentEnableButton::ComponentEnableButton(
  ComponentEnableControls* component_enable)
    : component_enable_(component_enable) {}

void ComponentEnableButton::OnClick() {
  component_enable_->toggle_enable(true);
}

ComponentEnableControls::ComponentEnableControls(unsigned int button_id,
                                                 unsigned int picture_id,
                                                 unsigned int enable_picture_id,
                                                 unsigned int disable_picture_id,
                                                 ComponentEnableListener* listener) 
    : button_id_(button_id) 
    , picture_id_(picture_id) 
    , enable_picture_id_(enable_picture_id) 
    , disable_picture_id_(disable_picture_id) 
    , enable_(false)
    , button_(this)
    , init_ui_(false)
    , listener_(listener) {}

void ComponentEnableControls::DoDataExchange( CDataExchange* pDX ) {
  DDX_Control(pDX, button_id_, button_);
  DDX_Control(pDX, picture_id_, picture_);
  if (init_ui_ == false) {
    init_ui_ = true;
    picture_.Load(enable() ? enable_picture_id_ : disable_picture_id_);
    button_.EnableWindow(TRUE);
  }
}
