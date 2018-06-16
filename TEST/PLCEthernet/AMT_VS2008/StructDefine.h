

typedef struct _tagEQ_GEN_INFO {
	BOOL		bAuto;				// 1 : Auto, 0 : Manual (Navigation Button)
	int			nJobMode;			// 1 : Pass Mode, 0 ; Normal (Command Button)
	BOOL		bMcOnline;			// Deposition Equipment Online 
	BOOL		bIfMcOnline;		// Deposition Equipment Online  
	BOOL		bMcOnlineReq;		// Deposition Equipment Online Request

	int			nGlassNo;			// Equipment Glass Number
	UINT		nTotalGlassNo;		// Historical glass number
	
	CString strModelName;

} EQ_GEN_INFO;


typedef struct _tagPLC_INFO {

	short						m_iStation;
	short						m_iNetWork;
	long						m_lNetPath;
	short						m_iNetMode;
	short						m_iNetChanel;

} PLC_INFO;

typedef struct _tagMAIN_FLAG {
	
	BOOL bInitDeviceNet;
	BOOL bInitMotion;
	
} MAIN_FLAG;


typedef struct sMarkSearchRet
{
	CString PanelID;
	int ContactRetry[MAX_CAM_NUM];				//chg-pck :ContactRetry[8]->ContactRetry[13]					
	CString ContactYN;						
	double ContactX[MAX_CAM_NUM];				//chg-pck :ContactX[8]->ContactX[13]				
	double ContactY[MAX_CAM_NUM];				//chg-pck :ContactY[8]->ContactY[13]
	double ContactFinalX[MAX_CAM_NUM];			//chg-pck :ContactFinalX[8]->ContactFinalX[13]	
	CString SearchYN[MAX_CAM_NUM];				//chg-pck :SearchYN[8]->SearchYN[13]
	CString ImagePath[MAX_CAM_NUM];				//chg-pck :ImagePath[8]->ImagePath[13]
	double ContactScore[MAX_CAM_NUM];

	double ContactDist_LX[MAX_CAM_NUM];
	double ContactDist_RX[MAX_CAM_NUM];
	double ContactDist_PX[MAX_CAM_NUM];

	double ContactDist_LY[MAX_CAM_NUM];
	double ContactDist_RY[MAX_CAM_NUM];
	double ContactDist_PY[MAX_CAM_NUM];

} STRUCT_SEARCH_RET;

typedef struct sResult
{
	CdblPoint PCBPos;		
	CdblPoint FPCPos;
	CdblPoint Distance;								// PCB, FPC Mark 거리차.//
	
} STRUCT_ALIGN_RESULT;

typedef struct sMotorStage								// Stage Motor Position.//
{
	double dx[MAX_CAM_NUM];										// Stage Left #1 X.//
	double dy[MAX_CAM_NUM];										// Stage Right #1 X.//
} STRUCT_STAGE_POS;

// User Info Struct
typedef struct USER_INFO{
	int			nLEVEL[100];
	CString		strName[100];
	CString		strPwd[100];
} USER_INFO;


// PATTEN DATA. //


typedef struct sMotorPara
{
	int iMotor;
	int iEncoder;
	int iVoltage;
	int iFeedback;
	int iEncoderType;
	BOOL bStepMode;
	BOOL bCordinateDir;
	BOOL bEncoderDir;
	BOOL bPosLimit;
	BOOL bNegLimit;
	BOOL bHomeSen;
	BOOL bAmpFault;
	BOOL bAmpReset;
	BOOL bAmpEnable;
	int iInPosion;
	double dPosSoftLimit;
	double dNegSoftLimit;
	double dGearRatio;
	double dGearPitch;
	int iAcc;
	int iHomeMethod;
	int iHomeSpeed;
	BOOL bHomeDir;
	DWORD dwHomeTimeOut;
	double dSpeed;
	int nAbsEncoderType;

} STRUCT_MOTOR_PARA;

typedef struct CamInfo
{
	// Vision Data. //
	int iCamNo;										
	int iCamX_Mode;	
	int iCamY_Mode;
	
} CAM_INFO;

typedef struct Position_data
{
	// Vision Data. //
	double dx;
	double dy;
	double dz;
	
} POSITION_DATA;

typedef struct sModels
{
	// Vision Data. //
	int iPanelSize;										
	int iContactNum;		// 1 = 1-Contact , 2= 2-Contact
	int iContactType;		// FPC , POGO Type..
	int iContactor;			// 몇 번째 Contact을 사용하는지.. 1=1_Contactor , 2=2_Contactor , 3=3_COntactor, 4 =4_Contactor 
	int iFPCLength;		// FPC , POGO Type..
	int iSizeX;
	int iSizeY;
	int iPinNum;

	int iAlignScore;
	BOOL bAlignAngle;

	CString strHandle;
	
	CString strResource;
	CString strModelName;
	CString strDeviceName;
	CString strGripper1;
	CString strGripper2;
	CString strGripper3;
	CString strGripper4;

	int m_nBluModel;

	// TV Model.//
	double TV_FPCY[MAX_CAM_NUM];	// (1) Y축으로 전진 거리 Image 1.//
	double TV_PCBX[MAX_CAM_NUM];	// (2) 왼쪽 X축으로 이동 Image 2.//
	double TV_StageReady;			// Grab / Ready간 Y축 거리차.//


	CRect rtPattern[MAX_MOD_BUFFER_NUM];
	int nFPCModelNum[MAX_CAM_NUM];
	CPoint ptCenter[MAX_MOD_BUFFER_NUM];

	POSITION_DATA	GrapPos[MAX_CAM_NUM];
	POSITION_DATA	ReadyPos[MAX_CAM_NUM];

	double			dPinZaixPos;
} STRUCT_MODEL_DATA;	

typedef struct sPanelInfo
{
	char	m_sHPanelID[sizeHPANELID+1];				// 13					
	char	m_sEPanelID[sizeEPANELID+1];				// 26
	char	m_sBatchID[sizeBATCHID+1];				// 39
	char	m_sProdType[sizePRODTYP+1];				// 42	
	char	m_sProdKind[sizePRODKND+1];				// 45
	char	m_sDeviceID[sizeDEVICEID+1];				// 64
	char	m_sStepID[sizeSTEPID+1];				// 75
	char	m_sPPID[sizePPID+1];					// 92
	char	m_sThickness[sizeTHICKNESS+1];				// 95
	char	m_sInspectionFlag[sizeINSPFLG+1];			// 98		
	char	m_sPanelSize[sizePANELSIZE+1];				// 103
	char	m_sPanelPosition[sizePANELPOS+1];			// 106				
	char	m_sCount1[sizeCOUNT1+1];					// 109
	char	m_sCount2[sizeCOUNT2+1];					// 112
	char	m_sGrade[sizeGRADE+1];					// 119
	char	m_sComment[sizeCOMMENT+1];				// 136
	char	m_sCompSize[sizeCOMPSIZE+1];				// 139
	char	m_sReadingFlag[sizeREADFLG+1];			// 142
	char	m_sPanelState[sizePANELSTATE+1];				// 145
	char	m_sJudgement[sizeJUDGEMENT+1];				// 150
	char	m_sCode[sizeCODE+1];					// 155
	char	m_sRunLine[sizeRUNLINE+1];				// 176
	char	m_sUniqueID[sizeUNIQUEID+1];				// 181
	char	m_sPairHPanelID[sizePAIR_HID+1];			// 194
	char	m_sPairEPanelID[sizePAIR_EID+1];			// 207
	char	m_sPairGrade[sizePAIR_GRADE+1];				// 214
	char	m_sPairUniqueID[sizePAIR_UNIQUEID+1];			// 219
	char	m_sFlowRecipe[sizeFLOWRECIPE+1];				// 222
	char	m_sReserved[sizeRESERVED+1];				// 235
	char	m_sBitsSignals[sizeBITSIGNAL+1];			// 238
	char	m_sReferData[sizeREFERDATA+1];				// 243
	char	m_sRawPath[sizeRAWPATH];					// 293	// File Server.//
	char	m_sSumPath[sizeSUMPATH];					// 343
	char	m_sImagePath[sizeIMGPATH];				// 363
	char	m_sDisk[sizeDISK];					// 383
	char	m_sOPID[sizeDISK];					// 383
}PANEL_INFO;

class LC_PANEL_INFO	// LC 보고 및 화면 display 전용
{
public:
	CString szPanelInfo[MAX_LC_GLASSDATA];

public:
	LC_PANEL_INFO()
	{
		for(int i = 0; i < MAX_LC_GLASSDATA; i++)
			szPanelInfo[i] = _T("");
	}
	void operator=(const PANEL_INFO &panelInfo)
	{
		szPanelInfo[LC_HPANELID		] = 	panelInfo.m_sHPanelID		;			// 13					
		szPanelInfo[LC_EPANELID		] = 	panelInfo.m_sEPanelID		;			// 26
		szPanelInfo[LC_BATCHID		] = 	panelInfo.m_sBatchID		;			// 39
		szPanelInfo[LC_PRODTYP		] = 	panelInfo.m_sProdType		;			// 42	
		szPanelInfo[LC_PRODKND		] = 	panelInfo.m_sProdKind		;			// 45
		szPanelInfo[LC_DEVICEID		] = 	panelInfo.m_sDeviceID		;			// 64
		szPanelInfo[LC_STEPID		] = 	panelInfo.m_sStepID			;			// 75
		szPanelInfo[LC_PPID			] = 	panelInfo.m_sPPID			;			// 92
		szPanelInfo[LC_THICKNESS	] = 	panelInfo.m_sThickness		;			// 95
		szPanelInfo[LC_INSPFLG		] = 	panelInfo.m_sInspectionFlag	;			// 98		
		szPanelInfo[LC_PANELSIZE	] = 	panelInfo.m_sPanelSize		;			// 103
		szPanelInfo[LC_PANELPOS		] = 	panelInfo.m_sPanelPosition	;			// 106				
		szPanelInfo[LC_COUNT1		] = 	panelInfo.m_sCount1			;			// 109
		szPanelInfo[LC_COUNT2		] = 	panelInfo.m_sCount2			;			// 112
		szPanelInfo[LC_GRADE		] = 	panelInfo.m_sGrade			;			// 119
		szPanelInfo[LC_COMMENT		] = 	panelInfo.m_sComment		;			// 136
		szPanelInfo[LC_COMPSIZE		] = 	panelInfo.m_sCompSize		;			// 139
		szPanelInfo[LC_READFLG		] = 	panelInfo.m_sReadingFlag	;			// 142
		szPanelInfo[LC_PANELSTATE	] = 	panelInfo.m_sPanelState		;			// 145
		szPanelInfo[LC_JUDGEMENT	] = 	panelInfo.m_sJudgement		;			// 150
		szPanelInfo[LC_CODE			] = 	panelInfo.m_sCode			;			// 155
		szPanelInfo[LC_RUNLINE		] = 	panelInfo.m_sRunLine		;			// 176
		szPanelInfo[LC_UNIQUEID		] = 	panelInfo.m_sUniqueID		;			// 181
		szPanelInfo[LC_PAIR_HID		] = 	panelInfo.m_sPairHPanelID	;			// 194
		szPanelInfo[LC_PAIR_EID		] = 	panelInfo.m_sPairEPanelID	;			// 207
		szPanelInfo[LC_PAIR_GRADE	] = 	panelInfo.m_sPairGrade		;			// 214
		szPanelInfo[LC_PAIR_UNIQUEID] = 	panelInfo.m_sPairUniqueID	;			// 219
		szPanelInfo[LC_FLOWRECIPE	] = 	panelInfo.m_sFlowRecipe		;			// 222
		szPanelInfo[LC_RESERVED		] = 	panelInfo.m_sReserved		;			// 235
		szPanelInfo[LC_BITSIGNAL	] = 	panelInfo.m_sBitsSignals	;			// 238
		szPanelInfo[LC_REFERDATA	] = 	panelInfo.m_sReferData		;			// 243
		szPanelInfo[LC_RAWPATH		] = 	panelInfo.m_sRawPath		;			// 293	// File Server.//
		szPanelInfo[LC_SUMPATH		] = 	panelInfo.m_sSumPath		;			// 343
		szPanelInfo[LC_IMGPATH		] = 	panelInfo.m_sImagePath		;			// 363
		szPanelInfo[LC_DISK			] = 	panelInfo.m_sDisk			;			// 383
		szPanelInfo[LC_OPID			] 	= 	panelInfo.m_sOPID			;			// 383

		// 뒤에 공백문자 없앰
		for(int i = 0; i < MAX_LC_GLASSDATA; i++)
			szPanelInfo[i].TrimRight(" ");

		//////////////////////////////////////////////////////////////////////////
		// Binary 변환

		int nThicknesss = panelInfo.m_sThickness[0];
		nThicknesss += panelInfo.m_sThickness[1] * 0x100;
		szPanelInfo[LC_THICKNESS].Format("%d", nThicknesss);

		int nSizeX = panelInfo.m_sPanelSize[0];
		nSizeX += panelInfo.m_sPanelSize[1] * 0x100;
		int nSizeY = panelInfo.m_sPanelSize[2];
		nSizeY += panelInfo.m_sPanelSize[3] * 0x100;
		szPanelInfo[LC_PANELSIZE].Format("%d/%d", nSizeX, nSizeY);

		int nCompSize = panelInfo.m_sCompSize[0];
		nCompSize += panelInfo.m_sCompSize[1] * 0x100;
		szPanelInfo[LC_COMPSIZE].Format("%d", nCompSize);

		int nPanelState = panelInfo.m_sPanelState[0];
		nPanelState += panelInfo.m_sPanelState[1] * 0x100;
		szPanelInfo[LC_PANELSTATE].Format("%d", nPanelState);
		//////////////////////////////////////////////////////////////////////////
	}
};						 

typedef struct sPG_Info
{
public:
#pragma pack(push, 1)
	unsigned short  m_wPacketsize;			// Size.//
	unsigned short	m_sCommandID;			// SendMessage ID.//
	unsigned char	m_sCommand;				// Command.//
	char			m_sStatus;				// 장비 상태.//
	char			m_sHPanelID[12 + 1];	// Part ID.//
	char			m_sDeviceID[18 + 1];	// Device ID.//
	char			m_sBatchID[12 + 1];		// Batch ID.//
	char			m_sStepID[10+1];
	char			m_sOPName[15];			// OP Name.//
	char			m_sOPID[15];			// OP ID.//
	char			m_sJudgement[4 + 1];			
	char			m_sCode[4 + 1];
	char			m_sRawPath[80];			
	char			m_sSumPath[80];			
	char			m_sImagePath[80];		
	char			m_sDisk[80];			
	char			m_sAlmMessage[128];
#pragma pack(pop)
}PG_INFO;


typedef struct sVI_DATA	
{
public:
	char			m_sHGlassID[12+1];					
	char			m_sEGlassID[12+1];				
	char			m_sProdType[2+1];					
	char			m_sProcessID[18+1];			
	char			m_sStepID[10+1];	
	char			m_sBatchID[12+1];				
	char			m_sPPID[16+1];					
	char			m_sGlassSize[4+1];				
	char			m_sGlassThickness[2+1];					
	char			m_sCount[2+1];	
	char			m_sGlassGrade[6+1];					
	char			m_sInspectionFlag[2+1];				
	char			m_sJudgement[2+1];					
	char			m_sCode[4+1];				
	char			m_sRunningHistory[20+1];	
	char			m_sGlassUniqueID[4+1];				
	char			m_sPairHGlassID[12+1];					
	char			m_sPairEGlassID[12+1];					
	char			m_sPairGlassUniqueID[4+1];
	char			m_sJobOrder[2+1];
	char			m_sMStartFlag[2+1];				
	char			m_sGlassPosition[2+1];					
	char			m_sProdKind[2+1];				
	char			m_sComment[16+1];	
	char			m_sMatchGroup[2+1];			
	char			m_sFileLocation[2+1];					
	char			m_sSortFlag[2+1];				
	char			m_sDefectCount[2+1];					
	char			m_sFlowRecipe[2+1];
	char			m_sReserved[18+1];				// // 20080709 14:20:01
	char			m_sReferData[4+1];				// // ECA1

	char			m_sRawPath[50];					// 293	// File Server.//		
	char			m_sSumPath[50];					// 343
	char			m_sImagePath[20];				// 363
	char			m_sDisk[20];					// 383
}VI_DATA;

class CSerialSettingData		// RS232 설정 Data.//
{
public:
	BOOL Used;
	int IndexPortNo;					// Port.//
	int IndexBaudRate;						//
	int IndexDataBit;						//
	int IndexStopBit;
	int IndexParityBit;

	CSerialSettingData()
	{
		Used = FALSE;
		IndexPortNo = Index_Port_1;
		IndexBaudRate = Index_Baud_110;
		IndexDataBit = Index_DataBit_5;
		IndexStopBit = Index_StopBit_10;
		IndexParityBit = Index_ParityBit_None;
	}
};

typedef struct sLastGlassData			// 마지막 패널 Data.//
{
	CString strHPanelID;				
	CString strEPanelID;				// EPanel ID.//
	CString strBatchID;					// Batch ID.//
	CString strDeviceID;
	CString strInspection;
	CString strPairH;
	CString strPairE;
	CString strOPID;
	CString strOPName;
// 	int		dOutFlag;
} STRUCT_LAST_GLASSDATA;
