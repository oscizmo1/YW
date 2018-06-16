/*
 *  HistoryEdit.cpp
 *
 *  Description:
 *    CHistoryEdit implementation
 *
 *    A CEdit subclass that allows you to display a text history
 *    of events.
 *
 *  Author:
 *    Ravi Bhavnani (ravib@datablast.net)
 *
 *  Revision History:
 *    15 Mar 1998   rab   Original version
 */

#include "stdafx.h"
#include "HistoryEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CHistoryEdit

CHistoryEdit::CHistoryEdit()
{
	m_bSelectable = FALSE;
	m_nLineCount = 0;
	m_nMaxLineCount = 200;
}

CHistoryEdit::~CHistoryEdit()
{
}

BEGIN_MESSAGE_MAP(CHistoryEdit, CEdit)
	//{{AFX_MSG_MAP(CHistoryEdit)
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHistoryEdit operations

//
//  Purpose:
//    Appends a text string to the history buffer.
//
//  Returns:
//    None.
//
void CHistoryEdit::AppendString(CString str)
{
	CString   strBuffer;    // current contents of edit control

	m_nLineCount ++; // 현재 line count증가

	// Append string
	if (m_nLineCount < m_nMaxLineCount) {
		GetWindowText (strBuffer);
		if (!strBuffer.IsEmpty()) {
			strBuffer += _T("\r\n");
		}
	}
	else {
		m_nLineCount = 1;
		strBuffer.Empty();
	}

	strBuffer += str;
	SetWindowText (strBuffer);

	// Scroll the edit control
	LineScroll (GetLineCount(), 0);
}

void CHistoryEdit::ClearBuffer()
{
	m_nLineCount = 0;
	SetWindowText ("");
}

/////////////////////////////////////////////////////////////////////////////
// CHistoryEdit message handlers

void CHistoryEdit::OnSetFocus(CWnd* pOldWnd) 
{
	/*
	// Don't allow user to select text
	if (m_bSelectable) {
		CEdit::OnSetFocus (pOldWnd);
	}
	else {
		pOldWnd->SetFocus();
	}
	*/
}
