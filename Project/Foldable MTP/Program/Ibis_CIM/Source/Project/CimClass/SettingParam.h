#ifndef _SETTING_PARAM_H_
#define _SETTING_PARAM_H_

class CSettingParm
{
public:
	CSettingParm();
	~CSettingParm();
public:
	static char cMODEL_PATH[STR_DEF_LEN];		// �PC�� Model ���� ���� ���
	static char cEQPCONFIG_PATH[STR_DEF_LEN];	// �PC�� Config ���
	static char cEQID[STR_DEF_LEN];				// B1BND15N_A21 
	static char cEQNAME[STR_DEF_LEN];			// AMT
	static char cXComIP[STR_DEF_LEN];			// XCOM IP
	static char cFDC_SV_PATH[STR_DEF_LEN];		// �PC�� FDC ��� ���� ������
	static char cCELL_LOG_PATH[STR_DEF_LEN];	// DATAPC���� ������ �� �ֵ��� ������ �����ϴ� ���� ���

	static int  iACTUATOR_EXTENT_LOW;			// FDC List ǥ�� ���� �ּ� (���� ��������)
	static int  iACTUATOR_EXTENT_UP;			// FDC List ǥ�� ���� �ִ� (���� ��������)
	static int  iUtil_EXTENT_LOW;				// Utility FDC List ǥ�� ���� �ּ� (���� ��������)
	static int  iUtil_EXTENT_UP;				// Utility FDC List ǥ�� ���� �ִ� (���� ��������)


	static char cECID_PATH[STR_DEF_LEN];		// ECIM �׸� �������
	static char cECID_ECV_PATH[STR_DEF_LEN];	// ECIM �׸� ���� ���� �� �������


	static char cHsmsConfig[STR_DEF_LEN];		// XCOM ���� �������� ���� �׸� ������� "cfgXComAMT_MIT.cfg"

	static char cQMS_DATA_PATH[STR_DEF_LEN];	// QMS DATA PATH (S:W)
	static char cQMS_IMG_PATH[STR_DEF_LEN];		// QMS IMG PATH (T:\)
	static char cQMS_DUMMY_PATH[STR_DEF_LEN];	// QMS DUMMY PATH (U:\)

	static char cPG_RESULT_PATH[STR_DEF_LEN];	// ��ȣ�� ��� ������� (X:\)
	static char cVT1_INFO_PATH[STR_DEF_LEN];	// Inform.ini ������  (Z:\inform.ini)
	static char cVT1_RESULT_PATH[STR_DEF_LEN];	// �˻�PC1 �˻��� ������� (Z:\)
	static char cVT2_RESULT_PATH[STR_DEF_LEN];	// �˻�PC2 �˻���� ������� (Y:\)

	static int  iVT1_EXTENT_LOW;				// �˻�PC 1�� FDC �׸� �ּҰ�
	static int  iVT1_EXTENT_UP;					// �˻�PC 1�� FDC �׸� �ִ밪
	static int  iVT2_EXTENT_LOW;				// �˻�PC 2�� FDC �׸� �ּҰ�
	static int  iVT2_EXTENT_UP;					// �˻�PC 2�� FDC �׸� �ִ밪

	static char cSOFT_REV[STR_DEF_LEN];			// ��������� SoftVersion 6�ڸ� ����Ÿ S1F1, S1F2
	static char cProcessID[STR_DEF_LEN];		// ��� Process �̸� ? 
	static BOOL bFdcReportUsed;					// FDC �׸� Trace Data ���� ����
	static char cRecipePath[STR_DEF_LEN];		// �PC Recipe(Model) ���� ���
	static BOOL bCellLoadingID;					// ����� Cell Loading�� ID���� CELL ID or INNER ID ���� ���� ( Y: CELLID , N: INNERID)
	static BOOL	bQmsWriteAmt;					// QMS ��� ����
	static BOOL	bMtpWUsed;						// QMS �� MTPW �������
	static BOOL	bTspUsed;						// QMS �� MTPR �������
	static BOOL	bMtpRUsed;						// QMS �� TSP �������

	// Machine ����
	static char cMaint[STR_DEF_LEN];
	static char cAvilability[STR_DEF_LEN];
	static char cInterlock[STR_DEF_LEN];
	static char cMove[STR_DEF_LEN];
	static char cRun[STR_DEF_LEN];
	static char cFront[STR_DEF_LEN];			// INILE  Front ���� �������
	static char cRear[STR_DEF_LEN];				// INLINE Rear ���� �������
	static char cSampleLot[STR_DEF_LEN];

	static BOOL bWriteLog;						// �α� �ۼ� ����
	static BOOL bWriteTrackOutInfo;				// 20170704 BKH Cell Track out info �ۼ� ����
	static char cMachineIP[STR_DEF_LEN];		// �PC(Machine) IP
	static int  iMachinePort;					// �PC(Machine) Port

	static char cDES_BASIC_PATH[STR_DEF_LEN];	//XCOM S8F1���� ������ S8F2�� ������ �ʿ��� ����Ÿ
	static char cDES_HOST_IP[STR_DEF_LEN];		//XCOM S8F1���� ������ S8F2�� ������ �ʿ��� ����Ÿ
	static char cDES_LOGIN_ID[STR_DEF_LEN];		//XCOM S8F1���� ������ S8F2�� ������ �ʿ��� ����Ÿ
	static char cDES_LOGIN_PW[STR_DEF_LEN];		//XCOM S8F1���� ������ S8F2�� ������ �ʿ��� ����Ÿ
	static char cMCC_BASIC_PATH[STR_DEF_LEN];	//XCOM S8F1���� ������ S8F2�� ������ �ʿ��� ����Ÿ
	static int iMCC_SAMPLING_TIME;				//XCOM S8F1���� ������ S8F2�� ������ �ʿ��� ����Ÿ
	static char cMCC_HOST_IP[STR_DEF_LEN];		//XCOM S8F1���� ������ S8F2�� ������ �ʿ��� ����Ÿ
	static char cMCC_LOGIN_ID[STR_DEF_LEN];		//XCOM S8F1���� ������ S8F2�� ������ �ʿ��� ����Ÿ
	static char cMCC_LOGIN_PW[STR_DEF_LEN];		//XCOM S8F1���� ������ S8F2�� ������ �ʿ��� ����Ÿ




	//////////////////////////////////////////////////////////////////////////
	// DELTE
	//static char cALARM_PATH[STR_DEF_LEN];
	//static char cTEACH_PATH[STR_DEF_LEN];
	//static char cDEVICEID[STR_DEF_LEN];			
	//static char cEQIP[STR_DEF_LEN];
	//static char cFDC_PATH[STR_DEF_LEN];
	//static char cREPAIR_INFO_PATH[STR_DEF_LEN];
	//static char cEQP_SAVE_PATH[STR_DEF_LEN];				
	//static char cHOST_SAVE_PATH[STR_DEF_LEN];				
	//static char cHOST_SAVE_IMG_PATH[STR_DEF_LEN];			
	//static char cHOST_SAVE_MONITOR_PATH[STR_DEF_LEN];		
	//static char cSIGNAL_RESULT_PATH[STR_DEF_LEN];			
	//static BOOL bTprUsed;
	//static char cFDC_GRAPH_LOWER[STR_DEF_LEN];
	//static char cFDC_GRAPH_UPPER[STR_DEF_LEN];
	//static int  iSemPort;
	//static int  iSemIntervalTime;
	//static char cTACT_GRAPH_LOWER[STR_DEF_LEN];
	//static char cTACT_GRAPH_UPPER[STR_DEF_LEN];
	//static char cOfflinePassword[STR_DEF_LEN];
	//static BOOL bLightLog;
	//static int  iDBCellStorePeriod;
	//static int  iDBAlarmStorePeriod;	
	



	static BOOL Read(CString strFileName);
	static BOOL Write(CString strFileName);
	static BOOL Write2(CString strFileName, CString strSection, CString strKey, CString strValue);
};

#endif

