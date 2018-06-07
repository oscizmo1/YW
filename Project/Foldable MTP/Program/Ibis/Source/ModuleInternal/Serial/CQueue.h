
#pragma once

//#define WM_COMM_READ (WM_USER +1)
#define SERIAL_MAX_BUFFER	6500


//	��� Ŭ����	CCommThread 

// ��Ʈ���� �б� :
//   ��Ʈ�� �� �Ŀ� ��Ʈ�� �ڷᰡ �����ϸ� WM_COMM_READ �޽����� ���� 
//   �����쿡 ���޵ȴ�. ON_MESSAGE ��ũ�θ� �̿�, �Լ��� �����ϰ�
//   m_ReadData String�� ����� �����͸� �̿� �б�
// ��Ʈ�� ���� : WriteComm(buff, 30)�� ���� ���ۿ� �� ũ�⸦ �ǳ׸� �ȴ�.
class CQueue
{
public:
	BYTE buff[SERIAL_MAX_BUFFER];//ť ����
	int m_iHead, m_iTail;//ť ���ۿ� �����͸� �ְ� �� �ö� ����� ���� 
	CQueue();
	void Clear();//���۸� �ʱ�ȭ ��Ŵ
	int GetSize();//���� ���ۿ� ����ִ� �������� size�� ����
	BOOL PutByte(BYTE b);//ť���ۿ� 1����Ʈ�� ����
	BOOL GetByte(BYTE *pb);//ť���ۿ��� 1����Ʈ�� �� ��
	BOOL GetWord(short *pb);
	BOOL CQueue::Get8Byte(BYTE *pb);
	BOOL CQueue::Get12Byte(BYTE *pb);
	int GetData(BYTE* pByte, int nSize);

	int GetSTXPos();
	int GetETXPos();
	int GetSizeSTX_To_ETX();
	int GetDataSTX_To_ETX(BYTE *pb, int nMaxBuffer);	// STX���� ETX���� ������ ����.

};

