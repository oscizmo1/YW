#pragma once

#include "Serial/CommThread.h"

class CDeviceSerialVacuum
{
public:
	// A/D Board �ϳ��� �ִ� 32ch���� ����
	// 1Slot�� 4Ch�� �ִ� 8Slot
	enum 
	{
		eMAX_ADSlot		= 8,		// ��� 2���� 4Slot�� �����Ǿ�����, �ִ� 8�� �������� 
		eMAX_ADChannel	= 4,		// A/D ���忡�� �� �� �о��� �� �ִ� ���� �� �ִ� ������ 8���̴�.

		eLEN_STX		= 1,				// STX ����
		eLEN_Header		= 8,				// Header
		eLEN_Data		= 4,				// ������ ���� ����
		eLEN_Delimiter	= 1,

		ePOS_ResCode	= 6,				// ���� �ڵ尡 ��ġ�� ��
		ePOS_DataStart	= eLEN_STX + eLEN_Header,		// = 9
	};

private:
	CCommThread m_Serial;
	CString m_strSerialName;
	int m_nReadCount;
	CString	m_sRcvData[eMAX_ADSlot][eMAX_ADChannel];	// ���� ���ŵ� ����
	double m_dOffset;			// ���а��Ϸ� �߻��ϴ� Offset
public:
	int m_nReadSlot;
	double	m_dbVacValue[eMAX_ADSlot][eMAX_ADChannel];	// �� ���� ���� �� �ִ� �ʴ� ������ �⺻���� ����Ѵ�. eMAX_VACCount];		// �о� �� �������� ����

	CDeviceSerialVacuum();
	~CDeviceSerialVacuum(void);


	BOOL SerialOpen(short nPortNo);
	void SerialClose();
	BOOL IsOpen();

	// A/D����κ��� 232��Ʈ���� �� ���¿� ���� ���а��ϰ� �Ͼ��
	// ���� ��ġ���� ������ ���ڸ��� �޴´�.
	// �װ� �޲ٱ� ���� Offset
	void SetValueOffset(double dOffset) { m_dOffset = dOffset; }	

	BOOL ParsingReciveValue();
	void ClearQueue();


	BYTE GetChecksum(BYTE* pData, int nLen);
	DWORD SendReadCommand(int nCount);
	DWORD SendRangeSetCommand(int nCh, int nMode);

	CCommThread GetCommThreadHandler() {return m_Serial;};
	void SetSerialName(CString strValue);
};

