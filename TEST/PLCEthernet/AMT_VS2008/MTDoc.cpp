// MTDoc.cpp : CMTDoc 클래스의 구현
//

#include "stdafx.h"
#include "MT.h"
#include "MTDoc.h"
#include "MainFrm.h"
#include "MMCWFP39.h"
#include "ActDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// 다른 데서 참조 못하게 여기서 선언
#define DCF_PATH					SYSTEM_PATH"\\VCCG20V30-nomal.dcf"

// Camera FOV
// 주석해 놓은 것은 기존 Camera
// #define	RFOV_X											0.01003									// 8.32 * 6.24 / 640 or 480 = 13um
// #define	RFOV_Y											0.01003									// 8.32 * 6.24 / 640 or 480 = 13um
#define	RFOV_X											0.01481										// 9.6 / 648
#define	RFOV_Y											0.01457										// 7.2 / 494
//////////////////////////////////////////////////////////////////////////


// CMTDoc

IMPLEMENT_DYNCREATE(CMTDoc, CDocument)

BEGIN_MESSAGE_MAP(CMTDoc, CDocument)
END_MESSAGE_MAP()


// CMTDoc 생성/소멸

CMTDoc::CMTDoc()
{
	SetSuperUser(FALSE);	// 임시로 항시 로그인
	gl_pDoc = this;
	m_pDeviceNet = NULL;
	m_pMotion = NULL;
	m_pAmtData = NULL;
	m_pThreadLiveGrap = NULL;
	m_pLiveThread = NULL;
	m_bDeviceChange = FALSE;
	
	m_nInsp_Count = 0;
	m_bInspCount = FALSE;

	m_pMotorParam = (STRUCT_MOTOR_PARA*) new STRUCT_MOTOR_PARA[MAX_AXIS_BUFFER];

	ZeroMemory( m_pMotorParam , MAX_AXIS_BUFFER * sizeof(STRUCT_MOTOR_PARA));
	ZeroMemory( m_bLiveGrapFlag , MAX_CAM_NUM * sizeof(BOOL) );
	ZeroMemory( m_bLightOn , id_Max * sizeof(BOOL) );
	ZeroMemory( m_nBinarizeFPC , MAX_CAM_NUM * sizeof(long) );
	ZeroMemory( m_nBinarizePCB , MAX_CAM_NUM * sizeof(long) );


	m_pAmtData = new CAmtData();
	// Panel Model에 따라 변하지 않는 것 부터 먼저 불러온다
	LoadConfigFile();
	// Non Stop Device ID Read
	FileOpenModelAtDevice();

	SetSerialArray();
	SetErrorCode();
	ReadBrightness();

	if(GetDoc()->m_bPlcEthernetType == FALSE)
		gl_pPLCComm = &m_Melsec;

	OpenModelFile(m_pAmtData->m_CurModelInfo.strModelName);

	m_NonStopDeviceID = _T("");


	m_pDeviceNet = new CCIF32;
	m_pMotion = new CMMC39;
	m_pEqMainSeq = new CEqMainSequence;
	m_pThreadLiveGrap = new CThreadLiveGrap;
	
	if( ! m_pDeviceNet->OpenCard() )
	{
		//여기서..Can't Open Error Card 처리.. (Error Msg)
		m_pAmtData->m_Flag.bInitDeviceNet = FALSE;
	}
	else
	{
		m_pAmtData->m_Flag.bInitDeviceNet = TRUE ;
	}
	
	if( !m_pMotion->InitMotion() )
	{
		//여기서..Can't Open Error Card 처리.. (Error Msg)
		m_pAmtData->m_Flag.bInitMotion = FALSE;
	}
	else
	{
		m_pAmtData->m_Flag.bInitMotion = TRUE ;
	}
	m_dMotionstrokeSingleX = 140;
	m_dMotionstrokeSingleY = 80;
	m_dMotionstrokeFullX = 180;

	ApplyCamParamToVision();
	GetAlignCtl()->GetVision()->Open(GetApp()->m_strDCFPath);

	int nFPCNum = 0;
	for(int nCamIndex = CAM_0; nCamIndex < MAX_CAM_NUM; nCamIndex++)
	{
		/*nFPCNum = m_pAmtData->m_CurModelInfo.nFPCModelNum[nCamIndex];
		for(int i = 0; i < nFPCNum; i++)
			GetAlignCtl()->LoadPattern( ((nCamIndex*MAX_CAM_NUM)+i), &m_pAmtData->m_CurModelInfo );
		GetAlignCtl()->LoadPattern( (nCamIndex*MAX_CAM_NUM)+3, &m_pAmtData->m_CurModelInfo);*/
		//  [6/9/2009 janghan]
		GetAlignCtl()->LoadPattern( (nCamIndex*MAX_CAM_NUM+0), &m_pAmtData->m_CurModelInfo);
		GetAlignCtl()->LoadPattern( (nCamIndex*MAX_CAM_NUM+1), &m_pAmtData->m_CurModelInfo);
		GetAlignCtl()->LoadPattern( (nCamIndex*MAX_CAM_NUM)+2, &m_pAmtData->m_CurModelInfo);
		GetAlignCtl()->LoadPattern( (nCamIndex*MAX_CAM_NUM)+3, &m_pAmtData->m_CurModelInfo);

	}
	SetMotorParameter();
	ReadTiltPos();

	GetTowerLamp();
	GetDeviceCtl()->SetAmtData(m_pAmtData);
	GetDeviceCtl()->SetMMC(m_pMotion);

// 	// NonStop Test
// 	STRUCT_DEVICE a;
// 	a.strDeviceName = "aaa460AA-01";
// 	a.strMoDelName = "46_2FFC_NORMAL";
// 	a.iPanelSize = 46;
// 	m_AryDevice.Add(a);
// 	FileSaveModelAtDevice();

	m_bUsed_Tune = FALSE;//  [7/6/2009 janghan]
	m_nYaxis_Back = 99;
	m_bNonStop_Run = FALSE; //  [8/17/2009 janghan]

	m_bInspCount = FALSE;		// Insp Count 처음에는 False로 정의한다. [12/2/2009 JJH-RomanticBoy]

	SYSTEMTIME	time;//  [9/10/2009 janghan]
	GetLocalTime(&time);//  [9/10/2009 janghan]

	m_nHourCheck = time.wHour;//  [9/10/2009 janghan]
}
 
CMTDoc::~CMTDoc()
{
	if( m_pDeviceNet )
	{
		m_pDeviceNet->CloseCard();
		delete m_pDeviceNet;
	}
	GetAlignCtl()->GetVision()->Close();

	if( m_pMotion )		delete m_pMotion;
	if( m_pMotorParam )		delete m_pMotorParam;
	if( m_pAmtData )		delete m_pAmtData;
}

BOOL CMTDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}



// CMTDoc serialization

void CMTDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}


// CMTDoc 진단

#ifdef _DEBUG
void CMTDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMTDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMTDoc 명령

void Log(LPCSTR format, int iLogID)
{

	CString strFile_Name , strMsg , strLogPath;

	SYSTEMTIME	time;
	GetLocalTime(&time);

	GetDoc()->m_strDayLogDirName.Format("%s\\%4u%02u%02u" , GetApp()->m_strLogPath , time.wYear , time.wMonth , time.wDay );

	_mkdir(GetDoc()->m_strDayLogDirName);

	switch( iLogID )
	{
// 	case LOG_VI:				strFile_Name = GetDoc()->m_strDayLogDirName + "\\VILog.txt";							break;
	case LOG_XNET:				strFile_Name = GetDoc()->m_strDayLogDirName + "\\XNetLog.txt";							break;
	case LOG_ERROR:				strFile_Name = GetDoc()->m_strDayLogDirName + "\\ErrorLog.txt";							break;
	case LOG_LOGIN:				strFile_Name = GetDoc()->m_strDayLogDirName + "\\Login.txt";							break;
	case LOG_GLASS_DATA:		strFile_Name = GetDoc()->m_strDayLogDirName + "\\GlassData.txt";						break;
	case LOG_COMM:				strFile_Name = GetDoc()->m_strDayLogDirName + "\\Comm.txt";								break;
// 	case LOG_INSPECTION:		strFile_Name = GetDoc()->m_strDayLogDirName + "\\VILog.txt";							break;
	case LOG_EQ:				strFile_Name.Format("%s\\Log_%02u.txt" , GetDoc()->m_strDayLogDirName , time.wHour );	break;
	case LOG_SYSTEM:			strFile_Name = GetDoc()->m_strDayLogDirName + "\\SystemLog.txt";						break;
	case LOG_ALIGN:				strFile_Name = GetDoc()->m_strDayLogDirName + "\\Align.txt";							break;
	case LOG_MOVE_COMPLET:		strFile_Name = GetDoc()->m_strDayLogDirName + "\\Move.txt";								break;
	case LOG_MOVE_PULSE:		strFile_Name = GetDoc()->m_strDayLogDirName + "\\Move_Pulses.txt";						break;
	case LOG_AUTOTUNE:          strFile_Name = GetDoc()->m_strDayLogDirName + "\\AutoTune.txt";	    					break;
	case LOG_TILT_ERR:			strFile_Name = GetDoc()->m_strDayLogDirName + "\\TiltErr.txt";							break;//  [8/13/2009 janghan]
	case LOG_Reset_Count:		strFile_Name = GetDoc()->m_strDayLogDirName + "\\InspectionCount.txt";					break;//  [9/10/2009 janghan]

	default:
			break;
	}

	FILE *stream;
	if(fopen_s( &stream, strFile_Name.GetBuffer(0), "a+t" ) == 0 )
	{
 		fprintf(stream, "%02u:%02u:%02u:%03u\t%s\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, format);
		fclose(stream);
	}
	else
	{
		TRACE("Log File open Fail! (%s)", strFile_Name);
	}
}

void CMTDoc::SetLog(CString strLogMsg, int iMsgID)
{
	Log(strLogMsg , iMsgID);
}


CString CMTDoc::GetDataFromString(CString inStr, CString startStr, CString endStr)
{
	CString strReturn;
	int iStart	= inStr.Find(startStr);	
	int iEnd	= inStr.Find(endStr, iStart);
	if (iStart < iEnd)
	{
		strReturn = inStr.Mid(iStart + startStr.GetLength(), iEnd - iStart - startStr.GetLength());
		return strReturn;
	}
	else if (iStart > iEnd)
	{
		ASSERT(FALSE);
	}
	
	return "";
}

void CMTDoc::OpenModelFile(CString strModelName)
{
	//////////////////////////////////////////////////////////////////////////
	// CIni class 사용 [1/22/2009 OSC]

	CString strDirFilePath, strSection, strEntry, strFilePath;

	strDirFilePath.Format("%s\\%s\\System.INI" , GetApp()->m_strRecipePath ,strModelName );

	CIni ini;	
	ini.SetIniFilename(strDirFilePath);
	
	m_pAmtData->m_CurModelInfo.strModelName = strModelName ;															
																														
	strSection = _T("MODEL INFO");																						
	m_pAmtData->m_CurModelInfo.iPanelSize		= 		ini.GetLong(strSection, "Panel_Size", 40);									
	m_pAmtData->m_CurModelInfo.iContactNum		= 		ini.GetLong(strSection, "ContactNum", 2);									
	m_pAmtData->m_CurModelInfo.iContactor		= 		ini.GetLong(strSection, "Contactor", 0);									
	m_pAmtData->m_CurModelInfo.iContactType		= 		ini.GetLong(strSection, "ContactType", 0);
	m_pAmtData->m_CurModelInfo.iFPCLength		= 		ini.GetLong(strSection, "FPC_Length", 0);
	m_pAmtData->m_CurModelInfo.strResource		= 		ini.GetString(strSection, "Resource", _T("0"));							
	m_pAmtData->m_CurModelInfo.iAlignScore		= 		ini.GetLong(strSection, "Align Score", 70);								
	m_pAmtData->m_CurModelInfo.bAlignAngle		= 		ini.GetBoolean(strSection,"Align Angle", FALSE);
	m_pAmtData->m_CurModelInfo.strDeviceName	= 		ini.GetString(strSection, "DeviceName", _T("0"));							
	m_pAmtData->m_CurModelInfo.iPinNum			= 		ini.GetLong(strSection, "Contact Pin Num", 0);							

	m_pAmtData->m_CurModelInfo.strGripper1		= 		ini.GetString(strSection, "Gripper1", _T("0"));							
	m_pAmtData->m_CurModelInfo.strGripper2		= 		ini.GetString(strSection, "Gripper2", _T("0"));							
	m_pAmtData->m_CurModelInfo.strGripper3		= 		ini.GetString(strSection, "Gripper3", _T("0"));							
	m_pAmtData->m_CurModelInfo.strGripper4		= 		ini.GetString(strSection, "Gripper4", _T("0"));							

	strSection = _T("BLU Model");																						
	m_pAmtData->m_CurModelInfo.m_nBluModel = ini.GetLong(strSection, "BLU_Model", BluModel_WT);							

	strSection	=	_T("Insp Count");
	m_bInspCount								=		ini.GetBoolean(strSection, "Insp Count Bool", FALSE);

	if(CFileSupport::FileCheck(strFilePath))
	{
		FILE* fout;
		CString strMsg;
		char cTemp[81];
		ZeroMemory(cTemp,81);
		if(fopen_s(&fout, strFilePath.GetBuffer(0) , "r") == 0)
		{
			fscanf_s(fout , "%s" , cTemp , 81);
			strMsg.Format("%s",cTemp);
			m_pAmtData->m_CurModelInfo.strHandle = strMsg;
			fclose(fout);
		}
	}
	else
		m_pAmtData->m_CurModelInfo.strHandle = _T("");

	CString strText;
	int nCamIndex;
	strSection = _T("FPC PTR DATA");
	for(int n = 0; n < MAX_MOD_BUFFER_NUM; n++)
	{
		switch (n)
		{
		case CAM_0_FPC_0:
		case CAM_0_FPC_1:
		//case CAM_0_FPC_2:
		case CAM_1_FPC_0:
		case CAM_1_FPC_1:
		//case CAM_1_FPC_2:
		case CAM_2_FPC_0:
		case CAM_2_FPC_1:
		//case CAM_2_FPC_2:
		case CAM_3_FPC_0:
		case CAM_3_FPC_1:
		//case CAM_3_FPC_2:
			strText = _T("FPC");
			break;
		case CAM_0_PCB_0:
		case CAM_0_PCB_1:
		case CAM_1_PCB_0:
		case CAM_1_PCB_1:
		case CAM_2_PCB_0:
		case CAM_2_PCB_1:
		case CAM_3_PCB_0:
		case CAM_3_PCB_1:
			strText = _T("PCB");
			break;
		}
		nCamIndex = n/MAX_CAM_NUM;
		strEntry.Format("CAM_%d_%s_%d_left" , nCamIndex,  strText, n - (nCamIndex*MAX_CAM_NUM) );
		m_pAmtData->m_CurModelInfo.rtPattern[n].left= ini.GetLong(strSection, strEntry, 1);
		strEntry.Format("CAM_%d_%s_%d_top" , nCamIndex,  strText, n - (nCamIndex*MAX_CAM_NUM) );
		m_pAmtData->m_CurModelInfo.rtPattern[n].top = ini.GetLong(strSection, strEntry, 1);
		strEntry.Format("CAM_%d_%s_%d_right" , nCamIndex,  strText, n - (nCamIndex*MAX_CAM_NUM) );
		m_pAmtData->m_CurModelInfo.rtPattern[n].right= ini.GetLong(strSection, strEntry, 1);
		strEntry.Format("CAM_%d_%s_%d_bottom" , nCamIndex,  strText, n - (nCamIndex*MAX_CAM_NUM) );
		m_pAmtData->m_CurModelInfo.rtPattern[n].bottom = ini.GetLong(strSection, strEntry, 1);
		strEntry.Format("CAM_%d_%s_%d_CenterX" , nCamIndex,  strText, n - (nCamIndex*MAX_CAM_NUM) );
		m_pAmtData->m_CurModelInfo.ptCenter[n].x = ini.GetLong(strSection, strEntry, 1);
		strEntry.Format("CAM_%d_%s_%d_CenterY" , nCamIndex,  strText, n - (nCamIndex*MAX_CAM_NUM) );
		m_pAmtData->m_CurModelInfo.ptCenter[n].y = ini.GetLong(strSection, strEntry, 1);
	}
	
	for(int i=0 ; i < MAX_CAM_NUM ; i ++ )
	{
		strSection = _T("FPC PTR DATA");
		strEntry.Format("CAM_%d_FPC_Num", i);
		m_pAmtData->m_CurModelInfo.nFPCModelNum[i] = ini.GetLong(strSection, strEntry, 0);

		strSection = _T("POSITION");
		strEntry.Format("RDY_CCD%d_X" , i + 1 );
		m_pAmtData->m_CurModelInfo.ReadyPos[i].dx = ini.GetDouble(strSection, strEntry, 0.0);
		strEntry.Format("RDY_CCD%d_Y" , i + 1 );
		m_pAmtData->m_CurModelInfo.ReadyPos[i].dy = ini.GetDouble(strSection, strEntry, 0.0);
		strEntry.Format("RDY_CCD%d_Z" , i + 1 );
		m_pAmtData->m_CurModelInfo.ReadyPos[i].dz = ini.GetDouble(strSection, strEntry, 0.0);

		strEntry.Format("GRAP_CCD%d_X" , i + 1 );
		m_pAmtData->m_CurModelInfo.GrapPos[i].dx = ini.GetDouble(strSection, strEntry, 0.0);
		strEntry.Format("GRAP_CCD%d_Y" , i + 1 );
		m_pAmtData->m_CurModelInfo.GrapPos[i].dy = ini.GetDouble(strSection, strEntry, 0.0);
		strEntry.Format("GRAP_CCD%d_Z" , i + 1 );
		m_pAmtData->m_CurModelInfo.GrapPos[i].dz = ini.GetDouble(strSection, strEntry, 0.0);

		strSection = _T("OFFSET");
		strEntry.Format("FPCX%d" , i +1 );
		m_pAmtData->m_CurModelInfo.TV_PCBX[i] = ini.GetDouble(strSection, strEntry, 0.0);
		strEntry.Format("FPCY%d" , i +1 );
		m_pAmtData->m_CurModelInfo.TV_FPCY[i] = ini.GetDouble(strSection, strEntry, 0.0);
	}

	strSection = _T("OFFSET");
	strEntry = _T("STAGEOFFSET");
	m_pAmtData->m_CurModelInfo.TV_StageReady = ini.GetDouble(strSection, strEntry, 0.0);

	if(m_bUseBinary)	OpenBinarizeModelFile();
}

void CMTDoc::OpenBinarizeModelFile()
{
	//////////////////////////////////////////////////////////////////////////
	// Binarize 사용하면 RECEIPE 폴더에 있는 걸로 불러온다

	CString strDirFilePath, strSection, strEntry, strFilePath;

	strDirFilePath.Format("%s\\System.INI" , GetApp()->m_strRecipePath );

	CIni ini;	
	ini.SetIniFilename(strDirFilePath);

	CString strText;
	int nCamIndex;
	strSection = _T("FPC PTR DATA");
	for(int i = 0; i < MAX_CAM_NUM; i++)
	{
		strEntry.Format("CAM_%d_FPC_Num", i);
		m_pAmtData->m_CurModelInfo.nFPCModelNum[i] = ini.GetLong(strSection, strEntry, 1);
	}

	for(int n = CAM_0_FPC_0; n < MAX_MOD_BUFFER_NUM; n ++)
	{
		switch (n)
		{
		case CAM_0_FPC_0:
		case CAM_0_FPC_1:
		//case CAM_0_FPC_2:
		case CAM_1_FPC_0:
		case CAM_1_FPC_1:
		//case CAM_1_FPC_2:
		case CAM_2_FPC_0:
		case CAM_2_FPC_1:
		//case CAM_2_FPC_2:
		case CAM_3_FPC_0:
		case CAM_3_FPC_1:
		//case CAM_3_FPC_2:
			strText = _T("FPC");
			break;
		case CAM_0_PCB_0:
		case CAM_0_PCB_1:
		case CAM_1_PCB_0:
		case CAM_1_PCB_1:
		case CAM_2_PCB_0:
		case CAM_2_PCB_1:
		case CAM_3_PCB_0:
		case CAM_3_PCB_1:

			strText = _T("PCB");
			break;
		}
		nCamIndex = n/MAX_CAM_NUM;
		strEntry.Format("CAM_%d_%s_%d_left" , nCamIndex,  strText, n - (nCamIndex*MAX_CAM_NUM) );
		m_pAmtData->m_CurModelInfo.rtPattern[n].left= ini.GetLong(strSection, strEntry, 1);
		strEntry.Format("CAM_%d_%s_%d_top" , nCamIndex,  strText, n - (nCamIndex*MAX_CAM_NUM) );
		m_pAmtData->m_CurModelInfo.rtPattern[n].top = ini.GetLong(strSection, strEntry, 1);
		strEntry.Format("CAM_%d_%s_%d_right" , nCamIndex,  strText, n - (nCamIndex*MAX_CAM_NUM) );
		m_pAmtData->m_CurModelInfo.rtPattern[n].right= ini.GetLong(strSection, strEntry, 1);
		strEntry.Format("CAM_%d_%s_%d_bottom" , nCamIndex,  strText, n - (nCamIndex*MAX_CAM_NUM) );
		m_pAmtData->m_CurModelInfo.rtPattern[n].bottom = ini.GetLong(strSection, strEntry, 1);
		strEntry.Format("CAM_%d_%s_%d_CenterX" , nCamIndex,  strText, n - (nCamIndex*MAX_CAM_NUM) );
		m_pAmtData->m_CurModelInfo.ptCenter[n].x = ini.GetLong(strSection, strEntry, 1);
		strEntry.Format("CAM_%d_%s_%d_CenterY" , nCamIndex,  strText, n - (nCamIndex*MAX_CAM_NUM) );
		m_pAmtData->m_CurModelInfo.ptCenter[n].y = ini.GetLong(strSection, strEntry, 1);
	}
}

void CMTDoc::SaveModelFile(void)
{
	CString strFilePath, strSection, strEntry;

	strFilePath.Format("%s\\%s\\" , GetApp()->m_strRecipePath , m_pAmtData->m_CurModelInfo.strModelName );

	_mkdir(strFilePath);

	strFilePath += "System.INI";

	CIni ini(strFilePath);

	// 이전 내용 삭제
	strSection = _T("MODEL INFO");
	ini.DeleteSection(strSection);
	strSection = _T("FPC PTR DATA");
	ini.DeleteSection(strSection);
	strSection = _T("POSITION");
	ini.DeleteSection(strSection);
	strSection = _T("OFFSET");
	ini.DeleteSection(strSection);

	strSection	=			_T("MODEL INFO");
	strEntry	=			_T("Panel_Size");			ini.WriteLong		(strSection,			strEntry, 		m_pAmtData->m_CurModelInfo.iPanelSize);
	strEntry	=			_T("ContactNum");			ini.WriteLong		(strSection,			strEntry, 		m_pAmtData->m_CurModelInfo.iContactNum);
	strEntry	=			_T("Contactor");			ini.WriteLong		(strSection,			strEntry, 		m_pAmtData->m_CurModelInfo.iContactor);
	strEntry	=			_T("ContactType");			ini.WriteLong		(strSection,			strEntry, 		m_pAmtData->m_CurModelInfo.iContactType);
	strEntry	=			_T("FPC_Length");			ini.WriteLong		(strSection,			strEntry,		m_pAmtData->m_CurModelInfo.iFPCLength);
	strEntry	=			_T("Resource");				ini.WriteString		(strSection, 			strEntry, 		m_pAmtData->m_CurModelInfo.strResource);
	strEntry	=			_T("Align Score");			ini.WriteLong		(strSection,			strEntry, 		m_pAmtData->m_CurModelInfo.iAlignScore);
	strEntry	=			_T("Align Angle");			ini.WriteBoolean	(strSection,			strEntry, 		m_pAmtData->m_CurModelInfo.bAlignAngle);
	strEntry	=			_T("DeviceName");			ini.WriteString		(strSection, 			strEntry, 		m_pAmtData->m_CurModelInfo.strDeviceName);
	strEntry	=			_T("Contact Pin Num");		ini.WriteLong		(strSection, 			strEntry, 		m_pAmtData->m_CurModelInfo.iPinNum);

	strEntry	=			_T("Gripper1");				ini.WriteString		(strSection, 			strEntry, 		m_pAmtData->m_CurModelInfo.strGripper1);
	strEntry	=			_T("Gripper2");				ini.WriteString		(strSection, 			strEntry, 		m_pAmtData->m_CurModelInfo.strGripper2);
	strEntry	=			_T("Gripper3");				ini.WriteString		(strSection, 			strEntry, 		m_pAmtData->m_CurModelInfo.strGripper3);
	strEntry	=			_T("Gripper4");				ini.WriteString		(strSection, 			strEntry, 		m_pAmtData->m_CurModelInfo.strGripper4);

	strSection	=			_T("BLU Model");
	strEntry	=			_T("BLU_Model");			ini.WriteLong		(strSection,			strEntry, 		m_pAmtData->m_CurModelInfo.m_nBluModel);

	strSection	=			_T("Insp Count");
	strEntry	=			_T("Insp Count Bool");		ini.WriteBoolean	(strSection,			strEntry, 		m_bInspCount);

	SaveCurrentModelName();

	CString strText;
	int nCamIndex;
	strSection = _T("FPC PTR DATA");
	for(int n = 0; n < MAX_MOD_BUFFER_NUM; n++)
	{
		switch (n)
		{
		case CAM_0_FPC_0:
		case CAM_0_FPC_1:
		//case CAM_0_FPC_2:
		case CAM_1_FPC_0:
		case CAM_1_FPC_1:
		//case CAM_1_FPC_2:
		case CAM_2_FPC_0:
		case CAM_2_FPC_1:
		//case CAM_2_FPC_2:
		case CAM_3_FPC_0:
		case CAM_3_FPC_1:
		//case CAM_3_FPC_2:
			strText = _T("FPC");
			break;
		case CAM_0_PCB_0:
		case CAM_0_PCB_1:
		case CAM_1_PCB_0:
		case CAM_1_PCB_1:
		case CAM_2_PCB_0:
		case CAM_2_PCB_1:
		case CAM_3_PCB_0:
		case CAM_3_PCB_1:
			strText = _T("PCB");
			break;
		}
		nCamIndex = n/MAX_CAM_NUM;
		strEntry.Format("CAM_%d_%s_%d_left" , nCamIndex,  strText, n - (nCamIndex*MAX_CAM_NUM) );
		ini.WriteLong(strSection, strEntry, m_pAmtData->m_CurModelInfo.rtPattern[n].left);
		strEntry.Format("CAM_%d_%s_%d_top" , nCamIndex,  strText, n - (nCamIndex*MAX_CAM_NUM) );
		ini.WriteLong(strSection, strEntry, m_pAmtData->m_CurModelInfo.rtPattern[n].top);
		strEntry.Format("CAM_%d_%s_%d_right" , nCamIndex,  strText, n - (nCamIndex*MAX_CAM_NUM) );
		ini.WriteLong(strSection, strEntry, m_pAmtData->m_CurModelInfo.rtPattern[n].right);
		strEntry.Format("CAM_%d_%s_%d_bottom" , nCamIndex,  strText, n - (nCamIndex*MAX_CAM_NUM) );
		ini.WriteLong(strSection, strEntry, m_pAmtData->m_CurModelInfo.rtPattern[n].bottom);

		strEntry.Format("CAM_%d_%s_%d_CenterX" , nCamIndex,  strText, n - (nCamIndex*MAX_CAM_NUM) );
		ini.WriteLong(strSection, strEntry, m_pAmtData->m_CurModelInfo.ptCenter[n].x);
		strEntry.Format("CAM_%d_%s_%d_CenterY" , nCamIndex,  strText, n - (nCamIndex*MAX_CAM_NUM) );
		ini.WriteLong(strSection, strEntry, m_pAmtData->m_CurModelInfo.ptCenter[n].y);
	}

	for( int i = 0 ; i < MAX_CAM_NUM ; i ++ )
	{
		strSection = _T("FPC PTR DATA");
		strEntry.Format("CAM_%d_FPC_Num", i);
		ini.WriteLong(strSection, strEntry, m_pAmtData->m_CurModelInfo.nFPCModelNum[i]);

		strSection = _T("POSITION");
		strEntry.Format("RDY_CCD%d_X", i + 1);	ini.WriteDouble(strSection, strEntry, m_pAmtData->m_CurModelInfo.ReadyPos[i].dx);
		strEntry.Format("RDY_CCD%d_Y", i + 1);	ini.WriteDouble(strSection, strEntry, m_pAmtData->m_CurModelInfo.ReadyPos[i].dy);
		strEntry.Format("RDY_CCD%d_Z", i + 1);	ini.WriteDouble(strSection, strEntry, m_pAmtData->m_CurModelInfo.ReadyPos[i].dz);

		strEntry.Format("GRAP_CCD%d_X", i + 1);	ini.WriteDouble(strSection, strEntry, m_pAmtData->m_CurModelInfo.GrapPos[i].dx);
		strEntry.Format("GRAP_CCD%d_Y", i + 1);	ini.WriteDouble(strSection, strEntry, m_pAmtData->m_CurModelInfo.GrapPos[i].dy);
		strEntry.Format("GRAP_CCD%d_Z", i + 1);	ini.WriteDouble(strSection, strEntry, m_pAmtData->m_CurModelInfo.GrapPos[i].dz);

		strSection = _T("OFFSET");
		strEntry.Format("FPCX%d" , i +1 );		ini.WriteDouble(strSection, strEntry, m_pAmtData->m_CurModelInfo.TV_PCBX[i]);
		strEntry.Format("FPCY%d" , i +1 );		ini.WriteDouble(strSection, strEntry, m_pAmtData->m_CurModelInfo.TV_FPCY[i]);
	}
	strSection = _T("OFFSET");
	strEntry = _T("STAGEOFFSET");	ini.WriteDouble(strSection, strEntry, m_pAmtData->m_CurModelInfo.TV_StageReady);

	// Binarize Model일 경우 RECEIPE에 또 저장
	//if(m_bUseBinary)	SaveBinarizeModelFile();
	SaveBinarizeModelFile();//  [6/17/2009 SESL]
}

void CMTDoc::SaveBinarizeModelFile(void)
{
	// Binarize Model일 경우 RECEIPE에 또 저장
	CString strFilePath , strSection, strEntry;

	strFilePath.Format("%s\\" , GetApp()->m_strRecipePath);

	_mkdir(strFilePath);

	strFilePath += "System.INI";

	CIni ini(strFilePath);

	// 이전 내용 삭제
	strSection = _T("FPC PTR DATA");
	ini.DeleteSection(strSection);

	CString strText;
	int nCamIndex;
	strSection = _T("FPC PTR DATA");
	// 2번째 모델만 저장
	for(int n = 0; n < MAX_MOD_BUFFER_NUM; n++)
	{
		switch (n)
		{
		case CAM_0_FPC_0:
		case CAM_0_FPC_1:
		//case CAM_0_FPC_2:
		case CAM_1_FPC_0:
		case CAM_1_FPC_1:
		//case CAM_1_FPC_2:
		case CAM_2_FPC_0:
		case CAM_2_FPC_1:
		//case CAM_2_FPC_2:
		case CAM_3_FPC_0:
		case CAM_3_FPC_1:
		//case CAM_3_FPC_2:
			strText = _T("FPC");
			break;
		case CAM_0_PCB_0:
		case CAM_1_PCB_0:
		case CAM_2_PCB_0:
		case CAM_3_PCB_0:
		case CAM_0_PCB_1:
		case CAM_1_PCB_1:
		case CAM_2_PCB_1:
		case CAM_3_PCB_1:
			strText = _T("PCB");
			break;
		}
		nCamIndex = n/MAX_CAM_NUM;
		strEntry.Format("CAM_%d_%s_%d_left" , nCamIndex,  strText, n - (nCamIndex*MAX_CAM_NUM) );
		ini.WriteLong(strSection, strEntry, m_pAmtData->m_CurModelInfo.rtPattern[n].left);
		strEntry.Format("CAM_%d_%s_%d_top" , nCamIndex,  strText, n - (nCamIndex*MAX_CAM_NUM) );
		ini.WriteLong(strSection, strEntry, m_pAmtData->m_CurModelInfo.rtPattern[n].top);
		strEntry.Format("CAM_%d_%s_%d_right" , nCamIndex,  strText, n - (nCamIndex*MAX_CAM_NUM) );
		ini.WriteLong(strSection, strEntry, m_pAmtData->m_CurModelInfo.rtPattern[n].right);
		strEntry.Format("CAM_%d_%s_%d_bottom" , nCamIndex,  strText, n - (nCamIndex*MAX_CAM_NUM) );
		ini.WriteLong(strSection, strEntry, m_pAmtData->m_CurModelInfo.rtPattern[n].bottom);

		strEntry.Format("CAM_%d_%s_%d_CenterX" , nCamIndex,  strText, n - (nCamIndex*MAX_CAM_NUM) );
		ini.WriteLong(strSection, strEntry, m_pAmtData->m_CurModelInfo.ptCenter[n].x);
		strEntry.Format("CAM_%d_%s_%d_CenterY" , nCamIndex,  strText, n - (nCamIndex*MAX_CAM_NUM) );
		ini.WriteLong(strSection, strEntry, m_pAmtData->m_CurModelInfo.ptCenter[n].y);
	}
}

void CMTDoc::SaveCurrentModelName(void)
{
	CIni ini(CONFIG_PATH);
	ini.WriteString(_T("CONFIG"), _T("Current_Model"), m_pAmtData->m_CurModelInfo.strModelName);
	ini.WriteString(_T("CONFIG"), _T("m_CurDeviceID"), m_CurDeviceID);
}

void CMTDoc::SetMotorParameter(void)
{
	for( int i =0 ; i < MAX_AXIS ; i++ )
	{
		GetMotionParameter(i);
	}
	if( GetLineType() == REMOVE_TILTING_TYPE )
		GetMotionParameter(MAX_AXIS);

	m_pMotion->dMotionFullStrokeX = m_dMotionstrokeSingleX ;
	m_pMotion->dMotionFullStrokeY = m_dMotionstrokeSingleY ;
	m_pMotion->dMotionFullStrokeSingle = m_dMotionstrokeFullX ;

}

void CMTDoc::ApplyCamParamToVision(void)
{
	for( int i = 0 ; i < MAX_CAM_NUM ; i ++ )
	{
		GetAlignCtl()->GetVision()->m_pCamInfo[i].iCamNo = m_CamInfo[i].iCamNo ;
		GetAlignCtl()->GetVision()->m_pCamInfo[i].iCamX_Mode = m_CamInfo[i].iCamX_Mode ;
		GetAlignCtl()->GetVision()->m_pCamInfo[i].iCamY_Mode = m_CamInfo[i].iCamY_Mode ;
	}

	GetAlignCtl()->GetVision()->m_strDirAlignPath = GetApp()->m_strAlignPath ;
	GetAlignCtl()->GetVision()->m_strDirLogPath = GetApp()->m_strLogPath;
}

BOOL CMTDoc::SetErrorCode()
{
	CString m_strPath;
	m_strPath = ERROR_PATH;

	if (!m_fileErrorCode.Open(m_strPath, CFile::modeRead | CFile::shareDenyNone )) 
	{
		AfxMessageBox("File not found...");
		return FALSE;  
	}

	CString		strErrorCode;
	int			nIndex, nEnd;

	nIndex = nEnd = 0;
	
	while(m_fileErrorCode.ReadString(strErrorCode))
	{
		nIndex	= strErrorCode.Find("=",0);
		nEnd	= strErrorCode.Find(";",0);
		m_mapErrorCode.SetAt(strErrorCode.Left(nIndex), strErrorCode.Mid(nIndex+1, nEnd-nIndex-1));
	}

	return TRUE;
}

UINT ThreadFunc(LPVOID pParam)
{
	GetDoc()->m_pThreadLiveGrap->ThreadLiveGrapFunction();

	return 1;
}


void CMTDoc::RunThread(void)
{

	if( m_pEqMainSeq )
	{
		m_pEqMainSeq->SetMelsec(GetMelsec());
		m_pEqMainSeq->SetMMC39(m_pMotion);
		m_pEqMainSeq->SetAmtData(m_pAmtData);
		m_pEqMainSeq->m_pComm = (CFormCOMM *)GetMainFrame()->m_pForm[FORM_COMM];
		m_pEqMainSeq->ProcessInit();
		m_pEqMainSeq->Start( GetMainFrame()->m_hWnd );
		m_pEqMainSeq->SetSeqRun();
	}

	if( m_pThreadLiveGrap )
	{
		m_pThreadLiveGrap->m_bLiveGrapThreadRun = TRUE;
		m_pLiveThread = AfxBeginThread( (AFX_THREADPROC) ThreadFunc , NULL );
	}

}

void CMTDoc::Initial(void)
{
	//////////////////////////////////////////////////////////////////////////
	// 이 함수는 창들이 전부 만들어지고 난 후에 초기화할 것만 코딩한다.
	// 창들이 만들어지기 전에 해야할것들은 생성자에서 할것
	// GetDoc()로 호출하는 것들도 생성자에서 하지 못함

	GetMelsec()->m_pPLCInfo = &m_pAmtData->m_Melsec;
	if( IsSimulation() == FALSE )
	{
		if( GetMelsec()->Open() == FALSE)
		{
			//여기서..Can't Open Error Card 처리.. (Error Msg)
		}
	}

	if(m_pAmtData->m_Flag.bInitMotion == TRUE)
	{
		m_pMotion->InitParameter();
	}
	GetAlignCtl()->GetVision()->Zoom(0);
	GetApp()->ChangeStatus(STATUS_STOP);
	RunThread();

	// PLC Interface init
	GetDeviceCtl()->EnableAreaSensor(FALSE);

	if( GetLineType() == REMOVE_TILTING_TYPE )
	{
		GetMainFrame()->m_bMelsecWrite = TRUE;
		GetMelsec()->MelsecNetWordSend(TILT_OUTPUT_JOG_SPEED, 1);
		GetMelsec()->MelsecNetWordSend(TILT_OUTPUT_RUN_SPEED, (short)m_pMotorParam[AXIS_TILT_MOVE].dSpeed );
		GetMainFrame()->m_bMelsecWrite = FALSE;
	}
	GetDeviceCtl()->SetPCRun(TRUE);
	GetMainFrame()->StartMainFrmTimer();
	// 다 끝난 후에 화면을 보인다.
	GetMainFrame()->SwitchForm(FORM_AUTO);

	// User 표시
	::SendMessage(GetMainFrame()->m_hTop, WM_CHANGE_USER, (WPARAM)NULL, (LPARAM)NULL);
	::SendMessage(GetMainFrame()->m_hBottom, WM_CHANGE_USER, (WPARAM)NULL, (LPARAM)NULL);

	/*GetDoc()->m_fPCBMark_Fix_X[0] = 0.f;
	GetDoc()->m_fPCBMark_Fix_X[1] = 0.f;
	GetDoc()->m_fPCBMark_Fix_X[2] = 0.f;
	GetDoc()->m_fPCBMark_Fix_X[3] = 0.f;*/

	GetDoc()->m_bUsed_Tune = FALSE;

	SetLog("Program Start", LOG_SYSTEM);
}

void CMTDoc::GrapAliveFlagAllStop()
{
	for( int i = 0 ; i < MAX_CAM_NUM ; i ++ )
		m_bLiveGrapFlag[i] = FALSE ;
}


void CMTDoc::OnCloseDocument()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if( m_pEqMainSeq )
	{
		m_pEqMainSeq->SetSeqStop();
		m_pEqMainSeq->Kill();
		delete m_pEqMainSeq;
	}

	m_pThreadLiveGrap->m_bLiveGrapThreadRun = FALSE;
	if( m_pThreadLiveGrap )
	{
		// 2초동안 Thread 종료 대기
		int i = 0;
		while(i < 20)
		{
			if(m_pThreadLiveGrap->m_bLiveGrabThreadStart == FALSE)
				break;
			Sleep(100);
			i++;
		}
		ASSERT(i < 20);
		Sleep(200);
		delete m_pThreadLiveGrap;
		m_pThreadLiveGrap = NULL;
	}
	m_AryDevice.RemoveAll();

	CDocument::OnCloseDocument();
}

void CMTDoc::SaveSerialData()
{
	CString strSection, strEntry;


	CIni ini;	
	ini.SetIniFilename(CONFIG_PATH);

	strSection = _T("BLU Serial Data");
	ini.DeleteSection(strSection);
	strEntry = _T("USE");		ini.WriteBoolean(strSection, strEntry, m_SerialData[id_BLU].Used);
	strEntry = _T("PortName");	ini.WriteLong(strSection, strEntry, m_SerialData[id_BLU].IndexPortNo);
	strEntry = _T("Baudrate");	ini.WriteLong(strSection, strEntry, m_SerialData[id_BLU].IndexBaudRate);
	strEntry = _T("DataBit");	ini.WriteLong(strSection, strEntry, m_SerialData[id_BLU].IndexDataBit);
	strEntry = _T("StopBit");	ini.WriteLong(strSection, strEntry, m_SerialData[id_BLU].IndexStopBit);
	strEntry = _T("ParityBit");	ini.WriteLong(strSection, strEntry, m_SerialData[id_BLU].IndexParityBit);

	strSection = _T("BTM Light Serial Data");
	ini.DeleteSection(strSection);
	strEntry = _T("USE");		ini.WriteBoolean(strSection, strEntry, m_SerialData[id_BtmLight].Used);
	strEntry = _T("PortName");	ini.WriteLong(strSection, strEntry, m_SerialData[id_BtmLight].IndexPortNo);
	strEntry = _T("Baudrate");	ini.WriteLong(strSection, strEntry, m_SerialData[id_BtmLight].IndexBaudRate);
	strEntry = _T("DataBit");	ini.WriteLong(strSection, strEntry, m_SerialData[id_BtmLight].IndexDataBit);
	strEntry = _T("StopBit");	ini.WriteLong(strSection, strEntry, m_SerialData[id_BtmLight].IndexStopBit);
	strEntry = _T("ParityBit");	ini.WriteLong(strSection, strEntry, m_SerialData[id_BtmLight].IndexParityBit);

	strSection = _T("OMIT Serial Data");
	ini.DeleteSection(strSection);
	strEntry = _T("USE");		ini.WriteBoolean(strSection, strEntry, m_SerialData[id_OMIT].Used);
	strEntry = _T("PortName");	ini.WriteLong(strSection, strEntry, m_SerialData[id_OMIT].IndexPortNo);
	strEntry = _T("Baudrate");	ini.WriteLong(strSection, strEntry, m_SerialData[id_OMIT].IndexBaudRate);
	strEntry = _T("DataBit");	ini.WriteLong(strSection, strEntry, m_SerialData[id_OMIT].IndexDataBit);
	strEntry = _T("StopBit");	ini.WriteLong(strSection, strEntry, m_SerialData[id_OMIT].IndexStopBit);
	strEntry = _T("ParityBit");	ini.WriteLong(strSection, strEntry, m_SerialData[id_OMIT].IndexParityBit);
}

void CMTDoc::SetSerialArray()
{

	m_szPortNo[Index_Port_1] 				= _T("COM1");
	m_szPortNo[Index_Port_2] 				= _T("COM2");
	m_szPortNo[Index_Port_3] 				= _T("COM3");
	m_szPortNo[Index_Port_4] 				= _T("COM4");
	m_szBaudRate[Index_Baud_110] 			= _T("110");
	m_szBaudRate[Index_Baud_300] 			= _T("300");
	m_szBaudRate[Index_Baud_1200] 			= _T("1200");
	m_szBaudRate[Index_Baud_2400] 			= _T("2400");
	m_szBaudRate[Index_Baud_4800] 			= _T("4800");
	m_szBaudRate[Index_Baud_9600] 			= _T("9600");
	m_szBaudRate[Index_Baud_19200] 			= _T("19200");
	m_szBaudRate[Index_Baud_38400] 			= _T("38400");
	m_szBaudRate[Index_Baud_57600] 			= _T("57600");
	m_szBaudRate[Index_Baud_115200] 		= _T("115200");
	m_szDataBit[Index_DataBit_5] 			= _T("5");
	m_szDataBit[Index_DataBit_6] 			= _T("6");
	m_szDataBit[Index_DataBit_7] 			= _T("7");
	m_szDataBit[Index_DataBit_8] 			= _T("8");
	m_szStopBit[Index_StopBit_10] 			= _T("1");
	m_szStopBit[Index_StopBit_15] 			= _T("1.5");
	m_szStopBit[Index_StopBit_20] 			= _T("2");
	m_szParityBit[Index_ParityBit_Even]		= _T("Even");
	m_szParityBit[Index_ParityBit_Odd]		= _T("Odd");
	m_szParityBit[Index_ParityBit_None]		= _T("None");
	m_szParityBit[Index_ParityBit_Mark]		= _T("Mark");
	m_szParityBit[Index_ParityBit_Space]	= _T("Space");

	m_BaudRate[Index_Baud_110] 			= CBR_110;
	m_BaudRate[Index_Baud_300] 			= CBR_300;
	m_BaudRate[Index_Baud_1200] 		= CBR_1200;
	m_BaudRate[Index_Baud_2400] 		= CBR_2400;
	m_BaudRate[Index_Baud_4800] 		= CBR_4800;
	m_BaudRate[Index_Baud_9600] 		= CBR_9600;
	m_BaudRate[Index_Baud_19200] 		= CBR_19200;
	m_BaudRate[Index_Baud_38400] 		= CBR_38400;
	m_BaudRate[Index_Baud_57600] 		= CBR_57600;
	m_BaudRate[Index_Baud_115200] 		= CBR_115200;
	m_DataBit[Index_DataBit_5] 			= 5;
	m_DataBit[Index_DataBit_6] 			= 6;
	m_DataBit[Index_DataBit_7] 			= 7;
	m_DataBit[Index_DataBit_8] 			= 8;
	m_StopBit[Index_StopBit_10] 		= ONESTOPBIT;
	m_StopBit[Index_StopBit_15] 		= ONE5STOPBITS;
	m_StopBit[Index_StopBit_20] 		= TWOSTOPBITS;
	m_ParityBit[Index_ParityBit_Even]	= EVENPARITY;
	m_ParityBit[Index_ParityBit_Odd]	= ODDPARITY;
	m_ParityBit[Index_ParityBit_None]	= NOPARITY;
	m_ParityBit[Index_ParityBit_Mark]	= MARKPARITY;
	m_ParityBit[Index_ParityBit_Space]	= SPACEPARITY;
}

void CMTDoc::ReadBrightness()
{
	CString strDirFilePath, strSection, strEntry;

	strDirFilePath.Format("%s\\Brightness.INI" , GetApp()->m_strSystemPath );

	CIni ini;	
	ini.SetIniFilename(strDirFilePath);

	strSection = _T("BLU WT Brightness");
	for(int i = 0; i < 32; i++)
	{
		strEntry.Format("CH%02d", i);	m_nBluBrightness[BluModel_WT][i] = ini.GetLong(strSection, strEntry);
	}
	strSection = _T("BLU WS Brightness");
	for(int i = 0; i < 32; i++)
	{
		strEntry.Format("CH%02d", i);	m_nBluBrightness[BluModel_WS][i] = ini.GetLong(strSection, strEntry);
	}
	strSection = _T("BLU HD Brightness");
	for(int i = 0; i < 32; i++)
	{
		strEntry.Format("CH%02d", i);	m_nBluBrightness[BluModel_HD][i] = ini.GetLong(strSection, strEntry);
	}
	strSection = _T("BLU ETC Brightness");
	for(int i = 0; i < 32; i++)
	{
		strEntry.Format("CH%02d", i);	m_nBluBrightness[BluModel_ETC][i] = ini.GetLong(strSection, strEntry);
	}

	strSection = _T("BTM Light Brightness");
	for(int i = BtmLight_Ch1; i < BtmLight_ChMax; i++)
	{
		strEntry.Format("CH%02d", i);	m_nBtmLightBrightness[i] = ini.GetLong(strSection, strEntry);
	}

	strSection = _T("OMIT Brightness");
	strEntry.Format("CH%02d", 1);	m_nOmitBrightness = ini.GetLong(strSection, strEntry);
}

void CMTDoc::SaveBrightness()
{
	CString strDirFilePath, strSection, strEntry;

	strDirFilePath.Format("%s\\Brightness.INI" , GetApp()->m_strSystemPath );

	CIni ini;	
	ini.SetIniFilename(strDirFilePath);

	strSection = _T("BLU WT Brightness");
	ini.DeleteSection(strSection);
	for(int i = 0; i < 32; i++)
	{
		strEntry.Format("CH%02d", i);	ini.WriteLong(strSection, strEntry, m_nBluBrightness[BluModel_WT][i]);
	}
	strSection = _T("BLU WS Brightness");
	ini.DeleteSection(strSection);
	for(int i = 0; i < 32; i++)
	{
		strEntry.Format("CH%02d", i);	ini.WriteLong(strSection, strEntry, m_nBluBrightness[BluModel_WS][i]);
	}
	strSection = _T("BLU HD Brightness");
	ini.DeleteSection(strSection);
	for(int i = 0; i < 32; i++)
	{
		strEntry.Format("CH%02d", i);	ini.WriteLong(strSection, strEntry, m_nBluBrightness[BluModel_HD][i]);
	}
	strSection = _T("BLU ETC Brightness");
	ini.DeleteSection(strSection);
	for(int i = 0; i < 32; i++)
	{
		strEntry.Format("CH%02d", i);	ini.WriteLong(strSection, strEntry, m_nBluBrightness[BluModel_ETC][i]);
	}

	strSection = _T("BTM Light Brightness");
	ini.DeleteSection(strSection);
	for(int i = BtmLight_Ch1; i < BtmLight_ChMax; i++)
	{
		strEntry.Format("CH%02d", i);	ini.WriteLong(strSection, strEntry, m_nBtmLightBrightness[i]);
	}

	strSection = _T("OMIT Brightness");
	ini.DeleteSection(strSection);
	strEntry.Format("CH%02d", 1);	ini.WriteLong(strSection, strEntry, m_nOmitBrightness);
}

BOOL CMTDoc::SerialPortOpen( int nID )
{
	if(m_SerialData[nID].Used == FALSE)
		return FALSE;

	if(m_Comm[nID].m_bConnected)
		return TRUE;

	return m_Comm[nID].OpenPort(m_szPortNo[m_SerialData[nID].IndexPortNo], m_BaudRate[m_SerialData[nID].IndexBaudRate], 
		m_DataBit[m_SerialData[nID].IndexDataBit], m_StopBit[m_SerialData[nID].IndexStopBit], m_ParityBit[m_SerialData[nID].IndexParityBit]);
}

void CMTDoc::SerialPortClose( int nID )
{
	if(m_Comm[nID].m_bConnected == FALSE)
		return;

	m_Comm[nID].ClosePort();
}

void CMTDoc::BluOn()
{
	CString strMsg;
	strMsg.Format("setonex %x%c", 0xffffffff , 0x0D);
	nMakeAsciiData(id_BLU, strMsg);
	m_bLightOn[id_BLU] = TRUE;
}

void CMTDoc::BluOff()
{
	CString strMsg;
	strMsg.Format("setonex %x%c", 0x00000000 , 0x0D);
	nMakeAsciiData(id_BLU, strMsg);
	m_bLightOn[id_BLU] = FALSE;
}

void CMTDoc::OmitOn()
{
	if(m_bUseOmitIO)
	{
		DioOutput(GetPortMap()->oOmitOnOff, TRUE);
	}
	else
	{
		CString strMsg;
		strMsg.Format("setbrightness 0 %d%c", m_nOmitBrightness, 0x0D);
		nMakeAsciiData(id_OMIT, strMsg);
	}
	m_bLightOn[id_OMIT] = TRUE;
}

void CMTDoc::OmitOff()
{
	if(m_bUseOmitIO)
	{
		DioOutput(GetPortMap()->oOmitOnOff, FALSE);
	}
	else
	{
		CString strMsg;
		strMsg.Format("setbrightness 0 0%c", 0x0D);
		nMakeAsciiData(id_OMIT, strMsg);
	}
	m_bLightOn[id_OMIT] = FALSE;
}

void CMTDoc::BtmOn()
{
	CString strMsg;
	for(int i = BtmLight_Ch1; i < BtmLight_ChMax; i++)
	{
		strMsg.Format("L%d%03d%c%c", i+1, m_nBtmLightBrightness[i], 0x0D/*CR*/, 0x0A/*LF*/);
		nMakeAsciiData(id_BtmLight, strMsg);
		Sleep(10);
	}
	m_bLightOn[id_BtmLight] = TRUE;
}

void CMTDoc::BtmOff()
{
	CString strMsg;
	for (int i = BtmLight_Ch1; i < BtmLight_ChMax; i++)
	{
		strMsg.Format("E%d%c%c", i+1, 0x0D/*CR*/, 0x0A/*LF*/);
		nMakeAsciiData(id_BtmLight, strMsg);
		Sleep(10);
	}
	m_bLightOn[id_BtmLight] = TRUE;
}

int CMTDoc::nMakeAsciiData( int nID, CString strMsg )
{
	if(m_SerialData[nID].Used == FALSE)
		return -1;
	if(m_Comm[nID].m_bConnected == FALSE)
		return -1;

	int nSize = strMsg.GetLength();
	int bufPos = 0;
	BYTE *Send_buff;
	Send_buff = new BYTE[nSize];
	for(int i = 0; i < nSize; i++)//ASCII 데이터 이기 때문에 버터에 그대로 저장
	{
		Send_buff[bufPos++] = strMsg[i];
	}
	int etc = bufPos % 8;
	//포트에 데이터를 8개씩 쓰기 위해
	//데이터의 길이가 8의 배수가 아니면 나머지 데이터는 따로 보내줌
	for(int j =0; j < bufPos-etc; j+= 8)
	{
		m_Comm[nID].WriteComm(&Send_buff[j], 8) ;
	}
	if(etc != 0)//나머지 데이터 전송
	{
		m_Comm[nID].WriteComm(&Send_buff[bufPos -etc], etc) ;
	}

	delete [] Send_buff;
	return nSize;
}

void CMTDoc::SaveMotorParameter( CString strAxis, int nAxisNum, BOOL bInitParam /*= FALSE*/ )
{
	CString strSection, strKeyName;
	strSection.Format("%s - Axis_%2d","Motion Parameter",nAxisNum);
	CIni ini;
	ini.SetIniFilename(MOTION_SOCKET_PARAM_PATH);
	ini.WriteLong(		strSection,		"Motor",				m_pMotorParam[nAxisNum].iMotor				);
	ini.WriteLong(		strSection,		"Encoder",				m_pMotorParam[nAxisNum].iEncoder			);
	ini.WriteLong(		strSection,		"Voltage",				m_pMotorParam[nAxisNum].iVoltage			);
	ini.WriteLong(		strSection,		"FeedBack",				m_pMotorParam[nAxisNum].iFeedback			);
	ini.WriteLong(		strSection,		"EncoderType",			m_pMotorParam[nAxisNum].iEncoderType		);
	ini.WriteBoolean(	strSection, 	"StepMode",				m_pMotorParam[nAxisNum].bStepMode			);
	ini.WriteBoolean(	strSection, 	"CordinateDirection",	m_pMotorParam[nAxisNum].bCordinateDir		);
	ini.WriteBoolean(	strSection, 	"PosLimit",				m_pMotorParam[nAxisNum].bPosLimit			);
	ini.WriteBoolean(	strSection, 	"NegLimit",				m_pMotorParam[nAxisNum].bNegLimit			);
	ini.WriteBoolean(	strSection, 	"HomeSen",				m_pMotorParam[nAxisNum].bHomeSen			);
	ini.WriteBoolean(	strSection, 	"AmpFault",				m_pMotorParam[nAxisNum].bAmpFault			);
	ini.WriteBoolean(	strSection, 	"AmpReset",				m_pMotorParam[nAxisNum].bAmpReset			);
	ini.WriteBoolean(	strSection, 	"AmpEnable",			m_pMotorParam[nAxisNum].bAmpEnable			);
	ini.WriteLong(		strSection,		"InPosion",				m_pMotorParam[nAxisNum].iInPosion			);
	ini.WriteDouble(	strSection,		"PosSoftLimit",			m_pMotorParam[nAxisNum].dPosSoftLimit		);
	ini.WriteDouble(	strSection,		"NegSoftLimit",			m_pMotorParam[nAxisNum].dNegSoftLimit		);
	ini.WriteLong(		strSection,		"Acc",					m_pMotorParam[nAxisNum].iAcc				);
	ini.WriteLong(		strSection,		"HomeMethod",			m_pMotorParam[nAxisNum].iHomeMethod			);
	ini.WriteLong(		strSection,		"HomeSpeed",			m_pMotorParam[nAxisNum].iHomeSpeed			);
	ini.WriteBoolean(	strSection,		"HomeDir",				m_pMotorParam[nAxisNum].bHomeDir			);
	ini.WriteLong(		strSection,		"HomeTimeOut",			(LONG)m_pMotorParam[nAxisNum].dwHomeTimeOut	);
	ini.WriteDouble(	strSection,		"GearRatio",			m_pMotorParam[nAxisNum].dGearRatio			);
	ini.WriteDouble(	strSection,		"GearPitch",			m_pMotorParam[nAxisNum].dGearPitch			);
	ini.WriteDouble(	strSection,		"Speed",			    m_pMotorParam[nAxisNum].dSpeed				);
	ini.WriteLong(	strSection,		"absEncoderType",		    m_pMotorParam[nAxisNum].nAbsEncoderType		);

	if( GetLineType() == REMOVE_TILTING_TYPE && nAxisNum == AXIS_TILT_MOVE )
		return;	// 2009.06.17

	if(m_pAmtData->m_Flag.bInitMotion == TRUE && bInitParam)
		m_pMotion->InitParameter();
}

void CMTDoc::GetMotionParameter(int nAxisNum)
{
	CString strSection, strKeyName;
	strSection.Format("%s - Axis_%2d","Motion Parameter",nAxisNum);
	CIni ini;
	ini.SetIniFilename(MOTION_SOCKET_PARAM_PATH);
	m_pMotorParam[nAxisNum].iMotor				=	ini.GetLong(	strSection, "Motor",				0);
	m_pMotorParam[nAxisNum].iEncoder			=	ini.GetLong(	strSection, "Encoder",			    0);
	m_pMotorParam[nAxisNum].iVoltage			=	ini.GetLong(	strSection,	"Voltage",				0);
	m_pMotorParam[nAxisNum].iFeedback			=	ini.GetLong(	strSection,	"FeedBack",				0);
	m_pMotorParam[nAxisNum].iEncoderType		=	ini.GetLong(	strSection,	"EncoderType",			0);
	m_pMotorParam[nAxisNum].bStepMode			=	ini.GetBoolean(	strSection,	"StepMode",				FALSE);
	m_pMotorParam[nAxisNum].bCordinateDir		=	ini.GetBoolean(	strSection,	"CordinateDirection",	FALSE);
	m_pMotorParam[nAxisNum].bPosLimit			=	ini.GetBoolean(	strSection,	"PosLimit",				FALSE);
	m_pMotorParam[nAxisNum].bNegLimit			=	ini.GetBoolean(	strSection,	"NegLimit",				FALSE);
	m_pMotorParam[nAxisNum].bHomeSen			=	ini.GetBoolean(	strSection,	"HomeSen",				FALSE);
	m_pMotorParam[nAxisNum].bAmpFault			=	ini.GetBoolean( 	strSection,	"AmpFault",			FALSE);
	m_pMotorParam[nAxisNum].bAmpReset			=	ini.GetBoolean(	strSection,	"AmpReset",				FALSE);
	m_pMotorParam[nAxisNum].bAmpEnable			=	ini.GetBoolean(	strSection,	"AmpEnable",			FALSE);
	m_pMotorParam[nAxisNum].iInPosion			=	ini.GetBoolean(	strSection,	"InPosion",				FALSE);
	m_pMotorParam[nAxisNum].dPosSoftLimit		=	ini.GetDouble(	strSection,	"PosSoftLimit",			0.0);
	m_pMotorParam[nAxisNum].dNegSoftLimit		=	ini.GetDouble(	strSection,	"NegSoftLimit",			0.0);
	m_pMotorParam[nAxisNum].iAcc				=	ini.GetLong(	strSection,	"Acc",					0);
	m_pMotorParam[nAxisNum].iHomeMethod			=	ini.GetLong(	strSection,	"HomeMethod",			0);
	m_pMotorParam[nAxisNum].iHomeSpeed			=	ini.GetLong(	strSection,	"HomeSpeed",			0);
	m_pMotorParam[nAxisNum].bHomeDir			=	ini.GetBoolean(	strSection,	"HomeDir",				FALSE);
	m_pMotorParam[nAxisNum].dwHomeTimeOut       =	ini.GetLong(	strSection,	"HomeTimeOut",			0);
	m_pMotorParam[nAxisNum].dGearRatio			=	ini.GetDouble(	strSection,	"GearRatio",			0.0);
	m_pMotorParam[nAxisNum].dGearPitch			=	ini.GetDouble(	strSection,	"GearPitch",			0.0);
	m_pMotorParam[nAxisNum].dSpeed				=	ini.GetDouble(	strSection,	"Speed",				0.0);
	m_pMotorParam[nAxisNum].nAbsEncoderType		=	ini.GetLong(	strSection,	"absEncoderType",		AbsSigma3);
}																	

void CMTDoc::SaveTiltPos(void)
{
	CString strSection, strEntry;

	CIni ini;	
	ini.SetIniFilename(CONFIG_PATH);

	strSection = _T("Tilting Position");
	strEntry = _T("Tilting Up Position");		ini.WriteDouble(strSection, strEntry, m_dTiltingPos);
	strEntry = _T("Tilting Wait Position");		ini.WriteDouble(strSection, strEntry, m_dUnTiltingPos);
}

void CMTDoc::ReadTiltPos(void)
{
	CString strSection, strEntry;

	CIni ini;	
	ini.SetIniFilename(CONFIG_PATH);
	strSection = _T("Tilting Position");
	strEntry = _T("Tilting Up Position");		m_dTiltingPos = ini.GetDouble(strSection, strEntry, 0.0);
	strEntry = _T("Tilting Wait Position");		m_dUnTiltingPos = ini.GetDouble(strSection, strEntry, 0.0);
}

void CMTDoc::SaveTowerLamp(void)
{
	CString strSection, strKeyName;
	strSection.Format("Tower-Lamp Setting");
	CIni ini;
	ini.SetIniFilename(CONFIG_PATH);
	ini.WriteString(strSection, "Status Ready Mode Lamp",		strTowerLamp[STATUS_READY]			);
	ini.WriteString(strSection, "Status INIT Mode Lamp",		strTowerLamp[STATUS_INIT]			);
	ini.WriteString(strSection, "Status RUN Mode Lamp",			strTowerLamp[STATUS_RUN]			);
	ini.WriteString(strSection, "Status Stop Mode Lamp",		strTowerLamp[STATUS_STOP]			);
	ini.WriteString(strSection, "Status Error Mode Lamp",		strTowerLamp[STATUS_ERROR]			);
	ini.WriteString(strSection, "Status EMO Mode Lamp",			strTowerLamp[STATUS_EMO]			);
	ini.WriteString(strSection, "Status OPCall Mode Lamp",		strTowerLamp[STATUS_OPCALL]			);


}

void CMTDoc::GetTowerLamp(void)
{
	CString strSection, strKeyName;
	strSection.Format("Tower-Lamp Setting");
	CIni ini;
	ini.SetIniFilename(CONFIG_PATH);
	strTowerLamp[STATUS_READY]		=		ini.GetString(strSection,		"Status Ready Mode Lamp",	"R_2, Y_2, G_2"		);
	strTowerLamp[STATUS_INIT]		=		ini.GetString(strSection,		"Status INIT Mode Lamp",	"R_2, Y_2, G_2"		);
	strTowerLamp[STATUS_RUN]		=		ini.GetString(strSection,		"Status RUN Mode Lamp",		"R_2, Y_2, G_2"		);
	strTowerLamp[STATUS_STOP]		=		ini.GetString(strSection,		"Status Stop Mode Lamp",	"R_2, Y_2, G_2"		);
	strTowerLamp[STATUS_ERROR]		=		ini.GetString(strSection,		"Status Error Mode Lamp",	"R_2, Y_2, G_2"		);
	strTowerLamp[STATUS_EMO]		=		ini.GetString(strSection,		"Status EMO Mode Lamp",		"R_2, Y_2, G_2"		);
	strTowerLamp[STATUS_OPCALL]		=		ini.GetString(strSection,		"Status OPCall Mode Lamp",	"R_2, Y_2, G_2"		);

}

void CMTDoc::LoadConfigFile()
{
	CString strSection, strEntry;
	BOOL bSave = FALSE;

	CIni ini;	
	ini.SetIniFilename(CONFIG_PATH);

	//////////////////////////////////////////////////////////////////////////
	// 읽기 전용 (따로 저장하는 구분은 없고 직접 파일을 건드려서 수정해야 하는것)
	// 기존 저장된 값이 없을 경우 기본값으로 파일에 저장해 놓는다.
	// DCF 경로
	strSection = _T("PATH");
	strEntry = _T("DCF");		GetApp()->m_strDCFPath = ini.GetString(strSection, strEntry);
	if(GetApp()->m_strDCFPath.GetLength() < 5)
	{
		GetApp()->m_strDCFPath = DCF_PATH;		bSave = TRUE;
	}

	// FOV
	strSection = _T("FOV");
	strEntry = _T("FOV_X");		GetApp()->m_dFovX = ini.GetDouble(strSection, strEntry);
	if(GetApp()->m_dFovX < 0.0)
	{
		GetApp()->m_dFovX = RFOV_X;		bSave = TRUE;
	}
	strEntry = _T("FOV_Y");		GetApp()->m_dFovY = ini.GetDouble(strSection, strEntry);
	if(GetApp()->m_dFovY < 0.0)
	{
		GetApp()->m_dFovY = RFOV_Y;		bSave = TRUE;
	}

	// CAM Param
	for(int i = CAM_0; i < MAX_CAM_NUM; i++)
	{
		strSection.Format("CAM_%d Parameter", i);
		strEntry = _T("CAM_Channel");	m_CamInfo[i].iCamNo = ini.GetLong(strSection, strEntry, 99);

		strEntry = _T("X_Mode");	m_CamInfo[i].iCamX_Mode = ini.GetLong(strSection, strEntry, 99);

		strEntry = _T("Y_Mode");	m_CamInfo[i].iCamY_Mode = ini.GetLong(strSection, strEntry, 99);
	}
	if(m_CamInfo[CAM_0].iCamNo == 99)
	{
		m_CamInfo[CAM_0].iCamNo = 0;	bSave = TRUE;
	}
	if(m_CamInfo[CAM_0].iCamX_Mode == 99)
	{
		m_CamInfo[CAM_0].iCamX_Mode = 1;	bSave = TRUE;
	}
	if(m_CamInfo[CAM_0].iCamY_Mode == 99)
	{
		m_CamInfo[CAM_0].iCamY_Mode = 0;	bSave = TRUE;
	}

	if(m_CamInfo[CAM_1].iCamNo == 99)
	{
		m_CamInfo[CAM_1].iCamNo = 1;	bSave = TRUE;
	}
	if(m_CamInfo[CAM_1].iCamX_Mode == 99)
	{
		m_CamInfo[CAM_1].iCamX_Mode = 0;	bSave = TRUE;
	}
	if(m_CamInfo[CAM_1].iCamY_Mode == 99)
	{
		m_CamInfo[CAM_1].iCamY_Mode = 0;	bSave = TRUE;
	}

	if(m_CamInfo[CAM_2].iCamNo == 99)
	{
		m_CamInfo[CAM_2].iCamNo = 4;	bSave = TRUE;
	}
	if(m_CamInfo[CAM_2].iCamX_Mode == 99)
	{
		m_CamInfo[CAM_2].iCamX_Mode = 0;	bSave = TRUE;
	}
	if(m_CamInfo[CAM_2].iCamY_Mode == 99)
	{
		m_CamInfo[CAM_2].iCamY_Mode = 0;	bSave = TRUE;
	}

	if(m_CamInfo[CAM_3].iCamNo == 99)
	{
		m_CamInfo[CAM_3].iCamNo = 5;	bSave = TRUE;
	}
	if(m_CamInfo[CAM_3].iCamX_Mode == 99)
	{
		m_CamInfo[CAM_3].iCamX_Mode = 1;	bSave = TRUE;
	}
	if(m_CamInfo[CAM_3].iCamY_Mode == 99)
	{
		m_CamInfo[CAM_3].iCamY_Mode = 0;	bSave = TRUE;
	}
	// Image Save 여부
	strSection = _T("CONFIG");
	strEntry = _T("m_bContactImgSave");		m_bContactImgSave = ini.GetBoolean(strSection, strEntry, -1);
	if(m_bContactImgSave == -1)
	{
		m_bContactImgSave = FALSE;	bSave = TRUE;
	}
	strEntry = _T("m_bSuccessImgSave");		m_bSuccessImgSave = ini.GetBoolean(strSection, strEntry, -1);
	if(m_bSuccessImgSave == -1)
	{
		m_bSuccessImgSave = FALSE;	bSave = TRUE;
	}
	strEntry = _T("m_bErrImgSave");		m_bErrImgSave = ini.GetBoolean(strSection, strEntry, -1);
	if(m_bErrImgSave == -1)
	{
		m_bErrImgSave = TRUE;	bSave = TRUE;
	}
	// Binarize Condition value
	strSection = _T("BINARIZE");
	strEntry = _T("m_bUseBinary");		m_bUseBinary = ini.GetBoolean(strSection, strEntry, -1);
	if(m_bUseBinary == -1)
	{
		m_bUseBinary = FALSE;	bSave = TRUE;
	}
	for(int i = 0; i < MAX_CAM_NUM; i++)
	{
		strEntry.Format("m_nBinarizeFPC%d", i);		m_nBinarizeFPC[i] = ini.GetLong(strSection, strEntry, -1);
		if(m_nBinarizeFPC[i] == -1)
		{
			m_nBinarizeFPC[i] = BINARIZE_CONDITION;	bSave = TRUE;
		}
		strEntry.Format("m_nBinarizePCB%d", i);		m_nBinarizePCB[i] = ini.GetLong(strSection, strEntry, -1);
		if(m_nBinarizePCB[i] == -1)
		{
			m_nBinarizePCB[i] = BINARIZE_CONDITION;	bSave = TRUE;
		}

		strEntry.Format("m_nBinarizeFPC_Mark%d", i);		m_nBinarizeFPC_Mark[i] = ini.GetLong(strSection, strEntry, -1);
		if(m_nBinarizeFPC_Mark[i] == -1)
		{
			m_nBinarizeFPC_Mark[i] = BINARIZE_CONDITION;	bSave = TRUE;
		}
		strEntry.Format("m_nBinarizePCB_Mark%d", i);		m_nBinarizePCB_Mark[i] = ini.GetLong(strSection, strEntry, -1);
		if(m_nBinarizePCB_Mark[i] == -1)
		{
			m_nBinarizePCB_Mark[i] = BINARIZE_CONDITION;	bSave = TRUE;
		}

	}

	//  [7/6/2009 janghan]
	strEntry.Format("Use_AutoTune");		m_bAutoTune_Using = ini.GetLong(strSection, strEntry, -1);
	if(m_nFFCEdge_Min == -1)
	{
		m_bAutoTune_Using = TRUE;	bSave = TRUE;
	}

//  [6/28/2009 janghan] 이진화 Level Rage Load /////////////////////////////////////////////////////////////////
	strEntry.Format("m_nFFCEdge_Min");		m_nFFCEdge_Min = ini.GetLong(strSection, strEntry, -1);
	if(m_nFFCEdge_Min == -1)
	{
		m_nFFCEdge_Min = 150;	bSave = TRUE;
	}

	strEntry.Format("m_nFFCEdge_Max");		m_nFFCEdge_Max = ini.GetLong(strSection, strEntry, -1);
	if(m_nFFCEdge_Max == -1)
	{
		m_nFFCEdge_Max = 254;	bSave = TRUE;
	}

	strEntry.Format("m_nFFCMark_Min");		m_nFFCMark_Min = ini.GetLong(strSection, strEntry, -1);
	if(m_nFFCMark_Min == -1)
	{
		m_nFFCMark_Min = 30;	bSave = TRUE;
	}

	strEntry.Format("m_nFFCMark_Max");		m_nFFCMark_Max = ini.GetLong(strSection, strEntry, -1);
	if(m_nFFCMark_Max == -1)
	{
		m_nFFCMark_Max = 150;	bSave = TRUE;
	}

	strEntry.Format("m_nPCBEdge_Min");		m_nPCBEdge_Min = ini.GetLong(strSection, strEntry, -1);
	if(m_nPCBEdge_Min == -1)
	{
		m_nPCBEdge_Min = 150;	bSave = TRUE;
	}

	strEntry.Format("m_nPCBEdge_Max");		m_nPCBEdge_Max = ini.GetLong(strSection, strEntry, -1);
	if(m_nPCBEdge_Max == -1)
	{
		m_nPCBEdge_Max = 254;	bSave = TRUE;
	}

	strEntry.Format("m_nPCBMark_Min");		m_nPCBMark_Min = ini.GetLong(strSection, strEntry, -1);
	if(m_nPCBMark_Min == -1)
	{
		m_nPCBMark_Min = 25;	bSave = TRUE;
	}

	strEntry.Format("m_nPCBMark_Max");		m_nPCBMark_Max = ini.GetLong(strSection, strEntry, -1);
	if(m_nPCBMark_Max == -1)
	{
		m_nPCBMark_Max = 150;	bSave = TRUE;
	}

	strEntry.Format("m_nLevelStep");		m_nLevelStep = ini.GetLong(strSection, strEntry, -1);
	if(m_nLevelStep == -1)
	{
		m_nLevelStep = 5;	bSave = TRUE;
	}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// PLC Ethernet Parameter
	strSection = _T("PLC");
	strEntry = _T("m_bPlcEthernetType");		m_bPlcEthernetType = ini.GetBoolean(strSection, strEntry, -1);
	if(m_bPlcEthernetType == -1)
	{
		m_bPlcEthernetType = TRUE;	bSave = TRUE;
	}
	strEntry = _T("m_strHostIP");		m_strHostIP = ini.GetString(strSection, strEntry, "NONE");
	if(m_strHostIP == "NONE")
	{
		m_strHostIP = _T("192.168.10.100");	bSave = TRUE;
	}
	strEntry = _T("m_nCpuType");		m_nCpuType = ini.GetLong(strSection, strEntry, -1);
	if(m_nCpuType == -1)
	{
		m_nCpuType = CPU_Q06CPU;	bSave = TRUE;
	}
// 	strEntry = _T("m_nNetworkNo");		m_nNetworkNo = ini.GetLong(strSection, strEntry, -1);
// 	if(m_nNetworkNo == -1)
// 	{
// 		m_nNetworkNo = 1;	bSave = TRUE;
// 	}
// 	strEntry = _T("m_nHostStationNo");		m_nHostStationNo = ini.GetLong(strSection, strEntry, -1);
// 	if(m_nHostStationNo == -1)
// 	{
// 		m_nHostStationNo = 1;	bSave = TRUE;
// 	}
// 	strEntry = _T("m_nPcStationNo");		m_nPcStationNo = ini.GetLong(strSection, strEntry, -1);
// 	if(m_nPcStationNo == -1)
// 	{
// 		m_nPcStationNo = 6;	bSave = TRUE;
// 	}


	//////////////////////////////////////////////////////////////////////////
	// RS-232 설정
	strSection = _T("BLU Serial Data");
	strEntry = _T("USE");		m_SerialData[id_BLU].Used = ini.GetBoolean(strSection, strEntry, FALSE);
	strEntry = _T("PortName");	m_SerialData[id_BLU].IndexPortNo = ini.GetLong(strSection, strEntry, Index_Port_1);
	strEntry = _T("Baudrate");	m_SerialData[id_BLU].IndexBaudRate = ini.GetLong(strSection, strEntry, Index_Baud_110);
	strEntry = _T("DataBit");	m_SerialData[id_BLU].IndexDataBit = ini.GetLong(strSection, strEntry, Index_DataBit_5);
	strEntry = _T("StopBit");	m_SerialData[id_BLU].IndexStopBit = ini.GetLong(strSection, strEntry, Index_StopBit_10);
	strEntry = _T("ParityBit");	m_SerialData[id_BLU].IndexParityBit = ini.GetLong(strSection, strEntry, Index_ParityBit_None);

	strSection = _T("BTM Light Serial Data");
	strEntry = _T("USE");		m_SerialData[id_BtmLight].Used = ini.GetBoolean(strSection, strEntry, FALSE);
	strEntry = _T("PortName");	m_SerialData[id_BtmLight].IndexPortNo = ini.GetLong(strSection, strEntry, Index_Port_1);
	strEntry = _T("Baudrate");	m_SerialData[id_BtmLight].IndexBaudRate = ini.GetLong(strSection, strEntry, Index_Baud_110);
	strEntry = _T("DataBit");	m_SerialData[id_BtmLight].IndexDataBit = ini.GetLong(strSection, strEntry, Index_DataBit_5);
	strEntry = _T("StopBit");	m_SerialData[id_BtmLight].IndexStopBit = ini.GetLong(strSection, strEntry, Index_StopBit_10);
	strEntry = _T("ParityBit");	m_SerialData[id_BtmLight].IndexParityBit = ini.GetLong(strSection, strEntry, Index_ParityBit_None);

	strSection = _T("OMIT Serial Data");
	strEntry = _T("USE");		m_SerialData[id_OMIT].Used = ini.GetBoolean(strSection, strEntry, FALSE);
	strEntry = _T("PortName");	m_SerialData[id_OMIT].IndexPortNo = ini.GetLong(strSection, strEntry, Index_Port_1);
	strEntry = _T("Baudrate");	m_SerialData[id_OMIT].IndexBaudRate = ini.GetLong(strSection, strEntry, Index_Baud_110);
	strEntry = _T("DataBit");	m_SerialData[id_OMIT].IndexDataBit = ini.GetLong(strSection, strEntry, Index_DataBit_5);
	strEntry = _T("StopBit");	m_SerialData[id_OMIT].IndexStopBit = ini.GetLong(strSection, strEntry, Index_StopBit_10);
	strEntry = _T("ParityBit");	m_SerialData[id_OMIT].IndexParityBit = ini.GetLong(strSection, strEntry, Index_ParityBit_None);
	//////////////////////////////////////////////////////////////////////////
	// 기타 Model 따라가지 않는 것들
	strSection = _T("CONFIG");
	strEntry = _T("Current_Model");	m_pAmtData->m_CurModelInfo.strModelName = ini.GetString(strSection, strEntry, "42_2FFC_NORMAL");
	strEntry = _T("m_CurDeviceID");	m_CurDeviceID = ini.GetString(strSection, strEntry, "");
	strEntry = _T("Contact Cleanning Setting"); m_nCleanSet = ini.GetLong(strSection, strEntry, 0);
	strEntry = _T("m_bUseOmitIO"); m_bUseOmitIO = ini.GetBoolean(strSection, strEntry, -1);
	if(m_bUseOmitIO == -1)
	{
		m_bUseOmitIO = FALSE;	bSave = TRUE;
	}
	 
	for(int i=0 ; i < MAX_CAM_NUM ; i ++ )
	{
		strSection = _T("PCB CHANGE POSITION");
		strEntry.Format("PCB_CCD%d_X" , i + 1 );
		m_pAmtData->PCBChangePos.dx[i] = ini.GetDouble(strSection, strEntry, 0.0);
		strEntry.Format("PCB_CCD%d_Y" , i + 1 );
		m_pAmtData->PCBChangePos.dy[i] = ini.GetDouble(strSection, strEntry, 0.0);
	}

	// INSPECTION_COUNT//  [9/10/2009 janghan]/////////////////////////////////////////////////////////////////
	strSection = _T("CONFIG");
	strEntry = _T("m_nInspectionCount");		m_nInspectionCount = ini.GetLong(strSection, strEntry, -1);
	if(m_nInspectionCount == -1)
	{
		m_nInspectionCount = 0;	bSave = TRUE;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Contact On/Off Delay [12/20/2010 OSC]
	strSection = _T("CONFIG");
	strEntry = _T("m_dwContactDelay");		m_dwContactDelay = ini.GetLong(strSection, strEntry, 500);
	//////////////////////////////////////////////////////////////////////////

	LoadAutoModelChangeOption();

	if(bSave)	SaveConfigFile();
}

void CMTDoc::SaveConfigFile()
{
	CString strSection, strEntry;

	CIni ini;	
	ini.SetIniFilename(CONFIG_PATH);

	//////////////////////////////////////////////////////////////////////////
	// 읽기 전용 (따로 저장하는 구분은 없고 직접 파일을 건드려서 수정해야 하는것)
	strSection = _T("PATH");
	strEntry = _T("DCF");		ini.WriteString(strSection, strEntry, GetApp()->m_strDCFPath);

	strSection = _T("FOV");
	strEntry = _T("FOV_X");		ini.WriteDouble(strSection, strEntry, GetApp()->m_dFovX);
	strEntry = _T("FOV_Y");		ini.WriteDouble(strSection, strEntry, GetApp()->m_dFovY);

	for(int i = CAM_0; i < MAX_CAM_NUM; i++)
	{
		strSection.Format("CAM_%d Parameter", i);
		strEntry = _T("CAM_Channel");	ini.WriteLong(strSection, strEntry, m_CamInfo[i].iCamNo);

		strEntry = _T("X_Mode");	ini.WriteLong(strSection, strEntry, m_CamInfo[i].iCamX_Mode);

		strEntry = _T("Y_Mode");	ini.WriteLong(strSection, strEntry, m_CamInfo[i].iCamY_Mode);
	}
	// Image Save 여부
	strSection = _T("CONFIG");
	strEntry = _T("m_bContactImgSave");		ini.WriteBoolean(strSection, strEntry, m_bContactImgSave);
	strEntry = _T("m_bSuccessImgSave");		ini.WriteBoolean(strSection, strEntry, m_bSuccessImgSave);
	strEntry = _T("m_bErrImgSave");			ini.WriteBoolean(strSection, strEntry, m_bErrImgSave);
	strEntry = _T("Contact Cleanning Setting");		ini.WriteLong	(strSection, strEntry, m_nCleanSet);

	// PLC Ethernet Parameter
	strSection = _T("PLC");
	strEntry = _T("m_bPlcEthernetType");ini.WriteBoolean(strSection, strEntry, m_bPlcEthernetType);
	strEntry = _T("m_strHostIP");		ini.WriteString(strSection, strEntry, m_strHostIP);
	strEntry = _T("m_nCpuType");		ini.WriteLong(strSection, strEntry, m_nCpuType);
// 	strEntry = _T("m_nNetworkNo");		ini.WriteLong(strSection, strEntry, m_nNetworkNo);
// 	strEntry = _T("m_nHostStationNo");	ini.WriteLong(strSection, strEntry, m_nHostStationNo);
// 	strEntry = _T("m_nPcStationNo");	ini.WriteLong(strSection, strEntry, m_nPcStationNo);

	// Binarize Condition value
	SaveBinarizeCondition();
	SaveInspectionCount();//  [9/10/2009 janghan]
	//////////////////////////////////////////////////////////////////////////
	// RS-232 설정
	SaveSerialData();
	//////////////////////////////////////////////////////////////////////////
	// 기타 저장할 것들
	SaveCurrentModelName();
	strSection = _T("CONFIG");
	strEntry = _T("m_bUseOmitIO");		ini.WriteBoolean(strSection, strEntry, m_bUseOmitIO);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Contact On/Off Delay [12/20/2010 OSC]
	strSection = _T("CONFIG");
	strEntry = _T("m_dwContactDelay");		ini.WriteLong(strSection, strEntry, m_dwContactDelay);
	//////////////////////////////////////////////////////////////////////////

	SavePCBChangePos();
}

void CMTDoc::LoadLastGlassFile()
{
	CString strSection, strEntry;
	CIni ini;	
	ini.SetIniFilename(LASTGLASS_PATH);
	strSection = _T("Parameter");
	strEntry = _T("LastGlass.strHPanelID");		m_LastGlass.strHPanelID		= ini.GetString(strSection, strEntry);
	strEntry = _T("LastGlass.strEPanelID");		m_LastGlass.strEPanelID		= ini.GetString(strSection, strEntry);
	strEntry = _T("LastGlass.strBatchID");		m_LastGlass.strBatchID		= ini.GetString(strSection, strEntry);
	strEntry = _T("LastGlass.strDeviceID");		m_LastGlass.strDeviceID		= ini.GetString(strSection, strEntry);
	strEntry = _T("LastGlass.strInspection");	m_LastGlass.strInspection	= ini.GetString(strSection, strEntry);
	strEntry = _T("LastGlass.strPairH");		m_LastGlass.strPairH		= ini.GetString(strSection, strEntry);
	strEntry = _T("LastGlass.strPairE");		m_LastGlass.strPairE		= ini.GetString(strSection, strEntry);
	strEntry = _T("LastGlass.strOPID");			m_LastGlass.strOPID			= ini.GetString(strSection, strEntry);
	strEntry = _T("LastGlass.strOPName");		m_LastGlass.strOPName		= ini.GetString(strSection, strEntry);
}

void CMTDoc::SaveLastGlassFile()
{
	CString strSection, strEntry;
	CIni ini;	
	ini.SetIniFilename(LASTGLASS_PATH);
	strSection = _T("Parameter");
	//ini.DeleteSection(strSection);

	// 공백문자 제거
	m_LastGlass.strHPanelID.TrimRight(" ");
	m_LastGlass.strEPanelID.TrimRight(" ");
	m_LastGlass.strBatchID.TrimRight(" ");
	m_LastGlass.strDeviceID.TrimRight(" ");
	m_LastGlass.strInspection.TrimRight(" ");
	m_LastGlass.strPairH.TrimRight(" ");
	m_LastGlass.strPairE.TrimRight(" ");
	m_LastGlass.strOPID.TrimRight(" ");
	m_LastGlass.strOPName.TrimRight(" ");

	strEntry = _T("LastGlass.strHPanelID");		ini.WriteString(strSection, strEntry, m_LastGlass.strHPanelID	);
	strEntry = _T("LastGlass.strEPanelID");		ini.WriteString(strSection, strEntry, m_LastGlass.strEPanelID	);
	strEntry = _T("LastGlass.strBatchID");		ini.WriteString(strSection, strEntry, m_LastGlass.strBatchID	);
	strEntry = _T("LastGlass.strDeviceID");		ini.WriteString(strSection, strEntry, m_LastGlass.strDeviceID	);
	strEntry = _T("LastGlass.strInspection");	ini.WriteString(strSection, strEntry, m_LastGlass.strInspection	);
	strEntry = _T("LastGlass.strPairH");		ini.WriteString(strSection, strEntry, m_LastGlass.strPairH		);
	strEntry = _T("LastGlass.strPairE");		ini.WriteString(strSection, strEntry, m_LastGlass.strPairE		);
	strEntry = _T("LastGlass.strOPID");			ini.GetString(strSection, strEntry,	m_LastGlass.strOPID			);
	strEntry = _T("LastGlass.strOPName");		ini.WriteString(strSection, strEntry, m_LastGlass.strOPName		);
}

void CMTDoc::SetSuperUser( BOOL bSet )
{
	m_bSuperLogin = bSet;
	m_bEngLogin = bSet;
	m_nUserClass = bSet ? SUPER_USER:OPERATOR;
}

void CMTDoc::SaveBinarizeCondition()
{
	// Binarize Condition value
	CString strSection, strEntry;

	CIni ini;	
	ini.SetIniFilename(CONFIG_PATH);
	strSection = _T("BINARIZE");
	strEntry = _T("m_bUseBinary");		ini.WriteBoolean(strSection, strEntry, m_bUseBinary);
	for(int i = 0; i < MAX_CAM_NUM; i++)
	{
		strEntry.Format("m_nBinarizeFPC%d", i);		ini.WriteLong(strSection, strEntry, m_nBinarizeFPC[i]);
		strEntry.Format("m_nBinarizeFPC_Mark%d", i);		ini.WriteLong(strSection, strEntry, m_nBinarizeFPC_Mark[i]);
		strEntry.Format("m_nBinarizePCB%d", i);		ini.WriteLong(strSection, strEntry, m_nBinarizePCB[i]);
		strEntry.Format("m_nBinarizePCB_Mark%d", i);		ini.WriteLong(strSection, strEntry, m_nBinarizePCB_Mark[i]);
	}
}

void CMTDoc::SaveBinarizeLevelRange()//  [6/28/2009 janghan]
{
	//이진화 level Range 설정 저장
	CString strSection, strEntry;

	CIni ini;	
	ini.SetIniFilename(CONFIG_PATH);
	strSection = _T("BINARIZE");
	strEntry = _T("m_bUseBinary");		ini.WriteBoolean(strSection, strEntry, m_bUseBinary);
	
	strEntry.Format("m_nFFCEdge_Min");		ini.WriteLong(strSection, strEntry, m_nFFCEdge_Min);
	strEntry.Format("m_nFFCEdge_Max");		ini.WriteLong(strSection, strEntry, m_nFFCEdge_Max);
	strEntry.Format("m_nFFCMark_Min");		ini.WriteLong(strSection, strEntry, m_nFFCMark_Min);
	strEntry.Format("m_nFFCMark_Max");		ini.WriteLong(strSection, strEntry, m_nFFCMark_Max);
	strEntry.Format("m_nPCBEdge_Min");		ini.WriteLong(strSection, strEntry, m_nPCBEdge_Min);
	strEntry.Format("m_nPCBEdge_Max");		ini.WriteLong(strSection, strEntry, m_nPCBEdge_Max);
	strEntry.Format("m_nPCBMark_Min");		ini.WriteLong(strSection, strEntry, m_nPCBMark_Min);
	strEntry.Format("m_nPCBMark_Max");		ini.WriteLong(strSection, strEntry, m_nPCBMark_Max);
	strEntry.Format("m_nLevelStep");		ini.WriteLong(strSection, strEntry, m_nLevelStep);

	strEntry.Format("Use_AutoTune");		ini.WriteLong(strSection, strEntry, m_bAutoTune_Using);//  [7/6/2009 janghan]
}

void CMTDoc::SaveInspectionCount()//  [9/10/2009 janghan]
{
	//Zone 별 검사 횟수 저장
	CString strSection, strEntry;

	CIni ini;	
	ini.SetIniFilename(CONFIG_PATH);
	strSection = _T("CONFIG");
	strEntry = _T("m_nInspectionCount");		ini.WriteLong(strSection, strEntry, m_nInspectionCount);
}

//2009.05.31 Non-stop
//Kang Hee Won
void CMTDoc::SaveAutoModelChangeOption()
{
	CString strSection, strEntry;
	CIni ini;	
	ini.SetIniFilename(CONFIG_PATH);
	strSection = _T("AutoM/C Option");

	strEntry = _T("AutoM/C Use");			ini.WriteLong(strSection, strEntry, m_bUseAMC			);
	strEntry = _T("AutoCreate Use");		ini.WriteLong(strSection, strEntry, m_bUseAutoCreate	);

}

void CMTDoc::LoadAutoModelChangeOption()
{
	CString strSection, strEntry;
	CIni ini;	
	ini.SetIniFilename(CONFIG_PATH);
	strSection = _T("AutoM/C Option");

	strEntry = _T("AutoM/C Use");			m_bUseAMC			= ini.GetLong(strSection, strEntry	);
	strEntry = _T("AutoCreate Use");		m_bUseAutoCreate	= ini.GetLong(strSection, strEntry	);

}

void CMTDoc::FileOpenModelAtDevice()
{
	char cBuf[100];
	char Databuf[300] , TitleBuf[300];
	CString strText1;
	int i , nSize ;

	m_AryDevice.RemoveAll();

	strText1.Format ("%s\\DeviceInfo.INI", SYSTEM_PATH);
	strcpy_s(cBuf, strText1 );


	sprintf_s(Databuf, "NUM"); GetPrivateProfileString("DEVICE INFO COUNT", Databuf, "0", Databuf, sizeof(Databuf), cBuf);	
	nSize= atoi(Databuf);

	for( i = 0 ; i < nSize ; i++ )
	{
		strText1.Format("DEVICE INFO %d" , i );
		sprintf_s( TitleBuf , strText1 );

		STRUCT_DEVICE TmpDevice;

		sprintf_s(Databuf, "DeviceName"); GetPrivateProfileString(TitleBuf, Databuf, "0", Databuf, sizeof(Databuf), cBuf);	
		TmpDevice.strDeviceName = Databuf ;

		sprintf_s(Databuf, "ModelName"); GetPrivateProfileString(TitleBuf, Databuf, "0", Databuf, sizeof(Databuf), cBuf);	
		TmpDevice.strModelName = Databuf ;

		TmpDevice.iPanelSize = atoi( TmpDevice.strModelName.Mid(0,2) );
		m_AryDevice.Add(TmpDevice);
	}

	for ( i = 0 ; i < nSize-1 ; i++ )
	{
		for ( int j = i + 1 ; j < nSize ; j++ ){	
			STRUCT_DEVICE temp, Ary1 = m_AryDevice.GetAt(i), Ary2 = m_AryDevice.GetAt(j);
			if( Ary1.strDeviceName.Mid(3, 2) > Ary2.strDeviceName.Mid(3,2)){	
				temp = Ary1;
				Ary1 = Ary2;
				Ary2 = temp;
				m_AryDevice.SetAt(i, Ary1);
				m_AryDevice.SetAt(j, Ary2);
			}
		}
	}
	for ( i = 0 ; i < nSize-1 ; i++ )
	{
		for ( int j = i + 1 ; j < nSize ; j++ ){	
			STRUCT_DEVICE temp, Ary1 = m_AryDevice.GetAt(i), Ary2 = m_AryDevice.GetAt(j);
			if(Ary1.strDeviceName.Mid(3, 2) == Ary2.strDeviceName.Mid(3,2)){	
				if ( Ary1.strDeviceName > Ary2.strDeviceName)
				{
					temp = Ary1;
					Ary1 = Ary2;
					Ary2 = temp;
					m_AryDevice.SetAt(i, Ary1);
					m_AryDevice.SetAt(j, Ary2);
				}
			}
		}
	}
}

void CMTDoc::FileSaveModelAtDevice()
{
	char cBuf[100];
	CString strPath , strTmp , str ;
	int i , nSize ;

	strPath.Format ("%s\\DeviceInfo.INI", SYSTEM_PATH);
	strcpy_s(cBuf, strPath );

	CFile cfgFile( strPath , CFile::modeCreate | CFile::modeWrite );
	CArchive ar ( &cfgFile , CArchive::store );

	strTmp.Format("[DEVICE INFO COUNT]\r\n");
	ar.WriteString(strTmp);

	nSize = m_AryDevice.GetSize();
	str.Format("NUM=%d\r\n" , nSize );
	ar.WriteString(str);

	for( i =0 ; i < nSize ; i ++ )
	{
		STRUCT_DEVICE TmpDevice = m_AryDevice.GetAt( nSize - ( i + 1) );

		strTmp.Format("[DEVICE INFO %d]\r\n" , i );
		ar.WriteString(strTmp);

		str.Format("DeviceName=%s\r\n" , TmpDevice.strDeviceName );
		ar.WriteString(str);


		str.Format("ModelName=%s\r\n" , TmpDevice.strModelName );
		ar.WriteString(str);

	}

	str.Format("[END]\r\n");
	ar.WriteString(str);
}

void CMTDoc::SavePCBChangePos()
{
	CString strSection, strEntry;
	CIni ini;	
	ini.SetIniFilename(CONFIG_PATH);

	for(int i=0 ; i < MAX_CAM_NUM ; i ++ )
	{
		strSection = _T("PCB CHANGE POSITION");
		strEntry.Format("PCB_CCD%d_X" , i + 1 );
		ini.WriteDouble(strSection, strEntry, m_pAmtData->PCBChangePos.dx[i]);
		strEntry.Format("PCB_CCD%d_Y" , i + 1 );
		ini.WriteDouble(strSection, strEntry, m_pAmtData->PCBChangePos.dy[i]);
	}
}

void CMTDoc::AddModelAtDevice(CString strDeviceID, CString strModelName)
{
	STRUCT_DEVICE TmpDevice;
	TmpDevice.strDeviceName = strDeviceID ;
	TmpDevice.strModelName = strModelName ;

	TmpDevice.iPanelSize = atoi( TmpDevice.strModelName.Mid(0,2) );
	m_AryDevice.Add(TmpDevice);

	FileSaveModelAtDevice();
}

BOOL CMTDoc::CheckLimit( int nAxis, double dPos )
{
	if(GetDoc()->m_pMotorParam[nAxis].dPosSoftLimit < dPos)
	{
		AfxMessageBox("This value is over the S/W Positive limit");
		return FALSE;
	}
	if(GetDoc()->m_pMotorParam[nAxis].dNegSoftLimit > dPos)
	{
		AfxMessageBox("This value is lower the S/W Negative limit");
		return FALSE;
	}
	return TRUE;
}

BOOL CMTDoc::SetAlignPos( int nCamNum)
{
	POSITION_DATA ReadyPos;
	POSITION_DATA GrabPos;

	STRUCT_MODEL_DATA* pModelData = &(GetDoc()->m_pAmtData->m_CurModelInfo);

	// 일단 현재 position을 Grab position에 저장한다.
	switch (nCamNum)
	{
	case 0:
		GrabPos.dx = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X0);
		GrabPos.dy = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_Y0);
		break;

	case 1:
		GrabPos.dx = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X1);
		GrabPos.dy = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_Y1);
		break;

	case 2:
		GrabPos.dx = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X2);
		GrabPos.dy = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_Y2);
		break;

	case 3:
		GrabPos.dx = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X3);
		GrabPos.dy = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_Y3);
		break;
	}
	

	// 사용하는 Cam, 안하는 Cam 선정
	BOOL bUse[MAX_CAM_NUM] = {TRUE, TRUE, TRUE, TRUE};
	if(pModelData->iContactNum == CAmtData::TWO_CONT)
	{
		bUse[CAM_0] = FALSE;	bUse[CAM_3] = FALSE;
	}
	else if(pModelData->iContactNum == CAmtData::ONE_CONT)
	{
		for(int i = CAmtData::CONTACT_0; i <= CAmtData::CONTACT_3; i++)
		{
			if(i != pModelData->iContactor)	bUse[i-1] = FALSE;
		}
	}

	// Grab position을 Ready position에 저장
	ReadyPos.dx = GrabPos.dx;
	ReadyPos.dy = GrabPos.dy - pModelData->TV_StageReady;
	
	// S/W Limit check
	if(CheckLimit(nCamNum, ReadyPos.dx) == FALSE)	return 0;
	if(CheckLimit(nCamNum+4, ReadyPos.dy) == FALSE) return 0;
	if(CheckLimit(nCamNum, GrabPos.dx) == FALSE)	return 0;
	if(CheckLimit(nCamNum+4, GrabPos.dy) == FALSE)	return 0;
	
	// 최종 적용
	pModelData->ReadyPos[nCamNum].dx = ReadyPos.dx;
	pModelData->ReadyPos[nCamNum].dy = ReadyPos.dy;
	pModelData->GrapPos[nCamNum].dx = GrabPos.dx;
	pModelData->GrapPos[nCamNum].dy = GrabPos.dy;

	GetDoc()->m_pAmtData->m_CurModelInfo.ReadyPos[nCamNum].dx = ReadyPos.dx;
	GetDoc()->m_pAmtData->m_CurModelInfo.ReadyPos[nCamNum].dy = ReadyPos.dy;
	GetDoc()->m_pAmtData->m_CurModelInfo.GrapPos[nCamNum].dx = GrabPos.dx;
	GetDoc()->m_pAmtData->m_CurModelInfo.GrapPos[nCamNum].dy = GrabPos.dy;

	return 1;
}

void CMTDoc::SaveContiactDelay()
{
	CString strSection, strEntry;

	CIni ini;	
	ini.SetIniFilename(CONFIG_PATH);

	// Contact On/Off Delay [12/20/2010 OSC]
	strSection = _T("CONFIG");
	strEntry = _T("m_dwContactDelay");		ini.WriteLong(strSection, strEntry, m_dwContactDelay);
}