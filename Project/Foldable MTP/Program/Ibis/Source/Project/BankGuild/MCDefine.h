////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// MCDefine : 현재 시스템에 특화된 정의 파일들
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

// Y : 영우 (A : ANI)
// M : Main Program. (L : Loader, U : Unloader, D : Data)
// MTP : MTP설비 (MT : MT설비, FT : FT설비)
#define PROGRAM_VERSION				_T("Y_M_MTP_18.05.31.01")		// Program Version

// Defect Circle을 Cell에 표시하기위한 색 지정
#define COLOR_PTN_RED				RGB(255, 0, 0)
#define COLOR_PTN_GREEN				RGB(0, 255, 0)
#define COLOR_PTN_BLUE				RGB(0, 0, 255)
#define COLOR_PTN_WHITE				RGB(255, 255, 255)
#define COLOR_PTN_GRAY32			RGB(32, 32, 32)
#define COLOR_PTN_GRAY75			RGB(75, 75, 75)
#define COLOR_PTN_GRAY87			RGB(87, 87, 87)
#define COLOR_PTN_GRAY127			RGB(127, 127, 127)
#define COLOR_PTN_BLACK				RGB(0, 0, 0)


// // 각종 파일이름
#define NAME_FILE_DEFECT_LIST		_T("DefectCode_BZone.ini")				// Defect list (작업자 입력)
#define NAME_FILE_LOSSTIME_LIST		_T("LosstimeList.ini")			// 비가동 항목 list
#define NAME_FILE_USER_LIST			_T("UserList.par")				// 작업자 계정관리 List

// 
// 현재 System의 각종 메세지 정의, User Defined Message ...
#define UDMSG_MESSAGE_BASE				(WM_USER + 5000)					// 현재 시스템에서 사용할 사용자정의 메세지 Base
#define UDMSG_STATE_CHANGE				(UDMSG_MESSAGE_BASE + 1)		// 시스템의 상태가 변경되었다.
#define UDMSG_COMM_STATE				(UDMSG_MESSAGE_BASE + 4)		// 통신 연결 상태가 변경되었다.
// 
#define UDMSG_DISP_LAN_CHANGE			(UDMSG_MESSAGE_BASE + 40)		//  프로그램 표시언어가 변화되었다.
#define UDMSG_DISP_QUAL_WARNING			(UDMSG_MESSAGE_BASE + 41)		//  품질경고 화면을 보여주도록 한다.
#define UDMSG_CIM_STATE_CHANGE			(UDMSG_MESSAGE_BASE + 42)		//  CIM 상태가 변경되었다.
#define UDMSG_CIM_EQIP_ID_CHANGE			(UDMSG_MESSAGE_BASE + 43)		//  CIM Eqip ID가 변경되었다.
#define UDMSG_COUNT_UPDATE				(UDMSG_MESSAGE_BASE + 44)		
#define MSG_MCR_READ_UPDATE				(UDMSG_MESSAGE_BASE + 45)		
//kjpark 20170831 Interlock, Opcoall, Terminal UI 및 기능 구현
#define MSG_CIM_MESSAGE_UPDATE			(UDMSG_MESSAGE_BASE + 46)	

#define MSG_WARNING_SHOW				(UDMSG_MESSAGE_BASE + 47)		

// MainFrame에 등록된 모달리스 창을 표시하기 위한 메세지
#define MSG_SHOW_DIALOG					(UDMSG_MESSAGE_BASE + 48)		
// 
// // Defect 데이터를 저장하는 구조체, ini file로 된 defect list를 읽어와서 프로그램 내부에서 사용된다.
// // FormDefect, DlgDefectSelect, DlgDefectEdit 참조.
#define DELIMITER_DEFECT				'>'								// Defect 및 LossTime 설정값 관련 문자열 분리 구분자


// 비가동 로그에서 사용되는 비가동 사유 
// 최 하위 3rdClass 데이터
// PM>000>Shift점검>프로그램 점검>Check Program
typedef struct _TLosstimeData {
	// enum { LAN_MAX = 2 };		// 최대 지원 언어

	CString	sCode;						// 비가동 Code, 현재는 미사용, 향후 사용예정
	CString	sLine;						// 데이터 원본, 단축버튼에 활용
	CString	sReason[LAN_MAX];	// 분리된 비가동 사유 이름

	_TLosstimeData() { 
		sCode			= _T("");
		sLine			= _T("");
		for (int i=0; i < LAN_MAX; i++) sReason[i] = _T("");
	};

	// 대입연산자
	_TLosstimeData& operator = (const _TLosstimeData &p) {
		sCode = p.sCode;
		sLine	= p.sLine;
		for(int i=0; i<LAN_MAX; i++) sReason[i] = p.sReason[i];
		return *this;
	};

} TLosstimeData;


// 중간 2nd Class Data
typedef struct _TLoss2ndClass {
	enum {	//LAN_MAX = 2,			// 최대 지원 언어
				eMAX_3rdCount = 24,	};		// 하위 항목 수

	CString	sLine;						// 데이터 원본, 단축버튼에 활용
	CString	sName[LAN_MAX];	// 2nd Class name

	UINT		n3rdCount;								// 3rd class 수
	TLosstimeData r3rdClass[eMAX_3rdCount];	// 3rd Class data

	_TLoss2ndClass() {			// 초기화
		sLine			= _T("");
		n3rdCount	= 0;
		for (int i=0; i < LAN_MAX; i++) sName[i] = _T("");
	};

} TLoss2ndClass;


// 1st Class Data
typedef struct _TLoss1stClass {
	enum {	eMAX_2ndCount = 8,	};				// 하위 항목 수

	CString	sLine;									// 데이터 원본, 단축버튼에 활용
	CString	sName;									// 1st Class name

	UINT		n2ndCount;								// 2nd class 수
	TLoss2ndClass r2ndClass[eMAX_2ndCount];	// 2nd Class data

	_TLoss1stClass() {			// 초기화
		sLine			= _T("");
		sName			= _T("");
		n2ndCount	= 0;
	};

} TLoss1stClass;


// 비가동 사유를 그룹별로 관리하기위한 구조체
typedef struct _TLosstimeList {
	enum {	eMAX_1stCount			= 8,			// 최대 8개 1st class 그룹	
				eMAX_ShortcutCount	= 6,			// 단축 버튼 수량, 6개
	};		

	//2014-12-24, jhLee TPSystem Code 보고시 필요
	// 자동으로 처리해하는 3가지 예외 처리
	CString			sAutoRunDownCode;						// AUTO RUN DOWN 선택 항목
	CString			sOPLossCode;							// OP LOSS 선택 항목
	CString			sTactTimeOver;							// TATC TIME OVER 선택 항목
	CString			sCellInputOver;							// 자재 없음 항목


	UINT			nShortcutCount;						// 단축 버튼 정의 수량
	TLosstimeData	rShortcut[ eMAX_ShortcutCount ];	// 단축 버튼 defect 저장

	UINT			nCount_1st;								// 1st class 그룹 수
	TLoss1stClass	rLosstime[eMAX_1stCount];			// 1st Class의 Data

	_TLosstimeList() {	nShortcutCount = 0; nCount_1st = 0;	};		// 초기화
	TLosstimeData*	GetDefectData(int n1st, int n2nd, int n3rd) { return &(rLosstime[n1st].r2ndClass[n2nd].r3rdClass[n3rd]); };		// losstime Data pointer 획득
	TLosstimeData*	GetShortcut(int nIdx) { return &(rShortcut[nIdx]); };		// Shortcut Losstime Data pointer 획득

} TLosstimeList;


// 비가동 Log를 남기기 위한 데이터 기록
typedef struct _TLosstimeRecord {

	BOOL			bPrevDay;						// TRUE:강제로 이전 날짜로 기록한다, FALSE:오늘 날짜로 기록한다.
	int				nMode;							// 현재 비가동 기록 모드 0: STOP으로 전환, 1:Auto로 전환

	// 선택된 내용들
	CString			sReasonLine;					// 선택된 전체 Code 내용
	CString			s1stClass;						// 각 Class별 선택 내용
	CString			s2ndClass;
	CString			s3rdClass;
	CString			MesCode;						// 상위 보고용 코드, 현재 미사용
	CString			sID;								// Stop 시킨 작업자 ID
	CString			sIDNo;							// 작업자 사원번호;
	CString			sAlarmType;						// Alarm 여부, 중/경 알람여부
	CString			sAlarmCode;						// Alarm Code
	CString			sRepairCode;					// MD/BM의 경우 조치내역
	CString			sUserMsg;						// 사용자 정의 입력
	float			fWaitTime;						// 알람발생 ~ 고장조치 시작까지
	float			fRepairTime;					// 실제 조치가 일어난 시간
	float			fDuration;						// 실제 설비가 멈춘 시간

	//2014-09-11 추가 (백윤기S 요청)
	CString			sTPSystemName;					// TP System loss name
	CString			sTPSystemCode;					// TP System Loss Code(sdefaultCellInfomation.MesCode로 대체. 현재 미사용)

	SYSTEMTIME		tmStartTime;					// 정지 시작 시각
	SYSTEMTIME		tmResetTime;					// 인지 시각
	SYSTEMTIME		tmEndTime;						// 정지 종료시각

	// ini file로 저장하기 위한 변수
	CString			sStartTime;						// 문자열로 변경된 시작 시간
	CString			sResetTime;						// 문자열로 변경된 인지 시간

	double			dbAddLossTime;					// 추가적인 미가동 시간

	// STOP시 입력한 비가동창을 START시 다시 띄우게 하기 위한 index
	int				n1stClass;
	int				n2ndClass;
	int				n3rdClass;

	void Clear()
	{
		bPrevDay		= FALSE;						//  FALSE:오늘 날짜로 기록한다.
		nMode			= 0;							// 현재 비가동 기록 모드 0: STOP으로 전환, 1:Auto로 전환
	
		sReasonLine		= _T("");					// 선택된 전체 Code 내용
		s1stClass		= _T("");					// 각 Class별 선택 내용
		s2ndClass		= _T("");
		s3rdClass		= _T("");
		MesCode			= _T("");					// 상위 보고용 코드, 현재 미사용
		sID				= _T("");					// Stop 시킨 작업자 ID
		sIDNo				= _T("");					// 작업자 사원번호;
		sAlarmType		= _T("");					// Alarm 여부, 중/경 알람여부
		sAlarmCode		= _T("");					// Alarm Code
		sRepairCode		= _T("");					// MD/BM의 경우 조치내역
		sUserMsg			= _T("");					// 사용자 정의 입력
		fWaitTime		= 0.0;						// 알람발생 ~ 고장조치 시작까지
		fRepairTime		= 0.0;						// 실제 조치가 일어난 시간
		fDuration		= 0.0;						// 실제 설비가 멈춘 시간

		sTPSystemName	= _T("UNDEFINDED");					// TP System loss name
		sTPSystemCode	= _T("UNDEFINDED");					// TP System Loss Code

		::GetLocalTime( &tmStartTime );			// 정지 시작 시각
		::GetLocalTime( &tmResetTime );			// 인지 시각
		::GetLocalTime( &tmEndTime );				// 정지 종료시각

		sStartTime		= _T("");					// 문자열로 변경된 시작 시간
		sResetTime		= _T("");					// 문자열로 변경된 인지 시간

		dbAddLossTime	= 0.0;						// 추가적인 미가동 시간

		n1stClass = -1;
		n2ndClass = -1;
		n3rdClass = -1;
	};

	_TLosstimeRecord () { Clear(); };				// 생성자

} TLosstimeRecord;





/*	----------------------------------------------- */
//2014/10/08 JH-PARK 

// 최 하위 3rdClass 데이터
typedef struct _TRepairAlarmData {
	// enum { LAN_MAX = 2 };		

	CString	sCode;						
	CString	sLine;						
	CString	sReason[LAN_MAX];	

	_TRepairAlarmData() { 
		sCode			= _T("");
		sLine			= _T("");
		for (int i=0; i < LAN_MAX; i++) sReason[i] = _T("");
	};

	// 대입연산자
	_TRepairAlarmData& operator = (const _TRepairAlarmData &p) {
		sCode = p.sCode;
		sLine	= p.sLine;
		for(int i=0; i<LAN_MAX; i++) sReason[i] = p.sReason[i];
		return *this;
	};
} TRepairAlarmData;


// 중간 2nd Class Data
typedef struct _TRepair2ndClass {
	enum {	// LAN_MAX = 2,			// 최대 지원 언어
		eMAX_3rdCount = 24,	};		// 하위 항목 수

	CString	sLine;						// 데이터 원본, 단축버튼에 활용
	CString	sName[LAN_MAX];	// 2nd Class name

	UINT		n3rdCount;								// 3rd class 수
	_TRepairAlarmData r3rdClass[eMAX_3rdCount];	// 3rd Class data

	_TRepair2ndClass() {			// 초기화
		sLine			= _T("");
		n3rdCount	= 0;
		for (int i=0; i < LAN_MAX; i++) sName[i] = _T("");
	};

} TRepair2ndClass;

// 1st Class Data
typedef struct _TRepair1stClass {
	enum {	eMAX_2ndCount = 8,	};				// 하위 항목 수

	CString	sLine;									// 데이터 원본, 단축버튼에 활용
	CString	sName;									// 1st Class name

	UINT		n2ndCount;								// 2nd class 수
	TRepair2ndClass r2ndClass[eMAX_2ndCount];	// 2nd Class data

	_TRepair1stClass() {			// 초기화
		sLine			= _T("");
		sName			= _T("");
		n2ndCount	= 0;
	};

} TRepair1stClass;

// Alarm 조치사유를 그룹별로 관리하기위한 구조체
typedef struct _TRepairAlarmList {
	enum {	eMAX_1stCount			= 8,			// 최대 8개 1st class 그룹	
				eMAX_ShortcutCount	= 6,			// 단축 버튼 수량, 6개
			};		

	UINT				nShortcutCount;						// 단축 버튼 정의 수량
	TRepairAlarmData	rShortcut[ eMAX_ShortcutCount ];	// 단축 버튼 defect 저장

	UINT				nCount_1st;								// 1st class 그룹 수
	TRepair1stClass	rRepairAlarm[eMAX_1stCount];			// 1st Class의 Data

	_TRepairAlarmList() {	nShortcutCount = 0; nCount_1st = 0;	};		// 초기화
	TRepairAlarmData*	GetDefectData(int n1st, int n2nd, int n3rd) { return &(rRepairAlarm[n1st].r2ndClass[n2nd].r3rdClass[n3rd]); };		// losstime Data pointer 획득
	TRepairAlarmData*	GetShortcut(int nIdx) { return &(rShortcut[nIdx]); };		// Shortcut Losstime Data pointer 획득

} TRepairAlarmList;




// Alarm 발생 내역을 Log 기록하기위한 구조체
typedef struct _AlarmRecord {

	CString			sCode;							// Alarm Code
	CString			sName;							// Alarm 내용
	CString			sParts;							// 발생 부분
	int				nLevel;							// Alarm Level

	SYSTEMTIME		tmStartTime;					// Alarm 발생 시각 시작 시각
	SYSTEMTIME		tmResetTime;					// Alarm 인지 시각
	SYSTEMTIME		tmLossStartTime;				// Loss time기록용 Alarm 발생 시각 
} TAlarmRecord;


// NG Code list와 Defect code와의 매칭을 위한 Table 구조체
typedef struct _TDefectNameTable
{
	CString		sKey;						// 검색할 때 사용되는 Key값, 공백제거, 대문자 변환

	CString		sName1;					// 파일에서 읽어들인 Defect 글자
	CString		sName2;					// 영문 및 한글 defect
	CString		sName3;					
	CString		sCode;					// MES 보고용 Defect Code

} TDefectNameTable;
 typedef CTypedPtrMap<CMapStringToPtr, CString, TDefectNameTable*> CMapStringToDefectName;


// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==

//
// SysWarning에서 사용하는 경보 Defect data
//
// 품질 경고용 개별 Defect 정보
typedef struct _DefectItem {
	CString		sName;								// Defect Name, 검사PC에서 보내온 
	CString		sCode;								// Defect Code, Defect List에서 검색을 통해 찾아올 수 있다.

	_DefectItem() { sName = _T(""); sCode = _T(""); };
} TDefectItem;


// 품질 경고용 자료구조

// 구간별 영역 자료구조
typedef struct _QualArea {

	int			nBoundX1;							// X축 경계 시작
	int			nBoundX2;							// X축 경계 끝
	int			nBoundY1;							// Y축 경계 시작
	int			nBoundY2;							// Y축 경계 끝

	CString		sLastCellID;						// 최근에 Defect이 발생한 Cell ID, 동일 영역에 대한 중복 누적 방지
	int			nErrorCount;						// 발생한 Defect 건수 보관

	int			nAlertLevel;						// 경고 발생 수위, 0:Normal, 1:주의, 2:경고, 3:위험
	SYSTEMTIME	tmEvent;				// 경보가 발생된 시각

	//d CList <TDefectItem, TDefectItem> lstDefectItems;		// 발생된 Defect 지정

	void Clear() { 
		sLastCellID = _T("");	
		nErrorCount = 0;
		nAlertLevel = 0;
	//d lstDefectItems.RemoveAll(); 
	};

	void ClearBound()			// 영역을 초기화 한다.
	{
		nBoundX1 = 0; 
		nBoundY1 = 0; 
		nBoundX2 = 0; 
		nBoundY2 = 0; 
	};

	_QualArea() 
	{  
		ClearBound();
		Clear(); 
	};

	~_QualArea() 
	{ 
		// lstDefectItems.RemoveAll(); 		
	};

} TQualArea;



// 품질 검사를 위한 지정 Defect 관련 데이터 관리 
typedef struct _QualItem {
	CString		sCode;					// Defect Code
	CString		sName[LAN_MAX];	// 언어별 Defect 이름
	CString		sStepName;				// Feedback해줄 공정명

	int			nMode;					// 동작 모드, 0:미사용, 1:발생횟수로 경보, 2:발생율 %로 경보
	int			nLimitCount;			// 경보를 울리기위한 제한 값, 발생횟수
	int			nLimitRatio;			// 경보를 울리기 위한 제한 값, 발생 비율 %

	// 실행중의 누적 데이터, 별도의 파일로 관리한다.
	int			nIndex;					// 파일 저장관리를 위한 Index

	int			nAlertLevel;			// 경고 발생 수위, 0:Normal, 1:주의, 2:경고, 3:위험
	int			nCount;					// 발생한 빈도수
	double		dbRatio;				// 계산된 발생률

	SYSTEMTIME	tmEvent;				// 경보가 발생된 시각

	// 발생 횟수를 초기화 한다.
	void ClearCount()					// 발생 빈도수 Clear
	{
		nCount		= 0;
		nAlertLevel = 0; 
	};

	// 사용여부를 초기화하여 해당 항목을 비운다.
	void ClearItem()
	{
		sCode.Empty();					// 내용 삭제
		sStepName.Empty();
		for (int i=0; i<LAN_MAX; i++) sName[i].Empty();

		nMode = 0;						// 미사용 설정
		nLimitCount = 3;
		nLimitRatio = 3;

		ClearCount();					// 발생 내역도 초기화 한다.
	};
} TQualItem;
typedef CArray<TQualItem, TQualItem&> TArrayQualItem;				// 점검대상 Defect 관리용 CArray


// 품질경보 메세지를 보관하기 위한 구조체
typedef struct _QualMsg {

	int		nMode;						// 어떠한 경보의 종류인가 ?
	int		nIndex1;					// 발생한 곳의 Index, Area인경우 X축 Index, Defect 인 경우 해당 Defect의 배열 Index
	int		nIndex2;					// Area의 Y축 Index

	CString	sName;						// Area 명 or Defect 명
	CString sStep;						// Step 명
	int		nCount;						// 발생한 수치 : 카운트 혹은 불랼율
	int		nReason;					// 원인, 0 : 카운트 초과 / 1 : 불량률 초과

	CString		sMsg;					// 경고 내용
	SYSTEMTIME	tmEvent;				// 경보가 발생된 시각

	_QualMsg () {
		nMode		= 0;
		nIndex1		= 0;
		nIndex2		= 0;

		sMsg.Empty();
	};

} TQualMsg;
typedef CArray<TQualMsg, TQualMsg&> TArrayQualMsg;					// 경고 메세지 관리용 CArray


// C-Zone에서 검출한 Defect 정보를 저장하기 위한 구조체, 
// CellInfo에서 선언한 뒤 SysVIMonitor에서 정보를 누적시킨다.
typedef struct _QualDefect 
{
	int x;
	int y;

	CString DefectName;				// 해당 Defect의 이름
} TQualDefect;
typedef CList<TQualDefect, TQualDefect&> TListQualDefect;


// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==




// 기존 GAON ConstCollection.h에서 발췌
// 판정에 대한 정의
struct CONST_JUDGE_NAME
{
	CString strName;

	enum ID
	{
		JUDGE_MCR = 0,
		JUDGE_CELL_LOADING,
		JUDGE_MTP_WRITE,
		JUDGE_MTP_VERIFY,
		JUDGE_WHITE_CURRENT,
		JUDGE_SLEEP_CURRENT,
		JUDGE_HLPM_CURRENT,
		JUDGE_TSP_START,
		JUDGE_EVT_VERSION_CHECK,
		JUDGE_TE_CHECK,
		JUDGE_ID_CHECK,
		JUDGE_OTP_REG_CHECK,
		JUDGE_ICT_TEST,
		JUDGE_COPR_ICT_TEST,
		JUDGE_POC_ERROR_CHECK,
		JUDGE_DDI_BLOCK_TEST,
		JUDGE_OPTION_CHECK2,
		JUDGE_OPTION_CHECK3,
		JUDGE_OPTION_CHECK4,
		JUDGE_OPTION_CHECK5,
		JUDGE_OPTION_CHECK6,
		JUDGE_OPTION_CHECK7,
		JUDGE_OPTION_CHECK8,
		JUDGE_OPTION_CHECK9,
		JUDGE_OPTION_CHECK10,
		JUDGE_PG_ALARM,
		//kjpark 20171010 TMD March 구현
		JUDGE_CELL_MIXING,
		JUDGE_FINAL_CLASS,
		JUDGE_MAX,
	};
};

const CONST_JUDGE_NAME CONST_JUDGE_LIST[] =
{
	_T("MCR")					,
	_T("CELL_LOADING")			,
	_T("MTP_WRITE")				,
	_T("MTP_VERIFY")			,
	_T("WHITE_CURRENT_CHECK")	,
	_T("SLEEP_CURRENT_CHECK")	,			
	_T("HLPM_CURRENT_CHECK")	,			
	_T("TSP_START")				,
	_T("EVT_VERSION_CHECK")		,
	_T("TE_CHECK")				,
	_T("ID_CHECK")				,
	_T("OTP_REG_CHECK")			,
	_T("ICT_TEST")				,
	_T("COPR_ICT_TEST")			,
	_T("POC_ERROR_CHECK")		,
	_T("DDI_BLOCK_TEST")		,
	_T("OPTION_CHECK2")			,
	_T("OPTION_CHECK3")			,
	_T("OPTION_CHECK4")			,
	_T("OPTION_CHECK5")			,
	_T("OPTION_CHECK6")			,
	_T("OPTION_CHECK7")			,
	_T("OPTION_CHECK8")			,
	_T("OPTION_CHECK9")			,
	_T("OPTION_CHECK10")		,
	_T("PG ALARM")				,
	_T("CELL_MIXING")			,
	_T("FINAL CLASS")	
};


//kjpark 20170831 MCR 구현 채널 별 구현 완료
enum SOCKET_TYPE
{	// 통신
	eCOMM_MyPC				= 0,			// 0. 운영PC, 내 PC이므로 신호기PC의 Server 설정할 때 사용한다.
	eCOMM_SignalPC,							// 2. 신호기 PC
	eCOMM_DataPC,							// 3. 데이터 PC 와의 통신
	//kjpark 20161006 MCR 상하 수정
	eCOMM_MCR_1,							// 4. UVMCR #1과의 통신
	eCOMM_MCR_2,
	eCOMM_MCR_3,
	eCOMM_MCR_4,
	eCOMM_MCR_5,
	eCOMM_MCR_6,
	eCOMM_AccuraGPS,						// 6, GPS 전력량계 Accura 1
	eCOMM_AccuraUPS,						// 7, UPS 전력량계 Accura 2
	eMAX_COMMCount							//12. TCP/IP 통신 수량

};

enum {				// CIM 및 각종 Cell관련 정보를 처리하기 위한 상수 선언

	//kjpark2017/09/01 Serial 정리 - Temp 추가, 라벨 2 삭제, 토크 삭제, 메인에어
	//kjpark2017/09/21 온도센서 변경으로 통신 프로토콜 변경 테스트 중
	//kjpark 20170922 온도센서 통신 검증
	eIDX_TEMP_UTIL_BOX = 0,
	eIDX_TEMP_ETC_BOX,
	eIDX_TEMP_SERVO_BOX,				// PC-RACK 온도 센서
	eIDX_TEMP_CTRL_PC_BOX,				// PC-RACK 온도 센서
	eIDX_TEMP_PG_PC_BOX,
	eMAX_TEMPCount,						// 설치된 온도계 수량 (현재, 1개)

	eUSER_NONE			= 0,				// Login을 하지 않았다.
	eUSER_Operator,						// Operator level
	eUSER_Engineer,						// Engineer level
	eUSER_SuperUser,						// 최상위 권한, Setup시 사용

	// 사용자 로그인시 입력 글자 수 제한
	eMAX_UserIDLength	= 8,			// 사번 8자리 이내 입력

	// Label Printer Buffer 크기
	eMAX_PrinterBuffer		= 256,	// 256 bytes

	// 작업조 (Shift)
	eIDX_Shift_DAY			= 0,							// 오전 작업조 : 06 ~ 14
	eIDX_Shift_SW,											// 오후 작업조 : 14 ~ 22
	eIDX_Shift_GY,											// 야간 작업조 : 22 ~ 06
	eIDX_Shift_Today,										// 오늘 작업한 모든 내용
	eMAX_ShiftCount
};

// 작업자 계정관리용
class CUserAccount
{
public:
	CString		sID;						// 작업자 ID, 사번?
	CString		sPassword;				// 암호
	int			nLevel;					// eUSER_Operator, eUSER_Engineer, eUSER_SuperUser
	CString		sKorName;				// 한글이름
	CString		sEngName;				// 영어이름
	CString		sIDNo;					// 사원번호;
	

	CUserAccount () {	
		Init();// 생성자
	};
	void Init()
	{
		sID			= _T("");
		sPassword	= _T("");
		nLevel		= 0;
		sKorName		= _T("");
		sEngName		= _T("");
		sIDNo			= _T("");
	}
};


typedef CTypedPtrMap<CMapStringToPtr, CString, CUserAccount*> CMapStringToUserAccount;


typedef CTypedPtrMap<CMapStringToPtr, CString, CUserAccount*> CMapUserAccount;


// D-Zone에서 작업자의 Defect 입력을 위한 구조체
struct TDefectData {

	CString	sCode;						// Defect Code
	CString	sLine;						// 데이터 원본
	CString	sDefect[LAN_MAX];	// 분리된 Defect 이름

	TDefectData() { 
		sCode			= _T("");
		sLine			= _T("");
		for (int i=0; i < LAN_MAX; i++) sDefect[i] = _T("");
	};

};


// Defect을 그룹별로 관리하기위한 구조체
struct TDefectList {
	enum {	eMAX_GoupCount			= 12,								// 최대 12개 Defect 그룹	
		eMAX_DefectCount		= 40,								// 그룹당 최대 40개의 defet 존재
		eMAX_ShortcutCount		= 5,								// 단축 defect 수량, 8개
	};		

	UINT			nShortcutCount;										// 단축 버튼 정의 수량
	TDefectData		rShortcut[ eMAX_ShortcutCount ];					// 단축 버튼 defect 저장

	UINT			nGroupCount;										// 그룹 수
	CString			sGroupName[eMAX_GoupCount][LAN_MAX];			// 각 그룹의 이름
	UINT			nDefectCount[eMAX_GoupCount];						// 각 그룹당 Defect  수량
	TDefectData		rDefectData[eMAX_GoupCount][eMAX_DefectCount];		// defect data가 보관되는 배열

	TDefectData*	GetDefectData(int nGroup, int nIdx) { return &(rDefectData[nGroup][nIdx]); };		// Defect Datd pointer 획득
	TDefectData*	GetShortcut(int nIdx) { return &(rShortcut[nIdx]); };		// Shortcut Defect Datd pointer 획득

	TDefectList() 	// 초기화
	{	
		nShortcutCount = 0; 
		nGroupCount = 0;
		for (int i=0; i<eMAX_GoupCount; i++) 
			nDefectCount[i] = 0;	
	};	

};
