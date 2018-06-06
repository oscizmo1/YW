#pragma once

enum
{
	LogType_Buffer,	// Buffer�� �� ���� ���
	LogType_Daily,	// �ϸ��� ���
	LogType_Time,	// �ð����� ���
	LogType_LotEnd,	// Lot�� ���� �� ���� ���
	LogType_CstEnd,	// Cassette�� ���� �� ���� ���
	LogType_NoWait,	// ������� �ʰ� �ٷ� ���
	LogType_Max
};

enum
{
	LOG_SYSTEM,			// Program ���� ���� ���� �α�
 	LOG_SEQUENCE,		// Seq ���� ��Ȳ ��� �α�
	LOG_COMM,			// �˻�� ��� ���� ��� �α�
	LOG_OPERATION,		// ���� ��ư ���� ��� �α�
	LOG_ERR,			// ������ ��� �α�
	LOG_UV,				// MCR ��� 
	LOG_CELL,			// TRACKING �α�
	LOG_MOTOR_ERROR,
	LOG_LOSSTIME,
	LOG_ALIGN,
	LOG_STATUS,
	LOG_ERROR_TYPE,
	LOG_MCR,
	LOG_MAINTACT,
	LOG_TACTTIME,
	LOG_INSPECT,
	LOG_CIM,

	LOG_TRACKING,			// Track In/Out ���� �����
	LOG_MAX					// Log ���� �� ����
};

class CLogUnit
{
public:
	CLogUnit(void);
	~CLogUnit(void);

private:
	int m_nWriteType;
	int m_nBufferSize;
	CString m_strBuf;
	//char* m_pBuf;
	TCHAR* m_pBuf;

	int m_nWriteStart;
	CString m_strFolderPath;
	CString m_strFileName;
	

	BOOL m_bAddTime;
	BOOL m_bAddDay;
	BOOL m_bPartition;

	void Write(LPCTSTR szLogText);

public:
	void SetType(int nType);
	void SetFolderPath(CString strFolderPath);
	void SetFileName(CString szFileName);
	void SetBufferSize(UINT nBufferSize);
	void SetAddTime(BOOL bAdd);
	void SetAddDay(BOOL bAdd);
	void SetPartition(BOOL bUse);	// �ð���� ���ϸ� �з�

	//void AddBuf(LPCTSTR szLogText);
	void AddBuf(int nLogID, LPCTSTR szLogText);
	void WriteBuf(BOOL bOverFlow = FALSE);
	void WriteDaily();
	void WriteTime();
	void WriteLotEnd();
	void WriteCstEnd();
};

class CLogCtrl
{
	//////////////////////////////////////////////////////////////////////////
	// Singleton
public:
	static CLogCtrl* Instance();
	static void Release();
private:
	static CLogCtrl* _instance;
	//////////////////////////////////////////////////////////////////////////

public:
	CLogCtrl(void);
	~CLogCtrl(void);

private:
	CLogUnit m_Log[LOG_MAX];

public:
	void Initialize();
	void AddBuf(int nLogID, LPCTSTR fmt, ...);
	void WriteDaily();
	void WriteTime();
	void WriteLotEnd();
	void WriteCstEnd();
	void WriteNowAll();	// ���� ���ۿ� �׿��ִ� �͵��� ��� ���� �Լ�
	void WriteNowAt(int nLogID);	// ���� ���ۿ� �׿��ִ� �͵��� ��� ���� �Լ�
};
// inline CLogCtrl* GetLogCtrl() {	return CLogCtrl::Instance();	}
