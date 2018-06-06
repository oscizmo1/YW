#include "StdAfx.h"
#include "DeviceSerialTemperature.h"
#include "IbisApp.h"
#include "Etc/Etc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDeviceSerialTemperature::CDeviceSerialTemperature(CString strSerialName)
{
	m_strSerialName = strSerialName;
	//kjpark2017/09/01 Serial ���� - Temp �߰�, �� 2 ����, ��ũ ����, ���ο���
	m_serialMode = SERIAL_PC_BOX_TEMP;

	m_Serial.m_QueueRead.SetSTX(STX);
	//kjpark 20170922 �µ����� ��� ����
	m_Serial.m_QueueRead.SetETX(LF);

	for (int i = 0; i < eMAX_TEMPCount; i++) 
	{
		m_dbTempValue[i]	= 0.0;	
	}

	// Ƣ�� ����� �����͸� �����ϱ� ���� ��Ÿ�� ���
	//kjpark 20170922 �µ����� ��� ����
	m_nBoundLimit		= -1;						// ��ȸ �̻� ��/���� �������� �ݺ��Ǿ��� ������ ���ΰ� ?
	m_nUBoundCount		= 0;						// �������� Ƣ�� �� ���� �߻� Ƚ��
	m_nLBoundCount		= 0;						// ���Ѱ� ������ Ƣ�� �� ���� �߻� Ƚ��
	m_dbUBoundLimit	= 10;						// �������� Ƣ�� �� ����
	m_dbLBoundLimit	= 40;						// �������� Ƣ�� �� ����

	m_nDeviceAddr = 1;
}


CDeviceSerialTemperature::~CDeviceSerialTemperature(void)
{
}

BOOL CDeviceSerialTemperature::SerialOpen( short nPortNo )
{
	if(IsOpen()) return TRUE;

	return m_Serial.InitCommPort(nPortNo,CBR_9600,8,ONESTOPBIT,NOPARITY);
}

void CDeviceSerialTemperature::SerialClose()
{
	if(IsOpen() == FALSE) return;

	m_Serial.ClosePort();
}

BOOL CDeviceSerialTemperature::IsOpen()
{
	return m_Serial.m_bConnected;
}

//kjpark2017/09/01 Serial ���� - Temp �߰�, �� 2 ����, ��ũ ����, ���ο���
//kjpark2017/09/21 �µ����� �������� ��� �������� ���� �׽�Ʈ ��
void CDeviceSerialTemperature::SendCommand_TempValue(int nDeviceNum)
{
	// �ʿ��� �����͸� �����Ѵ�.
	BYTE bySend[32];
	BYTE byChecksum;
	int		nLen = 0;

	//char szToStr[8] = { 0x00, };
	//sprintf(szToStr, "%02d", nDeviceNum);

	CString sAddr;
	sAddr.Format(_T("%02d"),  nDeviceNum);

	bySend[nLen++]	= STX;		// STX
	bySend[nLen++]	= (BYTE)sAddr.GetAt(0);		// Address, 2
	bySend[nLen++]	= (BYTE)sAddr.GetAt(1);		

	bySend[nLen++]	= 'D';				// Command, ���ӵ����� �б� DRS
	bySend[nLen++]	= 'R';
	bySend[nLen++]	= 'S';

	bySend[nLen++]	= ',';				// Fixed

	bySend[nLen++]	= '0';				// Data count
	bySend[nLen++]	= '2';

	bySend[nLen++]	= ',';				// Fixed

	bySend[nLen++]	= '0';				// Read-Addr, 4, �����͸� �о� �� �ּ�
	bySend[nLen++]	= '0';
	bySend[nLen++]	= '0';
	bySend[nLen++]	= '1';

	byChecksum =  GetChecksum(bySend, nLen);		// Checksum�� �����.
	CString strCheckSum;
	strCheckSum.Format(_T("%02X"),  byChecksum);

	bySend[nLen++]	= (BYTE)strCheckSum.GetAt(0);		// Cehcksum, 2
	bySend[nLen++]	= (BYTE)strCheckSum.GetAt(1);

	bySend[nLen++]	= CR;			//	0x0D	// carriage return
	bySend[nLen++]	= LF;			//	0x0A	// NL line feed, new line


	// COM Port�� �����Ѵ�.
	int nSendLen =  m_Serial.WriteComm( bySend, nLen );

	//m_nSendCount = (m_nSendCount + 1) % eMAX_Count;		// ������ ���� ī���� ����

	//return nSendLen;

}
// Checksum�� ���Ѵ�.
// XOR�� ���� 2�� ������ ������ش�.
//kjpark 20170922 �µ����� ��� ����
BYTE CDeviceSerialTemperature::GetChecksum(BYTE* pData, int nLen)
{

	DWORD dwSum = 0;

	for(int i = 1 ; i < nLen ; i++)
	{
		dwSum = (dwSum + pData[i]);
	}
	return BYTE(dwSum & 0xFF);
}

//kjpark2017/09/01 Serial ���� - Temp �߰�, �� 2 ����, ��ũ ����, ���ο���
//kjpark2017/09/21 �µ����� �������� ��� �������� ���� �׽�Ʈ ��
BOOL CDeviceSerialTemperature::ParsingReciveValue()
{
	if(IsOpen() == FALSE) return FALSE;

	BYTE buf[200];
	BYTE buf1[200];
	BYTE buf2[200];
	int nReciveLength = m_Serial.m_QueueRead.GetDataSTX_To_ETX(buf2, 199);
	int nStartPos = 0;
	int nEndPos = 0;
	BOOL bCheckFlag = FALSE;

	if(nReciveLength < 1)
	{	
		return FALSE;
	}

	buf[nReciveLength] = NULL;	

	//           1         2
	// 012345678901234567890123
	//[ 0100000101000000000019 ]
	if ( nReciveLength > 0 )			// ���ŵ� �����Ͱ� �ִ°� ?
	{
		int nPos = 0;
		long lReadValue = 0;
	
		for (int i = 0; i < nReciveLength; i++)
		{
			//kjpark 20170922 �µ����� ��� ����
			if (buf2[i] == STX )		// ���� ���� LF�� �����ϴ°� ?
			{
				nStartPos = 0;			// ���� ���� ��ġ
			}
			buf1[nStartPos++] = buf2[i];

			//kjpark 20170922 �µ����� ��� ����
			if(nStartPos > 0 && buf2[i] == LF)
			{
				for(int j = 0; j < nStartPos; j++)
				{
					buf[j] = buf1[j];

				}
				nPos = nStartPos;
				break;
			}

		}	
			
		if ( nPos > 0 )
		{
			// �ʱ� ���ڰ� STX�̰�, �����ͱ��̰�  10����Ʈ�� �Ѵ°� ?
			if ( (nPos >= 23) && (buf[0] == 0x02) )		// �������� �������ΰ� ?
			{
				char szToStr[8] = { 0x00, };
				int nAddr = 0;
	
				// �ּ� ���
				szToStr[0] = buf[1];
				szToStr[1] = buf[2];
				szToStr[2] = 0x00;
	
				nAddr = atoi( szToStr );										// �ּҸ� ���������� ����
	
				// �������� �������� Ȯ��
				if (	(buf[7]	= 'O') &&		// ������ 'OK' �ΰ� ?
					(buf[8]	= 'K') )
				{
					char szValue[8] = { 0x00, };			// ���ڷ� ��Ȱ �� ���� �Ҵ�
					TCHAR szMsg[8] = { 0, };
					double dbValue = 0.0;		// �ӽ� ��
	
	
					// Checksum�� �����Ѵ�.
					// Checksum ä��
					szValue[0] = buf[nPos - 3];
					szValue[1] = buf[nPos - 2];
					szValue[2] = 0x00;
	
					long lReadValue = strtol(szValue, NULL, 16);													// 16�������ڿ��� ���������� ��ȯ
					BYTE byRcvChecksum = (BYTE)(lReadValue & 0xFF);												// ���Ź��� Checksum
					//kjpark 20170922 �µ����� ��� ����
					BYTE byLocalChecksum = GetChecksum( buf, nPos - 4 );	// ����� Checksum
	
					//TRACE(_T("- Checksum, Rcv:%c%c,%02X,    %02d,%02X\n"), szValue[0], szValue[1], byRcvChecksum, byLocalChecksum, byLocalChecksum);
	
					// Checksum�� ��ġ�ϸ� �ش� ������ ��� ����
					//if ( byRcvChecksum == byLocalChecksum )
					{
						// Data �κи� ���Ѵ�.
						szValue[0] = buf[10];
						szValue[1] = buf[11];
						szValue[2] = buf[12];
						szValue[3] = buf[13];
						szValue[4] = NULL;					// �� ����
	
						lReadValue = strtol(szValue, NULL, 16);								// 16�������ڿ��� ���������� ��ȯ
	
						for(int i = 0; i < 5; i++) szMsg[i] = szValue[i];
						szMsg[4] = 0x00;
						m_sRcvData[0].Format(_T("%s"), szMsg);							// ���ŵ� ������ ��ü�� ǥ��
	
						dbValue = lReadValue * 0.1;										// �µ��� ���
	
						// Ƣ�� �� ����ִ� �κ�

						// ���������� Ƣ�� ���
						if ( dbValue >= m_dbUBoundLimit )								
						{
							if ( m_nUBoundCount > m_nBoundLimit )						// ������ Ƚ���̻� �������� ���Դٸ� 
							{
								m_dbTempValue[m_nDeviceAddr-1] = dbValue;								// �ش簪�� �����ϰ� �޾Ƶ��δ�.
								bCheckFlag = TRUE;
							}
							else
							{
								m_nUBoundCount++;
							}// �������� ��� �� Ƚ���� ������Ų��.
						}
						else
						{
							if ( dbValue < m_dbLBoundLimit )							// ���� ���Ѱ� �Ʒ��� ���Դ�.
							{
								if ( m_nLBoundCount > m_nBoundLimit )					// ������ Ƚ���̻� �������� ���Դٸ� 
								{
									m_dbTempValue[m_nDeviceAddr-1] = dbValue;							// �ش簪�� �����ϰ� �޾Ƶ��δ�.
									bCheckFlag = TRUE;
								}
								else
								{
									m_nLBoundCount++;									// �������� ��� �� Ƚ���� ������Ų��.
								}
							}
							else		// �����ȿ� ������ ���̴�.
							{
								m_dbTempValue[m_nDeviceAddr-1] = dbValue;							// �ش簪�� �����ϰ� �޾Ƶ��δ�.
								bCheckFlag = TRUE;
								m_nUBoundCount = 0;
								m_nLBoundCount = 0;
							}	
						}						
	
					} //of if Checksum
				}//of if �����ڵ尡 �������̶��
			}//of Correct Data			
		}//of if CR Received ?
	
		// ó���Ϸ� 
	
		if(bCheckFlag == TRUE)
		{
			bCheckFlag = FALSE;
			//IncDeviceAddr();
		}
	}
	// ���̻� ó���Ұ� ������ TRUE [12/7/2016 OSC]
	if(m_Serial.m_QueueRead.GetDataSTX_To_ETX() <= 0)
		return TRUE;

	return FALSE;
}

//kjpark2017/09/01 Serial ���� - Temp �߰�, �� 2 ����, ��ũ ����, ���ο���
//�ּҰ� ����
void CDeviceSerialTemperature::IncDeviceAddr(void)
{
	//Receive�� �����ϰ�  Send �� �ϱ⶧���� 1-6���� �����ϰ�
	if(m_nDeviceAddr < eMAX_TEMPCount)
	{
		m_nDeviceAddr++;
	}
	else
	{
		m_nDeviceAddr = 1;  //6���� Receive�ϸ� 1�� �ʱ�ȭ
	}

}

//kjpark2017/09/01 Serial ���� - Temp �߰�, �� 2 ����, ��ũ ����, ���ο���
// ���� ������ ���� ���� �о�´�.
double CDeviceSerialTemperature::GetTempValue(int nIdx)
{
	if ( (nIdx < 0) || (nIdx >= eMAX_TEMPCount) ) return 0.0;

	return m_dbTempValue[nIdx];	
}
void CDeviceSerialTemperature::SetSerialName(CString strValue)
{
	m_strSerialName = strValue;
	m_Serial.m_serialMode = m_serialMode;
}