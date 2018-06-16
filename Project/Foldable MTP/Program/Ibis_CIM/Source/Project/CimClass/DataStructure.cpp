#include "StdAfx.h"
#include "DataStructure.h"
#include "../GaonNuri.h"
#include <strsafe.h>


CDataStructure	g_Data;
CDataStructure* g_pData = &g_Data;

// CDataStructure*			g_pData	= (CDataStructure*)malloc(sizeof(CDataStructure));

RCP_LIST		tag_Recipe_Validation::m_lstRcp;


int tag_Recipe_Validation::LoadParamList(tag_Recipe_Data* pPParam, char* pFileName, char* pSimbolChar)
{
	int m=0;
	char cLine[1024];

	FILE	*fp;
	errno_t		err = 0;
	char* p0 = &cLine[0];
	char* pS, *pE;

	char cParamName[tag_Recipe_Data::MAX_DATA_LEN];
	char cParamV[tag_Recipe_Data::MAX_DATA_LEN];
	CString strSection;
	CString strKey;
	CString strValue;
	CString strParaName;
	CString strParaValue;

	if(err = fopen_s(&fp, pFileName, "r"))
		return -1;

	int nCount=ERR_SYNTEXT;

	while(fgets(p0, 1024,fp)!=NULL)
	{
		if ( IsSkipLine( p0 ) ) continue;			// 제외시키라는 지시어가 있다면 해당 Line은 제외한다.

		int saaa=0; saaa = strlen(p0);
		if(strchr(p0, '#')!=NULL || strchr(p0, ';')!=NULL || IsBlankLine(p0) ) // || strchr(p0, '_')!=NULL)		// 지정한 문자가 첫글자로오면 무시한다.
			continue;


		pS = p0;

		if((pE=strchr(pS, '='))==NULL)
		{
			if((pE=strchr(pS, '\n'))!=NULL)			*(pE++) = 0x00;
			else if((pE=strchr(pS, 0x0a))!=NULL)	*(pE++) = 0x00;
			else if((pE=strchr(pS, 0x0d))!=NULL)	*(pE++) = 0x00;
			else if((pE=strchr(pS, 0x00))!=NULL)	*(pE++) = 0x00;
			else
			{
				nCount = ERR_SYNTEXT;
				break;
			}
			strValue = pS;
			strSection = pS;
			continue;
		}

		*(pE++) = 0x00;

		strKey = pS;

		if(strSection.GetLength() < 3)
			continue;

		if(strSection.Find("[HEADER]")>=0)
			continue;

		pS = pE;
		if((pE=strchr(pS, '\n'))!=NULL)			*(pE++) = 0x00;
		else if((pE=strchr(pS, 0x0a))!=NULL)	*(pE++) = 0x00;
		else if((pE=strchr(pS, 0x0d))!=NULL)	*(pE++) = 0x00;
		else if((pE=strchr(pS, 0x00))!=NULL)	*(pE++) = 0x00;
		else
		{
			nCount = ERR_SYNTEXT;
			break;
		}
		strValue = pS;
		strSection.Replace("[",""); strSection.Replace("]","");
		strParaName.Format("%s", strSection);
		strParaName.MakeUpper();	// 전부 대문자로 올려야함 띄어쓰기도 전부 _로 대체
		
		cParamName[0] = 0x00;
		cParamV[0] = 0x00;
		if(strParaName.GetLength()<tag_Recipe_Data::MAX_DATA_LEN)		strcpy_s(cParamName, strParaName);//strcpy_s(cParamName, strParaName.GetBuffer());
		else
		{
			memcpy(cParamName, strParaName.GetBuffer(), tag_Recipe_Data::MAX_DATA_LEN);
			cParamName[tag_Recipe_Data::MAX_DATA_LEN-1] = 0x00;
		}

		if(strValue.GetLength()<tag_Recipe_Data::MAX_DATA_LEN)		strcpy_s(cParamV, strValue.GetBuffer());
		else
		{
			memcpy(cParamV, strValue.GetBuffer(), tag_Recipe_Data::MAX_DATA_LEN);
			cParamV[tag_Recipe_Data::MAX_DATA_LEN-1] = 0x00;
		}

		//TRACE("LoadParamList - Source(%s)(%s)\n", strParaName, strValue);
		TRACE("LoadParamList - AddParam(%s)(%s)\n", cParamName, cParamV);

		if(!pPParam->AddData(cParamName, cParamV))
		{
			nCount = ERR_STACK_OVERFLOW;
			break;
		}
		nCount++;

	}

	fclose(fp);
	return nCount;
};

BOOL tag_Recipe_Validation::IsBlankLine(char* pLine)
{
	int nLine=0;
	BOOL bRtn=TRUE;

	nLine = strlen(pLine);
	nLine--;

	for( ;nLine>-1; nLine--)
	{
		if(pLine[nLine] != 0x20 && pLine[nLine] != 0x0d && pLine[nLine] != 0x0a)
		{
			bRtn = FALSE;
			break;
		}
	}
	return bRtn;
}

// 해당 Line을 무시할것인가 ?
BOOL tag_Recipe_Validation::IsSkipLine(char* pLine)
{
	int nLine = strlen(pLine);

	if ( nLine <= 0 ) return TRUE;				// 글자가 없다면 무시
	if ( pLine[0] == '_' ) return TRUE;			// 언더바로 시작하면 무시한다.
	if ( pLine[0] == ';' ) return TRUE;			// 
	if ( pLine[0] == '#' ) return TRUE;			// 
	if ( pLine[0] == '!' ) return TRUE;			// 
	if ( pLine[0] == '?' ) return TRUE;			// 

	return FALSE;
}

BOOL tag_Recipe_Validation::IsRcpName(char* pFileName)
{
	CFileFind finder;
	CString strName;

	if(pFileName==NULL)
		return FALSE;

	if(strlen(pFileName)>=256)
		return FALSE;

	strName.Format("%s", pFileName);

	if(!finder.FindFile(strName))
		return FALSE;

	return TRUE;
};

void tag_Recipe_Validation::GetRcpParam(char* pPPID, tag_Recipe_Data* pPParam, char* cMODE = PPID_MODE_CREATE)
{
	if(pPPID==NULL || pPParam==NULL)
		return ;

	char cFileName[256];
	CString strPPID;
	CString	strMessage;
	pPParam->Clear();
	strPPID.Format("%s", pPPID);

	int nCount = 0;

	if(strlen(CSettingParm::cMODEL_PATH)>0 && strlen(CSettingParm::cMODEL_PATH)<81)
	{
// 		if (strcmp(cMODE, PPID_MODE_DELETE)==0)
// 			sprintf_s(cFileName, "%s\\%s\\%s.ini.del", CSettingParm::cMODEL_PATH, pPPID, pPPID);
// 		else
// 			sprintf_s(cFileName, "%s\\%s\\%s.ini", CSettingParm::cMODEL_PATH, pPPID, pPPID);

//		sprintf_s(cFileName, "%s\\TeachDataPC.ini", CSettingParm::cEQPCONFIG_PATH);
		sprintf_s(cFileName, "%s\\%s\\RMSData.ini", CSettingParm::cMODEL_PATH, pPPID);

// 		if(!tag_Recipe_Validation::IsRcpName(cFileName))
// 		{
// 			GetLogCtrl()->AddBuf(LOG_CIM, "GetRcpParam Fail - Recipe File Not exist, Path=%s", cFileName);
// 			strMessage.Format("GetRcpParam Fail - Recipe File Not exist, Path=%s", cFileName);
// 			//AfxMessageBox(strMessage);
// 			return ;
// 		}
		nCount=tag_Recipe_Validation::LoadParamList(pPParam, cFileName);
		if(nCount<0) 
		{
			GetLogCtrl()->AddBuf(LOG_CIM, "GetRcpParam Fail - Recipe File Error, Path=%s", cFileName);
			strMessage.Format("GetRcpParam Fail - Recipe File Error, Path=%s", cFileName);
			
			return ;
		}

	
	}
}
void tag_Recipe_Validation::SaveRcp(CString strPath, CString strBackupPath, CString* ParamName,double* dParaVal, int nCount)
{
	CString strFolderPath = CFileSupport::GetPathName(strPath);
	if(CFileSupport::DirectoryCheck(strFolderPath) == FALSE)
	{
		CFileSupport::CreateDirectory(strFolderPath);		
	}
// 	strFolderPath = CFileSupport::GetPathName(strBackupPath);
// 
// 	if(CFileSupport::DirectoryCheck(strFolderPath) == FALSE)
// 	{
// 		CFileSupport::CreateDirectory(strFolderPath);			
// 	}

	if(CFileSupport::FileCheck(strPath))
	{
		// 기존 파일 백업
		::CopyFile(strPath, strBackupPath, FALSE);
		// File 내용을 깔끔하게 하기 위해 기존껀 지우고 새로 적는다
		::DeleteFile(strPath);
	}

	CIni ini(strPath);

	for(int i = 0; i < nCount; i++)
		ini.WriteDouble(ParamName[i]	, 	_T("Data"), dParaVal[i]		);
}
int	tag_Recipe_Validation::LoadRcpList()
{
	m_lstRcp.RemoveAll();

	CString strModelPath, strModelName;
	CFileFind finder;
	CString strRcpName;

	BOOL bFindFolder = TRUE;

	strModelPath.Format("%s\\*.*", CSettingParm::cMODEL_PATH);
	BOOL fExist = finder.FindFile(strModelPath);
	while (fExist)
	{
		fExist = finder.FindNextFile();
		strModelName = finder.GetFileName();

		if(bFindFolder)
		{
			if(strModelName.Right(1) != "." && finder.IsDirectory() ) 
			{
				strModelName.MakeUpper();

				strRcpName.Format("%s\\%s\\RMSData.ini", CSettingParm::cMODEL_PATH, strModelName);


				if(tag_Recipe_Validation::IsRcpName((LPSTR)(LPCTSTR)strRcpName))
					m_lstRcp.AddTail(strModelName);
			}
		}
		else
		{
			if(finder.IsDirectory() ) 
			{
				strModelName.MakeUpper();
				m_lstRcp.AddTail(strModelName);
			}			
		}
	}

	return m_lstRcp.GetCount();
}


CString g_strMyMessageType[] = 
{
	"TYPE_SET_NONE",
	//TO DATA PC
	"EQUIP_SET_CONTROL",
	"EQUIP_SET_DATAPC_VERSION",	//SONIC - 2014.02.14. 누락
	"EQUIP_SET_ALARM",
	"EQUIP_SET_ERMS",
	"EQUIP_SET_CELL_PROCESS_START_LOAD",
	"EQUIP_SET_CELL_PROCESS_COMPLETE_UNLOAD",
	"EQUIP_SET_CELL_ENDINS",
	"EQUIP_SET_EQUIPMENT_STATUS_CHANGE",
	"EQUIP_SET_PPID_CREATE",
	"EQUIP_SET_PPID_DELETE",
	"EQUIP_SET_PPID_MODIFY",
	"EQUIP_SET_PPID_CHANGE",
	"EQUIP_SET_PARAMETER_CHANGE",
	"EQUIP_SET_PPID_OPERATION",
	"EQUIP_SET_MATERIAL_CHANGE",
	"EQUIP_SET_OPCALL_EVENT",
	"EQUIP_SET_INTERLOCK_EVENT",
	"EQUIP_SET_CELL_INFORMATION_REQUEST",
	"EQUIP_SET_CODE_INFORMATION_REQUEST",
	"EQUIP_SET_CONTROL_STATE_CHANGE_ONESELF_ACK",
	"EQUIP_SET_CELL_LOADER_IN",
	"EQUIP_SET_CELL_TESTER_OUT",
	"EQUIP_SET_CELL_UNLOADER_OUT",

	"EQUIP_SET_CTL_USER_LOGIN",
	"EQUIP_SET_INS1_USER_LOGIN",
	"EQUIP_SET_INS2_USER_LOGIN",
	"EQUIP_SET_CTL_USER_LOGOUT",
	"EQUIP_SET_INS1_USER_LOGOUT",
	"EQUIP_SET_INS2_USER_LOGOUT",

	//UV MCR READING
	"EQUIP_SET_UV_READING_MCR",	//SONIC - 2013.07.22.
	//VISION ALIGN
	"EQUIP_SET_ALIGN_MARK_FIND",	//SONIC - 2013.07.22.
	"EQUIP_SET_LINE_NO",
	"EQUIP_SET_CELL_EXIST",
	//Loss Code Report
	"EQUIP_SET_LOSS_CODE",		//20141223 sms

	//TO AFT
	"HOST_SET_CONTROL",
	"HOST_SET_MODULE",
	"HOST_SET_PROC",
	"HOST_SET_DATAPC_VERSION",
	"HOST_SET_CELL_INFORMATION",

	"HOST_SET_OPCALL",
	"HOST_SET_INTERLOCK",
	"HOST_SET_TERMINAL",
	"HOST_SET_CONTROL_STATE_CHANGE",
	"HOST_SET_LINE_NO",

	"EQUIP_SET_USER_LOGIN_REQUEST",   // 58, 작업자의 Login 검증을 상위 서버에 요청한다.
	"EQUIP_SET_USER_LOGOUT_REQUEST",	// 59, 작업자의 Login 검증을 상위 서버에 요청한다.
	"HOST_SET_USER_LOGOUT_REQUEST",   // 60, 서버에서 설비를 세우고 작업자를 강제로 Logout 시키라는 지령 메세지

	"EQUIP_SET_CELL_LOT_INFORMATION_REQUEST",
	"HOST_SET_CELL_LOT_INFORMATION",				// 2016-01-04, jhLee, S3F109, POCH 처리를 위한 Cell Lot Information 응답

	// 2016-01-20, 지정 Cell Data의 내용을 Clear 하는 요청
	"EQUIP_SET_CELL_DATA_CLEAR_REQUEST",			// 지정 Cell Data의 내용을 Clear

	"HOST_SET_CELL_JOB_PROCESS_STATE",				// 2017-06-23, HWARANG Cell Track In 시 JobProcess State 받는다.
	"HOST_SET_PPID_CREATE",						// 상위로부터 신규 레시피 생성 [9/27/2017 OSC]

	// EQIUPMENT FUNCTION LIST [11/14/2017 OSC]
	"HOST_SET_EQUIP_FUNCTION_CHANGE_REQ",		// 상위에서 Function 변경 요청
	"EQUIP_SET_EQUIP_FUNCTION_CHANGE_REQ_ACK",	// 상위 Function 변경 요청 가능/불가능 여부 회신
	"EQUIP_SET_EQUIP_FUNCTION_REPORT",			// CIM과 Function List 동기화
	"EQUIP_SET_EQUIP_FUNCTION_CHANGE",			// 설비에서 Function 변경 보고

	// INTERLOCK (기존 HOST_SET_INTERLOCK은 안쓴다 [11/16/2017 OSC]
	"HOST_SET_INTERLOCK_REQ",		// 인터락 발생 가능한지 설비에 확인 요청
	"EQUIP_SET_INTERLOCK_REQ_ACK",	// 인터락 발생 가능여부 회신

	// Unit Status Change [11/28/2017 OSC]
	"EQUIP_SET_UNIT_STATUS_REPORT",		// CIM 프로그램과 상태 동기화
	"EQUIP_SET_UNIT_STATUS_CHANGE",		// UNIT 상태 변경

	"EQUIP_SET_UNIT_INTERLOCK_COMFIRM",	
	"EQUIP_SET_UNIT_OPCALL_COMFIRM"	

};

// CString g_strUnitName[] =
// {
// 	"MECA_TR",
// 	"PREALIGN",
// 	"PREALIGN_TR",
// 	"UNLOAD",
// };

// int CCellProcData::GetCellIndex(char* pInnerId)
// {
// 	for (int i=0; i<MAX_CELL_IN_EQ; i++)
// 	{
// 		if (strcmp(m_tCellProcData[i].cInnerId, pInnerId)==0)
// 		{
// 			return i;
// 		}
// 	}
// 	return -1;
// }
// 
// int CCellProcData::GetCellIndexNotExist()
// {
// 	for (int i=0; i<MAX_CELL_IN_EQ; i++)
// 	{
// 		if (m_tCellProcData[i].eCellExist == CELL_NOT_EXIST)
// 		{
// 			return i;
// 		}
// 	}
// 	return -1;
// }
// 
// 
// int CCellProcData::SetCellInit(char* pInnerId)
// {
// 	for (int i=0; i<MAX_CELL_IN_EQ; i++)
// 	{
// 		if (strcmp(m_tCellProcData[i].cInnerId, pInnerId)==0)
// 		{
// 			m_tCellProcData[i].Clear();
// 			return i;
// 		}
// 	}
// 	return -1;
// }
// 
// void CCellProcData::Clear()
// {
// 	for (int i=0; i<MAX_CELL_IN_EQ; i++)
// 		m_tCellProcData[i].Clear();
// }

char CDefNameMap::cFdcInfoMap[MAP_LINE_CNT][MAP_MAX_COL][STR_SIZE] = {0};
int CDefNameMap::LoadFdcInfoMap(const int nArrayNo, char* pArray0, char* pFileName)
{
	int m=0;
	char cLine[1024];

	FILE	*fp;
	errno_t		err = 0;
	char* p0 = &cLine[0];
	char* pS, *pE;

	if(err = fopen_s(&fp, pFileName, "r"))
		return -1;

	int nAddr;
	int nCount=0;
	while(fgets(p0, 1024,fp)!=NULL)
	{
		if(strchr(p0, '#')!=NULL)// 주석
			continue;
		pS = p0;

		for(int i=0; i<nArrayNo-1; i++)
		{
			nAddr = (nCount*MAP_MAX_COL*STR_SIZE) + i*STR_SIZE;
			if((pE=strchr(pS, ','))==NULL)
			{
				nCount = -1;
				break;
			}
			*(pE++) = 0x00;
			strcpy_s(pArray0 + nAddr,STR_SIZE, pS);
			pS = pE;
		}

		if(nCount>=0)
		{
			nAddr = (nCount*MAP_MAX_COL*STR_SIZE) + (nArrayNo-1)*STR_SIZE;
			if((pE=strchr(pS, '\n'))!=NULL)			*(pE++) = 0x00;
			else if((pE=strchr(pS, 0x0a))!=NULL)	*(pE++) = 0x00;
			else if((pE=strchr(pS, 0x0d))!=NULL)	*(pE++) = 0x00;
			else
			{
				nCount = -1;
				break;
			}
			strcpy_s(pArray0 + nAddr,STR_SIZE, pS);
			nCount++;
		}
		else
			break;
	}

	fclose(fp);
	return nCount;
}



// 2015-09-09 JH-PARK DELETE
// char CDefNameMap::cTactInfoMap[MAP_LINE_CNT][MAP_MAX_COL][STR_SIZE] = {0};
// int CDefNameMap::LoadTactInfoMap(const int nArrayNo, char* pArray0, char* pFileName)	//공용으로 쓸걸...
// {
// 	int m=0;
// 	char cLine[1024];
// 
// 	FILE	*fp;
// 	errno_t		err = 0;
// 	char* p0 = &cLine[0];
// 	char* pS, *pE;
// 
// 	if(err = fopen_s(&fp, pFileName, "r"))
// 		return -1;
// 
// 	int nAddr;
// 	int nCount=0;
// 	while(fgets(p0, 1024,fp)!=NULL)
// 	{
// 		if(strchr(p0, '#')!=NULL)// 주석
// 			continue;
// 		pS = p0;
// 
// 		for(int i=0; i<nArrayNo-1; i++)
// 		{
// 			nAddr = (nCount*MAP_MAX_COL*STR_SIZE) + i*STR_SIZE;
// 			if((pE=strchr(pS, ','))==NULL)
// 			{
// 				nCount = -1;
// 				break;
// 			}
// 			*(pE++) = 0x00;
// 			strcpy_s(pArray0 + nAddr,STR_SIZE, pS);
// 			pS = pE;
// 		}
// 
// 		if(nCount>=0)
// 		{
// 			nAddr = (nCount*MAP_MAX_COL*STR_SIZE) + (nArrayNo-1)*STR_SIZE;
// 			if((pE=strchr(pS, '\n'))!=NULL)			*(pE++) = 0x00;
// 			else if((pE=strchr(pS, 0x0a))!=NULL)	*(pE++) = 0x00;
// 			else if((pE=strchr(pS, 0x0d))!=NULL)	*(pE++) = 0x00;
// 			else
// 			{
// 				nCount = -1;
// 				break;
// 			}
// 			strcpy_s(pArray0 + nAddr,STR_SIZE, pS);
// 			nCount++;
// 		}
// 		else
// 			break;
// 	}
// 
// 	fclose(fp);
// 	return nCount;
// }


void EQUIP_FUNCTION::Clear()
{
	USE = FALSE;
	EFID = 0;
	EFNAME.Empty();
	EFST_TYPE1.Empty();
	EFST_TYPE2.Empty();
	EFST_TYPE3.Empty();
	EFST_TYPE4.Empty();
	EFST_Old.Empty();
	EFST.Empty();
}

BOOL EQUIP_FUNCTION::EFST_Set(CString strValue)
{
	// 해당하는 값이 아니면 넣지 않는다 [11/14/2017 OSC]
	if(EFST_Check(strValue))
	{
		EFST = strValue;
		if(EFST != EFST_Old)
			EFST_Old = EFST;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL EQUIP_FUNCTION::EFST_Check(CString strValue)
{
	// 기존하고 틀린 값이고, 형식에 맞으면 TRUE
	if(EFST == strValue)
		return FALSE;

	if( (strValue == EFST_TYPE1) && (EFST_TYPE1.IsEmpty() == FALSE) )
		return TRUE;
	else if( (strValue == EFST_TYPE2) && (EFST_TYPE2.IsEmpty() == FALSE) )
		return TRUE;
	else if( (strValue == EFST_TYPE3) && (EFST_TYPE3.IsEmpty() == FALSE) )
		return TRUE;
	else if( (strValue == EFST_TYPE4) && (EFST_TYPE4.IsEmpty() == FALSE) )
		return TRUE;

	return FALSE;
}

CString EQUIP_FUNCTION::EFST_Get()
{
	return EFST;
}
