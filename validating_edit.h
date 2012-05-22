#ifndef VALIDATING_EDIT_H_
#define VALIDATING_EDIT_H_
#pragma once

// change for http://www.codeproject.com/Articles/7832/CFilterEdit-Use-Regular-Expressions-to-Filter-Your 
// ValidatingEdit Provide validating framework and UI response for edit the
// restrict the user input, and guide them. a extend class can using regex to 
// implement the Validating method.
// using template technology for inherit from different Edit class(MFC is no 
// welcome for template)
// for using you need to define message handle struct
// in cpp define this:
// BEGIN_MESSAGE_MAP(CBaseEdit<TBaseEdit>, CEdit)
//   ON_WM_CHAR()
//   ON_WM_KEYDOWN()
//   ON_WM_KILLFOCUS()
//   ON_WM_SETFOCUS()
//   ON_WM_CTLCOLOR_REFLECT()
//   ON_WM_PAINT()
// END_MESSAGE_MAP()

template <class TBaseEdit>
class CBaseEdit : public TBaseEdit 
{
public:
  CBaseEdit (const bool bProcessChars = true)
    : m_bProcessChars (bProcessChars) {
    SetDefaults ();
  }
  virtual ~CBaseEdit () {};

  // Valid the Input. if the Text is Valid or allow Empty. if bSetInputValid is
  // true, change the UI Valid state.
  bool IsInputValid (const bool bSetInputValid = true) {
    bool bSuccess = true;
    CString str;

    GetWindowText (str);
    bSuccess = IsTextValid (str) || m_bAllowEmpty && str.IsEmpty ();

    if (bSetInputValid) SetInputValid (bSuccess);

    return bSuccess;
  }
  
  // Get the Text to Valid when OnChar() need to replace the select by input
  // char.
  const CString &GetProposedText () const {
    return m_strProposedText;
  }

  // in WindowProc() Process the cut, paste command, if the operate is no valid
  // no pass to the origin WindowProc()
  // TODO can this do in the OnCut or OnPaste
  void SetProcessChars (const bool bProcessChars) {
    m_bProcessChars = bProcessChars;
  }
  bool GetProcessChars () const {
    return m_bProcessChars;
  }

  // if allow Empty is true, the empty string is valid.
  // You can do this in the regex directly, but for more complex
  // controls (with multiple modes/regexes) it is cleaner to
  // use this flag.
  void SetAllowEmpty (const bool bAllowEmpty) {
    m_bAllowEmpty = bAllowEmpty;
  }
  bool GetAllowEmpty () const {
    return m_bAllowEmpty;
  }


  // Display related functions
  
  // whether show the wavy line on Error
  void SetWavyLineOnError (const bool bWavyLineOnError) {
    m_bWavyLineOnError = bWavyLineOnError;
  }
  bool GetWavyLineOnError () const {
    return m_bWavyLineOnError;
  }
  void SetWavyLineColour (const COLORREF crWavyLine) {
    m_crWavyLineColour = crWavyLine;
  }
  void SetBackgroundColourFocus (const COLORREF crBkFocus) {
    m_crBkFocus = crBkFocus;
    m_brFocus.DeleteObject ();
    m_brFocus.CreateSolidBrush (m_crBkFocus);
  }
  void SetForegroundColourFocus (const COLORREF crFgFocus) {
    m_crFgFocus = crFgFocus;
  }
  void SetBackgroundColourOK (const COLORREF crBkOK) {
    m_crBkOK = crBkOK;
    m_brOK.DeleteObject ();
    m_brOK.CreateSolidBrush (m_crBkOK);
  }
  void SetForegroundColourOK (const COLORREF crFgOK) {
    m_crFgOK = crFgOK;
  }
  void SetBackgroundColourError (const COLORREF crBkError) {
    m_crBkError = crBkError;
    m_brError.DeleteObject ();
    m_brError.CreateSolidBrush (m_crBkError);
  }
  void SetForegroundColourError (const COLORREF crFgError) {
    m_crFgError = crFgError;
  }

  // Tooltip functions
  
  // INFO the pParent is the Edit or the MainFrame, in the sample code the 
  // pParent is the sample Dialog.
  void CreateToolTip (CWnd *pParent, const TCHAR *pszText,
    const int iIconType = 1, const TCHAR *pszTitle = _T("Data Format")) {
      TOOLINFO ti;
      DWORD dwStyle = TTS_BALLOON;

      m_ToolTip.Create (pParent, dwStyle);
      m_ToolTip.FillInToolInfo (ti, this, 0);
      ti.uFlags |= TTF_TRACK | TTF_CENTERTIP | TTF_ABSOLUTE;
      ti.lpszText = (LPTSTR) pszText;
#if _MSC_VER >= 1310
      m_ToolTip.SetTitle (iIconType, pszTitle);
#endif
      m_ToolTip.SendMessage (TTM_ADDTOOL, 0, reinterpret_cast<LPARAM>(&ti));
  }

  void SetToolTipText (const TCHAR *pszText) {
    m_ToolTip.UpdateTipText (pszText, this);
  }
  void SetToolTipTitle (const int iIconType, const TCHAR *pszTitle) {
#if _MSC_VER >= 1310
    m_ToolTip.SetTitle (iIconType, pszTitle);
#endif
  }
  
  // whether Display ToolTip on NULL string
  void SetDisplayTTonNull (const bool bDisplayTTonNull) {
    m_bDisplayTTonNull = bDisplayTTonNull;
  }

  bool GetDisplayTTonNull () const {
    return m_bDisplayTTonNull;
  }

  // if the ToolTip Wnd is create
  bool GetToolTipCreated () const {
    return m_ToolTip.m_hWnd != 0;
  }

protected:
  // Text Valid functions, check the syntax and semantic
  
  // check the text with SyntaxCheck and SematicCheck.
  bool IsTextValid (const CString &strText) {
    bool bSuccess = SyntaxCheck (strText);

    if (bSuccess)
    {
      bSuccess = SemanticCheck (strText);
    }

    return bSuccess;
  }
  virtual bool SyntaxCheck (const CString &strText) { return true; }
  virtual bool SemanticCheck (const CString &strText, 
    CString *pstrErrorMsg = 0) { return true; }
  // check the strText is march so far, may be in the middle of input
  virtual bool MarchSoFar (const CString &strText) { return true; }

  // Validate char for OnChar
  bool ValidateChar (UINT nChar, const bool bDisplayToolTip = true) {
    int iStartIndex = -1;
    int iEndIndex = -1;
    bool bCharValid = true;

    GetSel (iStartIndex, iEndIndex);
    GetWindowText (m_strProposedText);

    //  Backspace validation is done here as it has a virtual key code.
    if (nChar == VK_BACK)
    {
      if (iStartIndex == iEndIndex)
      {
        --iStartIndex;
      }

      // Remove char
      m_strProposedText.Delete (iStartIndex, iEndIndex - iStartIndex);
      bCharValid = Match (m_strProposedText, bDisplayToolTip);
    }
    // Ignore ^X, ^C and ^V
    else if (nChar != '\x03' && nChar != '\x16' && nChar != '\x18')
    {
      m_strProposedText.Delete (iStartIndex, iEndIndex - iStartIndex);
      m_strProposedText.Insert (iStartIndex, static_cast<TCHAR>(nChar));
      bCharValid = Match (m_strProposedText, bDisplayToolTip);
    }

    return bCharValid;
  }
  
  // check user is input in the right way.
  // NOTE: This function checks if the given string matches SO FAR.
  // To check if the edit control contains a completely valid string,
  // IsInputValid is called.
  bool Match (const CString &strText, const bool bDisplayToolTip = true) {
    bool bSuccess = true;

    // An empty string is always valid for matching a string SO FAR.
    if (strText.GetLength ())
    {
      bSuccess = MarchSoFar(strText);

      if (bDisplayToolTip)
      {
        const bool bTT = GetDisplayTTonNull ();

        SetDisplayTTonNull (true);
        DisplayToolTip (!bSuccess);
        SetDisplayTTonNull (bTT);
      }
    }

    return bSuccess;
  }


  // return if the string is match the regex after the Delete or Paste.
  bool CanDelete () {
    CString strText;
    int iStartIndex = -1;
    int iEndIndex = -1;

    GetWindowText (strText);
    GetSel (iStartIndex, iEndIndex);

    if (iStartIndex == iEndIndex)
    {
      ++iEndIndex;
    }

    strText.Delete (iStartIndex, iEndIndex - iStartIndex);
    return Match (strText);
  }

  bool CanPaste () {
    bool bSuccess = OpenClipboard () != 0;

    if (bSuccess)
    {
#ifdef _UNICODE
      HANDLE hClipMem = ::GetClipboardData (CF_UNICODETEXT);
#else
      HANDLE hClipMem = ::GetClipboardData (CF_TEXT);
#endif

      bSuccess = hClipMem != 0;

      if (bSuccess)
      {
        const TCHAR *lpClipMem = (const TCHAR *) ::GlobalLock (hClipMem);

        bSuccess = lpClipMem != 0;

        if (bSuccess)
        {
          CString strClipText;
          CString strText;
          int iStartIndex = -1;
          int iEndIndex = -1;

          strClipText = lpClipMem;
          ::GlobalUnlock (hClipMem);
          GetWindowText (strText);
          GetSel (iStartIndex, iEndIndex);
          strText.Delete (iStartIndex, iEndIndex - iStartIndex);
          strText.Insert (iStartIndex, strClipText);

          if (strText.GetLength () > static_cast<int>(GetLimitText ()))
          {
            strText = strText.Left (GetLimitText ());
          }

          bSuccess = Match (strText);

          if (bSuccess)
          {
            bSuccess = SemanticCheck (strText);
          }
        }
      }

      ::CloseClipboard ();
    }

    return bSuccess;
  }
  // Set control state to input OK and hide tooltip
  void ResetValid () {
    if (!m_bInputValid)
    {
      SetInputValid (true);
      DisplayToolTip (FALSE);
    }
  }

  // Set control state to input OK/not OK, update the visuals and
  // set the focus if the input is not valid
  void SetInputValid (const bool bInputValid) {
    m_bInputValid = bInputValid;
    Invalidate ();

    if (!m_bInputValid) SetFocus ();
  }


  // Tooltip functions
  void DisplayToolTip (const bool bDisplay, const TCHAR *pszText = 0) {
    if (m_ToolTip.m_hWnd &&
      !(GetWindowTextLength () == 0 && !m_bDisplayTTonNull))
    {
      CToolInfo ti;

      m_ToolTip.GetToolInfo (ti, this, 0);

      if (bDisplay)
      {
        RECT ClientRect;
        int iHeight = 0;
        int iOffset = 0;
        const int iBorder = ::GetSystemMetrics (SM_CXEDGE) + 1;

        if (pszText)
        {
          m_bInputValid = false;

          if (m_strPrevToolTipText.IsEmpty ())
          {
            m_strPrevToolTipText = ti.lpszText;
          }

          SetToolTipText (pszText);
        }
        else if (!m_strPrevToolTipText.IsEmpty ())
        {
          SetToolTipText (m_strPrevToolTipText);
          m_strPrevToolTipText.Empty ();
        }

        GetClientRect (&ClientRect);
        iHeight = ClientRect.bottom - ClientRect.top;
        iOffset = (ClientRect.right - ClientRect.left) / 2;
        GetWindowRect (&ti.rect);
        ti.rect.top += iHeight + iBorder;
        ti.rect.bottom += iHeight + iBorder;
        ti.rect.left += iOffset;
        ti.rect.right += iOffset;

        m_ToolTip.SendMessage (TTM_TRACKPOSITION, 0, MAKELPARAM (ti.rect.left,
          ti.rect.top));
        m_ToolTip.SendMessage (TTM_TRACKACTIVATE, TRUE,
          reinterpret_cast<LPARAM>(&ti));
      }
      else
      {
        m_ToolTip.SendMessage (TTM_TRACKACTIVATE, FALSE,
          reinterpret_cast<LPARAM>(&ti));
      }
    }
  }

  // for process the cut and paste command before default process
  // Intercept messages that change the text and validate them.
  virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
    LRESULT lResult = 0;
    bool bExecute = true;
    bool bResetValid = false;

    if (m_bProcessChars)
    {
      switch (message)
      {
      case EM_REPLACESEL:
        {
          int iStartIndex = -1;
          int iEndIndex = -1;
          CString strReplace;

          GetSel (iStartIndex, iEndIndex);
          strReplace = reinterpret_cast<LPCTSTR>(lParam);
          GetWindowText (m_strProposedText);
          m_strProposedText.Delete (iStartIndex, iEndIndex -
            iStartIndex);
          m_strProposedText.Insert (iStartIndex, strReplace);
          bExecute = bResetValid = Match (m_strProposedText);

          if (bExecute)
          {
            bExecute = SemanticCheck (m_strProposedText);
          }

          break;
        }
      case WM_CLEAR:
      case WM_CUT:
        bExecute = bResetValid = CanDelete ();
        break;
      case WM_PASTE:
        bExecute = bResetValid = CanPaste ();
        break;
      case WM_SETTEXT:
        {
          const TCHAR *psz = reinterpret_cast<const TCHAR *>
            (lParam);

          bExecute = bResetValid = !*psz || IsTextValid (psz);
          break;
        }
      default:
        break;
      }
    }
    else
    {
      bResetValid = true;
    }

    if (bExecute)
    {
      lResult = CEdit::WindowProc (message, wParam, lParam);
    }

    if (bResetValid) ResetValid ();

    return lResult;
  }

protected:
  afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {
    if (!m_bProcessChars || ValidateChar (nChar))
    {
      CEdit::OnChar (nChar, nRepCnt, nFlags);
    }
  }

  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
    bool bExecute = true;

    ResetValid ();

    // VK_DELETE never reaches OnChar, so we process it here.
    if (nChar == VK_DELETE)
    {
      if (m_bProcessChars)
      {
        bExecute = CanDelete ();
      }
    }
    else if (!(::GetKeyState (VK_SHIFT) & 0x80) &&
      (::GetKeyState (VK_CONTROL) & 0x80))
    {
      // ^X, ^C and ^V do not have virtual key codes so we cannot
      // cleanly process them in OnChar. CBaseEdit::WindowProc deals with
      // responding to WM_CUT and WM_PASTE
      if ((nChar == 'x' || nChar == 'X'))
      {
        SendMessage (WM_CUT, 0, 0);
        // Will handle cut ourselves, so don't pass char on...
        bExecute = false;
      }
      // For some reason this is not needed...
      /*
      else if ((nChar == 'v' || nChar == 'V'))
      {
      SendMessage (WM_PASTE, 0, 0);
      // Will handle paste ourselves, so don't pass char on...
      bExecute = false;
      }
      */
    }

    if (bExecute)
    {
      CEdit::OnKeyDown (nChar, nRepCnt, nFlags);
    }
  }

  afx_msg void OnKillFocus(CWnd* pNewWnd) {
    // Pass in false so that focus is not set back
    // to this control (see function SetInputValid).
    m_bInputValid = IsInputValid (false);
    Invalidate ();
    DisplayToolTip (FALSE);

    // Don't allow cursor to disappear...
    if (pNewWnd && pNewWnd->m_hWnd != m_ToolTip.m_hWnd)
    {
      CEdit::OnKillFocus (pNewWnd);
    }
  }

  afx_msg void OnSetFocus(CWnd* pOldWnd) {
    CEdit::OnSetFocus (pOldWnd);
    Invalidate ();

    if (!m_bInputValid) DisplayToolTip (true);
  }

  afx_msg HBRUSH CtlColor(CDC *pDC, UINT nCtlColor) {
    HBRUSH hbr = 0;

    if (!(GetStyle () & ES_READONLY) && IsWindowEnabled ())
    {
      if (m_bInputValid)
      {
        if (GetFocus () == this)
        {
          hbr = m_brFocus;
          pDC->SetBkColor (m_crBkFocus);
          pDC->SetTextColor (m_crFgFocus);
        }
        else
        {
          hbr = m_brOK;
          pDC->SetBkColor (m_crBkOK);
          pDC->SetTextColor (m_crFgOK);
        }
      }
      else
      {
        hbr = m_brError;
        pDC->SetBkColor (m_crBkError);
        pDC->SetTextColor (m_crFgError);
      }
    }

    return hbr;
  }

  afx_msg void OnPaint() {
    if (GetStyle () & ES_READONLY)
    {
      CEdit::OnPaint ();
    }
    else
    {
      CClientDC dc (this);

      //  This stops the control from going grey
      Default ();

      if (!m_bInputValid && m_bWavyLineOnError)
      {
        CRect rect;
        CRect rectClient;
        const int iIncrement = 2;

        // Get the rect for the entire edit control
        GetRect (&rect);
        GetClientRect (&rectClient);

        // Check that wavy line will not over-write edit control border...
        if (rectClient.bottom - rect.bottom >= iIncrement + 1)
        {
          CFont *pFont = GetFont ();
          CPen *pPen = dc.GetCurrentPen ();
          LOGPEN lp;
          CPen MyPen;
          CString strText;
          CSize size;
          int iX = 0;

          rect.bottom -= iIncrement - 2;

          // We must use the correct font when using GetTextExtent..!
          dc.SelectObject (pFont);
          // We have to create a new pen to set a new colour...
          pPen->GetLogPen (&lp);
          lp.lopnColor = m_crWavyLineColour;
          MyPen.CreatePenIndirect (&lp);
          dc.SelectObject (&MyPen);
          GetWindowText (strText);
          // This part deals with the *displayed* Text Extent
          size = dc.GetTextExtent (strText.
            Mid (CharFromPos (CPoint (rect.left + 1, rect.top))));

          // Check for case of empty string
          if (size.cx == 0) size.cx = iIncrement * 3;
          // Dont draw off the end of the edit control
          else if (size.cx > rect.right) size.cx = rect.right;

          int iOffset = 0;

          if (GetStyle () & ES_CENTER)
          {
            iOffset = (rect.Width () - size.cx) / 2;
          }
          else if (GetStyle () & ES_RIGHT)
          {
            iOffset = rect.Width () - size.cx;
          }

          rect.left += iOffset;
          // Starting x co-ordinate to start drawing from
          iX = rect.left;
          // Start at the bottom left of the edit control
          dc.MoveTo (rect.left, rect.bottom);

          // Draw the wavy line like in Microsoft Word
          while (iX < size.cx + rect.left)
          {
            iX += iIncrement;

            if (iX > size.cx + rect.left) break;

            dc.LineTo (iX, rect.bottom + iIncrement);
            iX += iIncrement;

            if (iX > size.cx + rect.left) break;

            dc.LineTo (iX, rect.bottom);
          }
        }
        else
        {
          TRACE (_T("Edit Control too small to draw wavy line!\r\n"));
        }
      }
    }
  }

  DECLARE_MESSAGE_MAP()

private:
  void SetDefaults () {
    m_bAllowEmpty = false;
    m_bInputValid = true;
    m_bWavyLineOnError = true;
    SetWavyLineColour (RGB (255, 0, 0));
    SetBackgroundColourFocus (::GetSysColor(COLOR_WINDOW));
    SetForegroundColourFocus (::GetSysColor(COLOR_WINDOWTEXT));
    SetBackgroundColourOK (::GetSysColor(COLOR_WINDOW));
    SetForegroundColourOK (::GetSysColor(COLOR_WINDOWTEXT));
    SetBackgroundColourError (::GetSysColor(COLOR_WINDOW));
    SetForegroundColourError (::GetSysColor(COLOR_WINDOWTEXT));
    m_bDisplayTTonNull = true;
  }

  CBaseEdit (CBaseEdit const &); // No copy construction.
  CBaseEdit &operator = (CBaseEdit const &); // No assignment.

private:
  bool m_bProcessChars;
  bool m_bAllowEmpty;
  
  bool m_bInputValid;
  bool m_bControlDown;
  CString m_strProposedText;
  CString m_strPrevToolTipText;

  // Display related members
  bool m_bWavyLineOnError;
  COLORREF m_crWavyLineColour;
  COLORREF m_crFgFocus;
  COLORREF m_crBkFocus;
  COLORREF m_crFgOK;
  COLORREF m_crBkOK;
  COLORREF m_crFgError;
  COLORREF m_crBkError;
  CBrush m_brFocus;
  CBrush m_brOK;
  CBrush m_brError;
  bool m_bDisplayTTonNull;
  CToolTipCtrl m_ToolTip;
};

#endif // VALIDATING_EDIT_H_