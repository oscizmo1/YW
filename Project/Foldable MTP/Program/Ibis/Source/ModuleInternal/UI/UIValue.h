#pragma once

class CUIValue_CString
{
public:
	CUIValue_CString(void) {	m_pValue = NULL;	}
	~CUIValue_CString(void) {}

	int m_nID;
	CString *m_pValue;

	void ValuePtr_Link(int nID, void *ptr);
	CString ReturnString();
	void SetValue(void *ptr);
	void SetValueString(CString str);
};

class CUIValue_BOOL
{
public:
	CUIValue_BOOL(void) {	m_pValue = NULL;	}
	~CUIValue_BOOL(void) {}

	int m_nID;
	BOOL *m_pValue;

	void ValuePtr_Link(int nID, void *ptr);
	CString ReturnString();
	void SetValue(void *ptr);
	void SetValueString(CString str);
};

class CUIValue_Int
{
public:
	CUIValue_Int(void) {	m_pValue = NULL;	}
	~CUIValue_Int(void) {}

	int m_nID;
	int *m_pValue;

	void ValuePtr_Link(int nID, void *ptr);
	CString ReturnString();
	void SetValue(void *ptr);
	void SetValueString(CString str);
};

class CUIValue_Double
{
public:
	CUIValue_Double(void) {	m_pValue = NULL;	}
	~CUIValue_Double(void) {}

	int m_nID;
	double *m_pValue;

	void ValuePtr_Link(int nID, void *ptr);
	CString ReturnString();
	void SetValue(void *ptr);
	void SetValueString(CString str);
};
