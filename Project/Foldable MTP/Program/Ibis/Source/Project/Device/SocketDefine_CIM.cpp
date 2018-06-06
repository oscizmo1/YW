#include "stdafx.h"
#include "SocketDefine_CIM.h"
#include "Etc\Etc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


void tag_Message_Unicode::operator =(const tag_Message &src)
{
	sMsgClass = src.sMsgClass;
	sMsgType = src.sMsgType;
	sEqst = src.sEqst;
	sUnit = src.sUnit;
	sCellPos = src.sCellPos;
	sTactPoint = src.sTactPoint;
	const int length = MAX_MES_COUNT*MAX_MES_SIZE;
	wchar_t buf[length];
	const int longlength = MAX_LONG_MSG_COUNT*MAX_LONG_MSG_SIZE;
	wchar_t longbuf[longlength];

	for(int i = 0; i < MAX_MES_COUNT; i++)
	{
		memset(buf, NULL, length*sizeof(wchar_t));
		CEtc::ConvertUnicode((char*)src.cMsgData[i], (wchar_t*)buf, length-1);
		strMsgData[i].Format(_T("%s"), buf);
	}

	for(int i = 0; i < MAX_LONG_MSG_COUNT; i++)
	{
		memset(longbuf, NULL, longlength*sizeof(wchar_t));
		CEtc::ConvertUnicode((char*)src.cLongMsgData[i], (wchar_t*)longbuf, longlength-1);
		strLongMsgData[i].Format(_T("%s"), longbuf);
	}

	memset(buf, NULL, length*sizeof(wchar_t));
	CEtc::ConvertUnicode((char*)src.cCellID, (wchar_t*)buf, length-1);
	strCellID.Format(_T("%s"), buf);
}

void tag_Message_Unicode::Clear()
{
	sMsgClass = CLASS_NONE;
	sMsgType = TYPE_SET_NONE;
	sEqst = E_EQST_MATCH_NONE;
	sUnit = 0;
	sCellPos = 0;
	sTactPoint = 0;
	for(int i = 0; i < MAX_MES_COUNT; i++)
	{
		strMsgData[i].Empty();
	}
	for(int i = 0; i < MAX_LONG_MSG_COUNT; i++)
	{
		strLongMsgData[i].Empty();
	}
	strCellID.Empty();
}

void tag_Message_Unicode::Convert_tag_Message(tag_Message *pMsg)
{
	pMsg->sMsgClass = sMsgClass;
	pMsg->sMsgType = sMsgType;
	pMsg->sEqst = sEqst;
	pMsg->sUnit = sUnit;
	pMsg->sCellPos = sCellPos;
	pMsg->sTactPoint = sTactPoint;

	for(int i = 0; i < MAX_MES_COUNT; i++)
	{
		memset(pMsg->cMsgData[i], NULL, MAX_MES_SIZE*sizeof(char));
		if(strMsgData[i].IsEmpty() == FALSE)
			CEtc::ConvertMultibyte((wchar_t*)strMsgData[i].GetString(), (char*)pMsg->cMsgData[i], MAX_MES_SIZE-1);
	}

	for(int i = 0; i < MAX_LONG_MSG_COUNT; i++)
	{
		memset(pMsg->cLongMsgData[i], NULL, MAX_LONG_MSG_SIZE*sizeof(char));
		if(strLongMsgData[i].IsEmpty() == FALSE)
			CEtc::ConvertMultibyte((wchar_t*)strLongMsgData[i].GetString(), (char*)pMsg->cLongMsgData[i], MAX_LONG_MSG_SIZE-1);
	}

	memset(pMsg->cCellID, NULL, (MAX_CELL_ID+1)*sizeof(char));
	if(strCellID.IsEmpty() == FALSE)
		CEtc::ConvertMultibyte((wchar_t*)strCellID.GetString(), (char*)pMsg->cCellID, MAX_CELL_ID);
}

void EQUIP_FUNCTION::Clear()
{
	bChangeFlag = FALSE;
	USE = FALSE;
	EFID = EFID_CELL_TRACKING;
	EFNAME.Empty();
	EFST_TYPE1.Empty();
	EFST_TYPE2.Empty();
	EFST_TYPE3.Empty();
	EFST_TYPE4.Empty();
	EFST_Old.Empty();
	EFST.Empty();
}

BOOL EQUIP_FUNCTION::EFST_Set(CString strValue)
{
	// 해당하는 값이 아니면 넣지 않는다 [11/14/2017 OSC]
	if(EFST_Check(strValue))
	{
		// 입력 값과 현재 값이 다르 다면 입력 값 저장한다 - LSH171130
		if(EFST != strValue)
		{
			// EFST가 비어있지 않다면
			if(!EFST.IsEmpty())
			{
				// 변경 상태를 TRUE로 바꾼다 - LSH171130
				SetChangeState(TRUE);
			}
				
			// 새로운 값을 저장한다 - LSH171130
			EFST_Old = EFST;
			EFST = strValue;

			return TRUE;
		}

		return FALSE;
	}
	else
	{
		return FALSE;
	}
}

BOOL EQUIP_FUNCTION::EFST_Check(CString strValue)
{
	// 기존하고 틀린 값이고, 형식에 맞으면 TRUE
	if(EFST == strValue)
		return FALSE;

	if( (strValue == EFST_TYPE1) && (EFST_TYPE1.IsEmpty() == FALSE) )
		return TRUE;
	else if( (strValue == EFST_TYPE2) && (EFST_TYPE2.IsEmpty() == FALSE) )
		return TRUE;
	else if( (strValue == EFST_TYPE3) && (EFST_TYPE3.IsEmpty() == FALSE) )
		return TRUE;
	else if( (strValue == EFST_TYPE4) && (EFST_TYPE4.IsEmpty() == FALSE) )
		return TRUE;

	return FALSE;
}

CString EQUIP_FUNCTION::EFST_Get()
{
	return EFST;
}
