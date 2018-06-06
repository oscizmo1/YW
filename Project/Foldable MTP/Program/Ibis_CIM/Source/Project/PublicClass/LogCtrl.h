#pragma once

enum
{
	LogType_Buffer,	// Buffer가 다 차면 기록
	LogType_Daily,	// 일마다 기록
	LogType_Time,	// 시간마다 기록
	LogType_LotEnd,	// Lot이 끝날 때 마다 기록
	LogType_CstEnd,	// Cassette가 꽉찰 때 마다 기록
	LogType_NoWait,	// 대기하지 않고 바로 기록
	LogType_Max
};

enum
{
	LOG_SYSTEM,			// Program 내부 구동 관련 로그
 	LOG_SEQUENCE,		// Seq 변경 현황 기록 로그
	LOG_COMM,			// 검사부 통신 관련 기록 로그
	LOG_OPERATION,		// 유저 버튼 조작 기록 로그
	LOG_ERR,			// 에러명 기록 로그
	LOG_UV,				// MCR 결과 
	LOG_CELL,			// TRACKING 로그
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

	LOG_TRACKING,			// Track In/Out 상위 보고용
	LOG_MAX					// Log 폴더 총 갯수
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
	void SetPartition(BOOL bUse);	// 시간대로 파일명 분류

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
	void WriteNowAll();	// 현재 버퍼에 쌓여있는 것들을 모두 적는 함수
	void WriteNowAt(int nLogID);	// 현재 버퍼에 쌓여있는 것들을 모두 적는 함수
};
// inline CLogCtrl* GetLogCtrl() {	return CLogCtrl::Instance();	}
