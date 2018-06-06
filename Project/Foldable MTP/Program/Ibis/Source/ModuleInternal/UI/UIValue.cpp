#include "StdAfx.h"
#include "UIValue.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CUIValue_CString::ValuePtr_Link( int nID, void *ptr )
{
	m_nID = nID;
	m_pValue = (CString *)ptr;
}

CString CUIValue_CString::ReturnString()
{
	CString str;
	if(m_pValue)
		str.Format(_T("%s"), m_pValue->GetString());

	return str;
}

void CUIValue_CString::SetValue( void *ptr )
{
	CString *pValue = (CString *)ptr;
	m_pValue->Format(_T("%s"), pValue->GetString());
}

void CUIValue_CString::SetValueString( CString str )
{
	m_pValue->Format(_T("%s"), str.GetString());
}

void CUIValue_BOOL::ValuePtr_Link( int nID, void *ptr )
{
	m_nID = nID;
	m_pValue = (BOOL *)ptr;
}

CString CUIValue_BOOL::ReturnString()
{
	CString str;
	if(m_pValue)
		str.Format(_T("%s"), *m_pValue ? _T("Y"):_T("N"));

	return str;
}

void CUIValue_BOOL::SetValue( void *ptr )
{
	BOOL *pValue = (BOOL *)ptr;
	*m_pValue = *pValue;
}

void CUIValue_BOOL::SetValueString( CString str )
{
	if(str == _T("Y") || str == _T("1") || str == _T("T"))
		*m_pValue = TRUE;
	else
		*m_pValue = FALSE;
}

void CUIValue_Int::ValuePtr_Link( int nID, void *ptr )
{
	m_nID = nID;
	m_pValue = (int *)ptr;
}

CString CUIValue_Int::ReturnString()
{
	CString str;
	if(m_pValue)
		str.Format(_T("%d"), *m_pValue);

	return str;
}

void CUIValue_Int::SetValue( void *ptr )
{
	int *pValue = (int *)ptr;
	*m_pValue = *pValue;
}

void CUIValue_Int::SetValueString( CString str )
{
	*m_pValue = _tstoi(str);
}

void CUIValue_Double::ValuePtr_Link( int nID, void *ptr )
{
	m_nID = nID;
	m_pValue = (double *)ptr;
}

CString CUIValue_Double::ReturnString()
{
	CString str;
	if(m_pValue)
		str.Format(_T("%.f"), *m_pValue);

	return str;
}

void CUIValue_Double::SetValue( void *ptr )
{
	double *pValue = (double *)ptr;
	*m_pValue = *pValue;
}

void CUIValue_Double::SetValueString( CString str )
{
	*m_pValue = _tstof(str);
}
