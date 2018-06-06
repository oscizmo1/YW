#include "StdAfx.h"
#include "DeviceSerialVacuum.h"
#include "IbisApp.h"
#include "Etc/Etc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDeviceSerialVacuum::CDeviceSerialVacuum()
{
	m_Serial.m_QueueRead.SetStartChar(STX);
	m_Serial.m_QueueRead.SetEndChar(ETX);
	m_nReadCount	= 4;
	m_nReadSlot = 0;

	memset(m_dbVacValue, 0, sizeof(double)*eMAX_ADSlot*eMAX_ADChannel);

	m_dOffset = 0.;
}


CDeviceSerialVacuum::~CDeviceSerialVacuum(void)
{
}

BOOL CDeviceSerialVacuum::SerialOpen( short nPortNo )
{
	if(IsOpen()) return TRUE;

	return m_Serial.InitCommPort(nPortNo,CBR_19200,8,ONESTOPBIT,EVENPARITY);
}

void CDeviceSerialVacuum::SerialClose()
{
	if(IsOpen() == FALSE) return;

	m_Serial.ClosePort();
}

BOOL CDeviceSerialVacuum::IsOpen()
{
	return m_Serial.m_bConnected;
}

BOOL CDeviceSerialVacuum::ParsingReciveValue()
{
	if(IsOpen() == FALSE) return FALSE;

	BYTE buf[100];
	int nReciveLength = m_Serial.m_QueueRead.GetDataStartChar_To_EndChar(buf, 99);
	if(nReciveLength < 1)	return FALSE;
	buf[nReciveLength] = NULL;

	int nSizeLimit = 8 + (m_nReadCount * 5);		// �ּ� ������ ���� = �ش� + ���ſ�û ������ ���� * �����ʹ� ����
	char szValue[6];			// �ʿ��� ��ŭ�� �Ҵ�
	TCHAR szMsg[256] = { 0, };

	if(nReciveLength < nSizeLimit)
	{
		theLog[LOG_SERIAL].AddBuf(_T("RECIVE Length ERR %d/%d [%s]"), nReciveLength, nSizeLimit, buf);
		return FALSE;
	}

	if (	(buf[ ePOS_ResCode ]	= '0') &&		// Response Code�� '00' �ΰ� ?
			(buf[ ePOS_ResCode + 1 ]	= '0') )
	{
		//9, 14, 19, 24,
		// 9 + (0 * 5) = 9
		// 9 + (1 * 5) = 14
		// 9 + (2 * 5) = 19
		// 9 + (3 * 5) = 24
		int nStart = ePOS_DataStart;	// 9;				// ������ ������ġ

		for (int i = 0; i < m_nReadCount; i++)
		{
			// nStart = 9 + (i * 5);
			nStart = ePOS_DataStart + (i * (eLEN_Data + eLEN_Delimiter));		// �����͸� ������ ��ġ

			for (int j = 0; j < eLEN_Data; j++)
			{
				szValue[j]	= buf[ nStart + j ];			// 4Bytes�� �����Ѵ�.
				szMsg[j]	= szValue[j];
			}
			szValue[eLEN_Data]	= 0x00;				// �ι��ڷ� ���� ��Ų��.
			szMsg[eLEN_Data]	= 0x00;

			m_sRcvData[m_nReadSlot][i].Format(_T("%s"), szMsg);					// ���ŵ� ������ ��ü�� ǥ��

			if ( szValue[0] != 'L' )		// ���� ������ �������� �ʾҴٸ�, "LLLL"
			{
				m_dbVacValue[m_nReadSlot][i] = abs(atof(szValue) + m_dOffset);		// �Ǽ������� ��ȯ�Ѵ�.
			}
			else
				m_dbVacValue[m_nReadSlot][i] = 0.;		// �б� ����, ���� �Է� �ҷ� ���� ����
		}//of for i
	}

	// ���̻� ó���Ұ� ������ TRUE [12/7/2016 OSC]
	if(m_Serial.m_QueueRead.GetSizeStartChar_To_EndChar() <= 0)
		return TRUE;

	return FALSE;

}

void CDeviceSerialVacuum::ClearQueue()
{
	m_Serial.m_QueueRead.Clear();
}

// Checksum�� ���Ѵ�.
// XOR�� ���� 2�� ������ ������ش�.
BYTE CDeviceSerialVacuum::GetChecksum(BYTE* pData, int nLen)
{
	BYTE bySum = pData[0];

	for (int i = 1; i < nLen; i++) 
	{
		// XOR ������ �ƴ� bySum = bySum ^ pData[i];			// XOR ����
		bySum = (bySum + pData[i]) & 0xFF;			// �ջ� ����
	}

	return (( ~bySum + 1 ) & 0xFF);		// Byte size�� �°� �ٲپ� �ش�.
}
// ����� �����Ѵ�.
// Step�� 0 ~
// Count�� 1 ~
DWORD CDeviceSerialVacuum::SendReadCommand(int nCount)
{
	// �ʿ��� �����͸� �����Ѵ�.
	BYTE	bySend[32] = { 0x00, };
	int		nLen = 0;
	CString sCheck;

	int nBaseAddr =1000;
	int nNewAddr = nBaseAddr + (nCount * m_nReadSlot);			// �о�� �ּҸ� ����


	CString sAddr;
	sAddr.Format(_T("%04d"),  nNewAddr);


	bySend[nLen++]	= 0x02;				// STX
	bySend[nLen++]	= '0';
	bySend[nLen++]	= '1';
	bySend[nLen++]	= '0';
	bySend[nLen++]	= '1';
	bySend[nLen++]	= 'X';

	bySend[nLen++]	= 'R';				// ������ �б�
	bySend[nLen++]	= 'S';
	bySend[nLen++]	= ',';

	//bySend[nLen++]	= '1';				// �����͸� �о� �� �ּ�
	//bySend[nLen++]	= '0';
	//bySend[nLen++]	= '0';
	//bySend[nLen++]	= '0';
	//bySend[nLen++]	= 'W';

	// �����͸� �о� �� �ּ�
	bySend[nLen++]	= (BYTE)sAddr.GetAt(0);				
	bySend[nLen++]	= (BYTE)sAddr.GetAt(1);	
	bySend[nLen++]	= (BYTE)sAddr.GetAt(2);	
	bySend[nLen++]	= (BYTE)sAddr.GetAt(3);	
	bySend[nLen++]	= 'W';

	bySend[nLen++]	= ',';
	bySend[nLen++]	= '1' + (nCount - 1);				// �ִ� 4���� �����͸� �о�´�. -? 2015-07-31, jhLee, 5 - 8���� ������ ��� ������ �ȵ�
	bySend[nLen++]	= 0x03;				// ETX

	sCheck.Format(_T("%02X"), GetChecksum(bySend, nLen));		// Checksum�� �����.

	bySend[nLen++]	= (BYTE)sCheck.GetAt(0);					// Checksum�� BYTE 2�ڸ��� ����� �ش�.
	bySend[nLen++]	= (BYTE)sCheck.GetAt(1);

	bySend[nLen++]	= 0x0D;		// CR
	bySend[nLen++]	= 0x0A;		// LF
	bySend[nLen]	= 0x00;		// NULL

	int nSendLen =  m_Serial.WriteComm( bySend, nLen );

	return nSendLen;
}

// �Է� ���� ���� ����� �����Ѵ�.
//
// [in] int nCh : ������ �����ϰ��� �ϴ� ä�� ����, 0 ~ 31
// [in] int nMode : �����ϰ��� �ϴ� ���� ��尪
//
DWORD CDeviceSerialVacuum::SendRangeSetCommand(int nCh, int nMode)
{
	// �ʿ��� �����͸� �����Ѵ�.
	BYTE	bySend[32];
	int		nLen = 0;
	CString sCheck;

	int nNewAddr = 3000 + nCh;			// ������ ������ ä�� �ּ�, 0 ~ 31

	CString sAddr, sValue;
	sAddr.Format(_T("%04d"),  nNewAddr);		// �ּҸ� ���ڿ��� ������ش�.
	sValue.Format(_T("%04d"), nMode);			// ���� ��带 ����� �ش�. "0003"

	bySend[nLen++]	= 0x02;				// STX
	bySend[nLen++]	= '0';
	bySend[nLen++]	= '1';
	bySend[nLen++]	= '0';
	bySend[nLen++]	= '1';
	bySend[nLen++]	= 'X';


	bySend[nLen++]	= 'W';				// ������ ����
	bySend[nLen++]	= 'S';
	bySend[nLen++]	= ',';

	//bySend[nLen++]	= '3';				// �����͸� �� �ּ�
	//bySend[nLen++]	= '0';
	//bySend[nLen++]	= '0';
	//bySend[nLen++]	= '4';

	// �����͸� �о� �� �ּ�
	bySend[nLen++]	= (BYTE)sAddr.GetAt(0);				
	bySend[nLen++]	= (BYTE)sAddr.GetAt(1);	
	bySend[nLen++]	= (BYTE)sAddr.GetAt(2);	
	bySend[nLen++]	= (BYTE)sAddr.GetAt(3);	
	bySend[nLen++]	= 'W';

	bySend[nLen++]	= ',';

	//bySend[nLen++]	= '0';				
	//bySend[nLen++]	= '0';				
	//bySend[nLen++]	= '0';				// ������
	//bySend[nLen++]	= '3';				

	bySend[nLen++]	= (BYTE)sValue.GetAt(0);				
	bySend[nLen++]	= (BYTE)sValue.GetAt(1);	
	bySend[nLen++]	= (BYTE)sValue.GetAt(2);	
	bySend[nLen++]	= (BYTE)sValue.GetAt(3);	

	bySend[nLen++]	= 0x03;				// ETX

	sCheck.Format(_T("%02X"), GetChecksum(bySend, nLen));		// Checksum�� �����.

	bySend[nLen++]	= (BYTE)sCheck.GetAt(0);					// Checksum�� BYTE 2�ڸ��� ����� �ش�.
	bySend[nLen++]	= (BYTE)sCheck.GetAt(1);

	bySend[nLen++]	= 0x0D;		// CR
	bySend[nLen++]	= 0x0A;		// LF
	bySend[nLen]	= 0x00;		// NULL


	int nSendLen =  m_Serial.WriteComm( bySend, nLen );

	return nSendLen;
}

void CDeviceSerialVacuum::SetSerialName(CString strValue)
{
	m_strSerialName = strValue;
// 	m_Serial.m_serialMode = m_serialMode;
}