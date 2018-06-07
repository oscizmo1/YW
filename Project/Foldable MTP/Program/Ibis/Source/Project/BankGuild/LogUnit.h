//////////////////////////////////////////////////////////////////////////
// _MBCS, _UNICODE�� ���� File Open ��� ���� ���� [8/7/2015 OSC]
// Format ���� �����Ȱ� ���� [11/16/2014 OSC]
//////////////////////////////////////////////////////////////////////////

#pragma once

class CLogUnit
{
public:

	enum WRITE_TYPE
	{
		WriteType_Buffer,	// Buffer�� �� ���� ���
		WriteType_NoWait,	// ������� �ʰ� �ٷ� ���
		WriteType_Max
	};

	CLogUnit(void);
	CLogUnit(WRITE_TYPE logType, CString strFileName, CString strPath, int nOffsetHour, int nOffsetMin, 
		BOOL bAddDay = FALSE, BOOL bAddTime = TRUE, BOOL bPartition = FALSE);
	~CLogUnit(void);

private:
	HANDLE		m_hMutex;

	WRITE_TYPE m_WriteType;
	int m_nBufferSize;
	CString m_strBuf;
	TCHAR* m_pBuf;

	int m_nWriteStart;
	CString m_strFolderPath;
	CString m_strSubFolderName;
	CString m_strFileName;
	int m_nOffsetHour;
	int m_nOffsetMin;
	

	BOOL m_bAddTime;
	BOOL m_bAddDay;
	BOOL m_bPartition;

	void Write(LPCTSTR szLogText);

public:
	void SetType(WRITE_TYPE type);
	void SetFolderPath(CString strFolderPath);
	void SetSubFolderName(CString strFolderName);	// ��¥���� �ؿ� ���� �ϳ� �� ����� ���� ��...
	void SetFileName(CString szFileName);
	void SetBufferSize(UINT nBufferSize);
	void SetOffsetHour(int nOffsetHour);
	void SetOffsetMin(int nOffsetMin);
	void SetAddTime(BOOL bAdd);
	void SetAddDay(BOOL bAdd);
	void SetPartition(BOOL bUse);	// �ð���� ���ϸ� �з�

	void AddBuf(LPCTSTR fmt, ...);
	void WriteBuf(BOOL bOverFlow = FALSE);
	BOOL GetFullPath(CString &strPath, SYSTEMTIME &time);
};
