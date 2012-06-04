#include "regex_validating_edit.h"

// TODO using CXTPToolTipContext to show the tooltip. more nature.
class CXTPValidatingEditCtrl : public RegexValidatingEdit<CXTPControlEditCtrl>
{
public:
  CXTPValidatingEditCtrl(string full_match, string part_match, CString init_tooltip) 
      : RegexValidatingEdit<CXTPControlEditCtrl>(
        full_match, part_match)
      , init_tooltip_(init_tooltip) {
    SetBackgroundColourError(RGB(255, 148, 148));
  };
  virtual ~CXTPValidatingEditCtrl() {};

  virtual BOOL CreateEdit(DWORD dwStyle, CWnd* pParentWnd) {
    BOOL ret = CXTPControlEditCtrl::CreateEdit(dwStyle, pParentWnd);
    CreateToolTip(pParentWnd, init_tooltip_);
    return ret;
  }

private:
  afx_msg void OnPaint() {
    // no place to draw the wavy line.
    CXTPControlEditCtrl::OnPaint();
  }

  DECLARE_MESSAGE_MAP()

private:
  const CString init_tooltip_;
};

// Validating Edit assist class, it set the color and show tooltip for the
// embed control.
class CXTPValidatingControlEdit : public CXTPControlEdit
{
public:
  CXTPValidatingControlEdit() {};
  virtual ~CXTPValidatingControlEdit() {};

private:
  virtual CHARFORMAT2 GetDefaultCharFormat() {
    CHARFORMAT2 format = CXTPControlEdit::GetDefaultCharFormat();
     CClientDC dc(m_pEdit->GetParent());
     // using CtlColor to Get the Color.
     m_pEdit->GetParent()->SendMessage(WM_CTLCOLOREDIT, (WPARAM)(dc.GetSafeHdc())
       , (LPARAM)(m_pEdit->GetSafeHwnd()));
     format.crTextColor = dc.GetTextColor();
     format.crBackColor = dc.GetBkColor();

    return format;
  }
};
