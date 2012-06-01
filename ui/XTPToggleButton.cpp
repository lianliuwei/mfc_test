#include "StdAfx.h"

#include "XTPToggleButton.h"

IMPLEMENT_XTP_CONTROL(CXTPToggleButton, CXTPControlButton)

CXTPToggleButton::CXTPToggleButton(int set_icon_id, int unset_icon_id,
                                   State state)
  : set_icon_id_(set_icon_id)
  , unset_icon_id_(unset_icon_id)
  , state_(state) {
  m_nIconId = state == STATE_SET ? set_icon_id : unset_icon_id;
  SetFlags(GetFlags()|xtpFlagManualUpdate);
}

CXTPToggleButton::CXTPToggleButton() {
  SetFlags(GetFlags()|xtpFlagManualUpdate);
}



CXTPToggleButton::~CXTPToggleButton() {}


void CXTPToggleButton::ToggleState() {
  ChangeState(state_ == STATE_SET ? STATE_UNSET : STATE_SET, true);
}

void CXTPToggleButton::ChangeState(State state, bool notify) {
  if (state == state_)
    return;

  state_ = state;
  // notify the change
  if (notify)
    OnChangeState(state);

  SetIconId(state_ == STATE_SET ? set_icon_id_ : unset_icon_id_);
}

void CXTPToggleButton::OnChangeState(State new_sate) {
  // default do nothing
}


void CXTPToggleButton::OnClick(BOOL bKeyboard, CPoint pt) {
  // call the father method
  CXTPControlButton::OnClick(bKeyboard, pt);

  // change the state.
  if (!GetEnabled()) return;

  ToggleState();
}

void CXTPToggleButton::Copy(CXTPControl* pControl, BOOL bRecursive) {
  // NO IMPLEMENT
  VERIFY(FALSE);
}

void CXTPToggleButton::DoPropExchange( CXTPPropExchange* pPX )
{
  CXTPControlButton::DoPropExchange(pPX);

  PX_Int(pPX, _T("SetIconId"), set_icon_id_, 0);
  PX_Int(pPX, _T("UnSetIconId"), unset_icon_id_, 0);
  PX_Enum(pPX, _T("state"), state_, STATE_SET);

  if (pPX->IsLoading()) {
    ChangeState(state_ == STATE_SET ? STATE_UNSET : STATE_SET, true);
  }
}




