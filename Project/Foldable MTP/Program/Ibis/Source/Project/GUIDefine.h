//==================================================================================================================
//
// 화면 표시 처리를 하면서 정의하는 각종 상수 
//
//==================================================================================================================


#pragma once


// 폼/다이얼로그 화면 전환에 필요한 공통 사용 일련 번호 선언
enum {

	// CFormView, 서브 화면 폼 
	eFORM_Default			= 0,			// 프로그램 시작시 기본적으로 가지고 있는  CFormView
	eFORM_Main,								// Main 화면
	eFORM_Alarm,							// Alarm 표시 화면
	eFORM_PM,								// PM / 경고 화면	
	eFORM_Recipe,							// [W] Recipe 화면
	eFORM_Report1,							// 각종 Report 표시 화면
	eFORM_Report2,							
	eFORM_Report3,							
	eFORM_Report4,							
	//kjpark 20170928 OPCall, Interlock. terminel MSG 히스토리 따로 관리
	eFORM_Report5,
	eFORM_Report6,
	//kjpark 20180114 NG 판정 통계 UI 추가
	eFORM_Report7,
	eFORM_Teach_Shuttle_1,
	eFORM_Teach_Shuttle_2,
	eFORM_Teach_Shuttle_3,
	eFORM_Teach_PDT_IF,					// PDT Address [10/2/2017 OSC]
	eFORM_Setup1,							// 디바이스 설정 화면
	eFORM_Setup2,
	eFORM_Setup3,
	eFORM_Setup4,
	eFORM_Config_Option,				//Options	
	eFORM_Config_Lamp,				//	Lamp	
	eFORM_Config_CIM,				// CIM	
	eFORM_Config_System,	
	eFORM_Config_NG_Option,		
	eMAX_FORMCount,


	// CDialogBar, 화면
	eDLGBAR_None			= 0,			// 지정 안함.
	eDLGBAR_Top,							// 상위 정보표시 
	eDLGBAR_Bottom,						// 하위 메인 메뉴 표시
	eDLGBAR_SubReport,						// Report 하위 메뉴 선택
	eDLGBAR_SubTeach,						// Teach 하위 메뉴 선택
	eDLGBAR_SubSetup,						// Setup 하위 메뉴 선택
	eDLGBAR_SubConfig,						// Config 하위 메뉴 선택
	eDLGBAR_Jog,							// Motion Jog 동작 
	eMAX_DLGBARCount,
	

	// CDialog, 대화상자 
	eDLG_None				= 0,			// 지정 안함. (고정)
	eDLG_Confirm,							// 사용자의 확인을 받는 화면
	eDLG_Inform,							// 각종 정보 표시 화면
	eDLG_Initial,							// 초기화 진행 상황 표시 화면
	eDLG_MsgInterlock,					// 상위 Interlock 메세지 표시 화면
	eDLG_MsgUnitInterlock,					// 상위 UNIT Interlock 메세지 표시 화면
	eDLG_MsgOPCall,						// 상위 Operator Call 메세지 표시 화면
	eDLG_MsgTerminal,						// 상위 Terminal 메세지 표시 화면
	eMAX_DLGCount,

};