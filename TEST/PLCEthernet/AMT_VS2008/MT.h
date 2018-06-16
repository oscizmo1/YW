// MT.h : MT ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error PCH���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����Ͻʽÿ�.
#endif

#include "resource.h"       // �� ��ȣ
#include "MTDoc.h"
#include "MTView.h"

// CMTApp:
// �� Ŭ������ ������ ���ؼ��� MT.cpp�� �����Ͻʽÿ�.
//

class CMTApp : public CWinApp
{
public:
	CMTApp();

	//////////////////////////////////////////////////////////////////////////
	// path
	CString		m_strExePath;
	CString		m_strRecipePath;
	CString		m_strAlignPath;
	CString		m_strLogPath;
	CString		m_strSystemPath;
	CString		m_strDCFPath;
	//////////////////////////////////////////////////////////////////////////
	// DCF
	double m_dFovX, m_dFovY;
	//////////////////////////////////////////////////////////////////////////

	int m_nOldStatus;

// ������
public:
	virtual BOOL InitInstance();

private:
	void SetDefaultPath();
	void SetTypes();


	//////////////////////////////////////////////////////////////////////////
	// Run Status
public:
	int eqStatus;
	int m_RunStatus;
	void Start();
	void Stop();
	void Reset();
	void ChangeStatus(int nNewStatus);	// status ��ȯ�ÿ��� ���Լ��θ� ��ȯ�Ұ�
	BOOL IsRunStatus();	// STATUS_RUN�̳� STATUS_READY�϶� TRUE
	//////////////////////////////////////////////////////////////////////////
	// Run Mode
public:
	RunMode	m_RunMode;
	void ChangeRunMode(RunMode newMode); // mode ��ȯ�ÿ��� �� �Լ��θ� ��ȯ�Ұ�
	//////////////////////////////////////////////////////////////////////////
	// Etc Flag
	BOOL m_bInspectionStart;
	BOOL m_bInspectionEnd;
	BOOL m_bInspectionResult;
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Max_Axis
	int m_MaxAxis;
	//////////////////////////////////////////////////////////////////////////

// ����
	DECLARE_MESSAGE_MAP()
public:
	virtual int ExitInstance();
};

extern CMTApp theApp;
extern inline CMTApp* GetApp() { return (CMTApp*)AfxGetApp(); }

class CFormCOMM;
extern CFormCOMM* gl_pComm;
inline CFormCOMM* GetComm() { return gl_pComm; }
extern CMTDoc* gl_pDoc;
inline CMTDoc* GetDoc() { return gl_pDoc; }
extern CPLCComm* gl_pPLCComm;
inline CPLCComm* GetMelsec()	{	return gl_pPLCComm;}

// Singleton ��ü ȣ�� �Լ� �϶�
// �ݵ�� CMTApp::ExitInstance()���� Release()�� ȣ���Ͽ��� �Ѵ�.
// Release() �Լ��� ������ ������ ��ü delete���� �����̹Ƿ�
// �����Ǳ� ���� ������ �����ϰ� ������ ���� �Լ��� ����� ���������� ȣ���Ұ�
// Release() �Լ� �ȿ� �߰� �ڵ� ���� ����
#include "DeviceCtl.h"	
inline CDeviceCtl* GetDeviceCtl() {	return CDeviceCtl::Instance();	}
#include "AlignCtl.h"	
inline CAlignCtl* GetAlignCtl() {	return CAlignCtl::Instance();	}
