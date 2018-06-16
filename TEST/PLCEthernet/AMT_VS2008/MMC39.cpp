// MMC39.cpp: implementation of the CMMC39 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MT.h"
#include "MMC39.h"
#include "MMCWFP39.h"
#include "MainFrm.h"

UINT HomeSearchProc(LPVOID lParam)
{
	int nAxis = (int )lParam;
	TRACE("HomeSearchProc Start %d\n", nAxis);
	if(nAxis == AXIS_TILT_MOVE && GetLineType() == REMOVE_TILTING_TYPE )
	{
		if(GetDeviceCtl()->ChkArmStretch() == FALSE)
		{
			GetDeviceCtl()->m_bMoveTilting = TRUE;
			GetDeviceCtl()->EnableAreaSensor(TRUE);
			GetMelsec()->MelsecNetBitOn( MEL_SW2_TILT_INIT_REQ );
		}
		else
		{
			::PostMessage(GetMainFrame()->m_hWnd, WM_SHOW_MSG_BOX, MSG_ERR_INIT, ERR_HANDLER_CRASH_POS);
			return 0L;
		}
	}
	else
	{
		if( nAxis == AXIS_TILT_MOVE ){
			if(GetDeviceCtl()->ChkArmStretch() == FALSE)
			{
				GetDeviceCtl()->m_bMoveTilting = TRUE;
				GetDeviceCtl()->EnableAreaSensor(TRUE);
			}
			else
			{
				::PostMessage(GetMainFrame()->m_hWnd, WM_SHOW_MSG_BOX, MSG_ERR_INIT, ERR_HANDLER_CRASH_POS);
				return 0L;
			}
		}
		GetDoc()->m_pMotion->m_nOriginFlag[nAxis] = HOME_ING;

		if(GetDoc()->m_pMotorParam[nAxis].iEncoderType == ABS_ENC_MOTOR)
		{
			GetDoc()->m_pMotion->m_nOriginFlag[nAxis] = GetDoc()->m_pMotion->MoveHomeAbs(nAxis);
		}
		else
		{
			GetDoc()->m_pMotion->m_nOriginFlag[nAxis] = GetDoc()->m_pMotion->MoveHomeEnc(nAxis);
		}
	}
	if(nAxis == AXIS_TILT_MOVE)
	{
		GetDeviceCtl()->m_bMoveTilting = FALSE;
		GetDeviceCtl()->EnableAreaSensor(FALSE);
		if( GetLineType() == REMOVE_TILTING_TYPE )
		{
			Sleep(500);
			GetMelsec()->MelsecNetBitOff( MEL_SW2_TILT_INIT_REQ );
		}
	}
	TRACE("HomeSearchProc End %d\n", nAxis);

	return 0L;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMMC39::CMMC39()
{
	m_Init_Motion = FALSE;
	
	HomeFlagResetAll();

	dMotionFullStrokeX = 0.0;
	dMotionFullStrokeY = 0.0;
	dMotionFullStrokeSingle =0.0;
}

CMMC39::~CMMC39()
{

}

BOOL CMMC39::InitMotion()
{
	if( InitMC() )
	{
		return TRUE ;
	}
	else 
		return FALSE;
}

BOOL CMMC39::InitMC()
{
	m_Init_Motion= FALSE;

	// Parameter Setting
	// 	LONG Addr[2] = {0xD8000000, 0xD9000000};
	LONG Addr = 0x8000000;
	int e = mmc_initx(1, &Addr); 
	if(e != 0)
	{
		CString strErrMsg;
		strErrMsg.Format( "Failed to Initialize the MMC board : %s (%d) " , _error_message(e) , e );
		AfxMessageBox(strErrMsg);

		return FALSE;
	}

	m_Init_Motion = TRUE ;

	return TRUE ;
}

BOOL CMMC39::InitParameter()
{
	Sleep(1000);

	for( int i = 0 ; i < MAX_AXIS ; i ++)
	{
		switch( GetDoc()->m_pMotorParam[i].iMotor ){

		case SERVO_MOTOR: fset_servo(i);	break ;
		case STEPPER: fset_stepper(i);	break ;
		case MICRO_STEPPER: fset_micro_stepper(i);	break ;
		default: return FALSE;
		}

		fset_closed_loop(i,GetDoc()->m_pMotorParam[i].iEncoder);
		fset_feedback(i, GetDoc()->m_pMotorParam[i].iFeedback);
		fset_step_mode(i, GetDoc()->m_pMotorParam[i].bStepMode);

		fset_stop_rate(i, 20);
		fset_e_stop_rate(i, 10);
		fset_accel_limit(i, 1000);
		fset_vel_limit(i, 100000.0);

		fset_positive_level(i, GetDoc()->m_pMotorParam[i].bPosLimit);
		fset_negative_level(i, GetDoc()->m_pMotorParam[i].bNegLimit);
		fset_home_level(i, GetDoc()->m_pMotorParam[i].bHomeSen);

		fset_amp_enable_level(i, GetDoc()->m_pMotorParam[i].bAmpEnable);
		fset_amp_fault_level(i, GetDoc()->m_pMotorParam[i].bAmpFault);

		fset_amp_reset_level(i, GetDoc()->m_pMotorParam[i].bAmpReset);
		short sTest;
		fget_inposition_level(i,&sTest);
		if( GetDoc()->m_pMotorParam[i].iInPosion == HIGH )
		{
			fset_inposition_required(i, TRUE);	set_inposition_level(i, LOW);
		}
		else if( GetDoc()->m_pMotorParam[i].iInPosion == LOW )
		{
			fset_inposition_required(i, FALSE);	set_inposition_level(i, LOW);
		}
		else
		{
			fset_inposition_required(i, TRUE);	set_inposition_level(i, HIGH);
		}

		fset_coordinate_direction(i, GetDoc()->m_pMotorParam[i].bCordinateDir);
		fset_encoder_direction(i, GetDoc()->m_pMotorParam[i].bEncoderDir);

		fset_positive_sw_limit(i, MMC_POS_SW_LIMIT, STOP_EVENT);
		fset_negative_sw_limit(i, MMC_NEG_SW_LIMIT, STOP_EVENT);

		fset_positive_limit(i, E_STOP_EVENT);
		fset_negative_limit(i, E_STOP_EVENT);
		fset_home(i, NO_EVENT);
		fset_amp_fault(i, NO_EVENT);
		fset_index_required(i, 0);

		Sleep(100);
		ResetAxis(i);

		// 원점도 다시 잡아야 하므로 Reset
		m_nOriginFlag[i] = HOME_INIT;
	}

// 	for(int i = 0 ; i < MAX_AXIS ; i ++)	set_amp_enable(i, TRUE);
// 	Sleep(1000);
// 	for(int i = 0 ; i < MAX_AXIS ; i ++)	clear_status(i);

	return 1 ;

}

void CMMC39::SetSwLimit(int axis, double neg_limit, double pos_limit, int iEvent)
{
	ClearAxis(axis);

	fset_positive_sw_limit(axis , pos_limit*GetDoc()->m_pMotorParam[axis].dGearRatio , (INT) iEvent );
	fset_negative_sw_limit(axis , neg_limit*GetDoc()->m_pMotorParam[axis].dGearRatio , (INT) iEvent );

}


BOOL CMMC39::ClearAxis(int axis)
{
	if( !m_Init_Motion )	return FALSE;

	if( !CheckMoveDoneLoop( axis , MOTION_CHECK_DONE_INTERVAL * 10 ) ) // 100ms 동안 Move 상태 Check..
		return FALSE;

	return frames_clear(axis);
	
}

BOOL CMMC39::CheckMoveDoneLoop(int iAxis, DWORD timeout)
{

	DWORD dwCurTime = GetTickCount();

	do
	{
		if( ( GetTickCount() - dwCurTime ) > timeout )
		{
			return FALSE;
		}
		
		Sleep(MOTION_CHECK_DONE_INTERVAL);

	}while( !IsMoveOK(iAxis) );
	
	return TRUE ;

}

BOOL CMMC39::IsMoveOK(int iAxis)
{
	if( !axis_done(iAxis) )		return FALSE;

	return TRUE;
			
}

BOOL CMMC39::ResetAxis(int iAxis)
{
	frames_clear(iAxis);
    clear_status(iAxis);
    amp_fault_reset(iAxis);
	amp_fault_set(iAxis);
	
	return TRUE;
}

int CMMC39::StartMove(int iAxis, double dPos , double dVel)
{
	if(	(m_Init_Motion== FALSE) || (m_nOriginFlag[iAxis] != HOME_END) ) return MOVE_ERR_START;

	if((dPos < GetDoc()->m_pMotorParam[iAxis].dNegSoftLimit) || (dPos > GetDoc()->m_pMotorParam[iAxis].dPosSoftLimit))  return MOVE_ERR_SOFT_LIMIT;

	frames_clear(iAxis);

	dPos *= GetDoc()->m_pMotorParam[iAxis].dGearRatio;
	dVel *= GetDoc()->m_pMotorParam[iAxis].dGearRatio;

	int ret = start_move( iAxis , dPos , dVel , GetDoc()->m_pMotorParam[iAxis].iAcc );
	if(ret == MMC_OK)
		return MOVE_OK_START;
	else
		return MOVE_ERR_START;
}

double CMMC39::GetActPos(int iAxis)
{
	if(	m_Init_Motion== FALSE) return 0;

	double dPos;
	get_position(iAxis, &dPos);

	dPos /= GetDoc()->m_pMotorParam[iAxis].dGearRatio;
	return dPos;
}

void CMMC39::E_Stop(int iAxis)
{
	if(	m_Init_Motion== FALSE) return;
	set_e_stop(iAxis);

}

void CMMC39::E_StopAll()
{
	if(	m_Init_Motion== FALSE) return;
	for(int i = 0; i < MAX_AXIS ; i++)
		set_e_stop(i);
}

BOOL CMMC39::GetServoOnStatus(int iAxis)
{
	if(	m_Init_Motion== FALSE) return 0;

	short state;

	get_amp_enable(iAxis , &state);

	return state;
}

BOOL CMMC39::SetAmpEnable(int iAxis, BOOL bAction)
{
	if(	m_Init_Motion== FALSE) return 0;

	short state;

	set_amp_enable( iAxis , (INT) bAction );
	Sleep(5);

	get_amp_enable( iAxis , &state );

	if( state == (INT) bAction )	return TRUE;
	else							return FALSE;

}

double CMMC39::GetCmdPos(int iAxis)
{
	if(	m_Init_Motion== FALSE) return 0;

	double dPos;

	get_command(iAxis , &dPos);

	dPos /= GetDoc()->m_pMotorParam[iAxis].dGearRatio;

	return dPos;
}

void CMMC39::MoveStop(int iAxis)
{
	if(	m_Init_Motion== FALSE) return ;
	v_move_stop(iAxis);
}

void CMMC39::MoveAllStop()
{
	if(	m_Init_Motion== FALSE) return ;

	for(int i = 0 ; i < MAX_AXIS ; i ++)
	{
		v_move_stop(i);
	}
}

void CMMC39::MoveJog(int iAxis, int iAcc , double dVel, int iDir)
{
	if( m_Init_Motion == FALSE )	return;

	dVel *= GetDoc()->m_pMotorParam[iAxis].dGearRatio;

	if( iDir )
		v_move( iAxis , dVel , iAcc );
	else
		v_move( iAxis , dVel * (-1) , iAcc );
}

int CMMC39::AxisStatus(int iAxis)
{

	if(	!m_Init_Motion ) return FALSE;


	int nSource; 
	nSource	= axis_source( iAxis );
	nSource &= 0x1FFF;

	return nSource;

}

int CMMC39::MoveHomeAbs( int iAxis )
{
	//////////////////////////////////////////////////////////////////////////
	// Absolute type 원점 세팅 순서
	//
	// set_amp_resolution(iAxis , (INT) GetDoc()->m_pMotorParam[iAxis].dGearPitch );
	// set_abs_encoder_type(iAxis , 22);
	// set_abs_encoder(iAxis);
	// get_position(iAxis , &dPos );
	// set_command(iAxis , dPos );
	//
	// Yasukawa motor의 경우는 PC를 껏다가 키면 set_abs_encoder()가 제대로 동작하지 않는다.
	// PC를 껏다가 켰으면 set_abs_encoder()를 호출 후 Servo Off/On 후에
	// 다시 set_abs_encoder()를 호출해야 정상적으로 encoder값이 불어와진다.
	// 
	//////////////////////////////////////////////////////////////////////////

	// Motion 정지 
	// Clear는 맨 나중에 한다.
	// 원점 세팅 도중 튀는 것을 막기 위해...
	set_e_stop(iAxis);

	// Alarm이 발생하였을 경우를 대비해 Alarm Reset 1회 시도
	short nAmpStatus;
	get_amp_enable(iAxis, &nAmpStatus);
	if(nAmpStatus == TRUE)
	{
		set_amp_enable(iAxis , FALSE);	
		Sleep(100);
	}
	amp_fault_reset(iAxis);
	amp_fault_set(iAxis);
	frames_clear(iAxis);
	if(amp_fault_switch(iAxis))
		return HOME_ERR;

	// Resolution 설정
	set_amp_resolution(iAxis , (INT) GetDoc()->m_pMotorParam[iAxis].dGearPitch );
	Sleep(10);

	// absolute type 설정
	int ret;
	ret = set_abs_encoder_type(iAxis , GetDoc()->m_pMotorParam[iAxis].nAbsEncoderType);
	if(ret!= MMC_OK) 
		return HOME_ERR;
	Sleep(10);

	// Servo Off 상태에서 한번 호출
	ret = set_abs_encoder(iAxis);
	if(ret!= MMC_OK) 
		return HOME_ERR;
	Sleep(10);

	if(GetDoc()->m_pMotorParam[iAxis].nAbsEncoderType == AbsCSD3)
	{
		Sleep(100);

		set_amp_enable(iAxis , TRUE);	
		Sleep(300);
	}
	else if(GetDoc()->m_pMotorParam[iAxis].nAbsEncoderType == AbsSigma3)
	{
		set_amp_enable(iAxis , TRUE);	
		Sleep(300);
	
		// Servo On 후에 다시 호출
		ret = set_abs_encoder(iAxis);
		if(ret!= MMC_OK) 
			return HOME_ERR;
		Sleep(100);
	}
	//////////////////////////////////////////////////////////////////////////

	// 현재 Encoder 위치를 Command 위치에 적용
	double dPos ;
	short encodeType = 0 ;
	// 4체배를 해제할 수가 없어서 이렇게 4를 나눠준다.
	if(GetDoc()->m_pMotorParam[iAxis].nAbsEncoderType == AbsCSD3)
	{
		get_position(iAxis , &dPos );
		dPos *= -1;
		dPos /= 4;
		Sleep(10);
		set_position(iAxis , dPos );
	}
	else
	{
		get_position(iAxis , &dPos );
		Sleep(10);
		set_command(iAxis , dPos );
	}
	Sleep(10);

	// 설정값 확인
	ret= get_abs_encoder_type( iAxis , & encodeType );
	if(ret!= MMC_OK) 
	{
		TRACE("ERROR! [get_abs_encoder_type]\n");
		return HOME_ERR;
	}

	double dPos1= 0.;
	double dPos2= 0.;
	get_position(iAxis, &dPos1);
	get_command(iAxis, &dPos2);
	if(dPos1 != dPos2) 
	{
		Sleep(100);
		set_command(iAxis, dPos1);
	}

	frames_clear(iAxis);
	// 여기서 E_STOP_EVENT Clear
	clear_status(iAxis);
	return HOME_END;
}

int CMMC39::MoveHomeEnc( int iAxis )
{
	DWORD dwStartTime = GetTickCount();
	DWORD dwLimitTime;
	// Motion 정지 
	// Clear는 맨 나중에 한다.
	// 원점 세팅 도중 튀는 것을 막기 위해...
	set_e_stop(iAxis);

	// Alarm이 발생하였을 경우를 대비해 Alarm Reset 1회 시도
	set_amp_enable(iAxis , FALSE);	
	Sleep(1000);
	amp_fault_reset(iAxis);
	amp_fault_set(iAxis);
	frames_clear(iAxis);
	set_amp_enable(iAxis , TRUE);	
	Sleep(1000);
	if(amp_fault_switch(iAxis))
		return HOME_ERR;

	// 원점 잡을 동안 임시로 S/w linit 제한 해제
	set_positive_sw_limit(iAxis , MMC_POS_SW_LIMIT , NO_EVENT );
	set_negative_sw_limit(iAxis , MMC_NEG_SW_LIMIT , NO_EVENT );

	set_index_required(iAxis , FALSE);				//Encorder C상 사용 하지 않음.
	set_stop_rate( iAxis , GetDoc()->m_pMotorParam[iAxis].iAcc  ) ;

	// -Limit로 이동하기 위한 Event 설정
	set_home(iAxis , NO_EVENT);
	set_negative_limit(iAxis , E_STOP_EVENT);
	set_positive_limit(iAxis, E_STOP_EVENT);

	clear_status(iAxis);
	frames_clear(iAxis);

	double dVel = GetDoc()->m_pMotorParam[iAxis].iHomeSpeed * GetDoc()->m_pMotorParam[iAxis].dGearRatio ;

	// -Limit 감지시까지 무조건 Neg (-) 방향으로 움직인다...
	v_move( iAxis ,  - ( dVel  ) , GetDoc()->m_pMotorParam[iAxis].iAcc/4  );

	// 이동하는시간 최대 80초
	dwStartTime = GetTickCount();
	dwLimitTime = 80000;
	while(GetTickCount() - dwStartTime < dwLimitTime)
	{
		if(axis_done(iAxis))
			break;
		if(amp_fault_switch(iAxis))
		{
			set_e_stop(iAxis);
			return HOME_ERR;
		}
		Sleep(MOTION_CHECK_DONE_INTERVAL);
	}
	if(GetTickCount() - dwStartTime >= dwLimitTime)
	{
		set_e_stop(iAxis);
		return HOME_ERR;
	}

	// 멈춘 위치가 -Limit가 감지된 위치가 아닐 경우 Error
	// +Limit 감지시는 예외처리 구현 전까지 주석
	if( (neg_switch(iAxis) == FALSE)/* || pos_switch(iAxis)*/)
		return HOME_ERR;

	// Neg Sensor 감지시...
	// Home으로 이동하기 위한 Event 설정
	set_home( iAxis , STOP_EVENT );
	set_negative_limit( iAxis , NO_EVENT);
	set_positive_limit( iAxis , E_STOP_EVENT );	

	clear_status(iAxis);
	frames_clear(iAxis);

	// Home까지 가기 위해 Pos 방향으로 이동
	v_move( iAxis , (dVel) , GetDoc()->m_pMotorParam[iAxis].iAcc );
	// 이동하는시간 최대 15초
	dwStartTime = GetTickCount();
	dwLimitTime = 15000;
	while(GetTickCount() - dwStartTime < dwLimitTime)
	{
		if(axis_done(iAxis))
			break;
		if(amp_fault_switch(iAxis))
		{
			set_e_stop(iAxis);
			return HOME_ERR;
		}
		Sleep(MOTION_CHECK_DONE_INTERVAL);
	}
	if(GetTickCount() - dwStartTime >= dwLimitTime)
	{
		set_e_stop(iAxis);
		return HOME_ERR;
	}

	// Home을 지나 +Limit까지 갔을 경우 Error
	if(pos_switch(iAxis) || (home_switch(iAxis) == FALSE))
		return HOME_ERR;

	// Home에서 벗어나기 위한 Event 설정
	set_home( iAxis , NO_EVENT );
	set_negative_limit( iAxis , E_STOP_EVENT);
	set_positive_limit( iAxis , E_STOP_EVENT );	
	clear_status(iAxis);
	frames_clear(iAxis);

	// Home에서 벗어나기 위해 0.5mm씩 스텝 이동
	dwStartTime = GetTickCount();
	while(home_switch(iAxis))
	{
		start_r_move(iAxis, (0.25 * GetDoc()->m_pMotorParam[iAxis].dGearRatio), dVel, GetDoc()->m_pMotorParam[iAxis].iAcc);
		// 이동하는시간 최대 20초
		dwLimitTime = 20000;
		while(GetTickCount() - dwStartTime < dwLimitTime)
		{
			if(axis_done(iAxis))
				break;
			if(amp_fault_switch(iAxis))
			{
				set_e_stop(iAxis);
				return HOME_ERR;
			}
			Sleep(MOTION_CHECK_DONE_INTERVAL);
		}
		if(GetTickCount() - dwStartTime >= dwLimitTime)
		{
			set_e_stop(iAxis);
			return HOME_ERR;
		}
	}

	// Home으로 이동하기 위한 Event 설정
	set_home( iAxis , STOP_EVENT );
	set_negative_limit( iAxis , E_STOP_EVENT);
	set_positive_limit( iAxis , E_STOP_EVENT );	
	clear_status(iAxis);
	frames_clear(iAxis);

	// 다시 Home까지 천천히 접근
	v_move( iAxis , (-1*(dVel * 0.25)) , GetDoc()->m_pMotorParam[iAxis].iAcc/4 );
	// 이동하는시간 최대 10초
	dwStartTime = GetTickCount();
	dwLimitTime = 10000;
	while(GetTickCount() - dwStartTime < dwLimitTime)
	{
		if(axis_done(iAxis))
			break;
		if(amp_fault_switch(iAxis))
		{
			set_e_stop(iAxis);
			return HOME_ERR;
		}
		Sleep(MOTION_CHECK_DONE_INTERVAL);
	}
	if(GetTickCount() - dwStartTime >= dwLimitTime)
	{
		set_e_stop(iAxis);
		return HOME_ERR;
	}

	// Encoder 안정화 시간
	Sleep(1000);

	// Event 및 Limit 원복
	int nEvent;
	double dLimit;
	fget_home(iAxis, (pINT)&nEvent);
	set_home(iAxis , nEvent);

	fget_negative_sw_limit(iAxis, &dLimit, (pINT)&nEvent);
	fset_negative_sw_limit(iAxis , dLimit , nEvent );

	fget_positive_sw_limit(iAxis, &dLimit, (pINT)&nEvent);
	fset_positive_sw_limit(iAxis , dLimit , nEvent );

	// 최종 상태 체크
	if(amp_fault_switch(iAxis))
		return HOME_ERR;
	// Home이 감지된 상태에서 끝나야 하는데 감지 안됬으면 잘못 잡힌 거임
	if(home_switch(iAxis) == FALSE)
		return HOME_ERR;

	// 현재 위치 0
	set_position( iAxis , 0.0 );
	clear_status(iAxis);
	frames_clear(iAxis);

	return HOME_END;
}

BOOL CMMC39::CheckNoEvent( int nAxis )
{
	return (axis_state(nAxis) == NO_EVENT) ? TRUE:FALSE;
}

BOOL CMMC39::CheckHwLimit( int nAxis )
{
	if(neg_switch(nAxis) || pos_switch(nAxis))	return FALSE;
	else return TRUE;
}

BOOL CMMC39::CheckAxisError( int nAxis )
{
	if(CheckNoEvent(nAxis) && CheckHwLimit(nAxis) && GetServoOnStatus(nAxis) && CheckAmpFault(nAxis))
		return TRUE;
	else
		return FALSE;
}

BOOL CMMC39::CheckAmpFault( int nAxis )
{
	return amp_fault_switch(nAxis) ? FALSE:TRUE;
}

void CMMC39::HomeFlagResetAll()
{
	for(int i = 0; i < MAX_AXIS; i++)
	{
		set_amp_enable(i, FALSE);
		m_nOriginFlag[i] = HOME_INIT;
		frames_clear(i);
	}
}

void CMMC39::ClearStatusAll()
{
	for(int i = 0; i < MAX_AXIS; i++)
	{
		frames_clear(i);
		clear_status(i);
	}
}

BOOL CMMC39::CheckHomeSensor( int nAxis )
{
	return home_switch(nAxis);
}

BOOL CMMC39::CheckHomeFlagAll()
{
	for(int i = 0; i < MAX_AXIS; i++)
	{
		if(m_nOriginFlag[i] != HOME_END)
			return FALSE;
	}
	return TRUE;
}