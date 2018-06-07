#pragma once

#include "Accura/AccuraClient.h"

#define IP_ADDR_UPS	"192.168.10.161"
#define IP_ADDR_GPS	"192.168.10.162"

enum ACCURA_CLIENT
{
	ACCURA_CLIENT_UPS = 0,
// 	ACCURA_CLIENT_GPS,
	ACCURA_CLIENT_MAX
};

// 3���� �ƴ� �ܻ��� ��� ������ R, S, T �� �Ѱ��� �����µ� �װ� 
// ���񺰷� ����ġ �ʴٰ� �ϴ� �� ���� Optionȭ [12/7/2016 OSC]
enum ACCURA_1PHASE_TYPE
{
	ACCURA_1PHASE_R = 0,
	ACCURA_1PHASE_S,
	ACCURA_1PHASE_T,
	ACCURA_1PHASE_MAX
};


typedef struct _TAccuraData 
{
	int	nState;					// ��� ����, ����� �̷������ �ʾ����� 0, �� ���̶� ����� �Ǿ��ٸ� 1 �� �����ȴ�.

	double dbVoltage_R;			// R�� ����
	double dbVoltage_S;			// S�� ����
	double dbVoltage_T;			// T�� ����

	double dbCurrent_R;			// R�� ����
	double dbCurrent_S;			// S�� ����
	double dbCurrent_T;			// T�� ����

	double dbWatt_R;				// R�� ���·�
	double dbWatt_S;				// S�� ���·�
	double dbWatt_T;				// T�� ���·�
	double dbWattTotal;			// ���� ���·�
	double dbWattNet;				// ���� ���·�

	_TAccuraData()
	{
		nState			= 0;		// ����ϱ� �� ������

		dbVoltage_R		= 0.0;	// R�� ����
		dbVoltage_S		= 0.0;	// S�� ����
		dbVoltage_T		= 0.0;	// T�� ����

		dbCurrent_R		= 0.0;	// R�� ����
		dbCurrent_S		= 0.0;	// S�� ����
		dbCurrent_T		= 0.0;	// T�� ����

		dbWatt_R			= 0.0;	// R�� ���·�
		dbWatt_S			= 0.0;	// S�� ���·�
		dbWatt_T			= 0.0;	// T�� ���·�
		dbWattTotal		= 0.0;	// ���� ���·�
		dbWattNet		= 0.0;	// ���� ���·�
	};

} TAccuraData;

class CDeviceAccura
{
public:
	CDeviceAccura(void);
	~CDeviceAccura(void);

	AccuraClients&	m_accClients;
	AccuraModbusTcpClient** m_pClientsArray;
	int m_nClientCount;

	TAccuraData		m_rDataBlock[ACCURA_CLIENT_MAX];	// �����͸� �����ϴ� ����ü ����
	ACCURA_1PHASE_TYPE m_1PhaseType[ACCURA_CLIENT_MAX];

	void InitAccura();

	void ReadValue(ACCURA_CLIENT client);
	float ToFloat(unsigned short* buffer, int index);
	int ToInt32(unsigned short* buffer, int index);
};

