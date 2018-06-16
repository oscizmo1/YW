// MT.h : MT 응용 프로그램에 대한 주 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error PCH에서 이 파일을 포함하기 전에 'stdafx.h'를 포함하십시오.
#endif

#include "resource.h"       // 주 기호
#include "MTDoc.h"
#include "MTView.h"

// CMTApp:
// 이 클래스의 구현에 대해서는 MT.cpp을 참조하십시오.
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

// 재정의
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
	void ChangeStatus(int nNewStatus);	// status 변환시에는 이함수로만 변환할것
	BOOL IsRunStatus();	// STATUS_RUN이나 STATUS_READY일때 TRUE
	//////////////////////////////////////////////////////////////////////////
	// Run Mode
public:
	RunMode	m_RunMode;
	void ChangeRunMode(RunMode newMode); // mode 변환시에는 이 함수로만 변환할것
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

// 구현
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

// Singleton 객체 호출 함수 일람
// 반드시 CMTApp::ExitInstance()에서 Release()를 호출하여야 한다.
// Release() 함수의 목적은 오로지 객체 delete만을 위함이므로
// 삭제되기 전에 뭔가를 구현하고 싶으면 별도 함수를 만들어 개별적으로 호출할것
// Release() 함수 안에 추가 코드 구현 금지
#include "DeviceCtl.h"	
inline CDeviceCtl* GetDeviceCtl() {	return CDeviceCtl::Instance();	}
#include "AlignCtl.h"	
inline CAlignCtl* GetAlignCtl() {	return CAlignCtl::Instance();	}
