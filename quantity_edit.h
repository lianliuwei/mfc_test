#include "regex_validating_edit.h"

// TODO using CXTPToolTipContext to show the tooltip. more nature.
class QuantityEdit : public RegexValidatingEdit<CXTPControlEditCtrl>
{
public:
  QuantityEdit() 
    : RegexValidatingEdit<CXTPControlEditCtrl>(
      "^([+-]?(\\d+|\\d+\\.\\d+)([eE][+-]?\\d+)?)\\s*(\\w*)$",
      "^[+-]?((\\d*)|(\\d+\\.)|((\\d+|\\d+\\.\\d+)[eE][+-]?)|((\\d+|\\d+\\.\\d+)([eE][+-]?\\d+)?\\s*([a-zA-Z]*)))$")
    {
      SetBackgroundColourError(RGB(255, 148, 148));
  };
  virtual ~QuantityEdit() {};

  virtual BOOL CreateEdit(DWORD dwStyle, CWnd* pParentWnd) {
    BOOL ret = CXTPControlEditCtrl::CreateEdit(dwStyle, pParentWnd);
    CreateToolTip(pParentWnd, _T("type \"double( unit)\""));
    return ret;
  }
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

class CXTPQuantityEdit : public CXTPValidatingControlEdit
{
public:
  CXTPQuantityEdit() {};
  virtual ~CXTPQuantityEdit(){};
private:
  virtual CXTPControlEditCtrl* CreateEditControl() {
    return new QuantityEdit;
  }
};

