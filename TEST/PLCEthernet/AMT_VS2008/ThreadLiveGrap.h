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

	// SearchPattern�� �ϱ� ���� �Ͻ������� Live�� ���� �� ���
	// SearchPattern ȣ�� �� m_bLivePause = TRUE �� 
	// m_dwTime�� ����Ǳ���� ��ٸ� �� SearchPattern ����
	// ���� �ٽ� m_bLivePause = FALSE
	BOOL m_bLivePause;
	// ������ ���Ҵ��� Ȯ���ϱ� ���� ����
	//(Grab ������� �� �����尡 ���� Grab�� ������ ���´��� Ȯ�ο�)
	BOOL m_Resume;	
	// Live�� Ȯ���ϰ� �Ͻ������� ������ ��ٸ��� �Լ�
	void WaitLivePause();
	BOOL CheckLivePause();

//////////////////////////////////////////////////////////////////////////
// CFormTeachâ Ȱ��ȭ�� Grab Live ���� ������ Center/Side �ĺ��� �߰� [2/9/2009 OSC]
// TRUE = Center	FALSE = Side
// �⺻�� TRUE
public:
	void SetTeachCam(BOOL bSetCenter);
	BOOL m_bSetTeachCenter;
//////////////////////////////////////////////////////////////////////////
};
