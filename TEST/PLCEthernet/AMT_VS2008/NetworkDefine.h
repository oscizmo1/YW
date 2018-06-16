
//////////////////////////////////////////////////////////////////////////
//Melsec board Setting
#define INT_MNET_PATH				51
#define INT_MNET_MODE				1			//1: ONLINE, 2: OFFLINE, 3:Forward loop test, 4: Reverse loop test
#define INT_MNET_CHANNEL			51
#define INT_MNET_NETWORK			1			//Network Number
#define INT_MNET_STATION			255			//Station Number (255: Auto)
//////////////////////////////////////////////////////////////////////////
// PLC Start Address
#define LOAD_INPUT_START				GetDoc()->GetMelsecAddress()->LoadInputStart	// PLC Loader 
#define LOAD_OUTPUT_START				GetDoc()->GetMelsecAddress()->LoadOutputStart	// PC Loader
#define UNLOAD_INPUT_START				GetDoc()->GetMelsecAddress()->UnloadInputStart	// PLC Unloader
#define UNLOAD_OUTPUT_START				GetDoc()->GetMelsecAddress()->UnloadOutputStart	// PC Unloader
#define SW_INTERLOCK_INPUT_START		UNLOAD_INPUT_START + 0x10	// 
#define SW_INTERLOCK_OUTPUT_START		UNLOAD_OUTPUT_START + 0x10	// 
#define SW2_INTERLOCK_INPUT_START		LOAD_INPUT_START + 0x10	// 
#define SW2_INTERLOCK_OUTPUT_START		LOAD_OUTPUT_START + 0x10	// 
#define DATA_INPUT_START				GetDoc()->GetMelsecAddress()->DataInputStart	// PLC Data
#define DATA_OUTPUT_START				GetDoc()->GetMelsecAddress()->DataOutputStart	// PC Data

#define NON_STOP_INPUT_WORD_START		GetDoc()->GetMelsecAddress()->NonStopInputWordStart
#define NON_STOP_OUTPUT_WORD_START		GetDoc()->GetMelsecAddress()->NonStopOutputWordStart

//2009.06.07 Tilting Word Data Area
#define TILT_INPUT_WORD_START			GetDoc()->GetMelsecAddress()->TiltInputWordStart
#define TILT_OUTPUT_WORD_START			GetDoc()->GetMelsecAddress()->TiltOutputWordStart

#define DRY_RUN_PANEL_EXIST				GetDoc()->GetMelsecAddress()->DryRunPanelExist
//////////////////////////////////////////////////////////////////////////

#define MEL_READY				0	
#define MEL_PAUSE				1
#define MEL_DOWM				2
#define MEL_ALARM				3
#define MEL_REQ_SUPPLY			4
#define MEL_DO_SUPPLY			5
#define MEL_END_SUPPLY			6
#define MEL_VI_END_UNLOAD		7
#define MEL_PM_MODE				8
#define MEL_ALARM_MODE			9
#define MEL_EMO_MODE			10
#define MEL_CLAMP_OFF			11		
#define MEL_HAND_UP				12		//090212 Hui
#define MEL_PLC_PANEL_CHECK		14		//07/28/2009 Hui D군용.

#define MEL_SW_START			SW_INTERLOCK_INPUT_START
#define MEL_SW_STOP				SW_INTERLOCK_INPUT_START + 0x1
#define MEL_SW_RESET			SW_INTERLOCK_INPUT_START + 0x2
#define MEL_SW_ENABLE			SW_INTERLOCK_INPUT_START + 0x3
#define MEL_SW_LOADER_TILT		SW_INTERLOCK_INPUT_START + 0x4	// Handler Arm Stretch
#define MEL_SW_MMT_LOADING		SW_INTERLOCK_INPUT_START + 0x5
#define MEL_SW_MMT_EMO			SW_INTERLOCK_INPUT_START + 0x6

#define MEL_SW_LOADER_CLEAR		SW_INTERLOCK_OUTPUT_START + 0x7
#define MEL_SW_UNLOADER_CLEAR	SW_INTERLOCK_OUTPUT_START + 0x8
#define MEL_SW_OP_CALL			SW_INTERLOCK_OUTPUT_START + 0x9
#define MEL_SW_ALARM_MODE		SW_INTERLOCK_OUTPUT_START + 0xA
#define MEL_SW_WATCH_DOG		SW_INTERLOCK_OUTPUT_START + 0xB	// PC Run
#define MEL_SW_PANEL_EXIST		SW_INTERLOCK_OUTPUT_START + 0xC
#define MEL_SW_AREA				SW_INTERLOCK_OUTPUT_START + 0xD
#define MEL_SW_TILT				SW_INTERLOCK_OUTPUT_START + 0xE	// Tilting Interlock
#define MEL_SW_RUN_STATE		SW_INTERLOCK_OUTPUT_START + 0xF	// PC RUN

//////////////////////////////////////////////////////////////////////////
// Tilting 영역 추가 [5/19/2009 OSC]	
// 2009.06.15 KHW Update.
// #define MEL_SW2_TILT_READY		SW2_INTERLOCK_INPUT_START + 0xE		// Tilting 로딩 위치
// #define MEL_SW2_TILT_TILTING	SW2_INTERLOCK_INPUT_START + 0xF		// Tilting 검사 위치
#define MEL_SW2_TILT_ALARM_STAT		SW2_INTERLOCK_INPUT_START
#define MEL_SW2_TILT_INIT_COMP	SW2_INTERLOCK_INPUT_START + 0x1
#define MEL_SW2_TILT_BUSY		SW2_INTERLOCK_INPUT_START + 0x2
#define MEL_SW2_TILT_SERVO_STAT	SW2_INTERLOCK_INPUT_START + 0x3
#define MEL_SW2_TILT_READY		SW2_INTERLOCK_INPUT_START + 0x4
#define MEL_SW2_TILT_INSP		SW2_INTERLOCK_INPUT_START + 0x5
#define MEL_SW2_TILT_AXIS_STOP	SW2_INTERLOCK_INPUT_START + 0x6

// #define MEL_SW2_TILT_JOG_P		SW2_INTERLOCK_OUTPUT_START			// Tilting Jog +
// #define MEL_SW2_TILT_JOG_N		SW2_INTERLOCK_OUTPUT_START + 0x1	// Tilting Jog -
// #define MEL_SW2_TILT_SAVE_READY	SW2_INTERLOCK_OUTPUT_START + 0x2	// Tilting 로딩위치 확인
// #define MEL_SW2_TILT_SAVE_TILT	SW2_INTERLOCK_OUTPUT_START + 0x3	// Tilting 검사위치 확인
// #define MEL_SW2_TILT_SAVE		SW2_INTERLOCK_OUTPUT_START + 0x4	// Tilting Teaching position save
// #define MEL_SW2_TILT_MANUAL		SW2_INTERLOCK_OUTPUT_START + 0xF	// Tilting Manual Start

#define MEL_SW2_TILT_INTERLOCK	SW2_INTERLOCK_OUTPUT_START
#define MEL_SW2_TILT_ALARM_RST	SW2_INTERLOCK_OUTPUT_START + 0x1
#define MEL_SW2_TILT_INIT_RST	SW2_INTERLOCK_OUTPUT_START + 0x2
#define MEL_SW2_TILT_INIT_REQ	SW2_INTERLOCK_OUTPUT_START + 0x3
#define MEL_SW2_TILT_JOG_P		SW2_INTERLOCK_OUTPUT_START + 0x4
#define MEL_SW2_TILT_JOG_N		SW2_INTERLOCK_OUTPUT_START + 0x5
#define MEL_SW2_TILT_SAVE_READY	SW2_INTERLOCK_OUTPUT_START + 0x6
#define MEL_SW2_TILT_SAVE_INSP	SW2_INTERLOCK_OUTPUT_START + 0x7
#define MEL_SW2_TILT_MOVE_READY	SW2_INTERLOCK_OUTPUT_START + 0x8
#define MEL_SW2_TILT_MOVE_INSP	SW2_INTERLOCK_OUTPUT_START + 0x9
#define MEL_SW2_TILT_ABORT		SW2_INTERLOCK_OUTPUT_START + 0xA
#define MEL_SW2_TILT_SERVO_ON	SW2_INTERLOCK_OUTPUT_START + 0xB
#define MEL_SW2_TILT_SERVO_OFF	SW2_INTERLOCK_OUTPUT_START + 0xC
#define	MEL_SW2_TILT_PRE_SIGN	SW2_INTERLOCK_OUTPUT_START + 0xD
//#define		SW2_INTERLOCK_OUTPUT_START + 0xE
#define MEL_SW2_TILT_MANUAL		SW2_INTERLOCK_OUTPUT_START + 0xF


//////////////////////////////////////   Melsec In Put /////////////////////////////////////////////////

#define LD_IN_READY				LOAD_INPUT_START + MEL_READY
#define LD_IN_PAUSE				LOAD_INPUT_START + MEL_PAUSE
#define LD_IN_DOWM				LOAD_INPUT_START + MEL_DOWM
//#define LD_IN_ALARM				LOAD_INPUT_START + MEL_ALARM	Not Use
#define LD_IN_REQ_SUPPLY		LOAD_INPUT_START + MEL_REQ_SUPPLY
#define LD_IN_DO_SUPPLY			LOAD_INPUT_START + MEL_DO_SUPPLY
#define LD_IN_END_SUPPLY		LOAD_INPUT_START + MEL_END_SUPPLY
#define LD_IN_VI_END_UNLOAD		LOAD_INPUT_START + MEL_VI_END_UNLOAD	
#define LD_IN_PM_MODE			LOAD_INPUT_START + MEL_PM_MODE
#define LD_IN_EMO_MODE			LOAD_INPUT_START + MEL_EMO_MODE
#define LD_IN_CLAMP_OFF			LOAD_INPUT_START + MEL_CLAMP_OFF
#define LD_IN_HAND_UP			LOAD_INPUT_START + MEL_HAND_UP	// Handler Up

#define UNLD_IN_READY			UNLOAD_INPUT_START + MEL_READY
#define UNLD_IN_PAUSE			UNLOAD_INPUT_START + MEL_PAUSE
#define UNLD_IN_DOWM			UNLOAD_INPUT_START + MEL_DOWM
//#define UNLD_IN_ALARM			UNLOAD_INPUT_START + MEL_ALARM	Not Use
#define UNLD_IN_REQ_SUPPLY		UNLOAD_INPUT_START + MEL_REQ_SUPPLY
#define UNLD_IN_DO_SUPPLY		UNLOAD_INPUT_START + MEL_DO_SUPPLY
#define UNLD_IN_END_SUPPLY		UNLOAD_INPUT_START + MEL_END_SUPPLY
#define UNLD_IN_VI_END_UNLOAD	UNLOAD_INPUT_START + MEL_VI_END_UNLOAD	
#define UNLD_IN_PM_MODE			UNLOAD_INPUT_START + MEL_PM_MODE
#define UNLD_IN_EMO_MODE		UNLOAD_INPUT_START + MEL_EMO_MODE

#define UNLD_IN_PLC_PANEL_CHECK	UNLOAD_INPUT_START + MEL_PLC_PANEL_CHECK

//////////////////////////////////////   Melsec Out Put /////////////////////////////////////////////////

#define LD_OUT_READY			LOAD_OUTPUT_START + MEL_READY
#define LD_OUT_PAUSE			LOAD_OUTPUT_START + MEL_PAUSE
#define LD_OUT_DOWM				LOAD_OUTPUT_START + MEL_DOWM
//#define LD_OUT_ALARM			LOAD_OUTPUT_START + MEL_ALARM	Not Use
#define LD_OUT_REQ_SUPPLY		LOAD_OUTPUT_START + MEL_REQ_SUPPLY
#define LD_OUT_DO_SUPPLY		LOAD_OUTPUT_START + MEL_DO_SUPPLY
#define LD_OUT_END_SUPPLY		LOAD_OUTPUT_START + MEL_END_SUPPLY
#define LD_OUT_VI_END_UNLOAD	LOAD_OUTPUT_START + MEL_VI_END_UNLOAD	
#define LD_OUT_PM_MODE			LOAD_OUTPUT_START + MEL_PM_MODE
#define LD_OUT_EMO_MODE			LOAD_OUTPUT_START + MEL_EMO_MODE

#define UNLD_OUT_READY			UNLOAD_OUTPUT_START + MEL_READY
#define UNLD_OUT_PAUSE			UNLOAD_OUTPUT_START + MEL_PAUSE
#define UNLD_OUT_DOWM			UNLOAD_OUTPUT_START + MEL_DOWM
//#define UNLD_OUT_ALARM			UNLOAD_OUTPUT_START + MEL_ALARM	Not Use
#define UNLD_OUT_REQ_SUPPLY		UNLOAD_OUTPUT_START + MEL_REQ_SUPPLY
#define UNLD_OUT_DO_SUPPLY		UNLOAD_OUTPUT_START + MEL_DO_SUPPLY
#define UNLD_OUT_END_SUPPLY		UNLOAD_OUTPUT_START + MEL_END_SUPPLY
#define UNLD_OUT_VI_END_UNLOAD	UNLOAD_OUTPUT_START + MEL_VI_END_UNLOAD	
#define UNLD_OUT_PM_MODE		UNLOAD_OUTPUT_START + MEL_PM_MODE
#define UNLD_OUT_EMO_MODE		UNLOAD_OUTPUT_START + MEL_EMO_MODE
#define UNLD_OUT_CLAMP_OFF		UNLOAD_OUTPUT_START	+ MEL_CLAMP_OFF	//	
//////////////////////////////////////////////////////////////////////////
// Non Stop
#define NON_STOP_PLC_RESERVE	LOAD_INPUT_START + 0xF
#define NON_STOP_PC_RESERVE		LOAD_OUTPUT_START + 0xC
#define NON_STOP_PC_START		LOAD_OUTPUT_START + 0xD
#define NON_STOP_PC_END			LOAD_OUTPUT_START + 0xE
#define NON_STOP_PC_FAIL		LOAD_OUTPUT_START + 0xF

#define NON_STOP_PANEL_EXIST	LOAD_OUTPUT_START + 0x9	// 현재 재공(Number)	Panel Check Bit로 대체

#define NON_STOP_DEVICEID		NON_STOP_INPUT_WORD_START
#define NON_STOP_MACHINE_STATE	NON_STOP_OUTPUT_WORD_START			// 미사용
#define NON_STOP_FAIL_TYPE		NON_STOP_OUTPUT_WORD_START + 0x1	// 기종 변경 불가 요인(ASCII)

#define sizeNonStopDeviceID			20
#define sizeNonStopMACHINE_STATE	2
#define sizeNonStopFailType			4
#define sizeNonStopPANEL_EXIST		2
#define NON_STOP_FT_NONE			_T("    ")
#define NON_STOP_FT_DOWN			_T("DOWN")
#define NON_STOP_FT_T2TO			_T("T2TO")
#define NON_STOP_FT_T3TO			_T("T3TO")
//////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
/// Tilt
#define TILT_INPUT_CUR_POS			TILT_INPUT_WORD_START
#define TILT_INPUT_READY_POS		TILT_INPUT_WORD_START + 0x2 
#define TILT_INPUT_INSPECT_POS		TILT_INPUT_WORD_START + 0x4
#define TILT_INPUT_ALARM_CODE		TILT_INPUT_WORD_START + 0x6
// Tilt word output
#define TILT_OUTPUT_JOG_SPEED		TILT_OUTPUT_WORD_START
#define TILT_OUTPUT_RUN_SPEED		TILT_OUTPUT_WORD_START + 0x2


#define DioInput(no)			(GetDoc()->m_pDeviceNet->ReadIO((no)))
#define DioOutIn(no)			(GetDoc()->m_pDeviceNet->ReadOutBit((no)))
#define DioOutput(no, val)		(GetDoc()->m_pDeviceNet->WriteIO((no),(val)))

#define CONTACT_UNIT(A)			(GetDeviceCtl()->ContactUnit(A) )
#define CLAMP_UNIT(A)			(GetDeviceCtl()->ClampUnit(A) )
#define BLOW_UNIT(A)			(GetDeviceCtl()->BlowUnit(A) )

#define diCLAMP_UNIT_STATUS		( GetDeviceCtl()->ChkClampUnit() ? 1 : 0 )
#define diPANEL_EXIST_STATUS	( GetDeviceCtl()->ChkPanelExist() ? 1 : 0 )
#define diMAINPOWER_STATUS		(GetDeviceCtl()->ChkMainPower() ? 1 : 0 )

// GLASS DATA SIZE
#define sizeTotalGlassData		212 // 106 WORDs
#define sizeHPANELID			12	// + 1
#define sizeEPANELID			12	// + 1
#define sizeBATCHID				12	// + 1
#define sizePRODTYP				2	// + 1
#define sizePRODKND				2	// + 1
#define sizeDEVICEID			18	// + 1
#define sizeSTEPID				10	// + 1
#define sizePPID				16	// + 1
#define sizeTHICKNESS			2	// + 1
#define sizeINSPFLG				2	// + 1
#define sizePANELSIZE			4	// + 1
#define sizePANELPOS			2	// + 1
#define sizeCOUNT1				2	// + 1
#define sizeCOUNT2				2	// + 1
#define sizeGRADE				6	// + 1
#define sizeCOMMENT				16	// + 1
#define sizeCOMPSIZE			2	// + 1
#define sizeREADFLG				2	// + 1
#define sizePANELSTATE			2	// + 1
#define sizeJUDGEMENT			4	// + 1
#define sizeCODE				4	// + 1
#define sizeRUNLINE				20	// + 1
#define sizeUNIQUEID			4	// + 1
#define sizePAIR_HID			12	// + 1
#define sizePAIR_EID			12	// + 1
#define sizePAIR_GRADE			6	// + 1
#define sizePAIR_UNIQUEID		4	// + 1
#define sizeFLOWRECIPE			2	// + 1
#define sizeRESERVED			12	// + 1
#define sizeBITSIGNAL			2	// + 1
#define sizeREFERDATA			4	// + 1
#define sizeRAWPATH				50
#define sizeSUMPATH				50
#define sizeIMGPATH				20
#define sizeDISK				20
#define sizeOPID				15

// READ ADDRESS
#define DATA_INPUT_HPANELID			DATA_INPUT_START
#define DATA_INPUT_EPANELID			DATA_INPUT_HPANELID		+	sizeHPANELID/2			
#define DATA_INPUT_BATCHID			DATA_INPUT_EPANELID		+	sizeEPANELID/2			
#define DATA_INPUT_PRODTYP			DATA_INPUT_BATCHID		+	sizeBATCHID/2			
#define DATA_INPUT_PRODKND			DATA_INPUT_PRODTYP		+	sizePRODTYP/2			
#define DATA_INPUT_DEVICEID			DATA_INPUT_PRODKND		+	sizePRODKND/2			
#define DATA_INPUT_STEPID			DATA_INPUT_DEVICEID		+	sizeDEVICEID/2			
#define DATA_INPUT_PPID				DATA_INPUT_STEPID		+	sizeSTEPID/2				
#define DATA_INPUT_THICKNESS		DATA_INPUT_PPID			+	sizePPID/2					
#define DATA_INPUT_INSPFLG			DATA_INPUT_THICKNESS	+	sizeTHICKNESS/2			
#define DATA_INPUT_PANELSIZE		DATA_INPUT_INSPFLG		+	sizeINSPFLG/2			
#define DATA_INPUT_PANELPOS			DATA_INPUT_PANELSIZE	+	sizePANELSIZE/2				
#define DATA_INPUT_COUNT1			DATA_INPUT_PANELPOS		+	sizePANELPOS/2			
#define DATA_INPUT_COUNT2			DATA_INPUT_COUNT1		+	sizeCOUNT1/2			
#define DATA_INPUT_GRADE			DATA_INPUT_COUNT2		+	sizeCOUNT2/2			
#define DATA_INPUT_COMMENT			DATA_INPUT_GRADE		+	sizeGRADE/2				
#define DATA_INPUT_COMPSIZE			DATA_INPUT_COMMENT		+	sizeCOMMENT/2			
#define DATA_INPUT_READFLG			DATA_INPUT_COMPSIZE		+	sizeCOMPSIZE/2			
#define DATA_INPUT_PANELSTATE		DATA_INPUT_READFLG		+	sizeREADFLG	/2		
#define DATA_INPUT_JUDGEMENT		DATA_INPUT_PANELSTATE	+	sizePANELSTATE/2			
#define DATA_INPUT_CODE				DATA_INPUT_JUDGEMENT	+	sizeJUDGEMENT	/2		
#define DATA_INPUT_RUNLINE			DATA_INPUT_CODE			+	sizeCODE		/2		
#define DATA_INPUT_UNIQUEID			DATA_INPUT_RUNLINE		+	sizeRUNLINE		/2	
#define DATA_INPUT_PAIR_HID			DATA_INPUT_UNIQUEID		+	sizeUNIQUEID	/2		
#define DATA_INPUT_PAIR_EID			DATA_INPUT_PAIR_HID		+	sizePAIR_HID	/2		
#define DATA_INPUT_PAIR_GRADE		DATA_INPUT_PAIR_EID		+	sizePAIR_EID	/2		
#define DATA_INPUT_PAIR_UNIQUEID	DATA_INPUT_PAIR_GRADE	+	sizePAIR_GRADE	/2		
#define DATA_INPUT_FLOWRECIPE		DATA_INPUT_PAIR_UNIQUEID+	sizePAIR_UNIQUEID/2		
#define DATA_INPUT_RESERVED			DATA_INPUT_FLOWRECIPE	+	sizeFLOWRECIPE	/2		
#define DATA_INPUT_BITSIGNAL		DATA_INPUT_RESERVED		+	sizeRESERVED	/2		
#define DATA_INPUT_REFERDATA		DATA_INPUT_BITSIGNAL	+	sizeBITSIGNAL	/2		
#define DATA_INPUT_RAWPATH			DATA_INPUT_REFERDATA	+	sizeREFERDATA	/2		
#define DATA_INPUT_SUMPATH			DATA_INPUT_RAWPATH		+	sizeRAWPATH		/2	
#define DATA_INPUT_IMGPATH			DATA_INPUT_SUMPATH		+	sizeSUMPATH		/2	
#define DATA_INPUT_DISK				DATA_INPUT_IMGPATH		+	sizeIMGPATH		/2	
// WRITE ADDRESS
#define DATA_OUTPUT_HPANELID		DATA_OUTPUT_START
#define DATA_OUTPUT_EPANELID		DATA_OUTPUT_HPANELID	+	sizeHPANELID	/2		
#define DATA_OUTPUT_BATCHID			DATA_OUTPUT_EPANELID	+	sizeEPANELID	/2		
#define DATA_OUTPUT_PRODTYP			DATA_OUTPUT_BATCHID	+	sizeBATCHID			/2
#define DATA_OUTPUT_PRODKND			DATA_OUTPUT_PRODTYP	+	sizePRODTYP			/2
#define DATA_OUTPUT_DEVICEID		DATA_OUTPUT_PRODKND	+	sizePRODKND			/2
#define DATA_OUTPUT_STEPID			DATA_OUTPUT_DEVICEID	+	sizeDEVICEID	/2	
#define DATA_OUTPUT_PPID			DATA_OUTPUT_STEPID	+	sizeSTEPID			/2	
#define DATA_OUTPUT_THICKNESS		DATA_OUTPUT_PPID	+	sizePPID			/2		
#define DATA_OUTPUT_INSPFLG			DATA_OUTPUT_THICKNESS +	sizeTHICKNESS		/2	
#define DATA_OUTPUT_PANELSIZE		DATA_OUTPUT_INSPFLG	+	sizeINSPFLG			/2
#define DATA_OUTPUT_PANELPOS		DATA_OUTPUT_PANELSIZE	 +	sizePANELSIZE	/2			
#define DATA_OUTPUT_COUNT1			DATA_OUTPUT_PANELPOS	+	sizePANELPOS	/2		
#define DATA_OUTPUT_COUNT2			DATA_OUTPUT_COUNT1	+	sizeCOUNT1			/2
#define DATA_OUTPUT_GRADE			DATA_OUTPUT_COUNT2	+	sizeCOUNT2			/2
#define DATA_OUTPUT_COMMENT			DATA_OUTPUT_GRADE	+	sizeGRADE			/2	
#define DATA_OUTPUT_COMPSIZE		DATA_OUTPUT_COMMENT	+	sizeCOMMENT			/2
#define DATA_OUTPUT_READFLG			DATA_OUTPUT_COMPSIZE	+	sizeCOMPSIZE	/2		
#define DATA_OUTPUT_PANELSTATE		DATA_OUTPUT_READFLG	+	sizeREADFLG			/2
#define DATA_OUTPUT_JUDGEMENT		DATA_OUTPUT_PANELSTATE +	sizePANELSTATE	/2		
#define DATA_OUTPUT_CODE			DATA_OUTPUT_JUDGEMENT +	sizeJUDGEMENT		/2	
#define DATA_OUTPUT_RUNLINE			DATA_OUTPUT_CODE	+	sizeCODE			/2	
#define DATA_OUTPUT_UNIQUEID		DATA_OUTPUT_RUNLINE	+	sizeRUNLINE			/2
#define DATA_OUTPUT_PAIR_HID		DATA_OUTPUT_UNIQUEID	+	sizeUNIQUEID	/2		
#define DATA_OUTPUT_PAIR_EID		DATA_OUTPUT_PAIR_HID	+	sizePAIR_HID	/2		
#define DATA_OUTPUT_PAIR_GRADE		DATA_OUTPUT_PAIR_EID	+	sizePAIR_EID	/2		
#define DATA_OUTPUT_PAIR_UNIQUEID	DATA_OUTPUT_PAIR_GRADE +	sizePAIR_GRADE	/2		
#define DATA_OUTPUT_FLOWRECIPE		DATA_OUTPUT_PAIR_UNIQUEID +	sizePAIR_UNIQUEID/2		
#define DATA_OUTPUT_RESERVED		DATA_OUTPUT_FLOWRECIPE +	sizeFLOWRECIPE	/2		
#define DATA_OUTPUT_BITSIGNAL		DATA_OUTPUT_RESERVED	+	sizeRESERVED	/2		
#define DATA_OUTPUT_REFERDATA		DATA_OUTPUT_BITSIGNAL	+	sizeBITSIGNAL	/2		
#define DATA_OUTPUT_RAWPATH			DATA_OUTPUT_REFERDATA +	sizeREFERDATA		/2	
#define DATA_OUTPUT_SUMPATH			DATA_OUTPUT_RAWPATH	+	sizeRAWPATH			 /2
#define DATA_OUTPUT_IMGPATH			DATA_OUTPUT_SUMPATH	+	sizeSUMPATH			 /2
#define DATA_OUTPUT_DISK			DATA_OUTPUT_IMGPATH	+	sizeIMGPATH		/2


// LC GlassData
enum 
{
	LC_HPANELID			,	
	LC_EPANELID			,	
	LC_BATCHID			,	
	LC_PRODTYP			,	
	LC_PRODKND			,	
	LC_DEVICEID			,	
	LC_STEPID			,	
	LC_PPID				,	
	LC_THICKNESS		,	
	LC_INSPFLG			,	
	LC_PANELSIZE		,	
	LC_PANELPOS			,	
	LC_COUNT1			,	
	LC_COUNT2			,	
	LC_GRADE			,	
	LC_COMMENT			,	
	LC_COMPSIZE			,	
	LC_READFLG			,	
	LC_PANELSTATE		,	
	LC_JUDGEMENT		,	
	LC_CODE				,	
	LC_RUNLINE			,	
	LC_UNIQUEID			,	
	LC_PAIR_HID			,	
	LC_PAIR_EID			,	
	LC_PAIR_GRADE		,	
	LC_PAIR_UNIQUEID	,
	LC_FLOWRECIPE		,	
	LC_RESERVED			,	
	LC_BITSIGNAL		,	
	LC_REFERDATA		,	
	LC_RAWPATH			,	
	LC_SUMPATH			,	
	LC_IMGPATH			,	
	LC_DISK				,
	LC_OPID				,
	MAX_LC_GLASSDATA
};
