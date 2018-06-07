//==================================================================================================================
//
// ȭ�� ǥ�� ó���� �ϸ鼭 �����ϴ� ���� ��� 
//
//==================================================================================================================


#pragma once


// ��/���̾�α� ȭ�� ��ȯ�� �ʿ��� ���� ��� �Ϸ� ��ȣ ����
enum {

	// CFormView, ���� ȭ�� �� 
	eFORM_Default			= 0,			// ���α׷� ���۽� �⺻������ ������ �ִ�  CFormView
	eFORM_Main,								// Main ȭ��
	eFORM_Alarm,							// Alarm ǥ�� ȭ��
	eFORM_PM,								// PM / ��� ȭ��	
	eFORM_Recipe,							// [W] Recipe ȭ��
	eFORM_Report1,							// ���� Report ǥ�� ȭ��
	eFORM_Report2,							
	eFORM_Report3,							
	eFORM_Report4,							
	//kjpark 20170928 OPCall, Interlock. terminel MSG �����丮 ���� ����
	eFORM_Report5,
	eFORM_Report6,
	//kjpark 20180114 NG ���� ��� UI �߰�
	eFORM_Report7,
	eFORM_Teach_Shuttle_1,
	eFORM_Teach_Shuttle_2,
	eFORM_Teach_Shuttle_3,
	eFORM_Teach_PDT_IF,					// PDT Address [10/2/2017 OSC]
	eFORM_Setup1,							// ����̽� ���� ȭ��
	eFORM_Setup2,
	eFORM_Setup3,
	eFORM_Setup4,
	eFORM_Config_Option,				//Options	
	eFORM_Config_Lamp,				//	Lamp	
	eFORM_Config_CIM,				// CIM	
	eFORM_Config_System,	
	eFORM_Config_NG_Option,		
	eMAX_FORMCount,


	// CDialogBar, ȭ��
	eDLGBAR_None			= 0,			// ���� ����.
	eDLGBAR_Top,							// ���� ����ǥ�� 
	eDLGBAR_Bottom,						// ���� ���� �޴� ǥ��
	eDLGBAR_SubReport,						// Report ���� �޴� ����
	eDLGBAR_SubTeach,						// Teach ���� �޴� ����
	eDLGBAR_SubSetup,						// Setup ���� �޴� ����
	eDLGBAR_SubConfig,						// Config ���� �޴� ����
	eDLGBAR_Jog,							// Motion Jog ���� 
	eMAX_DLGBARCount,
	

	// CDialog, ��ȭ���� 
	eDLG_None				= 0,			// ���� ����. (����)
	eDLG_Confirm,							// ������� Ȯ���� �޴� ȭ��
	eDLG_Inform,							// ���� ���� ǥ�� ȭ��
	eDLG_Initial,							// �ʱ�ȭ ���� ��Ȳ ǥ�� ȭ��
	eDLG_MsgInterlock,					// ���� Interlock �޼��� ǥ�� ȭ��
	eDLG_MsgUnitInterlock,					// ���� UNIT Interlock �޼��� ǥ�� ȭ��
	eDLG_MsgOPCall,						// ���� Operator Call �޼��� ǥ�� ȭ��
	eDLG_MsgTerminal,						// ���� Terminal �޼��� ǥ�� ȭ��
	eMAX_DLGCount,

};