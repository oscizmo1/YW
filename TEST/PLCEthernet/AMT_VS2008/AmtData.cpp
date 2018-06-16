// AmtData.cpp: implementation of the CAmtData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MT.h"
#include "AmtData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAmtData::CAmtData()
{
	m_Melsec.m_iNetChanel = INT_MNET_CHANNEL;
	m_Melsec.m_iNetMode = INT_MNET_MODE;
	m_Melsec.m_iNetWork = INT_MNET_NETWORK;
	m_Melsec.m_iStation = INT_MNET_STATION;
	m_Melsec.m_lNetPath = INT_MNET_PATH;

	m_Flag.bInitDeviceNet = FALSE;
	m_Flag.bInitMotion = FALSE;

	QUAD_NOTUSEDAXIS_POS = 10 ;
}

CAmtData::~CAmtData()
{

}
