#include "stdafx.h"

#include "ui/float_edit.h"

UINT CFloatEdit::UWM_CHECK_VALUE = ::RegisterWindowMessage(_T("UWM_CHECK_VALUE-{6FE8A4C0-AE33-11d4-A002-006067718D04}"));
UINT CFloatEdit::UWM_VALID_CHANGE = ::RegisterWindowMessage(_T("UWM_VALID_CHANGE-{6FE8A4C1-AE33-11d4-A002-006067718D04}"));

CFloatEdit::CFloatEdit()
{
 errorBrush.CreateSolidBrush(  RGB(255, 128, 128));
 partialBrush.CreateSolidBrush(RGB(255, 255, 128));
 emptyBrush.CreateSolidBrush(  ::GetSysColor(COLOR_WINDOW));
 OKBrush.CreateSolidBrush(     RGB(192, 255, 192));
 SyntaxValid = FALSE;
 ValueValid = FALSE;
}

CFloatEdit::~CFloatEdit()
{
 errorBrush.DeleteObject();
 partialBrush.DeleteObject();
 emptyBrush.DeleteObject();
 OKBrush.DeleteObject();
}


BEGIN_MESSAGE_MAP(CFloatEdit, CEdit)
	//{{AFX_MSG_MAP(CFloatingEdit)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
	ON_WM_CTLCOLOR_REFLECT()
	ON_CONTROL_REFLECT_EX(EN_CHANGE, OnChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFloatingEdit message handlers

/****************************************************************************
*                         CFloatingEdit::Check
* Result: CBrush *
*       The desired drawing brush
* Effect: 
*       Sets the SyntaxValid and ValueValid flags
****************************************************************************/

CBrush * CFloatEdit::Check()
    {
     enum {S0, IPART, FPART, ESIGN, EPART};
     WORD state = S0;
     CBrush * brush;
     BOOL change = FALSE;

     brush = &emptyBrush;

     CString s;
     CEdit::GetWindowText(s);
     for(int i = 0; brush != &errorBrush && i < s.GetLength();)
	{ /* scan string */
	 TCHAR ch = s[i];
	 switch(MAKELONG(state, ch))
	    { /* states */
	     case MAKELONG(S0, _T(' ')):
	     case MAKELONG(S0, _T('\t')):
		i++;
		continue;
	     case MAKELONG(S0, _T('+')):
	     case MAKELONG(S0, _T('-')):
		i++;
		brush = &partialBrush;
		state = IPART;
		continue;
	     case MAKELONG(S0, _T('0')):
	     case MAKELONG(S0, _T('1')):
	     case MAKELONG(S0, _T('2')):
	     case MAKELONG(S0, _T('3')):
	     case MAKELONG(S0, _T('4')):
	     case MAKELONG(S0, _T('5')):
	     case MAKELONG(S0, _T('6')):
	     case MAKELONG(S0, _T('7')):
	     case MAKELONG(S0, _T('8')):
	     case MAKELONG(S0, _T('9')):
		state = IPART;
		continue;
	     case MAKELONG(S0, _T('.')):
		i++;
		state = FPART;
		brush = &partialBrush;
		continue;
	     case MAKELONG(S0, _T('E')):
	     case MAKELONG(S0, _T('e')):
		i++;
		state = ESIGN;
		brush = &partialBrush;
		continue;
	     case MAKELONG(IPART, _T('0')):
	     case MAKELONG(IPART, _T('1')):
	     case MAKELONG(IPART, _T('2')):
	     case MAKELONG(IPART, _T('3')):
	     case MAKELONG(IPART, _T('4')):
	     case MAKELONG(IPART, _T('5')):
	     case MAKELONG(IPART, _T('6')):
	     case MAKELONG(IPART, _T('7')):
	     case MAKELONG(IPART, _T('8')):
	     case MAKELONG(IPART, _T('9')):
		i++;
		brush = &OKBrush;
		continue;
	     case MAKELONG(IPART, _T('.')):
		i++;
		brush = &OKBrush;
		state = FPART;
		continue;
	     case MAKELONG(IPART, _T('e')):
	     case MAKELONG(IPART, _T('E')):
		i++;
		brush = &partialBrush;
		state = ESIGN;
		continue;
	     case MAKELONG(FPART, _T('0')):
	     case MAKELONG(FPART, _T('1')):
	     case MAKELONG(FPART, _T('2')):
	     case MAKELONG(FPART, _T('3')):
	     case MAKELONG(FPART, _T('4')):
	     case MAKELONG(FPART, _T('5')):
	     case MAKELONG(FPART, _T('6')):
	     case MAKELONG(FPART, _T('7')):
	     case MAKELONG(FPART, _T('8')):
	     case MAKELONG(FPART, _T('9')):
		i++;
		brush = &OKBrush;
		continue;
	     case MAKELONG(FPART, _T('e')):
	     case MAKELONG(FPART, _T('E')):
		i++;
		brush = &partialBrush;
		state = ESIGN;
		continue;
	     case MAKELONG(ESIGN, _T('+')):
	     case MAKELONG(ESIGN, _T('-')):
		i++;
		brush = &partialBrush;
		state = EPART;
		continue;
	     case MAKELONG(ESIGN, _T('0')):
	     case MAKELONG(ESIGN, _T('1')):
	     case MAKELONG(ESIGN, _T('2')):
	     case MAKELONG(ESIGN, _T('3')):
	     case MAKELONG(ESIGN, _T('4')):
	     case MAKELONG(ESIGN, _T('5')):
	     case MAKELONG(ESIGN, _T('6')):
	     case MAKELONG(ESIGN, _T('7')):
	     case MAKELONG(ESIGN, _T('8')):
	     case MAKELONG(ESIGN, _T('9')):
		state = EPART;
		continue;
	     case MAKELONG(EPART, _T('0')):
	     case MAKELONG(EPART, _T('1')):
	     case MAKELONG(EPART, _T('2')):
	     case MAKELONG(EPART, _T('3')):
	     case MAKELONG(EPART, _T('4')):
	     case MAKELONG(EPART, _T('5')):
	     case MAKELONG(EPART, _T('6')):
	     case MAKELONG(EPART, _T('7')):
	     case MAKELONG(EPART, _T('8')):
	     case MAKELONG(EPART, _T('9')):
		i++;
		brush = &OKBrush;
		continue;
	     default:
         brush = &errorBrush;
         break;
	    } /* states */
	} /* scan string */

     BOOL newValid = brush == &OKBrush;
     if(newValid != SyntaxValid)
       change = TRUE;

     SyntaxValid = newValid;

     if(SyntaxValid)
	{ /* send to parent */
	 newValid = GetParent()->SendMessage(UWM_CHECK_VALUE,
					     MAKELONG(GetDlgCtrlID(), EN_CHANGE),
					     (LPARAM)m_hWnd);
         if(!ValueValid)
             brush = &errorBrush;
	} /* send to parent */
     else
	newValid = FALSE;

     if(ValueValid != newValid)
	change = TRUE;

     ValueValid = newValid;

     if(change)
	GetParent()->SendMessage(UWM_VALID_CHANGE,
				 (WPARAM)MAKELONG(GetDlgCtrlID(), (ValueValid && SyntaxValid)),
				 (LPARAM)m_hWnd);
     return brush;
    } // CFloatingEdit::Check

/****************************************************************************
*                           CFloatingEdit::CtlColor
* Inputs:
*       CDC * dc: Display context
*	UINT id: code
* Result: HBRUSH
*       Brush handle
* Effect: 
*       Changes the control color based on the computed state
*	    emptyBrush: Control is empty
*	    OKBrush: Data is valid
*	    partialBrush: Data is incomplete
*	    errorBrush: Data is wrong
*	Sets the Valid flag, so the IsValid method will report the
*	current status. The IsValid flag will be TRUE only if the
*	control is "green" (&OKBrush)
* Notes:
*	This parses a floating point number of the form
*		{ '+' | '-' }? <0..9>* { '.' <0..9>+ }?
*                       { 'e' | 'E' { '+' | '-' }? <0..9>+ }?
****************************************************************************/

HBRUSH CFloatEdit::CtlColor(CDC * dc, UINT id)
    {
     CBrush * brush = Check();
     
     LOGBRUSH br;
     brush->GetLogBrush(&br);
     dc->SetBkColor(br.lbColor);
     return (HBRUSH)*brush;
    } // CFloatingEdit::CtlColor

/****************************************************************************
*                           CFloatingEdit::OnChange
* Result: BOOL
*       TRUE to terminate routing (not used)
*	FALSE to allow routing to continue (only value returned)
* Effect: 
*       Invalidates the entire control so colors come out right. Otherwise
*	the "optimizations" of the redraw will leave the colors banded in
*	odd ways and only update the area around the text, not the entire
*	box.
****************************************************************************/

BOOL CFloatEdit::OnChange()
    {
     InvalidateRect(NULL);
     return FALSE;
    } // CFloatingEdit::OnChange

/****************************************************************************
*                            CFloatingEdit::OnChar
* Inputs:
*       UINT nChar: Character code
*	UINT nRepCnt: Repeat count
*	UINT nFlags: Flags
* Result: void
*       
* Effect: 
*       If the character is not a valid character for the edit control,
*	reject it with a beep. This avoids the user typing completely
*	nonsensical characters.
****************************************************************************/

void CFloatEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
   {
    switch(nChar)
       { /* validate */
	case _T('+'):
	case _T('-'):
	case _T('.'):
	case _T('E'):
	case _T('e'):
	case _T('0'):
	case _T('1'):
	case _T('2'):
	case _T('3'):
	case _T('4'):
	case _T('5'):
	case _T('6'):
	case _T('7'):
	case _T('8'):
	case _T('9'):
	case _T('\b'):
	   break;
	default:
	   MessageBeep(0);
	   return;
       } /* validate */
    CEdit::OnChar(nChar, nRepCnt, nFlags);
}
