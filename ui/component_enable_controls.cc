#include "stdafx.h"

#include "ui/component_enable_controls.h"

namespace {
  void MoveWindowByRect(CWnd* wnd, CRect rect) {
    int x = rect.left;
    int y = rect.top;
    int cx = rect.Width();
    int cy = rect.Height();
    wnd->SetWindowPos(NULL, x, y, cx, cy, 
      SWP_NOSENDCHANGING |SWP_NOACTIVATE | SWP_NOZORDER);
  }

  static const LPTSTR kCursorID = IDC_HAND;
}

BEGIN_MESSAGE_MAP(ComponentEnableButton, CButton)
  ON_CONTROL_REFLECT(BN_CLICKED, OnClick)
  ON_WM_SETCURSOR()
END_MESSAGE_MAP()

ComponentEnableButton::ComponentEnableButton(
  ComponentEnableControls* component_enable)
    : component_enable_(component_enable) {}

void ComponentEnableButton::OnClick() {
  component_enable_->toggle_enable(true);
}

BOOL ComponentEnableButton::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) {
  ::SetCursor(LoadCursor(NULL, kCursorID));
  return TRUE;
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


void ComponentEnableControls::MoveWindow( RECT picture_rect, RECT button_rect ) {
  MoveWindowByRect(&picture_, picture_rect);
  MoveWindowByRect(&button_, button_rect);
}
