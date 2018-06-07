#include "StdAfx.h"
#include "DeviceAccura.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDeviceAccura::CDeviceAccura(void) :  m_accClients(*AccuraClients::Create())
{
}


CDeviceAccura::~CDeviceAccura(void)
{
	for (int i = 0; i< m_accClients.Count(); i++)
	{
		m_accClients.RemoveDevice(i);
	}
	delete &m_accClients;
}

void CDeviceAccura::InitAccura()
{
	// 첫번째 전력량계, GPS
	m_accClients.AddDevice( IP_ADDR_UPS );

// 	// 두번째 전력량계, UPS
// 	m_accClients.AddDevice( IP_ADDR_GPS );

	m_pClientsArray = m_accClients.GetDevicesAsAllocatedArray( m_nClientCount );		// Client 수량을 받아서 가져온다.

	m_pClientsArray[ ACCURA_CLIENT_UPS ]->SetConnectionCheckTime(1);
	m_pClientsArray[ ACCURA_CLIENT_UPS ]->SetTimeout(1);
// 	m_pClientsArray[ ACCURA_CLIENT_GPS ]->SetConnectionCheckTime(1);
// 	m_pClientsArray[ ACCURA_CLIENT_GPS ]->SetTimeout(1);

	m_1PhaseType[ACCURA_CLIENT_UPS] = ACCURA_1PHASE_T;	// 2호기가 T에 들어온다
// 	m_1PhaseType[ACCURA_CLIENT_GPS] = ACCURA_1PHASE_R;	// 3상이면 R로 해줌

}

void CDeviceAccura::ReadValue(ACCURA_CLIENT client)
{
	AccuraModbusTcpClient* pAccuraClient = NULL;

	pAccuraClient = m_pClientsArray[ client ];		

	unsigned short fetchAndValidity[42];
	unsigned short* selectedIndex		= &fetchAndValidity[0];
	unsigned short* ptValidity			= &fetchAndValidity[1];
	unsigned short* subunitValidity		= &fetchAndValidity[2];
	unsigned short ptData[100];	// 56];
	unsigned short subunitData[150];
	int nSubIdx = 0;				// 각 Client에 속한 SubUnit(2350 모듈)의 Index, 통상 0 혹은 1을 사용하는데, 우리는 0으로 사용하고 한다. 1은 2300 본체

	CString sMsg;

	// Selected Measured data index
	// 선택된 계측 데이터 인덱스, Index selection의 계측 데이터 인덱스가 유효할 경우, 이 레지스터에 선택된 인덱스값이 나타나며
	// 유효하지 않을 경우 FFFF로 읽힌다.
	// 이 레지스터를 읽으면 선택된 계측 데이터를 가지고 온다.
	BOOL bRet = pAccuraClient->ReadHoldingRegisters(11044, 42, fetchAndValidity);
	if ( bRet == TRUE )
	{
		if ( *ptValidity == 1 ) 
		{
			// PT Measured Data : Data Details 섹션의 PT Measured Data 참조
			// Aggregation Selection이 0 ~ 15, 또는 200일 경우에만 사용됨
			//
			// 표시창에 표시되는 기본 전압 데이터를 가져온다.
			bRet = pAccuraClient->ReadHoldingRegisters(11101, 56, ptData);		// 데이터 BYTE수는 현재 56개 밖에 없다.
			if ( bRet == TRUE )
			{
				m_rDataBlock[client].nState = 1;	

				if ( m_1PhaseType[client] == ACCURA_1PHASE_T )						// T상을 사용한다.
				{
					m_rDataBlock[client].dbVoltage_R	= ToFloat(ptData, 12);		// C상 (T상) 라인 전압
					m_rDataBlock[client].dbVoltage_S	= ToFloat(ptData, 10);		// B상 (S상) 라인 전압
					m_rDataBlock[client].dbVoltage_T	= ToFloat(ptData, 8);		// A상 (R상) 라인 전압
				}
				else if ( m_1PhaseType[client] == ACCURA_1PHASE_S )					// S상 을 사용하는가 ?
				{
					m_rDataBlock[client].dbVoltage_R	= ToFloat(ptData, 10);		// B상 (S상) 라인 전압
					m_rDataBlock[client].dbVoltage_S	= ToFloat(ptData, 8);		// A상 (R상) 라인 전압
					m_rDataBlock[client].dbVoltage_T	= ToFloat(ptData, 12);		// C상 (T상) 라인 전압
				}
				else			// 그 밖에는 원래대로 R상을 그대로 사용한다.
				{
					m_rDataBlock[client].dbVoltage_R	= ToFloat(ptData, 8);		// A상 (R상) 라인 전압
					m_rDataBlock[client].dbVoltage_S	= ToFloat(ptData, 10);		// B상 (S상) 라인 전압
					m_rDataBlock[client].dbVoltage_T	= ToFloat(ptData, 12);		// C상 (T상) 라인 전압
				}
				sMsg.Format(_T("Client:%d Ret:%d, Va=%.3f, Vb=%.3f, Vc=%.3f")
					, client
					, bRet
					, m_rDataBlock[client].dbVoltage_R, m_rDataBlock[client].dbVoltage_S, m_rDataBlock[client].dbVoltage_T );
			}
			else
				sMsg.Format(_T("* Client:%d PT Measure data read Fail "), client);
		}
		else
			sMsg.Format(_T("* Client:%d State is Invalidity "), client);

		// 하위 Accura 2350 unit의 종류별로 읽는 위치를 달리하여 처리한다.
		// 0이면 3상, 1이면 단상
		// nSubIdx는 현재 0으로 고정
		if ( subunitValidity[ nSubIdx ] == 0)		// 3상 인가 ?
		{
			// Sub Unit의 Measured Data를 읽어온다.
			// 주소는 11201 부터 Unit 0의 주소가 시작되며, 각 Unit 별로 150 Bytes 단위로 증가한다.
			bRet = pAccuraClient->ReadHoldingRegisters((11201 + (nSubIdx * 150)), 141, subunitData);
			if ( bRet == TRUE )		// 성공적으로 읽었다면,
			{
				m_rDataBlock[client].nState = 1;											// 통신 정상

				m_rDataBlock[client].dbCurrent_R	= ToFloat(subunitData, 0);	// A상 (R상) 전류
				m_rDataBlock[client].dbCurrent_S	= ToFloat(subunitData, 2);	// B상 (S상) 전류
				m_rDataBlock[client].dbCurrent_T	= ToFloat(subunitData, 4);	// C상 (T상) 전류

				m_rDataBlock[client].dbWatt_R	= ToFloat(subunitData, 60);	// R상 전력량
				m_rDataBlock[client].dbWatt_S	= ToFloat(subunitData, 62);	// S상 전력량
				m_rDataBlock[client].dbWatt_T	= ToFloat(subunitData, 64);	// T상 전력량

				m_rDataBlock[client].dbWattTotal	= ToFloat(subunitData, 66);	// 순시 전력량
				m_rDataBlock[client].dbWattNet	= (double)ToInt32(subunitData, 92);	// 적산 전력량

				sMsg.AppendFormat(_T(", SubID:%d Ia=%.3f, Ib=%.3f, Ic=%.3f, Pa=%.3f, Pb=%.3f, Pc=%.3f, PTot=%.3f, PNet=%.3f")
					, nSubIdx
					, m_rDataBlock[client].dbCurrent_R, m_rDataBlock[client].dbCurrent_S, m_rDataBlock[client].dbCurrent_T
					, m_rDataBlock[client].dbWatt_R, m_rDataBlock[client].dbWatt_S, m_rDataBlock[client].dbWatt_T
					, m_rDataBlock[client].dbWattTotal, m_rDataBlock[client].dbWattNet );
			}
			else
				sMsg.AppendFormat(_T(", * SubID:%d Detail measured data read Fail"), nSubIdx);
		}
		else if (subunitValidity[ nSubIdx ] == 1) // 1P : 단상인가 ?
		{
			bRet = pAccuraClient->ReadHoldingRegisters((11201 + (nSubIdx * 150)), 56, subunitData);

			if ( bRet == TRUE )		// 성공적으로 읽었다면,
			{

				m_rDataBlock[client].dbCurrent_R	= ToFloat(subunitData, 0);	// A상 (R상) 전류
				m_rDataBlock[client].dbWatt_R	= ToFloat(subunitData, 16);	// A상 (R상) 전력량

				m_rDataBlock[client].dbWattTotal	= ToFloat(subunitData, 16);	// 순시 전력량, 유효전력
				m_rDataBlock[client].dbWattNet	= (double)ToInt32(subunitData, 30);	// 적산 전력량

				sMsg.AppendFormat(_T(", SubID:%d I=%.3f, P=%.3f, PTot=%.3f, PNet=%.3f")
					, nSubIdx
					, m_rDataBlock[client].dbCurrent_R 
					, m_rDataBlock[client].dbWatt_R
					, m_rDataBlock[client].dbWattTotal, m_rDataBlock[client].dbWattNet );

				//m_debug.Log(_T("1 [0] %x, %x, [16] %x, %x, [30] %x, %x"),
				//	ptData[0], ptData[1], ptData[16], ptData[17], ptData[30], ptData[31]);

			}
			else
				sMsg.AppendFormat(_T(", * SubID:%d Detail measured data read Fail"), nSubIdx);
		}//of if 단상인가 ?
		else
			sMsg.Format(_T("* Client:%d Validity State read Fail "), client);

	}
// 	theLog[LOG_ACCURA].AddBuf(sMsg);
}

float CDeviceAccura::ToFloat(unsigned short* buffer, int index)
{
	unsigned short reversed[2];

	reversed[0] = buffer[index + 1];
	reversed[1] = buffer[index + 0];

	return *(float*)reversed;
}

int CDeviceAccura::ToInt32(unsigned short* buffer, int index)
{
	unsigned short reversed[2];

	reversed[0] = buffer[index + 1];
	reversed[1] = buffer[index + 0];

	return *(int*)reversed;
}

