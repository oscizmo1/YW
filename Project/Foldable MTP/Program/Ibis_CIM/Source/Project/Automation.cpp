#include "StdAfx.h"

#include "../GaonNuri.h"
#include "automation.h"

#include <fstream>
#include <wtypes.h>

int	g_REQ_CommState=-1;

CAutomation::CAutomation(CXComProCtrl* pXCom)
{
	int i = 0;

	m_CommType = E_HSMS;
	m_CommunicationState = E_NOT_COMMUNICATING; // OFF-LINE (LOCAL)
	m_lMsgId = 0;
	m_lSysByte = 0;
	m_pXCom = pXCom;
	m_nDevId = 1;
}


CAutomation::~CAutomation(void)
{

}


// 2015-08-17, jhLee, 허용되는 글자가 아니면 문자열 종료처리한다.
// Interlock Message에 깨진 쓰레기 문자가 들어가는 현상 방어
CString CAutomation::GetCorrectString(CString sSrc, int nLen)
{
	CString strConvert = sSrc;

	// 글자수를 제한한다. 제공된 글자 수 혹은 실제 문자열 길이중이 적은쪽으로 수렴한다.
	int nStrLen = sSrc.GetLength();
	int nLimit = nStrLen;						// 비교 대상이 될 문자열 길이, 일단은 제공된 문자열 길이 그대로 지정
	TCHAR cChar;

	if ( nLen < nLimit ) nLimit = nLen;			// 지정한 길이가 더 작으면 지정한 길이를 사용한다.

	nStrLen = nLimit;							// 사용할 글자 길이

	for (int i = 0; i < nLimit; i++)
	{
		cChar = sSrc.GetAt(i);					// 한 글자를 빼낸다.

		// 화면에 표시가 가능한 글자가 아닌가 ?
		if ( (cChar < 0x20 ) || (cChar > 0x7e) )
		{
			nStrLen = i;						// 현재 글자(직전)까지만 사용한다.
			break;								// 순환을 마친다.
		}
	}

	return strConvert.Left( nStrLen );			// 허용된 길이까지만 사용하도록 한다.
}






int CAutomation::Msg_Proc()
{
	BOOL bExistMsg = FALSE;

	TRY
	{
		m_critiXCom.Lock();							// 간섭 방어 시작 
		m_CommunicationState = E_COMMUNICATING;		// CIM <-> MES 통신 연결상태 : REMOTE

		while( m_pXCom->LoadSecsMsg( &m_lMsgId,					// 기본값 : 0
										&m_nDevId,				// Device : 1
										&m_nStrm,				// Strm Recvie value
					 					&m_nFunc,				// Func Recive value
					 					&m_lSysByte,			// 기본값 : 0
					 					&m_nWbit ) >= 0 )		// 글자 크기 : 0
		{
			TRACE("LoadSecsMsg - MsgID:%d, Deviced:%d, Strm:%d, Func:%d, \n", m_lMsgId, m_nDevId, m_nStrm, m_nFunc);

			bExistMsg = TRUE;									// Message가 있다.

			sprintf_s( m_szMsg, sizeof(m_szMsg), "RMSG Dev(%d) S(%d) F(%d)", m_nDevId, m_nStrm, m_nFunc );
			GetLogCtrl()->AddBuf(LOG_CIM, "%s", m_szMsg);	// 일단 로그 기록

			if  ( m_nDevId = m_pXCom->GetDeviceID() )		// Deivce 연결 여부 확인
			{
				switch (m_nStrm) 
				{
					case 0:
						RxS0Fxx(m_nFunc);
						break;
					case 1:
						RxS1Fxx(m_nFunc);
						break;	
					case 2:
						RxS2Fxx(m_nFunc);
						break;
					case 3:
						RxS3Fxx(m_nFunc);
						break;
					case 5:
						RxS5Fxx(m_nFunc);
    					break;
					case 6:
						RxS6Fxx(m_nFunc);
						break;
					case 7:
						RxS7Fxx(m_nFunc);
						break;
					case 8:
						RxS8Fxx(m_nFunc);
						break;
					case 10:
						RxS10Fxx(m_nFunc);		        

						break;
					default:
						//SendS9(3); //Unrecognized Stream Type
						break;
				}

				// reset Operator REMOTE REQ
				if ( g_REQ_CommState == 1 )					// Remote 연결상태 리셋
					g_REQ_CommState = -1;

			}
			else
			{
				//SendS9(1);
			}

			m_pXCom->CloseSecsMsg( m_lMsgId );					// XCom 연결 해지
			bExistMsg = FALSE;									// 메시지 여부 해지

			TRACE("CloseSecMsg MsgID:%d\n",m_lMsgId);
		}
	}
	CATCH_ALL (e) 
	{
		if(bExistMsg)
		{
			m_pXCom->CloseSecsMsg( m_lMsgId );					// XCom 연결 해지
			TRACE("CloseSecMsg MsgID:%d\n",m_lMsgId);
		}

		m_critiXCom.Unlock();									// 간섭 방어 해지


		return CB_ERR;
	} 
	END_CATCH_ALL

	m_critiXCom.Unlock();										// 간섭 방어 해지


	return true;
}


BOOL CAutomation::RxS0Fxx(const int f)
{
	TRY
	{
		switch ( m_CommType )
		{
		case E_HSMS :
			Sleep(100);
			break;
		}
	}
	CATCH_ALL (e) 
	{
		return CB_ERR;
	} 
	END_CATCH_ALL

	return TRUE;
}


void CAutomation::RxSxxF0(const int S)
{

}

void CAutomation::RxS1Fxx(const int F)
{

	TRY
	{

		switch (F) {
			case 1:		RxS1F1();	break;
			case 2:		RxS1F2();	break;
			case 3:		RxS1F3();	break;
			case 5:		RxS1F5();	break;
			case 11:	RxS1F11();	break;
			case 15:	RxS1F15();	break;
			case 17:	RxS1F17();	break;

			default:
				//SendS9(5);
				break;
		}
	}
	CATCH_ALL (e) 
	{
		return ;
	} 
	END_CATCH_ALL
}

BOOL CAutomation::RxS1F1() 
{
	// HOST로부터 S1F1 신호를 받을면 
	// HOST에 S1F2 방식으로 Data를 보낸다 ( XCom 통신규약)

	long nReturn;
	CString  cstr;

	GetLogCtrl()->AddBuf(LOG_CIM, "Recv S1F1 Are You There Request");

//	cstr.Format("%s", g_pData->m_cMDLN);		// Setting Paramer에 등록된 EQNAME "AMT"
	cstr.Format("%s", g_pData->m_cEQPID);		// Setting Paramer에 등록된 EQNAME "AMT"

	cstr.Format("%s", g_pData->m_cSoftRev);		// Sofe Version 6자리 Data

	long lMkMsgID=0;

	TRY
	{
		switch ( m_CommType )
		{
		case E_HSMS :
			//////////////////////////////////////////////////////////////////////////
			//
			//  통신 규약 Format
			//
			//  <L,2
			//  1.<A[20]	$MDLN>		*EQPID
			//  2.<A[6]		#SOFTREV>	*Equipment Software Version
			//
			//  Strm : 1, Func : 2 , lSysByte : 0
			// 
			//////////////////////////////////////////////////////////////////////////
				
			m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 1, 2, m_lSysByte );	

			// <L,2
			m_pXCom->SetListItem( lMkMsgID, 2 );	
			
			// MDLN 
			cstr.Format("%s", g_pData->m_cEQPID);
		
			m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());

			// SOFTREV
			cstr.Format("%s", g_pData->m_cMDLN);
			m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());


			// XCOM 전송 여부 로그 기록
			if( (nReturn = m_pXCom->Send( lMkMsgID )) == 0 ) 
			{
				GetLogCtrl()->AddBuf(LOG_CIM, "Reply S1F2 MDLN[ %s ], SOFTREV[ %s ]", g_pData->m_cMDLN, g_pData->m_cSoftRev);
			}
			else 
			{
				sprintf_s( m_szMsg, sizeof(m_szMsg), "Fail to reply S1F2 (%d)", nReturn );
				GetLogCtrl()->AddBuf(LOG_CIM, "%s", m_szMsg);
			}

			break;

		}
	}
	CATCH_ALL (e) 
	{
		return CB_ERR;
	} 
	END_CATCH_ALL

	return TRUE;
}

BOOL CAutomation::RxS1F2() 
{
	CWnd *pWnd;
	pWnd = AfxGetApp()->GetMainWnd();
	//  Send a reply message S1F2
	GetLogCtrl()->AddBuf(LOG_CIM,"Recv S1F2 On Line Data");
	
	CString cstr;

	TRY
	{
		TxS2F17();
		((CMainFrame *)AfxGetMainWnd())->m_pViewCIM->SetTimer(S2F18REP_TIMER_PTRID, 2000, NULL); // View CIM Remote 변경 요청
	}
	CATCH_ALL (e) 
	{
		return CB_ERR;
	} 
	END_CATCH_ALL

	return TRUE;
}
BOOL CAutomation::RxS1F3()
{
	// 발생 조건 및 기능
	// HOST 판단하여 필요시 언제든지 발생 가능
	// 설비의 실시간 상태 값을 SVID로 관리하는 설비가 대응한다

	// S1F4 형식으로 보내준다

	int cpcnt = 0;
	CString strItem;
	long		nItems;
	long		lCnt;
	CString		strEQPID;
	CString		cstr;
	CString		strSVID,strSV, strSetSVID, strSetSV;
	CString		strMESSVID[SVID_MAX];
	CString		strMESSV[SVID_MAX];
	int			nCnt;
	int			nSVIDCnt;
	signed char cAsciiBuf[255];

	GetLogCtrl()->AddBuf(LOG_CIM,"Recv S1F3 Selected Equipment Status Request");

	nCnt = 0;
	long lMkMsgID = 0;
	TRY
	{
		switch ( m_CommType )
		{
		case E_HSMS :

			//////////////////////////////////////////////////////////////////////////
			// S1F3 스펙을 기준으로 값을 추출한다

				m_pXCom->GetListItem( m_lMsgId, &nItems );
				m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));
				strEQPID.Format("%s", cAsciiBuf);
				m_pXCom->GetListItem( m_lMsgId, &nItems );
				GetLogCtrl()->AddBuf(LOG_CIM,"Recv S1F3 SVID Count(%d)", nItems);

				nSVIDCnt = nItems;

				

				//////////////////////////////////////////////////////////////////////////
				// SVID Counting
				if(nSVIDCnt>=1)	
				{
					for (int i=0; i<nSVIDCnt; i++)
					{
						m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));
						strSVID.Format("%s", cAsciiBuf);
						GetLogCtrl()->AddBuf(LOG_CIM,"Recv S1F3 SVID(%s)", strSVID);
						strSVID.Trim(' ');
						for (int j=0; j<g_pData->m_FDC.m_nFDC_TotalCount; j++)
						{
							strSetSVID = g_pData->m_FDC.m_cSetSVID[j];
							strSetSVID.Trim(' ');
							if (strcmp(strSVID, strSetSVID)==0)
							{
								strMESSVID[nCnt] = strSetSVID;
								strSetSV = g_pData->m_FDC.m_cSetSV[j];
								strSetSV.Trim(' ');
								strMESSV[nCnt] = strSetSV;
								nCnt++;
									
							}
						}
					}
					if (nSVIDCnt!=nCnt)
					{
						nCnt=0;
					}
				}
				else
				{
					for (int z=0; z<g_pData->m_FDC.m_nFDC_TotalCount; z++)
					{
						strMESSVID[z] = g_pData->m_FDC.m_cSetSVID[z];
						strMESSV[z] = g_pData->m_FDC.m_cSetSV[z];
						nCnt++;
					}
				}

				//////////////////////////////////////////////////////////////////////////
				//
				//  S1F4 통신 규약 Format
				//
				//	<L,2
				//	1.<A[40]	$EQPID>						*EQPID
				//	2.<L,n									*n=SVID의 갯수
				//		1.<A[20]			$SVID>			*State Variable ID
				//		2.<A[20]			$SV>			*State Variable 값
				//		*L, n = 0 설비에 존재하는 SVID 전체를 의미
				//
				//  Strm : 1, Func : 4 , lSysByte : 0
				// 
				//////////////////////////////////////////////////////////////////////////

				if (strcmp(g_pData->m_cEQPID, strEQPID.Trim())!=0)
				{
					m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 1, 4, m_lSysByte );
					// <L,2
					m_pXCom->SetListItem( lMkMsgID, 2 );
					cstr  = strEQPID; 
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());
					m_pXCom->SetListItem( lMkMsgID, 0 );

					if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)				
					{
						long ret = m_pXCom->Send( lMkMsgID );
						GetLogCtrl()->AddBuf(LOG_CIM, "Reply S1F4 Send, ret= %d", ret);
					}


					return FALSE;

				}

			m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 1, 4, m_lSysByte );

				// <L,2
				m_pXCom->SetListItem( lMkMsgID, 2 );

				// EQPID
				cstr  = g_pData->m_cEQPID;
				m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());

				// nCnt = SVID 수량
				if (nCnt==0)
				{
					// <L,n
					m_pXCom->SetListItem( lMkMsgID, 0 ); // SVID 수량 = 0
					GetLogCtrl()->AddBuf(LOG_CIM, "Reply S1F3.Current has No SVID");
				}// end of if

				else
				{
					// <L,n
					m_pXCom->SetListItem( lMkMsgID, nCnt );	// SVID 수량 = nCnt


					for (int k=0; k<nCnt; k++)
					{
						// <L,2
						m_pXCom->SetListItem( lMkMsgID, 2 );

						// SVID
						cstr = strMESSVID[k];
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());
						
						// SV
						cstr = strMESSV[k];
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());

						GetLogCtrl()->AddBuf(LOG_CIM, "Reply S1F4 SVID(%s) SV(%s)", strMESSVID[k], strMESSV[k]);
					}//end of for						
				}//end of else

				//if(strcmp(g_pData->m_cCRST, "0")!=0)
				if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)				
			{
				long ret = m_pXCom->Send( lMkMsgID );
				GetLogCtrl()->AddBuf(LOG_CIM, "Reply S1F4 Send, ret= %d", ret);
			}



			break;
		}
	}
	CATCH_ALL (e) 
	{
		return CB_ERR;
	} 
	END_CATCH_ALL


	return TRUE;
}

BOOL CAutomation::RxS1F5()  //Formatted Status Request
{
	// 발생 조건 및 시점

	// SFCD = 1 " Equipment State"
	// 설비 내 EQPID 별 현재 상태 (Availability, Interlock, Move, Run State)를 조회 
	// 설비는 EQP 별 (Availability, Interlock, Move, Run State)를 실시간으로 관리

	// SFCD = 2 " Unit State"
	// 설비 내 EQPID하위 Unit별 현재 상태 (Availability, Interlock, Move, Run State)를 조회 
	// 설비는 EQP하위 Unit별 (Availability, Interlock, Move, Run State)를 실시간으로 관리

	// 2015-09-10 기준 
	// SFCD 3,4 사용하지 않음

	// SFCD = 3 "Material State"
	// 설비 EQP에 장착 되어있는 Material의 현재 상태(Material Parameter)를 조회
	// 설비는 EQ에 장착 되어 현재 사용중인 자재에 대해서 실시간으로 관리하여야한다

	// SFCD = 4 "Port State"
	// 설비 내 Port별 현재 상태(Port 관련 Parameter)를 조회
	
	// 해당 정보를 S1F6 형식으로 전송


	long		nItems;
	long		lCnt;
	CString		cstr;
	CString		strSFCD;
	CString		strEQPID;
	BOOL		bEqpID;
	long		nReturn;
	CString cstrModuleID,strVal;
	signed char cAsciiBuf[255];

	GetLogCtrl()->AddBuf(LOG_CIM,"Recv S1F5 Formatted Status Request");

	bEqpID = FALSE;
	long lMkMsgID = 0;
	TRY
	{
		// S1F5 내용으로 정보를 취득
		switch ( m_CommType )
		{
		case E_HSMS :
			m_pXCom->GetListItem( m_lMsgId, &nItems );
			m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));
				strSFCD.Format("%s", cAsciiBuf);

				m_pXCom->GetListItem( m_lMsgId, &nItems );
				if(nItems>=1)		//Equipment count
				{
					for(short i=1; i<=nItems; i++)
					{
						m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));
						cstr.Format("%s", cAsciiBuf);
						if(strcmp(g_pData->m_cEQPID, cstr.Trim())==0)
						{
							bEqpID = TRUE;
						}						
					}
				}
				else
				{
					bEqpID = TRUE;
				}
				
				//////////////////////////////////////////////////////////////////////////
				//
				//  S1F6 통신 규약 Format Equipment Stateus
				//
				//	<L,2												* SFCD Set
				//	1.<A[1]							#SFCD>				* State Form Code
				//	2.<L,n												* EQP State Info List
				//		1.<L,2											* EQP State Info
				//			1.<L,2										* EQP Control State Set
				//				1.<A[40] $EQPID>						* HOST가 요청한 Eqpument ID
				//				2.>A[1]	 $CRST>							* Online Control State
				//			2.<L,9										* EQP State Set
				//				1.<A[1]		$AVAILABILITYSTATE>			* 설비 가용상태
				//				2.<A[1]		$INTERLOCKSTATE>			* Interlock 가용상태
				//				3.<A[1]		$MOVESTATE>					* 설비의 동작 상태
				//				4.<A[1]		$RUNSTATE>					* 설비의 Cell 존재 유무
				//				5.<A[1]		$FRONTSTATE>				* Inline 설비의 경우 상류 설비 물류 상태
				//				6.<A[1]		$REARSTATE>					* Inline 설비의 경우 하류 설비 물류 상태
				//				7.<A[1]		$PP_SPLSTATE>				* Sample Run-Normal Run 상태 구분
				//				8.<A[20]	$REASONCODE>				* State에 관련 된 Reason Code
				//				9.<A[40]	$DESCRIPTION>				* State에 관련 된 설명
				//
				//  Strm : 1, Func : 6 , lSysByte : 0
				// 
				//////////////////////////////////////////////////////////////////////////

				switch ( atoi(strSFCD) )
				{
				case 1:		//Equipment Status Request
					{
						if(bEqpID)
						{
							m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 1, 6, m_lSysByte);
								// <L,2
								m_pXCom->SetListItem( lMkMsgID, 2);

									cstr = "1";
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());		//SFCD

									//<L,n
									m_pXCom->SetListItem( lMkMsgID, 1);

										//<L,2
										m_pXCom->SetListItem( lMkMsgID, 2);
											//<L,2
											m_pXCom->SetListItem( lMkMsgID, 2);

												//EQPID
												cstr = g_pData->m_cEQPID;
												m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());

												//CRST
												cstr = g_pData->m_cCRST;
												m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());

											//<L,9
											m_pXCom->SetListItem( lMkMsgID, 9);	

												// AVAILABILITY
												cstr = g_pData->m_cAvilability;
												m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	

												// INTERLOCK
												cstr = g_pData->m_cInterlock;
												m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	

												// MOVE STATUS
												cstr = g_pData->m_cMove;
												m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	

												// RUN STATUS
// 												if (ExistAllCellChk())
// 												{									
// 													strcpy_s( g_pData->m_cOldRun,g_pData->m_cRun);
// 													strcpy_s( g_pData->m_cRun,"2");
// 												}	
// 												else
// 												{
// 													strcpy_s( g_pData->m_cOldRun,g_pData->m_cRun);
// 													strcpy_s( g_pData->m_cRun,"1");
// 												}

												cstr = g_pData->m_cRun;
												m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	

												// FRONT STATUS	
												cstr = g_pData->m_cFront;
												m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());

												// REAR STATUS
												cstr = g_pData->m_cRear;
												m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	

												// SAMPLE LOT STATUS	
												cstr = g_pData->m_cSampleLot;
												m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	

												// RESONCODE
												cstr = g_pData->m_cReasonCode;
												m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	

												// DESCRIPTION
												cstr = g_pData->m_cDescription;
												m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	

						}
						else
						{
							m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 1, 6, m_lSysByte);

								// <L,2
								m_pXCom->SetListItem( lMkMsgID, 2);
									// <L,1
									cstr = "1";
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());		//SFCD

									m_pXCom->SetListItem( lMkMsgID, 0);

						}

						break;
					}

					//////////////////////////////////////////////////////////////////////////
					//
					//  S1F6 통신 규약 Format UINT Stateus
					//
					//	<L,2												* SFCD Set
					//	1.<A[1]							#SFCD>				* State Form Code
					//	2.<L,n												* EQP State Info List
					//		1.<L,3											* EQP State Info
					//			1.<L,2										* EQP Control State Set
					//				1.<A[40] $EQPID>						* HOST가 요청한 Eqpument ID
					//				2.>A[1]	 $CRST>							* Online Control State
					//			2.<L,9										* EQP State Set
					//				1.<A[1]		$AVAILABILITYSTATE>			* 설비 가용상태
					//				2.<A[1]		$INTERLOCKSTATE>			* Interlock 가용상태
					//				3.<A[1]		$MOVESTATE>					* 설비의 동작 상태
					//				4.<A[1]		$RUNSTATE>					* 설비의 Cell 존재 유무
					//				5.<A[1]		$FRONTSTATE>				* Inline 설비의 경우 상류 설비 물류 상태
					//				6.<A[1]		$REARSTATE>					* Inline 설비의 경우 하류 설비 물류 상태
					//				7.<A[1]		$PP_SPLSTATE>				* Sample Run-Normal Run 상태 구분
					//				8.<A[20]	$REASONCODE>				* State에 관련 된 Reason Code
					//				9.<A[40]	$DESCRIPTION>				* State에 관련 된 설명
					//			3.<L,m										* Unit List
					//				1.<L,2									* Unit Set
					//					1.<A[40]	$UNITID>				* EQP단위 설비의 UNIT별로 관리하는 UNIT ID
					//					2.<A[1]		$UNITST>				* UNIT의 가동정보 상태 정보값
					//
					//  Strm : 1, Func : 6 , lSysByte : 0
					// 
					//////////////////////////////////////////////////////////////////////////

				case 2:		//UNIT Status Request
					{
						GetLogCtrl()->AddBuf(LOG_CIM,"Unit Status Change Msg Creating..");
						CUnitStatusBank *pBank = &((CMainFrame *)AfxGetMainWnd())->m_pViewCIM->m_UnitStatusBank;

						m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 1, 6, m_lSysByte);

						//<L,2
						m_pXCom->SetListItem( lMkMsgID, 2);

						cstr = "2";
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());		//SFCD

						// <L,n
						m_pXCom->SetListItem( lMkMsgID, 1);

							//<L,3
							m_pXCom->SetListItem( lMkMsgID, 3);
								
								//<L,2
								m_pXCom->SetListItem( lMkMsgID, 2);

									//EQPID
									cstr = g_pData->m_cEQPID;
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());			

									//CRST
									cstr = g_pData->m_cCRST;
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());		

								//<L,9
								m_pXCom->SetListItem( lMkMsgID, 9);

									// AVAILABILITY
									cstr = g_pData->m_cAvilability;
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	

									// INTERLOCK
									cstr = g_pData->m_cInterlock;
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	

									// MOVE STATUS
									cstr = g_pData->m_cMove;
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	

									cstr = g_pData->m_cRun;
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	

									// FRONT STATUS		
									cstr = g_pData->m_cFront;
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());

									// REAR STATUS	
									cstr = g_pData->m_cRear;
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	

									// SAMPLE LOT STATUS
									cstr = g_pData->m_cSampleLot;
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());		

									// RESONCODE
									cstr = g_pData->m_cReasonCode;
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	

									// DESCRIPTION
									cstr = g_pData->m_cDescription;
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	

								// <L,m : // Unit List
								m_pXCom->SetListItem( lMkMsgID, pBank->GetUnitCount());	
								{
									int nFullCh = 0;
									for(int jig = 0; jig < JIG_ID_MAX; jig++)
									{
										for(int ch = 0; ch < JIG_CH_MAX; ch++)
										{
											// UNIT STATUS INFO
											m_pXCom->SetListItem( lMkMsgID, 2);	
											{
												//MODULEID
												cstr = pBank->m_Unit[jig][ch].m_strUnitID;
												m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());			

												//<L,9
												m_pXCom->SetListItem( lMkMsgID, 9);
												{
													// AVAILABILITY
													cstr.Format(_T("%d"), pBank->m_Unit[jig][ch].m_AvilabilityState);
													m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	

													// INTERLOCK
													cstr.Format(_T("%d"), pBank->m_Unit[jig][ch].m_InterlockState);
													m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	

													// MOVE STATUS
													cstr.Format(_T("%d"), pBank->m_Unit[jig][ch].m_MoveState);
													m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	

													// RUN STATUS
													cstr.Format(_T("%d"), pBank->m_Unit[jig][ch].m_RunState);
													m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	

													// FRONT STATUS		
													cstr.Format(_T("%d"), pBank->m_Unit[jig][ch].m_FrontState);
													m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());

													// REAR STATUS	
													cstr.Format(_T("%d"), pBank->m_Unit[jig][ch].m_RearState);
													m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	

													// SAMPLE LOT STATUS
													cstr.Format(_T("%d"), pBank->m_Unit[jig][ch].m_PP_SplState);
													m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());		

													// RESONCODE
													cstr = pBank->m_Unit[jig][ch].m_strResonCode;
													m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	

													// DESCRIPTION
													cstr = pBank->m_Unit[jig][ch].m_strDescrition;
													m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	
												}
											}
										}
									}
								}
						break;
					}

				case 3:		//Material Status Request
					m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 1, 6, m_lSysByte);

						m_pXCom->SetListItem( lMkMsgID, 2);

							cstr = strSFCD;
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());		//SFCD

							m_pXCom->SetListItem( lMkMsgID, 1);

								m_pXCom->SetListItem( lMkMsgID, 3);

									m_pXCom->SetListItem( lMkMsgID, 2);

										cstr = g_pData->m_cEQPID;
										m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());		//EQPID

										cstr = g_pData->m_cCRST;
										m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());		//CRST

									m_pXCom->SetListItem( lMkMsgID, 9);

										cstr = g_pData->m_cAvilability;
										m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// AVAILABILITY

										cstr = g_pData->m_cInterlock;
										m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// INTERLOCK

										cstr = g_pData->m_cMove;
										m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// MOVE STATUS

										cstr = g_pData->m_cRun;
										m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// RUN STATUS

										cstr = g_pData->m_cFront;
										m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// FRONT STATUS		

										cstr = g_pData->m_cRear;
										m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// REAR STATUS	

										cstr = g_pData->m_cSampleLot;
										m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// SAMPLE LOT STATUS	

										cstr = g_pData->m_cReasonCode;
										m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// RESONCODE

										cstr = g_pData->m_cDescription;
										m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// DESCRIPTION

									m_pXCom->SetListItem( lMkMsgID, 0);
					break;

				case 4:		//Equipment Port State
					m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 1, 6, m_lSysByte);

					m_pXCom->SetListItem( lMkMsgID, 2);
					{
						cstr = strSFCD;
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());		//SFCD
						m_pXCom->SetListItem( lMkMsgID, 1);
						{
							m_pXCom->SetListItem( lMkMsgID, 2);
							{
								m_pXCom->SetListItem( lMkMsgID, 2);
								{
									cstr = g_pData->m_cEQPID;
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());		//EQPID

									cstr = g_pData->m_cCRST;
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());		//CRST
								}

								m_pXCom->SetListItem( lMkMsgID, 0);	// Port 사용 안하면 0
							}
						}
					}
					break;


				case 5:		//Function Status
					m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 1, 6, m_lSysByte);

					m_pXCom->SetListItem( lMkMsgID, 2);

						cstr = strSFCD;
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());		//SFCD

						// 운영이랑 동기화되있으면 보고
						if(((CMainFrame *)AfxGetMainWnd())->m_pViewCIM->m_EquipFunction.m_bSyncStarted)
						{
							m_pXCom->SetListItem( lMkMsgID, 1);

								m_pXCom->SetListItem( lMkMsgID, 3);

									m_pXCom->SetListItem( lMkMsgID, 2);

										cstr = g_pData->m_cEQPID;
										m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());		//EQPID

										cstr = g_pData->m_cCRST;
										m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());		//CRST

								m_pXCom->SetListItem( lMkMsgID, 9);

									cstr = g_pData->m_cAvilability;
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// AVAILABILITY

									cstr = g_pData->m_cInterlock;
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// INTERLOCK

									cstr = g_pData->m_cMove;
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// MOVE STATUS

									cstr = g_pData->m_cRun;
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// RUN STATUS

									cstr = g_pData->m_cFront;
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// FRONT STATUS		

									cstr = g_pData->m_cRear;
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// REAR STATUS	

									cstr = g_pData->m_cSampleLot;
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// SAMPLE LOT STATUS	

									cstr = g_pData->m_cReasonCode;
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// RESONCODE

									cstr = g_pData->m_cDescription;
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// DESCRIPTION

								m_pXCom->SetListItem( lMkMsgID, MAX_EQUIP_FUNCTION_LIST);

									for(int i = 0; i < MAX_EQUIP_FUNCTION_LIST; i++)
									{
										m_pXCom->SetListItem( lMkMsgID, 2);

										cstr.Format(_T("%d"), i+1);
										m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// EFID

										cstr = ((CMainFrame *)AfxGetMainWnd())->m_pViewCIM->m_EquipFunction.m_FunctionList[i].EFST_Get();
										
										m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// EFST
									}
						}
						else
						{
							// 안되어있으면 보고 불가
							m_pXCom->SetListItem( lMkMsgID, 0);
						}
					break;

				case 6:		//Equipment Inventory State
					m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 1, 6, m_lSysByte);

					m_pXCom->SetListItem( lMkMsgID, 2);

					cstr = strSFCD;
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());		//SFCD

					m_pXCom->SetListItem( lMkMsgID, 0);	// 대응 안되면 0
					break;

				default:
						m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 1, 6, m_lSysByte);

							m_pXCom->SetListItem( lMkMsgID, 2);

								cstr = strSFCD;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());		//SFCD
								m_pXCom->SetListItem( lMkMsgID, 0);

// 								m_pXCom->SetListItem( lMkMsgID, 1);
// 
// 									m_pXCom->SetListItem( lMkMsgID, 3);
// 
// 											m_pXCom->SetListItem( lMkMsgID, 2);
// 
// 												cstr = g_pData->m_cEQPID;
// 												m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());		//EQPID
// 
// 												cstr = g_pData->m_cCRST;
// 												m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());		//CRST
// 
// 											m_pXCom->SetListItem( lMkMsgID, 9);
// 
// 												cstr = g_pData->m_cAvilability;
// 												m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// AVAILABILITY
// 
// 												cstr = g_pData->m_cInterlock;
// 												m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// INTERLOCK
// 
// 												cstr = g_pData->m_cMove;
// 												m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// MOVE STATUS
// 
// 												cstr = g_pData->m_cRun;
// 												m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// RUN STATUS
// 
// 												cstr = g_pData->m_cFront;
// 												m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// FRONT STATUS		
// 
// 												cstr = g_pData->m_cRear;
// 												m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// REAR STATUS	
// 
// 												cstr = g_pData->m_cSampleLot;
// 												m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// SAMPLE LOT STATUS	
// 
// 												cstr = g_pData->m_cReasonCode;
// 												m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// RESONCODE
// 
// 												cstr = g_pData->m_cDescription;
// 												m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// DESCRIPTION
// 
// 											m_pXCom->SetListItem( lMkMsgID, 0);

						break;

				}

				GetLogCtrl()->AddBuf(LOG_CIM,"S1F5 Formatted Status Data SFCD( %s )", strSFCD);
				//if(strcmp(g_pData->m_cCRST, "0")!=0)
				if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
				{
					nReturn = m_pXCom->Send( lMkMsgID );	// 제대로 갔는가?
					GetLogCtrl()->AddBuf(LOG_CIM,"S1F6(%s) send result( %d )", strSFCD, nReturn );
				}

	

			break;
		}
	}
	CATCH_ALL (e) 
	{
		return CB_ERR;
	} 
	END_CATCH_ALL

	return TRUE;
}

BOOL CAutomation::RxS1F11() // SV Name List
{
	// HOST가 설비의 특정 SVID List 및 현재의 값 (SV) 을 요청 
	// S1F12로 전달한다

	long		nItems;
	CString		strEQPID;
	long		lCnt;
	CString		strSVID, strSetSVID, strSetSVNAME;
	CString		strMESSVID[SVID_MAX];
	CString		strMESSVNAME[SVID_MAX];
	int			nCnt;
	CString cstr;
	int			nSVIDCnt;
	signed char cAsciiBuf[255];

	GetLogCtrl()->AddBuf(LOG_CIM,"Recv S1F11 Status Variable Name List Request");

	long lMkMsgID = 0;
	nCnt = 0;
	TRY
	{
		switch ( m_CommType )
		{
		case E_HSMS :
			m_pXCom->GetListItem( m_lMsgId, &nItems );
				m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));
				strEQPID.Format("%s", cAsciiBuf);
				m_pXCom->GetListItem( m_lMsgId, &nItems );
				nSVIDCnt = nItems;
				GetLogCtrl()->AddBuf(LOG_CIM, "S1F11 SVID COUNT(%d)", nSVIDCnt);
				if(nSVIDCnt>=1)		//SVID count
				{
					for (int i=0; i<nSVIDCnt; i++)
					{
						m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));
						strSVID.Format("%s", cAsciiBuf);
						strSVID.Trim(' ');
						for (int j=0; j<g_pData->m_FDC.m_nFDC_TotalCount; j++)
						{
							strSetSVID = g_pData->m_FDC.m_cSetSVID[j];
							strSetSVID.Trim(' ');
							if (strcmp(strSVID, strSetSVID)==0)
							{
								strMESSVID[nCnt] = strSetSVID;
								strSetSVNAME = g_pData->m_FDC.m_cSetSVNAME[j];
								strSetSVNAME.Trim(' ');
								strMESSVNAME[nCnt] = strSetSVNAME;
								nCnt++;
							}

						}
					}

					if (nSVIDCnt!=nCnt)
					{
						nCnt=0;
					}
				}else
				{
					for (int z=0; z<g_pData->m_FDC.m_nFDC_TotalCount; z++)
					{
						strMESSVID[z] = g_pData->m_FDC.m_cSetSVID[z];
						strMESSVNAME[z] = g_pData->m_FDC.m_cSetSVNAME[z];
						nCnt++;
					}
				}


				//////////////////////////////////////////////////////////////////////////
				//
				//  S1F12 통신 규약 Format 
				//
				//	<L,2												
				//		1.<A[40]	$EQPID>			*HOST가 요청할 설비의 EQPID
				//		2.<L,n						*n = SVID의 갯수
				//			1.<A[20[	$SVID>		*State Variable ID
				//
				//  Strm : 1, Func : 12 , lSysByte : 0
				// 
				//////////////////////////////////////////////////////////////////////////

				if (strcmp(g_pData->m_cEQPID, strEQPID.Trim())!=0)
				{
					m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 1, 4, m_lSysByte );
					// <L,2
					m_pXCom->SetListItem( lMkMsgID, 2 );
					cstr  = strEQPID;
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());
					m_pXCom->SetListItem( lMkMsgID, 0 );
					if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
					{
						long ret = m_pXCom->Send( lMkMsgID );	// 제대로 갔는지 확인
						GetLogCtrl()->AddBuf(LOG_CIM, "Reply S1F12, ret= %d", ret);
					}


					return FALSE;

				}

				m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 1, 12, m_lSysByte );

				//<L,2
				m_pXCom->SetListItem( lMkMsgID, 2 );

				//EQPID
				cstr  = g_pData->m_cEQPID;
				m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());

				// SVID 갯수
				if (nCnt==0)
				{
					//<L,0>
					m_pXCom->SetListItem( lMkMsgID, 0 );
					GetLogCtrl()->AddBuf(LOG_CIM, "Reply S1F11.Current has No SVID");
				} 
				else
				{
					//<L,n>
					m_pXCom->SetListItem( lMkMsgID, nCnt );

					for (int k=0; k<nCnt; k++)
					{
						//<L,2>
						m_pXCom->SetListItem( lMkMsgID, 2 );

						// SVID
						cstr = strMESSVID[k];
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());

						// SV
						cstr = strMESSVNAME[k];
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());

						GetLogCtrl()->AddBuf(LOG_CIM, "S1F11 SVID(%s) SVNAME(%s)", strMESSVID[k], strMESSVNAME[k]);
					}
				}	

				// CIM State 0:OFFLINE, 1:REMOTE , 2:LOCAL

				//if(strcmp(g_pData->m_cCRST, "0")!=0)
				if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
				{
					long ret = m_pXCom->Send( lMkMsgID );	// 제대로 갔는지 확인
					GetLogCtrl()->AddBuf(LOG_CIM, "Reply S1F12, ret= %d", ret);
				}

	

			break;
		}
	}
	CATCH_ALL (e) 
	{
		return CB_ERR;
	} 
	END_CATCH_ALL


	return TRUE;
}

BOOL CAutomation::RxS1F15() // OFF-LINE (LOCAL) Mode Set
{
	// HOST가 설비의 OFFLINE Mode로 변경을 요청하기 위한 상태

	// 즉 HOST가 장비에게 CIM 연결상태를 물어본다

	// S1F16으로 전달


	GetLogCtrl()->AddBuf(LOG_CIM,"Recv S1F15 Request OFF-LINE");

	long	nReturn;
	short	nCRST;
	signed char cAsciiBuf[255];
	CString	cstr;
	long	nItems;
	BOOL	bValid;
	long	lCnt;
	CString	strEqpID;
	CString	strCRST;
	long lMkMsgID = 0;

	// ONFACK
	// 0 : Accepted.
	// 1 : Not Accepted.

	bValid = FALSE;

	TRY
	{
		switch ( m_CommType )
		{
		case E_HSMS :
			nCRST = atoi(g_pData->m_cCRST);
			switch ( nCRST)	//0:OFF-LINE, 1:REMOTE, 2:LOCAL
			{
			case 1:
			case 2:
				{
					m_pXCom->GetListItem( m_lMsgId, &nItems );
					if(nItems>=1)		//Equipment count
					{
						for(short i=1; i<=nItems; i++)
						{
							m_pXCom->GetListItem( m_lMsgId, &nItems );
								m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//EQPID
								strEqpID.Format("%s", cAsciiBuf);
								m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//CRST
								strCRST.Format("%s", cAsciiBuf);
								if(strcmp(g_pData->m_cEQPID, strEqpID.Trim())==0 && "0"==strCRST)
								{
									bValid = TRUE;
									break;
								}						
						}
					}
					else
					{
						bValid = TRUE;
					}

					//////////////////////////////////////////////////////////////////////////
					//
					//  S1F16 통신 규약 Format 
					//
					//	<L,n							* HOST가 요청한 EQP OFF 갯수
					//		1.<L,3						* EQP OFF SET
					//			1.<A[40]	$EQPID>		* HOST가 요청한 EQPID
					//			2.<A[1]		$CRST>		* Online Control State
					//			3.<A[1]		$OFLACK>	* Acknowlege Code
					//
					//  Strm : 1, Func : 16 , lSysByte : 0
					// 
					//////////////////////////////////////////////////////////////////////////

					if(bValid)
					{
						m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 1, 16, m_lSysByte );

							//<L,1
							m_pXCom->SetListItem( lMkMsgID, 1);

								//<L,3
								m_pXCom->SetListItem( lMkMsgID, 3);

									//EQPID
									cstr = g_pData->m_cEQPID; 
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), strlen(cstr));	

									//CRST
									cstr = "0"; 
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), strlen(cstr));	

									//OFLACK 0: Accepted	1:Not Accepted
									cstr = "0"; 
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), strlen(cstr));	

						GetLogCtrl()->AddBuf(LOG_CIM,"Recv OFFLINE Request : Accepted" );
						
						nReturn = m_pXCom->Send( lMkMsgID );
						GetLogCtrl()->AddBuf(LOG_CIM, "Reply S1F16 result( %d )", nReturn );

						((CMainFrame *)AfxGetMainWnd())->m_pViewCIM->ControlStateChange(E_CRST_OFFLINE);
						break;

					}
					else
					{
						m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 1, 16, m_lSysByte );
							//<L,1
							m_pXCom->SetListItem( lMkMsgID, 1);
								
								//<L,3
								m_pXCom->SetListItem( lMkMsgID, 3);

									//EQPID
									cstr = g_pData->m_cEQPID; 
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), strlen(cstr));	

									//CRST
									cstr = "0"; 
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), strlen(cstr));	

									//OFLACK 0: Accepted 1:Not Accepted
									cstr = "1"; 
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), strlen(cstr));	

						GetLogCtrl()->AddBuf(LOG_CIM,"Recv OFFLINE Request : Not Accepted" );
						
						nReturn = m_pXCom->Send( lMkMsgID ); // 제대로 갔는지 확인
						GetLogCtrl()->AddBuf(LOG_CIM, "Reply S1F16 result( %d )", nReturn );
						
						break;
					}
				}
			case 0:
				{
					m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 1, 16, m_lSysByte );
						m_pXCom->SetListItem( lMkMsgID, 1);
							m_pXCom->SetListItem( lMkMsgID, 3);
								cstr = g_pData->m_cEQPID; 
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), strlen(cstr));	//EQPID
								cstr = "0"; 
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), strlen(cstr));	//CRST
								cstr = "1"; 
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), strlen(cstr));	//OFLACK 0: Accepted 1:Not Accepted
					GetLogCtrl()->AddBuf(LOG_CIM,"Recv OFFLINE Request : Not Accepted" );
					
					nReturn = m_pXCom->Send( lMkMsgID );
					GetLogCtrl()->AddBuf(LOG_CIM, "Reply S1F16 result( %d )", nReturn );
					
					break;
				}
			}



			break;
		}		
	}
	CATCH_ALL (e) 
	{
		return CB_ERR;
	} 
	END_CATCH_ALL

	return TRUE;
}

BOOL CAutomation::RxS1F17() // ONLINE (REMOTE) Mode Set
{
	// HOST가 설비의 Online Mode로 변경을 요청

	// S1F18로 전송

	GetLogCtrl()->AddBuf(LOG_CIM,"Recv S1F17 Request ON-LINE");

// 	0: Accepted,
// 	1: Not Accepted,
// 	2: Already ON-LINE LOCAL,
// 	3: Already ON-LINE REMOTE.

	long	nReturn;
	short	nCRST;
	signed char cAsciiBuf[255];
	CString	cstr;
	long	nItems;
	BOOL	bValid;
	BOOL	bCheckCRST;
	long	lCnt;
	CString	strEqpID;
	CString	strCRST;
	CString	strCRSTRep;
	long lMkMsgID = 0;


	bValid = FALSE;
	bCheckCRST = FALSE;

	TRY
	{
		switch ( m_CommType )
		{
		case E_HSMS :
			nCRST = atoi(g_pData->m_cCRST);
			switch ( nCRST)	//0:OFF-LINE, 1:REMOTE, 2:LOCAL
			{


				//////////////////////////////////////////////////////////////////////////
				//
				//  S1F18 통신 규약 Format ON-LINE Acknowelege
				//
				//	<L,n							* HOST가 요청한 요청한 EQP ONLINE 갯수
				//		1.<L,3						* EQP ON SET
				//			1.<A[40]	$EQPID>		* HOST가 요청한 EQPID
				//			2.<A[1]		$CRST>		* Online Control State
				//			2.<A[1]		$ONLACK>	* Acknowledge Code
				//
				//  Strm : 1, Func : 18 , lSysByte : 0
				// 
				//////////////////////////////////////////////////////////////////////////


			//////////////////////////////////////////////////////////////////////////
			// OFFLINE
			case 0:	
				{
					m_pXCom->GetListItem( m_lMsgId, &nItems );
					if(nItems>=1)		//Equipment count
					{
						for(short i=1; i<=nItems; i++)
						{
							m_pXCom->GetListItem( m_lMsgId, &nItems );
								m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//EQPID
								strEqpID.Format("%s", cAsciiBuf);
								m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//CRST
								strCRST.Format("%s", cAsciiBuf);
								if((strcmp(g_pData->m_cEQPID, strEqpID.Trim())==0 && "1"==strCRST) || (strcmp(g_pData->m_cEQPID, strEqpID.Trim())==0 && "2"==strCRST))
								{
									bValid = TRUE;
									strCRSTRep = strCRST;
									break;
								}						
						}
					}
					else
					{
						bValid = FALSE;
					}


					


					if(bValid)
					{
						m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 1, 18, m_lSysByte );

							//<L,1
							m_pXCom->SetListItem( lMkMsgID, 1);
									
								//<L,3
								m_pXCom->SetListItem( lMkMsgID, 3);

									//EQPID
									cstr = g_pData->m_cEQPID; 
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), strlen(cstr));	
									cstr = strCRSTRep; 

									//CRST
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), strlen(cstr));	
									cstr = "0"; 

									//ONLACK
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), strlen(cstr));	

						GetLogCtrl()->AddBuf(LOG_CIM,"Recv ONLINE Request : Accepted" );
						nReturn = m_pXCom->Send( lMkMsgID );
						GetLogCtrl()->AddBuf(LOG_CIM, "Reply S1F18 result( %d )", nReturn );

						// 연결상태 체크
						if(strCRSTRep=="1")
						{
							((CMainFrame *)AfxGetMainWnd())->m_pViewCIM->ControlStateChange( E_CRST_REMOTE );
						}
						else
						{
							((CMainFrame *)AfxGetMainWnd())->m_pViewCIM->ControlStateChange( E_CRST_LOCAL );
						}
						break;

					}
					else
					{
						m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 1, 16, m_lSysByte );

							//<L,1
							m_pXCom->SetListItem( lMkMsgID, 1);

								//<L,3
								m_pXCom->SetListItem( lMkMsgID, 3);

									//EQPID
									cstr = g_pData->m_cEQPID; 
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), strlen(cstr));	

									//CRST
									cstr = g_pData->m_cCRST; 
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), strlen(cstr));	

									//ONLACK
									cstr = "1"; 
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), strlen(cstr));	

						GetLogCtrl()->AddBuf(LOG_CIM,"Recv ONLINE Request : Not Accepted" );
						nReturn = m_pXCom->Send( lMkMsgID );
						GetLogCtrl()->AddBuf(LOG_CIM, "Reply S1F18 result( %d )", nReturn );
						
						break;
					}
				} // end of OFFLINE

				break;



				
			//////////////////////////////////////////////////////////////////////////
			// REMOTE

			case 1:
				{
					m_pXCom->GetListItem( m_lMsgId, &nItems );
					if(nItems>=1)		//Equipment count
					{
						for(short i=1; i<=nItems; i++)
						{
							m_pXCom->GetListItem( m_lMsgId, &nItems );

								//EQPID
								m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	
								strEqpID.Format("%s", cAsciiBuf);

								//CRST
								m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	
								strCRST.Format("%s", cAsciiBuf);

								if((strcmp(g_pData->m_cEQPID, strEqpID.Trim())==0 && "1"==strCRST) || (strcmp(g_pData->m_cEQPID, strEqpID.Trim())==0 && "2"==strCRST))
								{
									bValid = TRUE;
									strCRSTRep = strCRST;

									if(g_pData->m_cCRST==strCRST)
										bCheckCRST = FALSE;
									else
										bCheckCRST = TRUE;
									
									break;
								}						
						}
					}
					else
					{
						bValid = FALSE;
					}

					if((bValid)&&(bCheckCRST))
					{
						m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 1, 18, m_lSysByte );

							m_pXCom->SetListItem( lMkMsgID, 1);

								m_pXCom->SetListItem( lMkMsgID, 3);

									//EQPID
									cstr = g_pData->m_cEQPID; 
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), strlen(cstr));	

									//CRST
									cstr = strCRSTRep; 
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), strlen(cstr));	

									//ONLACK
									cstr = "0"; 
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), strlen(cstr));	

						GetLogCtrl()->AddBuf(LOG_CIM,"Recv ONLINE Request : Accepted" );

						nReturn = m_pXCom->Send( lMkMsgID );
						GetLogCtrl()->AddBuf(LOG_CIM, "Reply S1F18 result( %d )", nReturn );

						((CMainFrame *)AfxGetMainWnd())->m_pViewCIM->ControlStateChange( E_CRST_LOCAL );
						break;

					}
					else
					{
						m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 1, 16, m_lSysByte );

							m_pXCom->SetListItem( lMkMsgID, 1);

								m_pXCom->SetListItem( lMkMsgID, 3);

									//EQPID
									cstr = g_pData->m_cEQPID; 
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), strlen(cstr));	

									//CRST
									cstr = g_pData->m_cCRST; 
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), strlen(cstr));	

									if(bCheckCRST)
										cstr = "1";		//1: Not Accepted,
									else
										if(bValid)
											cstr = "3";		//Already ON-LINE REMOTE.
										else
											cstr = "1";		//Not Accepted
									
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), strlen(cstr));	//ONLACK
						
						//if(bCheckCRST)
							GetLogCtrl()->AddBuf(LOG_CIM,"Recv ONLINE Request : Not Accepted" );
						//else
							GetLogCtrl()->AddBuf(LOG_CIM,"Recv ONLINE Request : Already ON-LINE REMOTE" );
					
						nReturn = m_pXCom->Send( lMkMsgID );
						GetLogCtrl()->AddBuf(LOG_CIM, "Reply S1F18 result( %d )", nReturn );			
									
						break;
					}
				} // end of REMOTE

				break;


			//////////////////////////////////////////////////////////////////////////
			// LOCAL
			case 2:
				{
					m_pXCom->GetListItem( m_lMsgId, &nItems );
					if(nItems>=1)		//Equipment count
					{
						for(short i=1; i<=nItems; i++)
						{
							m_pXCom->GetListItem( m_lMsgId, &nItems );
	
								//EQPID
								m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	
								strEqpID.Format("%s", cAsciiBuf);

								//CRST
								m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	
								strCRST.Format("%s", cAsciiBuf);

								if((strcmp(g_pData->m_cEQPID, strEqpID.Trim())==0 && "1"==strCRST) || (strcmp(g_pData->m_cEQPID, strEqpID.Trim())==0 && "2"==strCRST))
								{
									bValid = TRUE;
									strCRSTRep = strCRST;

									if(g_pData->m_cCRST==strCRST)
										bCheckCRST = FALSE;
									else
										bCheckCRST = TRUE;
									
									break;
								}						
						}
					}
					else
					{
						bValid = FALSE;
					}

					if((bValid)&&(bCheckCRST))
					{
						m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 1, 18, m_lSysByte );

							m_pXCom->SetListItem( lMkMsgID, 1);

								m_pXCom->SetListItem( lMkMsgID, 3);

									//EQPID
									cstr = g_pData->m_cEQPID; 
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), strlen(cstr));	

									//CRST
									cstr = strCRSTRep; 
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), strlen(cstr));	

									//ONLACK
									cstr = "0"; 
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), strlen(cstr));	

						GetLogCtrl()->AddBuf(LOG_CIM,"Recv ONLINE Request : Accepted" );
						nReturn = m_pXCom->Send( lMkMsgID );

						GetLogCtrl()->AddBuf(LOG_CIM, "Reply S1F18 result( %d )", nReturn );

						((CMainFrame *)AfxGetMainWnd())->m_pViewCIM->ControlStateChange( E_CRST_REMOTE );
						break;

					}
					else
					{
						m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 1, 16, m_lSysByte );

							m_pXCom->SetListItem( lMkMsgID, 1);

								m_pXCom->SetListItem( lMkMsgID, 3);

									//EQPID
									cstr = g_pData->m_cEQPID; 
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), strlen(cstr));	

									//CRST
									cstr = g_pData->m_cCRST; 
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), strlen(cstr));	

									if(bCheckCRST)
										cstr = "1";		//1: Not Accepted,
									else
										if(bValid)
											cstr = "2";		//Already ON-LINE LOCAL
										else
											cstr = "1";		//Not Accepted

									
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), strlen(cstr));	//ONLACK
						

			
							GetLogCtrl()->AddBuf(LOG_CIM,"Recv ONLINE Request : Not Accepted" );
							GetLogCtrl()->AddBuf(LOG_CIM,"Recv ONLINE Request : Already ON-LINE LOCAL" );
					
						nReturn = m_pXCom->Send( lMkMsgID );
						GetLogCtrl()->AddBuf(LOG_CIM, "Reply S1F18 result( %d )", nReturn );			
									
						break;
					}
				} //end of LOCAL

				break;
			}//end of swich


			break;
		}	//end of swich
	}//end of TRY
	CATCH_ALL (e) 
	{
		return CB_ERR;
	} 
	END_CATCH_ALL

	return TRUE;
}

BOOL CAutomation::TxS1F1()
{
	GetLogCtrl()->AddBuf(LOG_CIM,"TxS1F1 - Are You There Request");
	long nReturn;

	long lMkMsgID = 0;
	TRY
	{
		m_lSysByte = 0;

		switch ( m_CommType )
		{
		case E_HSMS :
 
			m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 1, 1, m_lSysByte );

			if( (nReturn = m_pXCom->Send( lMkMsgID )) == 0 ) 
			{
				g_REQ_CommState = 1;	// CIM State = Remoted

				GetLogCtrl()->AddBuf(LOG_CIM, "Send S1F1 successfully" );
			}
			else 
			{
				sprintf_s( m_szMsg, sizeof(m_szMsg), "[HOST <-] Send S1F1 Message Fail, Return :%d", nReturn );
				GetLogCtrl()->AddBuf(LOG_CIM, m_szMsg );
			}
			break;
		}
	}
	CATCH_ALL (e) 
	{
		return CB_ERR;
	} 
	END_CATCH_ALL

	return TRUE;
}

void CAutomation::RxS2Fxx(const int F)
{
	//int nTemp;

	TRY
	{
		switch (F) {
			case 0 :	RxSxxF0(2);			break;
			case 13 :	RxS2F13();			break;
			case 18:	RxS2F18();			break;
			case 23:	RxS2F23();			break;
			case 29:	RxS2F29();			break;
			case 31:	RxS2F31();			break;
			case 41:	RxS2F41();			break;
			case 43:	RxS2F43();			break;
			case 103:	RxS2F103();			break;
			case 105:	RxS2F105();			break;

			default:
				GetLogCtrl()->AddBuf(LOG_CIM,"Recv S2F%d", F);
				//SendS9(5);
				break;
		}
	}
	CATCH_ALL (e) 
	{
		return ;
	} 
	END_CATCH_ALL

}

BOOL CAutomation::RxS2F13()
{
	//HOST가 현재 설비의 특정 ECID 관련 요청
	//->S2F14 


	GetLogCtrl()->AddBuf(LOG_CIM,"Recv S2F13 New Equipment Constants Request(ECR)");

	long	nItems;
	long	lCnt;
	signed char cAsciiBuf[255];
	CString strEqpID, strTemp1, strECID;
	BOOL	bValid = TRUE;
	int		nECIDCnt = 0;
	int		nValidCnt = 0;
	CList<CString, CString> listECID;
	CList<CString, CString> listECNAME;
	CList<CString, CString> listECVALUE;
	CString	strECName, strECValue;
	CString	cstr;
	long lMkMsgID;
	lMkMsgID = 0;

	CMainFrame* pMainFrm = (CMainFrame*)::AfxGetMainWnd();
	//BOOL bFind = FALSE;

	TRY 
	{
		switch (m_CommType)
		{
		case E_HSMS:
			{
				m_pXCom->GetListItem( m_lMsgId, &nItems );
					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//EQPID
					strEqpID.Format("%s", cAsciiBuf);
					m_pXCom->GetListItem( m_lMsgId, &nItems );			//ECID COUNT
					nECIDCnt = nItems;
					strEqpID.Trim(' ');
					if(strcmp(g_pData->m_cEQPID, strEqpID)!=0)
					{
						bValid = FALSE;
						GetLogCtrl()->AddBuf(LOG_CIM,"S2F13 EQPID(%s) IS NOT MATCH.", strEqpID);
					}
					else
					{
						if (nECIDCnt>0)
						{
							for (int i=0; i<nECIDCnt; i++)
							{
								m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//ECID
								strECID.Format("%s", cAsciiBuf);
								for (int j=0; j<g_pData->m_ECID.m_nECID_TotalCount; j++)
								{
// 									if (bFind)
// 									{
// 										bFind = FALSE;
// 										break;
// 									}
									if (strcmp(g_pData->m_ECID.m_cECID[j], strECID)==0)
									{
										strECID = g_pData->m_ECID.m_cECID[j];
										strECName.Format("%s", g_pData->m_ECID.m_cECNAME[j]);
										strECValue = g_pData->m_ECID.m_cECV[j];
										listECID.AddTail(strECID);
										listECNAME.AddTail(strECName);
										listECVALUE.AddTail(strECValue);
										TRACE("ECID-%s , ECNAME-%s, ECVALUE-%s\n", strECID, strECName, strECValue);
										nValidCnt++;
										//bFind = TRUE;
										break;
									}
								}
							}

							if (nECIDCnt!=nValidCnt)
							{
								bValid = FALSE;
								GetLogCtrl()->AddBuf(LOG_CIM,"S2F13 ECID IS NOT MATCH. HOST(%d), EQ(%d)", nECIDCnt, nValidCnt);
							}
						}
						else if(nECIDCnt==0)	//ALL
						{
							for (int j=0; j<g_pData->m_ECID.m_nECID_TotalCount; j++)
							{
								strECID = g_pData->m_ECID.m_cECID[j];
								strECName.Format("%s", g_pData->m_ECID.m_cECNAME[j]);
								strECValue = g_pData->m_ECID.m_cECV[j];
								listECID.AddTail(strECID);
								listECNAME.AddTail(strECName);
								listECVALUE.AddTail(strECValue);
								TRACE("ECID-%s , ECNAME-%s, ECVALUE-%s\n", strECID, strECName, strECValue);
							}
					
						}
						else
							bValid = FALSE;
					}




					//////////////////////////////////////////////////////////////////////////
					//
					//  S2F14 통신 규약 Format 
					//
					//	<L,n							* L,n	= HOST에서 요청하는 Eqpipment Constant ID의 갯수
					//		1.A[40]		$ECV1>			* Equipment Constant 값
					//			......
					//		n.A[40]		$ECVn>			* Equipment Constant 값
					//
					//  Strm : 2, Func : 14 , lSysByte : 0
					// 
					//////////////////////////////////////////////////////////////////////////

					if (bValid)
					{
						m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 2, 14, m_lSysByte);

							//<L,n
							m_pXCom->SetListItem( lMkMsgID, listECID.GetCount());	//ECV Count

							POSITION pos1 = listECID.GetHeadPosition();
							POSITION pos2 = listECNAME.GetHeadPosition();
							POSITION pos3 = listECVALUE.GetHeadPosition();

							for(int i=0; i<listECID.GetCount(); i++)
							{
								cstr = listECVALUE.GetNext(pos3); cstr = MakeECString(cstr, TRUE);

								// ECM 항목
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());
								//GetLogCtrl()->AddBuf(LOG_CIM,"S2F13 ECVALUE(%s)", cstr);
							}
		

					}
					else
					{
						m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 2, 14, m_lSysByte);

							//<L,n
							m_pXCom->SetListItem( lMkMsgID, 0);	//ECV Count
							GetLogCtrl()->AddBuf(LOG_CIM,"S2F13 ECVALUE List 0");
					}

					//if(strcmp(g_pData->m_cCRST, "0")!=0)
					if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
					{	
						long ret = m_pXCom->Send( lMkMsgID );	// 제대로 전달 했는가?
						GetLogCtrl()->AddBuf(LOG_CIM, "S2F14 Equipment Constant Data Send, ret=%d", ret );
					}

					//

				break;
			}
		}

		return TRUE;
	}
	CATCH (CMemoryException, e)
	{
		return CB_ERR;	
	}
	END_CATCH

	return TRUE;
}

BOOL CAutomation::RxS2F17() // Data and Time Request
{
	GetLogCtrl()->AddBuf(LOG_CIM,"Recv S2F17 Date & Time Request");

	CString strTime;
	CTime t;


	long lMkMsgID = 0;
	switch ( m_CommType )
	{
	case E_HSMS :
		m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 2, 18, m_lSysByte);

		t = CTime::GetCurrentTime();
		strTime.Format("%02d%02d%02d%02d%02d%02d", t.GetYear(),t.GetMonth(),t.GetDay(), t.GetHour(),t.GetMinute(),t.GetSecond());
		m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)strTime.GetString(), strTime.GetLength());
		//if(strcmp(g_pData->m_cCRST, "0")!=0)
		if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
		{
			m_pXCom->Send( lMkMsgID );
		}
		
		break;
	}
	return TRUE;
}

BOOL CAutomation::TxS2F17()
{
	// HOST와 최초 통신 연결 시, HOST의 S1F2 Message를 전송받은 경우 Time 요청을 보낸다.

	// Strm : 2, Func : 17 , lSysByte : 0
	
	long lMkMsgID=0;
	long nReturn;

	TRY
	{
		switch ( m_CommType )
		{
		case E_HSMS :
			m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 2, 17, 0);
			GetLogCtrl()->AddBuf(LOG_CIM,"TxS2F17 Data and Time Request");

			nReturn = m_pXCom->Send( lMkMsgID ); // XCom에 전달
			GetLogCtrl()->AddBuf(LOG_CIM, "TxS2F17 Date & Time Request send result( %d )", nReturn );

			break;
		}
	}
	CATCH_ALL (e) 
	{
		return CB_ERR;
	} 
	END_CATCH_ALL

		return TRUE;
}

BOOL CAutomation::RxS2F18() // Data and Time Data
{
	// HOST시간변경시 설비에서 해당 시간을 받아온다.


	GetLogCtrl()->AddBuf(LOG_CIM,"Recv S2F18 Date and Time Data");

	//short nOldCRST;
	CString strTime;

	long lCnt;
	char szBuff[256];
	SYSTEMTIME st;
	CString strTiack;
	CString str;
	signed char cAsciiBuf[255];
	((CMainFrame *)AfxGetMainWnd())->m_pViewCIM->KillTimer(S2F18REP_TIMER_PTRID);

	switch ( m_CommType )
	{
	case E_HSMS :
		m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));
		strTime.Format("%s", cAsciiBuf);

		if(strTime.GetLength()==14)
		{
			sprintf_s(szBuff, sizeof(szBuff), "%s", strTime);
			memset(&st, 0, sizeof(st));

			szBuff[4] = 0x00;

			szBuff[0] = strTime.GetAt(0);
			szBuff[1] = strTime.GetAt(1);
			szBuff[2] = strTime.GetAt(2);
			szBuff[3] = strTime.GetAt(3);
			st.wYear = atoi(szBuff);

			szBuff[0] = strTime.GetAt(4);
			szBuff[1] = strTime.GetAt(5);
			szBuff[2] = 0x00;
			st.wMonth = atoi(szBuff);

			szBuff[0] = strTime.GetAt(6);
			szBuff[1] = strTime.GetAt(7);
			szBuff[2] = 0x00;
			st.wDay = atoi(szBuff);

			szBuff[0] = strTime.GetAt(8);
			szBuff[1] = strTime.GetAt(9);
			szBuff[2] = 0x00;
			st.wHour = atoi(szBuff);

			szBuff[0] = strTime.GetAt(10);
			szBuff[1] = strTime.GetAt(11);
			szBuff[2] = 0x00;
			st.wMinute = atoi(szBuff);

			szBuff[0] = strTime.GetAt(12);
			szBuff[1] = strTime.GetAt(13);
			szBuff[2] = 0x00;
			st.wSecond = atoi(szBuff);

			if(::SetLocalTime(&st))	strTiack="0";
			else					strTiack="1";

		}
		else
		{
			GetLogCtrl()->AddBuf(LOG_CIM, "S2F18 Date and Time Data - Number mistake of character(%s)", strTime);
		}


		((CMainFrame *)AfxGetMainWnd())->m_pViewCIM->ControlStateChange(E_CRST_REMOTE);
		((CMainFrame *)AfxGetMainWnd())->m_pViewCIM->EventEquipmentStatusChange();
		break;
	}
	return TRUE;
}

BOOL CAutomation::RxS2F23() // Trace Initialize Send
{
	//HOST가 설비에게 설비의 특정 SVID 값의 현황에 대해서 종류와 주기, 개수를 선택하여 보고하도록 요청한다

	//-> S2F24

// 	0: Everything correct,
// 	1: Too many SVIDs,
// 	2: No more traces allowed,
// 	3: Invalid period,
// 	4: Equipment-specified error.

	long	nItems;
	long	lCnt;
	long	lMkMsgID;
	signed char cAsciiBuf[255];
	//CWnd *pWnd;
	CString	strTOTSMP, strREPGSZ;
	CString strEqpID, strTRID, strDSPER;
	BOOL	bValid;
	CString	strAck;
	CString	strTemp1,strTemp2;
	int		nTRID;
	int		nDSPER;
	CString	str;
	int		nValidCnt;
	int		nSVIDCnt;


	g_pData->m_FDC.TraceBufClear();
	GetLogCtrl()->AddBuf(LOG_CIM,"Recv S2F23 Trace Initialize Send");

	nValidCnt = 0;
	strAck = "0";
	bValid = TRUE;
	lMkMsgID = 0;
	TRY 
	{
		switch (m_CommType)
		{
		case E_HSMS:
			{
				m_pXCom->GetListItem( m_lMsgId, &nItems );

					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//EQPID
					strEqpID.Format("%s", cAsciiBuf);

					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//TRID
					strTRID.Format("%s", cAsciiBuf);
					strTRID.Trim(' ');

					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//DSPER
					strDSPER.Format("%s", cAsciiBuf);
					strDSPER.Trim(' ');

					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//TOTSMP
					strTOTSMP.Format("%s", cAsciiBuf);
					strTOTSMP.Trim(' ');

					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//REPGSZ	/* STOP일 경우 0, SET일 경우 1 */

					strREPGSZ.Format("%s", cAsciiBuf);
					m_pXCom->GetListItem( m_lMsgId, &nItems );			//SVID COUNT	/* STOP일 경우 L 0 */

					nSVIDCnt = nItems;
					strTemp1 = g_pData->m_cEQPID;
					strTemp1.Trim(' ');
					strEqpID.Trim(' ');

					if(strcmp(strTemp1, strEqpID)!=0)
					{
						strAck = "4";	//EQPID 다를경우
						GetLogCtrl()->AddBuf(LOG_CIM, "S2F23 4: Equipment-specified error.");
					}

					else if (strREPGSZ.IsEmpty() || (strREPGSZ.Compare("0")!=0 && strREPGSZ.Compare("1")!=0))
					{
						strAck = "6";	//EQPID 다를경우
						GetLogCtrl()->AddBuf(LOG_CIM, "S2F23 6: REPGSZ error.");
					}

					else if (!ByDial(strTRID) || !ByDial(strDSPER) || !ByDial(strTOTSMP) ||(g_pData->m_FDC.GetTraceData(strTRID) < 0 && atoi(strDSPER)<1000))
					{
						strAck = "3";		//TRID, DSPER, TOTSMP 숫자가 아닐때
						GetLogCtrl()->AddBuf(LOG_CIM, "S2F23 3: Invalid period. [%d,%s][%d,%s][%d][%d,%d]",
							!ByDial(strTRID), strTRID, !ByDial(strDSPER), strDSPER, !ByDial(strTOTSMP), 
							g_pData->m_FDC.GetTraceData(strTRID) < 0, atoi(strDSPER)<1000);
					}

					else if (!CSettingParm::bFdcReportUsed)
					{
						strAck = "7";		//장비에서 Report 하지 않게 설정 되었을 경우.
						GetLogCtrl()->AddBuf(LOG_CIM, "S2F23 7: It does not report it.");
					}
					else
					{
						if (strTRID.Compare(_T("0"))==0)
						{	
							for (int i=0; i<TRID_MAX; i++)
							{
								g_pData->m_FDC.TraceSendClear(i);
							}
							GetLogCtrl()->AddBuf(LOG_CIM, "S2F23 Trace clear all.");							
						}
						else if (nSVIDCnt>0 && strREPGSZ.Compare("1")==0)
						{
							if (g_pData->m_FDC.GetTraceData(strTRID) < 0)
							{
								g_pData->m_FDC.m_tTraceBuf.m_nSVIDCnt = nSVIDCnt;
								for (int i=0; i<nSVIDCnt; i++)
								{
									m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//SVID
									strTemp1.Format("%s", cAsciiBuf);

									strcpy_s(g_pData->m_FDC.m_tTraceBuf.m_cSVID[i], strTemp1);
									strTemp1.Trim(' ');

									for (int j=0; j<g_pData->m_FDC.m_nFDC_TotalCount; j++)
									{
										strTemp2 = g_pData->m_FDC.m_cSetSVID[j];
										strTemp2.Trim(' ');
										if(strcmp(strTemp1, strTemp2)==0)
										{
											nValidCnt++;
											break;
										} 
									}
								}

								if (nValidCnt!=nSVIDCnt)
								{
									GetLogCtrl()->AddBuf(LOG_CIM, "S2F23 2: There is no SVID.");
									strAck = "2";	//TRID가 다를 경우, TRACE DATA SEND 안할때 STOP 내려 온경우.
								}
								else
								{
									strcpy_s(g_pData->m_FDC.m_tTraceBuf.m_cTRID, strTRID);
									g_pData->m_FDC.m_tTraceBuf.m_nTOTMP = atoi(strTOTSMP);
									g_pData->m_FDC.m_tTraceBuf.m_bSend = TRUE;
									g_pData->m_FDC.AddTraceData(g_pData->m_FDC.m_tTraceBuf);
									nTRID = atoi(strTRID);
									nDSPER = atoi(strDSPER);
									((CMainFrame *)AfxGetMainWnd())->m_pViewCIM->SetTimer(nTRID, nDSPER, NULL);
								}
								
							}
							else
							{
								strAck = "2";		//이미 TRACE DATA SEND 할때
								GetLogCtrl()->AddBuf(LOG_CIM, "S2F23 2: No more traces allowed");

								//동일한 TRID가 내려올경우 자동 Reset을 하고, 내려 받은 SVID로 대체 함.
								strAck = "0";
								GetLogCtrl()->AddBuf(LOG_CIM, "S2F23 Auto Reset TRID(%s)", strTRID);
								nTRID = atoi(strTRID);
								((CMainFrame *)AfxGetMainWnd())->m_pViewCIM->KillTimer(nTRID);
								g_pData->m_FDC.TraceSendClear(g_pData->m_FDC.GetTraceData(strTRID));
								GetLogCtrl()->AddBuf(LOG_CIM, "S2F23 Trace Initialize Send STOP.");

								if (g_pData->m_FDC.GetTraceData(strTRID) < 0)
								{
									g_pData->m_FDC.m_tTraceBuf.m_nSVIDCnt = nSVIDCnt;
									for (int i=0; i<nSVIDCnt; i++)
									{
										m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//SVID
										strTemp1.Format("%s", cAsciiBuf);

										strcpy_s(g_pData->m_FDC.m_tTraceBuf.m_cSVID[i], strTemp1);
										strTemp1.Trim(' ');

										for (int j=0; j<g_pData->m_FDC.m_nFDC_TotalCount; j++)
										{
											strTemp2 = g_pData->m_FDC.m_cSetSVID[j];
											strTemp2.Trim(' ');
											if(strcmp(strTemp1, strTemp2)==0)
											{
												nValidCnt++;
												break;
											} 
										}
									}

									if (nValidCnt!=nSVIDCnt)
									{
										GetLogCtrl()->AddBuf(LOG_CIM, "S2F23 2: There is no SVID.");
										strAck = "2";	//TRID가 다를 경우, TRACE DATA SEND 안할때 STOP 내려 온경우.
									}
									else
									{
										strcpy_s(g_pData->m_FDC.m_tTraceBuf.m_cTRID, strTRID);
										g_pData->m_FDC.m_tTraceBuf.m_nTOTMP = atoi(strTOTSMP);
										g_pData->m_FDC.m_tTraceBuf.m_bSend = TRUE;
										g_pData->m_FDC.AddTraceData(g_pData->m_FDC.m_tTraceBuf);
										nTRID = atoi(strTRID);
										nDSPER = atoi(strDSPER);
										((CMainFrame *)AfxGetMainWnd())->m_pViewCIM->SetTimer(nTRID, nDSPER, NULL);
									}

								}
								//동일한 TRID가 내려올경우 자동 Reset을 하고, 내려 받은 SVID로 대체 함.
							}

						}
						else	//SVID CNT = 0은 STOP
						{
							if (g_pData->m_FDC.GetTraceData(strTRID) >= 0)
							{
								nTRID = atoi(strTRID);
								((CMainFrame *)AfxGetMainWnd())->m_pViewCIM->KillTimer(nTRID);
								g_pData->m_FDC.TraceSendClear(g_pData->m_FDC.GetTraceData(strTRID));
								GetLogCtrl()->AddBuf(LOG_CIM, "S2F23 Trace Initialize Send STOP.");
							} 
							else
							{
								strAck = "5";	//TRID가 다를 경우, TRACE DATA SEND 안할때 STOP 내려 온경우.
								GetLogCtrl()->AddBuf(LOG_CIM, "S2F23 5: TRID differs");
							}
						}
					}
					
					
					//////////////////////////////////////////////////////////////////////////
					//
					//  S2F24 통신 규약 Format 
					//
					//	<A[1]		$TIAACK>			* Message Acknwldge Code
					//
					//
					//  Strm : 2, Func : 24 , lSysByte : 0
					// 
					//////////////////////////////////////////////////////////////////////////


				m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 2, 24, m_lSysByte);

					// Message 
					
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)strAck.GetString(), strAck.GetLength());
				
					//if(strcmp(g_pData->m_cCRST, "0")!=0)
					if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
				{	
					long ret = m_pXCom->Send( lMkMsgID );
					GetLogCtrl()->AddBuf(LOG_CIM, "S2F24 Trace Initialize Send, ACK=%s, ret=%d", strAck, ret );
				}

	

				break;
			}
		}

		return TRUE;
	}
	CATCH (CMemoryException, e)
	{
		return CB_ERR;	
	}
	END_CATCH

	return TRUE;
}

BOOL CAutomation::RxS2F29()
{
	// HOST가 설비 내 관리 중이 ECID List 대해 요청한다
	// >S2F30


	GetLogCtrl()->AddBuf(LOG_CIM,"Recv S2F29 Equipment Constant Name list Request");

	long	nItems;
	long	lCnt;
	signed char cAsciiBuf[255];
	CString strEqpID, strTemp1, strECID;
	BOOL	bValid = TRUE;
	int		nECIDCnt = 0;
	int		nValidCnt = 0;
	CList<CString, CString> listECID;
	CList<CString, CString> listECNAME;
	CList<CString, CString> listECVALUE;
	CString	strECName, strECValue;
	CString	cstr;
	long	lMkMsgID;
	lMkMsgID = 0;

	CMainFrame* pMainFrm = (CMainFrame*)::AfxGetMainWnd();
	//BOOL bFind = FALSE;

	TRY 
	{
		switch (m_CommType)
		{
		case E_HSMS:
			{
				m_pXCom->GetListItem( m_lMsgId, &nItems );

					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//EQPID
					strEqpID.Format("%s", cAsciiBuf);

					m_pXCom->GetListItem( m_lMsgId, &nItems );			//ECID COUNT
					nECIDCnt = nItems;

					strEqpID.Trim(' ');
					if(strcmp(g_pData->m_cEQPID, strEqpID)!=0)
					{
						bValid = FALSE;
						GetLogCtrl()->AddBuf(LOG_CIM,"S2F29 EQPID(%s) IS NOT MATCH.", strEqpID);
					}
					else
					{
						if (nECIDCnt>0)
						{
							for (int i=0; i<nECIDCnt; i++)
							{
								m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//ECID
								strECID.Format("%s", cAsciiBuf);
								for (int j=0; j<g_pData->m_ECID.m_nECID_TotalCount; j++)
								{
									if (strcmp(g_pData->m_ECID.m_cECID[j], strECID)==0)
									{
										strECID = g_pData->m_ECID.m_cECID[j];
										strECName.Format("%s", g_pData->m_ECID.m_cECNAME[j]);
										strECValue = g_pData->m_ECID.m_cECV[j];
										listECID.AddTail(strECID);
										listECNAME.AddTail(strECName);
										listECVALUE.AddTail(strECValue);
										TRACE("ECID-%s , ECNAME-%s, ECVALUE-%s\n", strECID, strECName, strECValue);
										nValidCnt++;
										//bFind = TRUE;
										break;
		
									}
								}
							}

							if (nECIDCnt!=nValidCnt)
							{
								bValid = FALSE;
								GetLogCtrl()->AddBuf(LOG_CIM,"S2F29 ECID IS NOT MATCH. HOST(%d), EQ(%d)", nECIDCnt, nValidCnt);
							}
						}
						else if(nECIDCnt==0)	//ALL
						{
							for (int j=0; j<g_pData->m_ECID.m_nECID_TotalCount; j++)
							{
								strECID = g_pData->m_ECID.m_cECID[j];
								strECName.Format("%s", g_pData->m_ECID.m_cECNAME[j]);
								strECValue = g_pData->m_ECID.m_cECV[j];
								listECID.AddTail(strECID);
								listECNAME.AddTail(strECName);
								listECVALUE.AddTail(strECValue);
								TRACE("ECID-%s , ECNAME-%s, ECVALUE-%s\n", strECID, strECName, strECValue);


							}
					
						}
						else
							bValid = FALSE;
					}



					//////////////////////////////////////////////////////////////////////////
					//
					//  S2F30 통신 규약 Format 
					//
					//	<L,2	
					//		1.A[40]			$EQPID>				* HOST가 요청한 설비의 EQPID
					//		2.<L,n								* n = HOST가 요청한 ECID 갯수
					//			1.<L,7							* ECID Set
					//				1.A[8]	$ECID>				* Equipment Constant ID
					//				2.A[40]	$ECNAME>			* Equipment Constant Name				
					//				3.A[20]	$ECDEF>				* Equipment Constant Set Value
					//				4.A[20]	$ECSLL>				* Equipment Direction Stop Low Limit
					//				5.A[20]	$ECSUL>				* Equipment Direction Stop Upper Limit
					//				6.A[20[	$ECWLL>				* Equipment Warning Low Limit
					//				7.A[20] $ECWUL>				* Equipment Warning Upper Limist
					//
					//  Strm : 2, Func : 30 , lSysByte : 0
					// 
					//////////////////////////////////////////////////////////////////////////


					if (bValid)
					{
						m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 2, 30, m_lSysByte);

							m_pXCom->SetListItem( lMkMsgID, 2);

								cstr = g_pData->m_cEQPID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());		//EQPID

								m_pXCom->SetListItem( lMkMsgID, listECID.GetCount());	//ECV Count

									POSITION pos1 = listECID.GetHeadPosition();
									POSITION pos2 = listECNAME.GetHeadPosition();
									POSITION pos3 = listECVALUE.GetHeadPosition();

									for(int i=0; i<listECID.GetCount(); i++)
									{
										m_pXCom->SetListItem( lMkMsgID, 7);

											// ECID
											cstr = listECID.GetNext(pos1);
											strECID = cstr;
											m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());
	
											// ECNAME
											cstr = listECNAME.GetNext(pos2); cstr = MakeECString(cstr, TRUE);
											strECName = cstr;
											m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());

											// ECDEF
											cstr = listECVALUE.GetNext(pos3); cstr = MakeECString(cstr, TRUE);
											strECValue = cstr;
											m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());


											cstr = "";
											m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	//ECSLL
											m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	//ECSUL
											m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	//ECWLL
											m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	//ECWUL

											//GetLogCtrl()->AddBuf(LOG_CIM,"S2F29 ECID(%s) , ECNAME(%s), ECVALUE(%s)", strECID, strECName, strECValue);
									}

					}
					else
					{
						m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 2, 30, m_lSysByte);

							m_pXCom->SetListItem( lMkMsgID, 2);

								cstr = g_pData->m_cEQPID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());		//EQPID

								m_pXCom->SetListItem( lMkMsgID, 0);	//ECV Count	

							GetLogCtrl()->AddBuf(LOG_CIM,"S2F29 ECID List 0");
					}

					//if(strcmp(g_pData->m_cCRST, "0")!=0)
					if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
					{	
						long ret = m_pXCom->Send( lMkMsgID ); // 제대로 보냈냐?
						GetLogCtrl()->AddBuf(LOG_CIM, "S2F30 Equipment Constant Name list Send, ret=%d", ret );
					}



				break;
			}
		}

		return TRUE;
	}
	CATCH (CMemoryException, e)
	{
		return CB_ERR;	
	}
	END_CATCH

	return TRUE;
}

BOOL CAutomation::RxS2F31()
{
	// HOST가 설비의 시간을 HOST의 시간과 동일하게 변경하기 위해 설비에 전송한다.

	// ->S2F32
	GetLogCtrl()->AddBuf(LOG_CIM,"Recv S2F31 Date and Time Set Request (DTS)");

	CString strTime;

	long lCnt;
	char szBuff[256];
	SYSTEMTIME st;
	CString strTiack;
	CString cstr;
	signed char cAsciiBuf[255];

	long lMkMsgID = 0;
	TRY
	{
		switch ( m_CommType )
		{
		case E_HSMS :
			m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));
			strTime.Format("%s", cAsciiBuf);

			if(strTime.GetLength()==14)
			{
				sprintf_s(szBuff, sizeof(szBuff), "%s", strTime);
				memset(&st, 0, sizeof(st));

				szBuff[4] = 0x00;

				szBuff[0] = strTime.GetAt(0);
				szBuff[1] = strTime.GetAt(1);
				szBuff[2] = strTime.GetAt(2);
				szBuff[3] = strTime.GetAt(3);
				st.wYear = atoi(szBuff);

				szBuff[0] = strTime.GetAt(4);
				szBuff[1] = strTime.GetAt(5);
				szBuff[2] = 0x00;
				st.wMonth = atoi(szBuff);

				szBuff[0] = strTime.GetAt(6);
				szBuff[1] = strTime.GetAt(7);
				szBuff[2] = 0x00;
				st.wDay = atoi(szBuff);

				szBuff[0] = strTime.GetAt(8);
				szBuff[1] = strTime.GetAt(9);
				szBuff[2] = 0x00;
				st.wHour = atoi(szBuff);

				szBuff[0] = strTime.GetAt(10);
				szBuff[1] = strTime.GetAt(11);
				szBuff[2] = 0x00;
				st.wMinute = atoi(szBuff);

				szBuff[0] = strTime.GetAt(12);
				szBuff[1] = strTime.GetAt(13);
				szBuff[2] = 0x00;
				st.wSecond = atoi(szBuff);

	 			if(::SetLocalTime(&st))	strTiack="0";
	 			else					strTiack="1";

			}
			else
			{
				strTiack = "2";
				GetLogCtrl()->AddBuf(LOG_CIM, "S2F31 Date and Set - Number mistake of character(%s)", strTime);
			}


			//////////////////////////////////////////////////////////////////////////
			//
			//  S2F32 통신 규약 Format 
			//
			//	<A[14]		$TIME>		* YYYYMMDD-HMMSS
			//
			//
			//  Strm : 2, Func : 32 , lSysByte : 0
			// 
			//////////////////////////////////////////////////////////////////////////

			m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 2, 32, m_lSysByte );

				// TIME
				cstr = strTiack;	
				m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), strlen(cstr));	// TIACK

			long ret = m_pXCom->Send( lMkMsgID );
			GetLogCtrl()->AddBuf(LOG_CIM, "S2F32 Date and Set Ack Send, TIACK=%s, ret=%d", strTiack, ret );

			break;
		}
	}
	CATCH (CMemoryException, e) 
	{
		return CB_ERR;
	} 
	END_CATCH
	return TRUE;
}

BOOL CAutomation::RxS2F41()
{
	// HOST가 Remote Command 관련된 parameter등을 이용하여 설비의 작업 수행을 요청한다 
	// Host가 Opcall 명령 내리는거
	// ->S2F42 


	//////////////////////////////////////////////////////////////////////////
	//
	//  S2F42 통신 규약 Format 
	//
	//	<L,2
	//		1.A<[2]		$RCMD>				* Remote Control Command
	//		2.A<[1]		$HCACK>				* Host Command Acknwledge
	
	//
	//  Strm : 2, Func : 42 , lSysByte : 0
	// 
	//////////////////////////////////////////////////////////////////////////
	
// 	0: OK,
// 	1: CELL is invalid,
// 	2: Command does not exist,
// 	3: Rejected, Already in Desired Condition,
// 	4: Other Errors.

	long nItems;
	long lCnt;
	CString	strRCMD;
	CString strEqpId;
	CString strModuleId;
	CString strMSG;
	CString strID;
	CString cstr;
	BOOL bCheck = FALSE;
	short RCMD;
	long nReturn;
	long lMkMsgID;
	signed char cAsciiBuf[255];
	lMkMsgID = 0;

	CString sTemp;				// 임시 사용변수


	GetLogCtrl()->AddBuf(LOG_CIM,"Recv S2F41 Host Command Send");

	TRY
	{
		m_pXCom->GetListItem( m_lMsgId, &nItems );
			m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));
			cstr.Format("%s", cAsciiBuf);
			RCMD = atoi(cstr);
		
		switch (RCMD)
		{
		//////////////////////////////////////////////////////////////////////////
		// RCMD : 1
		// Equipment Command (OPcall Send)

		case 1:		//Operator Call
			m_pXCom->GetListItem( m_lMsgId, &nItems );

				m_pXCom->GetListItem( m_lMsgId, &nItems );

					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//OPCALL

					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//EQPID

					strEqpId.Format("%s", cAsciiBuf);
					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//OPCALLCODE

					strID.Format("%s", cAsciiBuf);
					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//MESSAGE

					strMSG.Format("%s", cAsciiBuf);
			
					if (strcmp(g_pData->m_cEQPID, strEqpId)==0)
					{
						strcpy_s(g_pData->m_cOPCallID, strID);
						strcpy_s(g_pData->m_cOPCallMSG, strMSG);

						strcpy_s(g_pData->m_tOPCallMES[g_pData->m_nOPCallCount].m_cOPCallID, strID);
						strcpy_s(g_pData->m_tOPCallMES[g_pData->m_nOPCallCount].m_cOPCallMSG, strMSG);
						g_pData->m_nOPCallCount++;
						bCheck = TRUE;;
					}

				 	//S1F42로 전송					
					m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 2, 42, m_lSysByte);

						m_pXCom->SetListItem( lMkMsgID, 2);

							cstr = "1"; 
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// RCMD

							if(bCheck)
								cstr = "0";
							else
								cstr = "4";

							
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// HCACK

			GetLogCtrl()->AddBuf(LOG_CIM,"Reply S2F41 RCMD( %d ) OPCALL", RCMD);
			nReturn = m_pXCom->Send( lMkMsgID );	// 제대로 갔는가?
			GetLogCtrl()->AddBuf(LOG_CIM,"S2F42 Send Return( %d )", nReturn );
			
			if(bCheck)
				((CMainFrame *)AfxGetMainWnd())->m_pViewCIM->SetOPCall(strEqpId);	// View에 Opcall 진행
			
			break;



			//////////////////////////////////////////////////////////////////////////
			// RCMD : 2
			// Equipment Command (Interlock Send)
		
		case 2:		//Interlock
			m_pXCom->GetListItem( m_lMsgId, &nItems );

				m_pXCom->GetListItem( m_lMsgId, &nItems );

					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//INTERLOCK

					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//EQPID

					strEqpId.Format("%s", cAsciiBuf);
					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//INTERLOCKCODE

					strID.Format("%s", cAsciiBuf);

					// 2015-08-17, jhLee, 인터락 메세지의 끝에 쓰레기 문자가 붙어서 보고되는 문제가 발생, 의심 부분을 막도록 한다.
					//

					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//MESSAGE

					//ORG strMSG.Format("%s", cAsciiBuf);

					sTemp.Format("%s", cAsciiBuf);
					strMSG = GetCorrectString(sTemp, 120);				// 120 글자내에서 사용가능한 문자까지 수렴한다.

					if (strcmp(g_pData->m_cEQPID, strEqpId)==0)
					{
						strcpy_s(g_pData->m_cInterlockID, strID);
						strcpy_s(g_pData->m_cInterlockMSG, strMSG);

						// 구조체 배열에서 Interlock data가 비어있는 항목을 찾는다 - LSH171128
						int nInterlockID = 0;
						while(g_pData->m_tInterlockMES[nInterlockID].m_cInterlockState)
							nInterlockID++;

						// Interlock 데이터 추가 - LSH171128
						g_pData->m_tInterlockMES[nInterlockID].SetInterLock(strID, strMSG, RCMD);

						bCheck = TRUE;
						((CMainFrame *)AfxGetMainWnd())->m_pViewCIM->SetInterlockRequest(RCMD, strEqpId);

					}
					 				
					if(bCheck == FALSE)
					{
						//S1F42 전송
						m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 2, 42, m_lSysByte);

						m_pXCom->SetListItem( lMkMsgID, 2);

						cstr = "2"; 
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// RCMD

						if(bCheck)
							cstr = "0";
						else
							cstr = "4";

						
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// HCACK

						GetLogCtrl()->AddBuf(LOG_CIM,"Reply S2F41 RCMD( %d ) INTERLOCK", RCMD);
						//if(strcmp(g_pData->m_cCRST, "0")!=0)
						if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
						{
							nReturn = m_pXCom->Send( lMkMsgID );	// 제대로 갔는가?
							GetLogCtrl()->AddBuf(LOG_CIM,"S2F42 Send Return( %d )", nReturn );

						}
					}
			
			break;	

			//////////////////////////////////////////////////////////////////////////
			// RCMD : 3
			// Equipment job Command (Job=(PPID) Selete))

		case 3:		//PPID SELECT
			m_pXCom->GetListItem( m_lMsgId, &nItems );

				m_pXCom->GetListItem( m_lMsgId, &nItems );

					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//PPID

					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//EQPID

					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//PORTNO

					cstr.Format("%s", cAsciiBuf);
					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//MESSAGE

					cstr.Format("%s", cAsciiBuf);
 					

					//S1F42 전송
					m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 2, 42, m_lSysByte);

						m_pXCom->SetListItem( lMkMsgID, 2);

							cstr = "3"; 
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// RCMD

							cstr = "1"; 
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// HCACK


			GetLogCtrl()->AddBuf(LOG_CIM,"Reply S2F41 RCMD( %d ) PPID SELECT", RCMD);
			//if(strcmp(g_pData->m_cCRST, "0")!=0)
			if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
			{
				nReturn = m_pXCom->Send( lMkMsgID ); //제대로 갔는가?
				GetLogCtrl()->AddBuf(LOG_CIM,"S2F42 Send Return( %d )", nReturn );
			}
			
			break;	


			//////////////////////////////////////////////////////////////////////////
			// RCMD : 4,5,6,7,8
			// Equipment Process Command (Job Process

			// 미사용
			

			//////////////////////////////////////////////////////////////////////////
			// RCMD : 9
			// Equipment Job Command (Job(=PPID) Change reserve))

		case 9:	//Job(=PPID) Change
			m_pXCom->GetListItem( m_lMsgId, &nItems );

				m_pXCom->GetListItem( m_lMsgId, &nItems );

					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//ASSYCODE

					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//PPID

					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//EQPID

					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//MESSAGE

										 						
					//S2F42 전송
					m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 2, 42, m_lSysByte);

						m_pXCom->SetListItem( lMkMsgID, 2);

							cstr = "9"; 
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// RCMD

							cstr = "1"; 
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// HCACK

			GetLogCtrl()->AddBuf(LOG_CIM,"Reply S2F41 RCMD( %d ) PPID CHANGE", RCMD);
			//if(strcmp(g_pData->m_cCRST, "0")!=0)
			if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
			{
				nReturn = m_pXCom->Send( lMkMsgID );
				GetLogCtrl()->AddBuf(LOG_CIM,"S2F42 Send Return( %d )", nReturn );
				//pWnd->SetPPIDChange();
			}
			
			break;	


			//////////////////////////////////////////////////////////////////////////
			// RCMD : 10
			// Equipment Command (Function Change)

		case 10:		// Function Change
			m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//EQPID
			strEqpId.Format("%s", cAsciiBuf);

			m_pXCom->GetListItem( m_lMsgId, &nItems );				//Function Change Set

			if(nItems == 1)
			{
				m_pXCom->GetListItem( m_lMsgId, &nItems );			//Function Set

				m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//MODULEID
				strModuleId.Format("%s", cAsciiBuf);

				m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//EFID
				CString strEFID;	strEFID.Format("%s", cAsciiBuf);

				m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//EFST
				CString strEFST;	strEFST.Format("%s", cAsciiBuf);

				m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//MESSAGE
				sTemp.Format("%s", cAsciiBuf);
				strMSG = GetCorrectString(sTemp, 120);				// 120 글자내에서 사용가능한 문자까지 수렴한다.

				// 운영이 연결 안됬거나 형식이 않맞으면 NAK로 S1F42 전송
				// 운영이 연결되어 있으면 운영에 보내서 확인 받은 후에 응답
				if (strcmp(g_pData->m_cEQPID, strEqpId)==0)
				{
					if(GetSockStsDlg()->GetSocketStatus())
					{
						((CMainFrame *)AfxGetMainWnd())->m_pViewCIM->SetEquipFunctionChange(strEFID, strEFST, strMSG);	
						bCheck = TRUE;
					}
				}

				// 운영이 연결 안됬거나 형식이 않맞으면 NAK로 S1F42 전송
				// 운영이 연결되어 있으면 운영에 보내서 확인 받은 후에 응답
				if(bCheck == FALSE)
				{
					m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 2, 42, m_lSysByte);

					m_pXCom->SetListItem( lMkMsgID, 2);

					cstr.Format(_T("%d"), RCMD); 
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// RCMD

					if(bCheck)
						cstr = "0";
					else
						cstr = "4";

					
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// HCACK

					GetLogCtrl()->AddBuf(LOG_CIM,"Reply S2F41 RCMD( %d ) Function Change NACK(%s)", RCMD, cstr);
					if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
					{
						nReturn = m_pXCom->Send( lMkMsgID );	// 제대로 갔는가?
						GetLogCtrl()->AddBuf(LOG_CIM,"S2F42 Send Return( %d )", nReturn );
					}
				}
			}

			break;	


		//////////////////////////////////////////////////////////////////////////
		// RCMD : 11 Equipment Command (Transfer Stop)
		// RCMD : 12 Equipment Command (Loading Stop)
		// RCMD : 13 Equipment Command (Step Stop)
		// RCMD : 14 Equipment Command (Own Stop)
		case 11:		//Transfer Stop(배출을 안하고 설비 안에 셀이 가득찬 상태에서 멈춤)
		case 12:		//Loading Stop(로딩을 안하고 설비 안에 셀을 다 비운 상태에서 멈춤)
		case 13:		//Step Stop(현재 스텝만 완료하고 멈춤. 원래 하던 대로..)
		case 14:		//Own Stop(사용자 나름. 협의 후 결정)
			m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//EQPID
			strEqpId.Format("%s", cAsciiBuf);

			m_pXCom->GetListItem( m_lMsgId, &nItems );				//RCMD Type Set

				m_pXCom->GetListItem( m_lMsgId, &nItems );			//Interlock Set

					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//Interlock 구분자

					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//MODULEID
					strModuleId.Format("%s", cAsciiBuf);

					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//INTERLOCK ID
					strID.Format("%s", cAsciiBuf);

					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//MESSAGE
					sTemp.Format("%s", cAsciiBuf);
					strMSG = GetCorrectString(sTemp, 120);				// 120 글자내에서 사용가능한 문자까지 수렴한다.

					if ( (strcmp(g_pData->m_cEQPID, strEqpId)==0)
						&& UnitID_Validation(strModuleId)
						&& GetSockStsDlg()->GetSocketStatus() )
					{
						strcpy_s(g_pData->m_cInterlockID, strID);
						strcpy_s(g_pData->m_cInterlockMSG, strMSG);
						// 설비일 때만 일단 리스트에 집어넣자...
						if(UnitID_IsUnit(strModuleId) == FALSE)
						{
							// 구조체 배열에서 Interlock data가 비어있는 항목을 찾는다 - LSH171128
							int nInterlockID = 0;
							while(g_pData->m_tInterlockMES[nInterlockID].m_cInterlockState)
								nInterlockID++;

							// Interlock 2와 13이 같은 Step Interlock이기 때문에 처리의 호율을 위해 2로 Type을 저장한다 - LSH171128
							if(RCMD == 13)
								RCMD =2;

							// Interlock 데이터 추가 - LSH171128
							g_pData->m_tInterlockMES[nInterlockID].SetInterLock(strID, strMSG, RCMD);
						}
						else
						{
							CUnitStatus *pUnit = ((CMainFrame *)AfxGetMainWnd())->m_pViewCIM->m_UnitStatusBank.GetUnit(strModuleId);
							if(pUnit)
							{
								pUnit->AddInterlock(strID, strMSG, RCMD);
							}
						}
						bCheck = TRUE;
					}

			// 운영이 연결 안됬거나 형식이 않맞으면 NAK로 S1F42 전송
			// 운영이 연결되어 있으면 운영에 보내서 확인 받은 후에 응답
			if ( bCheck )
			{
				((CMainFrame *)AfxGetMainWnd())->m_pViewCIM->SetInterlockRequest(RCMD, UnitID_IsUnit(strModuleId) ? strModuleId:strEqpId);
			}

			if(bCheck == FALSE)
			{
				//S1F42 전송
				m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 2, 42, m_lSysByte);

				m_pXCom->SetListItem( lMkMsgID, 2);

				cstr.Format(_T("%d"), RCMD); 
				m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// RCMD

				if(bCheck)
					cstr = "0";
				else
					cstr = "4";

				
				m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// HCACK

				GetLogCtrl()->AddBuf(LOG_CIM,"Reply S2F41 RCMD( %d ) Function Change NACK(%s)", RCMD, cstr);
				if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
				{
					nReturn = m_pXCom->Send( lMkMsgID );	// 제대로 갔는가?
					GetLogCtrl()->AddBuf(LOG_CIM,"S2F42 Send Return( %d )", nReturn );

				}
			}

			break;	

			//////////////////////////////////////////////////////////////////////////
			// RCMD : 15
			// Equipment Command (Control Information)

		case 15:		// Control Information
			m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//EQPID
			strEqpId.Format("%s", cAsciiBuf);

			m_pXCom->GetListItem( m_lMsgId, &nItems );				//RCMD Attribute Set

			if(nItems == 4)
			{
				m_pXCom->GetListItem( m_lMsgId, &nItems );			//Action Attribute Set

					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//ACTIONTYPE
					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//ACTIONTYPE DATA
					CString strActionType;	strActionType.Format("%s", cAsciiBuf);

				m_pXCom->GetListItem( m_lMsgId, &nItems );			//Action Attribute Set

					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//ACTIONDETAIL
					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//ACTIONDETAIL DATA
					CString strActionDetaiil;	strActionDetaiil.Format("%s", cAsciiBuf);

				m_pXCom->GetListItem( m_lMsgId, &nItems );			//Action Attribute Set

					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//ACTION
					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//ACTION DATA
					CString strAction;	strAction.Format("%s", cAsciiBuf);

				m_pXCom->GetListItem( m_lMsgId, &nItems );			//Action Attribute Set

					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//DESCRIPTION
					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//DESCRIPTION DATA
					CString strDescription;	strDescription.Format("%s", cAsciiBuf);

				// 일단 전부 NACK
				bCheck = FALSE;

				// 운영이 연결 안됬거나 형식이 않맞으면 NAK로 S1F42 전송
				// 운영이 연결되어 있으면 운영에 보내서 확인 받은 후에 응답
				if(bCheck == FALSE)
				{
					m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 2, 42, m_lSysByte);

					m_pXCom->SetListItem( lMkMsgID, 2);

					cstr.Format(_T("%d"), RCMD); 
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// RCMD

					if(bCheck)
						cstr = "0";
					else
						cstr = "4";

					
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// HCACK

					GetLogCtrl()->AddBuf(LOG_CIM,"Reply S2F41 RCMD( %d ) Function Change NACK(%s)", RCMD, cstr);
					if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
					{
						nReturn = m_pXCom->Send( lMkMsgID );	// 제대로 갔는가?
						GetLogCtrl()->AddBuf(LOG_CIM,"S2F42 Send Return( %d )", nReturn );
					}
				}
			}
			break;	

			//////////////////////////////////////////////////////////////////////////
			// RCMD : 16
			// Equipment Command (Unit Op-call Send)

		case 16:		// Unit Op-call Send
			m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//EQPID
			strEqpId.Format("%s", cAsciiBuf);

			m_pXCom->GetListItem( m_lMsgId, &nItems );				//RCMD Type Set
			{
				m_pXCom->GetListItem( m_lMsgId, &nItems );			//Op-Call Set
				{
					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//OPCALL 구분자
					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//MODULEID
					strModuleId.Format("%s", cAsciiBuf);
					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//OPCALL ID
					strID.Format("%s", cAsciiBuf);
					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//OPCALL MSG
					sTemp.Format("%s", cAsciiBuf);
					strMSG = GetCorrectString(sTemp, 120);				// 120 글자내에서 사용가능한 문자까지 수렴한다.
				}
			}

			// UNIT일 때만 리스트에 집어넣자...
			if(UnitID_IsUnit(strModuleId))
			{
				strcpy_s(g_pData->m_cOPCallID, strID);
				strcpy_s(g_pData->m_cOPCallMSG, strMSG);

				strcpy_s(g_pData->m_tUnitOPCallMES[g_pData->m_nUnitOPCallCount].m_cOPCallID, strID);
				strcpy_s(g_pData->m_tUnitOPCallMES[g_pData->m_nUnitOPCallCount].m_cOPCallMSG, strMSG);
				g_pData->m_nUnitOPCallCount++;
				bCheck = ((CMainFrame *)AfxGetMainWnd())->m_pViewCIM->SetOPCall(strModuleId);
			}

			m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 2, 42, m_lSysByte);

			m_pXCom->SetListItem( lMkMsgID, 2);

			cstr.Format(_T("%d"), RCMD); 
			m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// RCMD

			if(bCheck)
				cstr = "0";
			else
				cstr = "4";

			
			m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// HCACK

			GetLogCtrl()->AddBuf(LOG_CIM,"Reply S2F41 RCMD( %d ) Function Change NACK(%s)", RCMD, cstr);
			if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
			{
				nReturn = m_pXCom->Send( lMkMsgID );	// 제대로 갔는가?
				GetLogCtrl()->AddBuf(LOG_CIM,"S2F42 Send Return( %d )", nReturn );
			}
			break;	


			//////////////////////////////////////////////////////////////////////////
			// RCMD : ??

		default:

			//S2F42 전송
			m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 2, 42, m_lSysByte);

				m_pXCom->SetListItem( lMkMsgID, 2);

					cstr.Format("%d", RCMD); 
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// RCMD

					cstr = "2"; 
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// HCACK

			GetLogCtrl()->AddBuf(LOG_CIM,"Reply S2F41 RCMD( %d ) Command does not exist", RCMD);
			
			//if(strcmp(g_pData->m_cCRST, "0")!=0)
			if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
			{
				nReturn = m_pXCom->Send( lMkMsgID );
				GetLogCtrl()->AddBuf(LOG_CIM,"S2F42 Send Return( %d )", nReturn );
			}
			
			break;	

		}
// 			
// 		SysFreeString(bstrBuff);	
	}
	CATCH (CMemoryException, e) 
	{
		return CB_ERR;
	} 
	END_CATCH
	return TRUE;
}

BOOL CAutomation::AckS2F42(CString RCMD, int nAck)
{
	long nReturn;
	CString cstr;

	long lMkMsgID = 0;

	m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 2, 42, m_lSysByte);

	m_pXCom->SetListItem( lMkMsgID, 2);

	
	m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)RCMD.GetString(), strlen(RCMD));	// RCMD

	// 0 : Accepted
	// 1 : Cell ID 비일치
	// 2 : RCMD가 정의된게 아님
	// 3 : 조건이 맞지 않아 수행할 수 없다
	// 4 : 설비의 특정 에러로 수행할 수 없다
	// 5 : 설비에 해당 기능이 없어 수행할 수 없다
// 	if(bAck)
// 		str = "0";
// 	else
// 		str = "3";
	cstr.Format(_T("%d"), nAck);

	
	m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// HCACK

	GetLogCtrl()->AddBuf(LOG_CIM,"Reply S2F41 RCMD( %s ) ACK(%s)", RCMD, cstr);
	if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
	{
		nReturn = m_pXCom->Send( lMkMsgID );	// 제대로 갔는가?
		GetLogCtrl()->AddBuf(LOG_CIM,"S2F42 Send Return( %d )", nReturn );
	}
		
	return TRUE;
}

BOOL CAutomation::RxS2F43()
{
	//RCMD= 21, 22, 23, 24

	long nItems;
	long lCnt;
	signed char cAsciiBuf[255];
	CString	strRCMD;
	CString strJobID;
	CString strCellID;
	CString strProductID;
	CString strStepID;
	CString strActionType;
	CString strLabelID;
	CString strInsCnt;
	CString cstr;
	BOOL bCheck = FALSE;
	//kjpark 20180123 CELL INFO, JOB PROCESS Cellid 추가
	CString strJobState;
	short RCMD;
	long nReturn;
	long lMkMsgID;

	lMkMsgID = 0;

	BOOL bJobState = FALSE;
	int nJigPos = 0;
	int nCellPos = 0;
	
	GetLogCtrl()->AddBuf(LOG_CIM,"Recv S2F43 Host Command Send for Job Process");

	TRY
	{
		m_pXCom->GetListItem( m_lMsgId, &nItems );
			m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));
			strRCMD.Format("%s", cAsciiBuf);
			RCMD = atoi(strRCMD);

		switch (RCMD)
		{
		case 21:		//Cell Job Process Start
		case 22:		//Cell Job Process Cancel
		case 23:		//Cell Job Process Pause
		case 24:		//Cell Job Process Resume
			m_pXCom->GetListItem( m_lMsgId, &nItems );	//L5
				m_pXCom->GetListItem( m_lMsgId, &nItems );	//L2
					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//JOBID (NAME)
					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//JOBID (DATA)
					strJobID.Format("%s", cAsciiBuf);
				m_pXCom->GetListItem( m_lMsgId, &nItems );	//L2
					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//CELLID
					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//CELLID
					strCellID.Format("%s", cAsciiBuf);
				m_pXCom->GetListItem( m_lMsgId, &nItems );	//L2
					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//PRODID
					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//PRODID
					strProductID.Format("%s", cAsciiBuf);
				m_pXCom->GetListItem( m_lMsgId, &nItems );	//L2
					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//STEPID
					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//STEPID
					strStepID.Format("%s", cAsciiBuf);
				m_pXCom->GetListItem( m_lMsgId, &nItems );	//L2
					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//ACTIONTYPE
					m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//ACTIONTYPE
					strActionType.Format("%s", cAsciiBuf);
								
					bCheck = g_pData->m_CellTrackIn.CheckCellID(strCellID);

			m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 2, 44, m_lSysByte);
				m_pXCom->SetListItem( lMkMsgID, 2);
				cstr = strRCMD; 
				m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// RCMD
				
				/*
				HCACK : Host Command Acknowledge Code
				* 0 = Accepted,
				* 1 = Job ID Invalid, Job ID 부정확하여 수행 할 수 없다.
				* 2 = CellD Invalid, 명령이 존재하지 않아 수행할 수 없다.
				* 3 = REJECT,ALREADYINDESIRECONDITION, 조건이 맞지 않아 수행 할 수 없다.
				* 4 = OTHERERRORS, 설비의 특정 ERROR로 수행 할 수 없다.
				* > 5 error, not Accepted
				*/
				if(bCheck)
					cstr = _T("0");
				else
					cstr = _T("2");
				
				m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// HCACK
			
			GetLogCtrl()->AddBuf(LOG_CIM,"Reply S2F43 RCMD( %d ) Cell Job Process",RCMD);
			if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
			{
				nReturn = m_pXCom->Send( lMkMsgID );		
				GetLogCtrl()->AddBuf(LOG_CIM,"S2F44 Send Return( %d )",nReturn);
			}

			if(bCheck)
			{
				if (RCMD==21)
					bJobState = TRUE;
				else if (RCMD==22)
					bJobState = FALSE;
				else
					bJobState = FALSE;
			}
			else
				bJobState = FALSE;
			
			//kjpark 20180123 CELL INFO, JOB PROCESS Cellid 추가
			strJobState.Format(_T("%d"), RCMD);

			((CMainFrame *)AfxGetMainWnd())->m_pViewCIM->SetCellJobPrecessState(bJobState, strCellID, strProductID, strStepID, strJobState);	

			break;
		default:
			m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 2, 44, m_lSysByte);
				m_pXCom->SetListItem( lMkMsgID, 3);
					cstr.Format(_T("%d"), RCMD); 
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// RCMD
					cstr = strLabelID; 
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// LabelID
					cstr = _T("5"); 
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// HCACK
				
				GetLogCtrl()->AddBuf(LOG_CIM,"Reply S2F43 RCMD( %d ) Command does not exist",RCMD);

				if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
				{
					nReturn = m_pXCom->Send( lMkMsgID );				
					GetLogCtrl()->AddBuf(LOG_CIM,"S2F44 Send Return( %d )",nReturn);
				}

			break;	
		}

			
	}
	CATCH (CMemoryException, e) 
	{
		return CB_ERR;
	} 
	END_CATCH
		return TRUE;
}

BOOL CAutomation::RxS2F103()
{
	// HOST가 설비로 Cell Loading 시점에 해당 Cell에 정보를 전송한다
	// ->S2F104

	long	nItems;
	signed char cAsciiBuf[255];
	long	lSize;
	CString	cstr, strCellID, strProductID;
	CString strCellInfoResult;
	BOOL	bCheck;
	long nReturn;
// 	int iJig, iCellPos;
	long lMkMsgID;

	lMkMsgID = 0;

	GetLogCtrl()->AddBuf(LOG_CIM,"Recv S2F103 Cell Information Download");

	bCheck = FALSE;
	TRY
	{
		switch ( m_CommType )
		{
		case E_HSMS :
			m_pXCom->GetListItem( m_lMsgId, &nItems );
				m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));	//EQPID
				m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));	//CELLID
				strCellID.Format("%s", cAsciiBuf);
				m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));	//PRODUCTID
				strProductID.Format("%s", cAsciiBuf);
				m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));	//CELLINFORESULT
				strCellInfoResult.Format("%s", cAsciiBuf);

				bCheck = g_pData->m_CellInformation.CheckCellID(strCellID);

			//////////////////////////////////////////////////////////////////////////
			//
			//  S2F4104 통신 규약 Format 
			//
			//	<A[1]		$ACK2>			* Acknowledge Cod Stream 2
			//
			//
			//
			//  Strm : 2, Func : 104 , lSysByte : 0
			// 
			//////////////////////////////////////////////////////////////////////////


			//0: Accepted,
			//1: Not Accepted.		
			m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 2, 104, m_lSysByte);

			if (bCheck)
			{
				GetLogCtrl()->AddBuf(LOG_CIM,"S2F103 Found Cell(%s), ProductId(%s), CellResult(%s)" , strCellID, strProductID, strCellInfoResult);

				// Accepted
				cstr = "0";	
				((CMainFrame *)AfxGetMainWnd())->m_pViewCIM->SetCellInformation(strCellID, strProductID, strCellInfoResult);
			}
			else
			{
				GetLogCtrl()->AddBuf(LOG_CIM,"S2F103 Can not find Cell(%s)" , strCellID);
			
				// Not Accepted
				cstr = "1"; 
			}
				// Ack
				m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// HCACK

			GetLogCtrl()->AddBuf(LOG_CIM,"Reply S2F104 Ack( %s ) Cell Information Download Reply", cstr);
			
			//if(strcmp(g_pData->m_cCRST, "0")!=0)
			if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
			{
				nReturn = m_pXCom->Send( lMkMsgID ); //제대로 갔나?
				GetLogCtrl()->AddBuf(LOG_CIM,"S2F104 Send Return( %d )", nReturn );
			}



			break;
		}
			
	}
	CATCH (CMemoryException, e) 
	{
		return CB_ERR;
	} 
	END_CATCH
	return TRUE;
}

BOOL CAutomation::RxS2F105()
{
	// HOST가 설비로 Code의 정보를 전송한다
	// ->S2F106 

	long	nItems;
	signed char cAsciiBuf[255];
	long	lSize;
	CString	cstr, strCodeType, strReasonCode, strDescription, strEngDescription, strChaDescription;
	int		nResonCodeListCnt;
	BOOL	bCheck;
	long nReturn;
	long lMkMsgID;

	lMkMsgID = 0;

	GetLogCtrl()->AddBuf(LOG_CIM,"Recv S2F105 Code Information Download");

	bCheck = FALSE;
	TRY
	{
		switch ( m_CommType )
		{
		case E_HSMS :
			m_pXCom->GetListItem( m_lMsgId, &nItems );
				m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));	//EQPID
				m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));	//CODETYPE
				strCodeType.Format("%s", cAsciiBuf);
					m_pXCom->GetListItem( m_lMsgId, &nItems );	//L[n] ResonCodeList
					nResonCodeListCnt =	nItems;
					if(strcmp(strCodeType, "M")==0)
					{	
						bCheck = TRUE;
						for (int i=0; i<nResonCodeListCnt; i++)
						{
							m_pXCom->GetListItem( m_lMsgId, &nItems );	//L[2] ResonCodeSet
								m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));	//REASONCODE
								strReasonCode.Format("%s", cAsciiBuf);
								m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));	//DESCRIPTION
								strDescription.Format("%s", cAsciiBuf);

								m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));	//ENGLISH DESCRIPTION
								strEngDescription.Format("%s", cAsciiBuf);
								m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));	//CHINESE DESCRIPTION
								strChaDescription.Format("%s", cAsciiBuf);

						}
					}
					else if(strcmp(strCodeType, "N")==0)
					{
						bCheck = TRUE;
						for (int i=0; i<nResonCodeListCnt; i++)
						{
							m_pXCom->GetListItem( m_lMsgId, &nItems );	//L[2] ResonCodeSet
							m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));	//REASONCODE
							strReasonCode.Format("%s", cAsciiBuf);
							m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));	//DESCRIPTION
							strDescription.Format("%s", cAsciiBuf);

							m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));	//ENGLISH DESCRIPTION
							strEngDescription.Format("%s", cAsciiBuf);
							m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));	//CHINESE DESCRIPTION
							strChaDescription.Format("%s", cAsciiBuf);

						}
					}
					else
					{
						bCheck = FALSE;
					}
					

					//////////////////////////////////////////////////////////////////////////
					//
					//  S2F4106 통신 규약 Format 
					//
					//	<A[1]		$ACK2>			* Acknowledge Cod Stream 2
					//
					//
					//
					//  Strm : 2, Func : 104 , lSysByte : 0
					// 
					//////////////////////////////////////////////////////////////////////////

					
			
			//0: Accepted,
			//1: Not Accepted.		
			m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 2, 106, m_lSysByte);

			if (bCheck)
			{
				// Accepted
				cstr = "0"; 
			}
			else
			{
				// Not Accepted
				cstr = "1"; 
			}	

				m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// ACK

			GetLogCtrl()->AddBuf(LOG_CIM,"Reply S2F106 RCMD( %s ) Code Information Download Reply", cstr);
			
			//if(strcmp(g_pData->m_cCRST, "0")!=0)
			if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
			{
				nReturn = m_pXCom->Send( lMkMsgID );	// 제대로 갔는가?
				GetLogCtrl()->AddBuf(LOG_CIM,"S2F106 Send Return( %d )", nReturn );
			}



			break;
		}
		

			
	}
	CATCH (CMemoryException, e) 
	{
		return CB_ERR;
	} 
	END_CATCH
	return TRUE;
}

void CAutomation::RxS3Fxx(const int F)
{
	//int nTemp;

	TRY
	{
		switch (F) {
			case 0 :
				RxSxxF0(2);
				break;
			case 109 :
				RxS3F109();					// POCH 추가
				break;
			default:
				GetLogCtrl()->AddBuf(LOG_CIM,"Recv S3F%d", F);
				//SendS9(5);
				break;
		}
	}
	CATCH_ALL (e) 
	{
		return ;
	} 
	END_CATCH_ALL

}


BOOL CAutomation::RxS3F109()
{
	long	nItems;
	signed char cAsciiBuf[255];
	long	lSize;
	CString	cstr;
	BOOL	bCntCheck=FALSE;
	BOOL	bCorrectNum=FALSE;
	BOOL	bFindCheck=TRUE;
	long nReturn;
//	int iJig = -1, iCellPos = -1;
	long lMkMsgID;

	lMkMsgID = 0;

	CString strCELLID; 
	CString strCASSETTID;
	CString strBATCHLOT;
	CString strPRODUCTID;
	CString strPRODUCT_TYPE;
	CString strPRODUCT_KIND;
	CString strPRODUCT_SPEC;
	CString strSTEPID;
	CString strPPID;
	CString strCELL_SIZE;
	CString strCELL_THINKNESS;
	CString strComment;
	CString* strName = NULL;
	CString* strValue = NULL;
	int nRepairCnt=0;
	int nRcvCnt=0;
	int nValueCnt=0;

	CString sItemName;
	CString sItemValue;
	CString sTemp;

	
	GetLogCtrl()->AddBuf(LOG_CIM,"Recv S3F109 Cell Lot Information Download");

	TRY
	{
		switch ( m_CommType )
		{
		case E_HSMS :
			m_pXCom->GetListItem( m_lMsgId, &nItems );
				m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));			//EQPID
				m_pXCom->GetListItem( m_lMsgId, &nItems );					//Cell Lot List Count
					m_pXCom->GetListItem( m_lMsgId, &nItems );				//Cell Lot Set
						m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));	//CELLID
						strCELLID.Format("%s", cAsciiBuf);
						m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));	//CASSETTID
						strCASSETTID.Format("%s", cAsciiBuf);
						m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));	//BATCHLOT
						strBATCHLOT.Format("%s", cAsciiBuf);
						m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));	//PRODUCTID
						strPRODUCTID.Format("%s", cAsciiBuf);
						m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));	//PRODUCT TYPE
						strPRODUCT_TYPE.Format("%s", cAsciiBuf);
						m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));	//PRODUCT KIND
						strPRODUCT_KIND.Format("%s", cAsciiBuf);
						m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));	//PRODUCT SPEC
						strPRODUCT_SPEC.Format("%s", cAsciiBuf);
						m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));	//STEPID
						strSTEPID.Format("%s", cAsciiBuf);
						m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));	//PPID
						strPPID.Format("%s", cAsciiBuf);
						m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));	//CELL SIZE
						strCELL_SIZE.Format("%s", cAsciiBuf);
						m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));	//CELL THINKNESS
						strCELL_THINKNESS.Format("%s", cAsciiBuf);
						m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));	//COMMENT
						strComment.Format("%s", cAsciiBuf);

						m_pXCom->GetListItem( m_lMsgId, &nItems );			// Cell 관련 정보
						nRcvCnt = nItems;

						strCELLID.Trim(' ');			// Cell ID의 공백을 없애준다.
						sItemName = "";
						sItemValue = "";

						// OLD
						//for (int i=0; i<nRcvCnt; i++)
						//{
						//	m_pXCom->GetListItem( m_lMsgId, &nItems );
						//	m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));
	 					//		sItemName.Format("%s", cAsciiBuf);

						//	m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));
						//	sItemValue.Format("%s", cAsciiBuf);

						//	// POC 값이 Value로 들어있는 경우
						//	sItemValue.Trim();
						//	if (sItemValue == _T("POC")) 	break;
						//}

						// 2017-05-02 BKH, 검사코드 추가 구현, 이게 맞는지는... ㅠ.ㅠ
						if(nRcvCnt < 1)
							bFindCheck = FALSE;

						// 상위에서 4자리 제한으로 DEFECT이 아니라 DFCT로 날려줌 [5/13/2017 OSC]
// 						if(strPRODUCT_TYPE==_T("DFCT") ) 
						if(strPRODUCT_TYPE== g_pData->m_CellLotInformation.GetOptionCode() ) 
						{
							if(nRcvCnt%7!=0)//7로 떨어지지 않으면 false 반환
							{
								bFindCheck = FALSE;
							}
							if(bFindCheck)
							{
								for (nValueCnt=0; nValueCnt<nRcvCnt/7; nValueCnt++)
								{
									m_pXCom->GetListItem( m_lMsgId, &nItems );
									m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));		// ITEM_NAME : SEQUENCE
									sItemName.Format("%s", cAsciiBuf);
									m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));		 
									sItemValue.Format("%s", cAsciiBuf);

									m_pXCom->GetListItem( m_lMsgId, &nItems );
									m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));		// ITEM_NAME : NAME
									sItemName.Format("%s", cAsciiBuf);
									m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));		 
									sItemValue.Format("%s", cAsciiBuf);
									m_sItemDefectNameValue[nValueCnt]=sItemValue;


									m_pXCom->GetListItem( m_lMsgId, &nItems );
									m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));		// ITEM_NAME : XY
									sItemName.Format("%s", cAsciiBuf);
									m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));		 
									sItemValue.Format("%s", cAsciiBuf);
									m_sItemXY_Value[nValueCnt]=sItemValue;

									m_pXCom->GetListItem( m_lMsgId, &nItems );
									m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));		// ITEM_NAME : IMG_XY
									sItemName.Format("%s", cAsciiBuf);
									m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));		 
									sItemValue.Format("%s", cAsciiBuf);

									m_pXCom->GetListItem( m_lMsgId, &nItems );
									m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));		// ITEM_NAME : CIRCLE_DRAW
									sItemName.Format("%s", cAsciiBuf);
									m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));		 
									sItemValue.Format("%s", cAsciiBuf);

									m_pXCom->GetListItem( m_lMsgId, &nItems );
									m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));		// ITEM_NAME : DEFECT_GROUP
									sItemName.Format("%s", cAsciiBuf);
									m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));		 
									sItemValue.Format("%s", cAsciiBuf);

									m_pXCom->GetListItem( m_lMsgId, &nItems );
									m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));		// ITEM_NAME : PATTERN_NAME
									sItemName.Format("%s", cAsciiBuf);
									m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));		 
									sItemValue.Format("%s", cAsciiBuf);
									m_sItemPatternNameValue[nValueCnt]=sItemValue;


								}

								// 운영 PC로 전송
								if(g_pData->m_CellLotInformation.CheckCellID(strCELLID))
								{
									((CMainFrame *)AfxGetMainWnd())->m_pViewCIM->SetCellLotDefectInformation(strCELLID, nValueCnt);	
								}

							}
						}
						else
						{
							if ( nRcvCnt >= 2 )				// 반드시 리스트 항목이 2개 이상 와야 한다.
								bFindCheck = FALSE;
							if(bFindCheck)
							{
								m_pXCom->GetListItem( m_lMsgId, &nItems );
									m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));		// ITEM_NAME : PRE_ITERATION
									sTemp.Format("%s", cAsciiBuf);
									m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));		// ITEM_VALUE : POCW 혹은 POCG
 									sItemName.Format("%s", cAsciiBuf);
							
								// 나머지 Defect code 취득
								m_pXCom->GetListItem( m_lMsgId, &nItems );
									m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));		// ITEM_NAME : RESULT_CODE
									sTemp.Format("%s", cAsciiBuf);
									m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));		// ITEM_VALUE : VT03 ~ 
									sItemValue.Format("%s", cAsciiBuf);

								// POC 대상지정 관련 정보를 항상 회신한다.
								// 운영 PC로 전송
								if(g_pData->m_CellLotInformation.CheckCellID(strCELLID))
								{
									((CMainFrame *)AfxGetMainWnd())->m_pViewCIM->SetCellLotInformation(strCELLID, sItemName, sItemValue);	
								}
							}
						}
			//0: Accepted,
			//1: Not Accepted.
			//2: messsage err.
			m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 3, 110, m_lSysByte);
			cstr = "0";	
			m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// HCACK
			GetLogCtrl()->AddBuf(LOG_CIM,"Reply S3F110 Ack( %s ) Repair Information Download Reply(POC)", cstr);

			if(strcmp(g_pData->m_cCRST, "0")!=0)
			{
				nReturn = m_pXCom->Send( lMkMsgID );
				GetLogCtrl()->AddBuf(LOG_CIM,"S3F110 Send Return( %d )", nReturn );
			}

				
			//for test
			//g_pData->m_Jig[iJig].tCellData[iCellPos].m_tRepairData.Clear();
			//Sleep(500);
			//((CMainFrame *)AfxGetMainWnd())->m_pViewCIM->GetRepairDataRead(iJig, iCellPos);

			break;
		}

	}
	CATCH (CMemoryException, e) 
	{
		return CB_ERR;
	} 
	END_CATCH
		return TRUE;
}


void CAutomation::RxS5Fxx(const int F)
{
	TRY
	{
		switch (F) {
			case 0 :
				RxSxxF0(5);
				break;

			case 2:
				RxS5F2();
				break;

			case 103:
				RxS5F103();
				break;
			default:
				//SendS9(5);
				break;
		}
	}
	CATCH_ALL (e) 
	{
		return ;
	} 
	END_CATCH_ALL

}
BOOL CAutomation::RxS5F0() 
{
	return TRUE;
}

BOOL CAutomation::RxS5F2()
{

	switch ( m_CommType )
	{
	case E_HSMS :
		break;

	}

	return TRUE;
}

BOOL CAutomation::TxS5F1(tag_Alarm Alarm) // Alarm Report Send
{

	CString cstr;
	long	nReturn;

	long lMkMsgID = 0;
	TRY
	{
		
		switch ( m_CommType )
		{
		case E_HSMS :

			m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 5, 1, 0);
				m_pXCom->SetListItem( lMkMsgID, 5);
					cstr = g_pData->m_cEQPID;
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	//EQPID
					cstr = Alarm.m_cALST;
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	//ALST
					cstr = Alarm.m_cALCD;;
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	//ALCD
					cstr = Alarm.m_cALID;
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	//ALID
					cstr = Alarm.m_cALTX;
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	//ALTX
			
					GetLogCtrl()->AddBuf(LOG_CIM,"S5F1 Alarm %s Report Send ALID( %s)", (strcmp(Alarm.m_cALST, "1")==0 ? "Set":"Clear"),  Alarm.m_cALID);
			                                                     
			//if(strcmp(g_pData->m_cCRST, "0")!=0)
			if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
			{
				nReturn = m_pXCom->Send( lMkMsgID );
				GetLogCtrl()->AddBuf(LOG_CIM,"S5F1 Alarm Report Send ALID( %s ) Send Result( %d )", Alarm.m_cALID, nReturn );	
			}

			break;
		}
	}
	CATCH_ALL (e) 
	{
		return CB_ERR;
	} 
	END_CATCH_ALL

	return TRUE;
}

BOOL CAutomation::RxS5F103() // Current Alarm List Request
{
	// HOST가 특정 시점에 설비의 현재 발생한 Alarm의 List를 요청하기 위해 설비로 요청한다
	// ->S5F104

	GetLogCtrl()->AddBuf(LOG_CIM,"Recv S5F103 Current Alarm List Request");

	long nItems;
	long lCnt;
	CString cstr, strEqpID;
	signed char cAsciiBuf[255];
	long nReturn;
	BOOL bValid;
	long lMkMsgID;

	lMkMsgID = 0;

	TRY
	{	
		bValid = FALSE;
		m_pXCom->GetListItem( m_lMsgId, &nItems );
		if (nItems == 0)
		{
			bValid = TRUE;
		} 
		else
		{
			for(int i=0; i<nItems; i++)
			{
				m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//EQPID
				cstr.Format("%s", cAsciiBuf);
				cstr.Trim(' ');
				strEqpID = g_pData->m_cEQPID;
				strEqpID.Trim(' ');
				if (strcmp(cstr, strEqpID)==0)
				{
					bValid = TRUE;
				}
			}
		}


		//////////////////////////////////////////////////////////////////////////
		//
		//  S5F104 통신 규약 Format 
		//
		//	<L,n										* EQP Alarm List
		//		1.<L,2									* EQP Alarm Set
		//			1.<A[40]		$EQPID>				* HOST가 요청한 EQPID
		//			2.<L,m								* 현재 발생 된 Alarm 갯수
		//				1.<L,3							* Alarm Set
		//					1.<A[1]		$ALCD>			* Alarm의 정도에 대한 구분 코드
		//					2.<A[10]	$ALID>			* Alarm 별로 정의 된 ID
		//					3.<A[120]	$ALTX>			* Alarm의 대한 내용
		//
		//
		//  Strm : 5, Func : 104 , lSysByte : 0
		// 
		//////////////////////////////////////////////////////////////////////////

		if (bValid)
		{
			m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 5, 104, m_lSysByte);

				m_pXCom->SetListItem( lMkMsgID, 1);

					m_pXCom->SetListItem( lMkMsgID, 2);

						// EQPID
						cstr = g_pData->m_cEQPID; 
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	
				
						// Alarm Cnt
						g_pData->m_nCurAlarmListCnt;
						m_pXCom->SetListItem( lMkMsgID, g_pData->m_nCurAlarmListCnt);

							for (int i=0; i<g_pData->m_nCurAlarmListCnt; i++)
							{
								m_pXCom->SetListItem( lMkMsgID, 3);

									// Alarm Code
									cstr = g_pData->m_tCurAlarmList[i].m_cALCD; 
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());

									// Alarm ID
									cstr = g_pData->m_tCurAlarmList[i].m_cALID; 
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());

									// Alarm 내용
									cstr = g_pData->m_tCurAlarmList[i].m_cALTX; 
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());

							}
							
			GetLogCtrl()->AddBuf(LOG_CIM,"Reply S5F103 ALARM COUNT( %d )", g_pData->m_nCurAlarmListCnt);
			
			//if(strcmp(g_pData->m_cCRST, "0")!=0)
			if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
			{
				nReturn = m_pXCom->Send( lMkMsgID );	// 제대로 갔냐?		
				GetLogCtrl()->AddBuf(LOG_CIM,"S5F104 Send Return( %d )", nReturn );								
			}



		}
		else
		{
			m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 5, 104, m_lSysByte);

			m_pXCom->SetListItem( lMkMsgID, 0);		

			if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
			{
				nReturn = m_pXCom->Send( lMkMsgID );	// 제대로 갔냐?		
				GetLogCtrl()->AddBuf(LOG_CIM,"S5F104 Send Return( %d )", nReturn );								
			}		
		}
	}
	CATCH_ALL (e) 
	{
		return CB_ERR;
	} 
	END_CATCH_ALL

		return TRUE;
}

void CAutomation::RxS6Fxx(const int F)
{
	TRY
	{
		switch (F) {
			case 0 :
				RxSxxF0(6);
				break;
			case 12 :
				RxS6F12();
				break;
			case 206 :
				RxS6F206();
				break;
			case 208 :
				RxS6F208();
				break;
			case 210 :
				RxS6F210();
				break;		
			default:
				//SendS9(5);
				break;
		}
	}
	CATCH_ALL (e) 
	{
		return;
	} 
	END_CATCH_ALL

}

BOOL CAutomation::RxS6F0() 
{
	return TRUE;
}

BOOL CAutomation::RxS6F12()
{	
	// HOST는 설비에서 보고 된 Report Data 수신함을 응답

	CString	cstr;
	signed char cAsciiBuf[255];
	long lSize;
	short nAck;

	switch ( m_CommType )
	{
	case E_HSMS :
		m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lSize, sizeof(cAsciiBuf));
		//cstr.Format("%s", cAsciiBuf);
		cstr.Format("%s", cAsciiBuf);
		nAck = atoi(cstr.Trim());

		switch ( nAck )
		{
		case 0 :
			GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 Event reply ACKC6( %d ) : Accepted", nAck);
			break;
		case 1 :
			GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 Event reply ACKC6( %d ) : Error. Not Accepted", nAck);
			break;
		case 2 :
			GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 Event reply ACKC6( %d ) : Error. Not Exist CEID", nAck);
			break;
		default :
			GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 Event reply ACKC6( %d ) : Error. Not ACKC6", nAck);
			break;
		};

		break;

	}

	return TRUE;
}

BOOL CAutomation::RxS6F206()
{	
	// 설비로부터 Cell Lot의 정보 요청을 HOST 수신함을 응답한다
	GetLogCtrl()->AddBuf(LOG_CIM,"Recv S6F206 Repair Information Reply");
	return TRUE;
}

BOOL CAutomation::RxS6F208()
{
	// 설비로부터 Cell의 정보 요청을 HOST 수신함을 응답한다
	GetLogCtrl()->AddBuf(LOG_CIM,"Recv S6F208 Cell Information Reply");
	return TRUE;
}
BOOL CAutomation::RxS6F210()
{
	// 설비로부터 Code 정보 요청을 HOST가 수신함을 알려준다
	GetLogCtrl()->AddBuf(LOG_CIM,"Recv S6F210 Code Information Reply");
	return TRUE;
}

BOOL CAutomation::TxS6F1(int nIndex) // Trace Data Send
{
	long	lMkMsgID = 0;
	CString cstr;
	CString strTemp1, strTemp2;
	long	nReturn;
	CTime	t;
	CWnd *pWnd = AfxGetApp()->GetMainWnd();

	TRY
	{
		//if ( m_pXCom->GetParam("HSMS.SELECTED") != "1" )
		if ( m_pXCom->GetParam("HSMS.SELECTED") != CRST_REMOTE )
			return FALSE;

		if(g_pData->m_FDC.m_tTraceData[nIndex].m_nTOTMP != 0)
		{
			if (g_pData->m_FDC.m_tTraceData[nIndex].m_nTOTMP < g_pData->m_FDC.m_tTraceData[nIndex].m_nSMPLN || g_pData->m_FDC.m_tTraceData[nIndex].m_nSMPLN >= 10000)
			{
				GetLogCtrl()->AddBuf(LOG_CIM,"S6F1 Trace Data Send - Stop, TOTSMP(%d), SMPLN(%d)", g_pData->m_FDC.m_tTraceData[nIndex].m_nTOTMP, g_pData->m_FDC.m_tTraceData[nIndex].m_nSMPLN);
				cstr = g_pData->m_FDC.m_tTraceData[nIndex].m_cTRID;
				pWnd->KillTimer(atoi(cstr));
				g_pData->m_FDC.TraceSendClear(nIndex);
				return FALSE;
			}
		}
		else if (!CSettingParm::bFdcReportUsed)
		{
			GetLogCtrl()->AddBuf(LOG_CIM,"S6F1 Trace Data Send - Stop, It does not report it! TRID(%d)", nIndex);
			cstr = g_pData->m_FDC.m_tTraceData[nIndex].m_cTRID;
			pWnd->KillTimer(atoi(cstr));
			g_pData->m_FDC.TraceSendClear(nIndex);
			return FALSE;
		}

		m_lSysByte += 1;

		switch ( m_CommType )
		{

		case E_HSMS:
			m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 6, 1, 0);
				m_pXCom->SetListItem( lMkMsgID, 5);
				cstr = g_pData->m_cEQPID;
				m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	//EQPID
				cstr = g_pData->m_FDC.m_tTraceData[nIndex].m_cTRID;
				m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	//TRID

				if (g_pData->m_FDC.m_tTraceData[nIndex].m_nSMPLN > 99999)
				{
					g_pData->m_FDC.m_tTraceData[nIndex].m_nSMPLN = 1;
				}

				cstr.Format("%d", g_pData->m_FDC.m_tTraceData[nIndex].m_nSMPLN);
				//g_pData->m_FDC.m_nSMPLN++;
				m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	//SMPLN
				t = CTime::GetCurrentTime();
				cstr.Format("%02d%02d%02d%02d%02d%02d", t.GetYear(),t.GetMonth(),t.GetDay(), t.GetHour(),t.GetMinute(),t.GetSecond());
				m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	//STIME
				m_pXCom->SetListItem( lMkMsgID, g_pData->m_FDC.m_tTraceData[nIndex].m_nSVIDCnt);
					for (short i=0; i<g_pData->m_FDC.m_tTraceData[nIndex].m_nSVIDCnt; i++)
					{
						for (short j=0; j<g_pData->m_FDC.m_nFDC_TotalCount; j++)
						{
							strTemp1 = g_pData->m_FDC.m_tTraceData[nIndex].m_cSVID[i];
							strTemp1.Trim(' ');
							strTemp2 = g_pData->m_FDC.m_cSetSVID[j];
							strTemp2.Trim(' ');
							if (strcmp(strTemp1, strTemp2)==0)
							{
								m_pXCom->SetListItem( lMkMsgID, 2);
									cstr = g_pData->m_FDC.m_cSetSVID[j];
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	//SVID
									cstr = g_pData->m_FDC.m_cSetSV[j];
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	//SV
									break;
							}

						}
						

					}

				if (g_pData->m_FDC.m_tTraceData[nIndex].m_nSMPLN%100 == 0 || g_pData->m_FDC.m_tTraceData[nIndex].m_nSMPLN==1)
					GetLogCtrl()->AddBuf(LOG_CIM,"S6F1 Trace Data Send");
				
				//if(strcmp(g_pData->m_cCRST, "0")!=0)
				if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
				{
					nReturn = m_pXCom->Send( lMkMsgID );
					if (g_pData->m_FDC.m_tTraceData[nIndex].m_nSMPLN%100 == 0 || g_pData->m_FDC.m_tTraceData[nIndex].m_nSMPLN==1)
						GetLogCtrl()->AddBuf(LOG_CIM,"S6F1 send result( %d )",nReturn );				
				}
		
				g_pData->m_FDC.m_tTraceData[nIndex].m_nSMPLN++;

				break;
		}
	}
	CATCH_ALL (e) 
	{
		return CB_ERR;
	} 
	END_CATCH_ALL

	return TRUE;
}


BOOL CAutomation::TxS6F11(CEID ceid, UNIT unitid, CELLPOSID cellposid) // Event Report Send
{
	long lMkMsgID = 0;
	CString strCEID, strRPTID;
	CString	strDATAID = "0";
	CString cstr;
	long nReturn;
	CString strUnitID;
	CString strCellID;
	CString	strUnitST;
	CString	strPJID;
	CString	strPlanQty;
	CString strProcessedQty;
	CString strReaderID;
	CString strReaderResultCode;
	CString	strMaterialId;
	CString strMLN;
	CString	strDVName;
	CString strDV;
	CString	strOperatorID;	// jhkim1 기존것 500 -> 501 변경되며 strOperatorID1~3까지 추가되어 추가 시작!!!
//	jhkim1 기존것 500 -> 501 변경되며 strOperatorID1~3까지 추가되어 추가 시작!!!
	CString	strOperatorID1;
	CString	strOperatorID2;
	CString	strOperatorID3;
//	jhkim1 기존것 500 -> 501 변경되며 strOperatorID1~3까지 추가되어 추가 끝!!!
	CString strJuge;
	CString	strReasonCode;
	CString	strDescription;
	CString	strStepID;
	CString	strProductID;
	CString strProcessFlag;

	UINT nInterlockType;

	int iStartCellCnt=10;
	CMainFrame* pMainFrm = (CMainFrame*)::AfxGetMainWnd();
	tag_CellData* pCellData;
	BOOL bCellCnt=FALSE;

	TRY
	{
		switch ( ceid )
		{
			case 401 :	//Cell Process Start
// 				pCellData = &g_pData->m_Jig[unitid].tCellData[cellposid];
				pCellData	= g_pData->m_CellTrackIn.GetCellDataPtr();
				strCellID = pCellData->m_cCellID;
				strPJID = pCellData->m_cPJID;
				strPlanQty = pCellData->m_cPlanQty;
				strProcessedQty = pCellData->m_cProcessedQty; 
				strReaderID = pCellData->m_cReaderID;
				strReaderResultCode = pCellData->m_cReaderResultCode;	
				strStepID = pCellData->m_cStepID;
				strProductID = pCellData->m_cProductID;	

				m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 6, 11, 0);
				m_pXCom->SetListItem( lMkMsgID, 3);
					cstr = "0";
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	//DATAID
					cstr = "401";
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CEID
					m_pXCom->SetListItem( lMkMsgID, 5);
						m_pXCom->SetListItem( lMkMsgID, 2);
							cstr = "100";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
							m_pXCom->SetListItem( lMkMsgID, 2);
								cstr = g_pData->m_cEQPID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// EQPID
								cstr = g_pData->m_cCRST;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CRST
						m_pXCom->SetListItem( lMkMsgID, 2);
							cstr = "101";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID

							m_pXCom->SetListItem( lMkMsgID, 9);
								cstr = g_pData->m_cAvilability;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// AVAILABILITY
								cstr = g_pData->m_cInterlock;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// INTERLOCK
								cstr = g_pData->m_cMove;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// MOVE STATUS
								cstr = g_pData->m_cRun;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RUN STATUS
								cstr = g_pData->m_cFront;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// FRONT STATUS		
								cstr = g_pData->m_cRear;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// REAR STATUS
								cstr = g_pData->m_cSampleLot;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// SAMPLE LOT STATUS
								cstr = g_pData->m_cReasonCode;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RESONCODE
								cstr = g_pData->m_cDescription;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// DESCRIPTION

						m_pXCom->SetListItem( lMkMsgID, 2);
							cstr = "300";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
							m_pXCom->SetListItem( lMkMsgID, 4);
								cstr = strCellID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CELLID
								cstr = g_pData->m_cPPID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// PPID
								cstr = strProductID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// PRODUCTID
								cstr = strStepID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// STEPID
						m_pXCom->SetListItem( lMkMsgID, 2);	
							cstr = "301";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
							m_pXCom->SetListItem( lMkMsgID, 3);
								cstr = strPJID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// PJID
								cstr = strPlanQty;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// PLANQTY
								cstr = strProcessedQty;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// PROCESSQTY
						m_pXCom->SetListItem( lMkMsgID, 2);	
							cstr = "400";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
							m_pXCom->SetListItem( lMkMsgID, 2);
								cstr = strReaderID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// READERID
								cstr = strReaderResultCode;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// READERRESULTCODE

				GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 Cell Process Start CEID( %d ), CELL POS( %d ), CELL ID( %s )", ceid, cellposid+1, strCellID);	//2012.02.04. Modifyed
				//if(strcmp(g_pData->m_cCRST, "0")!=0)
				if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
				{
					nReturn = m_pXCom->Send( lMkMsgID );
					GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 Cell Process Start CEID(%d) Send Result( %d )", ceid, nReturn );				
				}

				// 2016-09-09, jhLee, Tracking 정보만 별도로 Log 기록한다.
				GetLogCtrl()->AddBuf(LOG_TRACKING,"Track-In,%s", strCellID);	

				break;


			case 406 :	//Cell Process Complete	// jhkim1 기존것 402-> 406 변경되며 추가
				pCellData = &g_pData->m_CellDataTrackOut;
				strCellID = pCellData->m_cCellID;
				strPJID = pCellData->m_cPJID;
				strPlanQty = pCellData->m_cPlanQty;
				strProcessedQty = pCellData->m_cProcessedQty; 
// 				strReaderID = pCellData->m_cReaderID;
// 				strReaderResultCode = pCellData->m_cReaderResultCode;	
				strMaterialId = pCellData->m_cMaterialID;
				strMLN = pCellData->m_cMLN;
				strDVName = pCellData->m_cDVName;
				strDV = pCellData->m_cDV;

				strOperatorID1 = pCellData->m_cOperatorID1;	// jhkim1 OperatorID가 1~2밖에 존재 하지 않고 운영 프로그램이 OperatorID가 하나만 사용하여 1로 통일 추후 수정 필요
				strOperatorID2 = pCellData->m_cOperatorID2;
				strOperatorID3 = pCellData->m_cOperatorID1;

				strJuge = pCellData->m_cJudge;
				
				strReasonCode = pCellData->m_cReasonCode;
				strDescription = pCellData->m_cDescription;
				strStepID = pCellData->m_cStepID;
				strProductID = pCellData->m_cProductID;

				m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 6, 11, 0);
				m_pXCom->SetListItem( lMkMsgID, 3);
					cstr = "0";
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	//DATAID
					cstr = "406";
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CEID
					m_pXCom->SetListItem( lMkMsgID, 8);
						m_pXCom->SetListItem( lMkMsgID, 2);
							cstr = "100";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
							m_pXCom->SetListItem( lMkMsgID, 2);
								cstr = g_pData->m_cEQPID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// EQPID
								cstr = g_pData->m_cCRST;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CRST
						m_pXCom->SetListItem( lMkMsgID, 2);
							cstr = "101";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID

							m_pXCom->SetListItem( lMkMsgID, 9);
								cstr = g_pData->m_cAvilability;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// AVAILABILITY
								cstr = g_pData->m_cInterlock;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// INTERLOCK
								cstr = g_pData->m_cMove;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// MOVE STATUS
								cstr = g_pData->m_cRun;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RUN STATUS
								cstr = g_pData->m_cFront;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// FRONT STATUS		
								cstr = g_pData->m_cRear;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// REAR STATUS	
								cstr = g_pData->m_cSampleLot;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// SAMPLE LOT STATUS
								cstr = g_pData->m_cReasonCode;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RESONCODE
								cstr = g_pData->m_cDescription;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// DESCRIPTION

						m_pXCom->SetListItem( lMkMsgID, 2);
							cstr = "300";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
							m_pXCom->SetListItem( lMkMsgID, 4);
								cstr = strCellID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CELLID
								cstr = g_pData->m_cPPID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// PPID
								cstr = strProductID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// PRODUCTID
								cstr = strStepID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// STEPID
						m_pXCom->SetListItem( lMkMsgID, 2);	
							cstr = "301";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
							m_pXCom->SetListItem( lMkMsgID, 3);
								cstr = strPJID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// PJID
								cstr = strPlanQty;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// PLANQTY
								cstr = strProcessedQty;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// PROCESSQTY
						m_pXCom->SetListItem( lMkMsgID, 2);	
							cstr = "400";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
							m_pXCom->SetListItem( lMkMsgID, 2);
								cstr = "";	//str = strReaderID;			//2013.11.07. Reader 가독율 이중 취합으로 공백으로 보고. //SDC 송지훈 요청
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// READERID
								cstr = "";	//str = strReaderResultCode;	//2013.11.07. Reader 가독율 이중 취합으로 공백으로 보고. //SDC 송지훈 요청
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// READERRESULTCODE
						m_pXCom->SetListItem( lMkMsgID, 2);	
							cstr = "201";	// jhkim1 기존 것 200 -> 201 변경되며 추가
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
							m_pXCom->SetListItem( lMkMsgID, 0);
					
						m_pXCom->SetListItem( lMkMsgID, 2);	
						{
							cstr = "600";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
							//m_pXCom->SetListItem( lMkMsgID, 1);
							//m_pXCom->SetListItem( lMkMsgID, 2);
							//str = strDVName;
							//m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// DVNAME
							//str = strDV;
							//m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// DV

							m_pXCom->SetListItem( lMkMsgID, 3);	
							{
								//// 포스터치에서는 필요없음 [12/19/2016 OSC]
								//// 기존 DV항목
								//// BIN
								//m_pXCom->SetListItem( lMkMsgID, 2);
								//str = strDVName;
								//m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// DVNAME
								//str = strDV;
								//m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// DV
								//
								// 아래건 하든 안하든 무조건 올려야 함 [12/19/2016 OSC]
								// MTP_WRITE
								m_pXCom->SetListItem( lMkMsgID, 2);
								{
									cstr = "MTP_WRITE";
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// DVNAME
									cstr = pCellData->m_cMTPWrite;
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// DV
								}

								//REASONCOORDINATE, Defect 발생위치 정보 추가
								m_pXCom->SetListItem( lMkMsgID, 2);
								{
									cstr = "REASONCOORDINATE";
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// DVNAME
									cstr = pCellData->m_cDefectPos;
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// DV
								}

								// 채널 정보 추가 (SDC 백윤기 요청) [5/29/2018 OSC]
								m_pXCom->SetListItem( lMkMsgID, 2);
								{
									cstr = "FINAL_TESTER";
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// DVNAME
									cstr.Format("CN%02d", (unitid*JIG_CH_MAX) + cellposid+1);
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// DVNAME
								}
							}
						}

						m_pXCom->SetListItem( lMkMsgID, 2);	

							cstr = "501";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID						
							m_pXCom->SetListItem( lMkMsgID, 6);
							cstr = strOperatorID1;
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// OPERATORID1
							cstr = "";
// 							str = strOperatorID2;	// 임시로 주석
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// OPERATORID2	// jhkim1 OperatorID가 1~2밖에 존재 하지 않고 운영 프로그램이 OperatorID가 하나만 사용하여 공백으로 보냄 추후 수정 필요
							cstr = "";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// OPERATORID3	// jhkim1 OperatorID가 1~2밖에 존재 하지 않고 운영 프로그램이 OperatorID가 하나만 사용하여 공백으로 보냄 추후 수정 필요
							cstr = strJuge;
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// JUDGE
							cstr = strReasonCode;
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// REASONCODE
							cstr = strDescription;
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// DESCRIPTION
						// jhkim1 기존 것 500 -> 501 변경되며 추가 끝!!!

				GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 Cell Process Complete CEID(%d), CELL POS(%d), CELL ID( %s )", ceid, cellposid+1, strCellID);	//2012.02.04. Modifyed
				//if(strcmp(g_pData->m_cCRST, "0")!=0)
				if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
				{
					nReturn = m_pXCom->Send( lMkMsgID );
					GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 Cell Process Complete CEID(%d) Send Result ( %d )", ceid, nReturn );				
				}

				// 2016-09-09, jhLee, Tracking 정보만 별도로 Log 기록한다.
				GetLogCtrl()->AddBuf(LOG_TRACKING,"Track-Out,%s,%s,%s", strCellID, strJuge, strReasonCode);	

				// SDC 이정현 요청으로 Track Out Log 기록 [7/5/2017 OSC]
				WriteTrackOutLog(strCellID, pCellData->m_cInnerID, pCellData->m_cDescription);

				pCellData->Clear();
				
				break;

			case 101 :	//Equipment Status Change
				m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 6, 11, 0);
				m_pXCom->SetListItem( lMkMsgID, 3);
					cstr = "0";
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	//DATAID
					cstr = "101";
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CEID
					m_pXCom->SetListItem( lMkMsgID, 4);
						m_pXCom->SetListItem( lMkMsgID, 2);
							cstr = "100";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
							m_pXCom->SetListItem( lMkMsgID, 2);
								cstr = g_pData->m_cEQPID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// EQPID
								cstr = g_pData->m_cCRST;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CRST
						m_pXCom->SetListItem( lMkMsgID, 2);
							cstr = "101";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID

							m_pXCom->SetListItem( lMkMsgID, 9);
								cstr = g_pData->m_cAvilability;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// AVAILABILITY
								cstr = g_pData->m_cInterlock;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// INTERLOCK
								cstr = g_pData->m_cMove;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// MOVE STATUS
								
// 								if (ExistAllCellChk())
// 								{									
// 									strcpy_s( g_pData->m_cOldRun,g_pData->m_cRun);
// 									strcpy_s( g_pData->m_cRun,"2");
// 								}	
// 								else
// 								{
// 									strcpy_s( g_pData->m_cOldRun,g_pData->m_cRun);
// 									strcpy_s( g_pData->m_cRun,"1");
// 								}
								cstr = g_pData->m_cRun;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RUN STATUS
								cstr = g_pData->m_cFront;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// FRONT STATUS		
								cstr = g_pData->m_cRear;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// REAR STATUS	
								cstr = g_pData->m_cSampleLot;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// SAMPLE LOT STATUS
								cstr = g_pData->m_cReasonCode;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RESONCODE
								cstr = g_pData->m_cDescription;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// DESCRIPTION

						m_pXCom->SetListItem( lMkMsgID, 2);
							cstr = "103";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), 3);	// RPTID

							m_pXCom->SetListItem( lMkMsgID, 9);
								cstr = g_pData->m_cOldAvilability;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// AVAILABILITY
								cstr = g_pData->m_cOldInterlock;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// INTERLOCK
								cstr = g_pData->m_cOldMove;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// MOVE STATUS
								cstr = g_pData->m_cOldRun;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RUN STATUS
								cstr = g_pData->m_cOldFront;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// FRONT STATUS		
								cstr = g_pData->m_cOldRear;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// REAR STATUS	
								cstr = g_pData->m_cOldSampleLot;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// SAMPLE LOT STATUS
								cstr = g_pData->m_cReasonCode;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RESONCODE
								cstr = g_pData->m_cDescription;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// DESCRIPTION

						m_pXCom->SetListItem( lMkMsgID, 2);
							cstr = "104";
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
								
							if(g_pData->m_nCurAlarmListCnt > 0)
							{
								m_pXCom->SetListItem( lMkMsgID, g_pData->m_nCurAlarmListCnt);
								for (int i=0; i<g_pData->m_nCurAlarmListCnt; i++)
								{
									m_pXCom->SetListItem( lMkMsgID, 4);
									cstr = "1";
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());		//ALST
								cstr = g_pData->m_tCurAlarmList[i].m_cALCD;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());
									cstr = g_pData->m_tCurAlarmList[i].m_cALID;
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());
									cstr = g_pData->m_tCurAlarmList[i].m_cALTX;
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());

								}
							}
							else if(g_pData->GetInterlockCount() > 0)
							{
								// SDC 이현식PRO와 협의됨. 상태보고시는 인터락은 1개만 보고 [12/7/2017 OSC]
								m_pXCom->SetListItem( lMkMsgID, 1);
								for (int i=0; i<MAX_INTERLOCK_MES; i++)
								{
									if(g_pData->m_tInterlockMES[i].m_cInterlockState == INTERLOCK_HAPPEN)
									{
										m_pXCom->SetListItem( lMkMsgID, 4);
										cstr = "1";
										m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());		//ALST
										cstr = _T("2");	// 2 SERIUS, 1 Light
										m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());
										cstr = g_pData->m_tInterlockMES[i].m_cInterlockID;
										m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());
										cstr = g_pData->m_tInterlockMES[i].m_cInterlockMSG;
										m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());
										break;
									}
								}

								// Interlock 발생 후 RUN이 되면 인터락 리스트 클리어 [12/2/2017 OSC]
								if(strcmp(g_pData->m_cMove, EQST_MOVE_RUNNING) == 0)
								{
									for (int j=0; j<MAX_INTERLOCK_MES; j++)
									{
										if(g_pData->m_tInterlockMES[j].m_cInterlockState == INTERLOCK_HAPPEN)
											g_pData->m_tInterlockMES[j].Clear();
									}
								}
							}
							else
							{
								m_pXCom->SetListItem( lMkMsgID, g_pData->m_nCurAlarmListCnt);
							}
							
							//g_pData->m_nCurAlarmListCnt = 0;
				GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 Equipment Status Change CEID( %d )", ceid);
				//if(strcmp(g_pData->m_cCRST, "0")!=0)
				if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
				{
					nReturn = m_pXCom->Send( lMkMsgID );
					GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 Equipment Status Change(%d) Send Result( %d )", ceid, nReturn );
				}
				
// #define EQST_AVAILABILITY_DOWN			"1"
// #define EQST_AVAILABILITY_UP				"2"
// 
// #define EQST_INTERLOCK_ON				"1"
// #define EQST_INTERLOCK_OFF				"2"
// 
// #define EQST_MOVE_PAUSE					"1"
// #define EQST_MOVE_RUNNING				"2"
// 
// #define EQST_RUN_IS_CELL					"2"
// #define EQST_RUN_NOT_CELL				"1"
// 
// #define EQST_FRONT_UP					"1"
// #define EQST_FRONT_DOWN					"2"
// 
// #define EQST_REAR_UP						"1"
// #define EQST_REAR_DOWN					"2"
// 
// #define EQST_SAMPLE_ON					"1"
// #define EQST_SAMPLE_OFF					"2"

	

				GetLogCtrl()->AddBuf(LOG_CIM,"[EQST CHECK] MAINT - %s => %s", (strcmp(g_pData->m_cOldMaint, "1")==0 ? "ON":"OFF"),  (strcmp(g_pData->m_cMaint, "1")==0 ? "ON":"OFF"));
				GetLogCtrl()->AddBuf(LOG_CIM,"[EQST CHECK] AVAILABILITY - %s => %s", (strcmp(g_pData->m_cOldAvilability, "1")==0 ? "DOWN":"UP"),  (strcmp(g_pData->m_cAvilability, "1")==0 ? "DOWN":"UP"));
				GetLogCtrl()->AddBuf(LOG_CIM,"[EQST CHECK] INTERLOCK	  - %s => %s", (strcmp(g_pData->m_cOldInterlock, "1")==0 ? "ON":"OFF"),  (strcmp(g_pData->m_cInterlock, "1")==0 ? "ON":"OFF"));
				GetLogCtrl()->AddBuf(LOG_CIM,"[EQST CHECK] MOVE		  - %s => %s", (strcmp(g_pData->m_cOldMove, "1")==0 ? "PAUSE":"RUNNING"),  (strcmp(g_pData->m_cMove, "1")==0 ? "PAUSE":"RUNNING"));
				GetLogCtrl()->AddBuf(LOG_CIM,"[EQST CHECK] RUN		  - %s => %s", (strcmp(g_pData->m_cOldRun, "1")==0 ? "NOT CELL":"IS CELL"),  (strcmp(g_pData->m_cRun, "1")==0 ? "NOT CELL":"IS CELL"));
				GetLogCtrl()->AddBuf(LOG_CIM,"[EQST CHECK] FRONT		  - %s => %s", (strcmp(g_pData->m_cOldFront, "1")==0 ? "UP":"DOWN"),  (strcmp(g_pData->m_cFront, "1")==0 ? "UP":"DOWN"));
				GetLogCtrl()->AddBuf(LOG_CIM,"[EQST CHECK] REAR		  - %s => %s", (strcmp(g_pData->m_cOldRear, "1")==0 ? "UP":"DOWN"),  (strcmp(g_pData->m_cRear, "1")==0 ? "UP":"DOWN"));
				GetLogCtrl()->AddBuf(LOG_CIM,"[EQST CHECK] SAMPLE LOT	  - %s => %s", (strcmp(g_pData->m_cOldSampleLot, "1")==0 ? "ON":"OFF"),  (strcmp(g_pData->m_cSampleLot, "1")==0 ? "ON":"OFF"));


				break;

			case 104 : // Offline
			case 105 : // Local
			case 106 : // Remote
				m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 6, 11, 0);
					m_pXCom->SetListItem( lMkMsgID, 3);
					cstr = "0";
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	//DATAID				
					
					switch(ceid)
					{
							case 104: cstr = "104"; break;
							case 105: cstr = "105"; break;
							case 106: cstr = "106"; break;
					}

					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CEID
					m_pXCom->SetListItem( lMkMsgID, 2);
						m_pXCom->SetListItem( lMkMsgID, 2);
							cstr = "100";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID							
							m_pXCom->SetListItem( lMkMsgID, 2);
								cstr = g_pData->m_cEQPID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// EQPID
								cstr = g_pData->m_cCRST;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CRST
						m_pXCom->SetListItem( lMkMsgID, 2);
							cstr = "101";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID

							m_pXCom->SetListItem( lMkMsgID, 9);
								cstr = g_pData->m_cAvilability;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// AVAILABILITY
								cstr = g_pData->m_cInterlock;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// INTERLOCK
								cstr = g_pData->m_cMove;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// MOVE STATUS
// 								if (ExistAllCellChk())
// 								{
// 									strcpy_s( g_pData->m_cRun,"2");
// 								}
// 								else
// 									strcpy_s( g_pData->m_cRun,"1");
								cstr = g_pData->m_cRun;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RUN STATUS
								cstr = g_pData->m_cFront;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// FRONT STATUS		
								cstr = g_pData->m_cRear;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// REAR STATUS	
								cstr = g_pData->m_cSampleLot;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// SAMPLE LOT STATUS
								cstr = g_pData->m_cReasonCode;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RESONCODE
								cstr = g_pData->m_cDescription;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// DESCRIPTION

	
				GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 Control State Change CEID(%d)", ceid);
				nReturn = m_pXCom->Send( lMkMsgID );
				GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 Control State Change CEID(%d) Send Result(%d)", ceid, nReturn );
								
				break;

			case 107 : // PPID Change
				m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 6, 11, 0);
					m_pXCom->SetListItem( lMkMsgID, 3);
					cstr = "0";
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	//DATAID				
					cstr = "107";
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CEID

					m_pXCom->SetListItem( lMkMsgID, 2);

						m_pXCom->SetListItem( lMkMsgID, 2);
							cstr = "100";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID							
							m_pXCom->SetListItem( lMkMsgID, 2);
								cstr = g_pData->m_cEQPID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// EQPID
								cstr = g_pData->m_cCRST;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CRST

						m_pXCom->SetListItem( lMkMsgID, 2);
							cstr="302";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID

							m_pXCom->SetListItem( lMkMsgID, 3);
								cstr = g_pData->m_cPPID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// PPID
								cstr = "1";
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// PPID_TYPE
								cstr = g_pData->m_cOldPPID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// OLDPPID


				GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 PPID Change CEID(%d)", ceid);
				//if(strcmp(g_pData->m_cCRST, "0")!=0)
				if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
				{
					nReturn = m_pXCom->Send( lMkMsgID );
					GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 PPID Change CEID(%d) Send Result(%d)", ceid, nReturn );
				}
				break;

			case 200 : // Material Change
				break;

			case 403:	//Data Collection
// 				pDCollData = &g_pData->m_Jig[unitid].tCellData[cellposid].m_tDColl_Data ;
// 				pCellData = &g_pData->m_Jig[unitid].tCellData[cellposid];
// 
// 				strCellID = pCellData->m_cCellID;
// 				strProductID = pCellData->m_cProductID;
// 				strStepID = pCellData->m_cStepID;
// 
// 		
// 				m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 6, 11, 0);
// 					m_pXCom->SetListItem( lMkMsgID, 3);
// 						str = "0";
// 						m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	//DATAID
// 						str = "403";
// 						m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CEID
// 						m_pXCom->SetListItem( lMkMsgID, 3);
// 							m_pXCom->SetListItem( lMkMsgID, 2);
// 								str = "100";
// 								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
// 								m_pXCom->SetListItem( lMkMsgID, 2);
// 									str = g_pData->m_cEQPID;
// 									m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// EQPID
// 									str = g_pData->m_cCRST;
// 									m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CRST
// 							m_pXCom->SetListItem( lMkMsgID, 2);
// 								str = "300";
// 								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
// 								m_pXCom->SetListItem( lMkMsgID, 4);
// 									str = strCellID;
// 									m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CELLID
// 									str = g_pData->m_cPPID;
// 									m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// PPID
// 									str = strProductID;
// 									m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// PRODUCTID
// 									str = strStepID;
// 									m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// STEPID
// 							m_pXCom->SetListItem( lMkMsgID, 2);
// 								str = "600";
// 								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
// 								//FOR TEST
// 								m_pXCom->SetListItem( lMkMsgID, 0);	//TEST  
// // 								tag_Recipe_Validation::GetRcpParam(g_pData->m_cFixPPID, &recipeData, g_pData->m_cFixPPIDMode);
// 								m_pXCom->SetListItem( lMkMsgID, pDCollData->nDataCount + pTeachData->nDataCount + pConstantData->nDataCount + recipeData.nDataCount);
// 									for (int i=0; i<pDCollData->nDataCount; i++)
// 									{
// 										m_pXCom->SetListItem( lMkMsgID, 2);
// 											str=pDCollData->cDataName[i];
// 											m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// DVNAME
// 											str=pDCollData->cDataValue[i];
// 											m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// DV
// 									}
// 									for (int j=0; j<pTeachData->nDataCount; j++)
// 									{
// 										m_pXCom->SetListItem( lMkMsgID, 2);
// 											str=pTeachData->cDataName[j];
// 											m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// DVNAME
// 											str=pTeachData->cDataValue[j];
// 											m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// DV
// 									}
// 									for (int k=0; k<pConstantData->nDataCount; k++)
// 									{
// 										m_pXCom->SetListItem( lMkMsgID, 2);
// 										str=pConstantData->cDataName[k];
// 										m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// DVNAME
// 										str=pConstantData->cDataValue[k];
// 										m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// DV
// 									}
// 									for (int l=0; l<recipeData.nDataCount; l++)
// 									{
// 										m_pXCom->SetListItem( lMkMsgID, 2);
// 										str=recipeData.cDataName[l];
// 										m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// DVNAME
// 										str=recipeData.cDataValue[l];
// 										m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// DV
// 									}
// 
// 
// 				GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 Data Collection CEID(%d), CELL ID( %s )", ceid, strCellID);
// 				//if(strcmp(g_pData->m_cCRST, "0")!=0)
// 				if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
// 				{
// 					nReturn = m_pXCom->Send( lMkMsgID );
// 					GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 Data Collection CEID(%d) Send Result(%d)", ceid, nReturn );
// 				}

				break;

			case 501 : // OPCallEvent
				m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 6, 11, 0);
				m_pXCom->SetListItem( lMkMsgID, 3);
					cstr = "0";
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	//DATAID
					cstr = "501";
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CEID
					m_pXCom->SetListItem( lMkMsgID, 4);
						m_pXCom->SetListItem( lMkMsgID, 2);
							cstr = "100";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
							m_pXCom->SetListItem( lMkMsgID, 2);
								cstr = g_pData->m_cEQPID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// EQPID
								cstr = g_pData->m_cCRST;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CRST
						m_pXCom->SetListItem( lMkMsgID, 2);
							cstr = "101";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID

							m_pXCom->SetListItem( lMkMsgID, 9);
								cstr = g_pData->m_cAvilability;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// AVAILABILITY
								cstr = g_pData->m_cInterlock;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// INTERLOCK
								cstr = g_pData->m_cMove;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// MOVE STATUS
// 								if (ExistAllCellChk())
// 								{									
// 									strcpy_s( g_pData->m_cOldRun,g_pData->m_cRun);
// 									strcpy_s( g_pData->m_cRun,"2");
// 								}	
// 								else
// 								{
// 									strcpy_s( g_pData->m_cOldRun,g_pData->m_cRun);
// 									strcpy_s( g_pData->m_cRun,"1");
// 								}
								cstr = g_pData->m_cRun;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RUN STATUS
								cstr = g_pData->m_cFront;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// FRONT STATUS		
								cstr = g_pData->m_cRear;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// REAR STATUS	
								cstr = g_pData->m_cSampleLot;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// SAMPLE LOT STATUS
								cstr = g_pData->m_cReasonCode;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RESONCODE

								// 2015-08-12, jhLee, 쓰레기값 방지용, 항상 공백으로 보고
								cstr = _T("");	// g_pData->m_cDescription;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// DESCRIPTION

						m_pXCom->SetListItem( lMkMsgID, 2);
							cstr = "300";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
							m_pXCom->SetListItem( lMkMsgID, 4);
								cstr = "";
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CELLID
								cstr = g_pData->m_cPPID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// PPID
								cstr = "";
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// PRODUCTID
								cstr = "";
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// STEPID
						m_pXCom->SetListItem( lMkMsgID, 2);
							cstr = "700";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
							m_pXCom->SetListItem( lMkMsgID, g_pData->m_nOPCallCount);
							for (int i=0; i<g_pData->m_nOPCallCount; i++)
							{
								m_pXCom->SetListItem( lMkMsgID, 2);
								cstr = g_pData->m_tOPCallMES[i].m_cOPCallID;

								//2016-05-12, jhLee, OPCall/Interlock에 대한 Confirm 회신시 문자열 깨짐현상, 
								// 회신하고자 하는 문자열의 길이만큼만 전송하도록 변경함.
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// 20);	// OPCALLID
								
								cstr = g_pData->m_tOPCallMES[i].m_cOPCallMSG;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	//120);	// OPCALLMSG
							}		

// 							//Hwarang 마지막 발생내역만 Confirm 
// 							m_pXCom->SetListItem( lMkMsgID, 1);
// 
// 							m_pXCom->SetListItem( lMkMsgID, 2);
// 							str = g_pData->m_tOPCallMES[g_pData->m_nOPCallCount-1].m_cOPCallID;
// 							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// 20);	// OPCALLID
// 						
// 							str = g_pData->m_tOPCallMES[g_pData->m_nOPCallCount-1].m_cOPCallMSG;
// 							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	//120);	// OPCALLMSG

				GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 OPCallEvent( %d ), OPCallCount( %d )", ceid, g_pData->m_nOPCallCount);

				//if(strcmp(g_pData->m_cCRST, "0")!=0)
				if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
				{
					nReturn = m_pXCom->Send( lMkMsgID );
					GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 OPCallEvent Send Result( %d )", nReturn );
				}

				for (int j=0; j<MAX_OPCALL_MES; j++)
				{
					g_pData->m_tOPCallMES[j].Clear();
				}
				g_pData->m_nOPCallCount = 0;
									

				break;			

			case 502 : // InterlockEvent
				m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 6, 11, 0);
				m_pXCom->SetListItem( lMkMsgID, 3);
					cstr = "0";
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	//DATAID
					cstr = "502";
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CEID
					m_pXCom->SetListItem( lMkMsgID, 4);
						m_pXCom->SetListItem( lMkMsgID, 2);
							cstr = "100";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
							m_pXCom->SetListItem( lMkMsgID, 2);
								cstr = g_pData->m_cEQPID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// EQPID
								cstr = g_pData->m_cCRST;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CRST
						m_pXCom->SetListItem( lMkMsgID, 2);
							cstr = "101";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID

							m_pXCom->SetListItem( lMkMsgID, 9);
								cstr = g_pData->m_cAvilability;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// AVAILABILITY
								cstr = g_pData->m_cInterlock;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// INTERLOCK
								cstr = g_pData->m_cMove;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// MOVE STATUS
// 								if (ExistAllCellChk())
// 								{									
// 									strcpy_s( g_pData->m_cOldRun,g_pData->m_cRun);
// 									strcpy_s( g_pData->m_cRun,"2");
// 								}	
// 								else
// 								{
// 									strcpy_s( g_pData->m_cOldRun,g_pData->m_cRun);
// 									strcpy_s( g_pData->m_cRun,"1");
// 								}
								cstr = g_pData->m_cRun;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RUN STATUS
								cstr = g_pData->m_cFront;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// FRONT STATUS		
								cstr = g_pData->m_cRear;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// REAR STATUS	
								cstr = g_pData->m_cSampleLot;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// SAMPLE LOT STATUS
								cstr = g_pData->m_cReasonCode;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RESONCODE

								// 2015-08-17, jhLee, 이 부분이 아니므로 원복 함.
								// 2015-08-12, jhLee, 쓰레기값 방지용, 항상 공백으로 보고
								// str = _T("");	

								g_pData->m_cDescription;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// DESCRIPTION


						m_pXCom->SetListItem( lMkMsgID, 2);
							cstr = "300";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
							m_pXCom->SetListItem( lMkMsgID, 4);
								cstr = "";
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CELLID
								cstr = g_pData->m_cPPID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// PPID
								cstr = "";
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// PRODUCTID
								cstr = "";
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// STEPID
						m_pXCom->SetListItem( lMkMsgID, 2);
							cstr = "701";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
							
							// Interlock type별 구분 - LSH171128
							if(strcmp(g_pData->m_strInterlockClearType, EFST_STEP) == 0)
								nInterlockType = 2;
							else if(strcmp(g_pData->m_strInterlockClearType, EFST_LOADING) == 0)
								nInterlockType = 12;
							else if(strcmp(g_pData->m_strInterlockClearType, EFST_TRANSFER) == 0)
								nInterlockType = 11;
							else
								nInterlockType = 14;

							m_pXCom->SetListItem( lMkMsgID, g_pData->GetInterlockCount(nInterlockType));
							for (int i=0; i<MAX_INTERLOCK_MES ; i++)
							{
								if(g_pData->m_tInterlockMES[i].m_cInterlockType == nInterlockType)
								{
									if(g_pData->m_tInterlockMES[i].m_cInterlockState == INTERLOCK_HAPPEN)
									{
										m_pXCom->SetListItem( lMkMsgID, 2);

										/* ID */
										cstr = g_pData->m_tInterlockMES[i].m_cInterlockID;
										//2016-05-12, jhLee, OPCall/Interlock에 대한 Confirm 회신시 문자열 깨짐현상, 
										// 회신하고자 하는 문자열의 길이만큼만 전송하도록 변경함.
										m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());		// 20);	// INTERLOCKID

										/* Message */
										cstr = g_pData->m_tInterlockMES[i].m_cInterlockMSG;
										m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(),  cstr.GetLength());	// 120);	// INTERLOCKMSG
									}
								}
// 								CString sTemp;

								//2016-05-12, jhLee, strlen() 함수로 변경 함.
								//
								// 2015-08-17, jhLee, 이부분에서 인터락 메세지 끝에 쓰레기 값이 들어가서 전송되는 현상이 발생 함.
								// 메세지를 받는 부분으로 되돌아가서 허용된 글자 외의 문자는 곧바로 널처리 하도록 변경
								// 
								//ORG str = g_pData->m_tInterlockMES[i].m_cInterlockMSG;
								// sTemp =  g_pData->m_tInterlockMES[i].m_cInterlockMSG;
								// str = GetCorrectString(sTemp, 120);								// 사용가능한 글자까지만 수용하도록 처리한다.
							}	
							
 							////Hwarang 마지막 Interlock 만 Confirm
							////kjpark 20171106 마지막이 아니라 처음꺼
							//m_pXCom->SetListItem( lMkMsgID, 1);					

							//m_pXCom->SetListItem( lMkMsgID, 2);

							////str = g_pData->m_tInterlockMES[g_pData->GetInterlockCount()-1].m_cInterlockID;
							//str = g_pData->m_tInterlockMES[0].m_cInterlockID;
							//m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());		// 20);	// INTERLOCKID
						
							////str = g_pData->m_tInterlockMES[g_pData->GetInterlockCount()-1].m_cInterlockMSG;
							//str = g_pData->m_tInterlockMES[0].m_cInterlockMSG;
							//m_pXCom->SetAsciiItem( lMkMsgID, str,  cstr.GetLength());	// 120);	// INTERLOCKMSG

				GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 InterlockEvent( %d ), InterlockCount( %d )", ceid, g_pData->GetInterlockCount());
				//if(strcmp(g_pData->m_cCRST, "0")!=0)
				if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
				{
					nReturn = m_pXCom->Send( lMkMsgID );
					GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 InterlockEvent Send Result( %d )", nReturn );
				}
									
				break;	

			case 601 : //Cell ID Reading Result
				m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 6, 11, 0);
					m_pXCom->SetListItem( lMkMsgID, 3);
						cstr = "0";
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	//DATAID				
						cstr = "601";
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CEID
							m_pXCom->SetListItem( lMkMsgID, 1);
								m_pXCom->SetListItem( lMkMsgID, 5);
									cstr = "800";
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID							
									cstr = g_pData->m_cEQPID;
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// EQPID
									cstr = strCellID;
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CELLID
									cstr = strReaderID;
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// READERID
									cstr = strReaderResultCode;
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// READERRESULTCODE
				
				GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 Cell ID Reading Result CEID(%d)", ceid);

				//if(strcmp(g_pData->m_cCRST, "0")!=0)
				if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
				{
					nReturn = m_pXCom->Send( lMkMsgID );
					GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 Cell ID Reading Result CEID(%d) Send Result(%d)", ceid, nReturn );
				}
				break;

			case 602 :	//Start Cell Lot
				m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 6, 11, 0);
					m_pXCom->SetListItem( lMkMsgID, 3);
						cstr = "0";
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	//DATAID				
						cstr = "601";
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CEID
							m_pXCom->SetListItem( lMkMsgID, 1);
								m_pXCom->SetListItem( lMkMsgID, 5);
									cstr = "801";
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
									cstr = g_pData->m_cEQPID;
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// EQPID
									cstr = strReaderID;
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// READERID
									cstr = strReaderResultCode;
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// READERRESULTCODE
									//int iStartCellCnt=10;	//임시
									m_pXCom->SetListItem( lMkMsgID, iStartCellCnt);
									for (int i=0; i<iStartCellCnt; i++)
									{
										m_pXCom->SetListItem( lMkMsgID, 2);
										cstr="CELLID";
										m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// DVNAME
										cstr="PARENTLOT";
										m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// DV
									}
				GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 Start Cell Lot CEID(%d)", ceid);
				//if(strcmp(g_pData->m_cCRST, "0")!=0)
				if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
				{
					nReturn = m_pXCom->Send( lMkMsgID );
					GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 Start Cell Lot CEID(%d) Send Result(%d)", ceid, nReturn );
				}
				break;

			case 603 : // ERMSLossMsg //Equipment Status Change by User
				m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 6, 11, 0);
					m_pXCom->SetListItem( lMkMsgID, 3);
					cstr = "0";
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	//DATAID				
					cstr = "603";
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CEID
					m_pXCom->SetListItem( lMkMsgID, 3);
						m_pXCom->SetListItem( lMkMsgID, 2);
							cstr = "802";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID							
							m_pXCom->SetListItem( lMkMsgID, 2);
								cstr = g_pData->m_cEQPID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// EQPID
								cstr = "PC";
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// DATATYPE
						m_pXCom->SetListItem( lMkMsgID, 2);
							cstr = "803";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
							m_pXCom->SetListItem( lMkMsgID, 2);
								cstr = "";
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());
						m_pXCom->SetListItem( lMkMsgID, 2);
							cstr="804";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
							m_pXCom->SetListItem( lMkMsgID, 2);
// 								str = g_pData->m_tERSM.m_cLossDisplay;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// LOSSDISPLAY
// 								str = g_pData->m_tERSM.m_cLoss;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// LOSS
								

// 				GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 ERMSLossMsg CEID(%d) LossDisplay(%s) Loss(%s) ", ceid, g_pData->m_tERSM.m_cLossDisplay, g_pData->m_tERSM.m_cLoss);
				//if(strcmp(g_pData->m_cCRST, "0")!=0)
				if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
				{
					nReturn = m_pXCom->Send( lMkMsgID );
					GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 ERMSLossMsg CEID(%d) Send Result(%d)", ceid, nReturn );
				}

				//g_pData->m_tERSM.Clear();
				break;

			case 606 : //20141223 sms Equipment Loss Code Report
				m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 6, 11, 0);
					m_pXCom->SetListItem( lMkMsgID, 3);
					cstr = "0";
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	//DATAID				
					cstr = "606";
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CEID
					m_pXCom->SetListItem( lMkMsgID, 3);
						m_pXCom->SetListItem( lMkMsgID, 2);
							cstr = "100";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID							
							m_pXCom->SetListItem( lMkMsgID, 2);
								cstr = g_pData->m_cEQPID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// EQPID
								cstr = g_pData->m_cCRST;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CRST
						m_pXCom->SetListItem( lMkMsgID, 2);
							cstr = "101";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
							m_pXCom->SetListItem( lMkMsgID, 9);
								cstr = g_pData->m_cAvilability;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// AVAILABILITY
								cstr = g_pData->m_cInterlock;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// INTERLOCK
								cstr = g_pData->m_cMove;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// MOVE STATUS
// 								if (ExistAllCellChk())
// 								{									
// 									strcpy_s( g_pData->m_cOldRun,g_pData->m_cRun);
// 									strcpy_s( g_pData->m_cRun,"2");
// 								}	
// 								else
// 								{
// 									strcpy_s( g_pData->m_cOldRun,g_pData->m_cRun);
// 									strcpy_s( g_pData->m_cRun,"1");
// 								}
								cstr = g_pData->m_cRun;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RUN STATUS
								cstr = g_pData->m_cFront;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// FRONT STATUS		
								cstr = g_pData->m_cRear;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// REAR STATUS	
								cstr = g_pData->m_cSampleLot;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// SAMPLE LOT STATUS
								cstr = g_pData->m_cReasonCode;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RESONCODE
								cstr = g_pData->m_cDescription;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// DESCRIPTION
						m_pXCom->SetListItem( lMkMsgID, 2);

						//2016-05-12, jhLee, Spec V1.08 대응, 기존 805 -> 806으로 변경
							cstr = "806";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
							m_pXCom->SetListItem( lMkMsgID, 2);
								cstr = g_pData->m_tLossCode.m_cLossCode;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// Loss Code
								cstr = g_pData->m_tLossCode.m_cLossDesc;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// Description

								GetLogCtrl()->AddBuf(LOG_CIM,"[EQST CHECK] MAINT - %s => %s", (strcmp(g_pData->m_cOldMaint, "1")==0 ? "ON":"OFF"),  (strcmp(g_pData->m_cMaint, "1")==0 ? "ON":"OFF"));
								GetLogCtrl()->AddBuf(LOG_CIM,"[EQST CHECK] AVAILABILITY - %s => %s", (strcmp(g_pData->m_cOldAvilability, "1")==0 ? "DOWN":"UP"),  (strcmp(g_pData->m_cAvilability, "1")==0 ? "DOWN":"UP"));
								GetLogCtrl()->AddBuf(LOG_CIM,"[EQST CHECK] INTERLOCK	  - %s => %s", (strcmp(g_pData->m_cOldInterlock, "1")==0 ? "ON":"OFF"),  (strcmp(g_pData->m_cInterlock, "1")==0 ? "ON":"OFF"));
								GetLogCtrl()->AddBuf(LOG_CIM,"[EQST CHECK] MOVE		  - %s => %s", (strcmp(g_pData->m_cOldMove, "1")==0 ? "PAUSE":"RUNNING"),  (strcmp(g_pData->m_cMove, "1")==0 ? "PAUSE":"RUNNING"));
								GetLogCtrl()->AddBuf(LOG_CIM,"[EQST CHECK] RUN		  - %s => %s", (strcmp(g_pData->m_cOldRun, "1")==0 ? "NOT CELL":"IS CELL"),  (strcmp(g_pData->m_cRun, "1")==0 ? "NOT CELL":"IS CELL"));
								GetLogCtrl()->AddBuf(LOG_CIM,"[EQST CHECK] FRONT		  - %s => %s", (strcmp(g_pData->m_cOldFront, "1")==0 ? "UP":"DOWN"),  (strcmp(g_pData->m_cFront, "1")==0 ? "UP":"DOWN"));
								GetLogCtrl()->AddBuf(LOG_CIM,"[EQST CHECK] REAR		  - %s => %s", (strcmp(g_pData->m_cOldRear, "1")==0 ? "UP":"DOWN"),  (strcmp(g_pData->m_cRear, "1")==0 ? "UP":"DOWN"));
								GetLogCtrl()->AddBuf(LOG_CIM,"[EQST CHECK] SAMPLE LOT	  - %s => %s", (strcmp(g_pData->m_cOldSampleLot, "1")==0 ? "ON":"OFF"),  (strcmp(g_pData->m_cSampleLot, "1")==0 ? "ON":"OFF"));

				GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 Equipment Loss Code Report CEID(%d) LossCode(%s) Description(%s) ", ceid, g_pData->m_tLossCode.m_cLossCode, g_pData->m_tLossCode.m_cLossDesc);
				//if(strcmp(g_pData->m_cCRST, "0")!=0)
				if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
				{				
					nReturn = m_pXCom->Send( lMkMsgID );
					GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 Equipment Loss Code Report CEID(%d) Send Result( %d )", ceid, nReturn );						
				}

				break;
			case 607: //GT.PARK - Login Event
				m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 6, 11, 0);
					m_pXCom->SetListItem( lMkMsgID, 3);
					cstr = "0";
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	//DATAID				
					cstr = "607";
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CEID
					m_pXCom->SetListItem( lMkMsgID, 2);
						m_pXCom->SetListItem( lMkMsgID, 2);
							cstr = "105";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID							
							m_pXCom->SetListItem( lMkMsgID, 3);
								cstr = g_pData->m_cEQPID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// EQPID
								cstr = g_pData->m_cOptionInfo;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// OPTIONINFO
								cstr = g_pData->m_cComment;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// COMMENT
						m_pXCom->SetListItem( lMkMsgID, 2);
							cstr="106";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
							m_pXCom->SetListItem( lMkMsgID, 2);
								cstr = g_pData->m_cOperatorID1;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// OPERATORID
								cstr = g_pData->m_cPassword;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// PASSWORD

				GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 Login CEID(%d)", ceid);
				//if(strcmp(g_pData->m_cCRST, "0")!=0)
				if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
				{
					nReturn = m_pXCom->Send( lMkMsgID );
					GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 Login CEID(%d) Send Result(%d)", ceid, nReturn );
				}
				break;

			// 2016-11-19, jhLee, 화상검사 혹은 MTP등 검사 종료되었을 경우 Track-Out 보고전에 event를 전송한다.
			case 609 :	// Inspection Result Report
// 				pCellData = &g_pData->m_Jig[unitid].tCellData[cellposid];
				pCellData = &g_pData->m_CellDataInspectionEnd;
				strCellID = pCellData->m_cCellID;
				strOperatorID1 = pCellData->m_cOperatorID1;	// jhkim1 OperatorID가 1~2밖에 존재 하지 않고 운영 프로그램이 OperatorID가 하나만 사용하여 1로 통일 추후 수정 필요
				strJuge = pCellData->m_cJudge;
				strReasonCode = pCellData->m_cReasonCode;
				strDescription = pCellData->m_cDescription;


				//kjpark 20170710 판정 보고시 항목 추가
				strProcessFlag = pCellData->m_cProcessFlag;


				m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 6, 11, 0);
				m_pXCom->SetListItem( lMkMsgID, 3);
					cstr = "0";
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	//DATAID
					cstr = "609";	
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CEID
					m_pXCom->SetListItem( lMkMsgID, 2);
						m_pXCom->SetListItem( lMkMsgID, 2);
							cstr = "100";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
							m_pXCom->SetListItem( lMkMsgID, 2);
								cstr = g_pData->m_cEQPID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// EQPID
								cstr = g_pData->m_cCRST;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CRST

						m_pXCom->SetListItem( lMkMsgID, 2);
							cstr = "808";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
							m_pXCom->SetListItem( lMkMsgID, 8);
								cstr = g_pData->m_cProcessID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// PRPCESSNAME, 공정명, AMT로 고정, SFT,MTP....
								cstr = strCellID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CELLID
								//str = "SKIP";
								cstr = strProcessFlag;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// PROCESSFLAG, 공정 진행 결과, Y or N or OK or NG or SKIP
								cstr = strJuge;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// JUDGE
								cstr = strReasonCode;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// REASONCODE, 사유코드
								cstr = strOperatorID1;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// OPERATORID, 작업자 정보
								cstr = "";
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// DESCRIPTION, 추가정보
								cstr = "";
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// DESCRIPTION, 추가정보							

				if(strcmp(g_pData->m_cCRST, "0") != 0)	// 전송이 가능한 상태 ?
				{
					nReturn = m_pXCom->Send( lMkMsgID );
					GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 Inspection Result Report CEID(%d), CELL POS(%d), CELL ID(%s), Return:%d", ceid, cellposid+1, strCellID, nReturn);
				}
				else
					GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 Inspection Result Report CEID(%d), CELL POS(%d), CELL ID(%s), Send Fail, Off-Line", ceid, cellposid+1, strCellID);
				break;

// 			case 701 :
// 			case 771 :
// 			case 706 :	
// 				pCellData = &g_pData->m_Jig[unitid].tCellData[cellposid];
// 				strCellID = pCellData->m_cCellID;
// 				strPJID = pCellData->m_cPJID;
// 				strPlanQty = pCellData->m_cPlanQty;
// 				strProcessedQty = pCellData->m_cProcessedQty; 
// 				strReaderID = pCellData->m_cReaderID;
// 				strReaderResultCode = pCellData->m_cReaderResultCode;	
// 				strStepID = pCellData->m_cStepID;
// 				strProductID = pCellData->m_cProductID;	
// 
// 				m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 6, 11, 0);
// 				m_pXCom->SetListItem( lMkMsgID, 3);
// 					str = "0";
// 					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	//DATAID
// 					
// 					if(ceid==701)
// 						str = "701";
// 					else if(ceid==771)
// 						str = "771";
// 					else if(ceid==706)
// 						str = "706";
// 
// 					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CEID
// 					m_pXCom->SetListItem( lMkMsgID, 5);
// 						m_pXCom->SetListItem( lMkMsgID, 2);
// 							str = "100";
// 							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
// 							m_pXCom->SetListItem( lMkMsgID, 2);
// 								str = g_pData->m_cEQPID;
// 								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// EQPID
// 								str = g_pData->m_cCRST;
// 								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CRST
// 						m_pXCom->SetListItem( lMkMsgID, 2);
// 							str = "101";
// 							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
// 							m_pXCom->SetListItem( lMkMsgID, 9);
// 							str = g_pData->m_cAvilability;
// 							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// AVAILABILITY
// 							str = g_pData->m_cInterlock;
// 							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// INTERLOCK
// 							str = g_pData->m_cMove;
// 							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// MOVE STATUS
// 							str = g_pData->m_cRun;
// 							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RUN STATUS
// 							str = g_pData->m_cFront;
// 							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// FRONT STATUS		
// 							str = g_pData->m_cRear;
// 							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// REAR STATUS
// 							str = g_pData->m_cSampleLot;
// 							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// SAMPLE LOT STATUS
// 							str = g_pData->m_cReasonCode;
// 							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RESONCODE
// 							str = g_pData->m_cDescription;
// 							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// DESCRIPTION
// 						m_pXCom->SetListItem( lMkMsgID, 2);
// 							str = "300";
// 							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
// 							m_pXCom->SetListItem( lMkMsgID, 4);
// 								str = strCellID;
// 								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CELLID
// 								str = g_pData->m_cPPID;
// 								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// PPID
// 								str = strProductID;
// 								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// PRODUCTID
// 								str = strStepID;
// 								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// STEPID
// 						m_pXCom->SetListItem( lMkMsgID, 2);	
// 							str = "301";
// 							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
// 							m_pXCom->SetListItem( lMkMsgID, 3);
// 								str = strPJID;
// 								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// PJID
// 								str = strPlanQty;
// 								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// PLANQTY
// 								str = strProcessedQty;
// 								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// PROCESSQTY
// 						m_pXCom->SetListItem( lMkMsgID, 2);	
// 							str = "400";
// 							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
// 							m_pXCom->SetListItem( lMkMsgID, 2);
// 								str = "";
// 								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// READERID
// 								str = "";
// 								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// READERRESULTCODE
// 				if (ceid==701)
// 					GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 Cell Loader In - CEID( %d ), CELL POS( %d ), CELL ID( %s )", ceid, cellposid+1, strCellID);
// 				else if(ceid==771)
// 					GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 Cell Tester Out - CEID( %d ), CELL POS( %d ), CELL ID( %s )", ceid, cellposid+1, strCellID);
// 				else if(ceid==706)
// 					GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 Cell Unloader Out - CEID( %d ), CELL POS( %d ), CELL ID( %s )", ceid, cellposid+1, strCellID);
// 
// 				//if(strcmp(g_pData->m_cCRST, "0")!=0)
// 				if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
// 				{
// 					nReturn = m_pXCom->Send( lMkMsgID );
// 					if (ceid==701)
// 						GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 Cell Loader In - CEID( %d ), CELL POS( %d ), Send Result( %d )", ceid, cellposid+1, nReturn);
// 					else if(ceid==771)
// 						GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 Cell Tester Out - CEID( %d ), CELL POS( %d ), Send Result( %d )", ceid, cellposid+1, nReturn);
// 					else if(ceid==706)
// 						GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 Cell Unloader Out - CEID( %d ), CELL POS( %d ), Send Result( %d )", ceid, cellposid+1, nReturn);		
// 				}
// 
// 				break;
						

			// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

			// 2016-02-13, jhLee, 점등 불량 LOSS 보고를 위한 Track In/Out 통합 보고 지령
			case 4016 :	//Cell Process Start / Cell Process Complete
				pCellData			= &g_pData->m_CellDummy;				//  m_Jig[unitid].tCellData[cellposid];
				strCellID			= pCellData->m_cCellID;
				strPJID				= pCellData->m_cPJID;
				strPlanQty			= pCellData->m_cPlanQty;
				strProcessedQty		= pCellData->m_cProcessedQty; 
				strReaderID			= pCellData->m_cReaderID;
				strReaderResultCode	= pCellData->m_cReaderResultCode;	
				strStepID			= pCellData->m_cStepID;
				strProductID		= pCellData->m_cProductID;	

				strMaterialId	= pCellData->m_cMaterialID;
				strMLN			= pCellData->m_cMLN;
				strDVName		= pCellData->m_cDVName;
				strDV			= pCellData->m_cDV;

				strOperatorID1	= pCellData->m_cOperatorID1;	// jhkim1 OperatorID가 1~2밖에 존재 하지 않고 운영 프로그램이 OperatorID가 하나만 사용하여 1로 통일 추후 수정 필요
				strOperatorID2	= pCellData->m_cOperatorID1;
				strOperatorID3	= pCellData->m_cOperatorID1;

				strJuge			= pCellData->m_cJudge;
				strReasonCode	= pCellData->m_cReasonCode;
				strDescription	= pCellData->m_cDescription;


				m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 6, 11, 0);
				m_pXCom->SetListItem( lMkMsgID, 3);
					cstr = "0";
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	//DATAID
					cstr = "401";
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CEID
					m_pXCom->SetListItem( lMkMsgID, 5);
						m_pXCom->SetListItem( lMkMsgID, 2);
							cstr = "100";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
							m_pXCom->SetListItem( lMkMsgID, 2);
								cstr = g_pData->m_cEQPID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// EQPID
								cstr = g_pData->m_cCRST;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CRST
						m_pXCom->SetListItem( lMkMsgID, 2);
							cstr = "101";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID

							m_pXCom->SetListItem( lMkMsgID, 9);
								cstr = g_pData->m_cAvilability;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// AVAILABILITY
								cstr = g_pData->m_cInterlock;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// INTERLOCK
								cstr = g_pData->m_cMove;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// MOVE STATUS
								cstr = g_pData->m_cRun;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RUN STATUS
								cstr = g_pData->m_cFront;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// FRONT STATUS		
								cstr = g_pData->m_cRear;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// REAR STATUS
								cstr = g_pData->m_cSampleLot;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// SAMPLE LOT STATUS
								cstr = g_pData->m_cReasonCode;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RESONCODE
								cstr = g_pData->m_cDescription;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// DESCRIPTION

						m_pXCom->SetListItem( lMkMsgID, 2);
							cstr = "300";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
							m_pXCom->SetListItem( lMkMsgID, 4);
								cstr = strCellID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CELLID
								cstr = g_pData->m_cPPID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// PPID
								cstr = strProductID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// PRODUCTID
								cstr = strStepID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// STEPID
						m_pXCom->SetListItem( lMkMsgID, 2);	
							cstr = "301";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
							m_pXCom->SetListItem( lMkMsgID, 3);
								cstr = strPJID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// PJID
								cstr = strPlanQty;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// PLANQTY
								cstr = strProcessedQty;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// PROCESSQTY
						m_pXCom->SetListItem( lMkMsgID, 2);	
							cstr = "400";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
							m_pXCom->SetListItem( lMkMsgID, 2);
								cstr = strReaderID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// READERID
								cstr = strReaderResultCode;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// READERRESULTCODE

				// Track in 보고를 전송한다.
				if (strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
				{
					nReturn = m_pXCom->Send( lMkMsgID );
					GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 Cell Process Start (401), Loss cell report : %s", strCellID);				
				}

				// case 406 :	//Cell Process Complete	// jhkim1 기존것 402-> 406 변경되며 추가

				// Track out 보고 조립
				m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 6, 11, 0);
				m_pXCom->SetListItem( lMkMsgID, 3);
					cstr = "0";
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	//DATAID
					cstr = "406";	
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CEID
					m_pXCom->SetListItem( lMkMsgID, 8);
						m_pXCom->SetListItem( lMkMsgID, 2);
							cstr = "100";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
							m_pXCom->SetListItem( lMkMsgID, 2);
								cstr = g_pData->m_cEQPID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// EQPID
								cstr = g_pData->m_cCRST;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CRST
						m_pXCom->SetListItem( lMkMsgID, 2);
							cstr = "101";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID

							m_pXCom->SetListItem( lMkMsgID, 9);
								cstr = g_pData->m_cAvilability;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// AVAILABILITY
								cstr = g_pData->m_cInterlock;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// INTERLOCK
								cstr = g_pData->m_cMove;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// MOVE STATUS
								cstr = g_pData->m_cRun;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RUN STATUS
								cstr = g_pData->m_cFront;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// FRONT STATUS		
								cstr = g_pData->m_cRear;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// REAR STATUS	
								cstr = g_pData->m_cSampleLot;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// SAMPLE LOT STATUS
								cstr = g_pData->m_cReasonCode;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RESONCODE
								cstr = g_pData->m_cDescription;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// DESCRIPTION

						m_pXCom->SetListItem( lMkMsgID, 2);
							cstr = "300";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
							m_pXCom->SetListItem( lMkMsgID, 4);
								cstr = strCellID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CELLID
								cstr = g_pData->m_cPPID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// PPID
								cstr = strProductID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// PRODUCTID
								cstr = strStepID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// STEPID
						m_pXCom->SetListItem( lMkMsgID, 2);	
							cstr = "301";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
							m_pXCom->SetListItem( lMkMsgID, 3);
								cstr = strPJID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// PJID
								cstr = strPlanQty;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// PLANQTY
								cstr = strProcessedQty;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// PROCESSQTY
						m_pXCom->SetListItem( lMkMsgID, 2);	
							cstr = "400";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
							m_pXCom->SetListItem( lMkMsgID, 2);
								cstr = "";	//str = strReaderID;			//2013.11.07. Reader 가독율 이중 취합으로 공백으로 보고. //SDC 송지훈 요청
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// READERID
								cstr = "";	//str = strReaderResultCode;	//2013.11.07. Reader 가독율 이중 취합으로 공백으로 보고. //SDC 송지훈 요청
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// READERRESULTCODE
						m_pXCom->SetListItem( lMkMsgID, 2);	
							cstr = "201";	// jhkim1 기존 것 200 -> 201 변경되며 추가
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
							m_pXCom->SetListItem( lMkMsgID, 0);
					
						m_pXCom->SetListItem( lMkMsgID, 2);	
							cstr = "600";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
							m_pXCom->SetListItem( lMkMsgID, 1);
							m_pXCom->SetListItem( lMkMsgID, 2);
							cstr = strDVName;
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// DVNAME
							cstr = strDV;
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// DV
						m_pXCom->SetListItem( lMkMsgID, 2);	

							cstr = "501";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID						
							m_pXCom->SetListItem( lMkMsgID, 6);
							cstr = strOperatorID1;
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// OPERATORID1
							cstr = "";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// OPERATORID2	// jhkim1 OperatorID가 1~2밖에 존재 하지 않고 운영 프로그램이 OperatorID가 하나만 사용하여 공백으로 보냄 추후 수정 필요
							cstr = "";
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// OPERATORID3	// jhkim1 OperatorID가 1~2밖에 존재 하지 않고 운영 프로그램이 OperatorID가 하나만 사용하여 공백으로 보냄 추후 수정 필요
							cstr = strJuge;
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// JUDGE
							cstr = strReasonCode;
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// REASONCODE
							cstr = strDescription;
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// DESCRIPTION
						// jhkim1 기존 것 500 -> 501 변경되며 추가 끝!!!

				if ( strcmp(g_pData->m_cCRST, CRST_OFF) != 0 )
				{
					nReturn = m_pXCom->Send( lMkMsgID );
					GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 Cell Process Complete (406), Loss cell report : %s", strCellID);				
				}

				break; // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


			default :
				break;
				GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 unrecognized ceid(%d)", ceid);

		}
	}
	CATCH_ALL (e) 
	{
		return CB_ERR;
	} 
	END_CATCH_ALL

	return TRUE;
}

BOOL CAutomation::TxS6F11_CEID102(int nJig, int nCh)
{
	int ceid = 102;
	long lMkMsgID = 0;
	CString cstr;
	long nReturn;
	CMainFrame* pMainFrm = (CMainFrame*)::AfxGetMainWnd();
	CUnitStatus *pUnit = &pMainFrm->m_pViewCIM->m_UnitStatusBank.m_Unit[nJig][nCh];
	if(pUnit == NULL)
		return FALSE;

	switch(ceid)
	{
	case 102 :	//Unit Status Change
		m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 6, 11, 0);
		m_pXCom->SetListItem( lMkMsgID, 3);
			cstr = "0";
			m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	//DATAID				
			cstr = "102";
			m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CEID
			m_pXCom->SetListItem( lMkMsgID, 4);
			{
				m_pXCom->SetListItem( lMkMsgID, 2);
				{
					cstr = "100";
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID							
					m_pXCom->SetListItem( lMkMsgID, 2);
					{
						cstr = g_pData->m_cEQPID;
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// EQPID
						cstr = g_pData->m_cCRST;
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CRST
					}
				}
				m_pXCom->SetListItem( lMkMsgID, 2);
				{
					cstr = "111";
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
					m_pXCom->SetListItem( lMkMsgID, 2);
					{
						cstr = pUnit->m_strUnitID;
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// MODULEID
						m_pXCom->SetListItem( lMkMsgID, 9);
						{
							cstr.Format(_T("%d"), pUnit->m_AvilabilityState);
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// AVAILABILITY
							cstr.Format(_T("%d"), pUnit->m_InterlockState);
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// INTERLOCK
							cstr.Format(_T("%d"), pUnit->m_MoveState);
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// MOVE STATUS
							cstr.Format(_T("%d"), pUnit->m_RunState);
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RUN STATUS
							cstr.Format(_T("%d"), pUnit->m_FrontState);
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// FRONT STATUS		
							cstr.Format(_T("%d"), pUnit->m_RearState);
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// REAR STATUS	
							cstr.Format(_T("%d"), pUnit->m_PP_SplState);
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// SAMPLE LOT STATUS
							cstr = pUnit->m_strResonCode;
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RESONCODE
							cstr = pUnit->m_strDescrition;
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// DESCRIPTION
						}
					}
				}
				m_pXCom->SetListItem( lMkMsgID, 2);	// RPTID 112 Set
				{
					cstr = "112";
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID
					m_pXCom->SetListItem( lMkMsgID, 2);					// Old Unit State Set
					{
						cstr = pUnit->m_strUnitID;
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// MODULEID
						m_pXCom->SetListItem( lMkMsgID, 9);					// Old Unit State Set
						{
							cstr.Format(_T("%d"), pUnit->m_oldAvilabilityState);
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// AVAILABILITY
							cstr.Format(_T("%d"), pUnit->m_oldInterlockState);
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// INTERLOCK
							cstr.Format(_T("%d"), pUnit->m_oldMoveState);
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// MOVE STATUS
							cstr.Format(_T("%d"), pUnit->m_oldRunState);
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RUN STATUS
							cstr.Format(_T("%d"), pUnit->m_oldFrontState);
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// FRONT STATUS		
							cstr.Format(_T("%d"), pUnit->m_oldRearState);
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// REAR STATUS	
							cstr.Format(_T("%d"), pUnit->m_oldPP_SplState);
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// SAMPLE LOT STATUS
							cstr = pUnit->m_strResonCode;
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RESONCODE
							cstr = pUnit->m_strDescrition;
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// DESCRIPTION
						}
					}
				}
				m_pXCom->SetListItem( lMkMsgID, 2);	// RPTID 104 Set
				{
					cstr = "104";
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RPTID

					// SDC 이현식PRO와 협의됨. 상태보고시는 인터락은 1개만 보고 [12/7/2017 OSC]
					int nInterlockCount = pUnit->GetInterlockCount();
					if(nInterlockCount > 0)
					{
						m_pXCom->SetListItem( lMkMsgID, 1);
						for (int i=0; i<MAX_INTERLOCK_MES; i++)
						{
							if(pUnit->m_tUnitInterlockMES[i].m_cInterlockState == INTERLOCK_HAPPEN)
							{
								m_pXCom->SetListItem( lMkMsgID, 4);
								cstr = "1";
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());		//ALST
								cstr = _T("2");	// 2 SERIUS, 1 Light
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());
								cstr = pUnit->m_tUnitInterlockMES[i].m_cInterlockID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());
								cstr = pUnit->m_tUnitInterlockMES[i].m_cInterlockMSG;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());
								break;
							}
						}

						// Interlock 발생 후 RUN이 되면 인터락 리스트 클리어 [12/2/2017 OSC]
						if(pUnit->m_MoveState == STATUS_MOVE_RUNNING)
						{
							for (int j=0; j<MAX_INTERLOCK_MES; j++)
							{
								if(pUnit->m_tUnitInterlockMES[j].m_cInterlockState == INTERLOCK_HAPPEN)
									pUnit->m_tUnitInterlockMES[j].Clear();
							}
						}
					}

// 					m_pXCom->SetListItem( lMkMsgID, g_pData->m_nCurAlarmListCnt);
// 					for (int i=0; i<g_pData->m_nCurAlarmListCnt; i++)
// 					{
// 						m_pXCom->SetListItem( lMkMsgID, 4);
// 						str = "1";
// 						m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());		//ALST
// 						str = g_pData->m_tCurAlarmList[i].m_cALCD;
// 						m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());
// 						str = g_pData->m_tCurAlarmList[i].m_cALID;
// 						m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());
// 						str = g_pData->m_tCurAlarmList[i].m_cALTX;
// 						m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());
// 					}
				}
			}

		GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 Unit Status Change CEID(%d), UNITID(%s)", ceid, pUnit->m_strUnitID);
		//if(strcmp(g_pData->m_cCRST, "0")!=0)
		if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
		{				
			nReturn = m_pXCom->Send( lMkMsgID );
			GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 Unit Status Change CEID(%d) Send Result( %d )", ceid, nReturn );						
		}

		break;
	}
	return TRUE;

}

/* LSH 17/11/29 */
BOOL CAutomation::TxS6F11_CEID111(EQUIP_FUNCTION* function, UINT nIndexSize, CString strBYWHO)
{
	int ceid = 111;
	long lMkMsgID = 0;
	CString cstr;
	long nReturn;

	switch(ceid)
	{
	case 111 :	//Equipment Function Change
		m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 6, 11, 0);//S6F11 메세지 생성

		m_pXCom->SetListItem( lMkMsgID, 3);//Equipment event info
		{
			cstr = "0";
			m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());//Data ID		
			cstr.Format(_T("%d"), ceid);
			m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());//Collection Event ID
			m_pXCom->SetListItem( lMkMsgID, 2);// RPTID Set
			{
				m_pXCom->SetListItem( lMkMsgID, 2);// RPTID 100
				{
					cstr = "100";
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());// RPTID
					m_pXCom->SetListItem( lMkMsgID, 2);// EQP Control state
					{
						cstr = g_pData->m_cEQPID;
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());// EQPID
						cstr = g_pData->m_cCRST;
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());// CRST
					}
				}
				m_pXCom->SetListItem( lMkMsgID, 2);// RPTID 314
				{
					cstr = "314";
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());// RPTID

					m_pXCom->SetListItem( lMkMsgID, nIndexSize);// Function list
					{
						for(UINT itor=0 ; itor<nIndexSize ; itor++)
						{
							m_pXCom->SetListItem( lMkMsgID, 4);// Function set
							{
								cstr = strBYWHO;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());// 변경의 주최
								cstr.Format(_T("%d"), function[itor].EFID);
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());// EFID
								cstr = function[itor].EFST;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());// NEWDFST
								cstr = function[itor].EFST_Old;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());// OLDDFST
							}
						}
					}
				}
			}

			//GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 Equipment function Change CEID(%d) ByWho(%s) EFST(%s)", ceid, strBYWHO, function.EFST);

			if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
			{				
				nReturn = m_pXCom->Send( lMkMsgID );
				GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 Unit Status Change CEID(%d) Send Result( %d )", ceid, nReturn );						
			}

			break;
		}
		
	}
	return TRUE;


}

BOOL CAutomation::TxS6F11_CEID513(int nJig, int nCh, CString strCellID, CString strProductID, CString strStepID)
{
	int ceid = 513;
	long lMkMsgID = 0;
	CString cstr;
	long nReturn;
	CMainFrame* pMainFrm = (CMainFrame*)::AfxGetMainWnd();
	CUnitStatus unit = pMainFrm->m_pViewCIM->m_UnitStatusBank.m_Unit[nJig][nCh];

	switch(ceid)
	{
	case 513 :	// UNIT OPCALL Confirm
		m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 6, 11, 0);//S6F11 메세지 생성

		m_pXCom->SetListItem( lMkMsgID, 3);//OPCALL event info
		{
			cstr = "0";
			m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());//Data ID		
			cstr.Format(_T("%d"), ceid);
			m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());//Collection Event ID
			m_pXCom->SetListItem( lMkMsgID, 4);	// RPTID Set
			{
				m_pXCom->SetListItem( lMkMsgID, 2);// RPTID 100Set
				{
					cstr = "100";
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());// RPTID
					m_pXCom->SetListItem( lMkMsgID, 2);// EQP Control state set
					{
						cstr = g_pData->m_cEQPID;
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());// EQPID
						cstr = g_pData->m_cCRST;
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());// CRST
					}
				}
				m_pXCom->SetListItem( lMkMsgID, 2);// RPTID 102 set
				{
					cstr = "102";
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());// RPTID

					m_pXCom->SetListItem( lMkMsgID, 2);
					{
						cstr = unit.m_strUnitID;
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// MODULEID
						m_pXCom->SetListItem( lMkMsgID, 9);
						{
							cstr.Format(_T("%d"), unit.m_AvilabilityState);
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// AVAILABILITY
							cstr.Format(_T("%d"), unit.m_InterlockState);
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// INTERLOCK
							cstr.Format(_T("%d"), unit.m_MoveState);
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// MOVE STATUS
							cstr.Format(_T("%d"), unit.m_RunState);
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RUN STATUS
							cstr.Format(_T("%d"), unit.m_FrontState);
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// FRONT STATUS		
							cstr.Format(_T("%d"), unit.m_RearState);
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// REAR STATUS	
							cstr.Format(_T("%d"), unit.m_PP_SplState);
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// SAMPLE LOT STATUS
							cstr = unit.m_strResonCode;
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RESONCODE
							cstr = unit.m_strDescrition;
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// DESCRIPTION
						}
					}
				}
				m_pXCom->SetListItem( lMkMsgID, 2);// RPTID 300 set
				{
					cstr = "300";
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());// RPTID

					m_pXCom->SetListItem( lMkMsgID, 4);
					{
						cstr = strCellID;
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CELLID
						cstr = g_pData->m_cPPID;
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// PPID
						cstr = strProductID;
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// PRODUCTID
						cstr = strStepID;
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// STEPID
					}
				}
				m_pXCom->SetListItem( lMkMsgID, 2);// RPTID 700 set
				{
					cstr = "700";
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());// RPTID
					m_pXCom->SetListItem( lMkMsgID, g_pData->m_nUnitOPCallCount);	// OPCALL 갯수
					{
						for(int i = 0; i < g_pData->m_nUnitOPCallCount; i++)
						{
							m_pXCom->SetListItem( lMkMsgID, 2);
							{
								cstr = g_pData->m_tUnitOPCallMES[i].m_cOPCallID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// OPCALL ID
								cstr = g_pData->m_tUnitOPCallMES[i].m_cOPCallMSG;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// OPCALl MSG

								g_pData->m_tUnitOPCallMES[i].Clear();
							}
						}
					}
				}
			}

			g_pData->m_nUnitOPCallCount = 0;
			GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 CEID 513 UNIT OPCALL Confirm");

			if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
			{				
				nReturn = m_pXCom->Send( lMkMsgID );
				GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 CEID 513 UNIT OPCALL Confirm Send Result( %d )", ceid, nReturn );						
			}

			break;
		}

	}
	return TRUE;
}

BOOL CAutomation::TxS6F11_CEID514(int nJig, int nCh, CString strCellID, CString strProductID, CString strStepID)
{
	int ceid = 514;
	long lMkMsgID = 0;
	CString cstr;
	long nReturn;
	CMainFrame* pMainFrm = (CMainFrame*)::AfxGetMainWnd();
	CUnitStatus *pUnit = &pMainFrm->m_pViewCIM->m_UnitStatusBank.m_Unit[nJig][nCh];
	if(pUnit == NULL)
		return FALSE;

	switch(ceid)
	{
	case 514 :	// UNIT Interlock Confirm
		m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 6, 11, 0);//S6F11 메세지 생성

		m_pXCom->SetListItem( lMkMsgID, 3);//Equipment event info
		{
			cstr = "0";
			m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());//Data ID		
			cstr.Format(_T("%d"), ceid);
			m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());//Collection Event ID
			m_pXCom->SetListItem( lMkMsgID, 4);	// RPTID Set
			{
				m_pXCom->SetListItem( lMkMsgID, 2);// RPTID 100Set
				{
					cstr = "100";
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());// RPTID
					m_pXCom->SetListItem( lMkMsgID, 2);// EQP Control state set
					{
						cstr = g_pData->m_cEQPID;
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());// EQPID
						cstr = g_pData->m_cCRST;
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());// CRST
					}
				}
				m_pXCom->SetListItem( lMkMsgID, 2);// RPTID 102 set
				{
					cstr = "102";
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());// RPTID

					m_pXCom->SetListItem( lMkMsgID, 2);
					{
						cstr = pUnit->m_strUnitID;
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// MODULEID
						m_pXCom->SetListItem( lMkMsgID, 9);
						{
							cstr.Format(_T("%d"), pUnit->m_AvilabilityState);
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// AVAILABILITY
							cstr.Format(_T("%d"), pUnit->m_InterlockState);
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// INTERLOCK
							cstr.Format(_T("%d"), pUnit->m_MoveState);
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// MOVE STATUS
							cstr.Format(_T("%d"), pUnit->m_RunState);
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RUN STATUS
							cstr.Format(_T("%d"), pUnit->m_FrontState);
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// FRONT STATUS		
							cstr.Format(_T("%d"), pUnit->m_RearState);
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// REAR STATUS	
							cstr.Format(_T("%d"), pUnit->m_PP_SplState);
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// SAMPLE LOT STATUS
							cstr = pUnit->m_strResonCode;
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// RESONCODE
							cstr = pUnit->m_strDescrition;
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// DESCRIPTION
						}
					}
				}
				m_pXCom->SetListItem( lMkMsgID, 2);// RPTID 300 set
				{
					cstr = "300";
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());// RPTID

					m_pXCom->SetListItem( lMkMsgID, 4);
					{
						cstr = strCellID;
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// CELLID
						cstr = g_pData->m_cPPID;
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// PPID
						cstr = strProductID;
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// PRODUCTID
						cstr = strStepID;
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// STEPID
					}
				}
				m_pXCom->SetListItem( lMkMsgID, 2);// RPTID 700 set
				{
					cstr = "701";
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());// RPTID
					int nInterlockCount = pUnit->GetInterlockCount();
					m_pXCom->SetListItem( lMkMsgID, nInterlockCount);
					if(nInterlockCount > 0)
					{
						for(int i = 0; i < nInterlockCount; i++)
						{
							m_pXCom->SetListItem( lMkMsgID, 2);
							{
								cstr = pUnit->m_tUnitInterlockMES[i].m_cInterlockID;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// INTERLOCK ID
								cstr = pUnit->m_tUnitInterlockMES[i].m_cInterlockMSG;
								m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	// INTERLOCK MSG
							}
						}
					}
				}
			}
			GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 CEID 514 UNIT Interlock Confirm");

			if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
			{				
				nReturn = m_pXCom->Send( lMkMsgID );
				GetLogCtrl()->AddBuf(LOG_CIM,"S6F11 CEID 514 UNIT Interlock Confirm Send Result( %d )", ceid, nReturn );						
			}

			break;
		}

	}
	return TRUE;
}

//
// 2016-01-03, jhLee, POCH 기능 추가, Cell Lot Information 요청
// 기존 TPR -> WAD -> POCH 로 변경되었다고 함.
//
BOOL CAutomation::TxS6F205( CString strCellID, CString strOptionCode )
{
	long lMkMsgID = 0;
	CString	cstr;
	long nReturn;

	TRY 
	{
		m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 6, 205, 0);
		m_pXCom->SetListItem( lMkMsgID, 3);
		cstr = g_pData->m_cEQPID;
		m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	//EQPID
		// 				str = g_pData->m_cMDLN;
		cstr = strOptionCode;
		m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	//OPTION CODE	
		m_pXCom->SetListItem( lMkMsgID, 1 );

		cstr = strCellID;
		m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	//CELLID

		// GetLogCtrl()->AddBuf(LOG_CIM,"S6F205 Cell Lot Information Request CellID(POCH)( %s )", strCellID);
		if (strcmp(g_pData->m_cCRST, "0")!=0)
		{
			nReturn = m_pXCom->Send( lMkMsgID );
			GetLogCtrl()->AddBuf(LOG_CIM,"S6F205 Cell Lot Information Request Send CellID(POCH)( %s ), Result:%d", strCellID, nReturn );		
		}
		else
			GetLogCtrl()->AddBuf(LOG_CIM,"S6F205 Cell Lot Information Request CellID(POCH)( %s ) - OFF-Line", strCellID);


	}
	CATCH_ALL (e)
	{
		return CB_ERR;
	}
	END_CATCH_ALL

		return TRUE;
}


BOOL CAutomation::TxS6F207(CString	strCellID) // Cell Tracking Information Request
{
	long lMkMsgID = 0;
	CString	cstr;
	long nReturn;

	TRY 
	{
		m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 6, 207, 0);
			m_pXCom->SetListItem( lMkMsgID, 2);
				cstr = g_pData->m_cEQPID;
				m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	//EQPID
				cstr = strCellID;
				m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	//CELLID
			
			GetLogCtrl()->AddBuf(LOG_CIM,"S6F207 Cell Information Request CellID( %s )", strCellID);
			//if(strcmp(g_pData->m_cCRST, "0")!=0)
			if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
			{
				nReturn = m_pXCom->Send( lMkMsgID );
				GetLogCtrl()->AddBuf(LOG_CIM,"S6F207 Cell Information Request Send Result( %d )", nReturn );		
			}

	}
	CATCH_ALL (e)
	{
		return CB_ERR;
	}
	END_CATCH_ALL

	return TRUE;
}

BOOL CAutomation::TxS6F209(CString	strCodeType) // Code Information Request
{
	long lMkMsgID = 0;
	CString	cstr;
	long nReturn;

	TRY 
	{
		m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 6, 209, 0);
		m_pXCom->SetListItem( lMkMsgID, 2);
		cstr = g_pData->m_cEQPID;
		m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	//EQPID
		cstr = strCodeType;
		m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength());	//CODETYPE

		GetLogCtrl()->AddBuf(LOG_CIM,"S6F209 Code Information Request CellID( %s )", strCodeType);
		//if(strcmp(g_pData->m_cCRST, "0")!=0)
		if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
		{
			nReturn = m_pXCom->Send( lMkMsgID );
			GetLogCtrl()->AddBuf(LOG_CIM,"S6F209 Code Information Request Send Result( %d )", nReturn );		
		}

	}
	CATCH_ALL (e)
	{
		return CB_ERR;
	}
	END_CATCH_ALL

		return TRUE;
}

void CAutomation::RxS7Fxx(const int F)
{
	TRY
	{
		switch (F) {

			case 101:
				RxS7F101();
				break;
			case 109:
				RxS7F109();
				break;
			case 23:
				RxS7F23();
				break;

			case 25:
				RxS7F25();
				break;

			default:
				//SendS9(5);
				break;
		}
	}
	CATCH_ALL (e) 
	{
		return;
	} 
	END_CATCH_ALL
}

BOOL CAutomation::RxS7F0() 
{
	return TRUE;
}

BOOL CAutomation::RxS7F101() // Current Eqp PPID List Request
{
	// HOST가 원하는 PPID Type의 List를 요청하기위해 설비로 요청한다
	// ->S7F102

	GetLogCtrl()->AddBuf(LOG_CIM,"Recv S7F101 Current Equipment's PPID List Request");

	long nItems;
	long lCnt;
	CString cstr, strEqpID, strPPID_TYPE;
	signed char cAsciiBuf[255];
	long nReturn;
	BOOL bValid;
	CFileFind finder;
	//BOOL bExist;
	int nCount = 0;
	long lMkMsgID;
	lMkMsgID = 0;


	TRY
	{	
		bValid = FALSE;
		m_pXCom->GetListItem( m_lMsgId, &nItems );
			m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//EQPID
			strEqpID.Format("%s", cAsciiBuf);
			m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//PPID_TYPE
			strPPID_TYPE.Format("%s", cAsciiBuf);
			strEqpID.Trim(' ');
			// EQPID가 안맞을 때 [9/29/2017 OSC]
			if (strcmp(strEqpID, g_pData->m_cEQPID)!=0)
			{
				m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 7, 102, m_lSysByte);

				m_pXCom->SetListItem( lMkMsgID, 3);

					cstr = g_pData->m_cEQPID; 
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// EQPID

					cstr = strPPID_TYPE; 
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	//PPID_TYPE
					m_pXCom->SetListItem( lMkMsgID, 0);

						if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
						{
							nReturn = m_pXCom->Send( lMkMsgID );		// 제대로 갔는가?
							GetLogCtrl()->AddBuf(LOG_CIM,"S7F102 Send Return( %d )", nReturn );								
						}

						GetLogCtrl()->AddBuf(LOG_CIM,"S7F101 EQPID(%s) IS NOT MATCH.", strEqpID);
						return FALSE;
			}
			// PPID TYPE이 1이 아닐 때 [9/29/2017 OSC]
			else if(strcmp(strPPID_TYPE, "1")!=0)
			{
				m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 7, 102, m_lSysByte);

				m_pXCom->SetListItem( lMkMsgID, 3);

					cstr = g_pData->m_cEQPID; 
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// EQPID

					cstr = strPPID_TYPE; 
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	//PPID_TYPE
					m_pXCom->SetListItem( lMkMsgID, 0);

						if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
						{
							nReturn = m_pXCom->Send( lMkMsgID );		// 제대로 갔는가?
							GetLogCtrl()->AddBuf(LOG_CIM,"S7F102 Send Return( %d )", nReturn );								
						}

						GetLogCtrl()->AddBuf(LOG_CIM,"S7F101 PPID_TYPE(%s) IS NOT 1.", strPPID_TYPE);
						return FALSE;
			}
			else 		
				bValid = TRUE;


		if (bValid)
		{
			tag_Recipe_Validation::LoadRcpList();


			//////////////////////////////////////////////////////////////////////////
			//
			//  S7F102 통신 규약 Format 
			//
			//	<L,3
			//		1.<A[40]		$EQPID>				* HOST가 요청한 EQPID
			//		2.<A[1]			$PPID_TYPE>			* HOST가 요청한 PPID
			//		4.<L,n								* n = PPID 갯수
			//			1.<A[40]	$PPID>				* HOST가 요청한 PPID TYPE의 PPID
			//
			//  Strm : 7, Func : 102 , lSysByte : 0
			// 
			//////////////////////////////////////////////////////////////////////////



			m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 7, 102, m_lSysByte);

				m_pXCom->SetListItem( lMkMsgID, 3);

					cstr = g_pData->m_cEQPID; 
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// EQPID

					cstr = strPPID_TYPE; 
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	//PPID_TYPE

					//PPID 갯수
					m_pXCom->SetListItem( lMkMsgID, nCount=tag_Recipe_Validation::m_lstRcp.GetCount());

					for(int i=0; i<nCount; i++)
					{
						// PPID TYPE
						cstr = tag_Recipe_Validation::m_lstRcp.GetAt(tag_Recipe_Validation::m_lstRcp.FindIndex(i)); 
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());

						GetLogCtrl()->AddBuf(LOG_CIM,"S7F101 EQ PPID LIST(%s)", cstr);
					}
			
			GetLogCtrl()->AddBuf(LOG_CIM,"Reply S7F101 CURRENT EQUIPMENT PPID LIST REQUEST PPID COUNT( %d )", nCount);
			
			//if(strcmp(g_pData->m_cCRST, "0")!=0)
			if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
			{
				nReturn = m_pXCom->Send( lMkMsgID );		// 제대로 갔는가?
				GetLogCtrl()->AddBuf(LOG_CIM,"S7F102 Send Return( %d )", nReturn );								
			}



		}
		else
		{
				
		}
	}
	CATCH_ALL (e) 
	{
		return CB_ERR;
	} 
	END_CATCH_ALL

		return TRUE;
}

BOOL CAutomation::TxS7F107() //PPID Creation/Deletion and PP Body Modify Report
{
	tag_Recipe_Data		Param;
	CString strPPID, strPPID_TYPE;
	CFileFind finder;
	CFile file;
	CString str, cstr, strPath;
	long lMkMsgID = 0;
	CMainFrame* pMainFrm = (CMainFrame*)::AfxGetMainWnd();

	TRY
	{
		GetLogCtrl()->AddBuf(LOG_CIM,"S7F107 PPID(%s), MODE(%s)", g_pData->m_cFixPPID, g_pData->m_cFixPPIDMode);

		tag_Recipe_Validation::GetRcpParam(g_pData->m_cFixPPID, &Param, g_pData->m_cFixPPIDMode);

		m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 7, 107, m_lSysByte );
			m_pXCom->SetListItem( lMkMsgID, 5 );
				cstr = g_pData->m_cFixPPIDMode;
				m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength() );	//MODE
				cstr = g_pData->m_cEQPID;
				m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength() );	//EQPID
				cstr = g_pData->m_cFixPPID;
				m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength() );	//PPID
				cstr = "1";
				m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength() );	//PPID_TYPE
				m_pXCom->SetListItem( lMkMsgID, 1 );
					m_pXCom->SetListItem( lMkMsgID, 2 );
						cstr="1";
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength() );				//CCODE
								
						if(strcmp(g_pData->m_cFixPPIDMode, "2")==0)		// 2: DELETE
						{
							m_pXCom->SetListItem( lMkMsgID, 0 );
						}
						else
						{
							m_pXCom->SetListItem( lMkMsgID, Param.nDataCount );						//PARAMETER TOTAL COUNT
							for (int i=0; i<Param.nDataCount; i++)
							{
								m_pXCom->SetListItem( lMkMsgID, 2 );
									cstr = Param.cDataName[i]; cstr = MakeParaString(cstr, TRUE);
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength() );
									cstr = Param.cDataValue[i]; cstr = MakeParaString(cstr, FALSE);
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char *)cstr.GetString(), cstr.GetLength() );
							}
						}
							
		//if(strcmp(g_pData->m_cCRST, "0")!=0)
		if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
		{
			long ret = m_pXCom->Send(lMkMsgID);
			GetLogCtrl()->AddBuf(LOG_CIM,"S7F107 Send Result(%d)", ret);
		}

	}
	CATCH_ALL (e) 
	{
		return CB_ERR;
	} 
	END_CATCH_ALL

	return TRUE;
}


BOOL CAutomation::RxS7F109() // Current Eqp PPID
{
	// HOST가 현재 설비에 사용 중이거나 현재 사용 설정 된 PPID의 정보의 대해서 요청하기 위해 전송한다.
	// >S7F110 


	GetLogCtrl()->AddBuf(LOG_CIM,"Recv S7F109 Host requests to the Equipment to send current PPID");

	long nItems;
	long lCnt;
	CString cstr, strEqpID, strPPID_TYPE;
	signed char cAsciiBuf[255];
	long nReturn;
	BOOL bValid;
	long lMkMsgID;

	lMkMsgID = 0;

	TRY
	{	
		bValid = FALSE;
		m_pXCom->GetListItem( m_lMsgId, &nItems );
		m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//EQPID
		strEqpID.Format("%s", cAsciiBuf);
		m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//PPID_TYPE
		strPPID_TYPE.Format("%s", cAsciiBuf);
		strEqpID.Trim(' ');
		// EQP ID가 맞지 않을 때 [9/29/2017 OSC]
		if (strcmp(strEqpID, g_pData->m_cEQPID)!=0 )
		{
			m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 7, 110, m_lSysByte);

			m_pXCom->SetListItem( lMkMsgID, 2);
		
				cstr = "7"; 
				m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// ACK

				m_pXCom->SetListItem( lMkMsgID, 3);

					cstr = strEqpID; 
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// EQPID

					cstr = g_pData->m_cPPID; 
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// PPID

					cstr = strPPID_TYPE; 
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// PPID_TYPE
			
					if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
					{
						nReturn = m_pXCom->Send( lMkMsgID );		
						GetLogCtrl()->AddBuf(LOG_CIM,"S7F110 Send Return( %d )", nReturn );								
					}
					GetLogCtrl()->AddBuf(LOG_CIM,"S7F109 EQPID(%s) IS NOT MATCH.", strEqpID);
					return FALSE;
		}
		// PPID TYPE이 맞지 않을 때 [9/29/2017 OSC]
		else if (strcmp(strPPID_TYPE, "1")!=0 )
		{
			m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 7, 110, m_lSysByte);

			m_pXCom->SetListItem( lMkMsgID, 2);

				cstr = "8"; 
				m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// ACK

				m_pXCom->SetListItem( lMkMsgID, 3);

					cstr = strEqpID; 
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// EQPID

					cstr = g_pData->m_cPPID; 
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// PPID

					cstr = strPPID_TYPE; 
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// PPID_TYPE

					if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
					{
						nReturn = m_pXCom->Send( lMkMsgID );		
						GetLogCtrl()->AddBuf(LOG_CIM,"S7F110 Send Return( %d )", nReturn );								
					}
					GetLogCtrl()->AddBuf(LOG_CIM,"S7F109 PPID_TYPE(%s) IS NOT 1.", strPPID_TYPE);
					return FALSE;
		}
		else			
			bValid = TRUE;


		//////////////////////////////////////////////////////////////////////////
		//
		//  S7F110 통신 규약 Format 
		//
		//	<L,2
		//		1.<A[1]		$ACKC7>					* HOST가 요청한 EQPID
		//		2.<L,3
		//			1.<A[40]	$EQPID>				* 설비에 현재 설정 된 PPID
		//			2.<A[40]	$PPID>				* 설비에 현재 설정 된 PPID
		//			3.<A[40]	$PPID_TYPE>			* 설비에 현재 설정 된 PPID의 TYPE
		//
		//  Strm : 7, Func : 110 , lSysByte : 0
		// 
		//////////////////////////////////////////////////////////////////////////


		if (bValid)
		{
			m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 7, 110, m_lSysByte);

				m_pXCom->SetListItem( lMkMsgID, 2);

				if (strcmp(strPPID_TYPE,"1")!=0 )
				{
					cstr = "8"; 
				}
				else
					cstr = "0"; 
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// ACK

					m_pXCom->SetListItem( lMkMsgID, 3);

						cstr = strEqpID; 
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// EQPID

						cstr = g_pData->m_cPPID; 
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// PPID

						cstr = strPPID_TYPE; 
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// PPID_TYPE

					
			GetLogCtrl()->AddBuf(LOG_CIM,"Reply S7F109 CURRENT PPID( %s )", g_pData->m_cPPID);
			
			//if(strcmp(g_pData->m_cCRST, "0")!=0)
			if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
			{
				nReturn = m_pXCom->Send( lMkMsgID );		
				GetLogCtrl()->AddBuf(LOG_CIM,"S7F110 Send Return( %d )", nReturn );								
			}

		}
		else
		{
			m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 7, 110, m_lSysByte);

				m_pXCom->SetListItem( lMkMsgID, 2);

					cstr = "1";	//Module-ID mismatch

					m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// ACK

					m_pXCom->SetListItem( lMkMsgID, 3);

						cstr = strEqpID; 
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// EQPID

						cstr = ""; 
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// PPID

						cstr = strPPID_TYPE; 
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	// PPID_TYPE

					
			GetLogCtrl()->AddBuf(LOG_CIM,"Reply S7F109 CURRENT PPID, Module-ID mismatch ModuleId(%s)", strEqpID);
			
			//if(strcmp(g_pData->m_cCRST, "0")!=0)
			if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
			{
				nReturn = m_pXCom->Send( lMkMsgID );		
				GetLogCtrl()->AddBuf(LOG_CIM,"S7F110 Send Return( %d )", nReturn );								
			}
		}

		
	}
	CATCH_ALL (e) 
	{
		return CB_ERR;
	} 
	END_CATCH_ALL

		return TRUE;
}
BOOL CAutomation::RxS7F23() // Formatted PPID Change
{
	GetLogCtrl()->AddBuf(LOG_CIM,"Recv S7F23 Host PPID Change");
	
	long    lCnt;
	long    CCODE = 0;
	long		nItems;
	CString strPPID, strEqpID, strPPID_TYPE;

	CFileFind finder;
	CFile file;
	CString str, cstr, strPath;
	long lMkMsgID = 0;

	CString strAck=_T("0");
//	CString	*strName;
//	CString	*strVal;
	int a, b;
	CString strBaseName;
	BOOL bCreateBase = FALSE;
	BOOL bCreateRcp = FALSE;
	int nRcpErrorFlag=-1;
	BOOL bModifyRcp=FALSE;
	
	CString	strName[tag_Recipe_Data::MAX_DATA_CNT];
	CString	strVal[tag_Recipe_Data::MAX_DATA_CNT];

	CString strParaNameChk[tag_Recipe_Data::MAX_DATA_CNT];
	CString strSaveNameChk[tag_Recipe_Data::MAX_DATA_CNT];
	BOOL bParaNameChk[tag_Recipe_Data::MAX_DATA_CNT];
	double dParaVal[tag_Recipe_Data::MAX_DATA_CNT];

	signed char cAsciiBuf[255];

	tag_Recipe_Data recipeData;
	CMainFrame* pMainFrm = (CMainFrame*)::AfxGetMainWnd();

	// 너무 많으니 밑에 가서 실제 있는 만큼만 초기화한다 [9/27/2017 OSC]
// 	for (int i=0; i<tag_Recipe_Data::MAX_DATA_CNT; i++)
// 	{
// 		strParaNameChk[i].Empty();
// 		bParaNameChk[i]=FALSE;
// 		dParaVal[i] = 0.0;
// 	}
	
	TRY
	{
		strBaseName = _T("");

		m_pXCom->GetListItem( m_lMsgId, &nItems );
			m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));
			strEqpID.Format("%s", cAsciiBuf);
			m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));
			strPPID.Format("%s", cAsciiBuf);
			m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));
			strPPID_TYPE.Format("%s", cAsciiBuf);
			strEqpID.Trim(' ');
			strPPID.Trim(' ');
			m_pXCom->GetListItem( m_lMsgId, &nItems );				//Command SetList a = Number of process commands
			a = nItems;
			if (a==1)
			{
				for (int i=0; i<a; i++)
				{
				m_pXCom->GetListItem( m_lMsgId, &nItems );			//CCODE Set
					m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//CCODE
					m_pXCom->GetListItem( m_lMsgId, &nItems );		//b = Number of parameters
					b=nItems;
				//	strName = new CString[b];
				//	strVal = new CString[b];
					for (int j=0; j<b; j++)
					{
						m_pXCom->GetListItem( m_lMsgId, &nItems );	//Parameter Set List
							m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//Name
							str.Format("%s", cAsciiBuf); strName[j]=str; strName[j].Trim(' ');
							m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//Val
							str.Format("%s", cAsciiBuf); strVal[j] = str; strVal[j].Trim(' ');

							if (strName[j].Compare(_T("BaseName"))==0 || strName[j].Compare(_T("BASENAME"))==0)
							{
								strBaseName = strVal[j];
							}
					}
				}
			}
			else
			{
			//	strName = new CString[1];
			//	strVal = new CString[1];
			}

			if (1==1)
			{				
				// 일단 현재 모델값을 불러온 후... [9/27/2017 OSC]
				tag_Recipe_Validation::GetRcpParam(g_pData->m_cPPID, &recipeData, g_pData->m_cFixPPIDMode);
				// CString 변수로 옮겨 놓는다 [9/27/2017 OSC]
				for (int i=0; i<recipeData.nDataCount; i++)
				{
					strParaNameChk[i].Format(_T("%s"), recipeData.cDataName[i]);
					bParaNameChk[i]=FALSE;
					dParaVal[i] = 0.0;
				}


				if (strcmp(strEqpID, g_pData->m_cEQPID)!=0)
				{
					strAck = _T("7");			
					GetLogCtrl()->AddBuf(LOG_CIM,"Reply S7F23 CURRENT PPID, Module-ID mismatch ModuleId(%s)", strEqpID);
				}
				else if (strPPID_TYPE.Compare(_T("1"))!=0)
				{
					strAck = _T("8");				
					GetLogCtrl()->AddBuf(LOG_CIM,"Reply S7F23 CURRENT PPID, PPID_TYPE(%s) error", strPPID_TYPE);
				}
				else if (a!=1)
				{
					strAck = _T("9");				
					GetLogCtrl()->AddBuf(LOG_CIM,"Reply S7F23 CURRENT PPID, SetList(%d) error", a);
				}
				else if (/*strBaseName.GetLength()<=0 ||*/ strPPID.GetLength()<=0)
				{
					strAck = _T("2");				
					GetLogCtrl()->AddBuf(LOG_CIM,"Reply S7F23 CURRENT PPID, Can,t find BaseName or PPID the Msg(%s)", strBaseName);
				}
 				else if (b!=recipeData.nDataCount)
 				{
 					strAck = _T("2");				
 					GetLogCtrl()->AddBuf(LOG_CIM,"Reply S7F23 CURRENT PPID, Para count(%d) error", b);
 				}
				else if (strcmp(g_pData->m_cPPID, strPPID)==0)
				{
					strAck = _T("1");			
					GetLogCtrl()->AddBuf(LOG_CIM,"Reply S7F23 CURRENT PPID, Cur PPID(%s) error", strPPID);
				}

				strPath.Format("%s\\%s", CSettingParm::cMODEL_PATH, strBaseName);
				if (strAck.Compare(_T("0"))==0 && CFileSupport::DirectoryCheck(strPath) == FALSE)
				{
					bCreateBase = TRUE;
				//	strAck = _T("3");			//Base Model 생성 금지.			
					GetLogCtrl()->AddBuf(LOG_CIM,"Reply S7F23 CURRENT PPID, Can,t find BaseName in Eq(%s)", strBaseName);
				}
			
				strPath.Format("%s\\%s\\%s", CSettingParm::cMODEL_PATH, strPPID,strPPID);
				if (strAck.Compare(_T("0"))==0 && CFileSupport::DirectoryCheck(strPath) == FALSE)
				{
					bCreateRcp = TRUE;
				}

				for (int i=0; i<recipeData.nDataCount; i++)
				{
					for (int j=0; j<b; j++)
					{
						strParaNameChk[i].Trim(' ');
						strName[j].Trim(' ');
						strParaNameChk[i].MakeUpper();
						if (strParaNameChk[i].Compare(strName[j])==0)
						{
							bParaNameChk[i] = TRUE;
							dParaVal[i] = atof(strVal[j]);
							break;
						}
					}
					if (bParaNameChk[i]==FALSE)
					{
						nRcpErrorFlag = i;
						break;
					}
				}

				if (nRcpErrorFlag>=0)
				{
					strAck = _T("3");			
					GetLogCtrl()->AddBuf(LOG_CIM,"Reply S7F23 CURRENT PPID, Can,t find ParaName in Eq(%s)", strParaNameChk[nRcpErrorFlag]);
				}										
				
				if (strAck.Compare(_T("0"))==0)
				{		
// 					if (bCreateBase)
// 					{
// 						CTime time = CTime::GetCurrentTime();
// 						CString strPath, strBackupPath;
// 
// 						strPath.Format(_T("%s\\%s\\RMSData.ini"), CSettingParm::cMODEL_PATH, strBaseName);
// 						strBackupPath.Format(_T("%s\\%d_%d\\%s\\RMSData_%02d_%s.ini"), CSettingParm::cMODEL_PATH, 
// 							time.GetYear(), time.GetMonth(), strBaseName, time.GetDay(), time.Format(_T("%H%M%S")));				
// 			
// 						tag_Recipe_Validation::SaveRcp(strPath, strBackupPath,strParaNameChk,dParaVal);				
// 			
// 				//		TxS7F107(CString(theProcDataMgr.m_pProcData->m_CimStatus.m_NewPPID), 1);
// 					}
					if (bCreateRcp == FALSE)
					{
						strAck = _T("9");					
						GetLogCtrl()->AddBuf(LOG_CIM,"Reply S7F23 CURRENT PPID, Cim program error");
					}
				}

				m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 7, 24, m_lSysByte );
					str = strAck; 
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), _tcslen(str));	//Ack

				if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
				{
					long ret = m_pXCom->Send(lMkMsgID);				
					GetLogCtrl()->AddBuf(LOG_CIM,"S7F24 Send Fail Return( %d )", ret );	
				}

				if ( bCreateRcp && (strAck == _T("0")) )
				{
					CTime time = CTime::GetCurrentTime();
					CString strPath, strBackupPath;
					strPath.Format(_T("%s\\%s\\RMSData.ini"), CSettingParm::cMODEL_PATH, strPPID);
					strBackupPath.Format(_T("%s\\%d_%d\\%s\\RMSData_%02d_%s.ini"), CSettingParm::cMODEL_PATH, 
						time.GetYear(), time.GetMonth(), strPPID, time.GetDay(), time.Format(_T("%H%M%S")));				

					tag_Recipe_Validation::SaveRcp(strPath, strBackupPath,strParaNameChk,dParaVal, recipeData.nDataCount);	
					pMainFrm->m_pViewCIM->SetPPIDCreate(strPPID);


					strcpy_s(g_pData->m_cFixPPID, strPPID);
					strcpy_s(g_pData->m_cFixPPIDMode, PPID_MODE_CREATE);
					TxS7F107();

					((CMainFrame *)AfxGetMainWnd())->m_pViewRMS->GetModel();

				//		theProcDataMgr.SendHostEvent(CONST_HOST_EVENT::SET_CREATE_HOST_RECIPE);
				}				

			//	delete []strName;
			//	delete []strVal;
				return FALSE;		//여기서 리턴.
			}			
	
			GetLogCtrl()->AddBuf(LOG_CIM,"Reply S7F23 CURRENT PPID, PPID(%s)",strPPID);

			m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 7, 24, m_lSysByte );
				str = strAck; 
				m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), _tcslen(str));	//Ack

			if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
			{
				long ret = m_pXCom->Send(lMkMsgID);		
				GetLogCtrl()->AddBuf(LOG_CIM,"S7F24 Send Return( %d )", ret );			
			}	



		//	delete []strName;
		//	delete []strVal;
	}
	CATCH_ALL (e) 
	{
		return CB_ERR;
	} 
	END_CATCH_ALL

	return TRUE;
}
BOOL CAutomation::RxS7F25() 
{
	// 설비의 PPID의 Parameter 현재 값을 요청하기 위해 설비로 전송한다.
	// >S7F26

	GetLogCtrl()->AddBuf(LOG_CIM,"Recv S7F25 Formatted Process Program Request");

	tag_Recipe_Data		Param;

	long    lCnt;
	long    CCODE = 0;
	long		nItems;
	CString strPPID, strEqpID, strPPID_TYPE;

	CFileFind finder;
	CFile file;
	CString str, cstr, strPath;
	char cMode[2];
	strcpy_s(cMode, PPID_MODE_CREATE);
	long lMkMsgID = 0;
	signed char cAsciiBuf[255];



	CMainFrame* pMainFrm = (CMainFrame*)::AfxGetMainWnd();

	TRY
	{
		m_pXCom->GetListItem( m_lMsgId, &nItems );
			m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));
			strEqpID.Format("%s", cAsciiBuf);



			m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));
			strPPID.Format("%s", cAsciiBuf);
			m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));
			strPPID_TYPE.Format("%s", cAsciiBuf);


		strEqpID.Trim(' ');
		strPPID.Trim(' ');

		// EQP ID가 맞지 않을 때 [9/29/2017 OSC]
		if (strcmp(g_pData->m_cEQPID, strEqpID)!=0)
		{
			GetLogCtrl()->AddBuf(LOG_CIM,"S7F25 CURRENT PPID(%s) EQP ID NOT MATCH.", strPPID);


			m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 7, 26, m_lSysByte );

			m_pXCom->SetListItem( lMkMsgID, 7 );

			str = g_pData->m_cEQPID; 
			m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	//EQPID

			str = strPPID; 
			m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	//PPID

			str = strPPID_TYPE; 
			m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	//PPID_TYPE

			str = g_pData->m_cMDLN; 
			m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());		//MDLN

			str = g_pData->m_cSoftRev; 
			m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());		//SOFTREV	

			CTime t = CTime::GetCurrentTime();
			sprintf_s(g_pData->m_cLCTime, 15, "%04d%02d%02d%02d%02d%02d", t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute(),t.GetSecond());
			str = g_pData->m_cLCTime; 
			

			m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());		//CIMTIME
			m_pXCom->SetListItem( lMkMsgID, 0 );

			//if(strcmp(g_pData->m_cCRST, "0")!=0)
			if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
			{
				long ret = m_pXCom->Send(lMkMsgID);
				GetLogCtrl()->AddBuf(LOG_CIM,"S7F26 FAIL Send Result=%d", ret);
			}

			GetLogCtrl()->AddBuf(LOG_CIM,"S7F25 EQPID(%s) IS NOT MATCH.", strEqpID);
			return FALSE;
		}
		// PPID TYPE이 맞지 않을 때 [9/29/2017 OSC]
		else if (strcmp(strPPID_TYPE, "1")!=0)
		{
			GetLogCtrl()->AddBuf(LOG_CIM,"S7F25 CURRENT PPID(%s) PPID_TYPE NOT 1.", strPPID);


			m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 7, 26, m_lSysByte );

			m_pXCom->SetListItem( lMkMsgID, 7 );

				str = g_pData->m_cEQPID; 
				m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	//EQPID

				str = strPPID; 
				m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	//PPID

				str = strPPID_TYPE; 
				m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	//PPID_TYPE

				str = g_pData->m_cMDLN; 
				m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());		//MDLN

				str = g_pData->m_cSoftRev; 
				m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());		//SOFTREV	

				CTime t = CTime::GetCurrentTime();
				sprintf_s(g_pData->m_cLCTime, 15, "%04d%02d%02d%02d%02d%02d", t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute(),t.GetSecond());
				str = g_pData->m_cLCTime; 
				

				m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());		//CIMTIME
				m_pXCom->SetListItem( lMkMsgID, 0 );

					//if(strcmp(g_pData->m_cCRST, "0")!=0)
					if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
					{
						long ret = m_pXCom->Send(lMkMsgID);
						GetLogCtrl()->AddBuf(LOG_CIM,"S7F26 FAIL Send Result=%d", ret);
					}

					GetLogCtrl()->AddBuf(LOG_CIM,"S7F25 PPID_TYPE(%s) IS NOT 1.", strPPID_TYPE);
					return FALSE;
		}

		strPath.Format("%s\\%s", CSettingParm::cMODEL_PATH, strPPID);


		//////////////////////////////////////////////////////////////////////////
		//
		//  S7F26 통신 규약 Format 
		//
		//	<L,7		
		//		1.<A[40]				$EQPID>			*HOST가 요청한 EQPID
		//		2.<A[40]				$PPID>			*HOST가 요청한 PPID		
		//		3.<A[1]					$PPID_TYPE>		*HOST가 요청한 PPID TYPE
		//		4.<A[20]				$MDLN>			*CIM의 Software 버젼
		//		5.<A[6]					$SOFTREV>		*설비의 SOftware 버젼
		//		6.<A[14]				$CIMTIME>		*CIM의 현재시간
		//		7.<L,1									*PPID Set
		//			1.<L,2								*CCOD Set
		//				1.<A[3]			$CODE>			*
		//				2.<L,n							*Parameter갯수
		//					1.<L,2						*Parameter Set
		//						1.<A[40] $PPARMNAME>	*Parameter 이름
		//						2.<A[40] $PPARMVALUE>	*Parameter 값
		//
		//  Strm : 7, Func : 26 , lSysByte : 0
		// 
		//////////////////////////////////////////////////////////////////////////

		// Recipe는 폴더명으로 존재 유무를 확인한다 [9/27/2017 OSC]
		if ( CFileSupport::DirectoryCheck(strPath) == FALSE)
// 		if ( !finder.FindFile(strPath))
		{
			GetLogCtrl()->AddBuf(LOG_CIM,"S7F25 CURRENT PPID(%s) DATA FILE READ FAIL.", strPPID);


			m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 7, 26, m_lSysByte );

			m_pXCom->SetListItem( lMkMsgID, 7 );

				str = g_pData->m_cEQPID; 
				m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	//EQPID

				str = strPPID; 
				m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	//PPID

				str = strPPID_TYPE; 
				m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	//PPID_TYPE

				str = g_pData->m_cMDLN; 
				m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());		//MDLN

				str = g_pData->m_cSoftRev; 
				m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());		//SOFTREV	

 				CTime t = CTime::GetCurrentTime();
 				sprintf_s(g_pData->m_cLCTime, 15, "%04d%02d%02d%02d%02d%02d", t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute(),t.GetSecond());
 				str = g_pData->m_cLCTime; 
				
				
				m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());		//CIMTIME
				m_pXCom->SetListItem( lMkMsgID, 0 );

			//if(strcmp(g_pData->m_cCRST, "0")!=0)
			if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
			{
				long ret = m_pXCom->Send(lMkMsgID);
				GetLogCtrl()->AddBuf(LOG_CIM,"S7F26 FAIL Send Result=%d", ret);
			}

			return FALSE;
		}

		tag_Recipe_Validation::GetRcpParam((LPSTR)(LPCTSTR)strPPID, &Param, cMode);



		
 		GetLogCtrl()->AddBuf(LOG_CIM,"S7F25 PPID(%s).", strPPID);

		//S7F26 전송
		m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 7, 26, m_lSysByte );

			m_pXCom->SetListItem( lMkMsgID, 7 );

				cstr = g_pData->m_cEQPID;
				m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	//EQPID

				cstr = strPPID;
				m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	//PPID

				cstr = strPPID_TYPE;
				m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	//PPID_TYPE

				cstr = g_pData->m_cMDLN;
				m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	//MDLN

				cstr = g_pData->m_cSoftRev;
				m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	//SOFTREV

 				CTime t = CTime::GetCurrentTime();
 				sprintf_s(g_pData->m_cLCTime, 15, "%04d%02d%02d%02d%02d%02d", t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute(),t.GetSecond());
 				cstr = g_pData->m_cLCTime;
				//cstr = " "; 
				m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());		//CIMTIME

				m_pXCom->SetListItem( lMkMsgID, 1 );

					m_pXCom->SetListItem( lMkMsgID, 2 );

						cstr="1";
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());				//CCODE		


						m_pXCom->SetListItem( lMkMsgID, Param.nDataCount );						//PARAMETER TOTAL COUNT
						for (int i=0; i<Param.nDataCount; i++)
						{
							m_pXCom->SetListItem( lMkMsgID, 2 );

							//Parameter 이름
							cstr = Param.cDataName[i]; cstr = MakeParaString(cstr, TRUE);
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());

							//Paramer 값
							cstr = Param.cDataValue[i]; cstr = MakeParaString(cstr, FALSE);
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());
						}
		
		//if(strcmp(g_pData->m_cCRST, "0")!=0)
		if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
		{
			long ret = m_pXCom->Send(lMkMsgID);
			GetLogCtrl()->AddBuf(LOG_CIM,"S7F26 Send Result(%d)", ret);
		}

		

	}
	CATCH_ALL (e) 
	{
		return CB_ERR;
	} 
	END_CATCH_ALL

	return TRUE;
}

void CAutomation::RxS8Fxx(const int F)
{
	TRY
	{
		switch (F) {

			case 1:
				RxS8F1();
				break;
			case 3:
				RxS8F3();
				break;

			default:
				//SendS9(5);
				break;
		}
	}
	CATCH_ALL (e) 
	{
		return;
	} 
	END_CATCH_ALL
}

BOOL CAutomation::RxS8F0() 
{
	return TRUE;
}

BOOL CAutomation::RxS8F1()
{
	GetLogCtrl()->AddBuf(LOG_CIM,"Recv S8F1 Inquiry for specially designed multi-use data");

	long nItems;
	long lCnt;
	CString strEqpID, strItemType, strItemName, strItemValue, strReference, strFileName;
	signed char cAsciiBuf[255];
	long nReturn;
	int	  n;
	long lMkMsgID;
	lMkMsgID = 0;
	CString   strACK;
	CString		cstr, strBuff;
	CString		*strType;
	CString		*strName;
	CString		*strValue;
	CString		*strRef;
	CString		*strEAC;
	BOOL		*bNameValid;
	BOOL		bFileOpen, bEqpIDValid, bNameValidCheck;
	BOOL		bNameReValid;

	char cLine[1024];
	FILE	*fp;
	errno_t		err = 0;
	char* p0 = &cLine[0];
	char* pS, *pE;
	int nCount=0;
	CString buff, buff2;
	int nTypeCnt=0;


	enum
	{
		NAME_MAX = 100,
	};

	int	 nNameCnt=0;
	struct tag_MultiUseData 
	{
		char cType[41];
		char cName[41];
		char cValue[101];
		char cRef[21];
		
		tag_MultiUseData(void)
		{
			memset(cType, 0, 41);
			memset(cName, 0, 41);
			memset(cValue, 0, 101);
			memset(cRef, 0, 21);
		};

		void Clear(void)
		{
			memset(cType, 0, 41);
			memset(cName, 0, 41);
			memset(cValue, 0, 101);
			memset(cRef, 0, 21);
		};

		tag_MultiUseData& operator = (tag_MultiUseData &rb)
		{
			if (this == &rb)
				return *this;

			memcpy(cType, rb.cType, 41);
			memcpy(cName, rb.cName, 41);
			memcpy(cValue, rb.cValue, 101);
			memcpy(cRef, rb.cRef, 21);

			return *this;
		};

	};

	tag_MultiUseData tMultiUseData[NAME_MAX];

	bEqpIDValid = TRUE;
	bFileOpen = TRUE;
	bNameValidCheck = TRUE;

	TRY
	{	
		strFileName.Format("%s\\Setup.ini",CGaonNuriApp::GetSetupPath());
		CIni ini(strFileName);

		m_pXCom->GetListItem( m_lMsgId, &nItems );
			m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//EQPID
			strEqpID.Format("%s", cAsciiBuf);
			strEqpID.Trim(' ');
			if (strcmp(strEqpID, g_pData->m_cEQPID)!=0)
			{
				GetLogCtrl()->AddBuf(LOG_CIM,"S8F1 EQPID(%s) IS NOT MATCH.", strEqpID);
				bEqpIDValid = FALSE;
 			}

			if(err = fopen_s(&fp, strFileName, "r"))
			{	
				GetLogCtrl()->AddBuf(LOG_CIM,"S8F1 File Open Error(%s)", strFileName);
				bFileOpen = FALSE;
			}
			else
			{
				while(fgets(p0, 1024,fp)!=NULL)
				{
					pS = p0;
					if((pE=strchr(pS, '='))==NULL)
					{
						CTokenizer token = pS;
						buff = token.GetNextToken("[");
						buff = token.GetNextToken("]");
						continue;
					}
					*(pE++) = 0x00;

					pS = pE;
					if((pE=strchr(pS, '\n'))!=NULL)			*(pE++) = 0x00;
					else if((pE=strchr(pS, 0x0a))!=NULL)	*(pE++) = 0x00;
					else if((pE=strchr(pS, 0x0d))!=NULL)	*(pE++) = 0x00;
					else if((pE=strchr(pS, 0x00))!=NULL)	*(pE++) = 0x00;
					else
					{
						nCount = 0;
						break;
					}

					if((buff.Compare("MCC") == 0) || (buff.Compare("DES") == 0) )
					{
						cstr.Format("%s",buff);
						cstr.Format("%s",p0);
						cstr.Format("%s",pS);
						strcpy_s(tMultiUseData[nNameCnt].cType, buff);
						strcpy_s(tMultiUseData[nNameCnt].cName, p0);
						CTokenizer token = pS;
						buff2 = token.GetNextToken(",");
						strcpy_s(tMultiUseData[nNameCnt].cValue, buff2);
						buff2 = token.GetNextToken(",");
						strcpy_s(tMultiUseData[nNameCnt].cRef, buff2);
						nNameCnt++;
					}
				}

				fclose(fp);
			}

			m_pXCom->GetListItem( m_lMsgId, &nItems );
			n = nItems;

			strType = new CString[n];
			strName = new CString[n];
			strValue = new CString[n];
			strRef = new CString[n];
			strEAC = new CString[n];
			bNameValid = new BOOL[n];
				for (int i=0; i<n; i++)
				{
					bNameReValid = FALSE;
					m_pXCom->GetListItem( m_lMsgId, &nItems );
						m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//ITEM_TYPE
						strType[i].Format("%s", cAsciiBuf);
						if(((strType[i].Compare("MCC") == 0) || (strType[i].Compare("DES") == 0)) && bFileOpen)
						{
							strEAC[i] = "0";
							bNameValid[i] = TRUE;
						}
						else
						{
							strEAC[i] = "1";
							bNameValid[i] = FALSE;
						}
						m_pXCom->GetListItem( m_lMsgId, &nItems );
							m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//ITEM_NAME
							strName[i].Format("%s", cAsciiBuf);
							m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//ITEM_VALUE
							strValue[i].Format("%s", cAsciiBuf);
							m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//REFERENCE
							strRef[i].Format("%s", cAsciiBuf);
							GetLogCtrl()->AddBuf(LOG_CIM,"S8F1 - NO(%d) TYPE(%s) NAME(%s) VALUE(%s) REFERENCE(%s)", i+1, strType[i], strName[i],  strValue[i], strRef[i]);
							
							if(bNameValid[i])
							{
								for (int j=0; j<nNameCnt; j++)
								{
									if (strcmp(tMultiUseData[j].cName, strName[i])==0)
									{
										bNameReValid = TRUE;
										break;
									}
								}

								if (!bNameReValid)
								{
									strEAC[i] = "1";
									bNameValid[i] = FALSE;
									GetLogCtrl()->AddBuf(LOG_CIM,"S8F1 - Name not Match, NAME(%s)", strName[i]);
								}
							}
							
				}
	
		for (int i=0; i<n; i++)
		{
			if (!bNameValid[i])
			{
				bNameValidCheck = FALSE;
				break;
			}
		}

		if(bEqpIDValid && bNameValidCheck)
		{
			strACK = "0";
			GetLogCtrl()->AddBuf(LOG_CIM,"S8F1 - EqpID Pass, Type Define Pass");
		}
		else if(!bEqpIDValid && bNameValidCheck)
		{
			strACK = "1";
			GetLogCtrl()->AddBuf(LOG_CIM,"S8F1 - EqpID Fail, Type Define Pass");
		}
		else if(bEqpIDValid && !bNameValidCheck)
		{
			strACK = "2";
			GetLogCtrl()->AddBuf(LOG_CIM,"S8F1 - EqpID Pass, Type Define Fail");
		}
		else if(!bEqpIDValid && !bNameValidCheck)
		{
			strACK = "1";
			GetLogCtrl()->AddBuf(LOG_CIM,"S8F1 - EqpID Fail, Type Define Fail");
		}
		
		m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 8, 2, m_lSysByte);
			m_pXCom->SetListItem( lMkMsgID, 2 );
				cstr = strACK; 
				m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());
				m_pXCom->SetListItem( lMkMsgID, n );
				for (int i=0; i<n; i++)
				{
					m_pXCom->SetListItem( lMkMsgID, 3 );
						cstr = strType[i]; 
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());
						m_pXCom->SetListItem( lMkMsgID, 3 );
							cstr = strName[i]; 
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());
							cstr = strValue[i]; 
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());
							cstr = strRef[i]; 
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());
						cstr = strEAC[i]; 
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());

						if(bEqpIDValid && bNameValidCheck)
						{
							strBuff.Format("%s,%s", cstr = strValue[i], strRef[i]);
							ini.WriteString(strType[i], strName[i], strBuff);
							GetLogCtrl()->AddBuf(LOG_CIM,"S8F1 - WriteString TYPE(%s) NAME(%s) VALUE(%s) REFERENCE(%s)", strType[i], strName[i],  strValue[i], strRef[i]);
						}
						
				}			

		//if(strcmp(g_pData->m_cCRST, "0")!=0)
		if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
		{
			nReturn = m_pXCom->Send( lMkMsgID );		
			GetLogCtrl()->AddBuf(LOG_CIM,"S8F2 Send Return( %d )", nReturn );								
		}

		

		delete[] strType;
		delete[] strName;
		delete[] strValue;
		delete[] strRef;
		delete[] strEAC;
		
		if(bEqpIDValid && bNameValidCheck)
			((CMainFrame *)AfxGetMainWnd())->m_pViewSETTING->GetSetup();

	}
	CATCH_ALL (e) 
	{
		return CB_ERR;
	} 
	END_CATCH_ALL

		return TRUE;
}

BOOL CAutomation::RxS8F3()
{
	// HOST는 설비의 FILE SERVER UPLOAD하기 위해 설정 정보를 조회하기 위하여 전송한다
	// ->S8F4


	GetLogCtrl()->AddBuf(LOG_CIM,"Recv S8F3 Inquiry for Multi-use Set Data");

	long nItems;
	long lCnt;
	signed char cAsciiBuf[255];
	long nReturn;
	int	  n, n2, n3, n4;
	long lMkMsgID;
	lMkMsgID = 0;
	CString	strFileName, strEqpID;

	char cLine[1024];
	FILE	*fp;
	errno_t		err = 0;
	char* p0 = &cLine[0];
	char* pS, *pE;
	int nCount=0;
	CString buff, cstr, buff2;
	CString		strName;
	CString		strItemType;
	
	CString strItemName, strItemValue, strReference;


	enum
	{
		NAME_MAX = 100,
	};

	int	 nNameCnt=0;
	struct tag_MultiUseData 
	{
		char cType[41];
		char cName[41];
		char cValue[101];
		char cRef[21];

		tag_MultiUseData(void)
		{
			memset(cType, 0, 41);
			memset(cName, 0, 41);
			memset(cValue, 0, 101);
			memset(cRef, 0, 21);
		};

		void Clear(void)
		{
			memset(cType, 0, 41);
			memset(cName, 0, 41);
			memset(cValue, 0, 101);
			memset(cRef, 0, 21);
		};

		tag_MultiUseData& operator = (tag_MultiUseData &rb)
		{
			if (this == &rb)
				return *this;

			memcpy(cType, rb.cType, 41);
			memcpy(cName, rb.cName, 41);
			memcpy(cValue, rb.cValue, 101);
			memcpy(cRef, rb.cRef, 21);

			return *this;
		};

	};

	tag_MultiUseData tMultiUseData[NAME_MAX];

	int	 nRepNameCnt=0;
	tag_MultiUseData tMultiUseRepData[NAME_MAX];

 	BOOL		bValid1, bValid2;
 
 	bValid1 = TRUE;
 	bValid2 = TRUE;

	n = n2 = n3 = n4 = 0;


	//////////////////////////////////////////////////////////////////////////
	//
	//  S8F4 통신 규약 Format 
	//
	//	<L,2
	//		1.A[2]					$ACKC8>				* Acknowledge Code Stream 8
	//		2.<L,n
	//			1.<L,3
	//				1.<A[10]		$DATA_TYPE>			* DATA의 유형,MCC,SEM,DES,ETC
	//				2.<L,3
	//					1.<A[40]	$ITEM_NAME>			* 정의한 Item Name
	//					2.<A[100]	$ITEM_VAULE>		* Item에 대한 값
	//					3.<A[20]	$REFERENCE>			* Equipment Acknowledge Code
	//				3.<A[1]
	//
	//  Strm : 7, Func : 26 , lSysByte : 0
	// 
	//////////////////////////////////////////////////////////////////////////



	TRY
	{	

		m_pXCom->GetListItem( m_lMsgId, &nItems );
			m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//EQPID
			strEqpID.Format("%s", cAsciiBuf);
			strEqpID.Trim(' ');
			if (strcmp(strEqpID, g_pData->m_cEQPID)!=0)
			{
				GetLogCtrl()->AddBuf(LOG_CIM,"S8F3 EQPID(%s) IS NOT MATCH.", strEqpID);


				m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 8, 4, m_lSysByte);

					m_pXCom->SetListItem( lMkMsgID, 2 );

					cstr = "1"; 
					m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());

					m_pXCom->SetListItem( lMkMsgID, 0 );
			}
			else
			{
				strFileName.Format("%s\\Setup.ini",CGaonNuriApp::GetSetupPath());

				if(err = fopen_s(&fp, strFileName, "r"))
				{	
					GetLogCtrl()->AddBuf(LOG_CIM,"S8F3 File Open Error(%s)", strFileName);


					m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 8, 4, m_lSysByte);

						m_pXCom->SetListItem( lMkMsgID, 2 );

						cstr = "2"; 
						m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());
				}
				else
				{
					while(fgets(p0, 1024,fp)!=NULL)
					{
						pS = p0;
						if((pE=strchr(pS, '='))==NULL)
						{
							CTokenizer token = pS;
							buff = token.GetNextToken("[");
							buff = token.GetNextToken("]");
							continue;
						}
						*(pE++) = 0x00;

						pS = pE;
						if((pE=strchr(pS, '\n'))!=NULL)			*(pE++) = 0x00;
						else if((pE=strchr(pS, 0x0a))!=NULL)	*(pE++) = 0x00;
						else if((pE=strchr(pS, 0x0d))!=NULL)	*(pE++) = 0x00;
						else if((pE=strchr(pS, 0x00))!=NULL)	*(pE++) = 0x00;
						else
						{
							nCount = 0;
							break;
						}

						if((buff.Compare("MCC") == 0) || (buff.Compare("DES") == 0) )
						{
							cstr.Format("%s",buff);
							cstr.Format("%s",p0);
							cstr.Format("%s",pS);
							strcpy_s(tMultiUseData[nNameCnt].cType, buff);
							strcpy_s(tMultiUseData[nNameCnt].cName, p0);
							CTokenizer token = pS;
							buff2 = token.GetNextToken(",");
							strcpy_s(tMultiUseData[nNameCnt].cValue, buff2);
							buff2 = token.GetNextToken(",");
							strcpy_s(tMultiUseData[nNameCnt].cRef, buff2);
							nNameCnt++;
						}
					}

					fclose(fp);
				}
	
				m_pXCom->GetListItem( m_lMsgId, &nItems );
				n = nItems;
				if (n == 0)
				{
					GetLogCtrl()->AddBuf(LOG_CIM,"S8F3 Report all the items defined in the equipment");


					m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 8, 4, m_lSysByte);

						m_pXCom->SetListItem( lMkMsgID, 2 );

							cstr = "0"; 
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());

							m_pXCom->SetListItem( lMkMsgID, nNameCnt);

							for (int i=0; i<nNameCnt; i++)
							{
								m_pXCom->SetListItem( lMkMsgID, 2 );

									cstr = tMultiUseData[i].cType; 
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());

									m_pXCom->SetListItem( lMkMsgID, 3 );

										cstr = tMultiUseData[i].cName; 
										m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());

										cstr = tMultiUseData[i].cValue; 
										m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());

										cstr = tMultiUseData[i].cRef; 
										m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());


								GetLogCtrl()->AddBuf(LOG_CIM,"S8F4 TYPE(%s) NAME(%s) VALUE(%s) REF(%s).", tMultiUseData[i].cType, tMultiUseData[i].cName, tMultiUseData[i].cValue, tMultiUseData[i].cRef);
							}
				}
				else
				{
					//strItemType = new CString[n];
					for (int i=0; i<n; i++)
					{
						m_pXCom->GetListItem( m_lMsgId, &nItems );
							m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));
							strItemType.Format("%s", cAsciiBuf);
							m_pXCom->GetListItem( m_lMsgId, &nItems );
								n2 = nItems;
								if (n2 == 0)
								{
										for (int k=0; k<nNameCnt; k++)
										{
											if (strcmp(tMultiUseData[k].cType, strItemType)==0)
											{
												tMultiUseRepData[nRepNameCnt++] = tMultiUseData[k];
											}
										}
								}
								else
								{
/*									strName = new CString[n2];*/
									for (int m=0; m<n2; m++)
									{
										m_pXCom->GetAsciiItem( m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));
										strName.Format("%s", cAsciiBuf);

										for (int x=0; x<nNameCnt; x++)
										{

											if ( (strcmp(tMultiUseData[x].cType, strItemType)==0) && (strcmp(tMultiUseData[x].cName, strName)==0))
											{
												tMultiUseRepData[nRepNameCnt++] = tMultiUseData[x];
												n4++;
											}
										}	
									}

// 									for(int p=0; p<n2; p++)
// 									{
// 										for (int x=0; x<nNameCnt; x++)
// 										{
// 
// 											if ( (strcmp(tMultiUseData[x].cType, strItemType[i])==0) && (strcmp(tMultiUseData[x].cName, strName[p])==0))
// 											{
// 												tMultiUseRepData[nRepNameCnt++] = tMultiUseData[x];
// 												n4++;
// 											}
// 										}				
// 									}
/*									delete[] strName;*/
								}
					}


					m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 8, 4, m_lSysByte);

						m_pXCom->SetListItem( lMkMsgID, 2 );

						if (nRepNameCnt>0)
							cstr = "0";
						else
							cstr = "2";
							
							
							m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());

								m_pXCom->SetListItem( lMkMsgID, nRepNameCnt);

								for (int y=0; y<nRepNameCnt; y++)
								{
									m_pXCom->SetListItem( lMkMsgID, 2 );

									cstr = tMultiUseRepData[y].cType; 
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());

									m_pXCom->SetListItem( lMkMsgID, 3 );

									cstr = tMultiUseRepData[y].cName; 
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());

									cstr = tMultiUseRepData[y].cValue; 
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());

									cstr = tMultiUseRepData[y].cRef; 
									m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());

									GetLogCtrl()->AddBuf(LOG_CIM,"S8F4 TYPE(%s) NAME(%s) VALUE(%s) REF(%s).", tMultiUseRepData[y].cType, tMultiUseRepData[y].cName, tMultiUseRepData[y].cValue, tMultiUseRepData[y].cRef);

								}


			/*		delete[] strItemType;*/
				}
			}

		//if(strcmp(g_pData->m_cCRST, "0")!=0)
		if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
		{
			nReturn = m_pXCom->Send( lMkMsgID );		
			GetLogCtrl()->AddBuf(LOG_CIM,"S8F4 Send Return( %d )", nReturn );								
		}
		
		
		
}
	CATCH_ALL (e) 
	{
		return CB_ERR;
	} 
	END_CATCH_ALL

		return TRUE;
}

void CAutomation::RxS10Fxx(const int F)
{
	TRY
	{
		switch (F) {
			case 5:
				RxS10F5();
				break;
			
			default:
				//SendS9(5);
				break;
		}
	}
	CATCH_ALL (e) 
	{
		return;
	} 
	END_CATCH_ALL
}

BOOL CAutomation::RxS10F5() //Terminal Display, Multi-block
{
	// HOST가 특정 시점에 설비의 복수 Terminal에 Message Display 요청이 필요한 시점에 저놋ㅇ한다
	// ->S10F6

	GetLogCtrl()->AddBuf(LOG_CIM,"Recv S10F5 Terminal Display, Multi-block");

	long   nItems;
	signed char cAsciiBuf[255];
	CString	strEqpID, strMsg;
	CString	cstr;
	CString	strAck;
	long    lCnt;
	long lMkMsgID;
	lMkMsgID = 0;


	CMainFrame* pMainFrm = (CMainFrame*)::AfxGetMainWnd();

	TRY
	{
		m_pXCom->GetListItem( m_lMsgId, &nItems );
			m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//EQPID
			strEqpID.Format("%s", cAsciiBuf);
			strEqpID.Trim(' ');
			cstr = g_pData->m_cEQPID;
			cstr.Trim(' ');
			if (strcmp(strEqpID, cstr)==0)
			{
				m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//TID 0: Single or main terminal
				//kjpark 20170929 Terminel TID 출력
				strMsg.Format("%s", cAsciiBuf);
				strcpy_s(g_pData->m_cTerminalID, strMsg);				
				m_pXCom->GetListItem( m_lMsgId, &nItems );			//Terminal Block Count
				for (int i=0; i<nItems; i++)
				{
					m_pXCom->GetAsciiItem(m_lMsgId, cAsciiBuf, &lCnt, sizeof(cAsciiBuf));	//EQPID
					strMsg.Format("%s", cAsciiBuf);
					strcpy_s(g_pData->m_cTerminalMsg, strMsg);
					pMainFrm->m_pViewCIM->SetTerminalMessage();
					Sleep(100);		//AMT 화면에 누락 발생.
					strMsg.Trim(' ');
					GetLogCtrl()->AddBuf(LOG_CIM,"S10F5 Block(%d), MSG(%s), ", i, strMsg);
				}
				strAck = "0";

			} 
			else
			{
				strAck = "1";
				GetLogCtrl()->AddBuf(LOG_CIM,"S10F5 FAIL EQPID Mismatch(%s)", strEqpID);
			}


			//////////////////////////////////////////////////////////////////////////
			//
			//  S10F6 통신 규약 Format 
			//
			//	<A[1]		$ACKC10>			* Acknowledge Code Stream 10
			//
			// 
			//////////////////////////////////////////////////////////////////////////


		m_pXCom->MakeSecsMsg( &lMkMsgID, m_nDevId, 10, 6, m_lSysByte );

			//0: Accepted,
			//1: Not Accepted.

			cstr = strAck; 
			m_pXCom->SetAsciiItem( lMkMsgID, (signed char*)cstr.GetString(), cstr.GetLength());	//ACKC10

		//if(strcmp(g_pData->m_cCRST, "0")!=0)
		if(strcmp(g_pData->m_cCRST, CRST_OFF)!=0)
		{
			long nReturn = m_pXCom->Send( lMkMsgID );		
			GetLogCtrl()->AddBuf(LOG_CIM,"S10F6 Send Result( %d )", nReturn );
		}

		

	}
	CATCH_ALL (e) 
	{
		return CB_ERR;
	} 
	END_CATCH_ALL

	return TRUE;
}

void CAutomation::SecsLog(LPCSTR szMsg)
{
	GetLogCtrl()->AddBuf(LOG_CIM,"%s", szMsg);
}

BOOL CAutomation::ByDial(CString str)
{

	str.Trim(' ');

	if (str.GetLength()==0)
	{
		return false;
	}

	for (int i = 0; i < str.GetLength(); i++)
	{
		if (!isdigit(str[i]))
			return false;

	}

	return true;

}



// ECID 문자열 처리
CString CAutomation::MakeECString(CString str, BOOL bUpper)
{
	int nLen=str.GetLength();
	int nBreak=0;

	if (nLen <= 0 ) return str;			// 비어있다면 처리할 내용이 없다.

	while (str.GetAt(0) == 0x20 || str.GetAt(str.GetLength()-1) == 0x20
		|| str.GetAt(0) == '\t' || str.GetAt(str.GetLength()-1) == '\t'
		|| nBreak>=10)
	{
		str.Delete(str.Find("\t"));
		str.Trim(0x20);

		nBreak++;

		if ( nBreak >= nLen ) break;		// 가지고 있는 문자열 이상으로 처리하지 못하게 한다.
	}

	str.Replace(" ", "_");
	if (bUpper)
		str.MakeUpper();

	return str;
}

// 문자만 추려준다.
CString CAutomation::MakeParaString(CString str, BOOL bUpper)
{
	int nLen=0;
	int nBreak=0;

	if (nLen <= 0 ) return str;			// 비어있다면 처리할 내용이 없다.

	while (str.GetAt(0) == 0x20 || str.GetAt(str.GetLength()-1) == 0x20
		|| str.GetAt(0) == '\t' || str.GetAt(str.GetLength()-1) == '\t'
		|| nBreak>=10)
	{
		str.Delete(str.Find("\t"));
		str.Trim(0x20);

		nBreak++;

		if ( nBreak >= nLen ) break;		// 가지고 있는 문자열 이상으로 처리하지 못하게 한다.
	}

	str.Replace(" ", "_");
	if (bUpper)
		str.MakeUpper();

	return str;
}

void CAutomation::WriteTrackOutLog(CString strCellID, CString strInnerID, CString strDefectName)
{
	// 경로 : D:\TRACKOUT INFO\TRACKOUT_strCellID_strInnerID.txt
	// 파일 내용
	// EQP_ID=EQIPID
	// CELL_ID=strCellID
	// INNER_ID=strInnerID
	// RESULT=strDefectName

	if(CSettingParm::bWriteTrackOutInfo == FALSE)
		return;

	if(CFileSupport::DirectoryCheck(CGaonNuriApp::GetTrackOutInfoPath()) == FALSE)
		return;

	CString strPath, strData;
	strPath.Format(_T("%s\\TRACKOUT_%s_%s.txt"), CGaonNuriApp::GetTrackOutInfoPath(), strCellID, strInnerID);

	FILE *stream;
	if(fopen_s( &stream, strPath, "a+t" ) == 0 )
	{
		strData.Format(_T("%s=%s\n"), _T("EQP_ID"), g_pData->m_cEQPID);
		fprintf(stream, "%s", strData.GetString());
		strData.Format(_T("%s=%s\n"), _T("CELL_ID"), strCellID);
		fprintf(stream, "%s", strData.GetString());
		strData.Format(_T("%s=%s\n"), _T("INNER_ID"), strInnerID);
		fprintf(stream, "%s", strData.GetString());
		strData.Format(_T("%s=%s\n"), _T("RESULT"), strDefectName);
		fprintf(stream, "%s", strData.GetString());
		fclose(stream);
	}
}

BOOL CAutomation::UnitID_Validation(CString strID)
{
	// CIM으로 받은 UnitID가 정상적인지 확인
	CString strEqpID = g_pData->m_cEQPID;
	CMainFrame* pMainFrm = (CMainFrame*)::AfxGetMainWnd();
	CUnitStatusBank *pBank = &pMainFrm->m_pViewCIM->m_UnitStatusBank;

	// EQPID하고 같거나 NULL, 하이픈'-'이면 설비
	if(strID == strEqpID)
		return TRUE;
	else if(strID.IsEmpty())
		return TRUE;
	else if(strID == _T("-"))
		return TRUE;
	// Unit ID하고 같으면 Unit
	else
	{
		for(int jig = 0; jig < JIG_ID_MAX; jig++)
		{
			for(int ch = 0; ch < JIG_CH_MAX; ch++)
			{
				if(strID == pBank->m_Unit[jig][ch].m_strUnitID)
					return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL CAutomation::UnitID_IsUnit(CString strID)
{
	// CIM으로 받은 UnitID가 Unit이 맞는지 확인
	CString strEqpID = g_pData->m_cEQPID;
	CMainFrame* pMainFrm = (CMainFrame*)::AfxGetMainWnd();
	CUnitStatusBank *pBank = &pMainFrm->m_pViewCIM->m_UnitStatusBank;
	// Unit ID하고 같으면 Unit
	for(int jig = 0; jig < JIG_ID_MAX; jig++)
	{
		for(int ch = 0; ch < JIG_CH_MAX; ch++)
		{
			if(strID == pBank->m_Unit[jig][ch].m_strUnitID)
				return TRUE;
		}
	}
	return FALSE;
}
