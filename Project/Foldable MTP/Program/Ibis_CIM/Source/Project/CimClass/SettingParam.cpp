#include "stdafx.h"
#include "../GaonNuri.h"
#include "SettingParam.h"

#pragma warning(disable : 4996)


char CSettingParm::cMODEL_PATH[STR_DEF_LEN]					= {0};	// �PC�� Model ���� ���� ���
char CSettingParm::cEQPCONFIG_PATH[STR_DEF_LEN]				= {0};	// �PC�� Config ���
char CSettingParm::cEQID[STR_DEF_LEN]						= {0};	// B1BND15N_A21 
char CSettingParm::cEQNAME[STR_DEF_LEN]						= {0};	// AMT
char CSettingParm::cXComIP[STR_DEF_LEN]						= {0};	// XCom IP
char CSettingParm::cFDC_SV_PATH[STR_DEF_LEN]				= {0};	// �PC�� FDC ��� ���� ������
char CSettingParm::cCELL_LOG_PATH[STR_DEF_LEN]				= {0};	// DATAPC���� ������ �� �ֵ��� ������ �����ϴ� ���� ���

int CSettingParm::iACTUATOR_EXTENT_LOW						=0;		// FDC List ǥ�� ���� �ּ� (���� ��������)
int CSettingParm::iACTUATOR_EXTENT_UP						=0;		// FDC List ǥ�� ���� �ִ� (���� ��������)

int CSettingParm::iUtil_EXTENT_LOW							=0;		// FDC List ǥ�� ���� �ּ� (���� ��������)
int CSettingParm::iUtil_EXTENT_UP							=0;		// FDC List ǥ�� ���� �ִ� (���� ��������)

char CSettingParm::cECID_ECV_PATH[STR_DEF_LEN]				= {0};	// ECIM �׸� �������
char CSettingParm::cECID_PATH[STR_DEF_LEN]					= {0};	// ECIM �׸� ���� ���� �� �������

char CSettingParm::cHsmsConfig[STR_DEF_LEN]					= {0};	// XCOM ���� �������� ���� �׸� ������� "cfgXComAMT_MIT.cfg"

char CSettingParm::cQMS_DATA_PATH[STR_DEF_LEN]				= {0};	// QMS DATA PATH (S:W)
char CSettingParm::cQMS_IMG_PATH[STR_DEF_LEN]				= {0};	// QMS IMG PATH (T:\)
char CSettingParm::cQMS_DUMMY_PATH[STR_DEF_LEN]				= {0};	// QMS DUMMY PATH (U:\)
char CSettingParm::cPG_RESULT_PATH[STR_DEF_LEN]				= {0};	// ��ȣ�� ��� ������� (X:\)
char CSettingParm::cVT1_INFO_PATH[STR_DEF_LEN]				= {0};	// Inform.ini ������  (Z:\inform.ini)
char CSettingParm::cVT1_RESULT_PATH[STR_DEF_LEN]			= {0};	// �˻�PC1 �˻��� ������� (Z:\)
char CSettingParm::cVT2_RESULT_PATH[STR_DEF_LEN]			= {0};	// �˻�PC2 �˻���� ������� (Y:\)

int CSettingParm::iVT1_EXTENT_LOW							=0;		// �˻�PC 1�� FDC �׸� �ּҰ�
int CSettingParm::iVT1_EXTENT_UP							=0;		// �˻�PC 1�� FDC �׸� �ִ밪
int CSettingParm::iVT2_EXTENT_LOW							=0;		// �˻�PC 2�� FDC �׸� �ּҰ�
int CSettingParm::iVT2_EXTENT_UP							=0;		// �˻�PC 2�� FDC �׸� �ִ밪

char CSettingParm::cSOFT_REV[STR_DEF_LEN]					= {0};	// ��������� SoftVersion 6�ڸ� ����Ÿ S1F1,S1F2
char CSettingParm::cProcessID[STR_DEF_LEN]					= {0};	// ��� Process �̸� ? 

BOOL CSettingParm::bFdcReportUsed							=TRUE;	// FDC �׸� Trace Data ���� ����
char CSettingParm::cRecipePath[STR_DEF_LEN]					= {0};	// �PC Recipe(Model) ���� ���

BOOL CSettingParm::bCellLoadingID							=TRUE;	// ����� Cell Loading�� ID���� CELL ID or INNER ID ���� ���� ( Y: CELLID , N: INNERID)

BOOL CSettingParm::bQmsWriteAmt								=TRUE;	// QMS ��� ����
BOOL CSettingParm::bMtpWUsed								=FALSE; // QMS �� MTPW �������
BOOL CSettingParm::bMtpRUsed								=FALSE;	// QMS �� MTPR �������
BOOL CSettingParm::bTspUsed									=FALSE;	// QMS �� TSP �������

// Machine ����
char CSettingParm::cMaint[STR_DEF_LEN]						= {0};	// Equipment Status  Maint On/Off
char CSettingParm::cAvilability[STR_DEF_LEN]				= {0};	// Equipment Status  Avilability Down/up
char CSettingParm::cInterlock[STR_DEF_LEN]					= {0};	// Equipment Status  Interlock On/Off
char CSettingParm::cMove[STR_DEF_LEN]						= {0};	// Equipment Status  Move Pause/Running
char CSettingParm::cRun[STR_DEF_LEN]						= {0};	// Equipment Status  Run Idle/Run
char CSettingParm::cFront[STR_DEF_LEN]						= {0};	// INILE  Front ���� �������
char CSettingParm::cRear[STR_DEF_LEN]						= {0};	// INLINE Rear ���� �������
char CSettingParm::cSampleLot[STR_DEF_LEN]					= {0};	// Sample Lot state, 1:On, 2:Off

BOOL CSettingParm::bWriteLog								=FALSE;	// �α� �ۼ� ����
BOOL CSettingParm::bWriteTrackOutInfo						=FALSE;	// �α� �ۼ� ����
char CSettingParm::cMachineIP[STR_DEF_LEN]					= {0};	// �PC(Machine) IP
int CSettingParm::iMachinePort								=0;		// �PC(Machine) Port

char CSettingParm::cDES_BASIC_PATH[STR_DEF_LEN]				= {0};	//XCOM S8F1���� ������ S8F2�� ������ �ʿ��� ����Ÿ
char CSettingParm::cDES_HOST_IP[STR_DEF_LEN]				= {0};	//XCOM S8F1���� ������ S8F2�� ������ �ʿ��� ����Ÿ
char CSettingParm::cDES_LOGIN_ID[STR_DEF_LEN]				= {0};	//XCOM S8F1���� ������ S8F2�� ������ �ʿ��� ����Ÿ
char CSettingParm::cDES_LOGIN_PW[STR_DEF_LEN]				= {0};	//XCOM S8F1���� ������ S8F2�� ������ �ʿ��� ����Ÿ
char CSettingParm::cMCC_BASIC_PATH[STR_DEF_LEN]				= {0};	//XCOM S8F1���� ������ S8F2�� ������ �ʿ��� ����Ÿ
int CSettingParm::iMCC_SAMPLING_TIME						= 10;	//XCOM S8F1���� ������ S8F2�� ������ �ʿ��� ����Ÿ
char CSettingParm::cMCC_HOST_IP[STR_DEF_LEN]				= {0};	//XCOM S8F1���� ������ S8F2�� ������ �ʿ��� ����Ÿ
char CSettingParm::cMCC_LOGIN_ID[STR_DEF_LEN]				= {0};	//XCOM S8F1���� ������ S8F2�� ������ �ʿ��� ����Ÿ
char CSettingParm::cMCC_LOGIN_PW[STR_DEF_LEN]				= {0};	//XCOM S8F1���� ������ S8F2�� ������ �ʿ��� ����Ÿ



CSettingParm::CSettingParm()
{
}
CSettingParm::~CSettingParm()
{
}
BOOL CSettingParm::Read(CString strFileName)
{
	CIni ini(strFileName);
	strcpy_s(cMODEL_PATH			, ini.GetString("EQUIP", "MODEL_PATH", "D:\\YWDSP\\Recipe").GetBuffer());								// � Model���� ���
	strcpy_s(cEQPCONFIG_PATH		, ini.GetString("EQUIP", "EQPCONFIG_PATH", "D:\\YWDSP\\Config").GetBuffer());								// �PC�� Config ���
	strcpy_s(cEQID					, ini.GetString("EQUIP", "EQID", "Not").GetBuffer());											// B1BND15N_A21 
	strcpy_s(cEQNAME				, ini.GetString("EQUIP", "EQNAME", "AMT").GetBuffer());											// AMT
	strcpy_s(cFDC_SV_PATH			, ini.GetString("EQUIP", "FDC_SV_PATH", "R:\\FDC\\FDC_SV.par").GetBuffer());			// �PC�� FDC ��� ���� ������
	strcpy_s(cCELL_LOG_PATH			, ini.GetString("EQUIP", "CELL_LOG_PATH", "D:\\D-AMT\\DATA_PC\\CELL").GetBuffer());					// DATAPC���� ������ �� �ֵ��� ������ �����ϴ� ���� ���
	strcpy_s(g_pData->m_cLineID		, ini.GetString("EQUIP", "LINE_ID", "NoLineID").GetBuffer());									// ������ LINE_ID 

	iACTUATOR_EXTENT_LOW			= ini.GetLong("EQUIP", "ACTUATOR_EXTENT_LOW");													// FDC List ǥ�� ���� �ּ� (���� ��������)
	iACTUATOR_EXTENT_UP				= ini.GetLong("EQUIP", "ACTUATOR_EXTENT_UP");													// FDC List ǥ�� ���� �ִ� (���� ��������)

	// 2015-12-23, jhLee, Utility �׸� ��������� ���
	iUtil_EXTENT_LOW			= ini.GetLong("EQUIP", "UTIL_EXTENT_LOW");													// Utility FDC List ǥ�� ���� �ּ� (���� ��������)
	iUtil_EXTENT_UP				= ini.GetLong("EQUIP", "UTIL_EXTENT_UP");													// Utility FDC List ǥ�� ���� �ִ� (���� ��������)

	// sprintf_s(cECID_PATH			, "%s\\ECID.xml", CGaonNuriApp::GetSetupPath());												// ECIM �׸� �������
	sprintf_s(cECID_PATH			, "%s\\ECID.ini", CGaonNuriApp::GetSetupPath());												// ECIM �׸� �������
	strcpy_s(cECID_ECV_PATH			, ini.GetString("EQUIP", "ECV_PATH", "D:\\D-AMT\\Data\\Profile\\Motion\\AxisInfo.ini").GetBuffer());	// ECIM �׸� ���� ���� �� �������

	strcpy_s(cXComIP				, ini.GetString("HSMS", "XCOMIP", "127.0.0.1").GetBuffer());									// XCOM IP

	sprintf_s(cHsmsConfig			,"%s\\cfgXComSDV_AMT.cfg", CGaonNuriApp::GetSetupPath());										// XCOM ���� �������� ���� �׸� ������� "cfgXComAMT_MIT.cfg"

	strcpy_s(cQMS_DATA_PATH			, ini.GetString("EQUIP", "QMS_DATA_PATH", "S:\\").GetBuffer());									// QMS DATA PATH (S:W)
	strcpy_s(cQMS_IMG_PATH			, ini.GetString("EQUIP", "QMS_IMG_PATH", "T:\\").GetBuffer());									// QMS IMG PATH (T:\)
	strcpy_s(cQMS_DUMMY_PATH		, ini.GetString("EQUIP", "QMS_DUMMY_PATH", "U:\\").GetBuffer());								// QMS DUMMY PATH (U:\)

	strcpy_s(cPG_RESULT_PATH		, ini.GetString("EQUIP", "PG_RESULT_PATH", "X:\\").GetBuffer());								// ��ȣ�� ��� ������� (X:\)

	strcpy_s(cVT1_INFO_PATH			, ini.GetString("EQUIP", "VT1_INFO_PATH", "Z:\\Inform\\Inform.ini").GetBuffer());				// Inform.ini ������  (Z:\inform.ini)
	strcpy_s(cVT1_RESULT_PATH		, ini.GetString("EQUIP", "VT1_RESULT_PATH", "Z:\\").GetBuffer());								// �˻�PC1 �˻��� ������� (Z:\)
	strcpy_s(cVT2_RESULT_PATH		, ini.GetString("EQUIP", "VT2_RESULT_PATH", "Y:\\").GetBuffer());								// �˻�PC2 �˻���� ������� (Y:\)	(YoumAMT(2CH)�� ������� ����

	iVT1_EXTENT_LOW					= ini.GetLong("EQUIP", "VT1_EXTENT_LOW");														// �˻�PC 1�� FDC �׸� �ּҰ�
	iVT1_EXTENT_UP					= ini.GetLong("EQUIP", "VT1_EXTENT_UP");														// �˻�PC 1�� FDC �׸� �ִ밪
	iVT2_EXTENT_LOW					= ini.GetLong("EQUIP", "VT2_EXTENT_LOW");														// �˻�PC 2�� FDC �׸� �ּҰ�
	iVT2_EXTENT_UP					= ini.GetLong("EQUIP", "VT2_EXTENT_UP");														// �˻�PC 2�� FDC �׸� �ִ밪

	strcpy_s(cSOFT_REV				, ini.GetString("EQUIP", "SOFTREV", "000001").GetBuffer());										// ��������� SoftVersion 6�ڸ� ����Ÿ S1F1 ,S1F2
	strcpy_s(cProcessID				, ini.GetString("EQUIP", "PROCESSID", "MTP").GetBuffer());										// ��� Process �̸� ? 

	bFdcReportUsed					= ini.GetBoolean("FDC", "FDC_REPORT_USED", TRUE);												// FDC �׸� Trace Data ���� ����

	strcpy_s(cRecipePath			, ini.GetString("OTHERS", "RECIPE_PATH", "D:\\D-AMT\\Data\\Profile\\MCMain.ini").GetBuffer());			// �PC Recipe(Model) ���� ���

	bCellLoadingID					= ini.GetBoolean("INSP", "FILEPATH_CELLID", TRUE);												// ����� Cell Loading�� ID���� CELL ID or INNER ID ���� ���� ( Y: CELLID , N: INNERID)

	bQmsWriteAmt					= ini.GetBoolean("QMS", "QMS_WRITE_USED", TRUE);												// QMS ��� ����
	bMtpWUsed						= ini.GetBoolean("QMS", "MTPW_USED", FALSE);													// QMS �� MTPW �������
	bTspUsed						= ini.GetBoolean("QMS", "TSP_USED", FALSE);														// QMS �� MTPR �������
	bMtpRUsed						= ini.GetBoolean("QMS", "MTPR_USED", FALSE);													// QMS �� TSP �������

	// Machine ���� ����
	strcpy_s(cMaint					, ini.GetString("DOWNEQST", "MAINT", "1").GetBuffer());
	strcpy_s(cAvilability           , ini.GetString("DOWNEQST", "AVILABILITY", "1").GetBuffer());
	strcpy_s(cInterlock             , ini.GetString("DOWNEQST", "INTERLOCK", "1").GetBuffer());
	strcpy_s(cMove					, ini.GetString("DOWNEQST", "MOVE", "1").GetBuffer());
	strcpy_s(cRun					, ini.GetString("DOWNEQST", "RUN", "1").GetBuffer());
	strcpy_s(cFront					, ini.GetString("DOWNEQST", "FRONT", "1").GetBuffer());											// INILE  Front ���� �������
	strcpy_s(cRear					, ini.GetString("DOWNEQST", "REAR", "1").GetBuffer());											// INLINE Rear ���� �������
	strcpy_s(cSampleLot             , ini.GetString("DOWNEQST", "SAMPLE", "1").GetBuffer());

	bWriteLog						= ini.GetBoolean("EQUIP", "WRITE_LOG", FALSE);													// �α� �ۼ� ����
	bWriteTrackOutInfo				= ini.GetBoolean("EQUIP", "WRITE_TRACKOUT_INFO", FALSE);	
	strcpy_s(cMachineIP             , ini.GetString("SOCKET", "IP_MACHINE", "192.1680.10.110").GetBuffer());						// �PC(Machine) IP
	iMachinePort					= ini.GetLong("SOCKET", "PORT_MACHINE");														// �PC(Machine) Port

	strcpy_s(cDES_BASIC_PATH		, ini.GetString("DES", "DES_BASIC_PATH", "/fs1/fab01").GetBuffer());							//XCOM S8F1���� ������ S8F2�� ������ �ʿ��� ����Ÿ
	strcpy_s(cDES_HOST_IP			, ini.GetString("DES", "DES_HOST_IP", "127.0.0.1").GetBuffer());								//XCOM S8F1���� ������ S8F2�� ������ �ʿ��� ����Ÿ
	strcpy_s(cDES_LOGIN_ID			, ini.GetString("DES", "DES_LOGIN_ID", "mccadmin").GetBuffer());								//XCOM S8F1���� ������ S8F2�� ������ �ʿ��� ����Ÿ
	strcpy_s(cDES_LOGIN_PW			, ini.GetString("DES", "DES_LOGIN_PW", "password").GetBuffer());								//XCOM S8F1���� ������ S8F2�� ������ �ʿ��� ����Ÿ
	strcpy_s(cMCC_BASIC_PATH		, ini.GetString("MCC", "MCC_BASIC_PATH", "/fs1/fab01").GetBuffer());							//XCOM S8F1���� ������ S8F2�� ������ �ʿ��� ����Ÿ
	iMCC_SAMPLING_TIME				, ini.GetLong("MCC", "MCC_SAMPLING_TIME", 10);													//XCOM S8F1���� ������ S8F2�� ������ �ʿ��� ����Ÿ
	strcpy_s(cMCC_HOST_IP			, ini.GetString("MCC", "MCC_HOST_IP", "127.0.0.1").GetBuffer());								//XCOM S8F1���� ������ S8F2�� ������ �ʿ��� ����Ÿ
	strcpy_s(cMCC_LOGIN_ID			, ini.GetString("MCC", "MCC_LOGIN_ID", "mccadmin").GetBuffer());								//XCOM S8F1���� ������ S8F2�� ������ �ʿ��� ����Ÿ
	strcpy_s(cMCC_LOGIN_PW			, ini.GetString("MCC", "MCC_LOGIN_PW", "password").GetBuffer());								//XCOM S8F1���� ������ S8F2�� ������ �ʿ��� ����Ÿ



	//////////////////////////////////////////////////////////////////////////
	// 2015-09-08 JH-PARK DELETE

	//	strcpy_s(cALARM_PATH			, ini.GetString("EQUIP", "ALARM_PATH", "\\\\192.168.10.110\\GAON\\Setup\\ErrorListENG.ini").GetBuffer());
	//	strcpy_s(cTEACH_PATH			, ini.GetString("EQUIP", "TEACH_PATH", "\\\\192.168.10.110\\GAON\\Setup\\Teach.par").GetBuffer());
	//	strcpy_s(cDEVICEID				, ini.GetString("EQUIP", "DEVICEID", "1").GetBuffer());
	//	strcpy_s(cEQIP					, ini.GetString("HSMS", "EQIP", "127.0.0.1").GetBuffer());
	//	sprintf_s(cFDC_PATH				, "%s\\FDC.xml", CGaonNuriApp::GetSetupPath());
	//	strcpy_s(cREPAIR_INFO_PATH		, ini.GetString("EQUIP", "REPAIR_INFO_PATH", "\\\\192.168.10.110\\Project\\DATA_PC\\REPAIR").GetBuffer()); 	
	//	strcpy_s(cEQP_SAVE_PATH			, ini.GetString("EQUIP", "EQP_SAVE_PATH", "D:\\BACKUP_DATA").GetBuffer());
	//	strcpy_s(cHOST_SAVE_PATH		, ini.GetString("EQUIP", "HOST_SAVE_PATH", "S:\\").GetBuffer());
	//	strcpy_s(cHOST_SAVE_IMG_PATH		, ini.GetString("EQUIP", "HOST_SAVE_IMG_PATH", "T:\\").GetBuffer());
	//	strcpy_s(cHOST_SAVE_MONITOR_PATH		, ini.GetString("EQUIP", "HOST_SAVE_MONITOR_PATH", "U:\\").GetBuffer());
	//	strcpy_s(cSIGNAL_RESULT_PATH		, ini.GetString("EQUIP", "SIGNAL_RESULT_PATH", "\\\\192.168.10.140\\Result").GetBuffer());
	//	bTprUsed = ini.GetBoolean("EQUIP", "TPR_USED", FALSE);	
	//	strcpy_s(cFDC_GRAPH_LOWER		, ini.GetString("EQUIP", "FDC_GRAPH_RANGE_LOWER", "-100.0").GetBuffer());
	//	strcpy_s(cFDC_GRAPH_UPPER		, ini.GetString("EQUIP", "FDC_GRAPH_RANGE_UPPER", "100.0").GetBuffer());
	//	iSemPort			= ini.GetLong("SEM", "SEM_PORT");
	//	iSemIntervalTime	= ini.GetLong("SEM", "SEM_INTERVAL_TIME");
	//	bInspFileName = ini.GetBoolean("INSP", "FILEPATH_CELLID", TRUE);		//2015.03.17 GT.PARK - (Y=CELL ID, N=INNER ID)
	//	strcpy_s(cTACT_GRAPH_LOWER		, ini.GetString("EQUIP", "TACT_GRAPH_RANGE_LOWER", "0.0").GetBuffer());
	//	strcpy_s(cTACT_GRAPH_UPPER		, ini.GetString("EQUIP", "TACT_GRAPH_RANGE_UPPER", "100.0").GetBuffer());
	//	strcpy_s(cOfflinePassword             , ini.GetString("CONFIG", "OFFLINE_PASSWORD", "147258").GetBuffer());
	//	bLightLog = ini.GetBoolean("EQUIP", "LIGHT_LOG", FALSE);
	//	strcpy_s(cAftIP             , ini.GetString("SOCKET", "IP_AFT", "127.0.0.1").GetBuffer());
	//	iAftPort = ini.GetLong("SOCKET", "PORT_AFT");
	//	iDBCellStorePeriod = ini.GetLong("DBMS", "CELL_STORE_PERIOD", 3);
	//	iDBAlarmStorePeriod = ini.GetLong("DBMS", "ALARM_STORE_PERIOD", 3);
		

	return TRUE;
}
BOOL CSettingParm::Write(CString strFileName)
{
	CIni ini(strFileName);
	ini.WriteString("EQUIP"		, "LINE_ID"		, g_pData->m_cLineID);
	ini.WriteString("DOWNEQST"	, "MAINT"		, g_pData->m_cMaint);
	ini.WriteString("DOWNEQST"	, "AVILABILITY"	, g_pData->m_cAvilability);
	ini.WriteString("DOWNEQST"	, "INTERLOCK"	, g_pData->m_cInterlock);
	ini.WriteString("DOWNEQST"	, "MOVE"		, g_pData->m_cMove);
	ini.WriteString("DOWNEQST"	, "RUN"			, g_pData->m_cRun);
	ini.WriteString("DOWNEQST"	, "FRONT"		, g_pData->m_cFront);
	ini.WriteString("DOWNEQST"	, "REAR"		, g_pData->m_cRear);
	ini.WriteString("DOWNEQST"	, "SAMPLE"		, g_pData->m_cSampleLot);

	ini.WriteBoolean("FDC"		, "FDC_REPORT_USED", CSettingParm::bFdcReportUsed);


	// 2015-09-08 JH-PARK DELETE
	//ini.WriteString("EQUIP", "REPAIR_INFO_PATH", CSettingParm::cREPAIR_INFO_PATH);
	//ini.WriteBoolean("EQUIP", "TPR_USED", CSettingParm::bTprUsed);


	return TRUE;
}

BOOL CSettingParm::Write2(CString strFileName, CString strSection, CString strKey, CString strValue)
{
	CIni ini(strFileName);

	ini.WriteString(strSection, strKey, strValue);

	return TRUE;
}