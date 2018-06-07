#include "StdAfx.h"
#include "FDCBank.h"
#include "Etc\FileSupport.h"
#include "Etc\Ini.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CFDCItem::CFDCItem( void )
{
	Init();
}

CFDCItem::CFDCItem( CString strSection, CString strKey, FDC_DATATYPE type, void *ptr )
{
	m_strSection = strSection;
	m_strKey = strKey;
	SetValuePtr(type, ptr);
}

CFDCItem::~CFDCItem( void )
{

}

void CFDCItem::Init()
{
	m_strSection.Empty();
	InitValue();
}

void CFDCItem::InitValue()
{
	m_ptrInt = NULL;
	m_ptrDouble = NULL;
	m_ptrString = NULL;
	m_Type = FDC_DATA_STRING;
}
CString CFDCItem::GetSection()
{
	return m_strSection;
}

CString CFDCItem::GetKey()
{
	return m_strKey;
}

void CFDCItem::SetValuePtr( FDC_DATATYPE type, void *ptr )
{
	InitValue();
	m_Type = type;
	switch(m_Type)
	{
	case FDC_DATA_INT:				
	case FDC_DATA_INT_ADD1:			m_ptrInt =			(int *)ptr;				break;
	case FDC_DATA_DOUBLE:			m_ptrDouble =		(double *)ptr;			break;
	case FDC_DATA_STRING:			m_ptrString =		(CString *)ptr;			break;
		break;
	}
}

CString CFDCItem::ValueToString()
{
	CString strRetrunText;
	switch(m_Type)
	{
	case FDC_DATA_INT:	
		if(m_ptrInt)	strRetrunText.Format(_T("%d"), *m_ptrInt);			
		break;
	case FDC_DATA_INT_ADD1:	
		if(m_ptrInt)	strRetrunText.Format(_T("%d"), *m_ptrInt + 1);			
		break;
	case FDC_DATA_DOUBLE:	
		if(m_ptrDouble)	strRetrunText.Format(_T("%.3f"), *m_ptrDouble);		
		break;
	case FDC_DATA_STRING:	
		if(m_ptrString)	strRetrunText.Format(_T("%s"), m_ptrString->GetString());		
		break;
	}

	return strRetrunText;
}

void CFDCItem::WriteFile(CString strPath)
{
	CIni ini(strPath);

	ini.WriteString(GetSection(), GetKey(), ValueToString());
}

CFDCBank::CFDCBank(void)
{	
	//kjpark 20170929 FDC 실제값, 보정값, 보고값 구분
	m_strLoadMainAir = _T("48.4,48.4,48.4");
	//kjpark2017/09/01 Serial 정리 - Temp 추가, 라벨 2 삭제, 토크 삭제, 메인에어
	m_strUtilBox_Temp = _T("25.0");
	m_strEtcBox_Temp = _T("25.0");
	m_strServoBox_Temp = _T("25.0");
	m_strCtrlPCBox_Temp = _T("25.0");
	m_strPGPcBox_Temp = _T("25.0");
	m_stateMainAir = FDC_NORMAL;
	m_strUtilBoxFireAlarm = _T("0");
	m_strETCBoxFireAlarm = _T("0");
	m_strServoBoxFireAlarm = _T("0");
}

CFDCBank::~CFDCBank(void)
{

}

void CFDCBank::InitItem()
{
	m_listFDCItem.RemoveAll();

	CDeviceSerialGMS* pDeviceGMS1 = theSerialInterFace.GetDeviceGMSHandler1();
	CDeviceSerialGMS* pDeviceGMS2 = theSerialInterFace.GetDeviceGMSHandler2();

	CFDCItem item;

	//////////////////////////////////////////////////////////////////////////
	// COMMON [11/16/2016 OSC]
	//kjpark 20170929 FDC 실제값, 보정값, 보고값 구분
	item = CFDCItem(_T("UTIL"),				_T("JIG_1_CH1_VAC"),					FDC_DATA_STRING,	&m_strWorkTableVac[JIG_ID_A][JIG_CH_1]);			m_listFDCItem.AddTail(item);
 	item = CFDCItem(_T("UTIL"),				_T("JIG_2_CH1_VAC"),					FDC_DATA_STRING,	&m_strWorkTableVac[JIG_ID_B][JIG_CH_1]);			m_listFDCItem.AddTail(item);
 	item = CFDCItem(_T("UTIL"),				_T("MAIN_AIR"),							FDC_DATA_STRING,	&m_strLoadMainAir);									m_listFDCItem.AddTail(item);
  	item = CFDCItem(_T("UTIL"),				_T("GMS_1"),							FDC_DATA_STRING,	&pDeviceGMS1->m_LastGMSValue[GMS_SUB_CH1]);			m_listFDCItem.AddTail(item);
  	item = CFDCItem(_T("UTIL"),				_T("GMS_2"),							FDC_DATA_STRING,	&pDeviceGMS1->m_LastGMSValue[GMS_SUB_CH2]);			m_listFDCItem.AddTail(item);
  	item = CFDCItem(_T("UTIL"),				_T("GMS_3"),							FDC_DATA_STRING,	&pDeviceGMS1->m_LastGMSValue[GMS_SUB_CH3]);			m_listFDCItem.AddTail(item);
  	item = CFDCItem(_T("UTIL"),				_T("PGMS_1"),							FDC_DATA_STRING,	&pDeviceGMS1->m_LastPGMS);							m_listFDCItem.AddTail(item);
 
 	item = CFDCItem(_T("UTIL"),				_T("UTIL_BOX_TEMP"),					FDC_DATA_STRING,	&m_strUtilBox_Temp);								m_listFDCItem.AddTail(item);
 	item = CFDCItem(_T("UTIL"),				_T("ETC_BOX_TEMP"),						FDC_DATA_STRING,	&m_strEtcBox_Temp);									m_listFDCItem.AddTail(item);
 	item = CFDCItem(_T("UTIL"),				_T("SERVO_BOX_TEMP"),					FDC_DATA_STRING,	&m_strServoBox_Temp);								m_listFDCItem.AddTail(item);
 	item = CFDCItem(_T("UTIL"),				_T("CTRL_PC_BOX_TEMP"),					FDC_DATA_STRING,	&m_strCtrlPCBox_Temp);								m_listFDCItem.AddTail(item);
 	item = CFDCItem(_T("UTIL"),				_T("PG_PC_BOX_TEMP"),					FDC_DATA_STRING,	&m_strPGPcBox_Temp);								m_listFDCItem.AddTail(item);
 
	item = CFDCItem(_T("UTIL"),				_T("OP_PC_FIRE_ALARM"),					FDC_DATA_STRING,	&m_strOpPcFireAlarm);								m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("UTIL"),				_T("ALIGN_PG_PC_FIRE_ALARM"),			FDC_DATA_STRING,	&m_strAlignPgPCFireAlarm);							m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("UTIL"),				_T("UTIL_BOX_FIRE_ALARM"),				FDC_DATA_STRING,	&m_strUtilBoxFireAlarm);							m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("UTIL"),				_T("ETC_BOX_FIRE_ALARM"),				FDC_DATA_STRING,	&m_strETCBoxFireAlarm);								m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("UTIL"),				_T("SERVO_BOX_FIRE_ALARM"),				FDC_DATA_STRING,	&m_strServoBoxFireAlarm);							m_listFDCItem.AddTail(item);

	item = CFDCItem(_T("UTIL"),				_T("FRONT_DOOR1_OPEN"),					FDC_DATA_STRING,		&m_nFrontDoor1Open);							m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("UTIL"),				_T("FRONT_DOOR2_OPEN"),					FDC_DATA_STRING,		&m_nFrontDoor2Open);								m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("UTIL"),				_T("BACK_DOOR1_OPEN"),					FDC_DATA_STRING,		&m_nBackDoor1Open);							m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("UTIL"),				_T("BACK_DOOR2_OPEN"),					FDC_DATA_STRING,		&m_nBackDoor2Open);								m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("UTIL"),				_T("LEFT_DOOR_OPEN"),					FDC_DATA_STRING,		&m_nLeftDoorOpen);							m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("UTIL"),				_T("RIGHT_DOOR_OPEN"),					FDC_DATA_STRING,		&m_nRightDoorOpen);								m_listFDCItem.AddTail(item);
 
	item = CFDCItem(_T("SEM"),				_T("THREE_PHASE_POWER2"),				FDC_DATA_STRING,	&m_strbWattNet);									m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("SEM"),				_T("INSTANTANEOUS_POWER2"),				FDC_DATA_STRING,	&m_strbWattTotal);									m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("SEM"),				_T("R_PHASE_VOLTAGE2"),					FDC_DATA_STRING,	&m_strbVoltage_R);									m_listFDCItem.AddTail(item);
	item = CFDCItem(_T("SEM"),				_T("R_PHASE_CURRENT2"),					FDC_DATA_STRING,	&m_strbCurrent_R);									m_listFDCItem.AddTail(item);
}

void CFDCBank::DeleteFDC_SV_par()
{
	//////////////////////////////////////////////////////////////////////////
	// 항목이 변경될 때 마다 지우기 귀찮으니 프로그램 재시작 할 때마다 지워버리자 
	CString strPath;
	strPath.Format(_T("%s\\FDC_SV.par"), theConfigBank.m_System.m_strFDCPath);

	if(CFileSupport::FileCheck(strPath))
		DeleteFile(strPath);
	//////////////////////////////////////////////////////////////////////////
}

void CFDCBank::SaveFDC_SV_par()
{
	if(CFileSupport::DirectoryCheck(theConfigBank.m_System.m_strFDCPath) == FALSE)
	{
		if(CFileSupport::CreateDirectory(theConfigBank.m_System.m_strFDCPath) == FALSE)
			return;
	}

	CString strPath;
	strPath.Format(_T("%s\\FDC_SV.par"), theConfigBank.m_System.m_strFDCPath);

	CFDCItem item;
	POSITION pos = m_listFDCItem.GetHeadPosition();
	while (pos)
	{
		item = m_listFDCItem.GetNext(pos);
		item.WriteFile(strPath);
	}
}

void CFDCBank::CheckLoadADBoardValue()
{
	// 값을 읽다가 잘못된 값이 나오면 바로 보고하지 않고 몇초간 유예기간 뒀다가 그래도 복구 안되면
	// 그때 가서 보고한다. 순간적인 깜빡임 등으로 설비 인터락 걸리는 걸 방지하는 편법... [3/25/2017 OSC]

	if(theSerialInterFace.m_bADBoardReadable == FALSE)
	{
		// 프로그램 첫 시작이면 값이 다 들어올 때 까지 가짜갑으로 메꿈
		//kjpark 20170929 FDC 실제값, 보정값, 보고값 구분
		//m_strLoadMainAir = _T("50.1,50.1,50.1");
		//return;
	}

	//////////////////////////////////////////////////////////////////////////
	// Load Main Air
	//kjpark2017/09/01 Serial 정리 - Temp 추가, 라벨 2 삭제, 토크 삭제, 메인에어
	if(theSerialInterFace.GetVacuumMainHandler()->m_nReadSlot != 3)
		return;

	//kjpark 20170929 FDC 실제값, 보정값, 보고값 구분
	CString strTemp;
	CTokenizer t(m_strLoadMainAir);
	strTemp = t.GetNextToken(_T(","));
	if(_ttof(strTemp) < FDC_AIR_LIMIT_VALUE)
	{
		if(m_stateMainAir == FDC_NORMAL)
		{
			m_stateMainAir = FDC_WARNING;
			m_timerMainAirAlarm.Start();
			theLog[LOG_FDC].AddBuf(_T("Load Main Air Warning : %s"), m_strLoadMainAir);
		}

		if(m_stateMainAir == FDC_WARNING)
		{
			if(m_timerMainAirAlarm.Stop(FALSE) <= theConfigBank.m_Option.m_dFDCCheckTime)
			{
				// 지정된 시간 되기 전 까지는 다시 회복되길 바라며 가짜 값 보고
				m_strLoadMainAir = _T("50.1,50.1,50.1");
			}
			else
			{
				// 지정된 시간 지나도 계속 발생하면 그대로 보고
				m_stateMainAir = FDC_ALARM;
				theLog[LOG_FDC].AddBuf(_T("Load Main Air Alarm : %s"), m_strLoadMainAir);
			}
		}

		if(m_stateMainAir == FDC_ALARM)
		{
			// Air 센서가 정상이면 가상 값으로 보고
// 			if(theDeviceIO.ReadInBit(X_MAIN_AIR_CHK))
// 				m_dLoadMainAir = 50.1;

			//너무 알람 많이 나서 당분간 설비성 알람으로 대체
			//m_strLoadMainAir = _T("50.1,50.1,50.1");
			if( (theProcBank.MachineIsRunState())
				&& theConfigBank.m_Option.m_bFDCAlarm )
			{
// 				theProcBank.AlarmHappen(ALM_LOAD_MAIN_AIR_FDC_ALARM);
			}
		}
	}
	else if(m_stateMainAir != FDC_NORMAL)
	{
		m_stateMainAir = FDC_NORMAL;
		theLog[LOG_FDC].AddBuf(_T("Load Main Air Normal : %s\t%f"), m_strLoadMainAir, m_timerMainAirAlarm.Stop(FALSE));
	}


}

void CFDCBank::CheckGMSValue()
{
	//////////////////////////////////////////////////////////////////////////
	// GMS
	//kjpark 20170929 FDC 실제값, 보정값, 보고값 구분
	CString strTemp;
	double dValue;
	for(int i = 0; i < GMS_SUB_CH5; i++)
	{
		strTemp = theSerialInterFace.GetDeviceGMSHandler1()->m_LastGMSValue[i];
		CTokenizer t(strTemp);
		strTemp = t.GetNextToken(_T(","));
		dValue = _tstof(strTemp);
		if(dValue >= FDC_GMS_LIMIT_VALUE)
		{
			if(m_stateGMS1[i] == FDC_NORMAL)
			{
				m_stateGMS1[i] = FDC_WARNING;
				m_timerGMSAlarm1[i].Start();
				theLog[LOG_FDC].AddBuf(_T("GMS_%d Warning : %s"), i+1, strTemp);
			}

			if(m_stateGMS1[i] == FDC_WARNING)
			{
				if(m_timerGMSAlarm1[i].Stop(FALSE) <= theConfigBank.m_Option.m_dFDCCheckTime)
				{
					// 지정된 시간 되기 전 까지는 다시 회복되길 바라며 가짜 값 보고
					theSerialInterFace.GetDeviceGMSHandler1()->m_LastGMSValue[i].Format(_T("0.501000,0.501000,0.501000"));
				}
				else
				{
					// 지정된 시간 지나도 계속 발생하면 그대로 보고
					m_stateGMS1[i] = FDC_ALARM;
					theLog[LOG_FDC].AddBuf(_T("GMS_%d Alarm : %s"), i+1, strTemp);
				}
			}


			if(m_stateGMS1[i] == FDC_ALARM)
			{
// 				//너무 알람 많이 나서 당분간 설비성 알람으로 대체
// 				theSerialInterFace.GetDeviceGMSHandler1()->m_LastGMSValue[i].Format(_T("0.501000,0.501000,0.501000"));
				if( (theProcBank.MachineIsRunState())
					&& theConfigBank.m_Option.m_bFDCAlarm )
				{
// 					theProcBank.AlarmHappen((ALARM_ID)(ALM_INSP_GMS1_FDC_ALARM + i));
				}
			}

		}
		else if(m_stateGMS1[i] != FDC_NORMAL)
		{
			m_stateGMS1[i] = FDC_NORMAL;
			theLog[LOG_FDC].AddBuf(_T("GMS_%d Normal : %s\t%f"), i+1, strTemp, m_timerGMSAlarm1[i].Stop(FALSE));
		}
	}

	for(int i = 0; i < GMS_SUB_CH5; i++)
	{
		strTemp = theSerialInterFace.GetDeviceGMSHandler2()->m_LastGMSValue[i];
		CTokenizer t(strTemp);
		strTemp = t.GetNextToken(_T(","));
		dValue = _tstof(strTemp);
		if(dValue >= FDC_GMS_LIMIT_VALUE)
		{
			if(m_stateGMS2[i] == FDC_NORMAL)
			{
				m_stateGMS2[i] = FDC_WARNING;
				m_timerGMSAlarm2[i].Start();
				theLog[LOG_FDC].AddBuf(_T("GMS_%d Warning : %s"), i+1, strTemp);
			}

			if(m_stateGMS2[i] == FDC_WARNING)
			{
				if(m_timerGMSAlarm2[i].Stop(FALSE) <= theConfigBank.m_Option.m_dFDCCheckTime)
				{
					// 지정된 시간 되기 전 까지는 다시 회복되길 바라며 가짜 값 보고
					theSerialInterFace.GetDeviceGMSHandler2()->m_LastGMSValue[i].Format(_T("0.501000,0.501000,0.501000"));
				}
				else
				{
					// 지정된 시간 지나도 계속 발생하면 그대로 보고
					m_stateGMS2[i] = FDC_ALARM;
					theLog[LOG_FDC].AddBuf(_T("GMS_%d Alarm : %s"), i+1, strTemp);
				}
			}


			if(m_stateGMS2[i] == FDC_ALARM)
			{
				//너무 알람 많이 나서 당분간 설비성 알람으로 대체
				theSerialInterFace.GetDeviceGMSHandler2()->m_LastGMSValue[i].Format(_T("0.501000,0.501000,0.501000"));
				if( (theProcBank.MachineIsRunState())
					&& theConfigBank.m_Option.m_bFDCAlarm )
				{
					// 					theProcBank.AlarmHappen((ALARM_ID)(ALM_INSP_GMS1_FDC_ALARM + i));
				}
			}

		}
		else if(m_stateGMS2[i] != FDC_NORMAL)
		{
			m_stateGMS2[i] = FDC_NORMAL;
			theLog[LOG_FDC].AddBuf(_T("GMS_%d Normal : %s\t%f"), i+1, strTemp, m_timerGMSAlarm2[i].Stop(FALSE));
		}
	}
	//////////////////////////////////////////////////////////////////////////
}

void CFDCBank::CheckPGMSValue()
{
	//////////////////////////////////////////////////////////////////////////
	// PGMS
	//kjpark 20170929 FDC 실제값, 보정값, 보고값 구분
	CString strTemp;
	strTemp = theSerialInterFace.GetDeviceGMSHandler1()->m_LastPGMS;
	CTokenizer t(strTemp);
	strTemp = t.GetNextToken(_T(","));
	if(strTemp.Left(2) == FDC_PGMS_LIMIT_VALUE)
	{
		if(m_statePGMS == FDC_NORMAL)
		{
			m_statePGMS = FDC_WARNING;
			m_timerPGMSAlarm.Start();
			theLog[LOG_FDC].AddBuf(_T("PGMS_1 Warning : %s"), strTemp);
		}

		if(m_statePGMS == FDC_WARNING)
		{
			if(m_timerPGMSAlarm.Stop(FALSE) <= theConfigBank.m_Option.m_dFDCCheckTime)
			{
				// 지정된 시간 되기 전 까지는 다시 회복되길 바라며 가짜 값 보고
				theSerialInterFace.GetDeviceGMSHandler1()->m_LastPGMS.Format(_T("1,1,1"));
			}
			else
			{
				// 지정된 시간 지나도 계속 발생하면 그대로 보고
				m_statePGMS = FDC_ALARM;
				theLog[LOG_FDC].AddBuf(_T("PGMS_1 Alarm : %s"), strTemp);
			}
		}


		if(m_statePGMS == FDC_ALARM)
		{
			//너무 알람 많이 나서 당분간 설비성 알람으로 대체
			theSerialInterFace.GetDeviceGMSHandler1()->m_LastPGMS.Format(_T("1,1,1"));
			if( (theProcBank.MachineIsRunState())
				&& theConfigBank.m_Option.m_bFDCAlarm )
			{
// 				theProcBank.AlarmHappen(ALM_INSP_B_ZONE_PGMS_FDC_ALARM);
			}
		}
	}
	else if(m_statePGMS != FDC_NORMAL)
	{
		m_statePGMS = FDC_NORMAL;
		theLog[LOG_FDC].AddBuf(_T("PGMS_1 Normal : %s\t%f"), strTemp, m_timerPGMSAlarm.Stop(FALSE));
	}

	//////////////////////////////////////////////////////////////////////////
}
