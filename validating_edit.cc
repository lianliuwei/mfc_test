#include "stdafx.h"

#include "validating_edit.h"

// NOTE: Be very careful how you specify your regular expressions.
// For strictly formatted data, you will want a rigid regex
// and rely on automatic formatting in a derived class to make
// data entry easier (see the CDateEdit example to see this approach
// in action). For easier validations (say unsigned int) a much looser
// regex can be used without problems.

/////////////////////////////////////////////////////////////////////////////
// CBaseEdit

// public functions:


BEGIN_MESSAGE_MAP(CBaseEdit, CEdit)
	//{{AFX_MSG_MAP(CBaseEdit)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()