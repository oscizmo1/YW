
#pragma once

// Thread 갯수만큼 아래 INDEX를 늘린다 [8/2/2016 OSC]
enum THREAD_INDEX
{
	THREAD_INDEX_MAIN_SEQUENCE,
	//kjpark 20161019 IO Check Thread 추가
	THREAD_INDEX_IO_SEQUENCE,
	//kjpark 20161024 압력센서 읽기 위한 쓰레드 구현
	THREAD_INDEX_FINGER_PRINTER,
	THREAD_INDEX_FDC_MOITOR,
	THREAD_INDEX_IO_ALARM_CHECK,	
	THREAD_INDEX_PDT_IF,
	THREAD_INDEX_MAX
};


class CThreadInitializer
{
public:
	CThreadInitializer();
	~CThreadInitializer();


protected:
	CWinThread* m_pThr[THREAD_INDEX_MAX];
	HANDLE		m_hThr[THREAD_INDEX_MAX];

public:
	BOOL		m_bThrExist[THREAD_INDEX_MAX];

	BOOL CreateThread(THREAD_INDEX index, AFX_THREADPROC pThr, LPVOID pParam);
	BOOL ReleaseThread(THREAD_INDEX index);
};
