#ifndef _SETTING_PARAM_H_
#define _SETTING_PARAM_H_

class CSettingParm
{
public:
	CSettingParm();
	~CSettingParm();
public:
	static char cMODEL_PATH[STR_DEF_LEN];		// 운영PC의 Model 파일 저장 경로
	static char cEQPCONFIG_PATH[STR_DEF_LEN];	// 운영PC의 Config 경로
	static char cEQID[STR_DEF_LEN];				// B1BND15N_A21 
	static char cEQNAME[STR_DEF_LEN];			// AMT
	static char cXComIP[STR_DEF_LEN];			// XCOM IP
	static char cFDC_SV_PATH[STR_DEF_LEN];		// 운영PC의 FDC 기록 파일 저장경로
	static char cCELL_LOG_PATH[STR_DEF_LEN];	// DATAPC에서 가져갈 수 있도록 별도로 저장하는 파일 경로

	static int  iACTUATOR_EXTENT_LOW;			// FDC List 표기 범위 최소 (추후 삭제예정)
	static int  iACTUATOR_EXTENT_UP;			// FDC List 표기 범위 최대 (추후 삭제예정)
	static int  iUtil_EXTENT_LOW;				// Utility FDC List 표기 범위 최소 (추후 삭제예정)
	static int  iUtil_EXTENT_UP;				// Utility FDC List 표기 범위 최대 (추후 삭제예정)


	static char cECID_PATH[STR_DEF_LEN];		// ECIM 항목 저장장소
	static char cECID_ECV_PATH[STR_DEF_LEN];	// ECIM 항목에 대한 실제 값 저장장소


	static char cHsmsConfig[STR_DEF_LEN];		// XCOM 연결 설정값에 대한 항목 저장장소 "cfgXComAMT_MIT.cfg"

	static char cQMS_DATA_PATH[STR_DEF_LEN];	// QMS DATA PATH (S:W)
	static char cQMS_IMG_PATH[STR_DEF_LEN];		// QMS IMG PATH (T:\)
	static char cQMS_DUMMY_PATH[STR_DEF_LEN];	// QMS DUMMY PATH (U:\)

	static char cPG_RESULT_PATH[STR_DEF_LEN];	// 신호기 결과 저장장소 (X:\)
	static char cVT1_INFO_PATH[STR_DEF_LEN];	// Inform.ini 저장경로  (Z:\inform.ini)
	static char cVT1_RESULT_PATH[STR_DEF_LEN];	// 검사PC1 검사결과 저장장소 (Z:\)
	static char cVT2_RESULT_PATH[STR_DEF_LEN];	// 검사PC2 검사결좌 저장장소 (Y:\)

	static int  iVT1_EXTENT_LOW;				// 검사PC 1의 FDC 항목 최소값
	static int  iVT1_EXTENT_UP;					// 검사PC 1의 FDC 항목 최대값
	static int  iVT2_EXTENT_LOW;				// 검사PC 2의 FDC 항목 최소값
	static int  iVT2_EXTENT_UP;					// 검사PC 2의 FDC 항목 최대값

	static char cSOFT_REV[STR_DEF_LEN];			// 상위보고용 SoftVersion 6자리 데이타 S1F1, S1F2
	static char cProcessID[STR_DEF_LEN];		// 장비 Process 이름 ? 
	static BOOL bFdcReportUsed;					// FDC 항목 Trace Data 전송 유무
	static char cRecipePath[STR_DEF_LEN];		// 운영PC Recipe(Model) 저장 장소
	static BOOL bCellLoadingID;					// 운영에서 Cell Loading시 ID값을 CELL ID or INNER ID 선택 문구 ( Y: CELLID , N: INNERID)
	static BOOL	bQmsWriteAmt;					// QMS 기록 유무
	static BOOL	bMtpWUsed;						// QMS 내 MTPW 기록유무
	static BOOL	bTspUsed;						// QMS 내 MTPR 기록유무
	static BOOL	bMtpRUsed;						// QMS 내 TSP 기록유무

	// Machine 상태
	static char cMaint[STR_DEF_LEN];
	static char cAvilability[STR_DEF_LEN];
	static char cInterlock[STR_DEF_LEN];
	static char cMove[STR_DEF_LEN];
	static char cRun[STR_DEF_LEN];
	static char cFront[STR_DEF_LEN];			// INILE  Front 설비 연결상태
	static char cRear[STR_DEF_LEN];				// INLINE Rear 설비 연결상태
	static char cSampleLot[STR_DEF_LEN];

	static BOOL bWriteLog;						// 로그 작성 유무
	static BOOL bWriteTrackOutInfo;				// 20170704 BKH Cell Track out info 작성 유무
	static char cMachineIP[STR_DEF_LEN];		// 운영PC(Machine) IP
	static int  iMachinePort;					// 운영PC(Machine) Port

	static char cDES_BASIC_PATH[STR_DEF_LEN];	//XCOM S8F1으로 받을시 S8F2로 보낼때 필요한 데이타
	static char cDES_HOST_IP[STR_DEF_LEN];		//XCOM S8F1으로 받을시 S8F2로 보낼때 필요한 데이타
	static char cDES_LOGIN_ID[STR_DEF_LEN];		//XCOM S8F1으로 받을시 S8F2로 보낼때 필요한 데이타
	static char cDES_LOGIN_PW[STR_DEF_LEN];		//XCOM S8F1으로 받을시 S8F2로 보낼때 필요한 데이타
	static char cMCC_BASIC_PATH[STR_DEF_LEN];	//XCOM S8F1으로 받을시 S8F2로 보낼때 필요한 데이타
	static int iMCC_SAMPLING_TIME;				//XCOM S8F1으로 받을시 S8F2로 보낼때 필요한 데이타
	static char cMCC_HOST_IP[STR_DEF_LEN];		//XCOM S8F1으로 받을시 S8F2로 보낼때 필요한 데이타
	static char cMCC_LOGIN_ID[STR_DEF_LEN];		//XCOM S8F1으로 받을시 S8F2로 보낼때 필요한 데이타
	static char cMCC_LOGIN_PW[STR_DEF_LEN];		//XCOM S8F1으로 받을시 S8F2로 보낼때 필요한 데이타




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

