#include "StdAfx.h"
#include "UnitCtrlBank.h"
#include "IbisApp.h"
#include "UI\GausGUI\GxMsgBox.h"
#include "Etc\FileSupport.h"
#include "Etc\Ini.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


ALARM_ID CUnitCtrlBank::GetAlarmID_of_Shuttle( ALARM_ID id, JIG_ID jig )
{
	// 전부 1000의 배수로 나뉘어지니 Shuttle Index에 1000을 곱한다 [8/30/2017 OSC]
	return (ALARM_ID)(id + (jig*ALM_SHUTTLE1_PART));
}

//축들이 원점을 잡을때 인터락
BOOL CUnitCtrlBank::CheckOriginInterlock(AXIS_ID idAxis)
{
	theProcBank.m_strLastKorMsg = _T("");
	theProcBank.m_strLastEngMsg = _T("");
	theProcBank.m_strLastVnmMsg = _T("");
	BOOL bRet = TRUE;

	//20170912 hhkim 원점 인터락
	switch(idAxis)
	{

		case AXIS_JIG_SHUTTLE_Y1:
			if(Inspection_Z_UP_Check(JIG_ID_A) ==FALSE)
			{
				theProcBank.m_strLastKorMsg = _T("Inspection Z1축을 Up해주세요.");
				theProcBank.m_strLastEngMsg = _T("Please Move Up Inspection Z1.");
				theProcBank.m_strLastVnmMsg = _T("Xin hãy di chuyển Inspection trục Z1 lên.");
				bRet = FALSE;
			}
			break;

		case AXIS_JIG_SHUTTLE_Y2:
			if(Inspection_Z_UP_Check(JIG_ID_B) ==FALSE)
			{
				theProcBank.m_strLastKorMsg = _T("Inspection Z2축을 Up해주세요.");
				theProcBank.m_strLastEngMsg = _T("Please Move Up Inspection Z2.");
				theProcBank.m_strLastVnmMsg = _T("Xin hãy di chuyển Inspection trục Z2 lên.");
				bRet = FALSE;
			}
			break;

		case AXIS_INSPECTION_X1:
			if(Inspection_Z_UP_Check(JIG_ID_A) ==FALSE)
			{
				theProcBank.m_strLastKorMsg = _T("Inspection Z1축을 Up해주세요.");
				theProcBank.m_strLastEngMsg = _T("Please Move Up Inspection Z1.");
				theProcBank.m_strLastVnmMsg = _T("Xin hãy di chuyển Inspection trục Z1 lên.");
				bRet = FALSE;
			}
			break;

		case AXIS_INSPECTION_X2:
			if(Inspection_Z_UP_Check(JIG_ID_B) ==FALSE)
			{
				theProcBank.m_strLastKorMsg = _T("Inspection Z2축을 Up해주세요.");
				theProcBank.m_strLastEngMsg = _T("Please Move Up Inspection Z2.");
				theProcBank.m_strLastVnmMsg = _T("Xin hãy di chuyển Inspection trục Z2 lên.");
				bRet = FALSE;
			}
			break;

			// Z축은 인터락 필요가없나?? 없을거 같음...
	}
	return bRet;
}



BOOL CUnitCtrlBank::CheckTeachMoveInterlock(TEACH_PARAM::ID idTeach, double dOffset /*= 0.*/ )
{
	// CDeviceMotion::TeachMove에서 축에 이동명령을 내리기 전에 확인하는 Interlock [10/26/2016 OSC]
	// BOOL로 return하며, 관련 Msg를 따로 멤버변수에 남기므로 티칭 UI에서는 아래 변수 Text를 MsgBox로 띄우면 됨
	theProcBank.m_strLastKorMsg = _T("");
	theProcBank.m_strLastEngMsg = _T("");
	theProcBank.m_strLastVnmMsg = _T("");
	BOOL bRet = TRUE;
	//20170912 hhkim TeachMveInterlock
	switch(idTeach)
	{

	case TEACH_PARAM::JIG_SHUTTLE_Y1_to_LOAD:
		if(Inspection_Z_UP_Check(JIG_ID_A) ==FALSE)
		{
			theProcBank.m_strLastKorMsg = _T("Inspection Z1축을 Up해주세요.");
			theProcBank.m_strLastEngMsg = _T("Please Move Up Inspection Z1.");
			theProcBank.m_strLastVnmMsg = _T("Xin hãy di chuyển Inspection trục Z1 lên.");
			bRet = FALSE;
		}
		if(theConfigBank.m_System.m_bInlineMode)
		{
			if(PDT_IF_ArmStatus_Check(JIG_ID_A))
			{
				theProcBank.m_strLastKorMsg = _T("Robot을 치워주세요.");
				theProcBank.m_strLastEngMsg = _T("Please move out Robot.");
				theProcBank.m_strLastVnmMsg = _T("Xin hay di chuyen Robot ra.");
				bRet = FALSE;
			}
		}
		break;

	case TEACH_PARAM::JIG_SHUTTLE_Y1_to_MANUAL:
		if(Inspection_Z_UP_Check(JIG_ID_A) ==FALSE)
		{
			theProcBank.m_strLastKorMsg = _T("Inspection Z1축을 Up해주세요.");
			theProcBank.m_strLastEngMsg = _T("Please Move Up Inspection Z1.");
			theProcBank.m_strLastVnmMsg = _T("Xin hãy di chuyển Inspection trục Z1 lên.");
			bRet = FALSE;
		}
		break;

	case TEACH_PARAM::JIG_SHUTTLE_Y1_to_INSP:
		if(Inspection_Z_UP_Check(JIG_ID_A) ==FALSE)
		{
			theProcBank.m_strLastKorMsg = _T("Inspection Z1축을 Up해주세요.");
			theProcBank.m_strLastEngMsg = _T("Please Move Up Inspection Z1.");
			theProcBank.m_strLastVnmMsg = _T("Xin hãy di chuyển Inspection trục Z1 lên.");
			bRet = FALSE;
		}
		break;
/////////////////////////////////////////////////////////////////////////////////////////////
	case TEACH_PARAM::JIG_SHUTTLE_Y2_to_LOAD:
		if(Inspection_Z_UP_Check(JIG_ID_B) ==FALSE)
		{
			theProcBank.m_strLastKorMsg = _T("Inspection Z2축을 Up해주세요.");
			theProcBank.m_strLastEngMsg = _T("Please Move Up Inspection Z2.");
			theProcBank.m_strLastVnmMsg = _T("Xin hãy di chuyển Inspection trục Z2 lên.");
			bRet = FALSE;
		}
		if(theConfigBank.m_System.m_bInlineMode)
		{
			if(PDT_IF_ArmStatus_Check(JIG_ID_B))
			{
				theProcBank.m_strLastKorMsg = _T("Robot을 치워주세요.");
				theProcBank.m_strLastEngMsg = _T("Please move out Robot.");
				theProcBank.m_strLastVnmMsg = _T("Xin hay di chuyen Robot ra.");
				bRet = FALSE;
			}
		}
		break;

	case TEACH_PARAM::JIG_SHUTTLE_Y2_to_MANUAL:
		if(Inspection_Z_UP_Check(JIG_ID_B) ==FALSE)
		{
			theProcBank.m_strLastKorMsg = _T("Inspection Z2축을 Up해주세요.");
			theProcBank.m_strLastEngMsg = _T("Please Move Up Inspection Z2.");
			theProcBank.m_strLastVnmMsg = _T("Xin hãy di chuyển Inspection trục Z2 lên.");
			bRet = FALSE;
		}
		break;

	case TEACH_PARAM::JIG_SHUTTLE_Y2_to_INSP:
		if(Inspection_Z_UP_Check(JIG_ID_B) ==FALSE)
		{
			theProcBank.m_strLastKorMsg = _T("Inspection Z2축을 Up해주세요.");
			theProcBank.m_strLastEngMsg = _T("Please Move Up Inspection Z2.");
			theProcBank.m_strLastVnmMsg = _T("Xin hãy di chuyển Inspection trục Z2 lên.");
			bRet = FALSE;
		}
		break;
//////////////////////////////////////////////////////////////////////////////////////////////////
	case TEACH_PARAM::INSPECTION_X1_to_INSP:
		if(Inspection_Z_UP_Check(JIG_ID_A) ==FALSE)
		{
			theProcBank.m_strLastKorMsg = _T("Inspection Z1축을 Up해주세요.");
			theProcBank.m_strLastEngMsg = _T("Please Move Up Inspection Z1.");
			theProcBank.m_strLastVnmMsg = _T("Xin hãy di chuyển Inspection trục Z1 lên.");
			bRet = FALSE;
		}
		break;

	case TEACH_PARAM::INSPECTION_X2_to_INSP:
		if(Inspection_Z_UP_Check(JIG_ID_B) ==FALSE)
		{
			theProcBank.m_strLastKorMsg = _T("Inspection Z2축을 Up해주세요.");
			theProcBank.m_strLastEngMsg = _T("Please Move Up Inspection Z2.");
			theProcBank.m_strLastVnmMsg = _T("Xin hãy di chuyển Inspection trục Z2 lên.");
			bRet = FALSE;
		}
		break;
////////////////////////////////////////////////////////////////////////////////////////////////
	case TEACH_PARAM::INSPECTION_Z1_to_UP:
		break;
	case TEACH_PARAM::INSPECTION_Z2_to_UP:
		break;
/////////////////////////////////////////////////////////////////////////////////////////////////
	case TEACH_PARAM::INSPECTION_Z1_to_INSP:
		if(Shuttle_Y_LOAD_Check(JIG_ID_A) == FALSE)
		{
			if(Inspection_X_INSP_Check(JIG_ID_A) ==FALSE)
			{
				theProcBank.m_strLastKorMsg = _T("Inspection X1 위치에서 Down해주세요.");
				theProcBank.m_strLastEngMsg = _T("Please Move Down on the Inspection X1.");
				theProcBank.m_strLastVnmMsg = _T("Xin hãy di chuyển Inspection trục X1 xuống.");
				bRet = FALSE;
			}
		}
		break;

	case TEACH_PARAM::INSPECTION_Z2_to_INSP:
		if(Shuttle_Y_LOAD_Check(JIG_ID_B) == FALSE)
		{
			if(Inspection_X_INSP_Check(JIG_ID_B) ==FALSE)
			{
				theProcBank.m_strLastKorMsg = _T("Inspection X2 위치에서 Down해주세요.");
				theProcBank.m_strLastEngMsg = _T("Please Move Down on the Inspection X2.");
				theProcBank.m_strLastVnmMsg = _T("Xin hãy di chuyển Inspection trục X2 xuống.");
				bRet = FALSE;
			}
		}
		break;
/////////////////////////////////////////////////////////////////////////////////////////////////////
	case TEACH_PARAM::ACTIVE_ALIGN_X1_to_LEFT_X:
		break;
	case TEACH_PARAM::ACTIVE_ALIGN_X1_to_RIGHT_X:
		break;
	case TEACH_PARAM::ACTIVE_ALIGN_X2_to_LEFT_X:
		break;
	case TEACH_PARAM::ACTIVE_ALIGN_X2_to_RIGHT_X:
		break;
/////////////////////////////////////////////////////////////////////////////////////////////////////

    }
	return bRet;
}

//kjpark 20161030 정위치가 아니면 A존에 A지그 가도록 다이얼로그창 띄움
void CUnitCtrlBank::TeachMove(TEACH_PARAM::ID idTeach, double dOffset)
{
	theDeviceMotion.TeachMove(m_nThreadID, idTeach);
}


BOOL CUnitCtrlBank::CellTagExist( CELL_POS pos )
{
	return theCellBank.GetCellTag(pos).IsExist();
}

//kjpark 20170915 Restart 구현
BOOL CUnitCtrlBank::CellTagExist(JIG_ID jig, JIG_CH channel)
{	
	return theCellBank.GetCellTag(jig, channel).IsExist();
}


BOOL CUnitCtrlBank::CellTagExist( CELL_POS posStart, CELL_POS posEnd, CHECK_OPTION chkoption /*= CHECK_OR*/ )
{
	// 하나라도 비어있으면 FALSE [11/5/2016 OSC]
	if(chkoption == CHECK_AND)
	{
		for(int i = posStart; i <= posEnd; i++)
		{
			if(theCellBank.GetCellTag((CELL_POS)i).IsExist() == FALSE)
				return FALSE;
		}
		return TRUE;
	}
	// 하나라도 있으면 TRUE [11/5/2016 OSC]
	else
	{
		for(int i = posStart; i <= posEnd; i++)
		{
			if(theCellBank.GetCellTag((CELL_POS)i).IsExist())
				return TRUE;
		}
		return FALSE;
	}

}

void CUnitCtrlBank::CellTagChange( CELL_POS posBefore, CELL_POS posAfter )
{
	theCellBank.SetCellTagNextPosition(posBefore, posAfter);
}

void CUnitCtrlBank::PreInterlockHappen(CString strEFST)
{
	// 쟁여두었던 인터락을 발생시킨다
	INTERLOCK_ALARM_INFOMATION info;
	if(strEFST == EFST_STEP)
	{
		POSITION pos = theProcBank.m_listPreInterlockStep.GetHeadPosition();
		while(pos)
		{
			if(theProcBank.m_MsgInterlockCnt > INTERLOCKMAX) break;

			info = theProcBank.m_listPreInterlockStep.GetNext(pos);
			theProcBank.SetWriteInterlockLog(info);

			theProcBank.m_strInterlockMsg[theProcBank.m_MsgInterlockCnt] = info.strInterlock_Message;
			theProcBank.m_MsgInterlockCnt++;
			if(theProcBank.m_strOldInterlockID == _T(""))
			{
				theProcBank.m_strOldInterlockID = theProcBank.m_strInterlockID; 
				theProcBank.m_strInterlockID = info.strInterlock_ID;
			}
		}
		theProcBank.m_listPreInterlockStep.RemoveAll();
	}
	else if(strEFST == EFST_LOADING)
	{
		POSITION pos = theProcBank.m_listPreInterlockLoad.GetHeadPosition();
		while(pos)
		{
			if(theProcBank.m_MsgInterlockCnt > INTERLOCKMAX) break;

			info = theProcBank.m_listPreInterlockLoad.GetNext(pos);
			theProcBank.SetWriteInterlockLog(info);

			theProcBank.m_strInterlockMsg[theProcBank.m_MsgInterlockCnt] = info.strInterlock_Message;
			theProcBank.m_MsgInterlockCnt++;
			if(theProcBank.m_strOldInterlockID == _T(""))
			{
				theProcBank.m_strOldInterlockID = theProcBank.m_strInterlockID; 
				theProcBank.m_strInterlockID = info.strInterlock_ID;
			}
		}
		theProcBank.m_listPreInterlockLoad.RemoveAll();
	}
	else if(strEFST == EFST_TRANSFER)
	{
		POSITION pos = theProcBank.m_listPreInterlockTrans.GetHeadPosition();
		while(pos)
		{
			if(theProcBank.m_MsgInterlockCnt > INTERLOCKMAX) break;

			info = theProcBank.m_listPreInterlockTrans.GetNext(pos);
			theProcBank.SetWriteInterlockLog(info);

			theProcBank.m_strInterlockMsg[theProcBank.m_MsgInterlockCnt] = info.strInterlock_Message;
			theProcBank.m_MsgInterlockCnt++;
			if(theProcBank.m_strOldInterlockID == _T(""))
			{
				theProcBank.m_strOldInterlockID = theProcBank.m_strInterlockID; 
				theProcBank.m_strInterlockID = info.strInterlock_ID;
			}
		}
		theProcBank.m_listPreInterlockTrans.RemoveAll();
	}
	theProcBank.SetInterlock(strEFST);
}

void CUnitCtrlBank::PreUnitInterlockHappen()
{
	// 쟁여두었던 인터락을 발생시킨다
	INTERLOCK_ALARM_INFOMATION info;
	POSITION pos = theProcBank.m_listPreUnitInterlock.GetHeadPosition();
	while(pos)
	{
		if(theProcBank.m_UnitInterlockCnt > INTERLOCKMAX) break;

		info = theProcBank.m_listPreUnitInterlock.GetNext(pos);
		theProcBank.SetWriteInterlockLog(info);

		theProcBank.m_strUnitInterlockMsg[theProcBank.m_UnitInterlockCnt] = info.strInterlock_Message;
		theProcBank.m_UnitInterlockCnt++;
		theProcBank.m_strUnitInterlockID = info.strInterlock_ID;
		theProcBank.m_strInterlockedUnitID = info.strUnit_ID;

		theUnitStatusBank.SetInterlock(info.strUnit_ID);
	}
	theProcBank.PreUnitInterlock_Clear();
}

//kjpark 20161016 Cell Result 항목 추가
void CUnitCtrlBank::JudgeFinalClass(JIG_ID jig, JIG_CH ch)
{
	CCellTag tagCell = theCellBank.GetCellTag(jig, ch);

	if(tagCell.IsExist()==FALSE)
		return;

	CCellInfo *pCell = theCellBank.GetCellInfo(tagCell);

	CLASS_CELL nClass = NONE_CELL;
	CString strDefect;

	pCell->defaultData.FinalJudge = GetDefectFromJudge(ZONE_ID_MAX, pCell, strDefect, nClass);

	// ByPass 모드일 경우 모두 Good으로 판정한다 - LSH171208
	if( theConfigBank.m_Option.m_bUseByPass )
	{
		pCell->defaultData.m_strLastResult = _T("GOOD");
		pCell->defaultData.m_LastClass = GOOD_CELL;
	}
	else
	{
		pCell->defaultData.m_strLastResult = strDefect;
		pCell->defaultData.m_LastClass = nClass;
	}

	if(pCell->defaultData.m_LastClass != GOOD_CELL)
	{
		// 불량코드 판정
		pCell->defaultData.MesCode = MesCode(pCell->defaultData.m_strLastResult, 
			pCell->defaultData.m_bRetryAble,
			pCell->defaultData.FinalJudge);

		// 리트라이 옵션이 꺼져 있으면 해제 [12/1/2017 OSC]
		if(theConfigBank.m_Option.m_bUseRetryAB == FALSE)
			pCell->defaultData.m_bRetryAble = FALSE;

		// Job Start가 안되도 리트라이 해제 [12/4/2017 OSC]
		if( theConfigBank.m_CIM.TRACKING_CONTROL_InCheck() && (theProcBank.GetCimState() == CONST_CIM_STATE::CIM_REMOTE) )
		{
			if(pCell->defaultData.m_nInspectInvalidType != JOB_START)
				pCell->defaultData.m_bRetryAble = FALSE;
		}
	}

	// AB RULE 기록
	if(pCell->defaultData.m_bRetryAB)
	{
		// Retry한 거면 무조건 AB
		pCell->defaultData.m_strABRule = AB_RULE_AB;
	}
	else
	{
		if(pCell->defaultData.m_LastClass == GOOD_CELL)
		{
			// 양품이면 공백
			pCell->defaultData.m_strABRule = AB_RULE_GOOD;
		}
		else
		{
			// Retry 할 거면 A
			if( pCell->defaultData.m_bRetryAble && (pCell->defaultData.m_bRetryAB == FALSE) )
				pCell->defaultData.m_strABRule = AB_RULE_A;
		}
	}

	theLog[LOG_JUDGE].AddBuf(_T("%s\t%s\tdefaultCellInfomation.FinalJudge\t%s"), pCell->defaultData.m_strCellID, pCell->defaultData.m_strInnerID, CONST_JUDGE_LIST[pCell->defaultData.FinalJudge].strName);
	theLog[LOG_JUDGE].AddBuf(_T("%s\t%s\tFinalClass\t%d"), pCell->defaultData.m_strCellID, pCell->defaultData.m_strInnerID, pCell->defaultData.m_LastClass);
	theLog[LOG_JUDGE].AddBuf(_T("%s\t%s\tFinalDefect\t%s"), pCell->defaultData.m_strCellID, pCell->defaultData.m_strInnerID, pCell->defaultData.m_strLastResult);
	theLog[LOG_JUDGE].AddBuf(_T("%s\t%s\tAB RULE\t%s"), pCell->defaultData.m_strCellID, pCell->defaultData.m_strInnerID, pCell->defaultData.m_strABRule);
}

CString CUnitCtrlBank::MesCode(CString strDefectName, BOOL &bRetryAble, int nJudge /*= CONST_JUDGE_NAME::JUDGE_MCR*/)
{
	// 1. 전부 대문자로 바꾸고 비교
	// 2. 괄호부터는 무시
	CString strParsedDefectName = strDefectName;
	int nIndex = strDefectName.Find(_T("("));
	
	// 괄호부터 시작하는 불량명이 있어서 그런것들은 전체 비교
	if(nIndex > 3)
		strParsedDefectName = strDefectName.Mid(0, nIndex);
	strParsedDefectName.MakeUpper();

	BOOL bFind = FALSE;
	DEFECT_MES_CODE defectUndefinedCode, defectTSPUndefinedCode, defectForceUndefinedCode,defectCurrentUndefinedCode, defectFinal;
	map<int, DEFECT_MES_CODE>::iterator itmap = theProcBank.iMapDefectList.begin();
	for(; itmap != theProcBank.iMapDefectList.end(); ++ itmap)
	{		
		//if(pCell->defaultCellInfomation.m_LastResult.Find(itmap->second.strPopup) == 0) //문자 포함할 경우 사용
		if(strParsedDefectName == itmap->second.strPopup)
		{
			defectFinal = itmap->second;
			bFind = TRUE;
		}
		else if(TEXT_UNDEFINED == itmap->second.strPopup)
		{
			defectUndefinedCode = itmap->second;
// 			strUndefinedCode = TEXT_UNDEFINED_CODE;
		}
		else if(TEXT_TSP_IC == itmap->second.strPopup)
		{
			defectTSPUndefinedCode = itmap->second;
		}
// 		else if(TEXT_TOL_MAX_OVER== itmap->second.strPopup)
// 		{
// 			defectForceUndefinedCode = itmap->second;
// 		}
		else if(TEXT_I_BAT_OVER== itmap->second.strPopup)
		{
			defectCurrentUndefinedCode= itmap->second;
		}
		////////////////////////////////////////////////////////////////////
	}
	if(bFind == FALSE)
	{
		if( nJudge == CONST_JUDGE_NAME::JUDGE_TSP_START)
			defectFinal = defectTSPUndefinedCode;
// 		else if((nJudge == CONST_JUDGE_NAME::JUDGE_FORCE1)|| (nJudge == CONST_JUDGE_NAME::JUDGE_FORCE2)
// 			|| (nJudge == CONST_JUDGE_NAME::JUDGE_FORCE3))
// 			defectFinal = defectForceUndefinedCode;
		else if( nJudge == CONST_JUDGE_NAME::JUDGE_WHITE_CURRENT || nJudge == CONST_JUDGE_NAME::JUDGE_HLPM_CURRENT
			|| nJudge == CONST_JUDGE_NAME::JUDGE_SLEEP_CURRENT )
			defectFinal = defectCurrentUndefinedCode;
		else
			defectFinal = defectUndefinedCode;
	}


	bRetryAble = defectFinal.bRetryAble;
	return defectFinal.strMES_CODE;
}

//kjpark 20161016 Cell Result 항목 추가
//kjpark 20161017 WoorkTable Turn
int CUnitCtrlBank::GetDefectFromJudge( ZONE_ID zone, CCellInfo *pCell, CString &strDefect, CLASS_CELL &nClass )
{
	int nJudgeIndex = CONST_JUDGE_NAME::JUDGE_MCR;
	switch(zone)
	{
	case ZONE_ID_A: 
		nJudgeIndex = AZoneJudgeFlow(pCell, strDefect, nClass);
		break;
	case ZONE_ID_C: 
		nJudgeIndex = CZoneJudgeFlow(pCell, strDefect, nClass);
		break;
	case ZONE_ID_MAX: 
		nJudgeIndex = LastJudgeFlow(pCell, strDefect, nClass);
		break;
	}

	// SDC 이성민 요청으로 존 대표 불량도 괄호 이후 삭제 [7/13/2017 OSC]
	int nIndex = strDefect.Find(_T("("));
	if(nIndex > 3)
		strDefect = strDefect.Mid(0, nIndex);

	return nJudgeIndex;
}

//kjpark 20161016 Cell Result 항목 추가
int CUnitCtrlBank::AZoneJudgeFlow( CCellInfo *pCell, CString &strDefect, CLASS_CELL &nClass )
{
	if( pCell->defaultData.m_bPGAlarm/* && (pCell->defaultCellInfomation.m_PGAlarmZone == ZONE_ID_A)*/ )
	{
		strDefect = pCell->defaultData.m_strPGAlarmName;
		nClass = REJECT_CELL;
		return CONST_JUDGE_NAME::JUDGE_PG_ALARM;
	}
	else
	{
		CONST_JUDGE_NAME::ID nJudge;
		nJudge = ModuleListJudgeFlow(&theRecipeBank.m_Module.m_vct_AZone_Bef, pCell, strDefect, nClass);

		if(nClass == GOOD_CELL)
		{
			nJudge = ModuleListJudgeFlow(&theRecipeBank.m_Module.m_vct_AZone_Must, pCell, strDefect, nClass);
		}
		if(nClass == GOOD_CELL)
		{
			nJudge = ModuleListJudgeFlow(&theRecipeBank.m_Module.m_vct_AZone_Aft, pCell, strDefect, nClass);
		}

		return nJudge;
	}
}

//kjpark 20161016 Cell Result 항목 추가
int CUnitCtrlBank::CZoneJudgeFlow( CCellInfo *pCell, CString &strDefect, CLASS_CELL &nClass )
{
	if( pCell->defaultData.m_bPGAlarm/* && (pCell->defaultCellInfomation.m_PGAlarmZone == ZONE_ID_A)*/ )
	{
		strDefect = pCell->defaultData.m_strPGAlarmName;
		nClass = REJECT_CELL;
		return CONST_JUDGE_NAME::JUDGE_PG_ALARM;
	}
	else
	{
		CONST_JUDGE_NAME::ID nJudge;
		nJudge = ModuleListJudgeFlow(&theRecipeBank.m_Module.m_vct_CZone_Bef, pCell, strDefect, nClass);

		if(nClass == GOOD_CELL)
		{
			nJudge = ModuleListJudgeFlow(&theRecipeBank.m_Module.m_vct_CZone_Must, pCell, strDefect, nClass);
		}
		if(nClass == GOOD_CELL)
		{
			nJudge = ModuleListJudgeFlow(&theRecipeBank.m_Module.m_vct_CZone_Aft, pCell, strDefect, nClass);
		}

		return nJudge;
	}
}

int CUnitCtrlBank::MCRJudgeFlow(CCellInfo *pCell, CString &strDefect, CLASS_CELL &nClass)
{
	// MCR 못읽었어도 불량. 하지만 모든 검사는 다 원래대로 한다
	// 검사 결과에 따라 불량명이 바뀐다
	if(theConfigBank.m_CIM.CELL_MCR_MODE_Check())
	{
		if(pCell->defaultData.m_bMCR_OK == FALSE)
		{
			if(nClass == GOOD_CELL)
				strDefect = TEXT_MCR_READING_FAIL_GOOD;
			else
				strDefect = TEXT_MCR_READING_FAIL_NG;
			nClass = REJECT_CELL;
			return CONST_JUDGE_NAME::JUDGE_MCR;
		}
	}

	//kjpark 20180113 Cell infomation NG 시 Job Process 로 Last Result 남는 버그 수정
	if(theProcBank.GetCimState() == CONST_CIM_STATE::CIM_REMOTE)
	{		
		if(pCell->defaultData.m_strCellInfoResult != TEXT_0)
		{
			//kjpark 20180123 cellionfomation, jobprocess 페일시 라스트 리절트 정리
			if(pCell->defaultData.m_CellInfoResult == NONE_CELL)
			{
				if(nClass == GOOD_CELL)						
					strDefect = TEXT_CELL_INFO_TIMEOUT_GOOD;
				else
					strDefect = TEXT_CELL_INFO_TIMEOUT_NG;
			}
			else if((pCell->defaultData.m_CellInfoResult == SKIP_CELL)
				&& (pCell->defaultData.m_strCellInfoResult == TIMEOUT_NG))
			{
				if(nClass == GOOD_CELL)						
					strDefect = TEXT_CELL_INFO_TIMEOUT_GOOD;
				else
					strDefect = TEXT_CELL_INFO_TIMEOUT_NG;
			}
			else //if(pCell->defaultData.m_CellInfoResult == REJECT_CELL)
			{
				if(nClass == GOOD_CELL)						
					strDefect = TEXT_CELL_INFO_FAIL_GOOD;
				else
					strDefect = TEXT_CELL_INFO_FAIL_NG;
			}			
			nClass = REJECT_CELL;
			return CONST_JUDGE_NAME::JUDGE_MCR;
		}
	}
	
	//kjpark 20171010 TMD Match 구현
	//kjpark 20180120 Cell Mixing 처리(강원호 선임) - 컨텍 NG보다 셀믹싱이 우선
	if(theConfigBank.m_Option.m_bUseTMDNameMatch)
	{
		if(pCell->defaultData.m_bMCR_OK)
		{
			if(pCell->defaultData.CellMixing != GOOD_CELL)
			{
				strDefect = pCell->defaultData.m_strCellMixing;
				nClass = REJECT_CELL;
				return CONST_JUDGE_NAME::JUDGE_CELL_MIXING;
			}
		}
	}

	if(theConfigBank.m_CIM.TRACKING_CONTROL_InCheck())
	{
		if(theProcBank.GetCimState() == CONST_CIM_STATE::CIM_REMOTE)
		{
			if(pCell->defaultData.m_nInspectInvalidType != JOB_START)
			{
				//kjpark 20180123 cellionfomation, jobprocess 페일시 라스트 리절트 정리
				if(pCell->defaultData.m_nInspectInvalidType == JOB_INVALID)
				{
					if(nClass == GOOD_CELL)						
						strDefect = TEXT_VALIDATION_FAIL_GOOD;
					else
						strDefect = TEXT_VALIDATION_FAIL_NG;
				}
				//kjpark 20180124 cellionfomation, jobprocess 페일시 라스트 리절트 정리 버그 수정
				else if(pCell->defaultData.m_nInspectInvalidType == JOB_DEFAULT)
				{
					if(nClass == GOOD_CELL)						
						strDefect = TEXT_VALIDATION_TIMEOUT_GOOD;
					else
						strDefect = TEXT_VALIDATION_TIMEOUT_NG;
				}				
				else if(pCell->defaultData.m_nInspectInvalidType == JOB_SKIP)
				{
					if(nClass == GOOD_CELL)						
						strDefect = TEXT_VALIDATION_FAIL_GOOD;
					else
						strDefect = TEXT_VALIDATION_FAIL_NG;
				}				
				nClass = REJECT_CELL;
				return CONST_JUDGE_NAME::JUDGE_MCR;
			}
		}
	}
	return CONST_JUDGE_NAME::JUDGE_MCR;

}

int CUnitCtrlBank::LastJudgeFlow(CCellInfo *pCell, CString &strDefect, CLASS_CELL &nClass)
{
	int nJudge = CONST_JUDGE_NAME::JUDGE_MCR;
	nJudge = AZoneJudgeFlow(pCell, strDefect, nClass);
	if(nClass == GOOD_CELL)
	{
		nJudge = CZoneJudgeFlow(pCell, strDefect, nClass);
	}

	// SDC 이정현프로 요청으로 MCR을 맨 나중에 체크하되 우선순위 맨 위로 선정 [11/23/2017 OSC]
	nJudge = MCRJudgeFlow(pCell, strDefect, nClass);

	return nJudge;
}

CONST_JUDGE_NAME::ID CUnitCtrlBank::ModuleListJudgeFlow( std::vector<CString> *pVector, CCellInfo *pCell, CString &strDefect, CLASS_CELL &nClass )
{
	CString strModuleName = INSP_MODULE_NONE;
	CCellDefectInfo *pDefectInfo;

	int nCount = pVector->size();
	for(int i = 0; i < nCount; i++)
	{
		strModuleName = pVector->at(i);
		pDefectInfo = pCell->GetDefectInfo(strModuleName);
		if(pDefectInfo)
		{
			if(pDefectInfo->m_Class != GOOD_CELL)
			{
				strDefect = pDefectInfo->m_DefectName;
				nClass = pDefectInfo->m_Class;
				return pDefectInfo->m_Judge;
			}
		}
	}

	strDefect = GOOD;
	nClass = GOOD_CELL;
	return CONST_JUDGE_NAME::JUDGE_MCR;

}

//kjpark 20161017 WorkTable Turn
void CUnitCtrlBank::JudgeZoneDefect(JIG_ID jig, ZONE_ID defectzone)
{
	CCellInfo* pCell;
	CCellTag tagCell;

	for (int i = 0; i < JIG_CH_MAX; i++)
	{
		tagCell = theCellBank.GetCellTag(jig, (JIG_CH)i);
		if(tagCell.IsExist())
		{
			pCell = theCellBank.GetCellInfo(tagCell);
			switch(defectzone)
			{
			case ZONE_ID_A:
				GetDefectFromJudge(defectzone, pCell, pCell->m_AZone.m_DefectName, pCell->m_AZone.m_Class);
				break;
			case ZONE_ID_C:
				GetDefectFromJudge(defectzone, pCell, pCell->m_CZone.m_DefectName, pCell->m_CZone.m_Class);
				break;
			}
		}
	}
}

//kjpark 20180120 Cell Mixing 처리(강원호 선임)
BOOL CUnitCtrlBank::GetCellSkipCheck( JIG_ID jig, JIG_CH nCh, BOOL bCheckContinue)
{
	BOOL bRet = FALSE;
	if(theCellBank.GetCellTag(jig, nCh).IsExist())
	{
		if(theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, nCh))->defaultData.m_bIsInspectionSkip)
		{
			if(bCheckContinue)
			{
				if(theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, nCh))->defaultData.CellMixing == REJECT_CELL)
				{
					return FALSE;
				}
				else
				{
					return TRUE;
				}
			}				
			return FALSE;
		}		
		bRet = TRUE;
	}	
	return bRet;
}

void CUnitCtrlBank::Product_CountUpdate( JIG_ID jig )
{
	CCellInfo *pCell;
	for (int i = 0; i < JIG_CH_MAX; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			SetProductData(jig, (JIG_CH)i);
		}
	}
}

void CUnitCtrlBank::AZoneCell_RemoveSkipCell( JIG_ID jig )
{
	// 작업자가 SKIP이라고 처리한 채널의 CellTag를 삭제한다 [9/13/2017 OSC]
	CCellTag tag;
	for (int i = 0; i < JIG_CH_MAX; i++)
	{
		tag = theCellBank.GetCellTag(jig, (JIG_CH)i);
		if(theProcBank.AZoneChannelNotUse_Check(jig, (JIG_CH)i) && tag.IsExist())
		{
			theCellBank.RemoveCellTag(jig, (JIG_CH)i);
		}
	}
}

BOOL CUnitCtrlBank::CellInfo_GetInspFinish(JIG_ID jig, JIG_CH ch /*= JIG_CH_MAX*/)
{
	// 이 셀이 검사가 끝난 셀인지 확인
	CCellInfo *pCell;
	int nStart, nEnd;

	if(ch == JIG_CH_MAX)
	{
		nStart = 0;		nEnd = ch;
	}
	else
	{
		nStart = ch;	nEnd = ch+1;
	}

	for (int i = nStart; i < nEnd; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			if(pCell->defaultData.m_LastClass == NONE_CELL)
				return FALSE;
			else
				return TRUE;
		}
	}

	return FALSE; 
}

void CUnitCtrlBank::CellLog_SetOperatorID(JIG_ID jig, JIG_CH ch /*= JIG_CH_MAX*/)
{
	CCellInfo *pCell;
	if(ch == JIG_CH_MAX)
	{
		for (int i = 0; i < JIG_CH_MAX; i++)
		{
			pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
			if(pCell)
			{
				pCell->defaultData.m_str_Operator_SSO_ID = GetMainHandler()->GetCurOperatorUserInspectorData().sID;
			}
		}
	}
	else
	{
		pCell = theCellBank.GetCellInfo(jig, ch);
		if(pCell)
		{
			pCell->defaultData.m_str_Operator_SSO_ID = GetMainHandler()->GetCurOperatorUserInspectorData().sID;
		}
	}
}

void CUnitCtrlBank::CellLog_TactTime_SetStartTime(JIG_ID jig, JIG_CH ch /*= JIG_CH_MAX*/)
{
	// 물류 : 인터페이스 끝 ~ A존 도착 시점
	// 단동 : PG On 버튼 ~ A존 도착 시점
	CCellInfo *pCell;
	int nStart, nEnd;

	if(ch == JIG_CH_MAX)
	{
		nStart = 0;		nEnd = ch;
	}
	else
	{
		nStart = ch;	nEnd = ch+1;
	}

	for (int i = nStart; i < nEnd; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			pCell->m_TactTime.SetTimeStart();
		}
	}
}

void CUnitCtrlBank::CellLog_TactTime_SetEndTime(JIG_ID jig, JIG_CH ch /*= JIG_CH_MAX*/)
{
	// 물류 : 인터페이스 끝 ~ A존 도착 시점
	// 단동 : PG On 버튼 ~ A존 도착 시점
	CCellInfo *pCell;
	int nStart, nEnd;

	if(ch == JIG_CH_MAX)
	{
		nStart = 0;		nEnd = ch;
	}
	else
	{
		nStart = ch;	nEnd = ch+1;
	}

	for (int i = nStart; i < nEnd; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			pCell->m_TactTime.SetTimeEnd();
		}
	}
}

void CUnitCtrlBank::CellLog_SetUnloadTime(JIG_ID jig, JIG_CH ch /*= JIG_CH_MAX*/)
{
	CCellInfo *pCell;
	int nStart, nEnd;

	if(ch == JIG_CH_MAX)
	{
		nStart = 0;		nEnd = ch;
	}
	else
	{
		nStart = ch;	nEnd = ch+1;
	}

	for (int i = nStart; i < nEnd; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			pCell->m_UnloadTactTime.SetTimeStart(theProcBank.m_OldUnloadTime[jig][i]);
			pCell->m_UnloadTactTime.SetTimeEnd();
			theProcBank.m_OldUnloadTime[jig][i] = pCell->m_UnloadTactTime.m_timeEnd;
		}
	}
}

void CUnitCtrlBank::CellLog_WaitTime_SetStartTime(JIG_ID jig, JIG_CH ch /*= JIG_CH_MAX*/)
{
	// 물류 : A존 도착 시점 ~ 인터페이스 시작
	// 단동 : A존 도착 시점 ~ 현재 Cell PG On
	CCellInfo *pCell;
	int nStart, nEnd;

	if(ch == JIG_CH_MAX)
	{
		nStart = 0;		nEnd = ch;
	}
	else
	{
		nStart = ch;	nEnd = ch+1;
	}

	for (int i = nStart; i < nEnd; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			if(theProcBank.m_OldWaitTime[jig][i].wYear == 0)
				GetLocalTime(&theProcBank.m_OldWaitTime[jig][i]);
		}
	}
}

void CUnitCtrlBank::CellLog_WaitTime_SetEndTime(JIG_ID jig, JIG_CH ch /*= JIG_CH_MAX*/)
{
	// 물류 : A존 도착 시점 ~ 인터페이스 시작
	// 단동 : A존 도착 시점 ~ 현재 Cell PG On
	CCellInfo *pCell;
	int nStart, nEnd;

	if(ch == JIG_CH_MAX)
	{
		nStart = 0;		nEnd = ch;
	}
	else
	{
		nStart = ch;	nEnd = ch+1;
	}

	for (int i = nStart; i < nEnd; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			if(theProcBank.m_OldWaitTime[jig][i].wYear != 0)
				pCell->m_WaitTime.SetTimeStart(theProcBank.m_OldWaitTime[jig][i]);
			pCell->m_WaitTime.SetTimeEnd();
			// 값을 사용했으면 초기화
			theProcBank.m_OldWaitTime[jig][i] = SYSTEMTIME();
		}
	}
}

void CUnitCtrlBank::CellLog_LoadingTactTime_SetStartTime(JIG_ID jig, JIG_CH ch /*= JIG_CH_MAX*/)
{
	// 물류 : 인터페이스 끝 ~ CELL_LOADING 시작
	CCellInfo *pCell;
	int nStart, nEnd;

	if(ch == JIG_CH_MAX)
	{
		nStart = 0;		nEnd = ch;
	}
	else
	{
		nStart = ch;	nEnd = ch+1;
	}

	for (int i = nStart; i < nEnd; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			pCell->m_LoadingTactTime.SetTimeStart();
		}
	}
}

void CUnitCtrlBank::CellLog_LoadingTactTime_SetEndTime(JIG_ID jig, JIG_CH ch /*= JIG_CH_MAX*/)
{
	// 물류 : 인터페이스 끝 ~ CELL_LOADING 시작
	CCellInfo *pCell;
	int nStart, nEnd;

	if(ch == JIG_CH_MAX)
	{
		nStart = 0;		nEnd = ch;
	}
	else
	{
		nStart = ch;	nEnd = ch+1;
	}

	for (int i = nStart; i < nEnd; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			pCell->m_LoadingTactTime.SetTimeEnd();
		}
	}
}

void CUnitCtrlBank::CellLog_MCRReadTime_SetStartTime(JIG_ID jig, JIG_CH ch /*= JIG_CH_MAX*/)
{
	// 셔틀 MCR 위치 도착 완료 ~ Inspection 위치 이동 시작
	CCellInfo *pCell;
	int nStart, nEnd;

	if(ch == JIG_CH_MAX)
	{
		nStart = 0;		nEnd = ch;
	}
	else
	{
		nStart = ch;	nEnd = ch+1;
	}

	for (int i = nStart; i < nEnd; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			pCell->m_MCRReadTime.SetTimeStart();
		}
	}
}

void CUnitCtrlBank::CellLog_MCRReadTime_SetEndTime(JIG_ID jig, JIG_CH ch /*= JIG_CH_MAX*/)
{
	// 셔틀 MCR 위치 도착 완료 ~ Inspection 위치 이동 시작
	CCellInfo *pCell;
	int nStart, nEnd;

	if(ch == JIG_CH_MAX)
	{
		nStart = 0;		nEnd = ch;
	}
	else
	{
		nStart = ch;	nEnd = ch+1;
	}

	for (int i = nStart; i < nEnd; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			pCell->m_MCRReadTime.SetTimeEnd();
		}
	}
}

void CUnitCtrlBank::CellLog_AZoneETCTime_SetStartTime(JIG_ID jig, JIG_CH ch /*= JIG_CH_MAX*/)
{
	// A존의 Cell Loading 외 다른 검사시간. 어짜피 점등 이후니 AFT 검사만 신경쓰자
	CCellInfo *pCell;
	int nStart, nEnd;

	if(ch == JIG_CH_MAX)
	{
		nStart = 0;		nEnd = ch;
	}
	else
	{
		nStart = ch;	nEnd = ch+1;
	}

	for (int i = nStart; i < nEnd; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			pCell->m_AZoneETCTime.SetTimeStart();
		}
	}
}

void CUnitCtrlBank::CellLog_AZoneETCTime_SetEndTime(JIG_ID jig, JIG_CH ch /*= JIG_CH_MAX*/)
{
	// A존의 Cell Loading 외 다른 검사시간. 어짜피 점등 이후니 AFT 검사만 신경쓰자
	CCellInfo *pCell;
	int nStart, nEnd;

	if(ch == JIG_CH_MAX)
	{
		nStart = 0;		nEnd = ch;
	}
	else
	{
		nStart = ch;	nEnd = ch+1;
	}

	for (int i = nStart; i < nEnd; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			pCell->m_AZoneETCTime.SetTimeEnd();
		}
	}
}

void CUnitCtrlBank::CellLog_CZoneMTPReadyTime_SetStartTime(JIG_ID jig, JIG_CH ch /*= JIG_CH_MAX*/)
{
	// 셔틀 C존 도착부터 C존 검사 시작 전까지
	CCellInfo *pCell;
	int nStart, nEnd;

	if(ch == JIG_CH_MAX)
	{
		nStart = 0;		nEnd = ch;
	}
	else
	{
		nStart = ch;	nEnd = ch+1;
	}

	for (int i = nStart; i < nEnd; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			pCell->m_MTPReadyTime.SetTimeStart();
		}
	}
}

void CUnitCtrlBank::CellLog_CZoneMTPReadyTime_SetEndTime(JIG_ID jig, JIG_CH ch /*= JIG_CH_MAX*/)
{
	// 셔틀 C존 도착부터 C존 검사 시작 전까지
	CCellInfo *pCell;
	int nStart, nEnd;

	if(ch == JIG_CH_MAX)
	{
		nStart = 0;		nEnd = ch;
	}
	else
	{
		nStart = ch;	nEnd = ch+1;
	}

	for (int i = nStart; i < nEnd; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			pCell->m_MTPReadyTime.SetTimeEnd();
		}
	}
}

void CUnitCtrlBank::CellLog_CZoneETCTime_SetStartTime(JIG_ID jig, JIG_CH ch /*= JIG_CH_MAX*/)
{
	// C존의 MTP 검사시간 이후부터 C존 AFT 검사 완료까지
	CCellInfo *pCell;
	int nStart, nEnd;

	if(ch == JIG_CH_MAX)
	{
		nStart = 0;		nEnd = ch;
	}
	else
	{
		nStart = ch;	nEnd = ch+1;
	}

	for (int i = nStart; i < nEnd; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			pCell->m_CZoneETCTime.SetTimeStart();
		}
	}
}

void CUnitCtrlBank::CellLog_CZoneETCTime_SetEndTime(JIG_ID jig, JIG_CH ch /*= JIG_CH_MAX*/)
{
	// C존의 MTP 검사시간 이후부터 C존 AFT 검사 완료까지
	CCellInfo *pCell;
	int nStart, nEnd;

	if(ch == JIG_CH_MAX)
	{
		nStart = 0;		nEnd = ch;
	}
	else
	{
		nStart = ch;	nEnd = ch+1;
	}

	for (int i = nStart; i < nEnd; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			pCell->m_CZoneETCTime.SetTimeEnd();
		}
	}
}

void CUnitCtrlBank::CellLog_Write( JIG_ID jig, JIG_CH ch /*= JIG_CH_MAX*/ )
{
	if(ch == JIG_CH_MAX)
	{
		switch(jig)
		{
		case JIG_ID_A:
			theCellBank.WriteCellLog(CELL_POS_SHUTTLE1_CH1);
			theCellBank.WriteCellLogTemp(CELL_POS_SHUTTLE1_CH1);
			break;
		case JIG_ID_B:
			theCellBank.WriteCellLog(CELL_POS_SHUTTLE2_CH1);
			theCellBank.WriteCellLogTemp(CELL_POS_SHUTTLE2_CH1);
			break;
		}
	}
	else
	{
		theCellBank.WriteCellLog(jig, ch);
		theCellBank.WriteCellLogTemp(jig, ch);
	}
}

//kjpark 20180107 신호기 로그에서 MTP 측정 값 가지고와서 셀로그에 넣기
void CUnitCtrlBank::GetMTP_Isnpection_Value(JIG_ID jig, JIG_CH ch)
{
	CCellInfo *pCell;
	pCell = theCellBank.GetCellInfo(jig, ch);
	if(pCell)
	{
		SYSTEMTIME time;
		::GetLocalTime(&time);
		if(theProcBank.m_PGData[jig][ch].ScanLog(pCell->defaultData.m_strCellID, pCell->defaultData.m_strInnerID, time))
		{
			if(theProcBank.m_PGData[jig][ch].SelectData())
			{
				theProcBank.m_PGData[jig][ch].SetValueToCellInfo(pCell);
			}
		}
		
	}
}

void CUnitCtrlBank::CIM_BZoneCellInfoRequest()
{
	CCellInfo *pCell;
	for (int i = 0; i < JIG_CH_MAX; i++)
	{
		pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_SHUTTLE1_CH1 + i));
		if(pCell)
			theSocketInterFace.m_CIM.SendCmdCellToDataPC(EQUIP_SET_CELL_INFORMATION_REQUEST, pCell);
	}
}

void CUnitCtrlBank::CIM_BZoneCellTrackIn()
{
	CCellInfo *pCell;
	for (int i = 0; i < JIG_CH_MAX; i++)
	{
		pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_SHUTTLE1_CH1 + i));
		if(pCell)
		{
			theSocketInterFace.m_CIM.SendCmdCellToDataPC(EQUIP_SET_CELL_PROCESS_START_LOAD, pCell);
			theLog[LOG_TRACKING].AddBuf(_T("CellID[%s], InnerID[%s], Track In Send"), pCell->defaultData.m_strCellID, pCell->defaultData.m_strInnerID);
		}
	}
}

void CUnitCtrlBank::CIM_AZoneCellDefectInfoRequest()
{
	CCellInfo *pCell;
	for (int i = 0; i < JIG_CH_MAX; i++)
	{
		pCell = theCellBank.GetCellInfo((CELL_POS)(CELL_POS_SHUTTLE1_CH1 + i));
		if(pCell)
		{
			if(pCell->defaultData.m_strCellID != TEXT_FAIL)
				theSocketInterFace.m_CIM.SendCmdCellToDataPC(EQUIP_SET_CELL_LOT_INFORMATION_REQUEST, pCell, _T("DEFECT"));
		}
	}
}

void CUnitCtrlBank::CIM_CellExistInMachine()
{
	CCellTag tag;
	BOOL bExist = FALSE;
	if(theConfigBank.m_System.m_bInlineMode)
	{
		if(CellTagExist(CELL_POS_SHUTTLE1_CH1, CELL_POS_SHUTTLE2_CH1))
		{
			bExist = TRUE;
		}
		else
		{
			bExist = FALSE;
		}
	}
	else
	{
		if( 
			(	CellTagExist(CELL_POS_SHUTTLE1_CH1, CELL_POS_SHUTTLE2_CH1) == FALSE	)
			||
			(	(CellLoading_RecvCheck(JIG_ID_A) == FALSE)
			&& (CellLoading_RecvCheck(JIG_ID_B) == FALSE)		)
			)
		{
			bExist = FALSE;
		}
		else
		{
			bExist = TRUE;
		}
	}

	if(bExist)
	{
		theSocketInterFace.m_CIM.SendCmdStateToDataPC(EQUIP_SET_EQUIPMENT_STATUS_CHANGE, 
			E_EQST_MATCH_CELL_EXIST, _T(""));
	}
	else
	{
		theSocketInterFace.m_CIM.SendCmdStateToDataPC(EQUIP_SET_EQUIPMENT_STATUS_CHANGE, 
			E_EQST_MATCH_CELL_NOT_EXIST, _T(""));
	}
}

void CUnitCtrlBank::CIM_CellExistInMachine( BOOL bExist )
{
	if(bExist)
	{
		theSocketInterFace.m_CIM.SendCmdStateToDataPC(EQUIP_SET_EQUIPMENT_STATUS_CHANGE, 
			E_EQST_MATCH_CELL_EXIST, _T(""));
	}
	else
	{
		theSocketInterFace.m_CIM.SendCmdStateToDataPC(EQUIP_SET_EQUIPMENT_STATUS_CHANGE, 
			E_EQST_MATCH_CELL_NOT_EXIST, TP_CODE_IDLE_RUNDOWN_RUNDOWN);
	}
}

//kjpark 20170907 Tracking CellInfomation, JobProcess 추가
void CUnitCtrlBank::CIM_CellInfoRequest(JIG_ID jig, JIG_CH nCh)
{
	CCellInfo *pCell;
	if(nCh == JIG_CH_MAX)
	{
		for (int i = 0; i < JIG_CH_MAX; i++)
		{
			pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
			if(pCell)
			{
				//kjpark 20170912 셀아이디가 정상으로 읽혀야 Cellinfomation 요청
				if(pCell->CellInfo_CheckAble() == FALSE)
					continue;
				if(pCell->defaultData.m_CellInfoResult == NONE_CELL)
				{
					theSocketInterFace.m_CIM.SendCmdCellToDataPC(EQUIP_SET_CELL_INFORMATION_REQUEST, pCell);
					theLog[LOG_TRACKING].AddBuf(_T("CellID[%s], InnerID[%s], Cell Infomation Request Send"), pCell->defaultData.m_strCellID, pCell->defaultData.m_strInnerID);
				}
			}
		}
	}
	else
	{
		pCell = theCellBank.GetCellInfo(jig, nCh);
		if(pCell)
		{
			//kjpark 20170912 셀아이디가 정상으로 읽혀야 Cellinfomation 요청
			if(pCell->CellInfo_CheckAble() == FALSE)
				return;
			if(pCell->defaultData.m_CellInfoResult == NONE_CELL)
			theSocketInterFace.m_CIM.SendCmdCellToDataPC(EQUIP_SET_CELL_INFORMATION_REQUEST, pCell);
			{
				theLog[LOG_TRACKING].AddBuf(_T("CellID[%s], InnerID[%s], Cell Infomation Request Send"), pCell->defaultData.m_strCellID, pCell->defaultData.m_strInnerID);
			}
		}
	}

}

BOOL CUnitCtrlBank::CheckCellInfomationRecive(JIG_ID jig, JIG_CH ch /*= JIG_CH_MAX*/)
{
	if(theConfigBank.m_Option.m_bUseCellInforRequest == FALSE)
		return TRUE;
	if(theProcBank.GetCimState() != CONST_CIM_STATE::CIM_REMOTE)
		return TRUE;

	CCellInfo *pCell;
	int nStart, nEnd;

	if(ch == JIG_CH_MAX)
	{
		nStart = 0;		nEnd = ch;
	}
	else
	{
		nStart = ch;	nEnd = ch+1;
	}

	for (int i = nStart; i < nEnd; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			if(pCell->CellInfo_CheckAble())
			{
				if(pCell->defaultData.m_CellInfoResult == NONE_CELL)
				{
					return FALSE;
				}
			}
		}
	}

	return TRUE;
}

//kjpark 20170907 Tracking CellInfomation, JobProcess 추가
BOOL CUnitCtrlBank::CheckCellInfomationSuccess(JIG_ID jig, JIG_CH nCh /*= JIG_CH_MAX*/)
{
	if(theConfigBank.m_Option.m_bUseCellInforRequest == FALSE)
		return TRUE;
	if(theProcBank.GetCimState() != CONST_CIM_STATE::CIM_REMOTE)
		return TRUE;

	CCellInfo *pCell;
	pCell = theCellBank.GetCellInfo(jig, nCh);
	if(pCell == NULL)
		return FALSE;
	if(pCell->CellInfo_CheckAble() == FALSE)
		return TRUE;

	if(pCell->defaultData.m_strCellInfoResult == TEXT_0)
	{
		//kjpark 20170912 Cellinfomation 결과
		pCell->defaultData.m_CellInfoResult = GOOD_CELL;
		return TRUE;
	}
	return FALSE;
}

//kjpark 20170710 CIM QUAL Jot Start, Cell Infomation Request 사용
//kjpark 20170907 Tracking CellInfomation, JobProcess 추가
BOOL CUnitCtrlBank::CheckCellJobStartRecive(JIG_ID jig, JIG_CH nCh)
{
	//Invaide
	if(theConfigBank.m_CIM.TRACKING_CONTROL_InCheck() == FALSE)
		return TRUE;
	if(theProcBank.GetCimState() != CONST_CIM_STATE::CIM_REMOTE)
		return TRUE;

	CCellInfo *pCell;
	pCell = theCellBank.GetCellInfo(jig, nCh);
	if(pCell)
	{
		if(pCell->TrackIn_CheckAble() == FALSE)
			return TRUE;

		if(pCell->defaultData.m_nInspectInvalidType == JOB_DEFAULT)
			return FALSE;
		else
			return TRUE;
	}

	return TRUE;
}

BOOL CUnitCtrlBank::CheckCellJobStartSuccess(JIG_ID jig, JIG_CH ch)
{
	//Invaide
	if(theConfigBank.m_CIM.TRACKING_CONTROL_InCheck() == FALSE)
		return TRUE;
	if(theProcBank.GetCimState() != CONST_CIM_STATE::CIM_REMOTE)
		return TRUE;

	CCellInfo *pCell;
	pCell = theCellBank.GetCellInfo(jig, ch);
	if(pCell)
	{
		if(pCell->TrackIn_CheckAble() == FALSE)
			return TRUE;
		else if(pCell->defaultData.m_nInspectInvalidType >= JOB_START)
			return TRUE;
		else
			return FALSE;
	}

	return TRUE;
}

void CUnitCtrlBank::CIM_SetCellInfoNG(JIG_ID jig, JIG_CH ch /*= JIG_CH_MAX*/)
{
	CCellInfo *pCell;
	int nStart, nEnd;

	if(ch == JIG_CH_MAX)
	{
		nStart = 0;		nEnd = ch;
	}
	else
	{
		nStart = ch;	nEnd = ch+1;
	}

	for (int i = nStart; i < nEnd; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			if( (pCell->defaultData.m_CellInfoResult != GOOD_CELL)
				&& (pCell->defaultData.m_CellInfoResult != REJECT_CELL) )
			{
				pCell->defaultData.m_CellInfoResult = SKIP_CELL;
				pCell->defaultData.m_strCellInfoResult = TIMEOUT_NG;
				CIM_SetTrackOutNG(jig, (JIG_CH)i);
			}
		}
	}
}

void CUnitCtrlBank::CIM_SetTrackOutNG(JIG_ID jig, JIG_CH ch /*= JIG_CH_MAX*/)
{
	CCellInfo *pCell;
	int nStart, nEnd;

	if(ch == JIG_CH_MAX)
	{
		nStart = 0;		nEnd = ch;
	}
	else
	{
		nStart = ch;	nEnd = ch+1;
	}

	for (int i = nStart; i < nEnd; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			if(pCell->defaultData.m_nInspectInvalidType != JOB_START)
			{
				pCell->defaultData.m_nInspectInvalidType = JOB_SKIP;
// 				pCell->defaultData.m_bIsInspectionSkip = TRUE;
			}
		}
	}
}

//kjpark 20170907 Tracking CellInfomation, JobProcess 추가
void CUnitCtrlBank::CIM_CellTrackIn(JIG_ID jig, JIG_CH nCh)
{
	CCellInfo *pCell;
	if(nCh == JIG_CH_MAX)
	{
		for (int i = 0; i < JIG_CH_MAX; i++)
		{
			pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
			if(pCell)
			{
				if(pCell->TrackIn_CheckAble() && (pCell->defaultData.m_bTrackinFinish == FALSE) )
				{
					theSocketInterFace.m_CIM.SendCmdCellToDataPC(EQUIP_SET_CELL_PROCESS_START_LOAD, pCell);
					theLog[LOG_TRACKING].AddBuf(_T("CellID[%s], InnerID[%s], Track In Send"), pCell->defaultData.m_strCellID, pCell->defaultData.m_strInnerID);
					pCell->defaultData.m_bTrackinFinish = TRUE;					
				}				
			}
		}
	}
	else
	{
		pCell = theCellBank.GetCellInfo(jig, nCh);
		if(pCell)
		{
			if(pCell->TrackIn_CheckAble() && (pCell->defaultData.m_bTrackinFinish == FALSE) )
			{
				theSocketInterFace.m_CIM.SendCmdCellToDataPC(EQUIP_SET_CELL_PROCESS_START_LOAD, pCell);
				theLog[LOG_TRACKING].AddBuf(_T("CellID[%s], InnerID[%s], Track In Send"), pCell->defaultData.m_strCellID, pCell->defaultData.m_strInnerID);
				pCell->defaultData.m_bTrackinFinish = TRUE;					
			}			
		}
	}
}

void CUnitCtrlBank::CIM_CellCimJudge(JIG_ID jig, JIG_CH ch /*= JIG_CH_MAX*/)
{
	CCellInfo *pCell;
	int nStart, nEnd;

	if(ch == JIG_CH_MAX)
	{
		nStart = 0;		nEnd = ch;
	}
	else
	{
		nStart = ch;	nEnd = ch+1;
	}

	for (int i = nStart; i < nEnd; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			pCell->CimJudge_Judge();
		}
	}
}

void CUnitCtrlBank::CIM_CellLoadingStop(JIG_ID jig, JIG_CH ch /*= JIG_CH_MAX*/)
{
	// 리트라이 해야 할 것들을 전부 LOSS로 바꾸고 Inspection End 보고를 다시 한다
	CCellInfo *pCell;
	int nStart, nEnd;

	if(ch == JIG_CH_MAX)
	{
		nStart = 0;		nEnd = ch;
	}
	else
	{
		nStart = ch;	nEnd = ch+1;
	}

	for (int i = nStart; i < nEnd; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			if(pCell->defaultData.m_strCIMJudge == CIM_JUDGE_RETEST)
			{
				pCell->defaultData.m_strCIMJudge = CIM_JUDGE_LOSS;
				pCell->defaultData.m_bRetryAble = FALSE;

				CIM_CellAllInspectionEnd(jig, (JIG_CH)i);

				theLog[LOG_RETRY].AddBuf(_T("%cJIG %dCH InnerID[%s] CellID[%s] Retry Canceled. Because Loading Stop"), 
					pCell->defaultData.m_JigId+_T('A'),
					ch+1,
					pCell->defaultData.m_strInnerID,
					pCell->defaultData.m_strCellID);
			}
		}
		// PDT가 걍 뒤로 보내줘야 함 [1/8/2018 OSC]
		theProcBank.RetryCellInfo_AllTrackOut(jig, (JIG_CH)i);
	}
}

//kjpark 20170907 Tracking CellInfomation, JobProcess 추가
void CUnitCtrlBank::CIM_CellAllInspectionEnd(JIG_ID jig, JIG_CH nCh)
{
	CCellInfo *pCell;
	int nStart, nEnd;

	if(nCh == JIG_CH_MAX)
	{
		nStart = 0;		nEnd = nCh;
	}
	else
	{
		nStart = nCh;	nEnd = nCh+1;
	}

	for (int i = nStart; i < nEnd; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			if(pCell->InspectionEnd_CheckAble() == FALSE) 
				continue;
			theSocketInterFace.m_CIM.SendCmdCellToDataPC(EQUIP_SET_CELL_ENDINS, pCell);
		}
	}
}

//kjpark 20170907 Tracking CellInfomation, JobProcess 추가
void CUnitCtrlBank::CIM_CellTrackOut(JIG_ID jig, JIG_CH nCh)
{
	if(theProcBank.GetCimState() != CONST_CIM_STATE::CIM_REMOTE)
	{
		theLog[LOG_TRACKING].AddBuf(_T("Offline, Track Out Cancel"));
		return;
	}

	CCellInfo *pCell;
	int nStart, nEnd;

	if(nCh == JIG_CH_MAX)
	{
		nStart = 0;		nEnd = nCh;
	}
	else
	{
		nStart = nCh;	nEnd = nCh+1;
	}

	for (int i = nStart; i < nEnd; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			if(pCell->defaultData.m_bTrackOutFinish)
				continue;
			if(pCell->TrackOut_CheckAble()) 
			{
				if(pCell->TrackIn_CancelAble()) 
				{
					theSocketInterFace.m_CIM.SendCmdCellToDataPC(EQUIP_SET_CELL_PROCESS_COMPLETE_UNLOAD, pCell, CIM_JUDGE_OUT);
					theLog[LOG_TRACKING].AddBuf(_T("CellID[%s], InnerID[%s], Track Out Send [O]"), pCell->defaultData.m_strCellID, pCell->defaultData.m_strInnerID);
				}
				else
				{
					theSocketInterFace.m_CIM.SendCmdCellToDataPC(EQUIP_SET_CELL_PROCESS_COMPLETE_UNLOAD, pCell);
					theLog[LOG_TRACKING].AddBuf(_T("CellID[%s], InnerID[%s], Track Out Send"), pCell->defaultData.m_strCellID, pCell->defaultData.m_strInnerID);
				}
			}
		}
	}
}


void CUnitCtrlBank::CIM_CellRetryCheck( JIG_ID jig, JIG_CH nCh /*= JIG_CH_MAX*/ )
{
	// 리트라이 셀인지 확인해서 맞으면 관련 정보 넘겨받는다 [9/27/2017 OSC]
	CCellInfo *pCell;
	if(nCh == JIG_CH_MAX)
	{
		for (int i = 0; i < JIG_CH_MAX; i++)
		{
			pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
			if(pCell)
			{
				if(theProcBank.RetryCellInfo_Find(jig, (JIG_CH)i, pCell->defaultData.m_strCellID))
				{
					pCell->defaultData.m_bRetryAB = TRUE;
					pCell->defaultData.m_strABRule = AB_RULE_AB;
					pCell->defaultData.m_strAResult = theProcBank.m_pRetryCell->defaultData.m_strLastResult;
					pCell->defaultData.m_nInspectInvalidType = theProcBank.m_pRetryCell->defaultData.m_nInspectInvalidType;
					pCell->defaultData.m_bTrackinFinish = theProcBank.m_pRetryCell->defaultData.m_bTrackinFinish;
					pCell->defaultData.m_strCellInfoResult = theProcBank.m_pRetryCell->defaultData.m_strCellInfoResult;
					pCell->defaultData.m_CellInfoResult = theProcBank.m_pRetryCell->defaultData.m_CellInfoResult;
					theLog[LOG_RETRY].AddBuf(_T("%cJIG %dCH InnerID[%s] CellID[%s] Retry Start"), 
						pCell->defaultData.m_JigId+_T('A'),
						pCell->defaultData.m_JigCh+1,
						pCell->defaultData.m_strInnerID,
						pCell->defaultData.m_strCellID);
				}
			}
		}
	}
	// 리트라이 체크 후에 자기 자신 채널에 아직도 리트라이 카운트가 남아있다면 
// 	// 리트라이 안하고 그냥 빠져나갔을 경우이므로 걍 TrackOut L 보고해버린다
// 	theProcBank.RetryCellInfo_AllTrackOut(jig, JIG_CH_1);
// 	theProcBank.RetryCellInfo_AllTrackOut(jig, JIG_CH_2);
}

void CUnitCtrlBank::SendPGMessage(CString strCommand, JIG_ID jig, CString strExtraData, BOOL bIgnoreInspSkip /*= FALSE*/)
{
	if(theProcBank.m_bDryRunMode)
		return;

	for (int i = 0; i < JIG_CH_MAX; i++)
	{
		if( (strCommand == RESET) || (strCommand == SET_ZONE_A) )
		{
			theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(strCommand, jig,(JIG_CH)i, strExtraData);
		}
		else if(theCellBank.GetCellTag(jig, (JIG_CH)i).IsExist())
		{
			CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)i));
			if(bIgnoreInspSkip)
			{
				if(pCell->defaultData.m_bPGAlarm == FALSE)
				{
					theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(strCommand, jig,(JIG_CH)i, strExtraData);
				}
			}
			else
			{
				if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
				{
					theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(strCommand, jig,(JIG_CH)i, strExtraData);
				}
			}
		}
	}
}

void CUnitCtrlBank::SendPGMessage(CString strCommand, JIG_ID jig, JIG_CH nCh, CString strExtraData, BOOL bIgnoreInspSkip /*= FALSE*/)
{
	if(theProcBank.m_bDryRunMode)
		return;

	if( (strCommand == RESET) || (strCommand == SET_ZONE_B) )
	{
		theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(strCommand, jig,nCh, strExtraData);
	}
	else if(theCellBank.GetCellTag(jig, nCh).IsExist())
	{
		if(bIgnoreInspSkip)
		{
			CCellInfo *pCell = theCellBank.GetCellInfo(jig, nCh);
			if(pCell->defaultData.m_bPGAlarm == FALSE)
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(strCommand, jig,nCh, strExtraData);
		}
		else
		{
			if(theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, nCh))->defaultData.m_bIsInspectionSkip == FALSE)
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(strCommand, jig,nCh, strExtraData);
		}
	}
}

BOOL CUnitCtrlBank::GetMCRReadFinish( JIG_ID jig, JIG_CH channel)
{
	if(theProcBank.m_bDryRunMode)
		return TRUE;

	if(channel == JIG_CH_MAX)
	{
		for (int i = 0; i < JIG_CH_MAX; i++)
		{
			CCellInfo *pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
			if(pCell)
			{
				if(pCell->defaultData.m_bMCR_OK == FALSE)
					return FALSE;
			}
		}
		return TRUE;
	}
	else
	{
		CCellInfo *pCell = theCellBank.GetCellInfo(jig, (JIG_CH)channel);
		if(pCell)
		{
			if(pCell->defaultData.m_bMCR_OK == FALSE)
				return FALSE;
		}
		return TRUE;
	}
}
	

void CUnitCtrlBank::SetZoneEnd( JIG_ID jig, ZONE_ID zone )
{
	theProcBank.m_bIsZoneEnd[jig][zone] = TRUE;
	// Zone이 끝나면 SetZone을 Reset 시킨다 [9/8/2017 OSC]
	theProcBank.m_bIsSetZone[jig][zone] = FALSE;
}

BOOL CUnitCtrlBank::GetZoneEnd( JIG_ID jig, ZONE_ID zone )
{
	return theProcBank.m_bIsZoneEnd[jig][zone];
}

void CUnitCtrlBank::ResetZoneEnd( JIG_ID jig )
{
	for(int i = 0; i < ZONE_ID_MAX; i++)
		theProcBank.m_bIsZoneEnd[jig][i] = FALSE;
}

void CUnitCtrlBank::AZoneCellData_Create( JIG_ID jig, JIG_CH ch )
{
	CCellInfo* pCell;
	CTime time = CTime::GetCurrentTime();
	CString strInnerID;

	CELL_POS pos = (CELL_POS)(CELL_POS_SHUTTLE1_CH1 + (jig*JIG_CH_MAX) + ch);
	pCell = theCellBank.GetCellInfo(jig, ch);
	if(pCell)
	{
		// 리트라이 가능한 셀은 리스트에 추가해놓는다.  [9/29/2017 OSC]
		theProcBank.RetryCellInfo_Add(jig, pCell);
	}
	theCellBank.CreateCellInfo(pos);
	pCell = theCellBank.GetCellInfo(jig, ch);

	if(pCell)
	{
		int nFullCh = ((jig*JIG_CH_MAX) + ch)+1;
		pCell->defaultData.m_strInnerID.Format(_T("%d%02d%02d%02d"), nFullCh, time.GetHour(), time.GetMinute(), time.GetSecond());

// 		// 리트라이 CIM 검수용 [9/30/2017 OSC]
// 		if(strInnerID.IsEmpty() == FALSE)
// 	 		pCell->defaultCellInfomation.m_strInnerID = strInnerID;
// 		strInnerID.Empty();
		pCell->defaultData.m_JigId = (JIG_ID)(JIG_ID_A + jig);
		pCell->defaultData.m_JigCh = ch;

		// 검사 안하도록 미리 NG 처리 [9/16/2017 OSC]
		if(theProcBank.m_bDryRunMode || theProcBank.AZoneCellNG_Check(jig, ch))
		{
			pCell->defaultData.m_bIsInspectionSkip = TRUE;
		}

		theUnitStatusBank.SetCellInfo(jig, ch, pCell);
	}
}

void CUnitCtrlBank::InlineCellData_Create(JIG_ID jig, JIG_CH ch, CString strCellID)
{
	CELL_POS pos = (CELL_POS)(CELL_POS_SHUTTLE1_CH1 + (jig*JIG_CH_MAX) + ch);
	CTime time = CTime::GetCurrentTime();
	theCellBank.CreateCellInfo(pos);
	CCellInfo *pCell = theCellBank.GetCellInfo(jig, ch);

	if(pCell)
	{
		int nFullCh = ((jig*JIG_CH_MAX) + ch)+1;
		pCell->defaultData.m_strInnerID.Format(_T("%d%02d%02d%02d"), nFullCh, time.GetHour(), time.GetMinute(), time.GetSecond());

		pCell->defaultData.m_JigId = (JIG_ID)(JIG_ID_A + jig);
		pCell->defaultData.m_JigCh = ch;
		// PDT에서 아직 대응 안되서 주석 [12/19/2017 OSC]
// 		pCell->defaultData.m_strCellID = strCellID;
// 		if(strCellID.GetLength() > 15)
// 		{
// 			pCell->defaultData.m_bMCR_OK = TRUE;
// 			pCell->defaultData.m_strReadUnitMCR = _T("ROBOT");
// 		}

		// 검사 안하도록 미리 NG 처리 [9/16/2017 OSC]
		if(theProcBank.m_bDryRunMode || theProcBank.AZoneCellNG_Check(jig, ch))
		{
			pCell->defaultData.m_bIsInspectionSkip = TRUE;
		}

		theUnitStatusBank.SetCellInfo(jig, ch, pCell);

	}
}

void CUnitCtrlBank::InlineCellData_Remove( JIG_ID jig, JIG_CH ch )
{
	CCellInfo *pCell = theCellBank.GetCellInfo(jig, ch);

	if(pCell)
	{
		//theProcBank.RetryCellInfo_Remove(jig, ch);
		// Retry 가능한 셀은 저장해놓는다
		theProcBank.RetryCellInfo_Add(jig, pCell);
		theCellBank.RemoveCellTag(jig, ch);
	}
}

void CUnitCtrlBank::AZoneCellSkip_Reset( JIG_ID jig )
{
	// Cell NG 설정된 것은 한바퀴 갔다 오면 자동 해제해줘야 한다. Not Use는 계속 유지 [9/16/2017 OSC]
	if(theProcBank.AZoneChannelNotUse_Check(jig, JIG_CH_1) == FALSE)
		theProcBank.AZoneCellNG_OnOff(jig, JIG_CH_1, FALSE);
}

void CUnitCtrlBank::PatternReset_Send(JIG_ID jig, JIG_CH ch)
{
	if(ch == JIG_CH_MAX)
	{
		for (int i = 0; i < JIG_CH_MAX; i++)
		{
			theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(RESET, jig,(JIG_CH)i);
		}
	}
	else
	{
		theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(RESET, jig, ch);
	}
}

void CUnitCtrlBank::AZone_SetTimeStart( JIG_ID jig )
{
	// 물류 : 인터페이스 끝 시점
	// 단동 : PG On 버튼 누른 시점

	for (int i = 0; i < JIG_CH_MAX; i++)
	{
		theProcBank.m_AZone[jig][i].SetTimeStart();
// 		CCellInfo *pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
// 		if(pCell)
// 			pCell->m_AZone.SetTimeStart();
	}
}

void CUnitCtrlBank::AZone_SetTimeEnd( JIG_ID jig )
{
	// A존 AFT 검사 완료 시점
	for (int i = 0; i < JIG_CH_MAX; i++)
	{
		CCellInfo *pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			pCell->m_AZone.m_timeStart = theProcBank.m_AZone[jig][i].m_timeStart;
			pCell->m_AZone.m_RecvSetZone = theProcBank.m_AZone[jig][i].m_RecvSetZone;
			theProcBank.m_AZone[jig][i].Init();
			pCell->m_AZone.SetTimeEnd();
		}
	}
}

void CUnitCtrlBank::AZone_SetTimeWait( JIG_ID jig )
{
	// A존 검사 끝 ~ 셔틀 이동 전
	for (int i = 0; i < JIG_CH_MAX; i++)
	{
		CCellInfo *pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
			pCell->m_AZone.SetTimeWait();
	}
}

//kjpark 20170912 MCR 위치에따른 택타임 추가
void CUnitCtrlBank::AZonetoMCR_SetTimeStart( JIG_ID jig )
{
	for (int i = 0; i < JIG_CH_MAX; i++)
	{
		CCellInfo *pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
			pCell->m_AZonetoMCRZone.SetTimeStart();
	}
}

//kjpark 20170912 MCR 위치에따른 택타임 추가
void CUnitCtrlBank::AZonetoMCR_SetTimeEnd( JIG_ID jig )
{
	for (int i = 0; i < JIG_CH_MAX; i++)
	{
		CCellInfo *pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
			pCell->m_AZonetoMCRZone.SetTimeEnd();
	}
}

//kjpark 20170912 MCR 위치에따른 택타임 추가
void CUnitCtrlBank::MCRtoCZoneSetTimeStart( JIG_ID jig )
{
	for (int i = 0; i < JIG_CH_MAX; i++)
	{
		CCellInfo *pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
			pCell->m_MCRZonetoCZone.SetTimeStart();
	}
}

//kjpark 20170912 MCR 위치에따른 택타임 추가
void CUnitCtrlBank::MCRtoCZoneSetTimeEnd( JIG_ID jig )
{
	for (int i = 0; i < JIG_CH_MAX; i++)
	{
		CCellInfo *pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
			pCell->m_MCRZonetoCZone.SetTimeEnd();
	}
}

void CUnitCtrlBank::CZone_SetTimeStart( JIG_ID jig )
{
	for (int i = 0; i < JIG_CH_MAX; i++)
	{
		CCellInfo *pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
			pCell->m_CZone.SetTimeStart();
	}
}

void CUnitCtrlBank::CZone_SetTimeEnd( JIG_ID jig )
{
	// C존 검사 끝나는 시간
	for (int i = 0; i < JIG_CH_MAX; i++)
	{
		CCellInfo *pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
			pCell->m_CZone.SetTimeEnd();
	}
}

void CUnitCtrlBank::CZone_SetTimeWait( JIG_ID jig )
{
	// 셔틀 A존으로 이동 시작 시간
	for (int i = 0; i < JIG_CH_MAX; i++)
	{
		CCellInfo *pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
			pCell->m_CZone.SetTimeWait();
	}
}

//kjpark 20170912 MCR 위치에따른 택타임 추가
void CUnitCtrlBank::CZonetoAZone_SetTimeStart( JIG_ID jig )
{
	for (int i = 0; i < JIG_CH_MAX; i++)
	{
		CCellInfo *pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
			pCell->m_CZonetoAZone.SetTimeStart();
	}
}

//kjpark 20170912 MCR 위치에따른 택타임 추가
void CUnitCtrlBank::CZonetoAZone_SetTimeEnd( JIG_ID jig )
{
	for (int i = 0; i < JIG_CH_MAX; i++)
	{
		CCellInfo *pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
			pCell->m_CZonetoAZone.SetTimeEnd();
	}
}

void CUnitCtrlBank::SetZoneA_Send( JIG_ID jig )
{
	for (int i = 0; i < JIG_CH_MAX; i++)
	{
// 		CCellInfo *pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
// 		if(pCell)
// 		{
// 			if( (pCell->defaultCellInfomation.m_bIsInspectionSkip == FALSE) && (pCell->m_AZone.m_RecvSetZone == FALSE) )
// 			{
// 				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(SET_ZONE_A, jig, (JIG_CH)i);
// 			}
// 			pCell->m_AZone.SetTimeStart();
// 		}

		if( (theProcBank.AZoneChannelNotUse_Check(jig, (JIG_CH)i) == FALSE) && (theProcBank.m_AZone[jig][i].m_RecvSetZone == FALSE) )
		{
			theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(SET_ZONE_A, jig, (JIG_CH)i);
		}

	}
}

BOOL CUnitCtrlBank::SetZoneA_Check( JIG_ID jig )
{
	for (int i = 0; i < JIG_CH_MAX; i++)
	{
		if(theProcBank.AZoneChannelNotUse_Check(jig, (JIG_CH)i) == FALSE)
		{
			if( theProcBank.m_AZone[jig][i].m_RecvSetZone == FALSE )
				return FALSE;
		}

// 		CCellInfo *pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
// 		if(pCell)
// 		{
// 			if(pCell->defaultCellInfomation.m_bIsInspectionSkip == FALSE)
// 			{
// 				if(pCell->m_AZone.m_RecvSetZone == FALSE)
// 					return FALSE;
// 			}
// 		}
	}
	return TRUE;
}

void CUnitCtrlBank::SetZoneA_TimeOut( JIG_ID jig )
{
	for (int i = 0; i < JIG_CH_MAX; i++)
	{
		theProcBank.m_AZone[jig][i].m_RecvSetZone = TRUE;

// 		CCellInfo *pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
// 		if(pCell)
// 		{
// 			pCell->m_AZone.m_RecvSetZone = TRUE;
// 		}
	}
}

BOOL CUnitCtrlBank::AZoneDefect_GoodCheck(JIG_ID jig, JIG_CH channel)
{
	if(theProcBank.m_bDryRunMode)
		return TRUE;

	CCellTag	tag;
	CCellInfo* pCell;
	tag = theCellBank.GetCellTag(jig, channel);
	if ( tag.IsExist() == FALSE )
		return FALSE;

	pCell = theCellBank.GetCellInfo(tag);
	return pCell->m_AZone.m_Class == GOOD_CELL ? TRUE:FALSE;
}

void CUnitCtrlBank::SetZoneC_Send( JIG_ID jig )
{
	for (int i = 0; i < JIG_CH_MAX; i++)
	{
		CCellInfo *pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			if( (pCell->defaultData.m_bIsInspectionSkip == FALSE) && (pCell->m_CZone.m_RecvSetZone == FALSE) )
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_CZone.m_strCommand, jig, (JIG_CH)i);
			}
			pCell->m_CZone.SetTimeStart();
		}
	}
}

BOOL CUnitCtrlBank::SetZoneC_Check( JIG_ID jig )
{
	for (int i = 0; i < JIG_CH_MAX; i++)
	{
		CCellInfo *pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
			{
				if(pCell->m_CZone.m_RecvSetZone == FALSE)
					return FALSE;
			}
		}
	}
	return TRUE;
}

void CUnitCtrlBank::SetZoneC_TimeOut( JIG_ID jig )
{
	for (int i = 0; i < JIG_CH_MAX; i++)
	{
		CCellInfo *pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			pCell->m_CZone.m_RecvSetZone = TRUE;
		}
	}
}

//kjpark 20161018 TMD_INFO 추가
void CUnitCtrlBank::TMD_INFO_Send(JIG_ID jig, JIG_CH channel /*= JIG_CH_MAX*/ )
{
	CCellInfo* pCell;

	if(channel == JIG_CH_MAX)
	{
		for (int i = 0; i < JIG_CH_MAX; i++)
		{
			pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
			if(pCell)
			{
				if(pCell->defaultData.m_bReceive_TMD_Info == TMD_INFO_NOT_RECEIVE)
				{
					if(pCell->defaultData.m_strCellID == TEXT_FAIL)
						theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(TMD_INFO, jig,(JIG_CH)i, pCell->defaultData.m_strInnerID);
					else
						theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(TMD_INFO, jig,(JIG_CH)i, pCell->defaultData.m_strCellID);
				}
			}
		}
	}
	else
	{
		pCell = theCellBank.GetCellInfo(jig, channel);

		if(pCell)
		{
			if(pCell->defaultData.m_bReceive_TMD_Info == TMD_INFO_NOT_RECEIVE)
			{
				if(pCell->defaultData.m_strCellID == TEXT_FAIL)
					theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(TMD_INFO, jig, channel, pCell->defaultData.m_strInnerID);
				else
					theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(TMD_INFO, jig, channel, pCell->defaultData.m_strCellID);
			}
		}
	}
}

//kjpark 20180113 Tmd Version 추가
void CUnitCtrlBank::Host_Version_Send(JIG_ID jig, JIG_CH channel /*= JIG_CH_MAX*/ )
{
	if(theConfigBank.m_Option.m_bUseHostVersionSend == FALSE)
		return;

	CCellInfo* pCell;

	if(channel == JIG_CH_MAX)
	{
		for (int i = 0; i < JIG_CH_MAX; i++)
		{
			pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
			if(pCell)
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(HOST_VER, jig,(JIG_CH)i);
			}
		}
	}
	else
	{
		pCell = theCellBank.GetCellInfo(jig, channel);

		if(pCell)
		{
			theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(HOST_VER, jig, channel);
		}
	}
}

//kjpark 20180113 Tmd Version 추가
void CUnitCtrlBank::Client_Version_Send(JIG_ID jig, JIG_CH channel /*= JIG_CH_MAX*/ )
{
	if(theConfigBank.m_Option.m_bUseHostVersionSend == FALSE)
		return;

	CCellInfo* pCell;

	if(channel == JIG_CH_MAX)
	{
		for (int i = 0; i < JIG_CH_MAX; i++)
		{
			pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
			if(pCell)
			{
				theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(CLIENT_VER, jig,(JIG_CH)i);
			}
		}
	}
	else
	{
		pCell = theCellBank.GetCellInfo(jig, channel);

		if(pCell)
		{
			theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(CLIENT_VER, jig, channel);
		}
	}
}

//kjpark 20161018 TMD_INFO 추가
BOOL CUnitCtrlBank::TMD_INFO_Check(JIG_ID jig, JIG_CH channel)
{
	CCellInfo* pCell;

	pCell = theCellBank.GetCellInfo(jig, channel);

	if(pCell == NULL)
		return FALSE;

	if(pCell->defaultData.m_bReceive_TMD_Info >= TMD_INFO_RECEIVE)
		return TRUE;

	return FALSE;
}

void CUnitCtrlBank::TMD_INFO_Timeout(JIG_ID jig)
{
	CCellTag	tag;
	CCellInfo* pCell;

	//kjaprk 20161114 zone 상태에 따라 JIG ID 반환하여 셀테그 얻어오기
	for(int i = 0; i < JIG_CH_MAX; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);

		if(pCell == NULL)
			return;

		if(pCell->defaultData.m_bReceive_TMD_Info == TMD_INFO_NOT_RECEIVE)
		{
			pCell->defaultData.m_bReceive_TMD_Info = TMD_INFO_DOWN;
		}
	}
}

//kjpark 20171010 TMD March 구현
void CUnitCtrlBank::CellMixingBin2Input(JIG_ID jig, JIG_CH channel )
{
	theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, channel))->defaultData.m_strCellMixing = TEXT_CELL_MIXING;
	theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, channel))->defaultData.CellMixing = REJECT_CELL;
	theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, channel))->defaultData.m_bIsInspectionSkip = TRUE;
}

void CUnitCtrlBank::CellLoading_SetStartTime( JIG_ID jig, JIG_CH channel )
{
	CCellInfo* pCell;
	pCell = theCellBank.GetCellInfo_CellLoading(jig, channel);
	if(pCell)
	{
		pCell->m_CellLoading.SetTimeStart();
	}
}

void CUnitCtrlBank::CellLoading_SetEndTime(JIG_ID jig, JIG_CH ch /*= JIG_CH_MAX*/)
{
	// 물류 : 인터페이스 끝 ~ CELL_LOADING 시작
	CCellInfo *pCell;
	int nStart, nEnd;

	if(ch == JIG_CH_MAX)
	{
		nStart = 0;		nEnd = ch;
	}
	else
	{
		nStart = ch;	nEnd = ch+1;
	}

	for (int i = nStart; i < nEnd; i++)
	{
		pCell = theCellBank.GetCellInfo_CellLoading(jig, (JIG_CH)i);
		if(pCell)
		{
			pCell->m_CellLoading.SetTimeEnd();
		}
	}
}

void CUnitCtrlBank::CellLoading_Send( JIG_ID jig, JIG_CH channel, BOOL bUseInnerID)
{
	CCellInfo* pCell;
	CCellTag tagCell;

	pCell = theCellBank.GetCellInfo_CellLoading(jig, channel);
	if( (pCell->defaultData.m_strCellID == TEXT_FAIL) || bUseInnerID )
		theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_CellLoading.m_strCommand, jig, (JIG_CH)channel, pCell->defaultData.m_strInnerID);
	else
		theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_CellLoading.m_strCommand, jig, (JIG_CH)channel, pCell->defaultData.m_strCellID);
	pCell->m_CellLoading.m_CellLoadingSend = TRUE;
}

BOOL CUnitCtrlBank::CellLoading_SendCheck(JIG_ID jig, JIG_CH channel)
{
	// CELL_LOADING을 날렸는지 확인. 셀이 없으면 TRUE
	CCellInfo* pCell;
	pCell = theCellBank.GetCellInfo_CellLoading(jig, channel);
	if(pCell)
	{
		return pCell->m_CellLoading.m_CellLoadingSend;
	}
	return TRUE;
}

void CUnitCtrlBank::CellLoading_InitInfo(JIG_ID jig, JIG_CH channel, BOOL bResultOnly)
{
	CCellInfo* pCell;
	pCell = theCellBank.GetCellInfo_CellLoading(jig, channel);

	if(bResultOnly)
	{
		pCell->m_CellLoading.m_Class = NONE_CELL;
		pCell->m_CellLoading.m_DefectName.Empty();
	}
	else
	{
		pCell->Init();
		pCell->defaultData.m_JigId = jig;
		pCell->defaultData.m_JigCh = channel;
		int nFullCh = ((jig*JIG_CH_MAX) + channel)+1;
		CTime time = CTime::GetCurrentTime();
		pCell->defaultData.m_strInnerID.Format(_T("CONTACT_%d%02d%02d%02d"), nFullCh, time.GetHour(), time.GetMinute(), time.GetSecond());
	}

	pCell->defaultData.m_bIsInspectionSkip = FALSE;
}

BOOL CUnitCtrlBank::CellLoading_RecvCheck(JIG_ID jig, JIG_CH ch /*= JIG_CH_MAX*/)
{
	if(theProcBank.m_bDryRunMode)
		return TRUE;

	CCellInfo *pCell;
	int nStart, nEnd;

	if(ch == JIG_CH_MAX)
	{
		nStart = 0;		nEnd = ch;
	}
	else
	{
		nStart = ch;	nEnd = ch+1;
	}

	for (int i = nStart; i < nEnd; i++)
	{
		pCell = theCellBank.GetCellInfo_CellLoading(jig, (JIG_CH)i);
		if(pCell)
		{
			if(pCell->m_CellLoading.m_Class == NONE_CELL)
				return FALSE;
		}
	}

	return TRUE;
}

BOOL CUnitCtrlBank::CellLoading_GoodCheck( JIG_ID jig, JIG_CH channel )
{
	if(theProcBank.m_bDryRunMode)
		return TRUE;

	CCellInfo* pCell;
	pCell = theCellBank.GetCellInfo_CellLoading(jig, channel);
	return pCell->m_CellLoading.m_Class == GOOD_CELL ? TRUE:FALSE;
}

void CUnitCtrlBank::CellLoading_TimeOut(JIG_ID jig)
{
	// 아직까지 판정이 없는 것들은 전부 NG판정해 버린다 [12/12/2016 OSC]
	CCellInfo* pCell;
	for(int i = 0; i < JIG_CH_MAX; i++)
	{
		pCell = theCellBank.GetCellInfo_CellLoading(jig, (JIG_CH)i);
		if(pCell)
		{
			if(pCell->m_CellLoading.m_Class == NONE_CELL)
			{
				pCell->m_CellLoading.m_Class = REJECT_CELL;
				//pCell->m_CellLoading.m_DefectName = pCell->m_CellLoading.m_strCommand + TEXT_DEFECT_TIME_OUT;
				//kjpark 20180122 CELL Loading TIMEOUT을 NG 로 변경
				pCell->m_CellLoading.m_DefectName = pCell->m_CellLoading.m_strCommand + TEXT_DEFECT_NG;
				pCell->defaultData.m_bIsInspectionSkip = TRUE;
			}
		}
	}
}

void CUnitCtrlBank::CellLoading_CopyInfo(JIG_ID jig, JIG_CH channel)
{
	CCellInfo *pCell, *pCellCellLoading;
	pCell = theCellBank.GetCellInfo(jig, channel);
	pCellCellLoading = theCellBank.GetCellInfo_CellLoading(jig, channel);
	if(pCell)
	{
		pCell->m_CellLoading = pCellCellLoading->m_CellLoading;
		if(theProcBank.AZoneCellNG_Check(jig, channel) == FALSE)
		{
			pCell->defaultData.m_bIsInspectionSkip = pCellCellLoading->defaultData.m_bIsInspectionSkip;
		}
	}
}

void CUnitCtrlBank::MTPWrite_Send( JIG_ID jig, JIG_CH channel /* = JIG_CH_MAX*/ )
{
	if(theProcBank.m_bDryRunMode)
		return;

	CString strCellID;
	if(channel == JIG_CH_MAX)
	{
		for (int i = 0; i < JIG_CH_MAX; i++)
		{
			if(theCellBank.GetCellTag(jig, (JIG_CH)i).IsExist())
			{
				CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)i));
				if(pCell->m_MTPWrite.IsStarted() == FALSE)
				{
					if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
					{
						if(pCell->defaultData.m_bMCR_OK)
							strCellID = pCell->defaultData.m_strCellID;
						else
							strCellID = pCell->defaultData.m_strInnerID;
						pCell->m_MTPWrite.SetTimeStart();
						theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_MTPWrite.m_strCommand, jig, (JIG_CH)i, strCellID);
					}
					else
					{
						pCell->m_MTPWrite.m_Class = SKIP_CELL;
						pCell->m_MTPWrite.m_DefectName = SKIP;
					}
				}
			}
		}
	}
	else
	{
		if(theCellBank.GetCellTag(jig, (JIG_CH)channel).IsExist())
		{
			CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)channel));
			if(pCell->m_MTPWrite.IsStarted() == FALSE)
			{
				if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
				{
					if(pCell->defaultData.m_bMCR_OK)
						strCellID = pCell->defaultData.m_strCellID;
					else
						strCellID = pCell->defaultData.m_strInnerID;
					pCell->m_MTPWrite.SetTimeStart();
					theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_MTPWrite.m_strCommand, jig, (JIG_CH)channel, strCellID);
				}
				else
				{
					pCell->m_MTPWrite.m_Class = SKIP_CELL;
					pCell->m_MTPWrite.m_DefectName = SKIP;
				}
			}
		}
	}
	
}

BOOL CUnitCtrlBank::MTPWrite_IsStarted(JIG_ID jig, JIG_CH ch /*= JIG_CH_MAX*/)
{
	// 커맨드를 이미 날렸는지 체크
	CCellInfo *pCell;
	int nStart, nEnd;

	if(ch == JIG_CH_MAX)
	{
		nStart = 0;		nEnd = ch;
	}
	else
	{
		nStart = ch;	nEnd = ch+1;
	}

	for (int i = nStart; i < nEnd; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			if(pCell->m_MTPWrite.IsStarted() == FALSE)
				return FALSE;
		}
	}
	return TRUE;
}

BOOL CUnitCtrlBank::MTPWrite_Check( JIG_ID jig, JIG_CH ch /*= JIG_CH_MAX*/ )
{
	CCellInfo *pCell;
	int nStart, nEnd;

	if(ch == JIG_CH_MAX)
	{
		nStart = 0;		nEnd = ch;
	}
	else
	{
		nStart = ch;	nEnd = ch+1;
	}

	for (int i = nStart; i < nEnd; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			if(pCell->m_MTPWrite.m_Class == NONE_CELL)
				return FALSE;
		}
	}
	return TRUE;
}

void CUnitCtrlBank::MTPWrite_TimeOut( JIG_ID jig )
{
	// 아직까지 판정이 없는 것들은 전부 NG판정해 버린다 [12/12/2016 OSC]
	CCellInfo* pCell;
	for(int i = 0; i < JIG_CH_MAX; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			if(pCell->m_MTPWrite.m_Class == NONE_CELL)
			{
				pCell->m_MTPWrite.m_Class = REJECT_CELL;
				pCell->m_MTPWrite.m_DefectName = pCell->m_MTPWrite.m_strCommand + TEXT_DEFECT_TIME_OUT;
				pCell->defaultData.m_bIsInspectionSkip = TRUE;
			}
		}
	}
}

void CUnitCtrlBank::MTPVerify_Send( JIG_ID jig , JIG_CH channel /*= JIG_CH_MAX*/)
{
	if(theProcBank.m_bDryRunMode)
		return;

	CString strCellID;
	if(channel == JIG_CH_MAX)
	{
		for (int i = 0; i < JIG_CH_MAX; i++)
		{
			if(theCellBank.GetCellTag(jig, (JIG_CH)i).IsExist())
			{
				CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)i));
				if(pCell->m_MTPVerify.IsStarted() == FALSE)
				{
					if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
					{
						if(pCell->defaultData.m_bMCR_OK)
							strCellID = pCell->defaultData.m_strCellID;
						else
							strCellID = pCell->defaultData.m_strInnerID;
						pCell->m_MTPVerify.SetTimeStart();
						theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_MTPVerify.m_strCommand, jig, (JIG_CH)i, strCellID);
					}
					else
					{
						pCell->m_MTPVerify.m_Class = SKIP_CELL;
						pCell->m_MTPVerify.m_DefectName = SKIP;
					}
				}
			}
		}
	}
	else
	{
		if(theCellBank.GetCellTag(jig, (JIG_CH)channel).IsExist())
		{
			CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)channel));
			if(pCell->m_MTPVerify.IsStarted() == FALSE)
			{
				if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
				{
					if(pCell->defaultData.m_bMCR_OK)
						strCellID = pCell->defaultData.m_strCellID;
					else
						strCellID = pCell->defaultData.m_strInnerID;
					pCell->m_MTPVerify.SetTimeStart();
					theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_MTPVerify.m_strCommand, jig, (JIG_CH)channel, strCellID);
				}
				else
				{
					pCell->m_MTPVerify.m_Class = SKIP_CELL;
					pCell->m_MTPVerify.m_DefectName = SKIP;
				}
			}
		}
	}
	
}

BOOL CUnitCtrlBank::MTPVerify_IsStarted(JIG_ID jig, JIG_CH ch /*= JIG_CH_MAX*/)
{
	// 커맨드를 이미 날렸는지 체크
	CCellInfo *pCell;
	int nStart, nEnd;

	if(ch == JIG_CH_MAX)
	{
		nStart = 0;		nEnd = ch;
	}
	else
	{
		nStart = ch;	nEnd = ch+1;
	}

	for (int i = nStart; i < nEnd; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			if(pCell->m_MTPVerify.IsStarted() == FALSE)
				return FALSE;
		}
	}
	return TRUE;
}

BOOL CUnitCtrlBank::MTPVerify_Check( JIG_ID jig, JIG_CH ch /*= JIG_CH_MAX*/ )
{
	CCellInfo *pCell;
	int nStart, nEnd;

	if(ch == JIG_CH_MAX)
	{
		nStart = 0;		nEnd = ch;
	}
	else
	{
		nStart = ch;	nEnd = ch+1;
	}

	for (int i = nStart; i < nEnd; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			if(pCell->m_MTPVerify.m_Class == NONE_CELL)
				return FALSE;
		}
	}
	return TRUE;
}

void CUnitCtrlBank::MTPVerify_TimeOut( JIG_ID jig )
{
	// 아직까지 판정이 없는 것들은 전부 NG판정해 버린다 [12/12/2016 OSC]
	CCellInfo* pCell;
	for(int i = 0; i < JIG_CH_MAX; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			if(pCell->m_MTPVerify.m_Class == NONE_CELL)
			{
				pCell->m_MTPVerify.m_Class = REJECT_CELL;
				pCell->m_MTPVerify.m_DefectName = pCell->m_MTPVerify.m_strCommand + TEXT_DEFECT_TIME_OUT;
				pCell->defaultData.m_bIsInspectionSkip = TRUE;
			}
		}
	}
}

void CUnitCtrlBank::WhiteCurrent_Send(JIG_ID jig, JIG_CH channel  /*= JIG_CH_MAX*/)
{
	if(theProcBank.m_bDryRunMode)
		return;

	if(channel == JIG_CH_MAX)
	{
		for (int i = 0; i < JIG_CH_MAX; i++)
		{
			if(theCellBank.GetCellTag(jig, (JIG_CH)i).IsExist())
			{
				CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)i));
				if(pCell->m_WhiteCurrent.IsStarted() == FALSE)
				{
					if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
					{
						pCell->m_WhiteCurrent.SetTimeStart();
						theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_WhiteCurrent.m_strCommand, jig, (JIG_CH)i);
					}
					else
					{
						pCell->m_WhiteCurrent.m_Class = SKIP_CELL;
						pCell->m_WhiteCurrent.m_DefectName = SKIP;
					}
				}
			}
		}
	}
	else
	{
		if(theCellBank.GetCellTag(jig, (JIG_CH)channel).IsExist())
		{
			CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)channel));
			if(pCell->m_WhiteCurrent.IsStarted() == FALSE)
			{
				if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
				{
					pCell->m_WhiteCurrent.SetTimeStart();
					theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_WhiteCurrent.m_strCommand, jig, (JIG_CH)channel);
				}
				else
				{
					pCell->m_WhiteCurrent.m_Class = SKIP_CELL;
					pCell->m_WhiteCurrent.m_DefectName = SKIP;
				}
			}
		}
	}
	
}

BOOL CUnitCtrlBank::WhiteCurrent_Check( JIG_ID jig, JIG_CH channel )
{
	CCellTag	tag;
	CCellInfo* pCell;
	tag = theCellBank.GetCellTag(jig, channel);
	if ( tag.IsExist() == FALSE )
		return FALSE;

	pCell = theCellBank.GetCellInfo(tag);
	if(pCell->m_WhiteCurrent.m_Class == NONE_CELL)
		return FALSE;
	return TRUE;
}

void CUnitCtrlBank::WhiteCurrent_TimeOut( JIG_ID jig )
{
	// 아직까지 판정이 없는 것들은 전부 NG판정해 버린다 [12/12/2016 OSC]
	CCellInfo* pCell;
	for(int i = 0; i < JIG_CH_MAX; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			if(pCell->m_WhiteCurrent.m_Class == NONE_CELL)
			{
				pCell->m_WhiteCurrent.m_Class = REJECT_CELL;
				pCell->m_WhiteCurrent.m_DefectName = pCell->m_WhiteCurrent.m_strCommand + TEXT_DEFECT_TIME_OUT;
				pCell->defaultData.m_bIsInspectionSkip = TRUE;
			}
		}
	}
}

void CUnitCtrlBank::SleepCurrent_Send( JIG_ID jig ,JIG_CH channel /*= JIG_CH_MAX*/ )
{
	if(theProcBank.m_bDryRunMode)
		return;

	if(channel == JIG_CH_MAX)
	{
		for (int i = 0; i < JIG_CH_MAX; i++)
		{
			if(theCellBank.GetCellTag(jig, (JIG_CH)i).IsExist())
			{
				CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)i));
				if(pCell->m_SleepCurrent.IsStarted() == FALSE)
				{
					if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
					{
						pCell->m_SleepCurrent.SetTimeStart();
						theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_SleepCurrent.m_strCommand, jig, (JIG_CH)i);
					}
					else
					{
						pCell->m_SleepCurrent.m_Class = SKIP_CELL;
						pCell->m_SleepCurrent.m_DefectName = SKIP;
					}
				}
			}
		}
	}
	else
	{
		if(theCellBank.GetCellTag(jig, (JIG_CH)channel).IsExist())
		{
			CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)channel));
			if(pCell->m_SleepCurrent.IsStarted() == FALSE)
			{
				if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
				{
					pCell->m_SleepCurrent.SetTimeStart();
					theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_SleepCurrent.m_strCommand, jig, (JIG_CH)channel);
				}
				else
				{
					pCell->m_SleepCurrent.m_Class = SKIP_CELL;
					pCell->m_SleepCurrent.m_DefectName = SKIP;
				}
			}
		}
	}
}

BOOL CUnitCtrlBank::SleepCurrent_Check( JIG_ID jig, JIG_CH channel )
{
	CCellInfo* pCell;
	pCell = theCellBank.GetCellInfo(jig, channel);
	if ( pCell == NULL )
		return FALSE;

	if(pCell->m_SleepCurrent.m_Class == NONE_CELL)
		return FALSE;
	return TRUE;
}

void CUnitCtrlBank::SleepCurrent_TimeOut( JIG_ID jig )
{
	// 아직까지 판정이 없는 것들은 전부 NG판정해 버린다 [12/12/2016 OSC]
	CCellInfo* pCell;
	for(int i = 0; i < JIG_CH_MAX; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			if(pCell->m_SleepCurrent.m_Class == NONE_CELL)
			{
				pCell->m_SleepCurrent.m_Class = REJECT_CELL;
				pCell->m_SleepCurrent.m_DefectName = pCell->m_SleepCurrent.m_strCommand + TEXT_DEFECT_TIME_OUT;
				pCell->defaultData.m_bIsInspectionSkip = TRUE;
			}
		}
	}
}

void CUnitCtrlBank::HlpmCurrent_Send( JIG_ID jig , JIG_CH channel /*= JIG_CH_MAX*/ )
{
	if(theProcBank.m_bDryRunMode)
		return;

	if(channel == JIG_CH_MAX)
	{
		for (int i = 0; i < JIG_CH_MAX; i++)
		{
			if(theCellBank.GetCellTag(jig, (JIG_CH)i).IsExist())
			{
				CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)i));
				if(pCell->m_HLPMCurrent.IsStarted() == FALSE)
				{
					if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
					{
						pCell->m_HLPMCurrent.SetTimeStart();
						theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_HLPMCurrent.m_strCommand, jig, (JIG_CH)i);
					}
					else
					{
						pCell->m_HLPMCurrent.m_Class = SKIP_CELL;
						pCell->m_HLPMCurrent.m_DefectName = SKIP;
					}
				}
			}
		}
	}
	else
	{
		if(theCellBank.GetCellTag(jig, (JIG_CH)channel).IsExist())
		{
			CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)channel));
			if(pCell->m_HLPMCurrent.IsStarted() == FALSE)
			{
				if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
				{
					pCell->m_HLPMCurrent.SetTimeStart();
					theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_HLPMCurrent.m_strCommand, jig, (JIG_CH)channel);
				}
				else
				{
					pCell->m_HLPMCurrent.m_Class = SKIP_CELL;
					pCell->m_HLPMCurrent.m_DefectName = SKIP;
				}
			}
		}
	}
}

BOOL CUnitCtrlBank::HlpmCurrent_Check( JIG_ID jig, JIG_CH channel )
{
	CCellInfo* pCell;
	pCell = theCellBank.GetCellInfo(jig, channel);
	if ( pCell == NULL )
		return FALSE;

	if(pCell->m_HLPMCurrent.m_Class == NONE_CELL)
		return FALSE;
	return TRUE;
}

void CUnitCtrlBank::HlpmCurrent_TimeOut( JIG_ID jig )
{
	// 아직까지 판정이 없는 것들은 전부 NG판정해 버린다 [12/12/2016 OSC]
	CCellInfo* pCell;
	for(int i = 0; i < JIG_CH_MAX; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			if(pCell->m_HLPMCurrent.m_Class == NONE_CELL)
			{
				pCell->m_HLPMCurrent.m_Class = REJECT_CELL;
				pCell->m_HLPMCurrent.m_DefectName = pCell->m_HLPMCurrent.m_strCommand + TEXT_DEFECT_TIME_OUT;
				pCell->defaultData.m_bIsInspectionSkip = TRUE;
			}
		}
	}
}

void CUnitCtrlBank::TSP_START_Send(JIG_ID jig, JIG_CH channel/*= JIG_CH_MAX*/ )
{
	if(theProcBank.m_bDryRunMode)
		return;
	if(channel == JIG_CH_MAX)
	{
		for (int i = 0; i < JIG_CH_MAX; i++)
		{
			if(theCellBank.GetCellTag(jig, (JIG_CH)i).IsExist())
			{
				CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)i));
				if(pCell->m_TSPStart.IsStarted() == FALSE)
				{
					if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
					{
						pCell->m_TSPStart.SetTimeStart();
						theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_TSPStart.m_strCommand, jig, (JIG_CH)i);
					}
					else
					{
						pCell->m_TSPStart.m_Class = SKIP_CELL;
						pCell->m_TSPStart.m_DefectName = SKIP;
					}
				}
			}
		}
	}
	else
	{
		if(theCellBank.GetCellTag(jig, (JIG_CH)channel).IsExist())
		{
			CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)channel));
			if(pCell->m_TSPStart.IsStarted() == FALSE)
			{
				if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
				{
					pCell->m_TSPStart.SetTimeStart();
					theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_TSPStart.m_strCommand, jig, (JIG_CH)channel);
				}
				else
				{
					pCell->m_TSPStart.m_Class = SKIP_CELL;
					pCell->m_TSPStart.m_DefectName = SKIP;
				}
			}
		}
	}

}

BOOL CUnitCtrlBank::TSP_START_Check(JIG_ID jig, JIG_CH channel)
{
	if(theProcBank.m_bDryRunMode)
		return TRUE;

	CCellTag	tag;
	CCellInfo* pCell;
	tag = theCellBank.GetCellTag(jig, channel);
	if ( tag.IsExist() == FALSE )
		return FALSE;

	pCell = theCellBank.GetCellInfo(tag);
	if(pCell->m_TSPStart.m_Class == NONE_CELL)
		return FALSE;
	return TRUE;
}

void CUnitCtrlBank::TSP_START_TimeOut(JIG_ID jig)
{
	// 아직까지 판정이 없는 것들은 전부 NG판정해 버린다 [12/12/2016 OSC]
	CCellInfo* pCell;
	for(int i = 0; i < JIG_CH_MAX; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			if(pCell->m_TSPStart.m_Class == NONE_CELL)
			{
				pCell->m_TSPStart.m_Class = REJECT_CELL;
				pCell->m_TSPStart.m_DefectName = pCell->m_TSPStart.m_strCommand + TEXT_DEFECT_TIME_OUT;
				pCell->defaultData.m_bIsInspectionSkip = TRUE;
			}
		}
	}
}

void CUnitCtrlBank::EVTVersion_Send(JIG_ID jig, JIG_CH channel/*= JIG_CH_MAX*/)
{
	if(theProcBank.m_bDryRunMode)
		return;
	if(channel == JIG_CH_MAX)
	{
		for (int i = 0; i < JIG_CH_MAX; i++)
		{
			if(theCellBank.GetCellTag(jig, (JIG_CH)i).IsExist())
			{
				CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)i));
				if(pCell->m_EVTVersionCheck.IsStarted() == FALSE)
				{
					if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
					{
						pCell->m_EVTVersionCheck.SetTimeStart();
						theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_EVTVersionCheck.m_strCommand, jig, (JIG_CH)i);
					}
					else
					{
						pCell->m_EVTVersionCheck.m_Class = SKIP_CELL;
						pCell->m_EVTVersionCheck.m_DefectName = SKIP;
					}
				}
			}
		}
	}
	else
	{
		if(theCellBank.GetCellTag(jig, (JIG_CH)channel).IsExist())
		{
			CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)channel));
			if(pCell->m_EVTVersionCheck.IsStarted() == FALSE)
			{
				if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
				{
					pCell->m_EVTVersionCheck.SetTimeStart();
					theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_EVTVersionCheck.m_strCommand, jig, (JIG_CH)channel);
				}
				else
				{
					pCell->m_EVTVersionCheck.m_Class = SKIP_CELL;
					pCell->m_EVTVersionCheck.m_DefectName = SKIP;
				}
			}
		}
	}

}

BOOL CUnitCtrlBank::EVTVersion_Check(JIG_ID jig, JIG_CH channel)
{
	if(theProcBank.m_bDryRunMode)
		return TRUE;

	CCellTag	tag;
	CCellInfo* pCell;
	tag = theCellBank.GetCellTag(jig, channel);
	if ( tag.IsExist() == FALSE )
		return FALSE;

	pCell = theCellBank.GetCellInfo(tag);
	if(pCell->m_EVTVersionCheck.m_Class == NONE_CELL)
		return FALSE;
	return TRUE;
}

void CUnitCtrlBank::EVTVersion_TimeOut(JIG_ID jig)
{
	// 아직까지 판정이 없는 것들은 전부 NG판정해 버린다 [12/12/2016 OSC]
	CCellInfo* pCell;
	for(int i = 0; i < JIG_CH_MAX; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			if(pCell->m_EVTVersionCheck.m_Class == NONE_CELL)
			{
				pCell->m_EVTVersionCheck.m_Class = REJECT_CELL;
				pCell->m_EVTVersionCheck.m_DefectName = pCell->m_EVTVersionCheck.m_strCommand + TEXT_DEFECT_TIME_OUT;
				pCell->defaultData.m_bIsInspectionSkip = TRUE;
			}
		}
	}
}




void CUnitCtrlBank::TECheck_Send( JIG_ID jig, JIG_CH channel/*= JIG_CH_MAX*/ )
{
	if(theProcBank.m_bDryRunMode)
		return;
	if(channel == JIG_CH_MAX)
	{
		for (int i = 0; i < JIG_CH_MAX; i++)
		{
			if(theCellBank.GetCellTag(jig, (JIG_CH)i).IsExist())
			{
				CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)i));
				if(pCell->m_TECheck.IsStarted() == FALSE)
				{
					if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
					{
						pCell->m_TECheck.SetTimeStart();
						theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_TECheck.m_strCommand, jig, (JIG_CH)i);
					}
					else
					{
						pCell->m_TECheck.m_Class = SKIP_CELL;
						pCell->m_TECheck.m_DefectName = SKIP;
					}
				}
			}
		}
	}
	else
	{
		if(theCellBank.GetCellTag(jig, (JIG_CH)channel).IsExist())
		{
			CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)channel));
			if(pCell->m_TECheck.IsStarted() == FALSE)
			{
				if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
				{
					pCell->m_TECheck.SetTimeStart();
					theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_TECheck.m_strCommand, jig, (JIG_CH)channel);
				}
				else
				{
					pCell->m_TECheck.m_Class = SKIP_CELL;
					pCell->m_TECheck.m_DefectName = SKIP;
				}
			}
		}
	}
}

BOOL CUnitCtrlBank::TECheck_Check( JIG_ID jig, JIG_CH channel )
{
	CCellInfo* pCell;
	pCell = theCellBank.GetCellInfo(jig, channel);
	if ( pCell == NULL )
		return FALSE;

	if(pCell->m_TECheck.m_Class == NONE_CELL)
		return FALSE;
	return TRUE;
}

void CUnitCtrlBank::TECheck_TimeOut( JIG_ID jig )
{
	// 아직까지 판정이 없는 것들은 전부 NG판정해 버린다 [12/12/2016 OSC]
	CCellInfo* pCell;
	for(int i = 0; i < JIG_CH_MAX; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			if(pCell->m_TECheck.m_Class == NONE_CELL)
			{
				pCell->m_TECheck.m_Class = REJECT_CELL;
				pCell->m_TECheck.m_DefectName = pCell->m_TECheck.m_strCommand + TEXT_DEFECT_TIME_OUT;
				pCell->defaultData.m_bIsInspectionSkip = TRUE;
			}
		}
	}
}

void CUnitCtrlBank::IDCheck_Send(JIG_ID jig, JIG_CH channel/*= JIG_CH_MAX*/)
{
	if(theProcBank.m_bDryRunMode)
		return;

	if(channel == JIG_CH_MAX)
	{
		for (int i = 0; i < JIG_CH_MAX; i++)
		{
			if(theCellBank.GetCellTag(jig, (JIG_CH)i).IsExist())
			{
				CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)i));
				if(pCell->m_IDCheck.IsStarted() == FALSE)
				{
					if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
					{
						pCell->m_IDCheck.SetTimeStart();
						theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_IDCheck.m_strCommand, jig, (JIG_CH)i);
					}
					else
					{
						pCell->m_IDCheck.m_Class = SKIP_CELL;
						pCell->m_IDCheck.m_DefectName = SKIP;
					}
				}
			}
		}
	}
	else
	{
		if(theCellBank.GetCellTag(jig, (JIG_CH)channel).IsExist())
		{
			CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)channel));
			if(pCell->m_IDCheck.IsStarted() == FALSE)
			{
				if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
				{
					pCell->m_IDCheck.SetTimeStart();
					theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_IDCheck.m_strCommand, jig, (JIG_CH)channel);
				}
				else
				{
					pCell->m_IDCheck.m_Class = SKIP_CELL;
					pCell->m_IDCheck.m_DefectName = SKIP;
				}
			}
		}
	}

}

BOOL CUnitCtrlBank::IDCheck_Check(JIG_ID jig, JIG_CH channel/*= JIG_CH_MAX*/)
{
	if(theProcBank.m_bDryRunMode)
		return TRUE;

	CCellTag	tag;
	CCellInfo* pCell;
	tag = theCellBank.GetCellTag(jig, channel);
	if ( tag.IsExist() == FALSE )
		return FALSE;

	pCell = theCellBank.GetCellInfo(tag);
	if(pCell->m_IDCheck.m_Class == NONE_CELL)
		return FALSE;
	return TRUE;
}

void CUnitCtrlBank::IDCheck_TimeOut(JIG_ID jig)
{
	// 아직까지 판정이 없는 것들은 전부 NG판정해 버린다 [12/12/2016 OSC]
	CCellInfo* pCell;
	for(int i = 0; i < JIG_CH_MAX; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			if(pCell->m_IDCheck.m_Class == NONE_CELL)
			{
				pCell->m_IDCheck.m_Class = REJECT_CELL;
				pCell->m_IDCheck.m_DefectName = pCell->m_IDCheck.m_strCommand + TEXT_DEFECT_TIME_OUT;
				pCell->defaultData.m_bIsInspectionSkip = TRUE;
			}
		}
	}
}

void CUnitCtrlBank::OTPREG_Send(JIG_ID jig, JIG_CH channel/*= JIG_CH_MAX*/)
{
	if(theProcBank.m_bDryRunMode)
		return;
	if(channel == JIG_CH_MAX)
	{
		for (int i = 0; i < JIG_CH_MAX; i++)
		{
			if(theCellBank.GetCellTag(jig, (JIG_CH)i).IsExist())
			{
				CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)i));
				if(pCell->m_OTPREGCheck.IsStarted() == FALSE)
				{
					if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
					{
						pCell->m_OTPREGCheck.SetTimeStart();
						theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_OTPREGCheck.m_strCommand, jig, (JIG_CH)i);
					}
					else
					{
						pCell->m_OTPREGCheck.m_Class = SKIP_CELL;
						pCell->m_OTPREGCheck.m_DefectName = SKIP;
					}
				}
			}
		}
	}
	else
	{
		if(theCellBank.GetCellTag(jig, (JIG_CH)channel).IsExist())
		{
			CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)channel));
			if(pCell->m_OTPREGCheck.IsStarted() == FALSE)
			{
				if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
				{
					pCell->m_OTPREGCheck.SetTimeStart();
					theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_OTPREGCheck.m_strCommand, jig, (JIG_CH)channel);
				}
				else
				{
					pCell->m_OTPREGCheck.m_Class = SKIP_CELL;
					pCell->m_OTPREGCheck.m_DefectName = SKIP;
				}
			}
		}
	}

}

BOOL CUnitCtrlBank::OTPREG_Check(JIG_ID jig, JIG_CH channel)
{
	if(theProcBank.m_bDryRunMode)
		return TRUE;

	CCellTag	tag;
	CCellInfo* pCell;
	tag = theCellBank.GetCellTag(jig, channel);
	if ( tag.IsExist() == FALSE )
		return FALSE;

	pCell = theCellBank.GetCellInfo(tag);
	if(pCell->m_OTPREGCheck.m_Class == NONE_CELL)
		return FALSE;
	return TRUE;
}

void CUnitCtrlBank::OTPREG_TimeOut(JIG_ID jig)
{
	// 아직까지 판정이 없는 것들은 전부 NG판정해 버린다 [12/12/2016 OSC]
	CCellInfo* pCell;
	for(int i = 0; i < JIG_CH_MAX; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			if(pCell->m_OTPREGCheck.m_Class == NONE_CELL)
			{
				pCell->m_OTPREGCheck.m_Class = REJECT_CELL;
				pCell->m_OTPREGCheck.m_DefectName = pCell->m_OTPREGCheck.m_strCommand + TEXT_DEFECT_TIME_OUT;
				pCell->defaultData.m_bIsInspectionSkip = TRUE;
			}
		}
	}
}

void CUnitCtrlBank::ICTTest_Send(JIG_ID jig, JIG_CH channel/*= JIG_CH_MAX*/)
{
	if(theProcBank.m_bDryRunMode)
		return;
	if(channel == JIG_CH_MAX)
	{
		for (int i = 0; i < JIG_CH_MAX; i++)
		{
			if(theCellBank.GetCellTag(jig, (JIG_CH)i).IsExist())
			{
				CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)i));
				if(pCell->m_ICTCheck.IsStarted() == FALSE)
				{
					if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
					{
						pCell->m_ICTCheck.SetTimeStart();
						theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_ICTCheck.m_strCommand, jig, (JIG_CH)i);
					}
					else
					{
						pCell->m_ICTCheck.m_Class = SKIP_CELL;
						pCell->m_ICTCheck.m_DefectName = SKIP;
					}
				}
			}
		}
	}
	else
	{
		if(theCellBank.GetCellTag(jig, (JIG_CH)channel).IsExist())
		{
			CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)channel));
			if(pCell->m_ICTCheck.IsStarted() == FALSE)
			{
				if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
				{
					pCell->m_ICTCheck.SetTimeStart();
					theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_ICTCheck.m_strCommand, jig, (JIG_CH)channel);
				}
				else
				{
					pCell->m_ICTCheck.m_Class = SKIP_CELL;
					pCell->m_ICTCheck.m_DefectName = SKIP;
				}
			}
		}
	}

}

BOOL CUnitCtrlBank::ICTTest_Check(JIG_ID jig, JIG_CH channel)
{
	if(theProcBank.m_bDryRunMode)
		return TRUE;

	CCellTag	tag;
	CCellInfo* pCell;
	tag = theCellBank.GetCellTag(jig, channel);
	if ( tag.IsExist() == FALSE )
		return FALSE;

	pCell = theCellBank.GetCellInfo(tag);
	if(pCell->m_ICTCheck.m_Class == NONE_CELL)
		return FALSE;
	return TRUE;
}

void CUnitCtrlBank::ICTTest_TimeOut(JIG_ID jig)
{
	// 아직까지 판정이 없는 것들은 전부 NG판정해 버린다 [12/12/2016 OSC]
	CCellInfo* pCell;
	for(int i = 0; i < JIG_CH_MAX; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			if(pCell->m_ICTCheck.m_Class == NONE_CELL)
			{
				pCell->m_ICTCheck.m_Class = REJECT_CELL;
				pCell->m_ICTCheck.m_DefectName = pCell->m_ICTCheck.m_strCommand + TEXT_DEFECT_TIME_OUT;
				pCell->defaultData.m_bIsInspectionSkip = TRUE;
			}
		}
	}
}

void CUnitCtrlBank::COPR_ICTTest_Send(JIG_ID jig, JIG_CH channel/*= JIG_CH_MAX*/)
{
	if(theProcBank.m_bDryRunMode)
		return;
	if(channel == JIG_CH_MAX)
	{
		for (int i = 0; i < JIG_CH_MAX; i++)
		{
			if(theCellBank.GetCellTag(jig, (JIG_CH)i).IsExist())
			{
				CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)i));
				if(pCell->m_COPRICTTest.IsStarted() == FALSE)
				{
					if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
					{
						pCell->m_COPRICTTest.SetTimeStart();
						theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_COPRICTTest.m_strCommand, jig, (JIG_CH)i);
					}
					else
					{
						pCell->m_COPRICTTest.m_Class = SKIP_CELL;
						pCell->m_COPRICTTest.m_DefectName = SKIP;
					}
				}
			}
		}
	}
	else
	{
		if(theCellBank.GetCellTag(jig, (JIG_CH)channel).IsExist())
		{
			CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)channel));
			if(pCell->m_COPRICTTest.IsStarted() == FALSE)
			{
				if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
				{
					pCell->m_COPRICTTest.SetTimeStart();
					theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_COPRICTTest.m_strCommand, jig, (JIG_CH)channel);
				}
				else
				{
					pCell->m_COPRICTTest.m_Class = SKIP_CELL;
					pCell->m_COPRICTTest.m_DefectName = SKIP;
				}
			}
		}
	}

}

BOOL CUnitCtrlBank::COPR_ICTTest_Check(JIG_ID jig, JIG_CH channel)
{
	if(theProcBank.m_bDryRunMode)
		return TRUE;

	CCellTag	tag;
	CCellInfo* pCell;
	tag = theCellBank.GetCellTag(jig, channel);
	if ( tag.IsExist() == FALSE )
		return FALSE;

	pCell = theCellBank.GetCellInfo(tag);
	if(pCell->m_COPRICTTest.m_Class == NONE_CELL)
		return FALSE;
	return TRUE;
}

void CUnitCtrlBank::COPR_ICTTest_TimeOut(JIG_ID jig)
{
	// 아직까지 판정이 없는 것들은 전부 NG판정해 버린다 [12/12/2016 OSC]
	CCellInfo* pCell;
	for(int i = 0; i < JIG_CH_MAX; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			if(pCell->m_COPRICTTest.m_Class == NONE_CELL)
			{
				pCell->m_COPRICTTest.m_Class = REJECT_CELL;
				pCell->m_COPRICTTest.m_DefectName = pCell->m_COPRICTTest.m_strCommand + TEXT_DEFECT_TIME_OUT;
				pCell->defaultData.m_bIsInspectionSkip = TRUE;
			}
		}
	}
}

void CUnitCtrlBank::POCErrorCheck_Send(JIG_ID jig, JIG_CH channel/*= JIG_CH_MAX*/)
{
	if(theProcBank.m_bDryRunMode)
		return;
	if(channel == JIG_CH_MAX)
	{
		for (int i = 0; i < JIG_CH_MAX; i++)
		{
			if(theCellBank.GetCellTag(jig, (JIG_CH)i).IsExist())
			{
				CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)i));
				if(pCell->m_PocErrorCheck.IsStarted() == FALSE)
				{
					if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
					{
						pCell->m_PocErrorCheck.SetTimeStart();
						theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_PocErrorCheck.m_strCommand, jig, (JIG_CH)i);
					}
					else
					{
						pCell->m_PocErrorCheck.m_Class = SKIP_CELL;
						pCell->m_PocErrorCheck.m_DefectName = SKIP;
					}
				}
			}
		}
	}
	else
	{
		if(theCellBank.GetCellTag(jig, (JIG_CH)channel).IsExist())
		{
			CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)channel));
			if(pCell->m_PocErrorCheck.IsStarted() == FALSE)
			{
				if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
				{
					pCell->m_PocErrorCheck.SetTimeStart();
					theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_PocErrorCheck.m_strCommand, jig, (JIG_CH)channel);
				}
				else
				{
					pCell->m_PocErrorCheck.m_Class = SKIP_CELL;
					pCell->m_PocErrorCheck.m_DefectName = SKIP;
				}
			}
		}
	}

}

BOOL CUnitCtrlBank::POCErrorCheck_Check(JIG_ID jig, JIG_CH channel)
{
	if(theProcBank.m_bDryRunMode)
		return TRUE;

	CCellTag	tag;
	CCellInfo* pCell;
	tag = theCellBank.GetCellTag(jig, channel);
	if ( tag.IsExist() == FALSE )
		return FALSE;

	pCell = theCellBank.GetCellInfo(tag);
	if(pCell->m_PocErrorCheck.m_Class == NONE_CELL)
		return FALSE;
	return TRUE;
}

void CUnitCtrlBank::POCErrorCheck_TimeOut(JIG_ID jig)
{
	// 아직까지 판정이 없는 것들은 전부 NG판정해 버린다 [12/12/2016 OSC]
	CCellInfo* pCell;
	for(int i = 0; i < JIG_CH_MAX; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			if(pCell->m_PocErrorCheck.m_Class == NONE_CELL)
			{
				pCell->m_PocErrorCheck.m_Class = REJECT_CELL;
				pCell->m_PocErrorCheck.m_DefectName = pCell->m_PocErrorCheck.m_strCommand + TEXT_DEFECT_TIME_OUT;
				pCell->defaultData.m_bIsInspectionSkip = TRUE;
			}
		}
	}
}

void CUnitCtrlBank::DDIBlockTest_Send(JIG_ID jig, JIG_CH channel/*= JIG_CH_MAX*/)
{
	if(theProcBank.m_bDryRunMode)
		return;
	if(channel == JIG_CH_MAX)
	{
		for (int i = 0; i < JIG_CH_MAX; i++)
		{
			if(theCellBank.GetCellTag(jig, (JIG_CH)i).IsExist())
			{
				CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)i));
				if(pCell->m_DDIBlockTest.IsStarted() == FALSE)
				{
					if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
					{
						pCell->m_DDIBlockTest.SetTimeStart();
						theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_DDIBlockTest.m_strCommand, jig, (JIG_CH)i);
					}
					else
					{
						pCell->m_DDIBlockTest.m_Class = SKIP_CELL;
						pCell->m_DDIBlockTest.m_DefectName = SKIP;
					}
				}
			}
		}
	}
	else
	{
		if(theCellBank.GetCellTag(jig, (JIG_CH)channel).IsExist())
		{
			CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)channel));
			if(pCell->m_DDIBlockTest.IsStarted() == FALSE)
			{
				if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
				{
					pCell->m_DDIBlockTest.SetTimeStart();
					theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_DDIBlockTest.m_strCommand, jig, (JIG_CH)channel);
				}
				else
				{
					pCell->m_DDIBlockTest.m_Class = SKIP_CELL;
					pCell->m_DDIBlockTest.m_DefectName = SKIP;
				}
			}
		}
	}

}

BOOL CUnitCtrlBank::DDIBlockTest_Check(JIG_ID jig, JIG_CH channel)
{
	if(theProcBank.m_bDryRunMode)
		return TRUE;

	CCellTag	tag;
	CCellInfo* pCell;
	tag = theCellBank.GetCellTag(jig, channel);
	if ( tag.IsExist() == FALSE )
		return FALSE;

	pCell = theCellBank.GetCellInfo(tag);
	if(pCell->m_DDIBlockTest.m_Class == NONE_CELL)
		return FALSE;
	return TRUE;
}

void CUnitCtrlBank::DDIBlockTest_TimeOut(JIG_ID jig)
{
	// 아직까지 판정이 없는 것들은 전부 NG판정해 버린다 [12/12/2016 OSC]
	CCellInfo* pCell;
	for(int i = 0; i < JIG_CH_MAX; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			if(pCell->m_DDIBlockTest.m_Class == NONE_CELL)
			{
				pCell->m_DDIBlockTest.m_Class = REJECT_CELL;
				pCell->m_DDIBlockTest.m_DefectName = pCell->m_DDIBlockTest.m_strCommand + TEXT_DEFECT_TIME_OUT;
				pCell->defaultData.m_bIsInspectionSkip = TRUE;
			}
		}
	}
}

void CUnitCtrlBank::OptionCheck2_Send(JIG_ID jig, JIG_CH channel/*= JIG_CH_MAX*/)
{
	if(theProcBank.m_bDryRunMode)
		return;
	if(channel == JIG_CH_MAX)
	{
		for (int i = 0; i < JIG_CH_MAX; i++)
		{
			if(theCellBank.GetCellTag(jig, (JIG_CH)i).IsExist())
			{
				CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)i));
				if(pCell->m_OptionCheck2.IsStarted() == FALSE)
				{
					if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
					{
						pCell->m_OptionCheck2.SetTimeStart();
						theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_OptionCheck2.m_strCommand, jig, (JIG_CH)i);
					}
					else
					{
						pCell->m_OptionCheck2.m_Class = SKIP_CELL;
						pCell->m_OptionCheck2.m_DefectName = SKIP;
					}
				}
			}
		}
	}
	else
	{
		if(theCellBank.GetCellTag(jig, (JIG_CH)channel).IsExist())
		{
			CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)channel));
			if(pCell->m_OptionCheck2.IsStarted() == FALSE)
			{
				if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
				{
					pCell->m_OptionCheck2.SetTimeStart();
					theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_OptionCheck2.m_strCommand, jig, (JIG_CH)channel);
				}
				else
				{
					pCell->m_OptionCheck2.m_Class = SKIP_CELL;
					pCell->m_OptionCheck2.m_DefectName = SKIP;
				}
			}
		}
	}

}

BOOL CUnitCtrlBank::OptionCheck2_Check(JIG_ID jig, JIG_CH channel)
{
	if(theProcBank.m_bDryRunMode)
		return TRUE;

	CCellTag	tag;
	CCellInfo* pCell;
	tag = theCellBank.GetCellTag(jig, channel);
	if ( tag.IsExist() == FALSE )
		return FALSE;

	pCell = theCellBank.GetCellInfo(tag);
	if(pCell->m_OptionCheck2.m_Class == NONE_CELL)
		return FALSE;
	return TRUE;
}

void CUnitCtrlBank::OptionCheck2_TimeOut(JIG_ID jig)
{
	// 아직까지 판정이 없는 것들은 전부 NG판정해 버린다 [12/12/2016 OSC]
	CCellInfo* pCell;
	for(int i = 0; i < JIG_CH_MAX; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			if(pCell->m_OptionCheck2.m_Class == NONE_CELL)
			{
				pCell->m_OptionCheck2.m_Class = REJECT_CELL;
				pCell->m_OptionCheck2.m_DefectName = pCell->m_OptionCheck2.m_strCommand + TEXT_DEFECT_TIME_OUT;
				pCell->defaultData.m_bIsInspectionSkip = TRUE;
			}
		}
	}
}

void CUnitCtrlBank::OptionCheck3_Send(JIG_ID jig, JIG_CH channel/*= JIG_CH_MAX*/)
{
	if(theProcBank.m_bDryRunMode)
		return;
	if(channel == JIG_CH_MAX)
	{
		for (int i = 0; i < JIG_CH_MAX; i++)
		{
			if(theCellBank.GetCellTag(jig, (JIG_CH)i).IsExist())
			{
				CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)i));
				if(pCell->m_OptionCheck3.IsStarted() == FALSE)
				{
					if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
					{
						pCell->m_OptionCheck3.SetTimeStart();
						theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_OptionCheck3.m_strCommand, jig, (JIG_CH)i);
					}
					else
					{
						pCell->m_OptionCheck3.m_Class = SKIP_CELL;
						pCell->m_OptionCheck3.m_DefectName = SKIP;
					}
				}
			}
		}
	}
	else
	{
		if(theCellBank.GetCellTag(jig, (JIG_CH)channel).IsExist())
		{
			CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)channel));
			if(pCell->m_OptionCheck3.IsStarted() == FALSE)
			{
				if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
				{
					pCell->m_OptionCheck3.SetTimeStart();
					theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_OptionCheck3.m_strCommand, jig, (JIG_CH)channel);
				}
				else
				{
					pCell->m_OptionCheck3.m_Class = SKIP_CELL;
					pCell->m_OptionCheck3.m_DefectName = SKIP;
				}
			}
		}
	}

}

BOOL CUnitCtrlBank::OptionCheck3_Check(JIG_ID jig, JIG_CH channel)
{
	if(theProcBank.m_bDryRunMode)
		return TRUE;

	CCellTag	tag;
	CCellInfo* pCell;
	tag = theCellBank.GetCellTag(jig, channel);
	if ( tag.IsExist() == FALSE )
		return FALSE;

	pCell = theCellBank.GetCellInfo(tag);
	if(pCell->m_OptionCheck3.m_Class == NONE_CELL)
		return FALSE;
	return TRUE;
}

void CUnitCtrlBank::OptionCheck3_TimeOut(JIG_ID jig)
{
	// 아직까지 판정이 없는 것들은 전부 NG판정해 버린다 [12/12/2016 OSC]
	CCellInfo* pCell;
	for(int i = 0; i < JIG_CH_MAX; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			if(pCell->m_OptionCheck3.m_Class == NONE_CELL)
			{
				pCell->m_OptionCheck3.m_Class = REJECT_CELL;
				pCell->m_OptionCheck3.m_DefectName = pCell->m_OptionCheck3.m_strCommand + TEXT_DEFECT_TIME_OUT;
				pCell->defaultData.m_bIsInspectionSkip = TRUE;
			}
		}
	}
}

void CUnitCtrlBank::OptionCheck4_Send(JIG_ID jig, JIG_CH channel/*= JIG_CH_MAX*/)
{
	if(theProcBank.m_bDryRunMode)
		return;
	if(channel == JIG_CH_MAX)
	{
		for (int i = 0; i < JIG_CH_MAX; i++)
		{
			if(theCellBank.GetCellTag(jig, (JIG_CH)i).IsExist())
			{
				CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)i));
				if(pCell->m_OptionCheck4.IsStarted() == FALSE)
				{
					if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
					{
						pCell->m_OptionCheck4.SetTimeStart();
						theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_OptionCheck4.m_strCommand, jig, (JIG_CH)i);
					}
					else
					{
						pCell->m_OptionCheck4.m_Class = SKIP_CELL;
						pCell->m_OptionCheck4.m_DefectName = SKIP;
					}
				}
			}
		}
	}
	else
	{
		if(theCellBank.GetCellTag(jig, (JIG_CH)channel).IsExist())
		{
			CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)channel));
			if(pCell->m_OptionCheck4.IsStarted() == FALSE)
			{
				if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
				{
					pCell->m_OptionCheck4.SetTimeStart();
					theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_OptionCheck4.m_strCommand, jig, (JIG_CH)channel);
				}
				else
				{
					pCell->m_OptionCheck4.m_Class = SKIP_CELL;
					pCell->m_OptionCheck4.m_DefectName = SKIP;
				}
			}
		}
	}

}

BOOL CUnitCtrlBank::OptionCheck4_Check(JIG_ID jig, JIG_CH channel)
{
	if(theProcBank.m_bDryRunMode)
		return TRUE;

	CCellTag	tag;
	CCellInfo* pCell;
	tag = theCellBank.GetCellTag(jig, channel);
	if ( tag.IsExist() == FALSE )
		return FALSE;

	pCell = theCellBank.GetCellInfo(tag);
	if(pCell->m_OptionCheck4.m_Class == NONE_CELL)
		return FALSE;
	return TRUE;
}

void CUnitCtrlBank::OptionCheck4_TimeOut(JIG_ID jig)
{
	// 아직까지 판정이 없는 것들은 전부 NG판정해 버린다 [12/12/2016 OSC]
	CCellInfo* pCell;
	for(int i = 0; i < JIG_CH_MAX; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			if(pCell->m_OptionCheck4.m_Class == NONE_CELL)
			{
				pCell->m_OptionCheck4.m_Class = REJECT_CELL;
				pCell->m_OptionCheck4.m_DefectName = pCell->m_OptionCheck4.m_strCommand + TEXT_DEFECT_TIME_OUT;
				pCell->defaultData.m_bIsInspectionSkip = TRUE;
			}
		}
	}
}

void CUnitCtrlBank::OptionCheck5_Send(JIG_ID jig, JIG_CH channel/*= JIG_CH_MAX*/)
{
	if(theProcBank.m_bDryRunMode)
		return;
	if(channel == JIG_CH_MAX)
	{
		for (int i = 0; i < JIG_CH_MAX; i++)
		{
			if(theCellBank.GetCellTag(jig, (JIG_CH)i).IsExist())
			{
				CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)i));
				if(pCell->m_OptionCheck5.IsStarted() == FALSE)
				{
					if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
					{
						pCell->m_OptionCheck5.SetTimeStart();
						theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_OptionCheck5.m_strCommand, jig, (JIG_CH)i);
					}
					else
					{
						pCell->m_OptionCheck5.m_Class = SKIP_CELL;
						pCell->m_OptionCheck5.m_DefectName = SKIP;
					}
				}
			}
		}
	}
	else
	{
		if(theCellBank.GetCellTag(jig, (JIG_CH)channel).IsExist())
		{
			CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)channel));
			if(pCell->m_OptionCheck5.IsStarted() == FALSE)
			{
				if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
				{
					pCell->m_OptionCheck5.SetTimeStart();
					theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_OptionCheck5.m_strCommand, jig, (JIG_CH)channel);
				}
				else
				{
					pCell->m_OptionCheck5.m_Class = SKIP_CELL;
					pCell->m_OptionCheck5.m_DefectName = SKIP;
				}
			}
		}
	}

}

BOOL CUnitCtrlBank::OptionCheck5_Check(JIG_ID jig, JIG_CH channel)
{
	if(theProcBank.m_bDryRunMode)
		return TRUE;

	CCellTag	tag;
	CCellInfo* pCell;
	tag = theCellBank.GetCellTag(jig, channel);
	if ( tag.IsExist() == FALSE )
		return FALSE;

	pCell = theCellBank.GetCellInfo(tag);
	if(pCell->m_OptionCheck5.m_Class == NONE_CELL)
		return FALSE;
	return TRUE;
}

void CUnitCtrlBank::OptionCheck5_TimeOut(JIG_ID jig)
{
	// 아직까지 판정이 없는 것들은 전부 NG판정해 버린다 [12/12/2016 OSC]
	CCellInfo* pCell;
	for(int i = 0; i < JIG_CH_MAX; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			if(pCell->m_OptionCheck5.m_Class == NONE_CELL)
			{
				pCell->m_OptionCheck5.m_Class = REJECT_CELL;
				pCell->m_OptionCheck5.m_DefectName = pCell->m_OptionCheck5.m_strCommand + TEXT_DEFECT_TIME_OUT;
				pCell->defaultData.m_bIsInspectionSkip = TRUE;
			}
		}
	}
}

void CUnitCtrlBank::OptionCheck6_Send(JIG_ID jig, JIG_CH channel/*= JIG_CH_MAX*/)
{
	if(theProcBank.m_bDryRunMode)
		return;
	if(channel == JIG_CH_MAX)
	{
		for (int i = 0; i < JIG_CH_MAX; i++)
		{
			if(theCellBank.GetCellTag(jig, (JIG_CH)i).IsExist())
			{
				CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)i));
				if(pCell->m_OptionCheck6.IsStarted() == FALSE)
				{
					if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
					{
						pCell->m_OptionCheck6.SetTimeStart();
						theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_OptionCheck6.m_strCommand, jig, (JIG_CH)i);
					}
					else
					{
						pCell->m_OptionCheck6.m_Class = SKIP_CELL;
						pCell->m_OptionCheck6.m_DefectName = SKIP;
					}
				}
			}
		}
	}
	else
	{
		if(theCellBank.GetCellTag(jig, (JIG_CH)channel).IsExist())
		{
			CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)channel));
			if(pCell->m_OptionCheck6.m_Class == NONE_CELL)
			{
				if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
				{
					pCell->m_OptionCheck6.SetTimeStart();
					theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_OptionCheck6.m_strCommand, jig, (JIG_CH)channel);
				}
				else
				{
					pCell->m_OptionCheck6.m_Class = SKIP_CELL;
					pCell->m_OptionCheck6.m_DefectName = SKIP;
				}
			}
		}
	}

}

BOOL CUnitCtrlBank::OptionCheck6_Check(JIG_ID jig, JIG_CH channel)
{
	if(theProcBank.m_bDryRunMode)
		return TRUE;

	CCellTag	tag;
	CCellInfo* pCell;
	tag = theCellBank.GetCellTag(jig, channel);
	if ( tag.IsExist() == FALSE )
		return FALSE;

	pCell = theCellBank.GetCellInfo(tag);
	if(pCell->m_OptionCheck6.m_Class == NONE_CELL)
		return FALSE;
	return TRUE;
}

void CUnitCtrlBank::OptionCheck6_TimeOut(JIG_ID jig)
{
	// 아직까지 판정이 없는 것들은 전부 NG판정해 버린다 [12/12/2016 OSC]
	CCellInfo* pCell;
	for(int i = 0; i < JIG_CH_MAX; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			if(pCell->m_OptionCheck6.m_Class == NONE_CELL)
			{
				pCell->m_OptionCheck6.m_Class = REJECT_CELL;
				pCell->m_OptionCheck6.m_DefectName = pCell->m_OptionCheck6.m_strCommand + TEXT_DEFECT_TIME_OUT;
				pCell->defaultData.m_bIsInspectionSkip = TRUE;
			}
		}
	}
}

void CUnitCtrlBank::OptionCheck7_Send(JIG_ID jig, JIG_CH channel/*= JIG_CH_MAX*/)
{
	if(theProcBank.m_bDryRunMode)
		return;
	if(channel == JIG_CH_MAX)
	{
		for (int i = 0; i < JIG_CH_MAX; i++)
		{
			if(theCellBank.GetCellTag(jig, (JIG_CH)i).IsExist())
			{
				CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)i));
				if(pCell->m_OptionCheck7.IsStarted() == FALSE)
				{
					if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
					{
						pCell->m_OptionCheck7.SetTimeStart();
						theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_OptionCheck7.m_strCommand, jig, (JIG_CH)i);
					}
					else
					{
						pCell->m_OptionCheck7.m_Class = SKIP_CELL;
						pCell->m_OptionCheck7.m_DefectName = SKIP;
					}
				}
			}
		}
	}
	else
	{
		if(theCellBank.GetCellTag(jig, (JIG_CH)channel).IsExist())
		{
			CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)channel));
			if(pCell->m_OptionCheck7.IsStarted() == FALSE)
			{
				if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
				{
					pCell->m_OptionCheck7.SetTimeStart();
					theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_OptionCheck7.m_strCommand, jig, (JIG_CH)channel);
				}
				else
				{
					pCell->m_OptionCheck7.m_Class = SKIP_CELL;
					pCell->m_OptionCheck7.m_DefectName = SKIP;
				}
			}
		}
	}

}

BOOL CUnitCtrlBank::OptionCheck7_Check(JIG_ID jig, JIG_CH channel)
{
	if(theProcBank.m_bDryRunMode)
		return TRUE;

	CCellTag	tag;
	CCellInfo* pCell;
	tag = theCellBank.GetCellTag(jig, channel);
	if ( tag.IsExist() == FALSE )
		return FALSE;

	pCell = theCellBank.GetCellInfo(tag);
	if(pCell->m_OptionCheck7.m_Class == NONE_CELL)
		return FALSE;
	return TRUE;
}

void CUnitCtrlBank::OptionCheck7_TimeOut(JIG_ID jig)
{
	// 아직까지 판정이 없는 것들은 전부 NG판정해 버린다 [12/12/2016 OSC]
	CCellInfo* pCell;
	for(int i = 0; i < JIG_CH_MAX; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			if(pCell->m_OptionCheck7.m_Class == NONE_CELL)
			{
				pCell->m_OptionCheck7.m_Class = REJECT_CELL;
				pCell->m_OptionCheck7.m_DefectName = pCell->m_OptionCheck7.m_strCommand + TEXT_DEFECT_TIME_OUT;
				pCell->defaultData.m_bIsInspectionSkip = TRUE;
			}
		}
	}
}

void CUnitCtrlBank::OptionCheck8_Send(JIG_ID jig, JIG_CH channel/*= JIG_CH_MAX*/)
{
	if(theProcBank.m_bDryRunMode)
		return;
	if(channel == JIG_CH_MAX)
	{
		for (int i = 0; i < JIG_CH_MAX; i++)
		{
			if(theCellBank.GetCellTag(jig, (JIG_CH)i).IsExist())
			{
				CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)i));
				if(pCell->m_OptionCheck8.IsStarted() == FALSE)
				{
					if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
					{
						pCell->m_OptionCheck8.SetTimeStart();
						theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_OptionCheck8.m_strCommand, jig, (JIG_CH)i);
					}
					else
					{
						pCell->m_OptionCheck8.m_Class = SKIP_CELL;
						pCell->m_OptionCheck8.m_DefectName = SKIP;
					}
				}
			}
		}
	}
	else
	{
		if(theCellBank.GetCellTag(jig, (JIG_CH)channel).IsExist())
		{
			CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)channel));
			if(pCell->m_OptionCheck8.IsStarted() == FALSE)
			{
				if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
				{
					pCell->m_OptionCheck8.SetTimeStart();
					theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_OptionCheck8.m_strCommand, jig, (JIG_CH)channel);
				}
				else
				{
					pCell->m_OptionCheck8.m_Class = SKIP_CELL;
					pCell->m_OptionCheck8.m_DefectName = SKIP;
				}
			}
		}
	}

}

BOOL CUnitCtrlBank::OptionCheck8_Check(JIG_ID jig, JIG_CH channel)
{
	if(theProcBank.m_bDryRunMode)
		return TRUE;

	CCellTag	tag;
	CCellInfo* pCell;
	tag = theCellBank.GetCellTag(jig, channel);
	if ( tag.IsExist() == FALSE )
		return FALSE;

	pCell = theCellBank.GetCellInfo(tag);
	if(pCell->m_OptionCheck8.m_Class == NONE_CELL)
		return FALSE;
	return TRUE;
}

void CUnitCtrlBank::OptionCheck8_TimeOut(JIG_ID jig)
{
	// 아직까지 판정이 없는 것들은 전부 NG판정해 버린다 [12/12/2016 OSC]
	CCellInfo* pCell;
	for(int i = 0; i < JIG_CH_MAX; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			if(pCell->m_OptionCheck8.m_Class == NONE_CELL)
			{
				pCell->m_OptionCheck8.m_Class = REJECT_CELL;
				pCell->m_OptionCheck8.m_DefectName = pCell->m_OptionCheck8.m_strCommand + TEXT_DEFECT_TIME_OUT;
				pCell->defaultData.m_bIsInspectionSkip = TRUE;
			}
		}
	}
}

void CUnitCtrlBank::OptionCheck9_Send(JIG_ID jig, JIG_CH channel/*= JIG_CH_MAX*/)
{
	if(theProcBank.m_bDryRunMode)
		return;
	if(channel == JIG_CH_MAX)
	{
		for (int i = 0; i < JIG_CH_MAX; i++)
		{
			if(theCellBank.GetCellTag(jig, (JIG_CH)i).IsExist())
			{
				CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)i));
				if(pCell->m_OptionCheck9.IsStarted() == FALSE)
				{
					if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
					{
						pCell->m_OptionCheck9.SetTimeStart();
						theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_OptionCheck9.m_strCommand, jig, (JIG_CH)i);
					}
					else
					{
						pCell->m_OptionCheck9.m_Class = SKIP_CELL;
						pCell->m_OptionCheck9.m_DefectName = SKIP;
					}
				}
			}
		}
	}
	else
	{
		if(theCellBank.GetCellTag(jig, (JIG_CH)channel).IsExist())
		{
			CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)channel));
			if(pCell->m_OptionCheck9.IsStarted() == FALSE)
			{
				if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
				{
					pCell->m_OptionCheck9.SetTimeStart();
					theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_OptionCheck9.m_strCommand, jig, (JIG_CH)channel);
				}
				else
				{
					pCell->m_OptionCheck9.m_Class = SKIP_CELL;
					pCell->m_OptionCheck9.m_DefectName = SKIP;
				}
			}
		}
	}

}

BOOL CUnitCtrlBank::OptionCheck9_Check(JIG_ID jig, JIG_CH channel)
{
	if(theProcBank.m_bDryRunMode)
		return TRUE;

	CCellTag	tag;
	CCellInfo* pCell;
	tag = theCellBank.GetCellTag(jig, channel);
	if ( tag.IsExist() == FALSE )
		return FALSE;

	pCell = theCellBank.GetCellInfo(tag);
	if(pCell->m_OptionCheck9.m_Class == NONE_CELL)
		return FALSE;
	return TRUE;
}

void CUnitCtrlBank::OptionCheck9_TimeOut(JIG_ID jig)
{
	// 아직까지 판정이 없는 것들은 전부 NG판정해 버린다 [12/12/2016 OSC]
	CCellInfo* pCell;
	for(int i = 0; i < JIG_CH_MAX; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			if(pCell->m_OptionCheck9.m_Class == NONE_CELL)
			{
				pCell->m_OptionCheck9.m_Class = REJECT_CELL;
				pCell->m_OptionCheck9.m_DefectName = pCell->m_OptionCheck9.m_strCommand + TEXT_DEFECT_TIME_OUT;
				pCell->defaultData.m_bIsInspectionSkip = TRUE;
			}
		}
	}
}

void CUnitCtrlBank::OptionCheck10_Send(JIG_ID jig, JIG_CH channel/*= JIG_CH_MAX*/)
{
	if(theProcBank.m_bDryRunMode)
		return;
	if(channel == JIG_CH_MAX)
	{
		for (int i = 0; i < JIG_CH_MAX; i++)
		{
			if(theCellBank.GetCellTag(jig, (JIG_CH)i).IsExist())
			{
				CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)i));
				if(pCell->m_OptionCheck10.IsStarted() == FALSE)
				{
					if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
					{
						pCell->m_OptionCheck10.SetTimeStart();
						theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_OptionCheck10.m_strCommand, jig, (JIG_CH)i);
					}
					else
					{
						pCell->m_OptionCheck10.m_Class = SKIP_CELL;
						pCell->m_OptionCheck10.m_DefectName = SKIP;
					}
				}
			}
		}
	}
	else
	{
		if(theCellBank.GetCellTag(jig, (JIG_CH)channel).IsExist())
		{
			CCellInfo *pCell = theCellBank.GetCellInfo(theCellBank.GetCellTag(jig, (JIG_CH)channel));
			if(pCell->m_OptionCheck10.IsStarted() == FALSE)
			{
				if(pCell->defaultData.m_bIsInspectionSkip == FALSE)
				{
					pCell->m_OptionCheck10.SetTimeStart();
					theSocketInterFace.m_PGHost.SendMassageToPatternGenerator(pCell->m_OptionCheck10.m_strCommand, jig, (JIG_CH)channel);
				}
				else
				{
					pCell->m_OptionCheck10.m_Class = SKIP_CELL;
					pCell->m_OptionCheck10.m_DefectName = SKIP;
				}
			}
		}
	}

}

BOOL CUnitCtrlBank::OptionCheck10_Check(JIG_ID jig, JIG_CH channel)
{
	if(theProcBank.m_bDryRunMode)
		return TRUE;

	CCellTag	tag;
	CCellInfo* pCell;
	tag = theCellBank.GetCellTag(jig, channel);
	if ( tag.IsExist() == FALSE )
		return FALSE;

	pCell = theCellBank.GetCellInfo(tag);
	if(pCell->m_OptionCheck10.m_Class == NONE_CELL)
		return FALSE;
	return TRUE;
}

void CUnitCtrlBank::OptionCheck10_TimeOut(JIG_ID jig)
{
	// 아직까지 판정이 없는 것들은 전부 NG판정해 버린다 [12/12/2016 OSC]
	CCellInfo* pCell;
	for(int i = 0; i < JIG_CH_MAX; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			if(pCell->m_OptionCheck10.m_Class == NONE_CELL)
			{
				pCell->m_OptionCheck10.m_Class = REJECT_CELL;
				pCell->m_OptionCheck10.m_DefectName = pCell->m_OptionCheck10.m_strCommand + TEXT_DEFECT_TIME_OUT;
				pCell->defaultData.m_bIsInspectionSkip = TRUE;
			}
		}
	}
}

BOOL CUnitCtrlBank::CalcurateDefectSquare(JIG_ID jig)
{
	BOOL bRet = FALSE;
// 	theProcBank.ClearSquareData(jig);
// 	DEFECT_PATTERN pattern;
// 	SQUARE_POINT point;
// 	CCellInfo *pCell;
// 	ZONE_ID otherZone;
// 	if(zone == ZONE_ID_C)
// 		otherZone = ZONE_ID_D;
// 	else
// 		otherZone = ZONE_ID_C;
// 	for(int ch = 0; ch < JIG_CH_MAX; ch++)
// 	{
// 		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)ch);
// 		if(pCell)
// 		{
// 			if(pCell->m_nDefectPointCount)
// 			{
// 				if(pCell->defaultCellInfomation.m_bIsInspectionSkip)
// 				{
// 					theLog[LOG_SPECIAL_PROCESS].AddBuf(_T("[%cZONE][JIG%c][Ch%d] CellID : %s is Square Skip"),
// 						jig+_T('A'), pCell->defaultCellInfomation.m_JigId+_T('A'), ch+1, pCell->defaultCellInfomation.m_strCellID);
// 				}
// 				else
// 				{
// 					theLog[LOG_SPECIAL_PROCESS].AddBuf(_T("[%cZONE][JIG%c][Ch%d] CellID : %s Defect Square Count %d"),
// 						jig+_T('A'), pCell->defaultCellInfomation.m_JigId+_T('A'), ch+1, pCell->defaultCellInfomation.m_strCellID, pCell->m_nDefectPointCount);
// 
// 					// 상위에서 받은 불량좌표의 불량명 가지고...
// 					for(int j = 0; j < pCell->m_nDefectPointCount; j++)
// 					{
// 						// 패턴파일에서 해당하는 패턴을 불러와서....
// 						// 각 패턴별로 그려야 할 좌표를 취합한다
// 						point.nX = pCell->m_DefectPoint[j].nX;
// 						point.nY = pCell->m_DefectPoint[j].nY;
// 
// 						if(theConfigBank.m_DefectPattern.FindPattern(pCell->m_DefectPoint[j], pattern, ZONE))
// 						{
// 							point.colorLine = pattern.colorLine;
// 							point.nThickness = pattern.nThickness;
// 							point.nSize = pattern.nSize;
// 
// 							theProcBank.AddDZoneSquareData((JIG_CH)ch, pattern.nPatternNo, point, jig);
// 							theLog[LOG_SPECIAL_PROCESS].AddBuf(_T("[%cZONE][JIG%c][Ch%d] %dth Defect[%s] Square color : 0x%02X%02X%02X, Thickness : %d Size : %d Pattern : %d X : %d, Y : %d"),
// 								jig+_T('A'), pCell->defaultCellInfomation.m_JigId+_T('A'), ch+1, j,
// 								pCell->m_DefectPoint[j].strDefectName,
// 								GetRValue(point.colorLine), GetGValue(point.colorLine), GetBValue(point.colorLine),
// 								point.nThickness, point.nSize, pattern.nPatternNo,
// 								point.nX, point.nY);
// 							bRet = TRUE;
// 						}
// 						else
// 						{
// 							if( theConfigBank.m_DefectPattern.FindPattern(pCell->m_DefectPoint[j], pattern, otherZone) )
// 							{
// 								// 다른존에 있는 거면 SKIP
// 							}
// 							else
// 							{
// 								// 패턴파일에 없으면 201패턴을 띄운다
// 								pattern = theConfigBank.m_DefectPattern.m_UndefinePattern;
// 								point.colorLine = pattern.colorLine;
// 								point.nThickness = pattern.nThickness;
// 								point.nSize = pattern.nSize;
// 
// 								theProcBank.AddDZoneSquareData((JIG_CH)ch, pattern.nPatternNo, point, zone);
// 								theLog[LOG_SPECIAL_PROCESS].AddBuf(_T("[%cZONE][JIG%c][Ch%d] %dth Defect Not Found [%s|%s]"),
// 									jig+_T('A'), pCell->defaultCellInfomation.m_JigId+_T('A'), ch+1, j, 
// 									pCell->m_DefectPoint[j].strDefectName,
// 									pCell->m_DefectPoint[j].strPatternName);
// 								bRet = TRUE;
// 							}
// 						}
// 					}
// 				}
// 			}
// 		}
// 	}

	// 불량좌표 검사할게 있으면 TRUE
	return bRet;
}

BOOL CUnitCtrlBank::SendDefectSquare(int nIndex, JIG_ID jig)
{
	CCellInfo *pCell;
	SQUARE_POINT point;
	POSITION pos;
	int nPatternNo;
	BOOL bContinue = FALSE;
	for(int i = 0; i < JIG_CH_MAX; i++)
	{
		pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);
		if(pCell)
		{
			if(pCell->defaultData.m_bIsInspectionSkip)
			{
				// 불량은 아무것도 안한다
			}
			else if(theProcBank.m_nSquarePatternCnt[jig][i] >= nIndex+1)
			{
				// 좌표가 있는 경우 먼저 배경 패턴을 띄우고...
				nPatternNo = theProcBank.m_SquarePatternInfo[jig][i][nIndex].nPatternNo;
				theSocketInterFace.m_PGHost.ShowDZonePattern(nPatternNo, pCell->defaultData.m_JigId, pCell->defaultData.m_JigCh);

				// 그려야 할 좌표를 추가한다
				pos = theProcBank.m_SquarePatternInfo[jig][i][nIndex].listSquarePoint.GetHeadPosition();
				while(pos)
				{
					point = theProcBank.m_SquarePatternInfo[jig][i][nIndex].listSquarePoint.GetNext(pos);
					theSocketInterFace.m_PGHost.AddPGSquare(point);
				}

				// 추가했던 좌표 Draw
				theSocketInterFace.m_PGHost.DrawPGSquare(pCell->defaultData.m_JigId, pCell->defaultData.m_JigCh);

				// 추가했던거 Clear
				theSocketInterFace.m_PGHost.ClearPGSqueare();

				theLog[LOG_SPECIAL_PROCESS].AddBuf(_T("[%cZONE][JIG%c][Ch%d] CellID : %s Defect Square Draw"),
					jig+_T('A'), pCell->defaultData.m_JigId+_T('A'), i+1, pCell->defaultData.m_strCellID);

				// 다음 패턴도 있는지 확인. 5개 채널중 하나라도 다음 패턴이 있으면 TRUE
				if(theProcBank.m_nSquarePatternCnt[jig][i] >= nIndex+2)
				{
					bContinue = TRUE;
				}
			}
			else
			{
				// 좌표가 없으면 'NO CONFIRM' 메세지를 띄운다
				theSocketInterFace.m_PGHost.AddPGMsg(_T("NO CONFIRM"));
				theSocketInterFace.m_PGHost.DrawPGMsg(pCell->defaultData.m_JigId, pCell->defaultData.m_JigCh, 
					RGB(0,0,0), RGB(255,255,255), 20, 20, 80);
				theSocketInterFace.m_PGHost.ClearPGMsg();
			}
		}
	}

	return bContinue;
}

CString CUnitCtrlBank::GetCellID(JIG_CH nCh, JIG_ID jig)
{
	CCellTag	tag;
	CCellInfo* pCell;
	BOOL bRet = FALSE;
	tag = theCellBank.GetCellTag(jig, nCh);
	if ( tag.IsExist() == FALSE )
		return TEXT_FAIL;

	pCell = theCellBank.GetCellInfo(tag);
	return pCell->defaultData.m_strCellID;
}

BOOL CUnitCtrlBank::CheckTMDnProductIDMatch(JIG_ID jig, JIG_CH ch)
{

	CString strBuf;
	BOOL bResult = FALSE;

	// TMD 파일명과 ProductID 앞 10글자(AMB632NF01)와 비교한다
	CCellInfo *pCell;
	pCell = theCellBank.GetCellInfo(jig, ch);

	if(pCell == NULL)
		return TRUE;


	//pCell->defaultData.m_strProductID.Format(_T("AMB622NP13-012"));
	//20171001 BKH, 혼류 발생시에는 bin2처리 하도록 해야 하므로 변경한다.
	//kjpark 20180120 Cell Mixing 처리(강원호 선임)
	if(theConfigBank.m_Option.m_bUseTMDNameMatch == FALSE
		/*|| theConfigBank.m_Option.m_bUseCellMixingA3A4 == FALSE*/)
	return TRUE;
// 	pCell->defaultData.m_strProductID = _T("AMB622NP06-006");
// 	pCell->defaultData.m_strTMD_Info = _T("A3-AMB622NP06-V3-AMTP-00-EIN-180110.TMD");
	// MCR Fail 등으로 인해 상위로부터 정보 못받은거면 일단 스킵
	if(pCell->defaultData.m_strProductID.GetLength() < 10)
		return TRUE;

	// TMD 정보를 못받은 셀도 스킵
	if(pCell->defaultData.m_bReceive_TMD_Info != TMD_INFO_RECEIVE)
		return TRUE;

	//kjpark 2018018 TMD MACHING 특정 ProductID일때 전문자열 비교
	CString strKeyword;
	strKeyword=pCell->defaultData.m_strTMD_Info.Left(5);
	//20180313 Cell Mixing 처리(강원호 선임) 요청사항
	//kjpark 2018018 TMD MACHING 특정 ProductID일때 전문자열 비교<--- 박경진 과장님이 코딩한 부분을 대폭 수정
	//20180312 BKH, CellID 앞 두글자와 TMD간 Fab Site가 동일하면?
	if(strKeyword.Find(pCell->defaultData.m_strCellID.Left(2))>= 0)
	{
		//int test;
		//20180312 BKH, 동일하다면 TMD의 Fab Site가 2개인가? 2개일 경우는 Ax-Ax 이런식으로 -A1,-A2 이런식의 숫자값이 들어가 있으므로... (A1~A9까지의 상황이 올수 있다.)
		if(strKeyword.Find(_T("-A1"))>= 0 || strKeyword.Find(_T("-A2"))>= 0 || strKeyword.Find(_T("-A3"))>= 0 || strKeyword.Find(_T("-A4"))>= 0 || strKeyword.Find(_T("-A5"))>= 0
			|| strKeyword.Find(_T("-A6"))>= 0 || strKeyword.Find(_T("-A7"))>= 0 || strKeyword.Find(_T("-A8"))>= 0 || strKeyword.Find(_T("-A9"))>= 0)
		{//2개일 경우에는 ProductID의 14자리와 TMD 모델명의 영업코드가 동일한가?
			strKeyword =  pCell->defaultData.m_strProductID.Left(14);
			if(pCell->defaultData.m_strTMD_Info.Find(strKeyword)>=0)//14자리의 ProductID 값이 TMD내에도 동일하게 있다면?
			{
				return TRUE;
			}
			else
			{//동일하지 않을시에는 return false;
				return FALSE;
			}
		}
		else//TMD의 fab site가 2개가 아니면
		{
			//tmd에 영업코드가 있는가?
			//영업코드 -003:v1, -005:v3, -006 v3, -008 v3, -010 v3, -012 v3, -013 v1
			//영업코드가 TMD에 있는지를 검색, 만약 동별로도 구분을 해줘야 할 경우 위에 보이는 해당 동들 외에 나머지 동은 아래 if문 조건문에서 삭제하면 됨
			//if(pCell->defaultData.m_strTMD_Info.Find(_T("-003"))>=0 || pCell->defaultData.m_strTMD_Info.Find(_T("-005"))>=0 || pCell->defaultData.m_strTMD_Info.Find(_T("-006"))>=0
			//	|| pCell->defaultData.m_strTMD_Info.Find(_T("-008"))>=0 || pCell->defaultData.m_strTMD_Info.Find(_T("-010"))>=0 || pCell->defaultData.m_strTMD_Info.Find(_T("-012"))>=0 || pCell->defaultData.m_strTMD_Info.Find(_T("-013"))>=0)
			
			for(int i = 0;  i< 100; i++)
			{
				strBuf.Format(_T("-%03d-"),i);
				if(pCell->defaultData.m_strTMD_Info.Find(strBuf)>=0)
				{
					bResult = TRUE;
					break;
				}			
			}

			if(bResult)
			//(pCell->defaultData.m_strTMD_Info.Find(_T("-0"))>=0)//20180314 BKH, 영업코드가 -001~-099까지 올수가 있으므로..
			{

				strKeyword =  pCell->defaultData.m_strProductID.Left(14);
				if(pCell->defaultData.m_strTMD_Info.Find(strKeyword)>=0)//14자리의 ProductID 값이 TMD내에도 동일하게 있다면?
				{//ture return;
					strKeyword =  strKeyword.Right(3);//Product ID의 마지막 3자리가 영업코드를 말하므로
					if(pCell->defaultData.m_strTMD_Info.Find(strKeyword)>=0)//Product의 영업코드를 TMD내에서 찾아본다.
					{
						return TRUE;
					}
					else
					{
						return FALSE;
					}
					
				}
				else
				{//동일하지 않을시에는 return false;
					return FALSE;
				}
			}
			else
			{
				//영업코드가 TMD상에 존재하지 않는다면 ProductID의 10자리와 TMD의 모델명이 동일한가?
				strKeyword = pCell->defaultData.m_strProductID.Left(10);
				if( pCell->defaultData.m_strTMD_Info.Find(strKeyword)>=0)
				{
					//동일하면 return true
					return TRUE;
				}
				else
				{
					return FALSE;
				}
			}

		}
	}
	else//20180312 BKH, CellID 앞 두글자와 TMD간 Fab Site가 일치하지 않는다면 return false;
	{
		return FALSE;

	}
	//20180313 Cell Mixing 주석
	////kjpark 20180120 Cell Mixing 처리(강원호 선임)
	//strKeyword = pCell->defaultData.m_strProductID.Left(10);

	//if(pCell->defaultData.m_strTMD_Info.Find(strKeyword) >= 0)
	//{
	//	if(pCell->defaultData.m_strTMD_Info.Find(_T("A3-A4")) >= 0)
	//	{
	//		//kjpark 20180122 Cell Mixing 처리(강원호 선임) 005도 추가
	//		//kjpark 20180123 Cell Mixing 처리(강원호 선임) 010도 추가
	//		//cdtruong 20180214 Cell Mixing 처리(사귀진 선임) 012도 추가
	//		if(pCell->defaultData.m_strProductID.Find(_T("AMB622NP")) >= 0)
	//		{
	//			if((pCell->defaultData.m_strProductID.Find(_T("-008")) >= 0)
	//				|| (pCell->defaultData.m_strProductID.Find(_T("-005")) >= 0)
	//				|| (pCell->defaultData.m_strProductID.Find(_T("-010")) >= 0)
	//				|| (pCell->defaultData.m_strProductID.Find(_T("-012")) >= 0))
	//			{
	//				return TRUE;
	//			}
	//			else
	//			{
	//				return FALSE;
	//			}
	//		}
	//	}
	//	else
	//	{
	//		//kjpark 20180122 Cell Mixing 처리(강원호 선임) 005도 추가
	//		//kjpark 20180123 Cell Mixing 처리(강원호 선임) 010도 추가
	//		//cdtruong 20180214 Cell Mixing 처리(사귀진 선임) 012도 추가
	//		if(pCell->defaultData.m_strProductID.Find(_T("AMB622NP")) >= 0)
	//		{
	//			if((pCell->defaultData.m_strProductID.Find(_T("-008")) >= 0)
	//				|| (pCell->defaultData.m_strProductID.Find(_T("-005")) >= 0)
	//				|| (pCell->defaultData.m_strProductID.Find(_T("-010")) >= 0)
	//				|| (pCell->defaultData.m_strProductID.Find(_T("-012")) >= 0))
	//			{
	//				return FALSE;
	//			}
	//			else
	//			{
	//				return TRUE;
	//			}
	//		}
	//	}
	//}
	//else
	//{
	//	return FALSE;
	//}

	return TRUE;
}

BOOL CUnitCtrlBank::CheckTMDnCellIDMatch(JIG_ID jig, JIG_CH ch)
{
	// TMD 파일명과 CellID 앞 2글자(A3)와 비교한다
	CCellInfo *pCell;
	pCell = theCellBank.GetCellInfo(jig, ch);

	if(pCell == NULL)
		return TRUE;

	//kjpark 20171010 TMD March 구현
	if(theConfigBank.m_Option.m_bUseTMDNameMatch == FALSE)
		return TRUE;

	// Cell ID 못읽은 셀이면 스킵
	if(pCell->defaultData.m_bMCR_OK == FALSE)
		return TRUE;

	// TMD 정보를 못받은 셀도 스킵
	if(pCell->defaultData.m_bReceive_TMD_Info != TMD_INFO_RECEIVE)
		return TRUE;

	if(pCell->defaultData.m_strTMD_Info.Left(2) == pCell->defaultData.m_strCellID.Left(2))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


BOOL CUnitCtrlBank::CellInfo_CheckLoadable(JIG_ID jig)
{
	// 해당 지그가 로봇한테서 셀을 받을 상태인지 확인
	// MP2100 함수 하나하나가 상당히 오래걸려 주석 [10/15/2017 OSC]
// 	if(Shuttle_Y_LOAD_Check(jig) == FALSE)
// 		return FALSE;
	if(theProcBank.AZoneChannelNotUse_Check(jig, JIG_CH_1) == FALSE)
	{
		if(CellTagExist(jig, JIG_CH_1) == FALSE)
			return TRUE;
	}
	return FALSE;
}

BOOL CUnitCtrlBank::CellInfo_CheckUnloadable( JIG_ID jig, JIG_CH ch /*= JIG_CH_MAX*/ )
{
	// 해당 지그가 로봇한테서 셀을 받을 상태인지 확인
	// MP2100 함수 하나하나가 상당히 오래걸려 주석 [10/15/2017 OSC]
// 	if(Shuttle_Y_LOAD_Check(jig) == FALSE)
// 		return FALSE;

	if(ch == JIG_CH_MAX)
	{
		for(int i = 0; i < JIG_CH_MAX; i++)
		{
			if(theProcBank.AZoneChannelNotUse_Check(jig, (JIG_CH)i) == FALSE)
			{
				CCellInfo *pCell;
				pCell = theCellBank.GetCellInfo(jig, (JIG_CH)i);

				if(pCell)
				{
					if(pCell->defaultData.m_LastClass != NONE_CELL)
						return TRUE;
				}
			}
		}
	}
	else
	{
		if(theProcBank.AZoneChannelNotUse_Check(jig, ch) == FALSE)
		{
			CCellInfo *pCell;
			pCell = theCellBank.GetCellInfo(jig, ch);

			if(pCell)
			{
				if(pCell->defaultData.m_LastClass != NONE_CELL)
					return TRUE;
			}
		}
	}


	return FALSE;
}

//kjpark 20161027 IO Output check bug 수정
BOOL CUnitCtrlBank::GetInPutIOCheck(INPUT_ID ID)
{
	return theIOBank.GetInputIOParma((INPUT_ID)ID).GetIOCheck(theDeviceIO.ReadInBit(ID));
}

BOOL CUnitCtrlBank::GetInPutIOCheck( INPUT_ID ID, ONOFF bValue )
{
	return (theDeviceIO.ReadInBit(ID) == theIOBank.GetInputIOParma(ID).GetIOCheck(bValue)) ? TRUE:FALSE;
}

//kjpark 20161027 IO Output check bug 수정
BOOL CUnitCtrlBank::GetOutPutIOCheck(OUTPUT_ID ID)
{
	return theIOBank.GetOutputIOParma((OUTPUT_ID)ID).GetIOCheck(theDeviceIO.ReadOutBit(ID));
}

BOOL CUnitCtrlBank::GetOutPutIOCheck( OUTPUT_ID ID, ONOFF bValue )
{
	return (theDeviceIO.ReadOutBit(ID) == theIOBank.GetOutputIOParma(ID).GetIOCheck(bValue)) ? TRUE:FALSE;
}

//kjpark 20161027 IO Output check bug 수정
void CUnitCtrlBank::SetOutPutIO(OUTPUT_ID ID, BOOL bValue)
{
	theDeviceIO.WriteOutBit((OUTPUT_ID)ID, theIOBank.GetOutputIOParma((OUTPUT_ID)ID).GetIOCheck(bValue));		;
}

//kjpark 20161027 IO Output check bug 수정
void CUnitCtrlBank::SetOutPutIO(OUTPUT_ID ID, ONOFF bValue)
{
	theDeviceIO.WriteOutBit((OUTPUT_ID)ID, theIOBank.GetOutputIOParma((OUTPUT_ID)ID).GetIOCheck(bValue));		;
}

//kjpark 20161019 양수버튼 체크
BOOL CUnitCtrlBank::ReadySwitch_Check( JIG_ID jig )
{	
	if(theProcBank.m_bDryRunMode)
		return TRUE;
	if(LightCurtain_Check(jig) == FALSE)
		return FALSE;

	return theProcBank.m_bAZoneReadyPressed[jig];
}

BOOL CUnitCtrlBank::AutoTeachKey_AutoCheck(BOOL bAlarm /*= FALSE*/)
{
	if(bAlarm == FALSE)
	{
		theProcBank.m_strLastKorMsg = _T("");
		theProcBank.m_strLastEngMsg = _T("");
		theProcBank.m_strLastVnmMsg = _T("");
	}
	BOOL bRet = TRUE;

	//20170912 hhkim AutoTeachKey_TeachCheck
	if(theUnitFunc.GetInPutIOCheck(X_AUTO_TEACH_KEY))
	{
		if(bAlarm)
		{
			theProcBank.AlarmHappen(ALM_SAFTY_KEY_TEACH);
		}
		else
		{
			theProcBank.m_strLastKorMsg = _T("auto Teach key가 Teach mode 입니다.");	//한
			theProcBank.m_strLastEngMsg = _T("The auto teach key is Teach mode.");			//영
			theProcBank.m_strLastVnmMsg =_T("Auto Teach key đang ở chế độ Teach mode") ;				//베
		}
		bRet = FALSE;
	}

	return bRet;
}
BOOL CUnitCtrlBank::AutoTeachKey_TeachCheck()
{
	theProcBank.m_strLastKorMsg = _T("");
	theProcBank.m_strLastEngMsg = _T("");
	theProcBank.m_strLastVnmMsg = _T("");
	BOOL bRet = TRUE;

	//20170912 hhkim AutoTeachKey_AutoCheck
	if(theUnitFunc.GetInPutIOCheck(X_AUTO_TEACH_KEY)==FALSE)
	{
		theProcBank.m_strLastKorMsg = _T("auto Teach key가 현재 모두 auto mode 입니다.");	//한
		theProcBank.m_strLastEngMsg = _T("The auto teach key is all Auto mode.");			//영
		theProcBank.m_strLastVnmMsg =_T("Auto Teach Key hiện tại tất cả đang chế độ Auto Mode") ;				//베
		bRet = FALSE;
	}
	return bRet;
	//return TRUE;
}


BOOL CUnitCtrlBank::DoorClose_Check(BOOL bAlarm /*= FALSE*/)
{
	if(bAlarm == FALSE)
	{
		theProcBank.m_strLastKorMsg = _T("");
		theProcBank.m_strLastEngMsg = _T("");
		theProcBank.m_strLastVnmMsg = _T("");
	}
	BOOL bRet = TRUE;

	if(theUnitFunc.GetInPutIOCheck(X_FRONT_DOOR1_SENSOR))
	{
		if(bAlarm)
		{
			theProcBank.AlarmHappen(ALM_SHUTTLE1_LIGHT_CURTAIN);
		}
		else
		{
			theProcBank.m_strLastKorMsg = _T("FRONT DOOR1 가 열렸습니다.");		//한
			theProcBank.m_strLastEngMsg = _T("FRONT DOOR1 door opened.");			//영
			theProcBank.m_strLastVnmMsg =_T("Cửa phía trước số 1 đã bị mở.") ;		//베
		}
 		bRet = FALSE;
	}

	if(theUnitFunc.GetInPutIOCheck(X_FRONT_DOOR2_SENSOR))
	{
		if(bAlarm)
		{
			theProcBank.AlarmHappen(ALM_FRONT_DOOR2_SENSOR);
		}
		else
		{
			theProcBank.m_strLastKorMsg = _T("FRONT DOOR2 가 열렸습니다.");			//한
			theProcBank.m_strLastEngMsg = _T("FRONT DOOR2 door opened.");			//영
			theProcBank.m_strLastVnmMsg =_T("Cửa phía trước số 2 đã bị mở.") ;		//베
		}
		bRet = FALSE;
	}

	if(theUnitFunc.GetInPutIOCheck(X_RIGNT_DOOR_SENSOR))
	{
		if(bAlarm)
		{
			theProcBank.AlarmHappen(ALM_RIGHT_DOOR_SENSOR);
		}
		else
		{
			theProcBank.m_strLastKorMsg = _T("RIGNT DOOR 가 열렸습니다.");			//한
			theProcBank.m_strLastEngMsg = _T("RIGNT DOOR door opened.");			//영
			theProcBank.m_strLastVnmMsg =_T("Cửa bên phải đã bị mở.") ;			//베
		}
		bRet = FALSE;
	}

	if(theUnitFunc.GetInPutIOCheck(X_BACK_DOOR1_SENSOR))
	{
		if(bAlarm)
		{
			theProcBank.AlarmHappen(ALM_BACK_DOOR1_SENSOR);
		}
		else
		{
			theProcBank.m_strLastKorMsg = _T("BACK DOOR1 가 열렸습니다.");			//한
			theProcBank.m_strLastEngMsg = _T("BACK DOOR1 door opened.");			//영
			theProcBank.m_strLastVnmMsg =_T("Cửa phía sau số 1 đã bị mở.") ;		//베
		}
		bRet = FALSE;
	}

	if(theUnitFunc.GetInPutIOCheck(X_BACK_DOOR2_SENSOR))
	{
		if(bAlarm)
		{
			theProcBank.AlarmHappen(ALM_BACK_DOOR2_SENSOR);
		}
		else
		{
			theProcBank.m_strLastKorMsg = _T("BACK DOOR2 가 열렸습니다.");			//한
			theProcBank.m_strLastEngMsg = _T("BACK DOOR2 door opened.");			//영
			theProcBank.m_strLastVnmMsg =_T("Cửa phía sau số 2 đã bị mở.") ;		//베
		}
		bRet = FALSE;
	}

	if(theUnitFunc.GetInPutIOCheck(X_LEFT_DOOR_SENSOR))
	{
		if(bAlarm)
		{
			theProcBank.AlarmHappen(ALM_LEFT_DOOR_SENSOR);
		}
		else
		{
			theProcBank.m_strLastKorMsg = _T("LEFT DOOR 가 열렸습니다.");			//한
			theProcBank.m_strLastEngMsg = _T("LEFT DOOR door opened.");				//영
			theProcBank.m_strLastVnmMsg =_T("Cửa bên trái đã bị mở.") ;	//베
		}
		bRet = FALSE;
	}

	return bRet;
}


BOOL CUnitCtrlBank::DoorLockOn_Check()
{
	theProcBank.m_strLastKorMsg = _T("");
	theProcBank.m_strLastEngMsg = _T("");
	theProcBank.m_strLastVnmMsg = _T("");
	BOOL bRet = TRUE;

	if(theUnitFunc.GetOutPutIOCheck(Y_FRONT_DOOR1_LOCK_ONOFF))
	{
		theProcBank.m_strLastKorMsg = _T("FRONT DOOR1 LOCK을 ON해주세요");		//한
		theProcBank.m_strLastEngMsg = _T("FRONT DOOR1 door unlocked.");			//영
		theProcBank.m_strLastVnmMsg =_T("Cửa phía trước số 1 đã bị mở.") ;		//베
		bRet = FALSE;
	}

	if(theUnitFunc.GetOutPutIOCheck(Y_FRONT_DOOR2_LOCK_ONOFF))
	{
		theProcBank.m_strLastKorMsg = _T("FRONT DOOR2 LOCK을 ON해주세요");			//한
		theProcBank.m_strLastEngMsg = _T("FRONT DOOR2 door unlocked.");			//영
		theProcBank.m_strLastVnmMsg =_T("Cửa phía trước số 2 đã bị mở.") ;		//베
		bRet = FALSE;
	}

	if(theUnitFunc.GetOutPutIOCheck(Y_RIGNT_DOOR_LOCK_ONOFF))
	{
		theProcBank.m_strLastKorMsg = _T("RIGNT DOOR LOCK을 ON해주세요");			//한
		theProcBank.m_strLastEngMsg = _T("RIGNT DOOR door unlocked.");			//영
		theProcBank.m_strLastVnmMsg =_T("Cửa bên phải đã bị mở.") ;			//베
		bRet = FALSE;
	}

	if(theUnitFunc.GetOutPutIOCheck(Y_BACK_DOOR1_LOCK_ONOFF))
	{
		theProcBank.m_strLastKorMsg = _T("BACK DOOR1 LOCK을 ON해주세요");			//한
		theProcBank.m_strLastEngMsg = _T("BACK DOOR1 door unlocked.");			//영
		theProcBank.m_strLastVnmMsg =_T("Cửa phía sau số 1 đã bị mở.") ;		//베
		bRet = FALSE;
	}

	if(theUnitFunc.GetOutPutIOCheck(Y_BACK_DOOR2_LOCK_ONOFF))
	{
		theProcBank.m_strLastKorMsg = _T("BACK DOOR2 LOCK을 ON해주세요");			//한
		theProcBank.m_strLastEngMsg = _T("BACK DOOR2 door unlocked.");			//영
		theProcBank.m_strLastVnmMsg =_T("Cửa phía sau số 2 đã bị mở.") ;		//베
		bRet = FALSE;
	}

	if(theUnitFunc.GetOutPutIOCheck(Y_LEFT_DOOR_LOCK_ONOFF))
	{
		theProcBank.m_strLastKorMsg = _T("LEFT DOOR LOCK을 ON해주세요");			//한
		theProcBank.m_strLastEngMsg = _T("LEFT DOOR door unlocked.");				//영
		theProcBank.m_strLastVnmMsg =_T("Cửa bên trái đã bị mở.") ;	//베
		bRet = FALSE;
	}

	return bRet;
}

BOOL CUnitCtrlBank::LightCurtain_Check(JIG_ID jig /*= JIG_ID_MAX*/, BOOL bAlarm /*= FALSE*/)
{
	if(bAlarm == FALSE)
	{
		theProcBank.m_strLastKorMsg = _T("");
		theProcBank.m_strLastEngMsg = _T("");
		theProcBank.m_strLastVnmMsg = _T("");
	}
	BOOL bRet=TRUE;
	if(theConfigBank.m_System.m_bInlineMode)
		return bRet;

	if(  bRet  &&  ( (jig == JIG_ID_A ) || (jig == JIG_ID_MAX) )  )
	{
		if(GetInPutIOCheck(X_SHUTTLE_1_LIGHT_CURTAIN))
		{
			if(bAlarm)
			{
				theProcBank.AlarmHappen(ALM_SHUTTLE1_LIGHT_CURTAIN);
			}
			else
			{
				theProcBank.m_strLastKorMsg =_T("SHUTTLE 1 Light curtain을 감지 했습니다. ");		//한
				theProcBank.m_strLastEngMsg =_T("SHUTTLE 1 Light curtain detected.");				//영
				theProcBank.m_strLastVnmMsg =_T("SHUTTLE 1 Light curtain phát hiện cảm biến.") ;			//베
			}
			bRet = FALSE;
		}
	}

	if(  bRet  &&  ( (jig == JIG_ID_B ) || (jig == JIG_ID_MAX) )  )
	{
		if(GetInPutIOCheck(X_SHUTTLE_2_LIGHT_CURTAIN))
		{
			if(bAlarm)
			{
				theProcBank.AlarmHappen(ALM_SHUTTLE2_LIGHT_CURTAIN);
			}
			else
			{
				theProcBank.m_strLastKorMsg =_T("SHUTTLE 2 light curtain을 감지 했습니다. ");		//한
				theProcBank.m_strLastEngMsg =_T("SHUTTLE 2 light curtain detected.");			//영
				theProcBank.m_strLastVnmMsg =_T("SHUTTLE 2 Light curtain phát hiện cảm biến.") ;		//베
			}
			bRet = FALSE;
		}
	}

	
	return bRet;
}

void CUnitCtrlBank::LightCurtainMute_OnOff(JIG_ID jig, ONOFF value)
{
	if(theConfigBank.m_System.m_bInlineMode)
		value = ON;
	switch(jig)
	{
	case JIG_ID_A:
		SetOutPutIO(Y_SHUTTLE_1_LIGHT_MUTING_ON_TO_SNC , value);
		break;
	case JIG_ID_B:
		SetOutPutIO(Y_SHUTTLE_2_LIGHT_MUTING_ON_TO_SNC , value);
		break;
	case JIG_ID_MAX:
		SetOutPutIO(Y_SHUTTLE_1_LIGHT_MUTING_ON_TO_SNC , value);
		SetOutPutIO(Y_SHUTTLE_2_LIGHT_MUTING_ON_TO_SNC , value);
		break;
	default:
		break;

	}
	switch(jig)
	{
	case JIG_ID_A:
		SetOutPutIO(Y_SHUTTLE_1_LIGHT_MUTING_ON_TO_SNC , value);
		break;
	case JIG_ID_B:
		SetOutPutIO(Y_SHUTTLE_2_LIGHT_MUTING_ON_TO_SNC , value);
		break;
	case JIG_ID_MAX:
		SetOutPutIO(Y_SHUTTLE_1_LIGHT_MUTING_ON_TO_SNC , value);
		SetOutPutIO(Y_SHUTTLE_2_LIGHT_MUTING_ON_TO_SNC , value);
		break;
	default:
		break;

	}
}

void CUnitCtrlBank::LightCurtainMuteLamp_OnOff( JIG_ID jig, ONOFF value )
{
	switch(jig)
	{
	case JIG_ID_A:
		SetOutPutIO(Y_SHUTTLE_1_LIGHT_CURTAIN_MUTING_LAMP, value);
		break;
	case JIG_ID_B:
		SetOutPutIO(Y_SHUTTLE_2_LIGHT_CURTAIN_MUTING_LAMP, value);
		break;
	case JIG_ID_MAX:
		SetOutPutIO(Y_SHUTTLE_1_LIGHT_CURTAIN_MUTING_LAMP, value);
		SetOutPutIO(Y_SHUTTLE_2_LIGHT_CURTAIN_MUTING_LAMP, value);
		break;
	default:
		break;
	}
}

BOOL CUnitCtrlBank::LightCurtainMuteLamp_Check(JIG_ID jig)
{
	BOOL bRet = OFF;
	switch(jig)
	{
		case JIG_ID_A:
			bRet = GetOutPutIOCheck(Y_SHUTTLE_1_LIGHT_CURTAIN_MUTING_LAMP);
			break;
		case JIG_ID_B:
			bRet = GetOutPutIOCheck(Y_SHUTTLE_2_LIGHT_CURTAIN_MUTING_LAMP);
			break;
	}
	return bRet;
}

STO_STATE CUnitCtrlBank::STO_Check()
{
	BOOL bSTO1 = GetInPutIOCheck(X_SHUTTLE_TABLE_STO1);
	BOOL bSTO2 = GetInPutIOCheck(X_SHUTTLE_TABLE_STO2);
	theProcBank.m_strLastKorMsg.Empty();
	theProcBank.m_strLastEngMsg.Empty();
	theProcBank.m_strLastVnmMsg.Empty();

	if(bSTO1 && bSTO2)
	{
		theProcBank.m_strLastKorMsg =_T("STO가 걸려있습니다. RESET을 눌러주세요.");				//한
		theProcBank.m_strLastEngMsg =_T("STO is enabled. Please push RESET button.");		//영
		theProcBank.m_strLastVnmMsg =_T("STO bị tắt. Vui lòng bấm nút RESET.") ;		//베
		return STO_ALARM;
	}
	else if( (bSTO1 == FALSE) && (bSTO2 == FALSE) )
	{
		return STO_READY;
	}
	else
	{
		// 원래 둘 다 살아야 하지만 라인 하나가 죽어있는 상태. 전장 조치 필요 [9/19/2017 OSC]
		theProcBank.m_strLastKorMsg =_T("STO Line 하나가 불량입니다. 조치가 필요합니다.");				//한
		theProcBank.m_strLastEngMsg =_T("STO Line 1EA is NG. Please fix STO line.");		//영
		theProcBank.m_strLastVnmMsg =_T("STO Line 1EA bị lỗi. Vui lòng khoi tao lai.") ;		//베
		return STO_WARNING;
	}
}

void CUnitCtrlBank::TowerLamp_Change( int nRed, int nYellow, int nGreen, BOOL bBuzzer )
{
	if(bBuzzer == TRUE)
	{
		SetOutPutIO(Y_BUZZER_K1,ON);
		SetOutPutIO(Y_BUZZER_K2,OFF);
		SetOutPutIO(Y_BUZZER_K3,OFF);
		SetOutPutIO(Y_BUZZER_K4,OFF);
	}
	else
	{
		SetOutPutIO(Y_BUZZER_K1,OFF);
		SetOutPutIO(Y_BUZZER_K2,OFF);
		SetOutPutIO(Y_BUZZER_K3,OFF);
		SetOutPutIO(Y_BUZZER_K4,OFF);
	}

	//RED
	if(nRed == LAMP_ON)
	{
		SetOutPutIO(Y_TOWER_LAMP_RED, ON);

		GetMainHandler()->m_bChkLampR_Flick = FALSE;
	}
	else if(nRed == LAMP_OFF)
	{
		SetOutPutIO(Y_TOWER_LAMP_RED, OFF);
		GetMainHandler()->m_bChkLampR_Flick = FALSE;
	}
 	else if(nRed == LAMP_FLICKER)
 	{
 		GetMainHandler()->m_bChkLampR_Flick = TRUE;
 	}

 	// YELLOW
 	if(nYellow == LAMP_ON)
 	{
 		SetOutPutIO(Y_TOWER_LAMP_YELLOW, ON);
 		GetMainHandler()->m_bChkLampY_Flick = FALSE;
 	}
 	else if(nYellow == LAMP_OFF)
 	{
 		SetOutPutIO(Y_TOWER_LAMP_YELLOW, OFF);
 		GetMainHandler()->m_bChkLampY_Flick = FALSE;
 	}
 	else if(nYellow == LAMP_FLICKER)
 	{
 		GetMainHandler()->m_bChkLampY_Flick = TRUE;
 	}
 
 	// GREEN
 	if(nGreen == LAMP_ON)
 	{		
 		SetOutPutIO(Y_TOWER_LAMP_GREEN, ON);
 		GetMainHandler()->m_bChkLampG_Flick = FALSE;
 	}
 	else if(nGreen == LAMP_OFF)
 	{
 		SetOutPutIO(Y_TOWER_LAMP_GREEN, OFF);		
 		GetMainHandler()->m_bChkLampG_Flick = FALSE;
 	}
 	else if(nGreen == LAMP_FLICKER)
 	{
 		GetMainHandler()->m_bChkLampG_Flick = TRUE;
 	}
}

void CUnitCtrlBank::Temp_Check() 
{
	if(GetInPutIOCheck(X_UTIL_BOX_TEMP_ALARM))
 	{
 		theProcBank.AlarmHappen(ALM_UTIL_BOX_TEMP_ALARM);
 		return;
 	}
	if(GetInPutIOCheck(X_C_BOX_A_TEMP_ALARM))
	{
		theProcBank.AlarmHappen(ALM_ETC_BOX_TEMP_ALARM);
		return;
	}
	if(GetInPutIOCheck(X_OP_PC_TEMP_ALARM))
	{
		theProcBank.AlarmHappen(ALM_OP_PC_TEMP_ALARM);
		return;
	}
	if(GetInPutIOCheck(X_SIGNAL_PC_TEMP_ALARM))
	{
		theProcBank.AlarmHappen(ALM_SIGNAL_PC_TEMP_ALARM);
		return;
	}
	if(GetInPutIOCheck(X_C_BOX_B_TEMP_ALARM))
	{
		theProcBank.AlarmHappen(ALM_SERVO_BOX_TEMP_ALARM);
		return;
	}
}

void CUnitCtrlBank::EMSSwitch_Check()
{
	INPUT_ID id = X_FRONT_EMS_SWITCH;
	BOOL  bValue = GetInPutIOCheck(id);
	if(theProcBank.m_OldInput[id] != bValue)
	{
		theProcBank.m_OldInput[id] = bValue;
		if(bValue)
		{
			theProcBank.AlarmHappen(ALM_FRONT_EMS_SWITCH);
			return;
		}
	}
	id = X_LEFT_EMS_SWITCH;
	bValue = GetInPutIOCheck(id);
	if(theProcBank.m_OldInput[id] != bValue)
	{
		theProcBank.m_OldInput[id] = bValue;
		if(bValue)
		{
			theProcBank.AlarmHappen(ALM_LEFT_EMS_SWITCH);
			return;
		}
	}
	id = X_BACK_DOOR_EMS_SWITCH;
	bValue = GetInPutIOCheck(id);
	if(theProcBank.m_OldInput[id] != bValue)
	{
		theProcBank.m_OldInput[id] = bValue;
		if(bValue)
		{
			theProcBank.AlarmHappen(ALM_BACK_DOOR_EMS_SWITCH);
			return;
		}
	}
	id = X_RIGHT_EMS_SWITCH;
	bValue = GetInPutIOCheck(id);
	if(theProcBank.m_OldInput[id] != bValue)
	{
		theProcBank.m_OldInput[id] = bValue;
		if(bValue)
		{
			theProcBank.AlarmHappen(ALM_RIGHT_EMS_SWITCH);
			return;
		}
	}

	id = X_PC_RACK_EMS_SWITCH;
	bValue = GetInPutIOCheck(id);
	if(theProcBank.m_OldInput[id] != bValue)
	{
		theProcBank.m_OldInput[id] = bValue;
		if(bValue)
		{
			theProcBank.AlarmHappen(ALM_MANUAL_MODE_EMS_SWITCH);
			return;
		}
	}
}

void CUnitCtrlBank::FanAlarm_Check()
{
// 	if(GetInPutIOCheck(X_UTIL_BOX_FAN_ALARM))
// 	{
// 		theProcBank.AlarmHappen(ALM_UTIL_BOX_FAN_ALARM);
// 		return;
// 	}
// 	if(GetInPutIOCheck(X_ETC_BOX_FAN_ALARM))
// 	{
// 		theProcBank.AlarmHappen(ALM_ETC_BOX_FAN_ALARM);
// 		return;
// 	}
// 	if(GetInPutIOCheck(X_OP_PC_FAN_ALARM))
// 	{
// 		theProcBank.AlarmHappen(ALM_OP_PC_FAN_ALARM);
// 		return;
// 	}
// 	if(GetInPutIOCheck(X_SIGNAL_PC_FAN_ALARM))
// 	{
// 		theProcBank.AlarmHappen(ALM_SIGNAL_PC_FAN_ALARM);
// 		return;
// 	}
// 	if(GetInPutIOCheck(X_SERVO_BOX_FAN_ALARM))
// 	{
// 		theProcBank.AlarmHappen(ALM_SERVO_BOX_FAN_ALARM);
// 		return;
// 	}
}

void CUnitCtrlBank::MCPower_Check()
{
	if(GetInPutIOCheck(X_MC_ON_CHECK) == FALSE)
	{
		theProcBank.AlarmHappen(ALM_MC_POWER_OFF);
	}	
}

//kjpark 20161025 MCR 구현
//kjpark 20170902 Teach Shuttle UI 및 버튼 동작 추가
void CUnitCtrlBank::MCR_Trigger(JIG_ID jig, JIG_CH ch, BOOL bOn )
{
	// Trigger On 할 때 BOOL 변수를 자동으로 FALSE해 준다 [9/18/2017 OSC]
	if(bOn)		theProcBank.m_bMCRResultRecive[jig][ch] = FALSE;

	switch(jig)
	{
	case JIG_ID_A:
		{
			switch(ch)
			{
			case JIG_CH_1:
				SetOutPutIO((OUTPUT_ID) Y_MCR1_TRIGGER_ONOFF, bOn);
				break;
			}
		}
		break;
	case JIG_ID_B:
		{
			switch(ch)
			{
			case JIG_CH_1:
				SetOutPutIO((OUTPUT_ID) Y_MCR3_TRIGGER_ONOFF, bOn);
				break;
			}
		}
		break;
	}
}

//kjpark 20170912 MCR에서 읽은 셀아이디 정상 판별
BOOL CUnitCtrlBank::GetMCR_ResultRecive(JIG_ID jig, JIG_CH ch)
{	
	return theProcBank.m_bMCRResultRecive[jig][ch];
}

//kjpark 20161025 MCR 구현
CString CUnitCtrlBank::GetSoftTriggerData( JIG_ID jig, JIG_CH ch )
{
	if ( theSocketInterFace.m_MCR[jig][ch].m_pIDReader )
	{
		if ( theSocketInterFace.m_MCR[jig][ch].m_pIDReader->IsConnect() )
		{
			return theSocketInterFace.m_MCR[jig][ch].m_pIDReader->GetReadString();
		}
	}

	return _T("");
}

//kjpark 20161025 MCR 구현
int CUnitCtrlBank::GetSoftTriggerResult( JIG_ID jig, JIG_CH ch )
{
	if ( theSocketInterFace.m_MCR[jig][ch].m_pIDReader )
	{
		if ( theSocketInterFace.m_MCR[jig][ch].m_pIDReader->IsConnect() )
		{
			return theSocketInterFace.m_MCR[jig][ch].m_pIDReader->GetReadResult();
		}
	}

	return 0;
}


// Live 화면을 지정해준다.
//kjpark 20161025 MCR 구현
BOOL CUnitCtrlBank::SetLiveMode(JIG_ID jig, JIG_CH ch, BOOL bFlag)
{
	if ( theSocketInterFace.m_MCR[jig][ch].m_pIDReader )
	{
		if ( theSocketInterFace.m_MCR[jig][ch].m_pIDReader->IsConnect() )
		{
			if ( bFlag )			// Live On 요청
			{
				if ( theSocketInterFace.m_MCR[jig][ch].m_pIDReader->IsLiveOn() == FALSE )
				{
					theSocketInterFace.m_MCR[jig][ch].m_pIDReader->LiveOn();
				}

				return TRUE;
			}

			theSocketInterFace.m_MCR[jig][ch].m_pIDReader->LiveOff();

			return TRUE;
		}
	}

	return FALSE;
}

// Live mode인가 ?
//kjpark 20161025 MCR 구현
BOOL CUnitCtrlBank::GetLiveMode( JIG_ID jig, JIG_CH ch )
{
	if ( theSocketInterFace.m_MCR[jig][ch].m_pIDReader )
	{
		if ( theSocketInterFace.m_MCR[jig][ch].m_pIDReader->IsConnect() )
		{
			return theSocketInterFace.m_MCR[jig][ch].m_pIDReader->IsLiveOn();
		}

	}

	return FALSE;	
}

// 통신에 의한 Trigger 신호를 보내준다.
//kjpark 20161025 MCR 구현
BOOL CUnitCtrlBank::SetSoftTrigger( JIG_ID jig, JIG_CH ch )
{
	if ( theSocketInterFace.m_MCR[jig][ch].m_pIDReader )
	{
		if ( theSocketInterFace.m_MCR[jig][ch].m_pIDReader->IsConnect() )
		{
			if ( theSocketInterFace.m_MCR[jig][ch].m_pIDReader->IsLiveOn() )
			{
				theSocketInterFace.m_MCR[jig][ch].m_pIDReader->LiveOff();
				SleepEx(200, NULL);
			}

			theSocketInterFace.m_MCR[jig][ch].m_pIDReader->Trigger();

			return TRUE;
		}
	}

	return FALSE;
}


// 결과 화면을 저장한다.
//kjpark 20161025 MCR 구현
BOOL CUnitCtrlBank::SaveResultImage(JIG_ID jig, JIG_CH ch, CString sFile)
{
	if ( theSocketInterFace.m_MCR[jig][ch].m_pIDReader )
	{
		if ( theSocketInterFace.m_MCR[jig][ch].m_pIDReader->IsConnect() )
		{
			//kjpark 20170831 MCR 구현 채널 별 구현 완료
			GetMainHandler()->m_sLastSavedImage[jig][ch] = sFile;// = _T("O:\\123.jpg");					// 마지막에 저장된 이미지 파일이름
			theSocketInterFace.m_MCR[jig][ch].m_pIDReader->SaveImage(sFile);

			return TRUE;
		}
	}

	return FALSE;
}

//kjpark 20170912 MCR에서 읽은 셀아이디 정상 판별
void CUnitCtrlBank::SetMCRReadingID(JIG_ID shuttls, JIG_CH channel)
{
	//Cell Info에 MCR ID 삽입. 
	CCellInfo* pCellInfo;

	pCellInfo = theCellBank.GetCellInfo(shuttls, channel);

	if(pCellInfo)
	{
		if(theProcBank.m_strLastCellID[shuttls][channel].GetLength() < 1 || theProcBank.m_strLastCellID[shuttls][channel] == TEXT_FAIL)
		{
// 			if(theConfigBank.m_System.m_bInlineMode)
// 			{
// 				// 로봇에서 Cell ID를 받아오니 일단 TRUE로.. [10/7/2017 OSC]
// 				pCellInfo->defaultData.m_bMCR_OK = TRUE;
// 				pCellInfo->defaultData.m_strReadUnitMCR = _T("ROBOT");
// 			}
// 			else
			{
				pCellInfo->defaultData.m_strCellID = theProcBank.m_strLastCellID[shuttls][channel];	// 읽은것 어디서 가져다 넣는지 ?
// 				//////////////////////////////////////////////////////////////////////////
// 				// aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa [9/26/2017 yw]
// 				pCellInfo->defaultCellInfomation.m_bMCR_OK = TRUE;
// 				pCellInfo->defaultCellInfomation.m_strCellID = pCellInfo->defaultCellInfomation.m_strInnerID;
			}
		}	
		else
		{
			//20170306 kjpark MCR Read Unit
			pCellInfo->defaultData.m_bMCR_OK = TRUE;
			pCellInfo->defaultData.m_strCellID = theProcBank.m_strLastCellID[shuttls][channel];
		}
		if(pCellInfo->defaultData.m_strReadUnitMCR.IsEmpty())
		{
			int nFullCh = ((shuttls*JIG_CH_MAX) + channel)+1;
			pCellInfo->defaultData.m_strReadUnitMCR.Format(_T("MCR_CH%d"), nFullCh);
		}

		theLog[LOG_MCR].AddBuf(_T("%s InnerID[%s], CellID[%s]"), 
			pCellInfo->defaultData.m_strReadUnitMCR, pCellInfo->defaultData.m_strInnerID, pCellInfo->defaultData.m_strCellID);

	}
}

//kjpark 20170912 MCR Retry 추가
CString	CUnitCtrlBank::MCR_GetLastID(JIG_ID shuttls, JIG_CH channel)
{
	return theProcBank.m_strLastCellID[shuttls][channel];
}

BOOL CUnitCtrlBank::MCR_SuccessCheck( JIG_ID jig, JIG_CH channel )
{
	CCellInfo* pCellInfo;

	pCellInfo = theCellBank.GetCellInfo(jig, channel);
	if(pCellInfo == NULL)
		return TRUE;

	if(pCellInfo->defaultData.m_bMCR_OK == FALSE)
		return FALSE;
	if(pCellInfo->defaultData.m_strCellID.GetLength() < 1 || pCellInfo->defaultData.m_strCellID == TEXT_FAIL)
		return FALSE;

	return TRUE;
}
// 마지막에 저장된 이미지 파일이름
//kjpark 20161025 MCR 구현
//kjpark 20170831 MCR 구현 채널 별 구현 완료
CString CUnitCtrlBank::GetLastSavedImage(JIG_ID jig, JIG_CH ch)
{
	return	GetMainHandler()->m_sLastSavedImage[jig][ch];					
}

//kjpark 20170912 MCR Retry 추가
void CUnitCtrlBank::SaveLastMCRImage(JIG_ID jig, JIG_CH ch)
{
	CCellInfo *pCell = theCellBank.GetCellInfo(jig, ch);
	if(pCell == NULL)
		return;

	CString strPath;
	CTime time = CTime::GetCurrentTime();
	CEtc::ApplyTimeOffset(time, theConfigBank.m_Option.m_OffsetHour, 0);
	strPath.Format(_T("%s\\%04u-%02u-%02u"), theConfigBank.m_System.m_strMCRImageSavePath,
		time.GetYear(), time.GetMonth(), time.GetDay());

	if(CFileSupport::DirectoryCheck(strPath) == FALSE)
	{
		if(CFileSupport::CreateDirectory(strPath) == FALSE)
			return;
	}
	int nFullCh = ((jig*JIG_CH_MAX) + ch)+1;
	CString strFilePath;
	strFilePath.Format(_T("%s\\CH%d_%02u_%02u_%02u_%s.jpg"), 
		strPath, nFullCh, time.GetHour(), time.GetMinute(), time.GetSecond(), pCell->defaultData.m_strInnerID);

	// 마지막 저장했던걸 복사한다
	switch(jig)
	{
	case JIG_ID_A:
		{
			switch(ch)
			{
			case JIG_CH_1:
				CopyFile(MCRPath_CH1, strFilePath, FALSE);	
				break;
			}
		}
		break;
	case JIG_ID_B:
		{
			switch(ch)
			{
			case JIG_CH_1:
				CopyFile(MCRPath_CH3, strFilePath, FALSE);	
				break;
			}
		}
		break;
	}
}
//  [11/8/2016 WHLEE]

void CUnitCtrlBank::SetProductData(JIG_ID jig, JIG_CH ch)
{
	CCellInfo* pCell = theCellBank.GetCellInfo(jig, ch);

	if (pCell == NULL)
	{
		//	셀이없음
		return;
	}
	// 현재 날짜 업데이트
	theProductBank.DateCheck();

	/* Product */
	theProductBank.IncreaseProductCount(jig, ch);
	
	/* Inspectoin result */
	if(pCell->defaultData.m_LastClass == GOOD_CELL)
		theProductBank.IncreaseBinZoreCount(jig, ch);
	else if(pCell->defaultData.m_LastClass == REJECT_CELL)
		theProductBank.IncreaseBinOneCount(jig, ch);
	else
		theProductBank.IncreaseBinTwoCount(jig, ch);

	/* MCR */
	if ( !(pCell->defaultData.m_strCellID == TEXT_FAIL) )		
		theProductBank.IncreaseMcrOkCount(jig, ch);

	/* First Contact */
	if(pCell->m_CellLoading.m_FirstClass == GOOD_CELL)
		theProductBank.IncreaseFirstContactCount(jig, ch);

	/* Final Contact */
	if(pCell->m_CellLoading.m_Class == GOOD_CELL)
		theProductBank.IncreaseFinalContactCount(jig, ch);

	// 생산 데이터 입력 - LSH171216
	theProductBank.SaveReportFileToday();
	// Main UI 상태 Update - LSH171216
	theProductBank.MainCount_Update(pCell);
}

void CUnitCtrlBank::ReadFromFilePGMS()
{
	CStdioFile file;
	CFileException e;
	
	CString strPGMSPath = _T("C:/PGMS_LOG/PgmsLog.ini");
	if (!file.Open(strPGMSPath.GetBuffer(), CFile::modeRead, &e))			//	후에 파일이 없는경우 생성하도록 변경해야함
	{
		return;
	}

	CIni ini(strPGMSPath);;
	file.Close();

	int i, nIdx;
	CString sSection, sKey;
	CString sData;
	CString str;
	//
	//[DEVICE 1]
	//ID Num=1
	//Nick Name=
	//Model=GMS2
	//Channel 1=COM ERR
	//Channel 2=COM ERR

	//[DEVICE 2]
	//ID Num=2
	//Nick Name=
	//Model=PGMS
	//Wrist State=COM ERR


	// 저항값 측정
	sSection = _T("DEVICE 1");

	for (i = 0; i < GMS_SUB_CH5; i++)
	{
		nIdx = i;
		sKey.Format(_T("Channel %d"), i+1);
		sData = ini.GetString(sSection, sKey);		
		
  		if ( !sData.IsEmpty())		// 데이터가 존재한다면
		{
			sData.MakeUpper();

			if ( sData.CompareNoCase(_T("COM ERR")) == 0 )			// 통신 오류
			{
				//m_sErrMsg[nIdx]		= _T("ERROR");				// 연결오류
				//m_dbRcvValue[ nIdx ]	= 9996;			
				str.Format(_T("%d"), 9996);// 데이터 취득 불가
			}
			else if ( sData.CompareNoCase(_T("OPEN")) == 0 )		// 센서 미연결
			{
				//m_sErrMsg[nIdx]		= _T("OPEN");
				//m_dbRcvValue[ nIdx ]	= 9999;	
				str.Format(_T("%d"), 9999);
			}
			else if ( sData.CompareNoCase(_T("DISABLE")) == 0 )		// 미사용 설정
			{
				//m_sErrMsg[nIdx]		= _T("UNUSED");
				//m_dbRcvValue[ nIdx ]	= 9997;	
				str.Format(_T("%d"), 9997);

			}
			else if ( sData.CompareNoCase(_T("RANGE OVER")) == 0 )		// 저항값 초과
			{
				//m_sErrMsg[nIdx]		= _T("OVER");
				//m_dbRcvValue[ nIdx ]	= 9998;	
				str.Format(_T("%d"), 9998);
			}
			else
			{			
				str.Format(_T("%s"), sData);
				//m_sErrMsg[nIdx]		= _T("");					// 오류가 없다.
				//m_dbRcvValue[ nIdx ]	= _tstof( sData );	// 통신에 의해 수신된 값을 변환, 00.00 자릿수를 정해준다.
			}
		}
		else
		{
			//m_sErrMsg[nIdx]		= _T("ERROR");				// 연결오류
			//m_dbRcvValue[ nIdx ]	= 9996;						// 데이터 취득 불가
			str.Format(_T("%d"), 9996);
		}

		//kjpark 20170929 FDC 실제값, 보정값, 보고값 구분
		theSerialInterFace.GetDeviceGMSHandler1()->m_LastGMSValue[(GMS_SUB_CH)i].Format(_T("%s,%s,%s"), str,str,str);
	}//of for i

	sSection = _T("DEVICE 2");
	for (i = 0; i < GMS_SUB_CH5; i++)
	{
		nIdx = i;
		sKey.Format(_T("Channel %d"), i+1);
		sData = ini.GetString(sSection, sKey);		

		if ( !sData.IsEmpty())		// 데이터가 존재한다면
		{
			sData.MakeUpper();

			if ( sData.CompareNoCase(_T("COM ERR")) == 0 )			// 통신 오류
			{
				//m_sErrMsg[nIdx]		= _T("ERROR");				// 연결오류
				//m_dbRcvValue[ nIdx ]	= 9996;			
				str.Format(_T("%d"), 9996);// 데이터 취득 불가
			}
			else if ( sData.CompareNoCase(_T("OPEN")) == 0 )		// 센서 미연결
			{
				//m_sErrMsg[nIdx]		= _T("OPEN");
				//m_dbRcvValue[ nIdx ]	= 9999;	
				str.Format(_T("%d"), 9999);
			}
			else if ( sData.CompareNoCase(_T("DISABLE")) == 0 )		// 미사용 설정
			{
				//m_sErrMsg[nIdx]		= _T("UNUSED");
				//m_dbRcvValue[ nIdx ]	= 9997;	
				str.Format(_T("%d"), 9997);

			}
			else if ( sData.CompareNoCase(_T("RANGE OVER")) == 0 )		// 저항값 초과
			{
				//m_sErrMsg[nIdx]		= _T("OVER");
				//m_dbRcvValue[ nIdx ]	= 9998;	
				str.Format(_T("%d"), 9998);
			}
			else
			{			
				str.Format(_T("%s"), sData);
				//m_sErrMsg[nIdx]		= _T("");					// 오류가 없다.
				//m_dbRcvValue[ nIdx ]	= _tstof( sData );	// 통신에 의해 수신된 값을 변환, 00.00 자릿수를 정해준다.
			}
		}
		else
		{
			//m_sErrMsg[nIdx]		= _T("ERROR");				// 연결오류
			//m_dbRcvValue[ nIdx ]	= 9996;						// 데이터 취득 불가
			str.Format(_T("%d"), 9996);
		}

		//kjpark 20170929 FDC 실제값, 보정값, 보고값 구분
		theSerialInterFace.GetDeviceGMSHandler2()->m_LastGMSValue[(GMS_SUB_CH)i].Format(_T("%s,%s,%s"), str,str,str);
	}//of for i
	theFDCBank.CheckGMSValue();

	// PGMS 값 취득
	sSection = _T("DEVICE 3");
	nIdx = 2;
	sKey = _T("Wrist State");
	sData = ini.GetString(sSection, sKey);

	if ( !sData.IsEmpty())		// 데이터가 존재한다면
	{
		sData.MakeUpper();

		if ( sData.CompareNoCase(_T("ON OK")) == 0 )			
		{
			//m_sErrMsg[nIdx]			= _T("ON,OK");
			//m_dbRcvValue[ nIdx ]	= 1;	
			str.Format(_T("%d"), 1);// 정상적인 데이터
		}
		else if ( sData.CompareNoCase(_T("ON NG")) == 0 )			
		{
			//m_sErrMsg[nIdx]			= _T("ON,NG");				// 스위치 On, 스트랩 단선
			//m_dbRcvValue[ nIdx ]	= 0;	
			str.Format(_T("%d"), 0);
		}
		else if ( sData.CompareNoCase(_T("OFF OK")) == 0 )			
		{
			//m_sErrMsg[nIdx]			= _T("OFF,OK");
			//m_dbRcvValue[ nIdx ]	= 3;	
			// 스위치  Off, 스트랩 정상
			str.Format(_T("%d"), 3);
		}
		else if ( sData.CompareNoCase(_T("OFF NG")) == 0 )			
		{
			//m_sErrMsg[nIdx]			= _T("OFF,NG");				// 스위치 Off, 스트랩 단선
			//m_dbRcvValue[ nIdx ]	= 2;	
			str.Format(_T("%d"), 2);
		}
		else if ( sData.CompareNoCase(_T("COM ERR")) == 0 )	// 통신 단선
		{
			//m_sErrMsg[nIdx]			= _T("ERROR");				
			//m_dbRcvValue[ nIdx ]	= 4;	
			str.Format(_T("%d"), 4);
		}
		else	
		{
		//	m_sErrMsg[nIdx]			= _T("ERROR");				// 비정상적인 데이터
			//m_dbRcvValue[ nIdx ]	= _tstof( sData );			// 통신에 의해 수신된 값을 변환
			str.Format(_T("%s"), sData);
		}
		//kjpark 20170929 FDC 실제값, 보정값, 보고값 구분
		theSerialInterFace.GetDeviceGMSHandler1()->m_LastPGMS.Format(_T("%s,%s,%s"), str,str,str);
		theFDCBank.CheckPGMSValue();
	}	
}

BOOL CUnitCtrlBank::LoadingStop_IsAble()
{
	if(theProcBank.m_bMachineCellExist)
		return FALSE;

	if(theConfigBank.m_System.m_bInlineMode)
	{
		// 인라인일 때 Loading Stop 발생 조건 확인 [11/24/2017 OSC]
		if(CellTagExist(CELL_POS_SHUTTLE1_CH1, CELL_POS_SHUTTLE2_CH1) == FALSE)
		{
			return TRUE;
		}
	}
	else
	{
		if(CellTagExist(CELL_POS_SHUTTLE1_CH1, CELL_POS_SHUTTLE2_CH1) == FALSE)
		{
			return TRUE;
		}
		// 단동일 때 Loading Stop 발생 조건 확인 [11/24/2017 OSC]
		else if( (CellLoading_RecvCheck(JIG_ID_A) == FALSE)
			&& (CellLoading_RecvCheck(JIG_ID_B) == FALSE) )
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CUnitCtrlBank::LoadingStop_IsAble(JIG_ID jig)
{
	CELL_POS posStart, posEnd;
	if(jig == JIG_ID_A)
	{
		posStart = CELL_POS_SHUTTLE1_CH1;
		posEnd = CELL_POS_SHUTTLE1_CH1;
	}
	else if(jig == JIG_ID_B)
	{
		posStart = CELL_POS_SHUTTLE2_CH1;
		posEnd = CELL_POS_SHUTTLE2_CH1;
	}
	else if(jig == JIG_ID_MAX)
	{
		posStart = CELL_POS_SHUTTLE1_CH1;
		posEnd = CELL_POS_SHUTTLE1_CH1;
	}


	if(theConfigBank.m_System.m_bInlineMode)
	{
		// 인라인일 때 Loading Stop 발생 조건 확인 [11/24/2017 OSC]
		if(CellTagExist(posStart, posEnd) == FALSE)
		{
			return TRUE;
		}
	}
	else
	{
		if(CellTagExist(posStart, posEnd) == FALSE)
		{
			return TRUE;
		}
		else
		{
			if(jig == JIG_ID_MAX)
			{
				// 단동일 때 Loading Stop 발생 조건 확인 [11/24/2017 OSC]
				if( (CellLoading_RecvCheck(JIG_ID_A) == FALSE)
					&& (CellLoading_RecvCheck(JIG_ID_B) == FALSE) )
				{
					return TRUE;
				}
			}
			else
			{
				// 단동일 때 Loading Stop 발생 조건 확인 [11/24/2017 OSC]
				if( CellLoading_RecvCheck(jig) == FALSE )
				{
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

BOOL CUnitCtrlBank::TransferStop_IsAble()
{
	if( GetZoneEnd(JIG_ID_A, ZONE_ID_C)
		&& GetZoneEnd(JIG_ID_B, ZONE_ID_C)
		&& Shuttle_Y_LOAD_Check(JIG_ID_A)
		&& Shuttle_Y_LOAD_Check(JIG_ID_B)
		)
	{
		return TRUE;
	}
	return FALSE;
}

//kjpark 20170831 MCR 구현 채널 별 구현 완료
void CUnitCtrlBank::SetMCR_Reset(JIG_ID jig, JIG_CH ch, BOOL bOn)
{
	switch(jig)
	{
	case  JIG_ID_A:
		{
			switch(ch)
			{
			case JIG_CH_1:
				SetOutPutIO((OUTPUT_ID) Y_MCR1_RESET, bOn);	
				break;
			}
		}
		break;
	case  JIG_ID_B:
		{
			switch(ch)
			{
			case JIG_CH_1:
				SetOutPutIO((OUTPUT_ID) Y_MCR3_RESET, bOn);	
				break;
			}
		}
		break;
	}
	SetOutPutIO((OUTPUT_ID) Y_MCR1_RESET, bOn);	
}

//kjpark 20170902 Teach Shuttle UI 및 버튼 동작 추가
//kjpark 20180131 Robot Status interlock 회피 더 추가
BOOL CUnitCtrlBank::Shuttle_Y_LOAD_Move( JIG_ID jig )
{
	BOOL bRet = FALSE;
	switch(jig)
	{
	case  JIG_ID_A:
		bRet = theDeviceMotion.TeachMove(m_nThreadID, TEACH_PARAM::JIG_SHUTTLE_Y1_to_LOAD);
		break;
	case  JIG_ID_B:
		bRet = theDeviceMotion.TeachMove(m_nThreadID, TEACH_PARAM::JIG_SHUTTLE_Y2_to_LOAD);
		break;
	}
	return bRet;
}

//kjpark 20170902 Teach Shuttle UI 및 버튼 동작 추가
BOOL CUnitCtrlBank::Shuttle_Y_LOAD_Check( JIG_ID jig )
{
	BOOL bRet = FALSE;
	switch(jig)
	{
	case  JIG_ID_A:
		bRet = theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::JIG_SHUTTLE_Y1_to_LOAD);
		break;
	case  JIG_ID_B:
		bRet = theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::JIG_SHUTTLE_Y2_to_LOAD);
		break;
	}
	return bRet;
}

//kjpark 20170902 Teach Shuttle UI 및 버튼 동작 추가
void CUnitCtrlBank::Shuttle_Y_MANUAL_Move( JIG_ID jig )
{
	switch(jig)
	{
	case  JIG_ID_A:
		theDeviceMotion.TeachMove(m_nThreadID, TEACH_PARAM::JIG_SHUTTLE_Y1_to_MANUAL);
		break;
	case  JIG_ID_B:
		theDeviceMotion.TeachMove(m_nThreadID, TEACH_PARAM::JIG_SHUTTLE_Y2_to_MANUAL);
		break;
	}
}

//kjpark 20170902 Teach Shuttle UI 및 버튼 동작 추가
BOOL CUnitCtrlBank::Shuttle_Y_MANUAL_Check( JIG_ID jig )
{
	BOOL bRet = FALSE;
	switch(jig)
	{
	case  JIG_ID_A:
		bRet = theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::JIG_SHUTTLE_Y1_to_MANUAL);
		break;
	case  JIG_ID_B:
		bRet = theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::JIG_SHUTTLE_Y2_to_MANUAL);
		break;
	}
	return bRet;
}

//kjpark 20170902 Teach Shuttle UI 및 버튼 동작 추가
void CUnitCtrlBank::Shuttle_Y_INSP_Move( JIG_ID jig )
{
	switch(jig)
	{
	case  JIG_ID_A:
		theDeviceMotion.TeachMove(m_nThreadID, TEACH_PARAM::JIG_SHUTTLE_Y1_to_INSP);
		break;
	case  JIG_ID_B:
		theDeviceMotion.TeachMove(m_nThreadID, TEACH_PARAM::JIG_SHUTTLE_Y2_to_INSP);
		break;
	}
}

//kjpark 20170902 Teach Shuttle UI 및 버튼 동작 추가
BOOL CUnitCtrlBank::Shuttle_Y_INSP_Check( JIG_ID jig )
{
	BOOL bRet = FALSE;
	switch(jig)
	{
	case  JIG_ID_A:
		bRet = theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::JIG_SHUTTLE_Y1_to_INSP);
		break;
	case  JIG_ID_B:
		bRet = theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::JIG_SHUTTLE_Y2_to_INSP);
		break;
	}
	return bRet;
}

//kjpark 20170902 Teach Shuttle UI 및 버튼 동작 추가
void CUnitCtrlBank::Inspection_X_INSP_Move( JIG_ID jig )
{
	switch(jig)
	{
	case  JIG_ID_A:
		theDeviceMotion.TeachMove(m_nThreadID, TEACH_PARAM::INSPECTION_X1_to_INSP);
		break;
	case  JIG_ID_B:
		theDeviceMotion.TeachMove(m_nThreadID, TEACH_PARAM::INSPECTION_X2_to_INSP);
		break;
	}
}

//kjpark 20170902 Teach Shuttle UI 및 버튼 동작 추가
BOOL CUnitCtrlBank::Inspection_X_INSP_Check( JIG_ID jig )
{
	BOOL bRet = FALSE;
	switch(jig)
	{
	case  JIG_ID_A:
		bRet = theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::INSPECTION_X1_to_INSP);
		break;
	case  JIG_ID_B:
		bRet = theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::INSPECTION_X2_to_INSP);
		break;
	}
	return bRet;
}

//kjpark 20170902 Teach Shuttle UI 및 버튼 동작 추가
void CUnitCtrlBank::Inspection_Z_UP_Move( JIG_ID jig )
{
	switch(jig)
	{
	case  JIG_ID_A:
		theDeviceMotion.TeachMove(m_nThreadID, TEACH_PARAM::INSPECTION_Z1_to_UP);
		break;
	case  JIG_ID_B:
		theDeviceMotion.TeachMove(m_nThreadID, TEACH_PARAM::INSPECTION_Z2_to_UP);
		break;
	}
}

//kjpark 20170902 Teach Shuttle UI 및 버튼 동작 추가
BOOL CUnitCtrlBank::Inspection_Z_UP_Check( JIG_ID jig )
{
	BOOL bRet = FALSE;
	switch(jig)
	{
	case  JIG_ID_A:
		bRet = theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::INSPECTION_Z1_to_UP);
		break;
	case  JIG_ID_B:
		bRet = theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::INSPECTION_Z2_to_UP);
		break;
	}
	return bRet;
}

//kjpark 20170902 Teach Shuttle UI 및 버튼 동작 추가
void CUnitCtrlBank::Inspection_Z_INSP_Move( JIG_ID jig )
{
	switch(jig)
	{
	case  JIG_ID_A:
		theDeviceMotion.TeachMove(m_nThreadID, TEACH_PARAM::INSPECTION_Z1_to_INSP);
		break;
	case  JIG_ID_B:
		theDeviceMotion.TeachMove(m_nThreadID, TEACH_PARAM::INSPECTION_Z2_to_INSP);
		break;
	}
}

//kjpark 20170902 Teach Shuttle UI 및 버튼 동작 추가
BOOL CUnitCtrlBank::Inspection_Z_INSP_Check( JIG_ID jig )
{
	BOOL bRet = FALSE;
	switch(jig)
	{
	case  JIG_ID_A:
		bRet = theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::INSPECTION_Z1_to_INSP);
		break;
	case  JIG_ID_B:
		bRet = theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::INSPECTION_Z2_to_INSP);
		break;
	}
	return bRet;
}

//YJKIM 20180529 Teach Shuttle UI 및 버튼 동작 추가
void CUnitCtrlBank::Active_ALIGN_X_LEFT_Move(JIG_ID jig)
{
	switch(jig)
	{
	case  JIG_ID_A:
		theDeviceMotion.TeachMove(m_nThreadID, TEACH_PARAM::ACTIVE_ALIGN_X1_to_LEFT_X);
		break;
	case  JIG_ID_B:
		theDeviceMotion.TeachMove(m_nThreadID, TEACH_PARAM::ACTIVE_ALIGN_X2_to_LEFT_X);
		break;
	}
}

BOOL CUnitCtrlBank::Active_ALIGN_X_LEFT_Check(JIG_ID jig)
{
	BOOL bRet = FALSE;
	switch(jig)
	{
	case  JIG_ID_A:
		bRet = theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::ACTIVE_ALIGN_X1_to_LEFT_X);
		break;
	case  JIG_ID_B:
		bRet = theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::ACTIVE_ALIGN_X2_to_LEFT_X);
		break;
	}
	return bRet;
}

void CUnitCtrlBank::Active_ALIGN_X_RIGHT_Move(JIG_ID jig)
{
	switch(jig)
	{
	case  JIG_ID_A:
		theDeviceMotion.TeachMove(m_nThreadID, TEACH_PARAM::ACTIVE_ALIGN_X1_to_RIGHT_X);
		break;
	case  JIG_ID_B:
		theDeviceMotion.TeachMove(m_nThreadID, TEACH_PARAM::ACTIVE_ALIGN_X2_to_RIGHT_X);
		break;
	}
}

BOOL CUnitCtrlBank::Active_ALIGN_X_RIGHT_Check(JIG_ID jig)
{
	BOOL bRet = FALSE;
	switch(jig)
	{
	case  JIG_ID_A:
		bRet = theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::ACTIVE_ALIGN_X1_to_RIGHT_X);
		break;
	case  JIG_ID_B:
		bRet = theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::ACTIVE_ALIGN_X2_to_RIGHT_X);
		break;
	}
	return bRet;
}

void CUnitCtrlBank::Shuttle_Vac_OnOff( JIG_ID jig, JIG_CH ch, VAC_STATE vac )
{
	ONOFF value = (vac== VAC_ON) ? ON:OFF;

	if(jig == JIG_ID_A && ch == JIG_CH_1)
	{
		SetOutPutIO(SHUTTLE1_CH1_VACCUM_ONOFF,value);
	}
	else if(jig == JIG_ID_B && ch == JIG_CH_1)
	{
		SetOutPutIO(SHUTTLE2_CH3_VACCUM_ONOFF, value);
	}
}

BOOL CUnitCtrlBank::Shuttle_Vac_Check( JIG_ID jig, JIG_CH ch, VAC_STATE vac )
{
	if(theProcBank.m_bDryRunMode)
		return TRUE;

	ONOFF value = (vac== VAC_ON) ? ON:OFF;
	BOOL bRet = FALSE;
	if(jig == JIG_ID_A && ch == JIG_CH_1)
	{
		bRet = GetInPutIOCheck(X_SHUTTLE1_CH1_VACCUM_ON_SENSOR, value);
	}
	else if(jig == JIG_ID_B && ch == JIG_CH_1)
	{
		bRet = GetInPutIOCheck(X_SHUTTLE2_CH3_VACCUM_ON_SENSOR, value);
	}
	return bRet;
}

BOOL CUnitCtrlBank::Shuttle_VacOut_Check( JIG_ID jig, JIG_CH ch, VAC_STATE vac )
{
	ONOFF value = (vac== VAC_ON) ? ON:OFF;
	BOOL bRet = FALSE;
	if(jig == JIG_ID_A && ch == JIG_CH_1)
	{
		bRet= GetOutPutIOCheck(SHUTTLE1_CH1_VACCUM_ONOFF, value);
	}
	else if(jig == JIG_ID_B && ch == JIG_CH_1)
	{
		bRet = GetOutPutIOCheck(SHUTTLE2_CH3_VACCUM_ONOFF, value);
	}
	return bRet;
}

void CUnitCtrlBank::MTP_IF_Servival_AllSet()
{
	for(int jig = 0; jig < JIG_ID_MAX; jig++)
	{
		for(int ch = 0; ch < JIG_CH_MAX; ch++)
		{
			MTP_IF_Servival_OnOff((JIG_ID)jig, (JIG_CH)ch, TRUE);
// 			if(theProcBank.AZoneChannelNotUse_Check((JIG_ID)jig, (JIG_CH)ch))
// 				MTP_IF_Servival_OnOff((JIG_ID)jig, (JIG_CH)ch, FALSE);
// 			else
// 				MTP_IF_Servival_OnOff((JIG_ID)jig, (JIG_CH)ch, TRUE);
		}
	}
}

void CUnitCtrlBank::MTP_IF_Servival_AllReset()
{
	for(int jig = 0; jig < JIG_ID_MAX; jig++)
	{
		for(int ch = 0; ch < JIG_CH_MAX; ch++)
		{
			MTP_IF_Servival_OnOff((JIG_ID)jig, (JIG_CH)ch, FALSE);
		}
	}
}

void CUnitCtrlBank::MTP_IF_Servival_OnOff( JIG_ID jig, JIG_CH ch, BOOL bOn )
{
	theDevicePDT_IF.MTP_LB_Set(jig, CDevicePDT_IF::MTP_CH1_SURVIVAL, bOn);
// 	theDevicePDT_IF.MTP_LB_Set(jig, ch == JIG_CH_1 ? CDevicePDT_IF::MTP_CH1_SURVIVAL:CDevicePDT_IF::MTP_CH2_SURVIVAL, bOn);
}

void CUnitCtrlBank::MTP_IF_Able_OnOff(JIG_ID jig, BOOL bOn)
{
	theDevicePDT_IF.MTP_LB_Set(jig, CDevicePDT_IF::MTP_ABLE, bOn);
}

BOOL CUnitCtrlBank::MTP_IF_Able_Check(JIG_ID jig)
{
	return theDevicePDT_IF.MTP_LB_Get(jig, CDevicePDT_IF::MTP_ABLE);
}

void CUnitCtrlBank::MTP_IF_Start_OnOff(JIG_ID jig, BOOL bOn)
{
	theDevicePDT_IF.MTP_LB_Set(jig, CDevicePDT_IF::MTP_START, bOn);
}

void CUnitCtrlBank::MTP_IF_Complete_OnOff(JIG_ID jig, BOOL bOn)
{
	theDevicePDT_IF.MTP_LB_Set(jig, CDevicePDT_IF::MTP_COMPLETE, bOn);
}

void CUnitCtrlBank::MTP_IF_CheckSensor_OnOff(JIG_ID jig, JIG_CH ch, BOOL bOn)
{
	theDevicePDT_IF.MTP_LB_Set(jig, ch == JIG_CH_1 ? CDevicePDT_IF::MTP_CH1_CHECK_SENSOR_ON:CDevicePDT_IF::MTP_CH2_CHECK_SENSOR_ON, bOn);
}

void CUnitCtrlBank::MTP_IF_VacuumStatus_OnOff(JIG_ID jig, JIG_CH ch, BOOL bOn)
{
	theDevicePDT_IF.MTP_LB_Set(jig, ch == JIG_CH_1 ? CDevicePDT_IF::MTP_CH1_VAC_ON_STATUS:CDevicePDT_IF::MTP_CH2_VAC_ON_STATUS, bOn);
	theDevicePDT_IF.MTP_LB_Set(jig, ch == JIG_CH_1 ? CDevicePDT_IF::MTP_CH1_VAC_OFF_STATUS:CDevicePDT_IF::MTP_CH2_VAC_OFF_STATUS, bOn ? FALSE:TRUE);
}

void CUnitCtrlBank::MTP_IF_Reset(JIG_ID jig)
{
	theDevicePDT_IF.MTP_LB_Set(jig, CDevicePDT_IF::MTP_ABLE, FALSE);
	theDevicePDT_IF.MTP_LB_Set(jig, CDevicePDT_IF::MTP_START, FALSE);
	theDevicePDT_IF.MTP_LB_Set(jig, CDevicePDT_IF::MTP_COMPLETE, FALSE);
	theDevicePDT_IF.MTP_LB_Set(jig, CDevicePDT_IF::MTP_CH1_PRODUCT, FALSE);
	theDevicePDT_IF.MTP_LB_Set(jig, CDevicePDT_IF::MTP_CH1_INSPECTION_OK, FALSE);
	theDevicePDT_IF.MTP_LB_Set(jig, CDevicePDT_IF::MTP_CH1_INSPECTION_NG, FALSE);
	theDevicePDT_IF.MTP_LB_Set(jig, CDevicePDT_IF::MTP_CH1_INSPECTION_RETRY_AA, FALSE);
	theDevicePDT_IF.MTP_LB_Set(jig, CDevicePDT_IF::MTP_CH1_INSPECTION_RETRY_AB, FALSE);
	theDevicePDT_IF.MTP_LB_Set(jig, CDevicePDT_IF::MTP_CH1_INSPECTION_RETRY_AC, FALSE);
	theDevicePDT_IF.MTP_LB_Set(jig, CDevicePDT_IF::MTP_CH1_LOADING_STOP, FALSE);
	if(CellTagExist(jig, JIG_CH_1) == FALSE)
		Shuttle_Vac_OnOff(jig, JIG_CH_1, VAC_OFF);

//// 	theDevicePDT_IF.MTP_LB_Set(jig, CDevicePDT_IF::MTP_CH2_ABLE, FALSE);
//// 	theDevicePDT_IF.MTP_LB_Set(jig, CDevicePDT_IF::MTP_CH2_START, FALSE);
//// 	theDevicePDT_IF.MTP_LB_Set(jig, CDevicePDT_IF::MTP_CH2_COMPLETE, FALSE);
//	theDevicePDT_IF.MTP_LB_Set(jig, CDevicePDT_IF::MTP_CH2_PRODUCT, FALSE);
//	theDevicePDT_IF.MTP_LB_Set(jig, CDevicePDT_IF::MTP_CH2_INSPECTION_OK, FALSE);
//	theDevicePDT_IF.MTP_LB_Set(jig, CDevicePDT_IF::MTP_CH2_INSPECTION_NG, FALSE);
//	theDevicePDT_IF.MTP_LB_Set(jig, CDevicePDT_IF::MTP_CH2_INSPECTION_RETRY_AA, FALSE);
//	theDevicePDT_IF.MTP_LB_Set(jig, CDevicePDT_IF::MTP_CH2_INSPECTION_RETRY_AB, FALSE);
//	theDevicePDT_IF.MTP_LB_Set(jig, CDevicePDT_IF::MTP_CH2_INSPECTION_RETRY_AC, FALSE);
//	theDevicePDT_IF.MTP_LB_Set(jig, CDevicePDT_IF::MTP_CH2_LOADING_STOP, FALSE);
//	if(CellTagExist(jig, JIG_CH_2) == FALSE)
//		Shuttle_Vac_OnOff(jig, JIG_CH_2, VAC_OFF);
}

void CUnitCtrlBank::MTP_IF_CellInfo_Write( JIG_ID jig, JIG_CH ch )
{
	CCellInfo *pCell = theCellBank.GetCellInfo(jig, ch);
	if(pCell)
	{
		theDevicePDT_IF.MTP_LW_WriteCellID(jig, ch, pCell->defaultData.m_strCellID);
		theDevicePDT_IF.MTP_LW_WriteNGCode(jig, ch, pCell->defaultData.MesCode);
		switch(pCell->defaultData.m_LastClass)
		{
		case NONE_CELL:
			theDevicePDT_IF.MTP_LB_Set(jig, ch, CDevicePDT_IF::MTP_CH1_PRODUCT, FALSE);
			theDevicePDT_IF.MTP_LB_Set(jig, ch, CDevicePDT_IF::MTP_CH1_INSPECTION_OK, FALSE);
			theDevicePDT_IF.MTP_LB_Set(jig, ch, CDevicePDT_IF::MTP_CH1_INSPECTION_NG, FALSE);
			theDevicePDT_IF.MTP_LB_Set(jig, ch, CDevicePDT_IF::MTP_CH1_INSPECTION_RETRY_AA, FALSE);
			theDevicePDT_IF.MTP_LB_Set(jig, ch, CDevicePDT_IF::MTP_CH1_INSPECTION_RETRY_AB, FALSE);
			theDevicePDT_IF.MTP_LB_Set(jig, ch, CDevicePDT_IF::MTP_CH1_INSPECTION_RETRY_AC, FALSE);
			break;
		case GOOD_CELL:
			theDevicePDT_IF.MTP_LB_Set(jig, ch, CDevicePDT_IF::MTP_CH1_PRODUCT, TRUE);
			theDevicePDT_IF.MTP_LB_Set(jig, ch, CDevicePDT_IF::MTP_CH1_INSPECTION_OK, TRUE);
			theDevicePDT_IF.MTP_LB_Set(jig, ch, CDevicePDT_IF::MTP_CH1_INSPECTION_NG, FALSE);
			theDevicePDT_IF.MTP_LB_Set(jig, ch, CDevicePDT_IF::MTP_CH1_INSPECTION_RETRY_AA, FALSE);
			theDevicePDT_IF.MTP_LB_Set(jig, ch, CDevicePDT_IF::MTP_CH1_INSPECTION_RETRY_AB, FALSE);
			theDevicePDT_IF.MTP_LB_Set(jig, ch, CDevicePDT_IF::MTP_CH1_INSPECTION_RETRY_AC, FALSE);
			break;
		default:
			theDevicePDT_IF.MTP_LB_Set(jig, ch, CDevicePDT_IF::MTP_CH1_PRODUCT, TRUE);
			theDevicePDT_IF.MTP_LB_Set(jig, ch, CDevicePDT_IF::MTP_CH1_INSPECTION_OK, FALSE);
			theDevicePDT_IF.MTP_LB_Set(jig, ch, CDevicePDT_IF::MTP_CH1_INSPECTION_NG, TRUE);
			theDevicePDT_IF.MTP_LB_Set(jig, ch, CDevicePDT_IF::MTP_CH1_INSPECTION_RETRY_AA, FALSE);
			if(pCell->Retry_CheckAble())
				theDevicePDT_IF.MTP_LB_Set(jig, ch, CDevicePDT_IF::MTP_CH1_INSPECTION_RETRY_AB, TRUE);
			else
				theDevicePDT_IF.MTP_LB_Set(jig, ch, CDevicePDT_IF::MTP_CH1_INSPECTION_RETRY_AB, FALSE);
			theDevicePDT_IF.MTP_LB_Set(jig, ch, CDevicePDT_IF::MTP_CH1_INSPECTION_RETRY_AC, FALSE);
			break;
		}
	}
	else
	{
		theDevicePDT_IF.MTP_LW_WriteCellID(jig, ch, _T(""));
		theDevicePDT_IF.MTP_LW_WriteNGCode(jig, ch, _T(""));

		theDevicePDT_IF.MTP_LB_Set(jig, ch, CDevicePDT_IF::MTP_CH1_PRODUCT, FALSE);
		theDevicePDT_IF.MTP_LB_Set(jig, ch, CDevicePDT_IF::MTP_CH1_INSPECTION_OK, FALSE);
		theDevicePDT_IF.MTP_LB_Set(jig, ch, CDevicePDT_IF::MTP_CH1_INSPECTION_NG, FALSE);
		theDevicePDT_IF.MTP_LB_Set(jig, ch, CDevicePDT_IF::MTP_CH1_INSPECTION_RETRY_AA, FALSE);
		theDevicePDT_IF.MTP_LB_Set(jig, ch, CDevicePDT_IF::MTP_CH1_INSPECTION_RETRY_AB, FALSE);
		theDevicePDT_IF.MTP_LB_Set(jig, ch, CDevicePDT_IF::MTP_CH1_INSPECTION_RETRY_AC, FALSE);
	}
}

void CUnitCtrlBank::MTP_IF_LoadingStop_OnOff(JIG_ID jig)
{
	if(theProcBank.LoadingStop_IsRequire())
	{
		theDevicePDT_IF.MTP_LB_Set(jig, JIG_CH_1, CDevicePDT_IF::MTP_CH1_LOADING_STOP, TRUE);
		return;
	}

	for(int i = 0; i < JIG_CH_MAX; i++)
	{
		if(theProcBank.AZoneChannelNotUse_Check(jig, (JIG_CH)i))
			theDevicePDT_IF.MTP_LB_Set(jig, (JIG_CH)i, CDevicePDT_IF::MTP_CH1_LOADING_STOP, TRUE);
		else
			theDevicePDT_IF.MTP_LB_Set(jig, (JIG_CH)i, CDevicePDT_IF::MTP_CH1_LOADING_STOP, FALSE);
	}
}

BOOL CUnitCtrlBank::PDT_IF_Servival_Check(JIG_ID jig)
{
	return theDevicePDT_IF.PDT_LB_Get(jig, CDevicePDT_IF::PDT_SURVIVAL);
}

BOOL CUnitCtrlBank::PDT_IF_Able_Check(JIG_ID jig)
{
	return theDevicePDT_IF.PDT_LB_Get(jig, CDevicePDT_IF::PDT_ABLE);
}

BOOL CUnitCtrlBank::PDT_IF_Start_Check(JIG_ID jig)
{
	return theDevicePDT_IF.PDT_LB_Get(jig, CDevicePDT_IF::PDT_START);
}

BOOL CUnitCtrlBank::PDT_IF_Complete_Check(JIG_ID jig)
{
	return theDevicePDT_IF.PDT_LB_Get(jig, CDevicePDT_IF::PDT_COMPLETE);
}

BOOL CUnitCtrlBank::PDT_IF_ArmStatus_Check(JIG_ID jig)
{
	return theDevicePDT_IF.PDT_LB_Get(jig, CDevicePDT_IF::PDT_ARM_STATUS);
}

//kjparkk 20180131 PDT AB RULE 체크 기능 추가
BOOL CUnitCtrlBank::PDT_IF_RETRY_AB_Check(JIG_ID jig)
{
	return theDevicePDT_IF.PDT_LB_Get(jig, CDevicePDT_IF::PDT_RETRY_AB_RLUE);
}

BOOL CUnitCtrlBank::PDT_IF_VacOnReq_Check(JIG_ID jig, JIG_CH ch)
{
	return theDevicePDT_IF.PDT_LB_Get(jig, ch == JIG_CH_1 ? CDevicePDT_IF::PDT_CH1_VAC_ON_REQ:CDevicePDT_IF::PDT_CH2_VAC_ON_REQ);
}

BOOL CUnitCtrlBank::PDT_IF_VacOffReq_Check(JIG_ID jig, JIG_CH ch)
{
	return theDevicePDT_IF.PDT_LB_Get(jig, ch == JIG_CH_1 ? CDevicePDT_IF::PDT_CH1_VAC_OFF_REQ:CDevicePDT_IF::PDT_CH2_VAC_OFF_REQ);
}

CString CUnitCtrlBank::PDT_IF_CellID_Read(JIG_ID jig, JIG_CH ch)
{
	return theDevicePDT_IF.PDT_LW_ReadCellID(jig, ch);
}
