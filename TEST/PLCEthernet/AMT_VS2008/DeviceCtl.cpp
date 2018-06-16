// DeviceCtl.cpp: implementation of the CDeviceCtl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MT.h"
#include "DeviceCtl.h"
#include "MainFrm.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// singleton
CDeviceCtl* CDeviceCtl::_instance=NULL;
CDeviceCtl* CDeviceCtl::Instance()
{
	if (_instance == NULL) {
		_instance = new CDeviceCtl;
	}
	return _instance;
}
void CDeviceCtl::Release()
{
	// Release() 함수 안에 추가 코드 구현 금지!!!
	if (_instance != NULL) {
		delete _instance;
	}
}
//////////////////////////////////////////////////////////////////////////

CDeviceCtl::CDeviceCtl()
{
	m_nMoveStep = 0 ;
	m_bEnableAreaSensor = FALSE;
	m_bIgnoreInterlock = FALSE;
	m_bMoveTilting = FALSE;
	m_dTimer = 0.0;
	m_bTilt = FALSE;
}

CDeviceCtl::~CDeviceCtl()
{

}

void CDeviceCtl::ContactUnit(BOOL bStatus)
{
	// 올릴 때는 전부 올리고 내릴 때는 해당되는 것만 내린다.
	if(bStatus == UP)
	{
		DioOutput(GetDoc()->GetPortMap()->oContactY1Up, !bStatus);
		DioOutput(GetDoc()->GetPortMap()->oContactY1Down, bStatus);
		DioOutput(GetDoc()->GetPortMap()->oContactY2Up, !bStatus);
		DioOutput(GetDoc()->GetPortMap()->oContactY2Down, bStatus);
		DioOutput(GetDoc()->GetPortMap()->oContactY3Up, !bStatus);
		DioOutput(GetDoc()->GetPortMap()->oContactY3Down, bStatus);
		DioOutput(GetDoc()->GetPortMap()->oContactY4Up, !bStatus);
		DioOutput(GetDoc()->GetPortMap()->oContactY4Down, bStatus);
	}
	else
	{
		if(m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::FOUR_CONT)
		{
			DioOutput(GetDoc()->GetPortMap()->oContactY1Up, !bStatus);
			DioOutput(GetDoc()->GetPortMap()->oContactY1Down, bStatus);
			DioOutput(GetDoc()->GetPortMap()->oContactY2Up, !bStatus);
			DioOutput(GetDoc()->GetPortMap()->oContactY2Down, bStatus);
			DioOutput(GetDoc()->GetPortMap()->oContactY3Up, !bStatus);
			DioOutput(GetDoc()->GetPortMap()->oContactY3Down, bStatus);
			DioOutput(GetDoc()->GetPortMap()->oContactY4Up, !bStatus);
			DioOutput(GetDoc()->GetPortMap()->oContactY4Down, bStatus);
		}
		else if(m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::TWO_CONT)
		{
			DioOutput(GetDoc()->GetPortMap()->oContactY2Up, !bStatus);
			DioOutput(GetDoc()->GetPortMap()->oContactY2Down, bStatus);
			DioOutput(GetDoc()->GetPortMap()->oContactY3Up, !bStatus);
			DioOutput(GetDoc()->GetPortMap()->oContactY3Down, bStatus);
		}
		else
		{
			if(m_pAmtData->m_CurModelInfo.iContactor == CAmtData::CONTACT_0)
			{
				DioOutput(GetDoc()->GetPortMap()->oContactY1Up, !bStatus);
				DioOutput(GetDoc()->GetPortMap()->oContactY1Down, bStatus);
			}
			else if(m_pAmtData->m_CurModelInfo.iContactor == CAmtData::CONTACT_1)
			{
				DioOutput(GetDoc()->GetPortMap()->oContactY2Up, !bStatus);
				DioOutput(GetDoc()->GetPortMap()->oContactY2Down, bStatus);
			}
			else if(m_pAmtData->m_CurModelInfo.iContactor == CAmtData::CONTACT_2)
			{
				DioOutput(GetDoc()->GetPortMap()->oContactY3Up, !bStatus);
				DioOutput(GetDoc()->GetPortMap()->oContactY3Down, bStatus);
			}
			else if(m_pAmtData->m_CurModelInfo.iContactor == CAmtData::CONTACT_3)
			{
				DioOutput(GetDoc()->GetPortMap()->oContactY4Up, !bStatus);
				DioOutput(GetDoc()->GetPortMap()->oContactY4Down, bStatus);
			}
		}
	}
}

void CDeviceCtl::BlowUnit(BOOL bStatus)
{
	if(m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::FOUR_CONT)
	{
		DioOutput(GetDoc()->GetPortMap()->oContactY1Y4AirBlower, bStatus);
		DioOutput(GetDoc()->GetPortMap()->oContactY2Y3AirBlower, bStatus);
	}
	else if(m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::TWO_CONT)
	{
		DioOutput(GetDoc()->GetPortMap()->oContactY2Y3AirBlower, bStatus);
	}
	else
	{
		if(m_pAmtData->m_CurModelInfo.iContactor == CAmtData::CONTACT_0
			|| m_pAmtData->m_CurModelInfo.iContactor == CAmtData::CONTACT_3)
			DioOutput(GetDoc()->GetPortMap()->oContactY1Y4AirBlower, bStatus);
		else
			DioOutput(GetDoc()->GetPortMap()->oContactY2Y3AirBlower, bStatus);
	}
}

void CDeviceCtl::ClampUnit(BOOL bStatus)
{
	DioOutput(GetDoc()->GetPortMap()->oClampCylOn, bStatus);
	DioOutput(GetDoc()->GetPortMap()->oClampCylOff, !bStatus);
}

void CDeviceCtl::FrontLightUnit(BOOL bStatus)
{
	DioOutput(GetDoc()->GetPortMap()->oFrontLight, bStatus);
}

int CDeviceCtl::ChkContactUnitUp()
{
	// Error code를 return한다.
	if (DioInput(GetDoc()->GetPortMap()->iContactY1Up) == FALSE)
		return ERR_CONTACT1_UP;
	else if (DioInput(GetDoc()->GetPortMap()->iContactY2Up) == FALSE)
		return ERR_CONTACT2_UP;
	else if (DioInput(GetDoc()->GetPortMap()->iContactY3Up) == FALSE)
		return ERR_CONTACT3_UP;
	else if (DioInput(GetDoc()->GetPortMap()->iContactY4Up) == FALSE)
		return ERR_CONTACT4_UP;
	else
		return ERR_NONE;
}

void CDeviceCtl::SetMoveStageSoftLimit(int iAxis, bool bEvent)
{
	switch( iAxis )
	{
	case AXIS_CCD_X1:
		{	
			m_pMotion->SetSwLimit( iAxis , 0 , 200 , bEvent );
		}
		break;

	case AXIS_CCD_X2:
		{
			m_pMotion->SetSwLimit( iAxis , 0 , 200 , bEvent );
		}
		break;
	}
}

void CDeviceCtl::SetMovePositionData(int iTarget)
{
	int i ;

	m_iTargetPos = iTarget ;

	if( iTarget == READY_POS )
	{
		for(i = 0 ; i < MAX_CAM_NUM ; i++ )
		{
			ContactMovePos[i].dx = GetDoc()->m_pAmtData->m_CurModelInfo.ReadyPos[i].dx;
			ContactMovePos[i].dy = GetDoc()->m_pAmtData->m_CurModelInfo.ReadyPos[i].dy;
			if(GetLineType() == A_GROUP_TYPE)
				ContactMovePos[i].dz = GetDoc()->m_pAmtData->m_CurModelInfo.ReadyPos[i].dz;
		}
		strMovePosition = "Ready Position";

	}
	else if( iTarget == GRAB_POS )
	{
		for(i = 0 ; i < MAX_CAM_NUM ; i++ )
		{
			ContactMovePos[i].dx = GetDoc()->m_pAmtData->m_CurModelInfo.GrapPos[i].dx;
			ContactMovePos[i].dy = GetDoc()->m_pAmtData->m_CurModelInfo.GrapPos[i].dy;
			if(GetLineType() == A_GROUP_TYPE)
				ContactMovePos[i].dz = GetDoc()->m_pAmtData->m_CurModelInfo.GrapPos[i].dz;
		}
		strMovePosition = "Grab Position";
	}
	else if( iTarget == CONTACT_CHK_POS )
	{
		for(i = 0 ; i < MAX_CAM_NUM ; i++ )
		{
			ContactMovePos[i].dx = GetDoc()->m_pAmtData->StageContactPos.dx[i];
			ContactMovePos[i].dy = GetDoc()->m_pAmtData->StageContactPos.dy[i] - 5;
			if(GetLineType() == A_GROUP_TYPE)
				ContactMovePos[i].dz = GetDoc()->m_pAmtData->m_CurModelInfo.ReadyPos[i].dz;
			strMovePosition = "Contact Check Position";
		}
	}
	else if( iTarget == CONTACT_POS )
	{
		for(i = 0 ; i < MAX_CAM_NUM ; i++ )
		{
			ContactMovePos[i].dx = GetDoc()->m_pAmtData->StageContactPos.dx[i];
			ContactMovePos[i].dy = GetDoc()->m_pAmtData->StageContactPos.dy[i];
			if(GetLineType() == A_GROUP_TYPE)
				ContactMovePos[i].dz = GetDoc()->m_pAmtData->m_CurModelInfo.ReadyPos[i].dz;
			strMovePosition = "Contact Position";
		}
	}
	else if( iTarget == ORG_POS )
	{
		for( i = 0 ; i < MAX_CAM_NUM ; i++ )
		{
			ContactMovePos[i].dx = ORG_POS;
			ContactMovePos[i].dy = ORG_POS;
			strMovePosition = "Org Position";
		}
	}
	else if( iTarget == PCB_CHANGE_POS )
	{
		for( i = 0 ; i < MAX_CAM_NUM ; i++ )
		{
			ContactMovePos[i].dx = GetDoc()->m_pAmtData->PCBChangePos.dx[i];
			ContactMovePos[i].dy = GetDoc()->m_pAmtData->PCBChangePos.dy[i];
			strMovePosition = "Org Position";
		}
	}
}

void CDeviceCtl::SetMovePositionData_Auto(int nCamNum, int nBackOffset)//  [6/13/2009 janghan] Auto Tune 전용 - Grap Position Set
{
	//nBackOffset = 겹침 판정일 경우 Y축 후퇴 할 값
	ContactMovePos[nCamNum].dx = GetDoc()->m_fAutoContactPos_X;
	ContactMovePos[nCamNum].dy = GetDoc()->m_fAutoContactPos_Y - (float)(nBackOffset/10);
	if(GetLineType() == A_GROUP_TYPE)
			ContactMovePos[nCamNum].dz = GetDoc()->m_pAmtData->m_CurModelInfo.GrapPos[nCamNum].dz;
	
	strMovePosition = "Grab Position";		
}


int CDeviceCtl::CheckMoveStageInterference( int iTargetPos )
{
	CString strErrLog = _T("[INTERLOCK]\t\t");
	double dCurX0 , dCurX1 , dCurX2 , dCurX3 , dCurY0 , dCurY1 , dCurY2 , dCurY3;
	double dTarX0 , dTarX1 , dTarX2 , dTarX3 , dTarY0 , dTarY1 , dTarY2 , dTarY3;

	dCurX0 = m_pMotion->GetCmdPos(AXIS_CCD_X0);
	dCurX1 = m_pMotion->GetCmdPos(AXIS_CCD_X1);
	dCurX2 = m_pMotion->GetCmdPos(AXIS_CCD_X2);

	dCurY0 = m_pMotion->GetCmdPos(AXIS_CCD_Y0);
	dCurY1 = m_pMotion->GetCmdPos(AXIS_CCD_Y1);
	dCurY2 = m_pMotion->GetCmdPos(AXIS_CCD_Y2);

	dTarX0 = ContactMovePos[CAM_0].dx;
	dTarX1 = ContactMovePos[CAM_1].dx;
	dTarX2 = ContactMovePos[CAM_2].dx;

	dTarY0 = ContactMovePos[CAM_0].dy;
	dTarY1 = ContactMovePos[CAM_1].dy;
	dTarY2 = ContactMovePos[CAM_2].dy;

	if(GetLineType() != A_GROUP_TYPE)
	{
		dCurX3 = m_pMotion->GetCmdPos(AXIS_CCD_X3);
		dCurY3 = m_pMotion->GetCmdPos(AXIS_CCD_Y3);
		dTarX3 = ContactMovePos[CAM_3].dx;
		dTarY3 = ContactMovePos[CAM_3].dy;
	}

	if(GetLineType() == A_GROUP_TYPE)
	{
		// Check Soft Limit.//
		if( dTarX0 < GetDoc()->m_pMotorParam[AXIS_CCD_X0].dNegSoftLimit || 	dTarX1 < GetDoc()->m_pMotorParam[AXIS_CCD_X1].dNegSoftLimit || dTarX2 < GetDoc()->m_pMotorParam[AXIS_CCD_X2].dNegSoftLimit)
		{
			GetDoc()->SetLog(strErrLog + _T("X축이 이동 하려는 위치가 Soft Negative Limit 값 보다 작습니다."), LOG_ERROR);
			return	MOVE_ERR_SOFT_LIMIT ;
		}
		if( dTarY0 < GetDoc()->m_pMotorParam[AXIS_CCD_Y0].dNegSoftLimit || 	dTarY1 < GetDoc()->m_pMotorParam[AXIS_CCD_Y1].dNegSoftLimit || dTarY2 < GetDoc()->m_pMotorParam[AXIS_CCD_Y2].dNegSoftLimit)
		{
			GetDoc()->SetLog(strErrLog + _T("Y축이 이동 하려는 위치가 Soft Negative Limit 값 보다 작습니다."), LOG_ERROR);
			return	MOVE_ERR_SOFT_LIMIT ;
		}
		if( dTarX0 > GetDoc()->m_dMotionstrokeSingleX || dTarX1 > GetDoc()->m_dMotionstrokeSingleX || dTarX2 > GetDoc()->m_dMotionstrokeSingleX )
		{
			GetDoc()->SetLog(strErrLog + _T("X축이 이동하려는 위치가 X축 단독 Stroke Limit 값 보다 큽니다."), LOG_ERROR);
			return	MOVE_ERR_SOFT_LIMIT ;
		}
		if( dTarY0 > GetDoc()->m_dMotionstrokeSingleY || dTarY1 > GetDoc()->m_dMotionstrokeSingleY || dTarY2 > GetDoc()->m_dMotionstrokeSingleY )
		{
			GetDoc()->SetLog(strErrLog + _T("Y축이 이동하려는 위치가 Y축 단독 Stroke Limit 값 보다 큽니다."), LOG_ERROR);
			return	MOVE_ERR_SOFT_LIMIT ;
		}
		if( dTarX1+dTarX1 > GetDoc()->m_dMotionstrokeFullX )
		{	
			GetDoc()->SetLog(strErrLog + _T("X1번과 X2번이 이동하려는 위치가 충돌이 예상되는 위치입니다."), LOG_ERROR);
			return	MOVE_ERR_SOFT_LIMIT ;
		}
		if( dTarX0 > dTarX1 )
		{
			GetDoc()->SetLog(strErrLog + _T("X축이 이동하려는 위치가 X1이 이동하려는 위치보다 커서 충돌이 예상됩니다."), LOG_ERROR);
			return	MOVE_ERR_SOFT_LIMIT ;
		}

		// Check Preference.//
		if( dTarX1 < dCurX0 )	return	1 ;
		else	return	2 ;

		// return Value : 1  0-AXIS 먼저 이동.. 2 => 1-AXIS 먼저 이동.. -2 => Error.

	}
	else
	{
		
		// Check Soft Limit.//
		if( dTarX0 < GetDoc()->m_pMotorParam[AXIS_CCD_X0].dNegSoftLimit || dTarX1 < GetDoc()->m_pMotorParam[AXIS_CCD_X1].dNegSoftLimit ||  dTarX2 < GetDoc()->m_pMotorParam[AXIS_CCD_X2].dNegSoftLimit || dTarX3 < GetDoc()->m_pMotorParam[AXIS_CCD_X3].dNegSoftLimit)
		{
			GetDoc()->SetLog(strErrLog + _T("X축이 이동 하려는 위치가 Soft Negative Limit 값 보다 작습니다."), LOG_ERROR);
			return	MOVE_ERR_SOFT_LIMIT ;
		}
		if( dTarY0 < GetDoc()->m_pMotorParam[AXIS_CCD_Y0].dNegSoftLimit || dTarY1 < GetDoc()->m_pMotorParam[AXIS_CCD_Y1].dNegSoftLimit || dTarY2 < GetDoc()->m_pMotorParam[AXIS_CCD_Y2].dNegSoftLimit || dTarY3 < GetDoc()->m_pMotorParam[AXIS_CCD_Y3].dNegSoftLimit)
		{
			GetDoc()->SetLog(strErrLog + _T("Y축이 이동 하려는 위치가 Soft Negative Limit 값 보다 작습니다."), LOG_ERROR);
			return	MOVE_ERR_SOFT_LIMIT ;
		}
		if( dTarX0 > GetDoc()->m_dMotionstrokeSingleX || dTarX1 > GetDoc()->m_dMotionstrokeSingleX || dTarX2 > GetDoc()->m_dMotionstrokeSingleX || dTarX3 > GetDoc()->m_dMotionstrokeSingleX )
		{
			GetDoc()->SetLog(strErrLog + _T("X축이 이동하려는 위치가 X축 단독 Stroke Limit 값 보다 큽니다."), LOG_ERROR);
			return	MOVE_ERR_SOFT_LIMIT ;
		}
		if( dTarY0 > GetDoc()->m_dMotionstrokeSingleY || dTarY1 > GetDoc()->m_dMotionstrokeSingleY || dTarY2 > GetDoc()->m_dMotionstrokeSingleY || dTarY3 > GetDoc()->m_dMotionstrokeSingleY )
		{
			GetDoc()->SetLog(strErrLog + _T("Y축이 이동하려는 위치가 Y축 단독 Stroke Limit 값 보다 큽니다."), LOG_ERROR);
			return	MOVE_ERR_SOFT_LIMIT ;
		}
		if( dTarX1+dTarX2 > GetDoc()->m_dMotionstrokeFullX )
		{	// A군과 일반 4컨텍은 이부분 Rule이 다름.
			GetDoc()->SetLog(strErrLog + _T("X1번과 X2번이 이동하려는 위치가 서로 충돌이 예상되는 위치입니다."), LOG_ERROR);
			return	MOVE_ERR_SOFT_LIMIT ;
		}
		if( dTarX0 > dTarX1 )
		{
			GetDoc()->SetLog(strErrLog + _T("X축이 이동하려는 위치가 X1가 이동하려는 위치보다 커서 충돌이 예상됩니다."), LOG_ERROR);
			return	MOVE_ERR_SOFT_LIMIT ;
		}
		if( dTarX3 > dTarX2 )
		{
			GetDoc()->SetLog(strErrLog + _T("X3이 이동하려는 위치가 X2가 이동하려는 위치보다 커서 충돌이 예상됩니다."), LOG_ERROR);
			return	MOVE_ERR_SOFT_LIMIT ;
		}

		
		// 축 이동순서 선정 [3/6/2009 OSC]
		// 혹시나 밑에 걸로 충돌이 나오면 이 방법으로 할 예정
		// 4자리 숫자로 표현한다.
		int nMoveSequence = 0;

		if(dTarX0 < dCurX0)								// 1st Check
		{
			// 0번축이 원점쪽이면 선
			nMoveSequence += ( (CAM_0+1) * 1000 );
			if(dTarX3 < dCurX3)							// 2st Check
			{
				// 3번축이 원점쪽이면 선
				nMoveSequence += ( (CAM_3+1) * 100 );
				if(dTarX1 < dCurX1)						// 3rd Check
				{
					// 0, 3번이 원점쪽이고 1번이 원점쪽이면 1번이 2번보다 선
					nMoveSequence += ( (CAM_1+1) * 10 );
					nMoveSequence += ( (CAM_2+1) * 1 );
				}
				else
				{
					// 1번이 원점쪽이 아니면 2번이 1번보다 선
					nMoveSequence += ( (CAM_2+1) * 10 );
					nMoveSequence += ( (CAM_1+1) * 1 );
				}
			}
			else										// 2st Check
			{
				// 3번이 원점쪽이 아니면
				if(dTarX1 < dCurX1)						// 3rd Check
				{
					// 1번이 원점쪽이면 1, 2번이 먼저 움직이고 3번 움직인다.
					nMoveSequence += ( (CAM_1+1) * 100 );
					nMoveSequence += ( (CAM_2+1) * 10 );
					nMoveSequence += ( (CAM_3+1) * 1 );
				}
				else									// 3rd Check
				{
					// 1번이 원점쪽이 아니면2번이 먼저 움직이고 1, 3번이 움직인다.
					nMoveSequence += ( (CAM_2+1) * 100 );
					nMoveSequence += ( (CAM_1+1) * 10 );
					nMoveSequence += ( (CAM_3+1) * 1 );
				}
			}
		}
		else											// 1st Check
		{
			// 0번이 원점 방향이 아니면
			if(dTarX3 < dCurX3)							// 2st Check
			{
				// 3번이 원점방향이면 3 -> 2 -> 1 -> 0 순서로 움직인다.
				nMoveSequence += ( (CAM_3+1) * 1000 );
				nMoveSequence += ( (CAM_2+1) * 100 );
				nMoveSequence += ( (CAM_1+1) * 10 );
				nMoveSequence += ( (CAM_0+1) * 1 );
			}
			else										// 2st Check
			{
				// 0, 3번이 모두 안쪽으로 모이면
				if(dTarX1 < dCurX1)						// 3rd Check
				{
					// 1번이 원점쪽이면 1 -> 2 -> 0 -> 3으로 움직인다.
					nMoveSequence += ( (CAM_1+1) * 1000 );
					nMoveSequence += ( (CAM_2+1) * 100 );
					nMoveSequence += ( (CAM_0+1) * 10 );
					nMoveSequence += ( (CAM_3+1) * 1 );
				}
				else
				{
					// 2 -> 1 -> 0 -> 3으로 움직인다.
					nMoveSequence += ( (CAM_2+1) * 1000 );
					nMoveSequence += ( (CAM_1+1) * 100 );
					nMoveSequence += ( (CAM_0+1) * 10 );
					nMoveSequence += ( (CAM_3+1) * 1 );
				}

			}
		}
		
		return nMoveSequence;

// 		if( (dTarX1 < dCurX0 || dTarX2 < dCurX3) && dTarX0 < dTarX1 && dTarX3 < dTarX2 )		return	3 ;
// 		if( dTarX0 > dCurX1 || dCurX1+dTarX2 > GetDoc()->m_dMotionstrokeFullX )	return	1 ;
// 		if( dTarX3 > dCurX2 || dCurX2+dTarX1 > GetDoc()->m_dMotionstrokeFullX )	return	2 ;

		// return Value : 1 => 1-AXIS 먼저 이동.. 2 => 2-AXIS 먼저 이동.. -2 => Error.
	}
	return MOVE_ERR_NONE;	// return Value : 0 => A군에서는 2-AXIS 먼저 이동.. 4컨텍에서는 3-AXIS 먼저 이동.
}

int CDeviceCtl::MoveStage_Auto( int iTargetPos , int nCamNum, bool bEndCheck /*= TRUE*/)//  [6/13/2009 janghan] Auto Tune 전용
{
	int nReturn = MOVE_OK_START ;
	int nSeqNo = m_nMoveStep ;
	int nMoveSequenceX;
	static DWORD dTimer = 0 ;
	double dCmdPos, dEncPos;

	int nAxis_X, nAxis_Y;

	switch (nCamNum)
	{
	case 0:
		nAxis_X = 0;
		nAxis_Y = 4; 
		break;
	case 1:
		nAxis_X = 1;
		nAxis_Y = 5; 
		break;
	case 2:
		nAxis_X = 2;
		nAxis_Y = 6; 
		break;
	case 3:
		nAxis_X = 3;
		nAxis_Y = 7; 
		break;
	}

	if (GetDoc()->m_nYaxis_Back == 99)
	{
		SetMovePositionData_Auto(nCamNum);
	}
	else 
	{
		SetMovePositionData_Auto(nCamNum, 1);
	}
	//SetMovePositionData_Auto(nCamNum);
	
	nMoveSequenceX = CheckMoveStageInterference(iTargetPos);

	// Error 발생했을 경우 return
	if(nMoveSequenceX < MOVE_ERR_NONE)
	{
		// Error No만 전달하고 보여주는건 다 끝나고 보여준다.
		GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_CONTACT_CRASH_POS);
		nReturn = MOVE_ERR_SOFT_LIMIT;//Error 처리...
	}

	switch( nSeqNo )
	{
	case MS_CHK_EQUIPMENT:
		if(GetLineType() == A_GROUP_TYPE)
		{
			//nSeqNo = MS_CHK_PIN_Z_READY ;
		}
		else	
		{
			// READY_POS면 Y먼저 움직인다.
			if( m_iTargetPos == READY_POS )		
				nSeqNo =  MS_MOVE_Y ;
			else								
				nSeqNo =  MS_MOVE_X ;
		}
		break;

	case MS_MOVE_Y:
		
		if(m_pMotion->StartMove( nAxis_Y , ContactMovePos[nCamNum].dy , GetDoc()->m_pMotorParam[nAxis_Y].dSpeed ) != MOVE_OK_START)
		{
			GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_1YCONTACT_MOTOR_MOVING);
			nReturn = MOVE_ERR_END ;
			break;
		}

		dTimer = GetTickCount() ;
		nSeqNo = MS_ISOK_Y ;
		break;

	case MS_ISOK_Y:
		
		if( m_pMotion->IsMoveOK( nAxis_Y ))
		{
			if(m_pMotion->CheckAxisError(nAxis_Y) == FALSE)
			{
				// Error No만 전달하고 보여주는건 다 끝나고 보여준다.
				GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_1YCONTACT_MOTOR_MOVING);
				nReturn = MOVE_ERR_END ;	break;
			}
			
			// READY_POS면 X를 마저 움직인다.
			if( m_iTargetPos == READY_POS )		nSeqNo =  MS_MOVE_X ;
			else								nSeqNo =  MS_COMPLETE ;
		}
		else
		{
			if( GetTickCount() - dTimer > 10000 )
			{
				// Error No만 전달하고 보여주는건 다 끝나고 보여준다.
				GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_1YCONTACT_MOTOR_MOVING);
				nReturn = MOVE_ERR_END ;
			}
		}		
		break;

	case MS_MOVE_X:
		if( GetLineType() == A_GROUP_TYPE )
		{
		}
		else
		{
			if(m_pMotion->StartMove( nAxis_X , ContactMovePos[nCamNum].dx , GetDoc()->m_pMotorParam[nCamNum].dSpeed ) != MOVE_OK_START)
			{
				GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_1XCONTACT_MOTOR_MOVING);
				nReturn = MOVE_ERR_END ;
				break;
			}
		}
		dTimer = GetTickCount() ;
		nSeqNo = MS_ISOK_X ;
		break;

	case MS_ISOK_X:
				
		if( m_pMotion->IsMoveOK( nAxis_X ))
		{
			if(m_pMotion->CheckAxisError(nAxis_X) == FALSE)
			{
				// Error No만 전달하고 보여주는건 다 끝나고 보여준다.
				if (GetApp()->m_RunStatus == STATUS_INIT)
				{
					GetMainFrame()->SetMsgNo(MSG_ERR_INIT, ERR_1XCONTACT_MOTOR_MOVING);
				}
				else
				{
					GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_1XCONTACT_MOTOR_MOVING);
				}
				nReturn = MOVE_ERR_END ;	break;
			}
				
			// READY_POS가 아니면 Y를 마저 움직인다.
			if( m_iTargetPos == READY_POS )		nSeqNo =  MS_COMPLETE ;
			else
			{
				// CONACT_POS면 X만 보정한 영상을 찍고 나서 Y 이동한다.
				if(GetApp()->IsRunStatus() && iTargetPos == CONTACT_POS)	nSeqNo = MS_GRAB_CONTACT_IMG ;
				else						nSeqNo = MS_MOVE_Y ;
			}

		}
		else
		{
			if( GetTickCount() - dTimer > 10000 )
			{
				// Error No만 전달하고 보여주는건 다 끝나고 보여준다.
				if (GetApp()->m_RunStatus == STATUS_INIT)
				{
					GetMainFrame()->SetMsgNo(MSG_ERR_INIT, ERR_1XCONTACT_MOTOR_MOVING);
				}
				else
				{
					GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_1XCONTACT_MOTOR_MOVING);
				}
				nReturn = MOVE_ERR_END ;
			}
		}
		
		break;

	case MS_GRAB_CONTACT_IMG:
		// 보정 후 Image Atuo화면에 표시
		// 진행중이면은 Auto창에 마지막 영상을 그대로 남겨야 하기 때문에 일시 정지 유지
		GetAlignCtl()->GrabContactImage();
		nSeqNo = MS_MOVE_Y ;
		break;

	case MS_COMPLETE:
		{
			nSeqNo = MS_CHK_EQUIPMENT ;
			nReturn = MOVE_OK_END;
			for(int nAxis=0;nAxis <= MAX_AXIS;nAxis++)
			{
				if( (GetLineType() == REMOVE_TILTING_TYPE) && (nAxis == AXIS_TILT_MOVE))
				{
				//	GetMelsec()->MelsecNetPosRead( TILT_INPUT_CUR_POS, dEncPos );
				}
				else
				{
					dCmdPos = m_pMotion->GetCmdPos(nAxis);
					dEncPos = m_pMotion->GetActPos(nAxis);
				}

				DeviceLog(nAxis, dCmdPos, dEncPos, strMovePosition);
			}
		}
		break;
	}

	// Error가 발생했을 경우 Step을 처음으로 되돌린다.
	if(nReturn < MOVE_ERR_NONE)
	{
		m_pMotion->E_StopAll();
		m_nMoveStep = MS_CHK_EQUIPMENT;
	}
	else
		m_nMoveStep = nSeqNo;

	return nReturn;
}

int CDeviceCtl::MoveStage( int iTargetPos , bool bEndCheck /*= TRUE*/)
{
	int nReturn = MOVE_OK_START ;
	int nSeqNo = m_nMoveStep ;
	int nMoveSequenceX, nCamNo;
	static DWORD dTimer = 0 ;
	double dCmdPos, dEncPos;

	SetMovePositionData(iTargetPos);	// iTargetPos가 SET_POS이면 그냥 빠져나옴.

	nMoveSequenceX = CheckMoveStageInterference(iTargetPos);

	// Error 발생했을 경우 return
	if(nMoveSequenceX < MOVE_ERR_NONE)
	{
		// Error No만 전달하고 보여주는건 다 끝나고 보여준다.
		GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_CONTACT_CRASH_POS);
		nReturn = MOVE_ERR_SOFT_LIMIT;//Error 처리...
	}

	switch( nSeqNo )
	{
	case MS_CHK_EQUIPMENT:
		if(GetLineType() == A_GROUP_TYPE)
		{
			//nSeqNo = MS_CHK_PIN_Z_READY ;
		}
		else	
		{
			// READY_POS면 Y먼저 움직인다.
			if( m_iTargetPos == READY_POS )		
				nSeqNo =  MS_MOVE_Y ;
			else								
				nSeqNo =  MS_MOVE_X ;
		}
		break;
	case MS_MOVE_Y:
		if( GetLineType() == A_GROUP_TYPE )
		{
			if(m_pMotion->StartMove( AXIS_CCD_Y0 , ContactMovePos[CAM_0].dy , GetDoc()->m_pMotorParam[AXIS_CCD_Y0].dSpeed ) != MOVE_OK_START)
			{
				// Error No만 전달하고 보여주는건 다 끝나고 보여준다.
				GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_1YCONTACT_MOTOR_MOVING);
				nReturn = MOVE_ERR_END ;
				break;
			}
			if(m_pMotion->StartMove( AXIS_CCD_Y1 , ContactMovePos[CAM_1].dy , GetDoc()->m_pMotorParam[AXIS_CCD_Y1].dSpeed ) != MOVE_OK_START)
			{
				// Error No만 전달하고 보여주는건 다 끝나고 보여준다.
				GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_2YCONTACT_MOTOR_MOVING);
				nReturn = MOVE_ERR_END ;
				break;
			}
		}
		else
		{
//			if( (GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::FOUR_CONT) || 
//				((GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::ONE_CONT) && ))
			if(m_pMotion->StartMove( AXIS_CCD_Y0 , ContactMovePos[CAM_0].dy , GetDoc()->m_pMotorParam[AXIS_CCD_Y0].dSpeed ) != MOVE_OK_START)
			{
				GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_1YCONTACT_MOTOR_MOVING);
				nReturn = MOVE_ERR_END ;
				break;
			}
			if(m_pMotion->StartMove( AXIS_CCD_Y1 , ContactMovePos[CAM_1].dy , GetDoc()->m_pMotorParam[AXIS_CCD_Y1].dSpeed ) != MOVE_OK_START)
			{
				GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_2YCONTACT_MOTOR_MOVING);
				nReturn = MOVE_ERR_END ;
				break;
			}
			if(m_pMotion->StartMove( AXIS_CCD_Y2 , ContactMovePos[CAM_2].dy , GetDoc()->m_pMotorParam[AXIS_CCD_Y2].dSpeed ) != MOVE_OK_START)
			{
				GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_3YCONTACT_MOTOR_MOVING);
				nReturn = MOVE_ERR_END ;
				break;
			}
			if(m_pMotion->StartMove( AXIS_CCD_Y3 , ContactMovePos[CAM_3].dy , GetDoc()->m_pMotorParam[AXIS_CCD_Y3].dSpeed ) != MOVE_OK_START)
			{
				GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_4YCONTACT_MOTOR_MOVING);
				nReturn = MOVE_ERR_END ;
				break;
			}
		}
		dTimer = GetTickCount() ;
		nSeqNo = MS_ISOK_Y ;
		break;

	case MS_ISOK_Y:
		if( GetLineType() == A_GROUP_TYPE )
		{
			if( m_pMotion->IsMoveOK( AXIS_CCD_Y0 ) && m_pMotion->IsMoveOK( AXIS_CCD_Y1 ) )
			{
				//nSeqNo = MS_CHK_PREFERENCE ;
			}
			else
			{
				if( GetTickCount() - dTimer > 10000 )
				{
					// Error No만 전달하고 보여주는건 다 끝나고 보여준다.
					if (GetApp()->m_RunStatus == STATUS_INIT)
					{
						GetMainFrame()->SetMsgNo(MSG_ERR_INIT, ERR_1YCONTACT_MOTOR_MOVING);
					}
					else
					{
						GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_1YCONTACT_MOTOR_MOVING);
					}					
					nReturn = MOVE_ERR_END ;
				}
			}
		}
		else
		{
			if( m_pMotion->IsMoveOK( AXIS_CCD_Y0 ) && m_pMotion->IsMoveOK( AXIS_CCD_Y1 ) && m_pMotion->IsMoveOK( AXIS_CCD_Y2 ) && m_pMotion->IsMoveOK( AXIS_CCD_Y3 ) )
			{
				if(m_pMotion->CheckAxisError(AXIS_CCD_Y0) == FALSE)
				{
					// Error No만 전달하고 보여주는건 다 끝나고 보여준다.
					if (GetApp()->m_RunStatus == STATUS_INIT)
					{
						GetMainFrame()->SetMsgNo(MSG_ERR_INIT, ERR_1YCONTACT_MOTOR_MOVING);
					}
					else
					{
						GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_1YCONTACT_MOTOR_MOVING);
					}					
					nReturn = MOVE_ERR_END ;	break;
				}
				if(m_pMotion->CheckAxisError(AXIS_CCD_Y1) == FALSE)
				{
					// Error No만 전달하고 보여주는건 다 끝나고 보여준다.
					if (GetApp()->m_RunStatus == STATUS_INIT)
					{
						GetMainFrame()->SetMsgNo(MSG_ERR_INIT, ERR_2YCONTACT_MOTOR_MOVING);
					}
					else
					{
						GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_2YCONTACT_MOTOR_MOVING);
					}
					nReturn = MOVE_ERR_END ;	break;
				}
				if(m_pMotion->CheckAxisError(AXIS_CCD_Y2) == FALSE)
				{
					// Error No만 전달하고 보여주는건 다 끝나고 보여준다.
					if (GetApp()->m_RunStatus == STATUS_INIT)
					{
						GetMainFrame()->SetMsgNo(MSG_ERR_INIT, ERR_3YCONTACT_MOTOR_MOVING);
					}
					else
					{
						GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_3YCONTACT_MOTOR_MOVING);
					}					
					nReturn = MOVE_ERR_END ;	break;
				}
				if(m_pMotion->CheckAxisError(AXIS_CCD_Y3) == FALSE)
				{
					// Error No만 전달하고 보여주는건 다 끝나고 보여준다.
					if (GetApp()->m_RunStatus == STATUS_INIT)
					{
						GetMainFrame()->SetMsgNo(MSG_ERR_INIT, ERR_4YCONTACT_MOTOR_MOVING);
					}
					else
					{
						GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_4YCONTACT_MOTOR_MOVING);
					}	
					nReturn = MOVE_ERR_END ;	break;
				}
				// READY_POS면 X를 마저 움직인다.
				if( m_iTargetPos == READY_POS )		nSeqNo =  MS_MOVE_X ;
				else								nSeqNo =  MS_COMPLETE ;
			}
			else
			{
				if( GetTickCount() - dTimer > 10000 )
				{
					// Error No만 전달하고 보여주는건 다 끝나고 보여준다.
					if (GetApp()->m_RunStatus == STATUS_INIT)
					{
						GetMainFrame()->SetMsgNo(MSG_ERR_INIT, ERR_4YCONTACT_MOTOR_MOVING);
					}
					else
					{
						GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_4YCONTACT_MOTOR_MOVING);
					}					
					nReturn = MOVE_ERR_END ;
				}
			}
		}
		break;
	case MS_MOVE_X:
		if( GetLineType() == A_GROUP_TYPE )
		{
		}
		else
		{
			// 미리 정해놓은 순서대로 움직인다.
			nCamNo = (nMoveSequenceX/1000) - 1;
			if(m_pMotion->StartMove( nCamNo + AXIS_CCD_X0 , ContactMovePos[nCamNo].dx , GetDoc()->m_pMotorParam[nCamNo + AXIS_CCD_X0].dSpeed ) != MOVE_OK_START)
			{
				GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_1XCONTACT_MOTOR_MOVING);
				nReturn = MOVE_ERR_END ;
				break;
			}
			nMoveSequenceX = nMoveSequenceX%1000;
			nCamNo = (nMoveSequenceX/100) - 1;
			if(m_pMotion->StartMove( nCamNo + AXIS_CCD_X0 , ContactMovePos[nCamNo].dx , GetDoc()->m_pMotorParam[nCamNo + AXIS_CCD_X0].dSpeed ) != MOVE_OK_START)
			{
				GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_2XCONTACT_MOTOR_MOVING);
				nReturn = MOVE_ERR_END ;
				break;
			}
			nMoveSequenceX = nMoveSequenceX%100;
			nCamNo = (nMoveSequenceX/10) - 1;
			if(m_pMotion->StartMove( nCamNo + AXIS_CCD_X0 , ContactMovePos[nCamNo].dx , GetDoc()->m_pMotorParam[nCamNo + AXIS_CCD_X0].dSpeed ) != MOVE_OK_START)
			{
				GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_3XCONTACT_MOTOR_MOVING);
				nReturn = MOVE_ERR_END ;
				break;
			}
			nMoveSequenceX = nMoveSequenceX%10;
			nCamNo = nMoveSequenceX - 1;
			if(m_pMotion->StartMove( nCamNo + AXIS_CCD_X0 , ContactMovePos[nCamNo].dx , GetDoc()->m_pMotorParam[nCamNo + AXIS_CCD_X0].dSpeed ) != MOVE_OK_START)
			{
				GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_4XCONTACT_MOTOR_MOVING);
				nReturn = MOVE_ERR_END ;
				break;
			}
		}
		dTimer = GetTickCount() ;
		nSeqNo = MS_ISOK_X ;
		break;

	case MS_ISOK_X:
		if( GetLineType() == A_GROUP_TYPE )
		{
			if( m_pMotion->IsMoveOK( AXIS_CCD_X0 ) && m_pMotion->IsMoveOK( AXIS_CCD_X1 ) )
			{
				//nSeqNo = MS_CHK_PREFERENCE ;
			}
			else
			{
				if( GetTickCount() - dTimer > 10000 )
				{
					// Error No만 전달하고 보여주는건 다 끝나고 보여준다.
					if (GetApp()->m_RunStatus == STATUS_INIT)
					{
						GetMainFrame()->SetMsgNo(MSG_ERR_INIT, ERR_1XCONTACT_MOTOR_MOVING);
					}
					else
					{
						GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_1XCONTACT_MOTOR_MOVING);
					}					
					nReturn = MOVE_ERR_END ;
				}
			}
		}
		else
		{
			if( m_pMotion->IsMoveOK( AXIS_CCD_X0 ) && m_pMotion->IsMoveOK( AXIS_CCD_X1 ) && m_pMotion->IsMoveOK( AXIS_CCD_X2 ) && m_pMotion->IsMoveOK( AXIS_CCD_X3 ) )
			{
				if(m_pMotion->CheckAxisError(AXIS_CCD_X0) == FALSE)
				{
					// Error No만 전달하고 보여주는건 다 끝나고 보여준다.
					if (GetApp()->m_RunStatus == STATUS_INIT)
					{
						GetMainFrame()->SetMsgNo(MSG_ERR_INIT, ERR_1XCONTACT_MOTOR_MOVING);
					}
					else
					{
						GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_1XCONTACT_MOTOR_MOVING);
					}					
					nReturn = MOVE_ERR_END ;	break;
				}
				if(m_pMotion->CheckAxisError(AXIS_CCD_X1) == FALSE)
				{
					// Error No만 전달하고 보여주는건 다 끝나고 보여준다.
					if (GetApp()->m_RunStatus == STATUS_INIT)
					{
						GetMainFrame()->SetMsgNo(MSG_ERR_INIT, ERR_2XCONTACT_MOTOR_MOVING);
					}
					else
					{
						GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_2XCONTACT_MOTOR_MOVING);
					}
					nReturn = MOVE_ERR_END ;	break;
				}
				if(m_pMotion->CheckAxisError(AXIS_CCD_X2) == FALSE)
				{
					// Error No만 전달하고 보여주는건 다 끝나고 보여준다.
					if (GetApp()->m_RunStatus == STATUS_INIT)
					{
						GetMainFrame()->SetMsgNo(MSG_ERR_INIT, ERR_3XCONTACT_MOTOR_MOVING);
					}
					else
					{
						GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_3XCONTACT_MOTOR_MOVING);
					}		
					nReturn = MOVE_ERR_END ;	break;
				}
				if(m_pMotion->CheckAxisError(AXIS_CCD_X3) == FALSE)
				{
					// Error No만 전달하고 보여주는건 다 끝나고 보여준다.
					if (GetApp()->m_RunStatus == STATUS_INIT)
					{
						GetMainFrame()->SetMsgNo(MSG_ERR_INIT, ERR_4XCONTACT_MOTOR_MOVING);
					}
					else
					{
						GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_4XCONTACT_MOTOR_MOVING);
					}					
					nReturn = MOVE_ERR_END ;	break;
				}
				// READY_POS가 아니면 Y를 마저 움직인다.
				if( m_iTargetPos == READY_POS )		nSeqNo =  MS_COMPLETE ;
				else
				{
					// CONACT_POS면 X만 보정한 영상을 찍고 나서 Y 이동한다.
					if(GetApp()->IsRunStatus() && iTargetPos == CONTACT_POS)	nSeqNo = MS_GRAB_CONTACT_IMG ;
					else						nSeqNo = MS_MOVE_Y ;
				}

			}
			else
			{
				if( GetTickCount() - dTimer > 10000 )
				{
					// Error No만 전달하고 보여주는건 다 끝나고 보여준다.
					if (GetApp()->m_RunStatus == STATUS_INIT)
					{
						GetMainFrame()->SetMsgNo(MSG_ERR_INIT, ERR_1XCONTACT_MOTOR_MOVING);
					}
					else
					{
						GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_1XCONTACT_MOTOR_MOVING);
					}	
					nReturn = MOVE_ERR_END ;
				}
			}
		}
		break;

	case MS_GRAB_CONTACT_IMG:
		// 보정 후 Image Atuo화면에 표시
		// 진행중이면은 Auto창에 마지막 영상을 그대로 남겨야 하기 때문에 일시 정지 유지
		GetAlignCtl()->GrabContactImage();
		nSeqNo = MS_MOVE_Y ;
		break;

	case MS_COMPLETE:
		{
			nSeqNo = MS_CHK_EQUIPMENT ;
			nReturn = MOVE_OK_END;
			for(int nAxis=0;nAxis <= MAX_AXIS;nAxis++)
			{
				if( GetLineType() == REMOVE_TILTING_TYPE && nAxis == AXIS_TILT_MOVE )
				{
				//	GetMelsec()->MelsecNetPosRead( TILT_INPUT_CUR_POS, dEncPos );
				}
				else
				{
					dCmdPos = m_pMotion->GetCmdPos(nAxis);
					dEncPos = m_pMotion->GetActPos(nAxis);
				}

				DeviceLog(nAxis, dCmdPos, dEncPos, strMovePosition);
			}
		}
		break;
	}

	// Error가 발생했을 경우 Step을 처음으로 되돌린다.
	if(nReturn < MOVE_ERR_NONE)
	{
		m_pMotion->E_StopAll();
		m_nMoveStep = MS_CHK_EQUIPMENT;
	}
	else
		m_nMoveStep = nSeqNo;

	return nReturn;
}


BOOL CDeviceCtl::ChkClampUnit( BOOL bSensorOff /*= TRUE*/ )
{
	BYTE ioBit = bSensorOff ? GetDoc()->GetPortMap()->iTopClampOff : GetDoc()->GetPortMap()->iTopClampOn;
	if (DioInput(ioBit))
		return TRUE;

	return FALSE;
}

BOOL CDeviceCtl::ChkPanelExist()
{
	if (DioInput(GetDoc()->GetPortMap()->iPanelCheckSensor1)
		&& DioInput(GetDoc()->GetPortMap()->iPaneCheckSensor2) )
		return TRUE;

	return FALSE;
}

BOOL CDeviceCtl::ChkMainPower(void)
{
	return DioInput(GetDoc()->GetPortMap()->iPowerOnCheck);
}

BOOL CDeviceCtl::ChkArmStretch()
{
	if(m_bIgnoreInterlock)
		return FALSE;

	if(GetMelsec()->MelsecNetBitRecv(MEL_SW_LOADER_TILT) && DioInput(GetDoc()->GetPortMap()->iDDKArmStretch))
		return FALSE;
	else
		return TRUE;
}

void CDeviceCtl::EnableAreaSensor( BOOL bCheckEnable )
{
	m_bEnableAreaSensor = bCheckEnable;
	// SESL측 요청으로 Power를 떨구지는 않음
	//DioOutput(GetDoc()->GetPortMap()->oLightCurtainOff, (bCheckEnable ? FALSE:TRUE));
	DioOutput(GetDoc()->GetPortMap()->oLightCurtainOff, TRUE);
}

void CDeviceCtl::ManualSeqContactUnitMove( int &nMoveAction, BOOL &bPosMoving )
{
	CONTACT_UNIT(UP);
	
	if ((nMoveAction == actAUTOTUNE_CENTER_MOVE) || (nMoveAction == actAUTOTUNE_X_SORT))
		BLOW_UNIT(SENSOR_ON);
	else BLOW_UNIT(SENSOR_OFF);

	int    nRv ;
	double dCmdPos, dEncoderPos;

	switch( nMoveAction )
	{
	case actNONE:
		break;

	case actGRAB_MOVE:
		{
			bPosMoving = TRUE ;
			nRv = MoveStage(GRAB_POS);
			if( nRv == MOVE_OK_END )
			{
				nMoveAction = actMOVE_OK;
				strMoveTarget = "GRAB Position";
			}
			else if( nRv < MOVE_ERR_NONE )
			{
				nMoveAction = actMOVE_ERROR; //Error 처리...
			}
		}
		break;

	case actAUTOTUNE_CENTER_MOVE:
		{
			bPosMoving = TRUE ;
			nRv = MoveStage_Auto(GRAB_POS, GetDoc()->m_nCamera_Auto );//화면 중앙 이동;
			if( nRv == MOVE_OK_END )
			{
				nMoveAction = actMOVE_OK;
				strMoveTarget = "GRAB Position";
			}
			else if( nRv < MOVE_ERR_NONE )
			{
				nMoveAction = actMOVE_ERROR; //Error 처리...
			}
		}
		break;

	case actAUTOTUNE_X_SORT:
		{
			bPosMoving = TRUE ;
			nRv = MoveStage_Auto(GRAB_POS, GetDoc()->m_nCamera_Auto );
			if( nRv == MOVE_OK_END )
			{
				nMoveAction = actMOVE_OK;
				strMoveTarget = "GRAB Position";
			}
			else if( nRv < MOVE_ERR_NONE )
			{
				nMoveAction = actMOVE_ERROR; //Error 처리...
			}
		}
		break;

	case actCONTACT_CHECK_MOVE:
		{
			bPosMoving = TRUE ;
			nRv = MoveStage(CONTACT_CHK_POS);
			if( nRv == MOVE_OK_END )
			{
				nMoveAction = actMOVE_OK;
				strMoveTarget = "Contact Check Position";
			}
			else if( nRv < MOVE_ERR_NONE )
			{
				nMoveAction = actMOVE_ERROR; //Error 처리...
			}
		}
		break;

	case actREADY_MOVE:
		{
			bPosMoving = TRUE ;
			nRv = MoveStage(READY_POS);
			if( nRv == MOVE_OK_END )
			{
				nMoveAction = actMOVE_OK;
				strMoveTarget = "Ready Position";
			}
			else if( nRv < MOVE_ERR_NONE )
			{
				nMoveAction = actMOVE_ERROR; //Error 처리...
			}
		}
		break;

	case actPCB_CHANGE_MOVE:
		{
			bPosMoving = TRUE ;
			nRv = MoveStage(PCB_CHANGE_POS);
			if( nRv == MOVE_OK_END )
			{
				nMoveAction = actMOVE_OK;
				strMoveTarget = "PCB Change Position";
			}
			else if( nRv < MOVE_ERR_NONE )
			{
				nMoveAction = actMOVE_ERROR; //Error 처리...
			}
		}
		break;
	case actMOVE_OK:
		{
			nMoveAction = actNONE;
			bPosMoving = FALSE;
			GetDoc()->SetLog(_T("Move Completion."), LOG_MOVE_COMPLET);
			for (int i= 1; i<=MAX_AXIS;i++)
			{
				if( GetLineType() == REMOVE_TILTING_TYPE )
					if( i == MAX_AXIS )
						return;
				dCmdPos = m_pMotion->GetCmdPos(i);
				dEncoderPos = m_pMotion->GetActPos(i);
				DeviceLog(i, dCmdPos, dEncoderPos, strMoveTarget);
			}
			return;
		}
		break;

	case actMOVE_ERROR:
		{
			// 미리 MoveStage에서 보냈던 Error를 지금 표시
			::PostMessage(GetMainFrame()->m_hWnd, WM_SHOW_MSG_BOX, MSG_NORMAL, ERR_CANCEL);
			nMoveAction = actNONE ;
			bPosMoving = FALSE;
			return;
		}
	}
}

int CDeviceCtl::StartMoveTilting( BOOL bTilt )
{
	double dCmdPos, dEncoderPos;
	CString strMoveTarget;
	int nReturn = MOVE_ERR_NONE;
	CString strLog;

	if(ChkArmStretch())
		nReturn = MOVE_ERR_START;
	else
	{
		m_bTilt = bTilt;
		if( GetLineType() == REMOVE_TILTING_TYPE )
		{
			if( GetMelsec()->MelsecNetBitRecv(MEL_SW2_TILT_AXIS_STOP) )
			{
				strLog.Format("[StartMoveTilting]\tTilting T Axis Stop.");
				GetDoc()->SetLog(strLog, LOG_EQ);

				AfxMessageBox("Please push the reset switch!!!");
				return nReturn;
			}
		}
		GetDeviceCtl()->EnableAreaSensor(TRUE);
		m_bMoveTilting = TRUE;
		if(bTilt)
		{
			if( GetLineType() == REMOVE_TILTING_TYPE )
			{	// 2009.06.15
				if( !GetMelsec()->MelsecNetBitRecv( MEL_SW2_TILT_MOVE_INSP ) )
				{
					GetMelsec()->MelsecNetBitOn( MEL_SW2_TILT_MOVE_INSP );
					nReturn = MOVE_OK_START;

					strLog.Format("[StartMoveTilting]\tBit On\t[MEL_SW2_TILT_MOVE_INSP]");
					GetDoc()->SetLog(strLog, LOG_EQ);
				}
			}
			else
			{
				nReturn = m_pMotion->StartMove( AXIS_TILT_MOVE , GetDoc()->m_dTiltingPos , GetDoc()->m_pMotorParam[AXIS_TILT_MOVE].dSpeed ) ;
				strMoveTarget = "UnTilting Position";
				dCmdPos = m_pMotion->GetCmdPos(AXIS_TILT_MOVE);
				dEncoderPos = m_pMotion->GetActPos(AXIS_TILT_MOVE);
				DeviceLog(AXIS_TILT_MOVE, dCmdPos, dEncoderPos, strMoveTarget);
			}
		}
		else 
		{
			if( GetLineType() == REMOVE_TILTING_TYPE )
			{	// 2009.06.15
				if( !GetMelsec()->MelsecNetBitRecv( MEL_SW2_TILT_MOVE_READY ) )
				{
					GetMelsec()->MelsecNetBitOn( MEL_SW2_TILT_MOVE_READY );
					nReturn = MOVE_OK_START;

					strLog.Format("[StartMoveTilting]\tBit On\t[MEL_SW2_TILT_MOVE_READY]");
					GetDoc()->SetLog(strLog, LOG_EQ);
				}
			}
			else
			{
				nReturn = m_pMotion->StartMove( AXIS_TILT_MOVE , GetDoc()->m_dUnTiltingPos , GetDoc()->m_pMotorParam[AXIS_TILT_MOVE].dSpeed ) ;
				strMoveTarget = "Tilting Position";
				dCmdPos = m_pMotion->GetCmdPos(AXIS_TILT_MOVE);
				dEncoderPos = m_pMotion->GetActPos(AXIS_TILT_MOVE);
				DeviceLog(AXIS_TILT_MOVE, dCmdPos, dEncoderPos, strMoveTarget);
			}
		}
	}
	return nReturn;
}

int CDeviceCtl::CheckMoveTilting( BOOL bTilt )
{
	BOOL bComp;


	int nReturn = MOVE_ERR_NONE;

	if( GetLineType() == REMOVE_TILTING_TYPE )
	{	// 2009.06.15

		if( bTilt )
			bComp = GetMelsec()->MelsecNetBitRecv( MEL_SW2_TILT_INSP );
		else
			bComp = GetMelsec()->MelsecNetBitRecv( MEL_SW2_TILT_READY );

		if( !GetMelsec()->MelsecNetBitRecv( MEL_SW2_TILT_BUSY ) && bComp )
		{
			GetDeviceCtl()->EnableAreaSensor(FALSE);
			if(!GetMelsec()->MelsecNetBitRecv(MEL_SW2_TILT_ALARM_STAT))
			{
				m_bMoveTilting = FALSE;
				nReturn = MOVE_OK_END;
			}
			else
				nReturn = MOVE_ERR_END;
		}

	}
	else
	{
		if(m_pMotion->IsMoveOK(AXIS_TILT_MOVE))
		{
			GetDeviceCtl()->EnableAreaSensor(FALSE);
			if(m_pMotion->CheckAxisError(AXIS_TILT_MOVE))
			{
				m_bMoveTilting = FALSE;
				nReturn = MOVE_OK_END;
			}
			else
				nReturn = MOVE_ERR_END;
		}
	}
	return nReturn;
}

void CDeviceCtl::SetSafeTilting( BOOL bSafe )
{
	// Program 종료시와
	// CEqMainSequence::CheckInterlock에서만 관리
	if(bSafe)
	{
		// tilting이 안전한 위치에 있음
		GetMelsec()->MelsecNetBitOn(MEL_SW_TILT);
		DioOutput(GetDoc()->GetPortMap()->oDDKTiltOut, TRUE);
	}
	else
	{
		// tilting이 안전한 위치에 있지 않음
		GetMelsec()->MelsecNetBitOff(MEL_SW_TILT);
		DioOutput(GetDoc()->GetPortMap()->oDDKTiltOut, FALSE);
	}
}

void CDeviceCtl::SetPCRun( BOOL bSet )
{
	DioOutput(GetDoc()->GetPortMap()->oDDKNetReady, bSet);
	if(bSet)	GetMelsec()->MelsecNetBitOn(MEL_SW_RUN_STATE);
	else		GetMelsec()->MelsecNetBitOff(MEL_SW_RUN_STATE);
}

void CDeviceCtl::SetAlarmBit( BOOL bOn )
{
	if(bOn)	GetMelsec()->MelsecNetBitOn(MEL_SW_ALARM_MODE);
	else		GetMelsec()->MelsecNetBitOff(MEL_SW_ALARM_MODE);
}

void CDeviceCtl::DeviceLog(int nAxis, double dCommendPos, double dEncoderPos, CString strMoveTarget)
{
	CString strTime, strLog;
	CTime time = CTime::GetCurrentTime();
	strLog.Format("[ Axis ] %d\t[ Commend Pulse ] %.3f\t[ Encoder Pulse ] %.3f\t[ Target Position ]%s", nAxis, dCommendPos, dEncoderPos, strMoveTarget);
	strTime.Format("[%02d:%02d:%02d] ", time.GetHour(), time.GetMinute(), time.GetSecond());
	GetDoc()->SetLog(strLog, LOG_MOVE_PULSE);

}

void CDeviceCtl::MoveYReady()
{
	SetMovePositionData(READY_POS);	// iTargetPos가 SET_POS이면 그냥 빠져나옴.
	m_pMotion->StartMove( AXIS_CCD_Y0 , ContactMovePos[CAM_0].dy , GetDoc()->m_pMotorParam[AXIS_CCD_Y0].dSpeed );
	m_pMotion->StartMove( AXIS_CCD_Y1 , ContactMovePos[CAM_1].dy , GetDoc()->m_pMotorParam[AXIS_CCD_Y1].dSpeed );
	m_pMotion->StartMove( AXIS_CCD_Y2 , ContactMovePos[CAM_2].dy , GetDoc()->m_pMotorParam[AXIS_CCD_Y2].dSpeed );
	m_pMotion->StartMove( AXIS_CCD_Y3 , ContactMovePos[CAM_3].dy , GetDoc()->m_pMotorParam[AXIS_CCD_Y3].dSpeed );
}

