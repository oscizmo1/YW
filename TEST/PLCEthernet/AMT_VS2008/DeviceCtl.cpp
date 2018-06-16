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
	// Release() �Լ� �ȿ� �߰� �ڵ� ���� ����!!!
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
	// �ø� ���� ���� �ø��� ���� ���� �ش�Ǵ� �͸� ������.
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
	// Error code�� return�Ѵ�.
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

void CDeviceCtl::SetMovePositionData_Auto(int nCamNum, int nBackOffset)//  [6/13/2009 janghan] Auto Tune ���� - Grap Position Set
{
	//nBackOffset = ��ħ ������ ��� Y�� ���� �� ��
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
			GetDoc()->SetLog(strErrLog + _T("X���� �̵� �Ϸ��� ��ġ�� Soft Negative Limit �� ���� �۽��ϴ�."), LOG_ERROR);
			return	MOVE_ERR_SOFT_LIMIT ;
		}
		if( dTarY0 < GetDoc()->m_pMotorParam[AXIS_CCD_Y0].dNegSoftLimit || 	dTarY1 < GetDoc()->m_pMotorParam[AXIS_CCD_Y1].dNegSoftLimit || dTarY2 < GetDoc()->m_pMotorParam[AXIS_CCD_Y2].dNegSoftLimit)
		{
			GetDoc()->SetLog(strErrLog + _T("Y���� �̵� �Ϸ��� ��ġ�� Soft Negative Limit �� ���� �۽��ϴ�."), LOG_ERROR);
			return	MOVE_ERR_SOFT_LIMIT ;
		}
		if( dTarX0 > GetDoc()->m_dMotionstrokeSingleX || dTarX1 > GetDoc()->m_dMotionstrokeSingleX || dTarX2 > GetDoc()->m_dMotionstrokeSingleX )
		{
			GetDoc()->SetLog(strErrLog + _T("X���� �̵��Ϸ��� ��ġ�� X�� �ܵ� Stroke Limit �� ���� Ů�ϴ�."), LOG_ERROR);
			return	MOVE_ERR_SOFT_LIMIT ;
		}
		if( dTarY0 > GetDoc()->m_dMotionstrokeSingleY || dTarY1 > GetDoc()->m_dMotionstrokeSingleY || dTarY2 > GetDoc()->m_dMotionstrokeSingleY )
		{
			GetDoc()->SetLog(strErrLog + _T("Y���� �̵��Ϸ��� ��ġ�� Y�� �ܵ� Stroke Limit �� ���� Ů�ϴ�."), LOG_ERROR);
			return	MOVE_ERR_SOFT_LIMIT ;
		}
		if( dTarX1+dTarX1 > GetDoc()->m_dMotionstrokeFullX )
		{	
			GetDoc()->SetLog(strErrLog + _T("X1���� X2���� �̵��Ϸ��� ��ġ�� �浹�� ����Ǵ� ��ġ�Դϴ�."), LOG_ERROR);
			return	MOVE_ERR_SOFT_LIMIT ;
		}
		if( dTarX0 > dTarX1 )
		{
			GetDoc()->SetLog(strErrLog + _T("X���� �̵��Ϸ��� ��ġ�� X1�� �̵��Ϸ��� ��ġ���� Ŀ�� �浹�� ����˴ϴ�."), LOG_ERROR);
			return	MOVE_ERR_SOFT_LIMIT ;
		}

		// Check Preference.//
		if( dTarX1 < dCurX0 )	return	1 ;
		else	return	2 ;

		// return Value : 1  0-AXIS ���� �̵�.. 2 => 1-AXIS ���� �̵�.. -2 => Error.

	}
	else
	{
		
		// Check Soft Limit.//
		if( dTarX0 < GetDoc()->m_pMotorParam[AXIS_CCD_X0].dNegSoftLimit || dTarX1 < GetDoc()->m_pMotorParam[AXIS_CCD_X1].dNegSoftLimit ||  dTarX2 < GetDoc()->m_pMotorParam[AXIS_CCD_X2].dNegSoftLimit || dTarX3 < GetDoc()->m_pMotorParam[AXIS_CCD_X3].dNegSoftLimit)
		{
			GetDoc()->SetLog(strErrLog + _T("X���� �̵� �Ϸ��� ��ġ�� Soft Negative Limit �� ���� �۽��ϴ�."), LOG_ERROR);
			return	MOVE_ERR_SOFT_LIMIT ;
		}
		if( dTarY0 < GetDoc()->m_pMotorParam[AXIS_CCD_Y0].dNegSoftLimit || dTarY1 < GetDoc()->m_pMotorParam[AXIS_CCD_Y1].dNegSoftLimit || dTarY2 < GetDoc()->m_pMotorParam[AXIS_CCD_Y2].dNegSoftLimit || dTarY3 < GetDoc()->m_pMotorParam[AXIS_CCD_Y3].dNegSoftLimit)
		{
			GetDoc()->SetLog(strErrLog + _T("Y���� �̵� �Ϸ��� ��ġ�� Soft Negative Limit �� ���� �۽��ϴ�."), LOG_ERROR);
			return	MOVE_ERR_SOFT_LIMIT ;
		}
		if( dTarX0 > GetDoc()->m_dMotionstrokeSingleX || dTarX1 > GetDoc()->m_dMotionstrokeSingleX || dTarX2 > GetDoc()->m_dMotionstrokeSingleX || dTarX3 > GetDoc()->m_dMotionstrokeSingleX )
		{
			GetDoc()->SetLog(strErrLog + _T("X���� �̵��Ϸ��� ��ġ�� X�� �ܵ� Stroke Limit �� ���� Ů�ϴ�."), LOG_ERROR);
			return	MOVE_ERR_SOFT_LIMIT ;
		}
		if( dTarY0 > GetDoc()->m_dMotionstrokeSingleY || dTarY1 > GetDoc()->m_dMotionstrokeSingleY || dTarY2 > GetDoc()->m_dMotionstrokeSingleY || dTarY3 > GetDoc()->m_dMotionstrokeSingleY )
		{
			GetDoc()->SetLog(strErrLog + _T("Y���� �̵��Ϸ��� ��ġ�� Y�� �ܵ� Stroke Limit �� ���� Ů�ϴ�."), LOG_ERROR);
			return	MOVE_ERR_SOFT_LIMIT ;
		}
		if( dTarX1+dTarX2 > GetDoc()->m_dMotionstrokeFullX )
		{	// A���� �Ϲ� 4������ �̺κ� Rule�� �ٸ�.
			GetDoc()->SetLog(strErrLog + _T("X1���� X2���� �̵��Ϸ��� ��ġ�� ���� �浹�� ����Ǵ� ��ġ�Դϴ�."), LOG_ERROR);
			return	MOVE_ERR_SOFT_LIMIT ;
		}
		if( dTarX0 > dTarX1 )
		{
			GetDoc()->SetLog(strErrLog + _T("X���� �̵��Ϸ��� ��ġ�� X1�� �̵��Ϸ��� ��ġ���� Ŀ�� �浹�� ����˴ϴ�."), LOG_ERROR);
			return	MOVE_ERR_SOFT_LIMIT ;
		}
		if( dTarX3 > dTarX2 )
		{
			GetDoc()->SetLog(strErrLog + _T("X3�� �̵��Ϸ��� ��ġ�� X2�� �̵��Ϸ��� ��ġ���� Ŀ�� �浹�� ����˴ϴ�."), LOG_ERROR);
			return	MOVE_ERR_SOFT_LIMIT ;
		}

		
		// �� �̵����� ���� [3/6/2009 OSC]
		// Ȥ�ó� �ؿ� �ɷ� �浹�� ������ �� ������� �� ����
		// 4�ڸ� ���ڷ� ǥ���Ѵ�.
		int nMoveSequence = 0;

		if(dTarX0 < dCurX0)								// 1st Check
		{
			// 0������ �������̸� ��
			nMoveSequence += ( (CAM_0+1) * 1000 );
			if(dTarX3 < dCurX3)							// 2st Check
			{
				// 3������ �������̸� ��
				nMoveSequence += ( (CAM_3+1) * 100 );
				if(dTarX1 < dCurX1)						// 3rd Check
				{
					// 0, 3���� �������̰� 1���� �������̸� 1���� 2������ ��
					nMoveSequence += ( (CAM_1+1) * 10 );
					nMoveSequence += ( (CAM_2+1) * 1 );
				}
				else
				{
					// 1���� �������� �ƴϸ� 2���� 1������ ��
					nMoveSequence += ( (CAM_2+1) * 10 );
					nMoveSequence += ( (CAM_1+1) * 1 );
				}
			}
			else										// 2st Check
			{
				// 3���� �������� �ƴϸ�
				if(dTarX1 < dCurX1)						// 3rd Check
				{
					// 1���� �������̸� 1, 2���� ���� �����̰� 3�� �����δ�.
					nMoveSequence += ( (CAM_1+1) * 100 );
					nMoveSequence += ( (CAM_2+1) * 10 );
					nMoveSequence += ( (CAM_3+1) * 1 );
				}
				else									// 3rd Check
				{
					// 1���� �������� �ƴϸ�2���� ���� �����̰� 1, 3���� �����δ�.
					nMoveSequence += ( (CAM_2+1) * 100 );
					nMoveSequence += ( (CAM_1+1) * 10 );
					nMoveSequence += ( (CAM_3+1) * 1 );
				}
			}
		}
		else											// 1st Check
		{
			// 0���� ���� ������ �ƴϸ�
			if(dTarX3 < dCurX3)							// 2st Check
			{
				// 3���� ���������̸� 3 -> 2 -> 1 -> 0 ������ �����δ�.
				nMoveSequence += ( (CAM_3+1) * 1000 );
				nMoveSequence += ( (CAM_2+1) * 100 );
				nMoveSequence += ( (CAM_1+1) * 10 );
				nMoveSequence += ( (CAM_0+1) * 1 );
			}
			else										// 2st Check
			{
				// 0, 3���� ��� �������� ���̸�
				if(dTarX1 < dCurX1)						// 3rd Check
				{
					// 1���� �������̸� 1 -> 2 -> 0 -> 3���� �����δ�.
					nMoveSequence += ( (CAM_1+1) * 1000 );
					nMoveSequence += ( (CAM_2+1) * 100 );
					nMoveSequence += ( (CAM_0+1) * 10 );
					nMoveSequence += ( (CAM_3+1) * 1 );
				}
				else
				{
					// 2 -> 1 -> 0 -> 3���� �����δ�.
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

		// return Value : 1 => 1-AXIS ���� �̵�.. 2 => 2-AXIS ���� �̵�.. -2 => Error.
	}
	return MOVE_ERR_NONE;	// return Value : 0 => A�������� 2-AXIS ���� �̵�.. 4���ؿ����� 3-AXIS ���� �̵�.
}

int CDeviceCtl::MoveStage_Auto( int iTargetPos , int nCamNum, bool bEndCheck /*= TRUE*/)//  [6/13/2009 janghan] Auto Tune ����
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

	// Error �߻����� ��� return
	if(nMoveSequenceX < MOVE_ERR_NONE)
	{
		// Error No�� �����ϰ� �����ִ°� �� ������ �����ش�.
		GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_CONTACT_CRASH_POS);
		nReturn = MOVE_ERR_SOFT_LIMIT;//Error ó��...
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
			// READY_POS�� Y���� �����δ�.
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
				// Error No�� �����ϰ� �����ִ°� �� ������ �����ش�.
				GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_1YCONTACT_MOTOR_MOVING);
				nReturn = MOVE_ERR_END ;	break;
			}
			
			// READY_POS�� X�� ���� �����δ�.
			if( m_iTargetPos == READY_POS )		nSeqNo =  MS_MOVE_X ;
			else								nSeqNo =  MS_COMPLETE ;
		}
		else
		{
			if( GetTickCount() - dTimer > 10000 )
			{
				// Error No�� �����ϰ� �����ִ°� �� ������ �����ش�.
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
				// Error No�� �����ϰ� �����ִ°� �� ������ �����ش�.
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
				
			// READY_POS�� �ƴϸ� Y�� ���� �����δ�.
			if( m_iTargetPos == READY_POS )		nSeqNo =  MS_COMPLETE ;
			else
			{
				// CONACT_POS�� X�� ������ ������ ��� ���� Y �̵��Ѵ�.
				if(GetApp()->IsRunStatus() && iTargetPos == CONTACT_POS)	nSeqNo = MS_GRAB_CONTACT_IMG ;
				else						nSeqNo = MS_MOVE_Y ;
			}

		}
		else
		{
			if( GetTickCount() - dTimer > 10000 )
			{
				// Error No�� �����ϰ� �����ִ°� �� ������ �����ش�.
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
		// ���� �� Image Atuoȭ�鿡 ǥ��
		// �������̸��� Autoâ�� ������ ������ �״�� ���ܾ� �ϱ� ������ �Ͻ� ���� ����
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

	// Error�� �߻����� ��� Step�� ó������ �ǵ�����.
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

	SetMovePositionData(iTargetPos);	// iTargetPos�� SET_POS�̸� �׳� ��������.

	nMoveSequenceX = CheckMoveStageInterference(iTargetPos);

	// Error �߻����� ��� return
	if(nMoveSequenceX < MOVE_ERR_NONE)
	{
		// Error No�� �����ϰ� �����ִ°� �� ������ �����ش�.
		GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_CONTACT_CRASH_POS);
		nReturn = MOVE_ERR_SOFT_LIMIT;//Error ó��...
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
			// READY_POS�� Y���� �����δ�.
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
				// Error No�� �����ϰ� �����ִ°� �� ������ �����ش�.
				GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_1YCONTACT_MOTOR_MOVING);
				nReturn = MOVE_ERR_END ;
				break;
			}
			if(m_pMotion->StartMove( AXIS_CCD_Y1 , ContactMovePos[CAM_1].dy , GetDoc()->m_pMotorParam[AXIS_CCD_Y1].dSpeed ) != MOVE_OK_START)
			{
				// Error No�� �����ϰ� �����ִ°� �� ������ �����ش�.
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
					// Error No�� �����ϰ� �����ִ°� �� ������ �����ش�.
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
					// Error No�� �����ϰ� �����ִ°� �� ������ �����ش�.
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
					// Error No�� �����ϰ� �����ִ°� �� ������ �����ش�.
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
					// Error No�� �����ϰ� �����ִ°� �� ������ �����ش�.
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
					// Error No�� �����ϰ� �����ִ°� �� ������ �����ش�.
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
				// READY_POS�� X�� ���� �����δ�.
				if( m_iTargetPos == READY_POS )		nSeqNo =  MS_MOVE_X ;
				else								nSeqNo =  MS_COMPLETE ;
			}
			else
			{
				if( GetTickCount() - dTimer > 10000 )
				{
					// Error No�� �����ϰ� �����ִ°� �� ������ �����ش�.
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
			// �̸� ���س��� ������� �����δ�.
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
					// Error No�� �����ϰ� �����ִ°� �� ������ �����ش�.
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
					// Error No�� �����ϰ� �����ִ°� �� ������ �����ش�.
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
					// Error No�� �����ϰ� �����ִ°� �� ������ �����ش�.
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
					// Error No�� �����ϰ� �����ִ°� �� ������ �����ش�.
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
					// Error No�� �����ϰ� �����ִ°� �� ������ �����ش�.
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
				// READY_POS�� �ƴϸ� Y�� ���� �����δ�.
				if( m_iTargetPos == READY_POS )		nSeqNo =  MS_COMPLETE ;
				else
				{
					// CONACT_POS�� X�� ������ ������ ��� ���� Y �̵��Ѵ�.
					if(GetApp()->IsRunStatus() && iTargetPos == CONTACT_POS)	nSeqNo = MS_GRAB_CONTACT_IMG ;
					else						nSeqNo = MS_MOVE_Y ;
				}

			}
			else
			{
				if( GetTickCount() - dTimer > 10000 )
				{
					// Error No�� �����ϰ� �����ִ°� �� ������ �����ش�.
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
		// ���� �� Image Atuoȭ�鿡 ǥ��
		// �������̸��� Autoâ�� ������ ������ �״�� ���ܾ� �ϱ� ������ �Ͻ� ���� ����
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

	// Error�� �߻����� ��� Step�� ó������ �ǵ�����.
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
	// SESL�� ��û���� Power�� �������� ����
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
				nMoveAction = actMOVE_ERROR; //Error ó��...
			}
		}
		break;

	case actAUTOTUNE_CENTER_MOVE:
		{
			bPosMoving = TRUE ;
			nRv = MoveStage_Auto(GRAB_POS, GetDoc()->m_nCamera_Auto );//ȭ�� �߾� �̵�;
			if( nRv == MOVE_OK_END )
			{
				nMoveAction = actMOVE_OK;
				strMoveTarget = "GRAB Position";
			}
			else if( nRv < MOVE_ERR_NONE )
			{
				nMoveAction = actMOVE_ERROR; //Error ó��...
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
				nMoveAction = actMOVE_ERROR; //Error ó��...
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
				nMoveAction = actMOVE_ERROR; //Error ó��...
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
				nMoveAction = actMOVE_ERROR; //Error ó��...
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
				nMoveAction = actMOVE_ERROR; //Error ó��...
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
			// �̸� MoveStage���� ���´� Error�� ���� ǥ��
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
	// Program ����ÿ�
	// CEqMainSequence::CheckInterlock������ ����
	if(bSafe)
	{
		// tilting�� ������ ��ġ�� ����
		GetMelsec()->MelsecNetBitOn(MEL_SW_TILT);
		DioOutput(GetDoc()->GetPortMap()->oDDKTiltOut, TRUE);
	}
	else
	{
		// tilting�� ������ ��ġ�� ���� ����
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
	SetMovePositionData(READY_POS);	// iTargetPos�� SET_POS�̸� �׳� ��������.
	m_pMotion->StartMove( AXIS_CCD_Y0 , ContactMovePos[CAM_0].dy , GetDoc()->m_pMotorParam[AXIS_CCD_Y0].dSpeed );
	m_pMotion->StartMove( AXIS_CCD_Y1 , ContactMovePos[CAM_1].dy , GetDoc()->m_pMotorParam[AXIS_CCD_Y1].dSpeed );
	m_pMotion->StartMove( AXIS_CCD_Y2 , ContactMovePos[CAM_2].dy , GetDoc()->m_pMotorParam[AXIS_CCD_Y2].dSpeed );
	m_pMotion->StartMove( AXIS_CCD_Y3 , ContactMovePos[CAM_3].dy , GetDoc()->m_pMotorParam[AXIS_CCD_Y3].dSpeed );
}

