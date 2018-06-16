#include "StdAfx.h"
#include "MelsecNet.h"
#include "MT.h"

CMelsecNet::CMelsecNet(void)
{
	m_strSndBuf = _T("");
	m_strRcvBuf = _T("");
	m_bInitMelsec = FALSE;
}

CMelsecNet::~CMelsecNet(void)
{
}

long CMelsecNet::MelsecNetBitOn(short devno)
{
	mnetDevSet(m_pPLCInfo->m_iNetWork, m_pPLCInfo->m_iStation, DEV_TYP_BIT, devno);
	return TRUE;
}

long CMelsecNet::MelsecNetBitOff(short devno)
{
	mnetDevRst(m_pPLCInfo->m_iNetWork, m_pPLCInfo->m_iStation, DEV_TYP_BIT, devno);
	return TRUE;
}

short CMelsecNet::MelsecNetBitRecv(short devno)
{
	BOOL bFlag;
	mnetDevReceive(m_pPLCInfo->m_iNetWork, m_pPLCInfo->m_iStation, DEV_TYP_BIT, devno, &bFlag);

	return bFlag;
}

CString CMelsecNet::MelsecNetWordRecv(short devno, int nSize)
{
	int nWord = nSize / 2 ;
	int ret ;

	ret = ASCIIReceive(m_pPLCInfo->m_iNetWork, m_pPLCInfo->m_iStation, DEV_TYP_WORD, devno, &m_strRcvBuf, nWord);

	if(ret == 0)
	{
		return m_strRcvBuf;
	}
	CString strErrMsg;
	strErrMsg.Format("ER");
	return strErrMsg;
	return FALSE;
}

CString CMelsecNet::MelsecNetWordSend(short devno, char* sSndBuf)
{
	short ret;

	ret = ASCIISend(m_pPLCInfo->m_iNetWork, m_pPLCInfo->m_iStation, DEV_TYP_WORD, devno, sSndBuf);

	if(ret == 0)
	{
		return m_strSndBuf;
	}

	CString strErrMsg;
	strErrMsg.Format(" Error!!! ");
	return strErrMsg;
}

void CMelsecNet::GlassDataRead( PANEL_INFO &panelinfo, WORD wStartAddr )
{
	short size;
	int nNetwork = m_pPLCInfo->m_iNetWork;
	int nStation = m_pPLCInfo->m_iStation;

	size = sizeHPANELID		;	mnetReceive(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sHPanelID		);	wStartAddr += sizeHPANELID/2		;
	size = sizeEPANELID		;	mnetReceive(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sEPanelID		);	wStartAddr += sizeEPANELID/2		;
	size = sizeBATCHID		;	mnetReceive(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sBatchID		);	wStartAddr += sizeBATCHID/2			;
	size = sizePRODTYP		;	mnetReceive(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sProdType		);	wStartAddr += sizePRODTYP/2			;
	size = sizePRODKND		;	mnetReceive(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sProdKind		);	wStartAddr += sizePRODKND/2			;
	size = sizeDEVICEID		;	mnetReceive(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sDeviceID		);	wStartAddr += sizeDEVICEID/2		;
	size = sizeSTEPID		;	mnetReceive(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sStepID		);	wStartAddr += sizeSTEPID/2			;
	size = sizePPID			;	mnetReceive(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sPPID			);	wStartAddr += sizePPID/2			;
	size = sizeTHICKNESS	;	mnetReceive(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sThickness		);	wStartAddr += sizeTHICKNESS/2		;
	size = sizeINSPFLG		;	mnetReceive(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sInspectionFlag);	wStartAddr += sizeINSPFLG/2			;
	size = sizePANELSIZE	;	mnetReceive(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sPanelSize		);	wStartAddr += sizePANELSIZE/2		;
	size = sizePANELPOS		;	mnetReceive(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sPanelPosition	);	wStartAddr += sizePANELPOS/2		;
	size = sizeCOUNT1		;	mnetReceive(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sCount1		);	wStartAddr += sizeCOUNT1/2			;
	size = sizeCOUNT2		;	mnetReceive(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sCount2		);	wStartAddr += sizeCOUNT2/2			;
	size = sizeGRADE		;	mnetReceive(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sGrade			);	wStartAddr += sizeGRADE/2			;
	size = sizeCOMMENT		;	mnetReceive(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sComment		);	wStartAddr += sizeCOMMENT/2			;
	size = sizeCOMPSIZE		;	mnetReceive(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sCompSize		);	wStartAddr += sizeCOMPSIZE/2		;
	size = sizeREADFLG		;	mnetReceive(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sReadingFlag	);	wStartAddr += sizeREADFLG/2			;
	size = sizePANELSTATE	;	mnetReceive(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sPanelState	);	wStartAddr += sizePANELSTATE/2		;
	size = sizeJUDGEMENT	;	mnetReceive(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sJudgement		);	wStartAddr += sizeJUDGEMENT/2		;
	size = sizeCODE			;	mnetReceive(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sCode			);	wStartAddr += sizeCODE/2			;
	size = sizeRUNLINE		;	mnetReceive(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sRunLine		);	wStartAddr += sizeRUNLINE/2			;
	size = sizeUNIQUEID		;	mnetReceive(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sUniqueID		);	wStartAddr += sizeUNIQUEID/2		;
	size = sizePAIR_HID		;	mnetReceive(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sPairHPanelID	);	wStartAddr += sizePAIR_HID/2		;
	size = sizePAIR_EID		;	mnetReceive(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sPairEPanelID	);	wStartAddr += sizePAIR_EID/2		;
	size = sizePAIR_GRADE	;	mnetReceive(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sPairGrade		);	wStartAddr += sizePAIR_GRADE/2		;
	size = sizePAIR_UNIQUEID;	mnetReceive(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sPairUniqueID	);	wStartAddr += sizePAIR_UNIQUEID/2	;
	size = sizeFLOWRECIPE	;	mnetReceive(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sFlowRecipe	);	wStartAddr += sizeFLOWRECIPE/2		;
   	size = sizeRESERVED		;	mnetReceive(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sReserved		);	wStartAddr += sizeRESERVED/2		;
	size = sizeBITSIGNAL	;	mnetReceive(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sBitsSignals	);	wStartAddr += sizeBITSIGNAL/2		;
	size = sizeREFERDATA	;	mnetReceive(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sReferData		);

// 	memcpy(panelinfo.m_sReserved,panelinfo.m_sReserved, 4);

}

void CMelsecNet::GlassDataWrite( PANEL_INFO &panelinfo, WORD wStartAddr )
{
	short size;
	int nNetwork = m_pPLCInfo->m_iNetWork;
	int nStation = m_pPLCInfo->m_iStation;

	size = sizeHPANELID		;	mnetSend(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sHPanelID			);	wStartAddr += sizeHPANELID/2		;
	size = sizeEPANELID		;	mnetSend(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sEPanelID			);	wStartAddr += sizeEPANELID/2		;
	size = sizeBATCHID		;	mnetSend(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sBatchID			);	wStartAddr += sizeBATCHID/2			;
	size = sizePRODTYP		;	mnetSend(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sProdType			);	wStartAddr += sizePRODTYP/2			;
	size = sizePRODKND		;	mnetSend(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sProdKind			);	wStartAddr += sizePRODKND/2			;
	size = sizeDEVICEID		;	mnetSend(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sDeviceID			);	wStartAddr += sizeDEVICEID/2		;
	size = sizeSTEPID		;	mnetSend(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sStepID			);	wStartAddr += sizeSTEPID/2			;
	size = sizePPID			;	mnetSend(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sPPID				);	wStartAddr += sizePPID/2			;
	size = sizeTHICKNESS	;	mnetSend(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sThickness		);	wStartAddr += sizeTHICKNESS/2		;
	size = sizeINSPFLG		;	mnetSend(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sInspectionFlag	);	wStartAddr += sizeINSPFLG/2			;
	size = sizePANELSIZE	;	mnetSend(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sPanelSize		);	wStartAddr += sizePANELSIZE/2		;
	size = sizePANELPOS		;	mnetSend(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sPanelPosition	);	wStartAddr += sizePANELPOS/2		;
	size = sizeCOUNT1		;	mnetSend(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sCount1			);	wStartAddr += sizeCOUNT1/2			;
	size = sizeCOUNT2		;	mnetSend(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sCount2			);	wStartAddr += sizeCOUNT2/2			;
	size = sizeGRADE		;	mnetSend(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sGrade			);	wStartAddr += sizeGRADE/2			;
	size = sizeCOMMENT		;	mnetSend(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sComment			);	wStartAddr += sizeCOMMENT/2			;
	size = sizeCOMPSIZE		;	mnetSend(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sCompSize			);	wStartAddr += sizeCOMPSIZE/2		;
	size = sizeREADFLG		;	mnetSend(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sReadingFlag		);	wStartAddr += sizeREADFLG/2			;
	size = sizePANELSTATE	;	mnetSend(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sPanelState		);	wStartAddr += sizePANELSTATE/2		;
	size = sizeJUDGEMENT	;	mnetSend(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size," "							);
	size = sizeJUDGEMENT-2	;	mnetSend(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sJudgement		);	wStartAddr += sizeJUDGEMENT/2		;
	size = sizeCODE			;	mnetSend(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sCode				);	wStartAddr += sizeCODE/2			;
	size = sizeRUNLINE		;	mnetSend(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sRunLine			);	wStartAddr += sizeRUNLINE/2			;
	size = sizeUNIQUEID		;	mnetSend(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sUniqueID			);	wStartAddr += sizeUNIQUEID/2		;
	size = sizePAIR_HID		;	mnetSend(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sPairHPanelID		);	wStartAddr += sizePAIR_HID/2		;
	size = sizePAIR_EID		;	mnetSend(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sPairEPanelID		);	wStartAddr += sizePAIR_EID/2		;
	size = sizePAIR_GRADE	;	mnetSend(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sPairGrade		);	wStartAddr += sizePAIR_GRADE/2		;
	size = sizePAIR_UNIQUEID;	mnetSend(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sPairUniqueID		);	wStartAddr += sizePAIR_UNIQUEID/2	;
	size = sizeFLOWRECIPE	;	mnetSend(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sFlowRecipe		);	wStartAddr += sizeFLOWRECIPE/2		;
	size = sizeRESERVED		;	mnetSend(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sReserved			);	wStartAddr += sizeRESERVED/2		;
	size = sizeBITSIGNAL	;	mnetSend(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sBitsSignals		);	wStartAddr += sizeBITSIGNAL/2		;
	size = sizeREFERDATA	;	mnetSend(nNetwork, nStation, DEV_TYP_WORD, wStartAddr, &size, panelinfo.m_sReferData		);
}

BOOL CMelsecNet::Open()
{
	SetConnected(FALSE);
	if( mnetOpen( m_pPLCInfo->m_iNetChanel , m_pPLCInfo->m_iNetMode , &m_pPLCInfo->m_lNetPath) == 0)
	{
		SetConnected(TRUE);
		return TRUE;
	}

	return FALSE;
}

void CMelsecNet::Close()
{
	SetConnected(FALSE);
	mnetClose( m_pPLCInfo->m_lNetPath);
}

CString CMelsecNet::NonStopDeviceRead()
{
	short size;
	int nNetwork = m_pPLCInfo->m_iNetWork;
	int nStation = m_pPLCInfo->m_iStation;

	size = sizeNonStopDeviceID		;	
	CString strTemp;
	char* buf = strTemp.GetBuffer(sizeNonStopDeviceID+1);
	char* buf1[sizeNonStopDeviceID+1];
	mnetReceive(nNetwork, nStation, DEV_TYP_WORD, NON_STOP_DEVICEID, &size, buf1		);
	buf1[sizeNonStopDeviceID] = NULL;
	memcpy(buf, buf1, sizeNonStopDeviceID+1);
	strTemp.ReleaseBuffer();
	strTemp.Remove(_T(' '));
	return strTemp;
}

void CMelsecNet::NonStopFailTypeWrite( LPCTSTR szFailType )
{
	short size;
	int nNetwork = m_pPLCInfo->m_iNetWork;
	int nStation = m_pPLCInfo->m_iStation;

	size = sizeNonStopFailType		;	
	char buf[sizeNonStopFailType+1];
	memcpy(buf, szFailType, sizeNonStopFailType	);
	buf[size] = NULL;
	mnetSend(nNetwork, nStation, DEV_TYP_WORD, NON_STOP_FAIL_TYPE, &size, buf			);	
}

