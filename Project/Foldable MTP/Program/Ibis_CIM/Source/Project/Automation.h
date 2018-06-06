

#pragma once

#include "afxmt.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "XComProCtrl.h"


typedef UINT	VID;
typedef VID	SVID;
typedef VID	ECID;
typedef VID	CEID;
typedef VID RCODE;
typedef VID JIGID;
typedef VID CELLPOSID;
typedef VID UNIT;

class CAutomation 
{
public:
	CAutomation(CXComProCtrl* pXCom=NULL);
	
	~CAutomation(void);

public:
	enum E_COMMUNICATION_STATE {
		E_DSIABLED = 0,
		E_ENABLED,
		E_COMMUNICATING,
		E_NOT_COMMUNICATING
	};
	enum { eMAX_Value = 5 };
	E_COMMUNICATION_STATE m_CommunicationState;

public:	
	CCriticalSection m_critiXCom;

public:

	CString		GetCorrectString(CString sSrc, int nLen);			// 사용가능한 글자들만 추린다.
		
public:
	
	int Msg_Proc();
	
	// S: Strim , F : Function

	void RxSxxF0(const int S);
	BOOL RxS0Fxx(const int f);

	void RxS1Fxx(const int F);	
	BOOL RxS1F1() ;
	BOOL RxS1F2() ;
	BOOL RxS1F3();
	BOOL RxS1F5();
	BOOL RxS1F11() ;
	BOOL RxS1F15() ;
	BOOL RxS1F17() ;
	BOOL TxS1F1();
	
	void RxS2Fxx(const int F);
	BOOL RxS2F13();
	BOOL RxS2F17();
	BOOL RxS2F23();
	BOOL RxS2F29();
	BOOL RxS2F31();
	BOOL RxS2F41();
	BOOL AckS2F42(CString RCMD, int nAck);	// S2F41에 대한 Ack. 운영을 갖다와야 하는 형식들은 여기서 보낸다
	BOOL RxS2F43();

	BOOL RxS2F103();
	BOOL TxS2F104();

	BOOL RxS2F105();

	BOOL TxS2F17();
	BOOL RxS2F18();

	void RxS3Fxx(const int F);
	BOOL RxS3F109();											// Cell Lot Information reply, POCH


	void RxS5Fxx(const int F);
	BOOL RxS5F0() ;
	BOOL TxS5F1(tag_Alarm Alarm);
	BOOL RxS5F2() ;
	BOOL RxS5F3() ;
	BOOL RxS5F103();


	void RxS6Fxx(const int F);
	BOOL RxS6F0() ;
	BOOL TxS6F1(int nIndex);
	BOOL RxS6F12();
	
	BOOL TxS6F205(CString strCellID, CString strOptionCode);	// 2016-01-03, Cell Lot Information Request, POCH
	BOOL RxS6F206();
 	BOOL TxS6F207(CString	strCellID);	//Cell Information Request
	BOOL RxS6F208();
	BOOL TxS6F209(CString	strCodeType); // Code Information Request
	BOOL RxS6F210();

	void RxS7Fxx(const int F);
	BOOL RxS7F0() ;
	BOOL RxS7F101();
	BOOL TxS7F107();
	BOOL RxS7F109();
	BOOL RxS7F23();
	BOOL RxS7F25();

	void RxS8Fxx(const int F);
	BOOL RxS8F0() ;
	BOOL RxS8F1();
	BOOL RxS8F3();
	
	void RxS10Fxx(const int F);
	BOOL RxS10F0() ;
	BOOL RxS10F5() ;



	BOOL TxS6F11(CEID ceid = 0, UNIT unitid = 0, CELLPOSID cellposid = 0); // Event Report Send
	// CEID:102 UNIT ID 전송 전용으로 만듦 [11/18/2017 OSC]
	BOOL TxS6F11_CEID102(int nJig, int nCh); // Event Report Send
	BOOL TxS6F11_CEID111(EQUIP_FUNCTION* function, UINT nIndexSize, CString strBYWHO); // Event Report Send
	// CEID:513 UNIT OPCALL Confirm [11/30/2017 OSC]
	BOOL TxS6F11_CEID513(int nJig, int nCh, CString strCellID, CString strProductID, CString strStepID); // Event Report Send
	// CEID:514 UNIT Interlock Confirm [11/30/2017 OSC]
	BOOL TxS6F11_CEID514(int nJig, int nCh, CString strCellID, CString strProductID, CString strStepID); // Event Report Send





public:
	CXComProCtrl* m_pXCom;

	enum E_SECS_TYPE {		
		E_HSMS
	};

	E_SECS_TYPE m_CommType;

public:
	short   m_nStrm, m_nFunc;
	short	m_nWbit, m_nDevId;
	long    m_lMsgId, m_lSysByte;
	char    m_szMsg[256];
	void	SecsLog(LPCSTR szMsg);
	
	BOOL	ByDial(CString str);
	CCriticalSection m_critiEventLock;
	CString m_cstrModuleID;
	CString m_sItemDefectNameValue[eMAX_Value];
	CString m_sItemPatternNameValue[eMAX_Value];
	CString m_sItemXY_Value[eMAX_Value];
	
	CString MakeECString(CString str, BOOL bUpper);		// ECID 문자열 처리
	CString MakeParaString(CString str, BOOL bUpper);	// 문자만 추려준다.

	void WriteTrackOutLog(CString strCellID, CString strInnerID, CString strDefectName);

	// CIM으로 받은 UnitID가 정상적인지 확인
	BOOL UnitID_Validation(CString strID);
	// CIM으로 받은 UnitID가 Unit이 맞는지 확인
	BOOL UnitID_IsUnit(CString strID);
};

