#include "StdAfx.h"
#include "CellBank.h"
#include "Etc\FileSupport.h"
#include "Device\DeviceSocket_PGHost.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CCellInfo::Init()
{
	defaultData.Init();

	m_UnloadTactTime.Init();
	m_TactTime.Init();
	m_WaitTime.Init();
	m_LoadingTactTime.Init();
	m_TableTurnTime.Init();
	m_MCRReadTime.Init();
	m_AZoneETCTime.Init();
	m_MTPReadyTime.Init();

	m_AZone.Init();
	m_AZone.m_strCommand = SET_ZONE_A;

	//kjpark 20170912 MCR 위치에따른 택타임 추가
	m_AZonetoMCRZone.Init();
	m_MCRZonetoBZone.Init();
	m_BZonetoAZone.Init();

	m_BZone.Init();
	m_BZone.m_strCommand = SET_ZONE_C;

	m_CellLoading.Init();
	m_CellLoading.m_strCommand = CELL_LOADING;
	m_CellLoading.m_Judge = CONST_JUDGE_NAME::JUDGE_CELL_LOADING;
	m_CellLoading.FindWorkZone();

	m_MTPWrite.Init();
	m_MTPWrite.m_strCommand = MTP_WRITE_PRESCALE;
	m_MTPWrite.m_Judge = CONST_JUDGE_NAME::JUDGE_MTP_WRITE;
	m_MTPWrite.FindWorkZone();
	if(m_MTPWrite.m_strCommand == MTP_WRITE_PRESCALE)
		m_MTPWrite.m_strMTPType = _T("PRE");
	else
		m_MTPWrite.m_strMTPType = _T("NORMAL");

	m_MTPVerify.Init();
	m_MTPVerify.m_strCommand = MTP_VERIFY;
	m_MTPVerify.m_Judge = CONST_JUDGE_NAME::JUDGE_MTP_VERIFY;
	m_MTPVerify.FindWorkZone();

	m_WhiteCurrent.Init();
	m_WhiteCurrent.m_strCommand = WHITE_CURRENT_CHECK;
	m_WhiteCurrent.m_Judge = CONST_JUDGE_NAME::JUDGE_WHITE_CURRENT;
	m_WhiteCurrent.FindWorkZone();

	m_SleepCurrent.Init();
	m_SleepCurrent.m_strCommand = SLEEP_CURRENT_CHECK;
	m_SleepCurrent.m_Judge = CONST_JUDGE_NAME::JUDGE_SLEEP_CURRENT;
	m_SleepCurrent.FindWorkZone();

	m_HLPMCurrent.Init();
	m_HLPMCurrent.m_strCommand = HLPM_CURRENT_CHECK;
	m_HLPMCurrent.m_Judge = CONST_JUDGE_NAME::JUDGE_HLPM_CURRENT;
	m_HLPMCurrent.FindWorkZone();

	m_TSPStart.Init();
	m_TSPStart.m_strCommand = TSP_START;
	m_TSPStart.m_Judge = CONST_JUDGE_NAME::JUDGE_TSP_START;
	m_TSPStart.FindWorkZone();

	m_EVTVersionCheck.Init();
	m_EVTVersionCheck.m_strCommand = EVT_VERSION_CHECK;
	m_EVTVersionCheck.m_Judge = CONST_JUDGE_NAME::JUDGE_EVT_VERSION_CHECK;
	m_EVTVersionCheck.FindWorkZone();

	m_TECheck.Init();
	m_TECheck.m_strCommand = TE_CHECK;
	m_TECheck.m_Judge = CONST_JUDGE_NAME::JUDGE_TE_CHECK;
	m_TECheck.FindWorkZone();

	m_IDCheck.Init();
	m_IDCheck.m_strCommand = ID_CHECK;
	m_IDCheck.m_Judge = CONST_JUDGE_NAME::JUDGE_ID_CHECK;
	m_IDCheck.FindWorkZone();

	m_OTPREGCheck.Init();
	m_OTPREGCheck.m_strCommand = OTP_REG_CHECK;
	m_OTPREGCheck.m_Judge = CONST_JUDGE_NAME::JUDGE_OTP_REG_CHECK;
	m_OTPREGCheck.FindWorkZone();

	m_ICTCheck.Init();
	m_ICTCheck.m_strCommand = ICT_TEST;
	m_ICTCheck.m_Judge = CONST_JUDGE_NAME::JUDGE_ICT_TEST;
	m_ICTCheck.FindWorkZone();

	m_COPRICTTest.Init();
	m_COPRICTTest.m_strCommand = COPR_ICT_TEST;
	m_COPRICTTest.m_Judge = CONST_JUDGE_NAME::JUDGE_COPR_ICT_TEST;
	m_COPRICTTest.FindWorkZone();

	m_PocErrorCheck.Init();
	m_PocErrorCheck.m_strCommand = POC_ERROR_CHECK;
	m_PocErrorCheck.m_Judge = CONST_JUDGE_NAME::JUDGE_POC_ERROR_CHECK;
	m_PocErrorCheck.FindWorkZone();

	m_DDIBlockTest.Init();
	m_DDIBlockTest.m_strCommand = DDI_BLOCK_TEST;
	m_DDIBlockTest.m_Judge = CONST_JUDGE_NAME::JUDGE_DDI_BLOCK_TEST;
	m_DDIBlockTest.FindWorkZone();

	m_OptionCheck2.Init();
	m_OptionCheck2.m_strCommand = OPTION_CHECK2;
	m_OptionCheck2.m_Judge = CONST_JUDGE_NAME::JUDGE_OPTION_CHECK2;
	m_OptionCheck2.FindWorkZone();

	m_OptionCheck3.Init();
	m_OptionCheck3.m_strCommand = OPTION_CHECK3;
	m_OptionCheck3.m_Judge = CONST_JUDGE_NAME::JUDGE_OPTION_CHECK3;
	m_OptionCheck3.FindWorkZone();

	m_OptionCheck4.Init();
	m_OptionCheck4.m_strCommand = OPTION_CHECK4;
	m_OptionCheck4.m_Judge = CONST_JUDGE_NAME::JUDGE_OPTION_CHECK4;
	m_OptionCheck4.FindWorkZone();

	m_OptionCheck5.Init();
	m_OptionCheck5.m_strCommand = OPTION_CHECK5;
	m_OptionCheck5.m_Judge = CONST_JUDGE_NAME::JUDGE_OPTION_CHECK5;
	m_OptionCheck5.FindWorkZone();

	m_OptionCheck6.Init();
	m_OptionCheck6.m_strCommand = OPTION_CHECK6;
	m_OptionCheck6.m_Judge = CONST_JUDGE_NAME::JUDGE_OPTION_CHECK6;
	m_OptionCheck6.FindWorkZone();

	m_OptionCheck7.Init();
	m_OptionCheck7.m_strCommand = OPTION_CHECK7;
	m_OptionCheck7.m_Judge = CONST_JUDGE_NAME::JUDGE_OPTION_CHECK7;
	m_OptionCheck7.FindWorkZone();

	m_OptionCheck8.Init();
	m_OptionCheck8.m_strCommand = OPTION_CHECK8;
	m_OptionCheck8.m_Judge = CONST_JUDGE_NAME::JUDGE_OPTION_CHECK8;
	m_OptionCheck8.FindWorkZone();

	m_OptionCheck9.Init();
	m_OptionCheck9.m_strCommand = OPTION_CHECK9;
	m_OptionCheck9.m_Judge = CONST_JUDGE_NAME::JUDGE_OPTION_CHECK9;
	m_OptionCheck9.FindWorkZone();

	m_OptionCheck10.Init();
	m_OptionCheck10.m_strCommand = OPTION_CHECK10;
	m_OptionCheck10.m_Judge = CONST_JUDGE_NAME::JUDGE_OPTION_CHECK10;
	m_OptionCheck10.FindWorkZone();

	m_nDefectPointCount = 0;
	for(int i = 0; i < MAX_DEFECT_POINT; i++)
	{
		m_DefectPoint[i] = CIM_DEFECT_POINT();
	}

	m_PG_LOG_INFO.Init();
}

CCellDefectInfo* CCellInfo::GetDefectInfo( CString strINspModuleName )
{
	if(strINspModuleName == INSP_MODULE_CELL_LOADING)
		return &m_CellLoading;
	else if(strINspModuleName == INSP_MODULE_MTP_WRITE)
		return &m_MTPWrite;
	else if(strINspModuleName == INSP_MODULE_MTP_VERIFY)
		return &m_MTPVerify;
	else if(strINspModuleName == INSP_MODULE_WHITE_CURRENT)
		return &m_WhiteCurrent;
	else if(strINspModuleName == INSP_MODULE_SLEEP_CURRENT)
		return &m_SleepCurrent;
	else if(strINspModuleName == INSP_MODULE_HLPM_CURRENT)
		return &m_HLPMCurrent;
	else if(strINspModuleName == INSP_MODULE_TSP_START)
		return &m_TSPStart;
	else if(strINspModuleName == INSP_MODULE_EVT_VERSION_CHECK)
		return &m_EVTVersionCheck;
	else if(strINspModuleName == INSP_MODULE_TE_CHECK)
		return &m_TECheck;
	else if(strINspModuleName == INSP_MODULE_ID_CHECK)
		return &m_IDCheck;
	else if(strINspModuleName == INSP_MODULE_OTP_REG_CHECK)
		return &m_OTPREGCheck;
	else if(strINspModuleName == INSP_MODULE_ICT_TEST)
		return &m_ICTCheck;
	else if(strINspModuleName == INSP_MODULE_COPR_ICT_TEST)
		return &m_COPRICTTest;
	else if(strINspModuleName == INSP_MODULE_POC_ERROR_CHECK)
		return &m_PocErrorCheck;
	else if(strINspModuleName == INSP_MODULE_DDI_BLOCK_TEST)
		return &m_DDIBlockTest;
	else if(strINspModuleName == INSP_MODULE_OPTION_CHECK2)
		return &m_OptionCheck2;
	else if(strINspModuleName == INSP_MODULE_OPTION_CHECK3)
		return &m_OptionCheck3;
	else if(strINspModuleName == INSP_MODULE_OPTION_CHECK4)
		return &m_OptionCheck4;
	else if(strINspModuleName == INSP_MODULE_OPTION_CHECK5)
		return &m_OptionCheck5;
	else if(strINspModuleName == INSP_MODULE_OPTION_CHECK6)
		return &m_OptionCheck6;
	else if(strINspModuleName == INSP_MODULE_OPTION_CHECK7)
		return &m_OptionCheck7;
	else if(strINspModuleName == INSP_MODULE_OPTION_CHECK8)
		return &m_OptionCheck8;
	else if(strINspModuleName == INSP_MODULE_OPTION_CHECK9)
		return &m_OptionCheck9;
	else if(strINspModuleName == INSP_MODULE_OPTION_CHECK10)
		return &m_OptionCheck10;

	return NULL;
}

void CCellInfo::LinkCellLogItem()
{
	m_listCellLogItem.RemoveAll();

	CCellLogItem item;

	//////////////////////////////////////////////////////////////////////////
	// COMMON [11/16/2016 OSC]
	//kjpark 20170907 CellLog Title 및 해당 항목 추가
	item = CCellLogItem(_T("INNER ID"),						CELL_LOG_DATA_STRING,		&defaultData.m_strInnerID);							m_listCellLogItem.AddTail(item);	
	item = CCellLogItem(_T("CELL ID"),						CELL_LOG_DATA_STRING,		&defaultData.m_strCellID);							m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("READ UNIT"),					CELL_LOG_DATA_STRING,		&defaultData.m_strReadUnitMCR);						m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("MODEL"),						CELL_LOG_DATA_STRING,		&theRecipeBank.m_strRecipeName);								m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("EQUIP ID"),						CELL_LOG_DATA_STRING,		&theProcBank.m_strEqipID);										m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("JIG NAME"),						CELL_LOG_DATA_INT_ADD1,		&defaultData.m_JigId);								m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("CH NAME"),						CELL_LOG_DATA_INT_ADD1,		&defaultData.m_JigCh);								m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("Contact Count"),				CELL_LOG_DATA_INT,			&m_CellLoading.m_ContactCount);									m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("PRODUCT ID"),					CELL_LOG_DATA_STRING,		&defaultData.m_strProductID);							m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AZONE OPERATOR"),				CELL_LOG_DATA_STRING,		&defaultData.m_str_Operator_SSO_ID);					m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("MC_Ver"),						CELL_LOG_DATA_STRING,		&theProcBank.m_strControlPCVersion);							m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("L_Ver"),						CELL_LOG_DATA_STRING,		NULL);															m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("U_Ver"),						CELL_LOG_DATA_STRING,		NULL);															m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("D_Ver"),						CELL_LOG_DATA_STRING,		&theProcBank.m_strDataPCVersion);								m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("TX HOST VERSION"),				CELL_LOG_DATA_STRING,		&defaultData.m_strPG_Host_Version);						m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("TX CLIENT VERSION"),			CELL_LOG_DATA_STRING,		&defaultData.m_strPG_Client_Version);						m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("PROGRAM NAME"),					CELL_LOG_DATA_STRING,		&defaultData.m_strTMD_Info);							m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("CH Card MAC"),					CELL_LOG_DATA_STRING,		&defaultData.m_strPG_MAC_Address);					m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("UNLOAD TACT TIME"),				CELL_LOG_DATA_TACT,			&m_UnloadTactTime.m_timeStart, &m_UnloadTactTime.m_timeEnd);	m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("TACT TIME"),					CELL_LOG_DATA_TACT,			&m_TactTime.m_timeStart, &m_TactTime.m_timeEnd);				m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("Waiting Time"),					CELL_LOG_DATA_TACT,			&m_WaitTime.m_timeStart, &m_WaitTime.m_timeEnd);				m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("LOADING TACT TIME"),			CELL_LOG_DATA_TACT,			&m_LoadingTactTime.m_timeStart, &m_LoadingTactTime.m_timeEnd);	m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("MCR MOVE TIME"),				CELL_LOG_DATA_TACT,			&m_AZonetoMCRZone.m_timeStart, &m_AZonetoMCRZone.m_timeEnd);	m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("MCR READ TIME"),				CELL_LOG_DATA_TACT,			&m_MCRReadTime.m_timeStart, &m_MCRReadTime.m_timeEnd);					m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("MTP MOVE TIME"),				CELL_LOG_DATA_TACT,			&m_MCRZonetoBZone.m_timeStart, &m_MCRZonetoBZone.m_timeEnd);	m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("UNLOAD READY TIME"),			CELL_LOG_DATA_TACT,			&m_BZonetoAZone.m_timeStart, &m_BZonetoAZone.m_timeEnd);		m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("LAST CLASS"),					CELL_LOG_DATA_CLASS_CELL,	&defaultData.m_LastClass);							m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("LAST RESULT"),					CELL_LOG_DATA_STRING,		&defaultData.m_strLastResult);						m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("MES CODE"),						CELL_LOG_DATA_STRING,		&defaultData.MesCode);								m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AB RESULT"),					CELL_LOG_DATA_STRING,		&defaultData.m_strCIMJudge);						m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AB RULE"),						CELL_LOG_DATA_STRING,		&defaultData.m_strABRule);							m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("A RESULT"),						CELL_LOG_DATA_STRING,		&defaultData.m_strAResult);							m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("N/A"),							CELL_LOG_DATA_INT,			NULL);															m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("N/A"),							CELL_LOG_DATA_INT,			NULL);															m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AZONE START TIME"),				CELL_LOG_DATA_TIME,			&m_AZone.m_timeStart);											m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AZONE END TIME"),				CELL_LOG_DATA_TIME,			&m_AZone.m_timeEnd);											m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AZONE TACT TIME"),				CELL_LOG_DATA_TACT,			&m_AZone.m_timeStart, &m_AZone.m_timeEnd);						m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AZONE WAITING"),				CELL_LOG_DATA_TACT,			&m_AZone.m_timeEnd, &m_AZone.m_timeWait);						m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AZONE CLASS"),					CELL_LOG_DATA_CLASS_CELL,	&m_AZone.m_Class);												m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AZONE DEFECT"),					CELL_LOG_DATA_STRING,		&m_AZone.m_DefectName);											m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AZONE CONTACT START TIME"),		CELL_LOG_DATA_TIME,			&m_CellLoading.m_timeStart);									m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AZONE CONTACT END TIME"),		CELL_LOG_DATA_TIME,			&m_CellLoading.m_timeEnd);										m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AZONE CONTACT TACT TIME"),		CELL_LOG_DATA_TACT,			&m_CellLoading.m_timeStart, &m_CellLoading.m_timeEnd);			m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AZONE ETC TACT TIME"),			CELL_LOG_DATA_TACT,			&m_AZoneETCTime.m_timeStart, &m_AZoneETCTime.m_timeEnd);	m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AZONE CONTACT CLASS"),			CELL_LOG_DATA_CLASS_CELL,	&m_CellLoading.m_Class);										m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AZONE CONTACT IBAT"),			CELL_LOG_DATA_STRING,		&m_CellLoading.m_CurrentText[IBAT]);							m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AZONE CONTACT IVSS"),			CELL_LOG_DATA_STRING,		&m_CellLoading.m_CurrentText[IVSS]);							m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AZONE CONTACT IVDD"),			CELL_LOG_DATA_STRING,		&m_CellLoading.m_CurrentText[IVDD]);							m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AZONE CONTACT IVCI"),			CELL_LOG_DATA_STRING,		&m_CellLoading.m_CurrentText[IVCI]);							m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AZONE CONTACT IBAT2"),			CELL_LOG_DATA_STRING,		&m_CellLoading.m_CurrentText[IBAT2]);							m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("AZONE CONTACT IDD2"),			CELL_LOG_DATA_STRING,		&m_CellLoading.m_CurrentText[IDD2]);							m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("N/A"),							CELL_LOG_DATA_INT,			NULL);															m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("N/A"),							CELL_LOG_DATA_INT,			NULL);															m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("BZONE START TIME"),				CELL_LOG_DATA_TIME,			&m_BZone.m_timeStart);										m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("BZONE END TIME"),				CELL_LOG_DATA_TIME,			&m_BZone.m_timeEnd);										m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("BZONE TACT TIME"),				CELL_LOG_DATA_TACT,			&m_BZone.m_timeStart, &m_BZone.m_timeEnd);					m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("BZONE WAITING"),				CELL_LOG_DATA_TACT,			&m_BZone.m_timeEnd, &m_BZone.m_timeWait);					m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("BZONE CLASS"),					CELL_LOG_DATA_CLASS_CELL,	&m_BZone.m_Class);											m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("BZONE DEFECT"),					CELL_LOG_DATA_STRING,		&m_BZone.m_DefectName);										m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("BZONE MTP READY TIME"),			CELL_LOG_DATA_TACT,			&m_MTPReadyTime.m_timeStart, &m_MTPReadyTime.m_timeEnd);		m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("BZONE MTP WRITE START TIME"),	CELL_LOG_DATA_TIME,			&m_MTPWrite.m_timeStart);										m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("BZONE MTP WRITE END TIME"),		CELL_LOG_DATA_TIME,			&m_MTPWrite.m_timeEnd);											m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("BZONE MTP WRITE TACT TIME"),	CELL_LOG_DATA_TACT,			&m_MTPWrite.m_timeStart, &m_MTPWrite.m_timeEnd);				m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("BZONE MTP WRITE TYPE"),			CELL_LOG_DATA_STRING,		&m_MTPWrite.m_strMTPType);										m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("BZONE MTP WRITE CLASS"),		CELL_LOG_DATA_CLASS_CELL,	&m_MTPWrite.m_Class);											m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("BZONE MTP WRITE DEFECT"),		CELL_LOG_DATA_STRING,		&m_MTPWrite.m_DefectName);										m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("BZONE MTP VERIFY START TIME"),	CELL_LOG_DATA_TIME,			&m_MTPVerify.m_timeStart);										m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("BZONE MTP VERIFY END TIME"),	CELL_LOG_DATA_TIME,			&m_MTPVerify.m_timeEnd);										m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("BZONE MTP VERIFY TACT TIME"),	CELL_LOG_DATA_TACT,			&m_MTPVerify.m_timeStart, &m_MTPVerify.m_timeEnd);				m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("BZONE MTP VERIFY CLASS"),		CELL_LOG_DATA_CLASS_CELL,	&m_MTPVerify.m_Class);											m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("BZONE MTP VERIFY DEFECT"),		CELL_LOG_DATA_STRING,		&m_MTPVerify.m_DefectName);										m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("BZONE ETC TACT TIME"),			CELL_LOG_DATA_TACT,			&m_BZoneETCTime.m_timeStart, &m_BZoneETCTime.m_timeEnd);		m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("N/A"),							CELL_LOG_DATA_INT,			NULL);															m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("N/A"),							CELL_LOG_DATA_INT,			NULL);															m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("CORRELATION"),					CELL_LOG_DATA_STRING,		&m_PG_LOG_INFO.m_CORRELATION);									m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("BEF 255 MTP"),					CELL_LOG_DATA_STRING,		&m_PG_LOG_INFO.m_BEF_255_MTP);									m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("S MID 255 PRE L MTP"),			CELL_LOG_DATA_STRING,		&m_PG_LOG_INFO.m_S_MID_255_PRE_L_MTP);							m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("S MID 255 PRE X MTP"),			CELL_LOG_DATA_STRING,		&m_PG_LOG_INFO.m_S_MID_255_PRE_X_MTP);							m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("S MID 255 PRE Y MTP"),			CELL_LOG_DATA_STRING,		&m_PG_LOG_INFO.m_S_MID_255_PRE_Y_MTP);							m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("S MID 255 PRE TCOLOR MTP"),		CELL_LOG_DATA_STRING,		&m_PG_LOG_INFO.m_S_MID_255_PRE_TCOLOR_MTP);						m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("S MID 255 PRE MPCD MTP"),		CELL_LOG_DATA_STRING,		&m_PG_LOG_INFO.m_S_MID_255_PRE_MPCD_MTP);						m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("300CD V255 L VERIFY"),			CELL_LOG_DATA_STRING,		&m_PG_LOG_INFO.m_300CD_V255_L_VERIFY);							m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("300CD V255 X VERIFY"),			CELL_LOG_DATA_STRING,		&m_PG_LOG_INFO.m_300CD_V255_X_VERIFY);							m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("300CD V255 Y VERIFY"),			CELL_LOG_DATA_STRING,		&m_PG_LOG_INFO.m_300CD_V255_Y_VERIFY);							m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("300CD V255 TCOLOR VERIFY"),		CELL_LOG_DATA_STRING,		&m_PG_LOG_INFO.m_300CD_V255_TCOLOR_VERIFY);						m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("300CD V255 MPCD VERIFY"),		CELL_LOG_DATA_STRING,		&m_PG_LOG_INFO.m_300CD_V255_MPCD_VERIFY);						m_listCellLogItem.AddTail(item);
	//kjpark 20180129 MID ALPM 255 IBAT 항목추가
	item = CCellLogItem(_T("MID ALPM 255 IBAT"),			CELL_LOG_DATA_STRING,		&m_PG_LOG_INFO.m_MID_ALPM_255_IBAT);							m_listCellLogItem.AddTail(item);

	item = CCellLogItem(_T("HLPM CURRENT START TIME"),		CELL_LOG_DATA_TIME,			&m_HLPMCurrent.m_timeStart);									m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("HLPM CURRENT END TIME"),		CELL_LOG_DATA_TIME,			&m_HLPMCurrent.m_timeEnd);										m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("HLPM CURRENT TACT TIME"),		CELL_LOG_DATA_TACT,			&m_HLPMCurrent.m_timeStart, &m_HLPMCurrent.m_timeEnd);			m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("HLPM CURRENT CLASS"),			CELL_LOG_DATA_CLASS_CELL,		&m_HLPMCurrent.m_Class);										m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("HLPM CURRENT DEFECT"),			CELL_LOG_DATA_STRING,		&m_HLPMCurrent.m_DefectName);									m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("HLPM CURRENT IBAT"),			CELL_LOG_DATA_STRING,		&m_HLPMCurrent.m_CurrentText[IBAT]);							m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("HLPM CURRENT IVSS"),			CELL_LOG_DATA_STRING,		&m_HLPMCurrent.m_CurrentText[IVSS]);							m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("HLPM CURRENT IVDD"),			CELL_LOG_DATA_STRING,		&m_HLPMCurrent.m_CurrentText[IVDD]);							m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("HLPM CURRENT IVCI"),			CELL_LOG_DATA_STRING,		&m_HLPMCurrent.m_CurrentText[IVCI]);							m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("HLPM CURRENT IBAT2"),			CELL_LOG_DATA_STRING,		&m_HLPMCurrent.m_CurrentText[IBAT2]);							m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("HLPM CURRENT IDD2"),			CELL_LOG_DATA_STRING,		&m_HLPMCurrent.m_CurrentText[IDD2]);							m_listCellLogItem.AddTail(item);
	
	item = CCellLogItem(_T("OPTION CHECK 3 CHECK ZONE"),			CELL_LOG_DATA_STRING,		&m_OptionCheck3.m_strWorkZone);						m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("OPTION CHECK 3 CHECK START TIME"),		CELL_LOG_DATA_TIME,			&m_OptionCheck3.m_timeStart);							m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("OPTION CHECK 3 CHECK END TIME"),		CELL_LOG_DATA_TIME,			&m_OptionCheck3.m_timeEnd);								m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("OPTION CHECK 3 CHECK TACT TIME"),		CELL_LOG_DATA_TACT,			&m_OptionCheck3.m_timeStart, &m_OptionCheck3.m_timeEnd);m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("OPTION CHECK 3 CHECK CLASS"),			CELL_LOG_DATA_CLASS_CELL,		&m_OptionCheck3.m_Class);								m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("OPTION CHECK 3 CHECK DEFECT"),			CELL_LOG_DATA_STRING,		&m_OptionCheck3.m_DefectName);							m_listCellLogItem.AddTail(item);

	item = CCellLogItem(_T("OPTION CHECK 4 CHECK ZONE"),			CELL_LOG_DATA_STRING,		&m_OptionCheck4.m_strWorkZone);						m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("OPTION CHECK 4 CHECK START TIME"),		CELL_LOG_DATA_TIME,			&m_OptionCheck4.m_timeStart);							m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("OPTION CHECK 4 CHECK END TIME"),		CELL_LOG_DATA_TIME,			&m_OptionCheck4.m_timeEnd);								m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("OPTION CHECK 4 CHECK TACT TIME"),		CELL_LOG_DATA_TACT,			&m_OptionCheck4.m_timeStart, &m_OptionCheck4.m_timeEnd);m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("OPTION CHECK 4 CHECK CLASS"),			CELL_LOG_DATA_CLASS_CELL,		&m_OptionCheck4.m_Class);								m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("OPTION CHECK 4 CHECK DEFECT"),			CELL_LOG_DATA_STRING,		&m_OptionCheck4.m_DefectName);							m_listCellLogItem.AddTail(item);

	item = CCellLogItem(_T("OPTION CHECK 5 CHECK ZONE"),			CELL_LOG_DATA_STRING,		&m_OptionCheck5.m_strWorkZone);						m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("OPTION CHECK 5 CHECK START TIME"),		CELL_LOG_DATA_TIME,			&m_OptionCheck5.m_timeStart);							m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("OPTION CHECK 5 CHECK END TIME"),		CELL_LOG_DATA_TIME,			&m_OptionCheck5.m_timeEnd);								m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("OPTION CHECK 5 CHECK TACT TIME"),		CELL_LOG_DATA_TACT,			&m_OptionCheck5.m_timeStart, &m_OptionCheck5.m_timeEnd);m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("OPTION CHECK 5 CHECK CLASS"),			CELL_LOG_DATA_CLASS_CELL,		&m_OptionCheck5.m_Class);								m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("OPTION CHECK 5 CHECK DEFECT"),			CELL_LOG_DATA_STRING,		&m_OptionCheck5.m_DefectName);							m_listCellLogItem.AddTail(item);

	//20180517, cdtruong Crown 모델 검사위 해서 강원호 프로 OPTION CHECK 10까지 추가 요청
	item = CCellLogItem(_T("OPTION CHECK 6 CHECK ZONE"),			CELL_LOG_DATA_STRING,		&m_OptionCheck6.m_strWorkZone);						m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("OPTION CHECK 6 CHECK START TIME"),		CELL_LOG_DATA_TIME,			&m_OptionCheck6.m_timeStart);							m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("OPTION CHECK 6 CHECK END TIME"),		CELL_LOG_DATA_TIME,			&m_OptionCheck6.m_timeEnd);								m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("OPTION CHECK 6 CHECK TACT TIME"),		CELL_LOG_DATA_TACT,			&m_OptionCheck6.m_timeStart, &m_OptionCheck6.m_timeEnd);m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("OPTION CHECK 6 CHECK CLASS"),			CELL_LOG_DATA_CLASS_CELL,		&m_OptionCheck6.m_Class);								m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("OPTION CHECK 6 CHECK DEFECT"),			CELL_LOG_DATA_STRING,		&m_OptionCheck6.m_DefectName);							m_listCellLogItem.AddTail(item);

	item = CCellLogItem(_T("OPTION CHECK 7 CHECK ZONE"),			CELL_LOG_DATA_STRING,		&m_OptionCheck7.m_strWorkZone);						m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("OPTION CHECK 7 CHECK START TIME"),		CELL_LOG_DATA_TIME,			&m_OptionCheck7.m_timeStart);							m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("OPTION CHECK 7 CHECK END TIME"),		CELL_LOG_DATA_TIME,			&m_OptionCheck7.m_timeEnd);								m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("OPTION CHECK 7 CHECK TACT TIME"),		CELL_LOG_DATA_TACT,			&m_OptionCheck7.m_timeStart, &m_OptionCheck7.m_timeEnd);m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("OPTION CHECK 7 CHECK CLASS"),			CELL_LOG_DATA_CLASS_CELL,		&m_OptionCheck7.m_Class);								m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("OPTION CHECK 7 CHECK DEFECT"),			CELL_LOG_DATA_STRING,		&m_OptionCheck7.m_DefectName);							m_listCellLogItem.AddTail(item);

	item = CCellLogItem(_T("OPTION CHECK 8 CHECK ZONE"),			CELL_LOG_DATA_STRING,		&m_OptionCheck8.m_strWorkZone);						m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("OPTION CHECK 8 CHECK START TIME"),		CELL_LOG_DATA_TIME,			&m_OptionCheck8.m_timeStart);							m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("OPTION CHECK 8 CHECK END TIME"),		CELL_LOG_DATA_TIME,			&m_OptionCheck8.m_timeEnd);								m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("OPTION CHECK 8 CHECK TACT TIME"),		CELL_LOG_DATA_TACT,			&m_OptionCheck8.m_timeStart, &m_OptionCheck8.m_timeEnd);m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("OPTION CHECK 8 CHECK CLASS"),			CELL_LOG_DATA_CLASS_CELL,		&m_OptionCheck8.m_Class);								m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("OPTION CHECK 8 CHECK DEFECT"),			CELL_LOG_DATA_STRING,		&m_OptionCheck8.m_DefectName);							m_listCellLogItem.AddTail(item);

	item = CCellLogItem(_T("OPTION CHECK 9 CHECK ZONE"),			CELL_LOG_DATA_STRING,		&m_OptionCheck9.m_strWorkZone);						m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("OPTION CHECK 9 CHECK START TIME"),		CELL_LOG_DATA_TIME,			&m_OptionCheck9.m_timeStart);							m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("OPTION CHECK 9 CHECK END TIME"),		CELL_LOG_DATA_TIME,			&m_OptionCheck9.m_timeEnd);								m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("OPTION CHECK 9 CHECK TACT TIME"),		CELL_LOG_DATA_TACT,			&m_OptionCheck9.m_timeStart, &m_OptionCheck9.m_timeEnd);m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("OPTION CHECK 9 CHECK CLASS"),			CELL_LOG_DATA_CLASS_CELL,		&m_OptionCheck9.m_Class);								m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("OPTION CHECK 9 CHECK DEFECT"),			CELL_LOG_DATA_STRING,		&m_OptionCheck9.m_DefectName);							m_listCellLogItem.AddTail(item);

	item = CCellLogItem(_T("OPTION CHECK 10 CHECK ZONE"),			CELL_LOG_DATA_STRING,		&m_OptionCheck10.m_strWorkZone);						m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("OPTION CHECK 10 CHECK START TIME"),		CELL_LOG_DATA_TIME,			&m_OptionCheck10.m_timeStart);							m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("OPTION CHECK 10 CHECK END TIME"),		CELL_LOG_DATA_TIME,			&m_OptionCheck10.m_timeEnd);								m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("OPTION CHECK 10 CHECK TACT TIME"),		CELL_LOG_DATA_TACT,			&m_OptionCheck10.m_timeStart, &m_OptionCheck10.m_timeEnd);m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("OPTION CHECK 10 CHECK CLASS"),			CELL_LOG_DATA_CLASS_CELL,		&m_OptionCheck10.m_Class);								m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("OPTION CHECK 10 CHECK DEFECT"),			CELL_LOG_DATA_STRING,		&m_OptionCheck10.m_DefectName);							m_listCellLogItem.AddTail(item);

	//kjpark 20180113 Tmd Version 추가
	item = CCellLogItem(_T("TX FPGA VER"),							CELL_LOG_DATA_STRING,		&defaultData.m_strTXFPGAVersion);							m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("TX CPLD VER"),							CELL_LOG_DATA_STRING,		&defaultData.m_strTXCPLDVersion);							m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("TX OS VER"),							CELL_LOG_DATA_STRING,		&defaultData.m_strTXOSVersion);							m_listCellLogItem.AddTail(item);
	//kjpark 20180123 CELL INFO, JOB PROCESS Cellid 추가
	item = CCellLogItem(_T("CELL INFO"),							CELL_LOG_DATA_STRING,		&defaultData.m_strCellInfoResult);							m_listCellLogItem.AddTail(item);
	item = CCellLogItem(_T("JOB PROCESS"),							CELL_LOG_DATA_STRING,		&defaultData.m_strJobProcessState);							m_listCellLogItem.AddTail(item);
}

CString CCellInfo::GetCellLogTitle()
{
	CString strBuff;
	strBuff.ReleaseBuffer();
	TCHAR * pBuf = strBuff.GetBuffer(MAX_CELL_LOG_STRING);
	memset(pBuf, NULL, MAX_CELL_LOG_STRING);
	int nWriteStart = 0;
	int nLength;

	CCellLogItem item;
	POSITION pos = m_listCellLogItem.GetHeadPosition();
	while(pos)
	{
		item = m_listCellLogItem.GetNext(pos);
		nLength = item.GetTitle().GetLength();
		if(nWriteStart + nLength > MAX_CELL_LOG_STRING)
			break;
		memcpy(pBuf+nWriteStart, item.GetTitle().GetString() , nLength * sizeof(TCHAR));
		nWriteStart += nLength;

		// 뒤에 항목이 더 있으면 쉼표를 붙여준다 [11/16/2016 OSC]
		if(pos)
		{
			memcpy(pBuf+nWriteStart, _T(",") , 1 * sizeof(TCHAR));
			nWriteStart += 1;
		}
	}

	// 줄 넘기기 [11/16/2016 OSC]
	memcpy(pBuf+nWriteStart, _T("\r\n") , 2 * sizeof(TCHAR));
	strBuff.ReleaseBuffer();

	return strBuff;
}

CString CCellInfo::GetCellLogValue()
{
	CString strBuff;
	strBuff.ReleaseBuffer();
	TCHAR * pBuf = strBuff.GetBuffer(MAX_CELL_LOG_STRING);
	memset(pBuf, NULL, MAX_CELL_LOG_STRING);
	int nWriteStart = 0;
	int nLength;

	CCellLogItem item;
	POSITION pos = m_listCellLogItem.GetHeadPosition();
	while(pos)
	{
		item = m_listCellLogItem.GetNext(pos);
		nLength = item.ValueToString().GetLength();
		if(nWriteStart + nLength > MAX_CELL_LOG_STRING)
			break;
		memcpy(pBuf+nWriteStart, item.ValueToString().GetString() , nLength * sizeof(TCHAR));
		nWriteStart += nLength;

		// 뒤에 항목이 더 있으면 쉼표를 붙여준다 [11/16/2016 OSC]
		if(pos)
		{
			memcpy(pBuf+nWriteStart, _T(",") , 1 * sizeof(TCHAR));
			nWriteStart += 1;
		}
	}

	// 줄 넘기기 [11/16/2016 OSC]
	memcpy(pBuf+nWriteStart, _T("\r\n") , 2 * sizeof(TCHAR));
	strBuff.ReleaseBuffer();

	return strBuff;
}


BOOL CCellInfo::Retry_CheckAble()
{
	if( (defaultData.m_bRetryAB == FALSE) 
		&& (defaultData.m_LastClass != GOOD_CELL)
// 		&& defaultData.m_bTrackinFinish
		&& defaultData.m_bRetryAble )
		return TRUE;
	else
		return FALSE;
}

BOOL CCellInfo::CellInfo_CheckAble()
{
	if(defaultData.m_bRetryAB)
		return FALSE;
	if(defaultData.m_bMCR_OK == FALSE)
		return FALSE;
	if(theConfigBank.m_Option.m_bUseCellInforRequest == FALSE)
		return FALSE;
	if(theConfigBank.m_CIM.GetFunction(EFID_CELL_MCR_MODE)->EFST_Get() == EFST_UNUSE)
		return FALSE;
	return TRUE;
}

BOOL CCellInfo::TrackIn_CheckAble()
{
	if(defaultData.m_bRetryAB)
		return FALSE;
	if(defaultData.m_bMCR_OK == FALSE)
		return FALSE;
	if(theConfigBank.m_Option.m_bUseCellInforRequest)
	{
		if(defaultData.m_CellInfoResult != GOOD_CELL)
			return FALSE;
	}

// 	if(defaultData.m_bTrackinFinish)
// 		return FALSE;

	return TRUE;
}

BOOL CCellInfo::InspectionEnd_CheckAble()
{
	if(defaultData.m_bMCR_OK == FALSE)
		return FALSE;
// 	if(theConfigBank.m_Option.m_bUseCellInforRequest == FALSE)
// 		return FALSE;
	if(theConfigBank.m_Option.m_bUseCellInforRequest)
	{
		if(defaultData.m_CellInfoResult != GOOD_CELL)
			return FALSE;
	}

	if(theConfigBank.m_CIM.TRACKING_CONTROL_InCheck())
	{
		if(defaultData.m_nInspectInvalidType != JOB_START)
			return FALSE;
	}
	return TRUE;
}

BOOL CCellInfo::TrackOut_CheckAble()
{
	// Retry가 있어서 TrackIn_CheckAble를 사용 못함 
	if(defaultData.m_bMCR_OK == FALSE)
		return FALSE;

	if(theConfigBank.m_Option.m_bUseCellInforRequest)
	{
		if(defaultData.m_CellInfoResult != GOOD_CELL)
			return FALSE;
	}

	if(defaultData.m_bTrackinFinish == FALSE)
		return FALSE;

	if(theConfigBank.m_Option.m_bUseRetryAB)
	{
		if( defaultData.m_bRetryAble && (defaultData.m_bRetryAB == FALSE) )
		{
			if(theConfigBank.m_CIM.TRACKING_CONTROL_InCheck())
			{
				if(defaultData.m_nInspectInvalidType == JOB_START)
					return FALSE;
			}
			else
			{
				return FALSE;
			}
		}
	}
	// O보고도 해야 하니 JobStart는 확인하지 않는다

// 	if(defaultData.m_bTrackOutFinish)
// 		return FALSE;

	return TRUE;
}

BOOL CCellInfo::TrackIn_CancelAble()
{
	if(theConfigBank.m_CIM.TRACKING_CONTROL_InCheck())
	{
		if(defaultData.m_nInspectInvalidType == JOB_START)
			return FALSE;
		else
			return TRUE;
	}
	return FALSE;
}

void CCellInfo::CimJudge_Judge(CString strForcedJudge/* = _T("")*/)
{
	// 강제 변경하는 거면 아래거 무시
	// R을 L로 바꾼다던가, 임의취출로 O보고 한다던가 할 때...
	if(strForcedJudge.IsEmpty() == FALSE)
	{
		defaultData.m_strCIMJudge = strForcedJudge;		
	}
	else
	{
		// m_cJudge "G", "R", "L" -> BIN1, BIN2, REJECT
		if(defaultData.m_LastClass == GOOD_CELL)
			defaultData.m_strCIMJudge = CIM_JUDGE_GOOD;	
		//  사용자가 지정한 불량은 R(Retest)로 올린다
		else if(theConfigBank.m_DefectRetest.FindRetestList(defaultData.m_strLastResult))
			defaultData.m_strCIMJudge = CIM_JUDGE_RETEST;	
		// Retry를 이미 한 거나 Retry 불가한 불량은 L 보고
		else if( defaultData.m_bRetryAB || (defaultData.m_bRetryAble == FALSE) )	
			defaultData.m_strCIMJudge = CIM_JUDGE_LOSS;	
		// Retry 하기 전은 R 보고
		else												
			defaultData.m_strCIMJudge = CIM_JUDGE_RETEST;	
	}
}

CCellBank::CCellBank(void)
{
	m_nNextGenIndex = 0;
}


CCellBank::~CCellBank(void)
{
}

CCellTag CCellBank::GetCellTag( CELL_POS pos )
{
	return m_CellTag[pos];
}

CCellTag CCellBank::GetCellTag( JIG_ID jig, JIG_CH nCh )
{
	int nTagIndex = CELL_POS_SHUTTLE1_CH1;
	nTagIndex += ((int)jig*JIG_CH_MAX) + (int)nCh;

	return m_CellTag[nTagIndex];
}

CCellTag CCellBank::SearchCellTag( JIG_ID nJig, JIG_CH nCh, CELL_POS startPos /*= CELL_POS_AZONE_CH1*/, CELL_POS endPos /*= CELL_POS_DZONE_CH5*/ )
{
	int nStart = (int)startPos;
	int nEnd = (int)endPos;
	CCellTag tag;
	CCellInfo *pCell;
	for(int i =nStart; i <= nEnd; i++ )
	{
		if(m_CellTag[i].IsExist())
		{
			pCell = GetCellInfo(m_CellTag[i]);
			if( (pCell->defaultData.m_JigId == nJig) && (pCell->defaultData.m_JigCh == nCh) )
			{
				tag = m_CellTag[i];
				break;
			}
		}
	}
	return tag;
}

CCellTag CCellBank::SearchCellTag( CString strInnerID, CELL_POS startPos /*= CELL_POS_AZONE_CH1*/, CELL_POS endPos /*= CELL_POS_DZONE_CH5*/ )
{
	int nStart = (int)startPos;
	int nEnd = (int)endPos;
	CCellTag tag;
	CCellInfo *pCell;
	for(int i =nStart; i <= nEnd; i++ )
	{
		if(m_CellTag[i].IsExist())
		{
			pCell = GetCellInfo(m_CellTag[i]);
			if(pCell->defaultData.m_strInnerID == strInnerID)
			{
				tag = m_CellTag[i];
				break;
			}
		}
	}
	return tag;
}

void CCellBank::SetCellTag( CELL_POS pos, CCellTag tag )
{
	m_CellTag[pos] = tag;
}

CCellTag CCellBank::RemoveCellTag( CELL_POS pos )
{
	CCellTag tag = m_CellTag[pos];
	m_CellTag[pos].Init();
	return tag;
}

CCellTag CCellBank::RemoveCellTag( JIG_ID jig, JIG_CH nCh )
{
	int nTagIndex = CELL_POS_SHUTTLE1_CH1;
	nTagIndex += ((int)jig*JIG_CH_MAX) + (int)nCh;

	CCellTag tag =m_CellTag[nTagIndex];
	m_CellTag[nTagIndex].Init();

	return tag;
}

CCellTag CCellBank::CreateCellTag()
{
	CCellTag tag;
	tag.m_nIndex = m_nNextGenIndex;
	m_nNextGenIndex++;
	if(m_nNextGenIndex >= MAX_CELL_INFO_BUFFER)
		m_nNextGenIndex = 0;

	return tag;
}

void CCellBank::CreateCellInfo( CELL_POS pos )
{
	m_CellTag[pos] = CreateCellTag();
	GetCellInfo(m_CellTag[pos])->Init();
}

CCellInfo* CCellBank::GetCellInfo( CCellTag tag )
{
	// 없는 Cell이면 Tag를 초기화 해서 넘겨준다
	return (tag.IsExist()) ? &m_Cell[tag.m_nIndex] : NULL;
}

CCellInfo* CCellBank::GetCellInfo(CELL_POS pos )
{
	return GetCellInfo(m_CellTag[pos]);
}

CCellInfo* CCellBank::GetCellInfo( JIG_ID jig, JIG_CH nCh )
{
	return GetCellInfo(GetCellTag(jig, nCh));
}

CCellInfo* CCellBank::SearchCellInfo( JIG_ID nJig, JIG_CH nCh, CELL_POS startPos /*= CELL_POS_AZONE_CH1*/, CELL_POS endPos /*= CELL_POS_DZONE_CH5*/ )
{
	return GetCellInfo(SearchCellTag(nJig, nCh, startPos, endPos));	
}

CCellInfo* CCellBank::SearchCellInfo(int nJig, int nCh, CELL_POS startPos /*= CELL_POS_AZONE_CH1*/, CELL_POS endPos /*= CELL_POS_DZONE_CH5*/ )
{
	return SearchCellInfo((JIG_ID)nJig, (JIG_CH) nCh, startPos, endPos);
}

CCellInfo* CCellBank::SearchCellInfo(CString strCellID, CELL_POS startPos /*= CELL_POS_SHUTTLE1_CH1*/, CELL_POS endPos /*= CELL_POS_SHUTTLE3_CH2*/)
{
	CCellInfo *pCell;
	for(int i = startPos; i <= endPos; i++)
	{
		pCell = GetCellInfo((CELL_POS)i);
		if(pCell)
		{
			if(pCell->defaultData.m_strCellID == strCellID)
				return pCell;
		}
	}
	return NULL;
}

//kjpark 20161018 셀 Tag shift
void CCellBank::SetCellTagNextPosition(CELL_POS CurrentPos, CELL_POS NextPos)
{
	SetCellTag(NextPos, RemoveCellTag(CurrentPos));
}

BOOL CCellBank::WriteCellLog( CELL_POS pos )
{	
	CCellInfo *pCell = GetCellInfo(pos);
	if(pCell == NULL) return TRUE;

	if(pCell->defaultData.m_strCIMJudge == CIM_JUDGE_RETEST)
	{
		pCell->defaultData.m_strCIMJudge = _T("");
	}

	CTime time = CTime::GetCurrentTime();
	CEtc::ApplyTimeOffset(time, theConfigBank.m_Option.m_OffsetHour, 0);

	CString strDirPath;
	strDirPath.Format(_T("%s\\%02d\\CELL_DATA"), theConfigBank.m_System.m_strCellLogPath, time.GetMonth());

	if(CFileSupport::DirectoryCheck(strDirPath) == FALSE)
	{
		if(CFileSupport::CreateDirectory(strDirPath) == FALSE)
			return FALSE;
	}
	CString filepath, strOffline;

	if(theProcBank.GetCimState() == CONST_CIM_STATE::CIM_OFFLINE)
	{
		strOffline = ConstCimState[CONST_CIM_STATE::CIM_OFFLINE].strName;
		filepath.Format(_T("%s\\%d%02d%02d_CELL_%s_%s.csv"	)	, strDirPath  
			, time.GetYear(), time.GetMonth(), time.GetDay() 
			, theConfigBank.m_System.m_strLineNo, strOffline);
	}
	//CMiLRe 20170201 셀로그 파일이름 변경
	else
	{
		filepath.Format(_T("%s\\%d%02d%02d_CELL_%s.csv"	)	, strDirPath  
			, time.GetYear(), time.GetMonth(), time.GetDay() 
			, theConfigBank.m_System.m_strLineNo);
	}
	

	char szLogTextbuf[MAX_CELL_LOG_STRING];
	CString strLogText;
	// 처음 생성하는 거면 Title내용을 기록하기 위해 버퍼에 담아둔다 [11/16/2016 OSC]
	if(_taccess(filepath, 0) == -1)	// *csv가 실제 존재하는지 판별하자 (0 : 존재 -1 : 미존재);
	{
		strLogText = pCell->GetCellLogTitle();
	}
	
	// 파일명이 바꼈으면 기존에 open해 놓은건 닫고 새로 open한다 [11/16/2016 OSC]
	if( (filepath != m_fileCellData.GetFilePath()) && (m_fileCellData.GetFilePath().GetLength() > 1) )
	{
		m_fileCellData.Close();
	}
	if(m_fileCellData.GetFilePath().GetLength() < 1)
	{
		if(m_fileCellData.Open(filepath, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate | CFile::shareDenyWrite) == FALSE)  
		{
			m_fileCellData.Abort();

			// 파일이 깨졌을 경우 기존꺼를 이름을 바꾼다
			if(CFileSupport::FileCheck(filepath))
			{
				CString strBackupFile;
				strBackupFile.Format(_T("%s\\%d%02d%02d_CELL_%s_%s_BAK_%02d%02d%02d.csv"	)	, strDirPath  
					, time.GetYear(), time.GetMonth(), time.GetDay() 
					, theConfigBank.m_System.m_strLineNo, strOffline
					, time.GetHour(), time.GetMinute(), time.GetSecond());

				if(CFileSupport::MoveFile(filepath, strBackupFile) == FALSE)
				{
					return FALSE;
				}
			}
			else
			{
				return FALSE;
			}

			// 백업 성공했으면 다시 오픈 시도
			if(m_fileCellData.Open(filepath, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate | CFile::shareDenyWrite) == FALSE)  
			{
				m_fileCellData.Abort();
				return FALSE;
			}
		}
	}

	// Title 기록 [11/16/2016 OSC]
	if(strLogText.GetLength() > 0)
	{
		CEtc::ConvertMultibyte((wchar_t*)strLogText.GetString(), szLogTextbuf, MAX_CELL_LOG_STRING);
		m_fileCellData.SeekToEnd();
		m_fileCellData.Write(szLogTextbuf,strlen(szLogTextbuf));
	}

	// Cell Log 기록 [11/16/2016 OSC]
	strLogText = pCell->GetCellLogValue();
	if(strLogText.GetLength() > 0)
	{
		CEtc::ConvertMultibyte((wchar_t*)strLogText.GetString(), szLogTextbuf, MAX_CELL_LOG_STRING);
		m_fileCellData.SeekToEnd();
		m_fileCellData.Write(szLogTextbuf,strlen(szLogTextbuf));
	}

	//kjpark 20180114 NG 판정 통계 UI 추가
	if(pCell->defaultData.m_LastClass != GOOD_CELL)
		WriteNGResultLog(pCell, time);

	return TRUE;
}

BOOL CCellBank::WriteCellLogTemp( CELL_POS pos )
{
	CCellInfo *pCell = GetCellInfo(pos);
	if(pCell == NULL) return TRUE;

	CTime time = CTime::GetCurrentTime();
	CEtc::ApplyTimeOffset(time, theConfigBank.m_Option.m_OffsetHour, 0);

	CString strDirPath;
	strDirPath.Format(_T("%s\\%02d\\CELL_DATA"), theConfigBank.m_System.m_strCellLogPath, time.GetMonth());

	if(CFileSupport::DirectoryCheck(strDirPath) == FALSE)
	{
		if(CFileSupport::CreateDirectory(strDirPath) == FALSE)
			return FALSE;
	}
	CString filepath, strOffline;

	filepath.Format(_T("%s\\%d%02d%02d_CELL_%s_temp.csv")	, strDirPath  
		, time.GetYear(), time.GetMonth(), time.GetDay() 
		, theConfigBank.m_System.m_strLineNo);



	char szLogTextbuf[MAX_CELL_LOG_STRING];
	CString strLogText;
	// 처음 생성하는 거면 Title내용을 기록하기 위해 버퍼에 담아둔다 [11/16/2016 OSC]
	if(_taccess(filepath, 0) == -1)	// *csv가 실제 존재하는지 판별하자 (0 : 존재 -1 : 미존재);
	{
		strLogText = pCell->GetCellLogTitle();
	}

	// 파일명이 바꼈으면 기존에 open해 놓은건 닫고 새로 open한다 [11/16/2016 OSC]
	if( (filepath != m_fileCellDataTemp.GetFilePath()) && (m_fileCellDataTemp.GetFilePath().GetLength() > 1) )
	{
		m_fileCellDataTemp.Close();
	}
	if(m_fileCellDataTemp.GetFilePath().GetLength() < 1)
	{
		if(m_fileCellDataTemp.Open(filepath, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate | CFile::shareDenyWrite) == FALSE)  
		{
			//20171015 BKH, 오타 들어가서 에러발생, 제대로 수정하였음.
			m_fileCellDataTemp.Abort();

			// 파일이 깨졌을 경우 기존꺼를 이름을 바꾼다
			if(CFileSupport::FileCheck(filepath))
			{
				CString strBackupFile;
				strBackupFile.Format(_T("%s\\%d%02d%02d_CELL_%s_%s_BAK_%02d%02d%02d.csv"	)	, strDirPath  
					, time.GetYear(), time.GetMonth(), time.GetDay() 
					, theConfigBank.m_System.m_strLineNo, strOffline
					, time.GetHour(), time.GetMinute(), time.GetSecond());

				if(CFileSupport::MoveFile(filepath, strBackupFile) == FALSE)
				{
					return FALSE;
				}
			}
			else
			{
				return FALSE;
			}

			// 백업 성공했으면 다시 오픈 시도
			if(m_fileCellDataTemp.Open(filepath, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate | CFile::shareDenyWrite) == FALSE)  
			{
				m_fileCellDataTemp.Abort();
				return FALSE;
			}
		}
	}

	// Title 기록 [11/16/2016 OSC]
	if(strLogText.GetLength() > 0)
	{
		CEtc::ConvertMultibyte((wchar_t*)strLogText.GetString(), szLogTextbuf, MAX_CELL_LOG_STRING);
		m_fileCellDataTemp.SeekToEnd();
		m_fileCellDataTemp.Write(szLogTextbuf,strlen(szLogTextbuf));
	}

	// Cell Log 기록 [11/16/2016 OSC]
	strLogText = pCell->GetCellLogValue();
	if(strLogText.GetLength() > 0)
	{
		CEtc::ConvertMultibyte((wchar_t*)strLogText.GetString(), szLogTextbuf, MAX_CELL_LOG_STRING);
		m_fileCellDataTemp.SeekToEnd();
		m_fileCellDataTemp.Write(szLogTextbuf,strlen(szLogTextbuf));
	}
	m_fileCellDataTemp.Close();
	return TRUE;
}

BOOL CCellBank::WriteCellLog( JIG_ID jig, JIG_CH ch )
{
	CELL_POS pos = (CELL_POS)(CELL_POS_SHUTTLE1_CH1 + (jig*JIG_CH_MAX) + ch);

	return WriteCellLog(pos);
}

BOOL CCellBank::WriteCellLogTemp( JIG_ID jig, JIG_CH ch )
{
	CELL_POS pos = (CELL_POS)(CELL_POS_SHUTTLE1_CH1 + (jig*JIG_CH_MAX) + ch);

	return WriteCellLogTemp(pos);
}


//kjpark 20180114 NG 판정 통계 UI 추가
BOOL CCellBank::WriteNGResultLog(CCellInfo* pCell, CTime time)
{
	CString strStatisticsFileName;	
	CStdioFile file;
	CFileException e;
	CString szTitle, szLine;
	int nTimePart = 0;

	strStatisticsFileName.Format(_T("%s\\%04u-%02u-%02u\\NGResultStatistics.csv"	)	, theConfigBank.m_System.m_strDebugLogPath, time.GetYear(), time.GetMonth(), time.GetDay());

	if((int)time.GetHour()<19) //현재 시간에 맞춰서 작업조 설정
		nTimePart=DAY_TIME;
	else
		nTimePart=NIGHT_TIME;

	//kjpark 20180121 NG 판정 통계 UI 추가 Azone, MTPWrite, MTPVerify 별로 정리
	//kjpark 20180129 MCR reading fail 시 초기값 미지정으로 인한 버그 수정
	DEFECT_TYPE defectType = DEFECT_DEFAULT;
	if(pCell->m_AZone.m_Class != GOOD_CELL)
		defectType = AZONE_DEFECT_NG;
	else if(pCell->m_MTPWrite.m_Class != GOOD_CELL)
		defectType = MTP_WRITE_DEFECT_NG;
	else if(pCell->m_MTPVerify.m_Class != GOOD_CELL)
		defectType = MTP_VIRIFY_DEFECT_NG;

	szLine.Format(_T("%d,%d,%d,%d,%s"), pCell->defaultData.m_JigId, pCell->defaultData.m_JigCh, nTimePart, (int)defectType, pCell->defaultData.m_strLastResult);
	if ( file.Open(strStatisticsFileName.GetBuffer(), CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate, &e ) )
	{
		file.SeekToEnd();
		file.WriteString(szLine+_T("\n"));
		file.Close();
	}
	else
	{
		theLog[LOG_NG_RESULT_STATISTICS].AddBuf(_T("NGResultStatistics.csv 기록실패 %s"), szLine);
		return FALSE;
	}
	return TRUE;
}

CCellInfo* CCellBank::GetCellInfo_CellLoading(JIG_ID jig, JIG_CH nCh)
{
	return &m_InfoCellLoading[jig][nCh];
}

void CCellBank::BackupCellToFile()
{
	// 프로그램 재시작시 CellInfo복원.. 다 저장할 필요 없이
	// 종료 전에 CIM에 O 보고하여 새로 돌릴 수 있게 끔 해 놓고
	// TrackIn 관련 정보만 저장해 놓는다. [12/23/2017 OSC]

	CString strPath;
	strPath.Format(_T("%s\\BackupCell.par"), theConfigBank.m_System.m_strConfigPath);

	if(CFileSupport::FileCheck(strPath))
	{
		// File 내용을 깔끔하게 하기 위해 기존껀 지우고 새로 적는다
		::DeleteFile(strPath);
	}

	FILE *fStream;
	errno_t e = _tfopen_s(&fStream, strPath.GetString(), _T("w,ccs=UTF-8"));
	if (e != 0) return;
	CStdioFile		file(fStream);
	CString strLine;


	CCellInfo *pCell;

	// CELL_POS별 CellInfo 저장
	CString strSection;
	for(int i = 0; i < CELL_POS_MAX; i++)
	{
		pCell = GetCellInfo((CELL_POS)i);
		if(pCell)
		{
			strLine.Format(_T("CELL_START|%02d"), i);
			file.WriteString(strLine);

			strLine.Format(_T("m_strInnerID|%s"), pCell->defaultData.m_strInnerID);
			file.WriteString(strLine);

			strLine.Format(_T("m_JigId|%d"), pCell->defaultData.m_JigId);
			file.WriteString(strLine);

			strLine.Format(_T("m_JigCh|%d"), pCell->defaultData.m_JigCh);
			file.WriteString(strLine);

			strLine.Format(_T("m_strProductID|%s"), pCell->defaultData.m_strProductID);
			file.WriteString(strLine);

			strLine.Format(_T("m_strCellInfoResult|%s"), pCell->defaultData.m_strCellInfoResult);
			file.WriteString(strLine);

			strLine.Format(_T("m_strStepID|%s"), pCell->defaultData.m_strStepID);
			file.WriteString(strLine);

			strLine.Format(_T("m_CellInfoResult|%d"), pCell->defaultData.m_CellInfoResult);
			file.WriteString(strLine);

			strLine.Format(_T("m_nInspectInvalidType|%d"), pCell->defaultData.m_nInspectInvalidType);
			file.WriteString(strLine);

			strLine.Format(_T("m_bTrackinFinish|%d"), pCell->defaultData.m_bTrackinFinish);
			file.WriteString(strLine);

			strLine.Format(_T("m_bRetryAble|%d"), pCell->defaultData.m_bRetryAble);
			file.WriteString(strLine);

			strLine.Format(_T("m_bRetryAB|%d"), pCell->defaultData.m_bRetryAB);
			file.WriteString(strLine);

			strLine.Format(_T("m_strABRule|%s"), pCell->defaultData.m_strABRule);
			file.WriteString(strLine);

			strLine.Format(_T("m_strAResult|%s"), pCell->defaultData.m_strAResult);
			file.WriteString(strLine);

			strLine.Format(_T("CELL_END|%02d"), i);
			file.WriteString(strLine);
		}
	}

	file.Close();
}

void CCellBank::RestoreCellToFile()
{
	// 프로그램 재시작시 CellInfo복원.. 다 저장할 필요 없이
	// 종료 전에 CIM에 O 보고하여 새로 돌릴 수 있게 끔 해 놓고
	// TrackIn 관련 정보만 저장해 놓는다. [12/23/2017 OSC]

	CString strPath;
	strPath.Format(_T("%s\\BackupCell.par"), theConfigBank.m_System.m_strConfigPath);

	if(CFileSupport::FileCheck(strPath) == FALSE)
		return;

	FILE *fStream;
	errno_t e = _tfopen_s(&fStream, strPath.GetString(), _T("r,ccs=UTF-8"));
	if (e != 0) return;
	CStdioFile		file(fStream);

	CString strLine, strName, strValue;
	CELL_POS pos;
	CCellInfo *pCell = NULL;
	while(file.ReadString(strLine))
	{
		::AfxExtractSubString( strName, strLine	, 0, '|');
		::AfxExtractSubString( strValue, strLine	, 1, '|');

		// 시작이면 tag 생성
		if(strName == _T("CELL_START"))
		{
			pos = (CELL_POS)_tstoi(strValue);
			CreateCellInfo(pos);
			pCell = GetCellInfo(pos);
		}

		if(pCell == NULL)
			continue;

		if(strName == _T("m_strInnerID")			)	pCell->defaultData.m_strInnerID = strValue;
		if(strName == _T("m_JigId")					)	pCell->defaultData.m_JigId = (JIG_ID)_tstoi(strValue);
		if(strName == _T("m_JigCh")					)	pCell->defaultData.m_JigCh = (JIG_CH)_tstoi(strValue);
		if(strName == _T("m_strProductID")			)	pCell->defaultData.m_strProductID = strValue;
		if(strName == _T("m_strCellInfoResult")		)	pCell->defaultData.m_strCellInfoResult = strValue;
		if(strName == _T("m_nInspectInvalidType")	)	pCell->defaultData.m_nInspectInvalidType = (JOB_PROCESS_STATE)_tstoi(strValue);
		if(strName == _T("m_bTrackinFinish")		)	pCell->defaultData.m_bTrackinFinish = (BOOL)_tstoi(strValue);
		if(strName == _T("m_bRetryAble")			)	pCell->defaultData.m_bRetryAble = (BOOL)_tstoi(strValue);
		if(strName == _T("m_bRetryAB")				)	pCell->defaultData.m_bRetryAB = (BOOL)_tstoi(strValue);
		if(strName == _T("m_strABRule")				)	pCell->defaultData.m_strABRule = strValue;
		if(strName == _T("m_strAResult")			)	pCell->defaultData.m_strAResult = strValue;

		// 종료
		if(strName == _T("CELL_END"))		pCell = NULL;
	}

	file.Close();

	// 복원 끝나면 이름을 변경하여 다시 불러오지 않게 한다
	CString strBackupPath;
	strBackupPath.Format(_T("%s\\BackupCell.bak"), theConfigBank.m_System.m_strConfigPath);
	::DeleteFile(strBackupPath);
	CFileSupport::MoveFile(strPath, strBackupPath);
}
