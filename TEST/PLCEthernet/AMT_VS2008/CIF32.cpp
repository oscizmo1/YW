// CIF32.cpp: implementation of the CCIF32 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MT.h"
#include "CIF32.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCIF32::CCIF32()
{
	for(int i = 0; i < 512; i++)
	{
		m_sReadData[i] = 0;
		m_sSendData[i] = 0;
	}
}

CCIF32::~CCIF32()
{

}

BOOL CCIF32::OpenCard()
{
	if(DevOpenDriver(DEVBOD) != DRV_NO_ERROR)
		return FALSE;        
    if(DevInitBoard(DEVBOD, NULL) != DRV_NO_ERROR)
	{
		DevCloseDriver(DEVBOD);
		return FALSE;
	}
    if(DevSetHostState(DEVBOD, HOST_READY, 0L) != DRV_NO_ERROR) 
	{
		DevExitBoard(DEVBOD);
		DevCloseDriver(DEVBOD);
        return FALSE;
	}
	return TRUE;
}

BOOL CCIF32::CloseCard()
{
	if(DevSetHostState(DEVBOD, HOST_NOT_READY, 0L) != DRV_NO_ERROR)
		return FALSE;
	if(DevExitBoard(DEVBOD) != DRV_NO_ERROR)
		return FALSE;
	if(DevCloseDriver(DEVBOD) != DRV_NO_ERROR) 
		return FALSE;
	return TRUE;
}

int CCIF32::ReadOutBit(int iBitNum)
{
/*	BOOL bRet = FALSE;
	if (iBitNum < 0 || iBitNum > MAX_DEV_INPUT) 
		return FALSE;

	int DataNum = iBitNum / 16;
	int PortNum = 1 << (iBitNum % 16);
	int BitNum = iBitNum % 16;
	
	if(DevExchangeIO(DEVBOD, SEND_OFFSET, MAX_DEV_OUTPUT, m_sSendData, 0, MAX_DEV_INPUT, m_sReadData, DEV_TIME_OUT) != DRV_NO_ERROR)
		return FALSE;
	
	bRet = (m_sReadData[DataNum] >> BitNum) & 0x0001;


	return bRet;*/
	int DataNum = iBitNum / DEV_OUTPUT;
	int PortNum = 1 << (iBitNum % DEV_OUTPUT);
		
    if(iBitNum < 0 || iBitNum > MAX_DEV_OUTPUT)
		return FALSE;
		

    int d = m_sSendData[DataNum] & PortNum;
	if(d)
		return TRUE;
	else
		return FALSE;

}

int CCIF32::ReadIO(int iBitNum)
{
	BOOL bRet = FALSE;
	if (iBitNum < 0 || iBitNum > MAX_DEV_INPUT) 
		return FALSE;

	int DataNum = iBitNum / 16;
	int PortNum = 1 << (iBitNum % 16);
	int BitNum = iBitNum % 16;
	
	if(DevExchangeIO(DEVBOD, SEND_OFFSET, MAX_DEV_OUTPUT, m_sSendData, READ_OFFSET, MAX_DEV_INPUT, m_sReadData, DEV_TIME_OUT) != DRV_NO_ERROR)
		return FALSE;
	
	bRet = (m_sReadData[DataNum] >> BitNum) & 0x0001;


	return bRet;


}

void CCIF32::WriteIO(int nBitNum, BOOL bOnOff)
{
	int DataNum = nBitNum / DEV_OUTPUT;
    int PortNum = 1 << (nBitNum % DEV_OUTPUT);

   	if(bOnOff)
		m_sSendData[DataNum] = m_sSendData[DataNum] | PortNum;
	else
		m_sSendData[DataNum] = m_sSendData[DataNum] & (~PortNum);
	DevExchangeIO(DEVBOD, SEND_OFFSET, MAX_DEV_OUTPUT, m_sSendData, READ_OFFSET, MAX_DEV_INPUT, m_sReadData, DEV_TIME_OUT);

}
