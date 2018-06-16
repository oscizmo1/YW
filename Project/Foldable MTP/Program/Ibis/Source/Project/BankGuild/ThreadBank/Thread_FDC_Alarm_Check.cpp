#include "StdAfx.h"
#include "Thread_FDC_Alarm_Check.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CThread_FDC_Alarm_Check::CThread_FDC_Alarm_Check(void)
{
}


CThread_FDC_Alarm_Check::~CThread_FDC_Alarm_Check(void)
{
}

UINT CThread_FDC_Alarm_Check::FDCMonitorThr( LPVOID lpParam )
{
	int nThreadIndex = (int)lpParam;

	theDeviceMotion.Open_Mp2100(MP2100_THREAD_FDC_MONITOR);
	CUnitCtrlFunc					_func(MP2100_THREAD_FDC_MONITOR);

	// 	return 0;

	//////////////////////////////////////////////////////////////////////////
	// GMS
#ifdef PGMS_USE_DB
	theSerialInterFace.SerialOpen(SERIAL_GMS);
#endif

	CDeviceSerialGMS* pDeviceGMS = theSerialInterFace.GetDeviceGMSHandler1();
	int nGMSCh = 0;
	BOOL bGMSSended = FALSE;
	CStopWatch m_timerGMS;
	//////////////////////////////////////////////////////////////////////////
	// Accura
	CDeviceAccura DeviceAccura;
	DeviceAccura.InitAccura();
	CStopWatch m_timerAccura;
	//////////////////////////////////////////////////////////////////////////
	//kjpark2017/09/01 Serial 정리 - Temp 추가, 라벨 2 삭제, 토크 삭제, 메인에어
	//Temperature
	static UINT nTempIndex = 0;
	static UINT nReadCnt = 0;
	theSerialInterFace.SerialOpen(SERIAL_PC_BOX_TEMP);	
	CStopWatch m_timerPCTemperature;	
	BOOL bTempPCSended = FALSE;
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// Main Air (Main만 필요하기에 LD와 UD만 연결하면 됨)
	//kjpark2017/09/01 Serial 정리 - Temp 추가, 라벨 2 삭제, 토크 삭제, 메인에어
	theSerialInterFace.SerialOpen(SERIAL_VACUUM_MAIN);
	theSerialInterFace.SendSetVacRange();
	//kjpark 20170928 Adboard Read
	int nFDC_VacuumSlot = 0;
	BOOL bVacLoadDMainSended = FALSE;
	CStopWatch m_timerVacLoadMain;
	//////////////////////////////////////////////////////////////////////////
	AxisStatus axisstatus;
	//////////////////////////////////////////////////////////////////////////
	// FDC Write
	CStopWatch m_timerFDCWrite;
	BOOL bFDCStart = FALSE;
	//////////////////////////////////////////////////////////////////////////
	while(theThreadInitializer.m_bThrExist[nThreadIndex] == FALSE)
	{
		Sleep(50);
		// 		continue;

		//////////////////////////////////////////////////////////////////////////
		// GMS (1초 간격으로 GMS, PGMS 번갈아 Read) [12/7/2016 OSC]
#ifdef PGMS_USE_DB
		if(pDeviceGMS->IsOpen())
		{
			if(pDeviceGMS->ParsingReciveValue())
			{
				m_timerGMS.Start();
				bGMSSended = FALSE;
			}
			else if( (m_timerGMS.Stop(FALSE) > 1.) && (bGMSSended == FALSE) )
			{
				pDeviceGMS->SendCommand_Read((GMS_MODULE_CH)nGMSCh);
				nGMSCh++;
				nGMSCh %= GMS_MODULE_CH_MAX;
				bGMSSended = TRUE;
				m_timerGMS.Start();
			}
			else if((m_timerGMS.Stop(FALSE) > 10.) && bGMSSended )
			{
				// 10초가 넘도록 응답이 안오면 bSended를 초기화하여 다시 보내도록 한다
				bGMSSended = FALSE;
				theLog[LOG_FDC].AddBuf(_T("GMS or PGMS time out"));
			}
		}
#else
		_func.ReadFromFilePGMS();
#endif
		//////////////////////////////////////////////////////////////////////////
		// Accura 전력량계 (1초 간격으로 Read) [12/7/2016 OSC]
		if(m_timerAccura.Stop(FALSE) > 1.)
		{
			for(int i = 0; i < ACCURA_CLIENT_MAX; i++)
			{
				DeviceAccura.ReadValue((ACCURA_CLIENT)i);
				//theFDCBank.m_rDataBlock[i] = DeviceAccura.m_rDataBlock[i];
				//kjpark 20170929 FDC 실제값, 보정값, 보고값 구분
				theFDCBank.m_strbWattNet.Format(_T("%f,%f,%f"), DeviceAccura.m_rDataBlock[i].dbWattNet, DeviceAccura.m_rDataBlock[i].dbWattNet, DeviceAccura.m_rDataBlock[i].dbWattNet);
				theFDCBank.m_strbWattTotal.Format(_T("%f,%f,%f"), DeviceAccura.m_rDataBlock[i].dbWattTotal,  DeviceAccura.m_rDataBlock[i].dbWattTotal,  DeviceAccura.m_rDataBlock[i].dbWattTotal);
				theFDCBank.m_strbVoltage_R.Format(_T("%f,%f,%f"), DeviceAccura.m_rDataBlock[i].dbVoltage_R,  DeviceAccura.m_rDataBlock[i].dbVoltage_R,  DeviceAccura.m_rDataBlock[i].dbVoltage_R);
				theFDCBank.m_strbCurrent_R.Format(_T("%f,%f,%f"), DeviceAccura.m_rDataBlock[i].dbCurrent_R, DeviceAccura.m_rDataBlock[i].dbCurrent_R, DeviceAccura.m_rDataBlock[i].dbCurrent_R);
			}
			m_timerAccura.Start();
		}
		//////////////////////////////////////////////////////////////////////////
		// Torque
#ifdef NOT_USED
		for(int i = 0; i < AXIS_ID_MAX; i++)
		{
			theFDCBank.m_dTorque[i] = theDeviceMotion.GetTorque(MP2100_THREAD_FDC_MONITOR, (AXIS_ID)i);
		}
#endif

		////////////////////////////////////////////////////////////////////////////
		//// Temp Alarm
		//TEMPERATURE SENSOR
		//kjpark2017/09/01 Serial 정리 - Temp 추가, 라벨 2 삭제, 토크 삭제, 메인에어
		if(theSerialInterFace.GetPCTemperatureHandler()->IsOpen())
		{
			nTempIndex = theSerialInterFace.GetDeviceAddr();
			if(theSerialInterFace.ParsingTemperatureValue())
			{

				if(nReadCnt > eMAX_TEMPCount)
				{ 					
					theSerialInterFace.CopyTempValue_to_FDCBank(nTempIndex);
					theSerialInterFace.GetPCTemperatureHandler()->IncDeviceAddr();
					nReadCnt = 0;
				}
				nReadCnt++;
				m_timerPCTemperature.Start();
				bTempPCSended = FALSE;
			}
			else if( (m_timerPCTemperature.Stop(FALSE) > 1.) && (bTempPCSended == FALSE) )
			{
				theSerialInterFace.SendTempRead(nTempIndex);
				bTempPCSended = TRUE;
				m_timerPCTemperature.Start();				
			}
			else if((m_timerPCTemperature.Stop(FALSE) > 10.) && bTempPCSended )
			{
				// 10초가 넘도록 응답이 안오면 bSended를 초기화하여 다시 보내도록 한다
				bTempPCSended = FALSE;
				//2017-04-14,skcho,10초동안 응답 없으면 다음 어드레스로 넘어감
				m_timerPCTemperature.Start();
				theSerialInterFace.GetPCTemperatureHandler()->IncDeviceAddr();
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// Fire Alarm
		//kjpark 20170929 FDC 실제값, 보정값, 보고값 구분

		theFDCBank.m_strOpPcFireAlarm.Format(_T("%d,%d,%d"), _func.GetInPutIOCheck(X_OP_PC_FIRE_ALARM, ON),_func.GetInPutIOCheck(X_OP_PC_FIRE_ALARM, ON),_func.GetInPutIOCheck(X_OP_PC_FIRE_ALARM, ON));
		theFDCBank.m_strAlignPgPCFireAlarm.Format(_T("%d,%d,%d"), _func.GetInPutIOCheck(X_ALIGN_PG_PC_FIRE_ALARM, ON),_func.GetInPutIOCheck(X_ALIGN_PG_PC_FIRE_ALARM, ON),_func.GetInPutIOCheck(X_ALIGN_PG_PC_FIRE_ALARM, ON));
		theFDCBank.m_strUtilBoxFireAlarm.Format(_T("%d,%d,%d"), _func.GetInPutIOCheck(X_UTIL_BOX_FIRE_ALARM, ON),_func.GetInPutIOCheck(X_UTIL_BOX_FIRE_ALARM, ON),_func.GetInPutIOCheck(X_UTIL_BOX_FIRE_ALARM, ON));
		theFDCBank.m_strETCBoxFireAlarm.Format(_T("%d,%d,%d"), _func.GetInPutIOCheck(X_C_BOX_A_FIRE_ALARM, ON),_func.GetInPutIOCheck(X_C_BOX_A_FIRE_ALARM, ON),_func.GetInPutIOCheck(X_C_BOX_A_FIRE_ALARM, ON));
		theFDCBank.m_strServoBoxFireAlarm.Format(_T("%d,%d,%d"), _func.GetInPutIOCheck(X_C_BOX_B_FIRE_ALARM, ON),_func.GetInPutIOCheck(X_C_BOX_B_FIRE_ALARM, ON),_func.GetInPutIOCheck(X_C_BOX_B_FIRE_ALARM, ON));
		//////////////////////////////////////////////////////////////////////////
		// Door I/O [10/17/2017 OSC]
		theFDCBank.m_nFrontDoor1Open.Format(_T("%d,%d,%d"), _func.GetInPutIOCheck(X_FRONT_DOOR1_SENSOR, ON),_func.GetInPutIOCheck(X_FRONT_DOOR1_SENSOR, ON),_func.GetInPutIOCheck(X_FRONT_DOOR1_SENSOR, ON));// = _func.GetInPutIOCheck(X_FRONT_DOOR1_SENSOR);
		theFDCBank.m_nFrontDoor2Open.Format(_T("%d,%d,%d"), _func.GetInPutIOCheck(X_FRONT_DOOR2_SENSOR, ON),_func.GetInPutIOCheck(X_FRONT_DOOR2_SENSOR, ON),_func.GetInPutIOCheck(X_FRONT_DOOR2_SENSOR, ON));// = _func.GetInPutIOCheck(X_FRONT_DOOR2_SENSOR);
		theFDCBank.m_nLeftDoorOpen.Format(_T("%d,%d,%d"), _func.GetInPutIOCheck(X_LEFT_DOOR_SENSOR, ON),_func.GetInPutIOCheck(X_LEFT_DOOR_SENSOR, ON),_func.GetInPutIOCheck(X_LEFT_DOOR_SENSOR, ON));// = _func.GetInPutIOCheck(X_BACK_DOOR1_SENSOR);
		theFDCBank.m_nRightDoorOpen.Format(_T("%d,%d,%d"), _func.GetInPutIOCheck(X_RIGNT_DOOR_SENSOR, ON),_func.GetInPutIOCheck(X_RIGNT_DOOR_SENSOR, ON),_func.GetInPutIOCheck(X_RIGNT_DOOR_SENSOR, ON));// = _func.GetInPutIOCheck(X_RIGNT_DOOR_SENSOR);
		//////////////////////////////////////////////////////////////////////////
		// Main Air
		//kjpark2017/09/01 Serial 정리 - Temp 추가, 라벨 2 삭제, 토크 삭제, 메인에어
		if(theSerialInterFace.GetVacuumMainHandler()->IsOpen())
		{
			if(theSerialInterFace.ParsingLDMainVacuumValue())
			{
				theSerialInterFace.CopyLoadADBoardValue_to_FDCBank();
				bVacLoadDMainSended = FALSE;
				//kjpark 20170928 Adboard Read
				m_timerVacLoadMain.Start();
				theFDCBank.CheckLoadADBoardValue();
			}
			//kjpark 20170928 Adboard Read
			else if((m_timerVacLoadMain.Stop(FALSE) > 1.) && (bVacLoadDMainSended == FALSE))
			{
				if(nFDC_VacuumSlot > 4)
					nFDC_VacuumSlot = 0;

				theSerialInterFace.SendVacRead(nFDC_VacuumSlot++);
				bVacLoadDMainSended = TRUE;
				m_timerVacLoadMain.Start();
			}
			else if(m_timerVacLoadMain.Stop(FALSE) > 10.)
			{
				// 10초가 넘도록 응답이 안오면 bSended를 초기화하여 다시 보내도록 한다
				if(bVacLoadDMainSended)
				{
					bVacLoadDMainSended = FALSE;
					theLog[LOG_FDC].AddBuf(_T("Load AD Board time out"));
				}			
			}
		}
		//////////////////////////////////////////////////////////////////////////
		// FDC Write 1초마다 기록
		// 처음 10초는 Data가 아직 변수에 다 들어가 있지 않을 수도있어서 기다린다
		if( (bFDCStart == FALSE) && (m_timerFDCWrite.Stop(FALSE) > 20.))
		{
			if(theSocketInterFace.m_CIM.IsConnected() == FALSE)
			{
				theLog[LOG_FDC].AddBuf(_T("Delete FDC__SV.par"));
				theFDCBank.DeleteFDC_SV_par();
			}
			bFDCStart = TRUE;
		}
		if( bFDCStart && (m_timerFDCWrite.Stop(FALSE) > 1.) )
		{
			theFDCBank.SaveFDC_SV_par();
			m_timerFDCWrite.Start();
		}
		//////////////////////////////////////////////////////////////////////////
	}

	theSerialInterFace.SerialClose(SERIAL_GMS);
	//kjpark2017/09/01 Serial 정리 - Temp 추가, 라벨 2 삭제, 토크 삭제, 메인에어
	theSerialInterFace.SerialClose(SERIAL_VACUUM_MAIN);
	theSerialInterFace.SerialClose(SERIAL_PC_BOX_TEMP);
	return 0;
}
