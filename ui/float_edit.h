#pragma once

class CFloatEdit : public CEdit
{
public:
	CFloatEdit();
  virtual ~CFloatEdit();

// Attributes
public:
   BOOL IsValid() {return SyntaxValid && ValueValid; };
   BOOL IsSyntaxValid() { return SyntaxValid; }
   BOOL IsValueValid()  { return ValueValid; }
   /****************************************************************************
   *                               UWM_CHECK_VALUE
   * Inputs:
   *       WPARAM: MAKELONG(GetDlgCtrlID(), EN_CHANGE)
   *	LPARAM: (LPARAM)(HWND): Window handle
   * Result: BOOL
   *       TRUE if value is acceptable
   *	FALSE if value has an error
   * Effect: 
   *       If the value is FALSE, the window is marked as an invalid value
   *	If the value is TRUE, the window is marked as a valid value
   * Notes:
   *	This message is sent to the parent of the control as a consequence
   *	of the EN_CHANGE notification, but only if the value is syntactically
   *	correct. It may be sent at other times as well
   ****************************************************************************/
   static UINT UWM_CHECK_VALUE;

   /****************************************************************************
   *                              UWM_VALID_CHANGE
   * Inputs:
   *       WPARAM: (WPARAM)MAKELONG(GetDlgCtrlID(), BOOL) Flag indicating new valid state
   *       LPARAM: Window handle of child window
   * Result: LRESULT
   *       Logically void, 0, always
   * Effect: 
   *       Notifies the parent that the validity of the input value has changed
   ****************************************************************************/
   static UINT UWM_VALID_CHANGE;

protected:
  afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg HBRUSH CtlColor(CDC * dc, UINT id);
  afx_msg BOOL OnChange();
  DECLARE_MESSAGE_MAP()

  CBrush errorBrush;
  CBrush partialBrush;
  CBrush emptyBrush;
  CBrush OKBrush; 
  CBrush * Check();
  BOOL SyntaxValid;
  BOOL ValueValid;

};
