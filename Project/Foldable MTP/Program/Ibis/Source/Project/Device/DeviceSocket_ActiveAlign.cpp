#include "StdAfx.h"
#include "DeviceSocket_ActiveAlign.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDeviceSocket_ActiveAlign::CDeviceSocket_ActiveAlign(void)
	:CDeviceSocket_Base()
{
}
CDeviceSocket_ActiveAlign::~CDeviceSocket_ActiveAlign(void)
{
}

void CDeviceSocket_ActiveAlign::CreateSocket()
{
	if(m_Socket.IsCreated())
		m_Socket.Close();

	m_Socket.SetServer(TRUE);				// 서버로 설정한다.

	m_Socket.SetSocketName(SOCKET_ACTIVEALIGN_PC);
	m_Socket.SetIP(ALIGN_PC_IP);
	m_Socket.SetPort(ALIGN_PC_ACTIVE_PORT);

	ClientSocketName socketName;
	socketName.Name = SOCKET_ACTIVEALIGN_PC;
	socketName.IP = ALIGN_PC_IP;
	socketName.Port = ALIGN_PC_ACTIVE_PORT;

	m_Socket.AddClientName( socketName );
	m_Socket.SetReciveHwnd(m_hParent);

	if(m_Socket.Create(m_Socket.GetPort()))
	{
		m_Socket.Listen();											// 수신을 시작한다.
	}
}

void CDeviceSocket_ActiveAlign:: PreparseMessage(CDataSocket* pSocket)
{
	int nMsgLen = pSocket->GetLastLength();

	wchar_t wszMsg[4096];
	if (nMsgLen > 4095) 
		nMsgLen = 4095;

	CEtc::ConvertUnicode(pSocket->GetLastMsg(), nMsgLen, wszMsg, 4096);
	wszMsg[nMsgLen] = NULL;

	CString strMsg;
	strMsg.Format(_T("%s"), wszMsg);

	ParseActiveAlignMessage(strMsg);


}

BOOL CDeviceSocket_ActiveAlign::SendMassageToActiveAlign(CString strCmd, int nTeach/*=0*/, int nStage/*=0*/, CString strInnerID /*= _T("")*/, CString strCellID /*= _T("")*/, CString strRecipe/* = _T("")*/)
{
	// Socket연결상태 확인
	if( !m_Socket.IsConnected() )
		return FALSE;

	// Message 저장 변수 [1/30/2018 LSH]
	CString strMsg;

	BYTE ucData[1024];
	ucData[0] = STX;
	ucData[1] = ETX;
	if(strCmd == AVTIVE_ALIGN_ALIGN_REQ)
	{
		strMsg.Format(_T("%c%s;0;%d;%s;%s;%c"),STX, strCmd, nStage, strInnerID, strCellID, ETX);
	}
	else if(strCmd == AVTIVE_ALIGN_RECIPE_REQ)
	{
		strMsg.Format(_T("%c%s;%s;%c"),STX, strCmd, strRecipe, ETX);
	}
	else if(strCmd == AVTIVE_ALIGN_TEACHING_SAVE)
	{
		//	strMsg.Format(_T("%c%s;0;%d;%c"),STX, nStage, strCmd, ETX);
		strMsg.Format(_T("%c%s;0;%d;%c"),STX, strCmd, nStage, ETX);
	}

	char szBuff[256];

	CEtc::ConvertMultibyte((wchar_t*)strMsg.GetString(), szBuff, 256);
	//JSW180319 ALIGN SOCKET NAME 변경.
	if( m_Socket.SendClient(SOCKET_ACTIVEALIGN_PC,szBuff,strlen(szBuff)) )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

CString CDeviceSocket_ActiveAlign::ParseActiveAlignMessage(CString strReadingText)
{
	int nIndex = 0;

	// Parse할 데이터를 저장할 변수 선언 - LSH171010
	CString strCommand = _T("");	//명령
	CString strParam1 = _T("");		//result
	CString strParam2 = _T("");		//Array index
	CString strParam3 = _T("");		//stage번호
	CString strParam4 = _T("");		//inner id
	CString strParam5 = _T("");		//cell id
	CString strParam6X = _T("");		// ALIGN_RES X축 저장할 변수
	CString strParam6Y = _T("");		// ALIGN_RES Y축 저장할 변수
	CString strParam6T = _T("");		// ALIGN_RES Theta 저장할 변수

	// STX위치를 찾은 후 이전데이터를 삭제 - LSH171010 
	nIndex = strReadingText.Find(STX);
	if(nIndex > 0)
	{
		strReadingText.Delete(0, nIndex);
	}

	// STX - ETX 사이에 데이터만 사용 - LSH171010
	nIndex = strReadingText.Find(ETX);
	CString strTemp = strReadingText.Mid(0, nIndex+1);

	// STX, ETX 제거
	strTemp.TrimLeft(STX);
	strTemp.TrimRight(ETX);
	CTokenizer t(strTemp);

	// Data parse - LSH171010
	strCommand = t.GetNextToken(_T(";"));	// Command => ALIGN_RES, TCH_RES, RECIPE_RES
	strParam1 = t.GetNextToken(_T(";"));	// Param1 => OK, NG
	strParam2 = t.GetNextToken(_T(";"));	// Param2 => 0
	strParam3 = t.GetNextToken(_T(";"));	// Param3 => Ch Number(0~7) or (1~8)
	strParam4 = t.GetNextToken(_T(";"));	// Param4 => Inner ID
	strParam5 = t.GetNextToken(_T(";"));	// Param5 => Cell ID
	strParam6Y = t.GetNextToken(_T(";"));	// Param6Y => Y Value
	strParam6X = t.GetNextToken(_T(";"));	// Param6X => X Value
	strParam6T = t.GetNextToken(_T(";"));	// Param6T => T Value

	if(AVTIVE_ALIGN_ALIGN_RES == strCommand)
	{
		if(strParam1 == _T("OK"))
		{
			// Align ch값을 저장 [1/31/2018 LSH]
			int nCh;
			nCh = _wtoi(strParam3.GetString());

			// Align 완료 여부 True 변경 [1/31/2018 LSH]
			m_LastAlignValue[nCh].bAlignFlag = TRUE;

			// Align 성공 여부 True 변경 [1/31/2018 LSH]
			m_LastAlignValue[nCh].bAlignSuucess = TRUE;

			// 보상값을 저장 [1/31/2018 LSH]
			m_LastAlignValue[nCh].dY = _wtof(strParam6Y.GetString());
			m_LastAlignValue[nCh].dX = _wtof(strParam6X.GetString());
			m_LastAlignValue[nCh].dT = _wtof(strParam6T.GetString());
		}
		else	// NG
		{
			// Align ch값을 저장 [1/31/2018 LSH]
			int nCh;
			nCh = _wtoi(strParam3.GetString());

			// Align 성공 여부 True 변경 [1/31/2018 LSH]
			m_LastAlignValue[nCh].bAlignSuucess = TRUE;

			// 보상값을 저장 [1/31/2018 LSH]
			m_LastAlignValue[nCh].dY = 0.0;
			m_LastAlignValue[nCh].dX = 0.0;
			m_LastAlignValue[nCh].dT = 0.0;

			// Align 완료 여부 True 변경 동기화를 위해 무조건 값을 입력후에 TRUE로 변경 [1/31/2018 LSH]
			m_LastAlignValue[nCh].bAlignFlag = TRUE;
		}
	}
	else if(AVTIVE_ALIGN_TEACHING_RES == strCommand)
	{
	}
	else if(AVTIVE_ALIGN_RECIPE_RES == strCommand)
	{
		if(strParam1 == _T("OK"))
		{
			//theProcBank.strPreAlignRecipeResult = strParam1;
		}
		else
		{
			//theProcBank.strPreAlignRecipeResult = strParam1;
		}
	}

	return _T("Align receive ready");
}

// Align Save [2/1/2018 LSH]
BOOL CDeviceSocket_ActiveAlign::ActiveAlignSave(JIG_CH nCh)
{
	// Align save 명령 실행 [2/1/2018 LSH]
	return SendMassageToActiveAlign(AVTIVE_ALIGN_TEACHING_SAVE, 0, nCh);
}

// Align Save [2/1/2018 LSH]
BOOL CDeviceSocket_ActiveAlign::ActiveAlignRecipeChange(JIG_CH nCh)
{
	// Recipe 변경 [2/1/2018 LSH]
	return SendMassageToActiveAlign(AVTIVE_ALIGN_RECIPE_REQ, 0, nCh);
}

// Align Save [2/1/2018 LSH]
BOOL CDeviceSocket_ActiveAlign::ActiveAlignGrab(JIG_CH nCh)
{
	// Recipe 변경 [2/1/2018 LSH]
	return SendMassageToActiveAlign(AVTIVE_ALIGN_ALIGN_REQ, 0, nCh);
}
