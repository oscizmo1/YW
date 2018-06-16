
#if !defined(AFX_ADOCLASS_H__3672C5FF_18F6_487D_B1D3_CBE7C810D401__INCLUDED_)
#define AFX_ADOCLASS_H__3672C5FF_18F6_487D_B1D3_CBE7C810D401__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CADOClass  
{
public:
	
	CADOClass();
	virtual ~CADOClass();
	BOOL GetRsDataInt(LPCTSTR lpszColumn, INT& nValue);
	BOOL GetRsDataChar(LPCTSTR lpszColumn, CString& csValue);
	BOOL GetRsDataChar(LPCTSTR lpszColumn, char* pszValue);
	BOOL GetRsDataLong(LPCTSTR lpszColumn, LONG& lValue);
	BOOL GetRsDataTime(LPCTSTR lpszColumn, CTime &cTime);
	BOOL GetRsDataBool(LPCTSTR lpszColumn, BOOL &bValue);
	BOOL GetRsDataFloat(LPCTSTR lpszColumn, float& fValue);
	BOOL IsEOF();
	void LastRS();
	void PrevRS();
	void NextRS();
	void FirstRS();
	BOOL CloseRS();
	bool ExecuteQryCmd(LPCSTR lpszQry, long lop=adCmdUnknown);
	bool ExecuteQryRs(LPCSTR lpszQry, CursorTypeEnum emCurType=adOpenStatic, LockTypeEnum emLockType=adLockReadOnly, long lop=adCmdUnknown);
	bool InUpDelQryCmd(LPCSTR lpszQry);
	bool InUpDelQryConn(LPCSTR lpszQry);
	bool Connect(LPCSTR lpszProvider, ExecuteOptionEnum emOption = adOptionUnspecified);
	bool DisConnect();
	LONG BeginTrans() const { return m_pConnection->BeginTrans();}
	HRESULT RollbackTrans() const { return m_pConnection->RollbackTrans();}
	HRESULT CommitTrans() const { return m_pConnection->CommitTrans();}
	int NextRecord(void);
	int setFilter(_bstr_t strField, _bstr_t strFilter);
	long GetRecordCount(void);

private:
	_RecordsetPtr m_pRs;
	_CommandPtr m_pCommand;
	_ConnectionPtr m_pConnection;

	void DumpError(const _com_error &e) const;

};

#endif // !defined(AFX_ADOCLASS_H__3672C5FF_18F6_487D_B1D3_CBE7C810D401__INCLUDED_)
