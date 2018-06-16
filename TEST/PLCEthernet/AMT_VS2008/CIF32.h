// CIF32.h: interface for the CCIF32 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CIF32_H__C8FA351C_B05B_44F1_A4AC_9599DD8FCD09__INCLUDED_)
#define AFX_CIF32_H__C8FA351C_B05B_44F1_A4AC_9599DD8FCD09__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cifuser.h"

#define DEVBOD					0
#define SEND_OFFSET				0
#define READ_OFFSET				0
#define SEND_SIZE				0
#define DEV_INPUT				16
#define DEV_OUTPUT				16
#define MAX_DEV_INPUT			32
#define MAX_DEV_OUTPUT			32
#define DEV_TIME_OUT			100L

class CCIF32  
{
public:
	int ReadOutBit(int iBitNum);
	int ReadIO(int iBitNum);

	BOOL CloseCard();
	BOOL OpenCard();
	CCIF32();
	virtual ~CCIF32();

public:
	void WriteIO(int nBitNum, BOOL bOnOff);
	unsigned short m_sReadData[512];
    unsigned short m_sSendData[512];
};

#endif // !defined(AFX_CIF32_H__C8FA351C_B05B_44F1_A4AC_9599DD8FCD09__INCLUDED_)
