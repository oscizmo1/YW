#pragma once

class CThreadLiveGrap : public CWnd  
{
public:
	CThreadLiveGrap();
	virtual ~CThreadLiveGrap();


public:
	void ThreadLiveGrapFunction();

public:
	BOOL m_bLiveGrapThreadRun;
	BOOL m_bLiveGrabThreadStart;

	// SearchPattern을 하기 위해 일시적으로 Live를 멈출 때 사용
	// SearchPattern 호출 전 m_bLivePause = TRUE 후 
	// m_dwTime이 변경되기까지 기다린 후 SearchPattern 실행
	// 이후 다시 m_bLivePause = FALSE
	BOOL m_bLivePause;
	// 루프를 돌았는지 확인하기 위한 변수
	//(Grab 정지명령 후 쓰레드가 이전 Grab을 완전히 끝냈는지 확인용)
	BOOL m_Resume;	
	// Live가 확실하게 일시정지될 때까지 기다리는 함수
	void WaitLivePause();
	BOOL CheckLivePause();

//////////////////////////////////////////////////////////////////////////
// CFormTeach창 활성화시 Grab Live 영상 때문에 Center/Side 식별자 추가 [2/9/2009 OSC]
// TRUE = Center	FALSE = Side
// 기본값 TRUE
public:
	void SetTeachCam(BOOL bSetCenter);
	BOOL m_bSetTeachCenter;
//////////////////////////////////////////////////////////////////////////
};
