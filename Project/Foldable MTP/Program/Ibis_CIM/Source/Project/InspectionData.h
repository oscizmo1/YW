#pragma once
#ifndef _INSPECTION_DATA_H_
#define _INSPECTION_DATA_H_
class CInspectionData
{
public:
	CInspectionData(void);
	~CInspectionData(void);

protected:
	//CCriticalSection	m_criticalData;

	
public:
	CString MakeString(CString str);
	CString MakeString(CString str, int nLen);
	int GetChannelNo(UINT uJig, UINT uCellPos);
	CString GetChannelString(UINT uJig, UINT uCellPos);

//	BOOL ReadTeachFile(CString strPPID);
//	BOOL ReadCellFile(CString strPPID);
//	BOOL ReadECVFile();
//	BOOL ReadErrListFile();

	BOOL bIctEvent[MAX_JIG_CNT][MAX_JIG_CELL_CNT];
	BOOL bInfoEvent[MAX_JIG_CNT][MAX_JIG_CELL_CNT];
	BOOL bPwrEvent[MAX_JIG_CNT][MAX_JIG_CELL_CNT];
	BOOL bMtpWEvent[MAX_JIG_CNT][MAX_JIG_CELL_CNT];
	BOOL bTspEvent[MAX_JIG_CNT][MAX_JIG_CELL_CNT];
	BOOL bMtpREvent[MAX_JIG_CNT][MAX_JIG_CELL_CNT];
	BOOL bVtEvent[MAX_JIG_CNT][MAX_JIG_CELL_CNT];
	
	BOOL InitEvent();
	BOOL CellIdCheckUp(char* pCellId);
	BOOL CellIdCheckUp(CString strCellId);
	BOOL LoadVTInformIni();

	// 2015-09-21 JH-PARK, Yms -> QMS¸íÄª º¯°æ
	BOOL SetQMSRtn(UINT uJig, UINT uCellPos, E_RESULT_FILE_TYPE eRType, E_QMS_RETURN eYmsRtn);
	E_QMS_RETURN GetQMSRtn(UINT uJig, UINT uCellPos, E_RESULT_FILE_TYPE eRType);

// 	BOOL SetYmsRtn(UINT uJig, UINT uCellPos, E_RESULT_FILE_TYPE eRType, E_YMS_RETURN eYmsRtn);
// 	E_YMS_RETURN GetYmsRtn(UINT uJig, UINT uCellPos, E_RESULT_FILE_TYPE eRType);
};

#endif