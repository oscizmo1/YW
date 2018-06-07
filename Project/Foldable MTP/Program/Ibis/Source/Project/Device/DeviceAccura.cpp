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
	// ù��° ���·���, GPS
	m_accClients.AddDevice( IP_ADDR_UPS );

// 	// �ι�° ���·���, UPS
// 	m_accClients.AddDevice( IP_ADDR_GPS );

	m_pClientsArray = m_accClients.GetDevicesAsAllocatedArray( m_nClientCount );		// Client ������ �޾Ƽ� �����´�.

	m_pClientsArray[ ACCURA_CLIENT_UPS ]->SetConnectionCheckTime(1);
	m_pClientsArray[ ACCURA_CLIENT_UPS ]->SetTimeout(1);
// 	m_pClientsArray[ ACCURA_CLIENT_GPS ]->SetConnectionCheckTime(1);
// 	m_pClientsArray[ ACCURA_CLIENT_GPS ]->SetTimeout(1);

	m_1PhaseType[ACCURA_CLIENT_UPS] = ACCURA_1PHASE_T;	// 2ȣ�Ⱑ T�� ���´�
// 	m_1PhaseType[ACCURA_CLIENT_GPS] = ACCURA_1PHASE_R;	// 3���̸� R�� ����

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
	int nSubIdx = 0;				// �� Client�� ���� SubUnit(2350 ���)�� Index, ��� 0 Ȥ�� 1�� ����ϴµ�, �츮�� 0���� ����ϰ� �Ѵ�. 1�� 2300 ��ü

	CString sMsg;

	// Selected Measured data index
	// ���õ� ���� ������ �ε���, Index selection�� ���� ������ �ε����� ��ȿ�� ���, �� �������Ϳ� ���õ� �ε������� ��Ÿ����
	// ��ȿ���� ���� ��� FFFF�� ������.
	// �� �������͸� ������ ���õ� ���� �����͸� ������ �´�.
	BOOL bRet = pAccuraClient->ReadHoldingRegisters(11044, 42, fetchAndValidity);
	if ( bRet == TRUE )
	{
		if ( *ptValidity == 1 ) 
		{
			// PT Measured Data : Data Details ������ PT Measured Data ����
			// Aggregation Selection�� 0 ~ 15, �Ǵ� 200�� ��쿡�� ����
			//
			// ǥ��â�� ǥ�õǴ� �⺻ ���� �����͸� �����´�.
			bRet = pAccuraClient->ReadHoldingRegisters(11101, 56, ptData);		// ������ BYTE���� ���� 56�� �ۿ� ����.
			if ( bRet == TRUE )
			{
				m_rDataBlock[client].nState = 1;	

				if ( m_1PhaseType[client] == ACCURA_1PHASE_T )						// T���� ����Ѵ�.
				{
					m_rDataBlock[client].dbVoltage_R	= ToFloat(ptData, 12);		// C�� (T��) ���� ����
					m_rDataBlock[client].dbVoltage_S	= ToFloat(ptData, 10);		// B�� (S��) ���� ����
					m_rDataBlock[client].dbVoltage_T	= ToFloat(ptData, 8);		// A�� (R��) ���� ����
				}
				else if ( m_1PhaseType[client] == ACCURA_1PHASE_S )					// S�� �� ����ϴ°� ?
				{
					m_rDataBlock[client].dbVoltage_R	= ToFloat(ptData, 10);		// B�� (S��) ���� ����
					m_rDataBlock[client].dbVoltage_S	= ToFloat(ptData, 8);		// A�� (R��) ���� ����
					m_rDataBlock[client].dbVoltage_T	= ToFloat(ptData, 12);		// C�� (T��) ���� ����
				}
				else			// �� �ۿ��� ������� R���� �״�� ����Ѵ�.
				{
					m_rDataBlock[client].dbVoltage_R	= ToFloat(ptData, 8);		// A�� (R��) ���� ����
					m_rDataBlock[client].dbVoltage_S	= ToFloat(ptData, 10);		// B�� (S��) ���� ����
					m_rDataBlock[client].dbVoltage_T	= ToFloat(ptData, 12);		// C�� (T��) ���� ����
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

		// ���� Accura 2350 unit�� �������� �д� ��ġ�� �޸��Ͽ� ó���Ѵ�.
		// 0�̸� 3��, 1�̸� �ܻ�
		// nSubIdx�� ���� 0���� ����
		if ( subunitValidity[ nSubIdx ] == 0)		// 3�� �ΰ� ?
		{
			// Sub Unit�� Measured Data�� �о�´�.
			// �ּҴ� 11201 ���� Unit 0�� �ּҰ� ���۵Ǹ�, �� Unit ���� 150 Bytes ������ �����Ѵ�.
			bRet = pAccuraClient->ReadHoldingRegisters((11201 + (nSubIdx * 150)), 141, subunitData);
			if ( bRet == TRUE )		// ���������� �о��ٸ�,
			{
				m_rDataBlock[client].nState = 1;											// ��� ����

				m_rDataBlock[client].dbCurrent_R	= ToFloat(subunitData, 0);	// A�� (R��) ����
				m_rDataBlock[client].dbCurrent_S	= ToFloat(subunitData, 2);	// B�� (S��) ����
				m_rDataBlock[client].dbCurrent_T	= ToFloat(subunitData, 4);	// C�� (T��) ����

				m_rDataBlock[client].dbWatt_R	= ToFloat(subunitData, 60);	// R�� ���·�
				m_rDataBlock[client].dbWatt_S	= ToFloat(subunitData, 62);	// S�� ���·�
				m_rDataBlock[client].dbWatt_T	= ToFloat(subunitData, 64);	// T�� ���·�

				m_rDataBlock[client].dbWattTotal	= ToFloat(subunitData, 66);	// ���� ���·�
				m_rDataBlock[client].dbWattNet	= (double)ToInt32(subunitData, 92);	// ���� ���·�

				sMsg.AppendFormat(_T(", SubID:%d Ia=%.3f, Ib=%.3f, Ic=%.3f, Pa=%.3f, Pb=%.3f, Pc=%.3f, PTot=%.3f, PNet=%.3f")
					, nSubIdx
					, m_rDataBlock[client].dbCurrent_R, m_rDataBlock[client].dbCurrent_S, m_rDataBlock[client].dbCurrent_T
					, m_rDataBlock[client].dbWatt_R, m_rDataBlock[client].dbWatt_S, m_rDataBlock[client].dbWatt_T
					, m_rDataBlock[client].dbWattTotal, m_rDataBlock[client].dbWattNet );
			}
			else
				sMsg.AppendFormat(_T(", * SubID:%d Detail measured data read Fail"), nSubIdx);
		}
		else if (subunitValidity[ nSubIdx ] == 1) // 1P : �ܻ��ΰ� ?
		{
			bRet = pAccuraClient->ReadHoldingRegisters((11201 + (nSubIdx * 150)), 56, subunitData);

			if ( bRet == TRUE )		// ���������� �о��ٸ�,
			{

				m_rDataBlock[client].dbCurrent_R	= ToFloat(subunitData, 0);	// A�� (R��) ����
				m_rDataBlock[client].dbWatt_R	= ToFloat(subunitData, 16);	// A�� (R��) ���·�

				m_rDataBlock[client].dbWattTotal	= ToFloat(subunitData, 16);	// ���� ���·�, ��ȿ����
				m_rDataBlock[client].dbWattNet	= (double)ToInt32(subunitData, 30);	// ���� ���·�

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
		}//of if �ܻ��ΰ� ?
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

