
#pragma once

//#define WM_COMM_READ (WM_USER +1)
#define SERIAL_MAX_BUFFER	6500


//	��� Ŭ����	CCommThread 

// ��Ʈ���� �б� :
//   ��Ʈ�� �� �Ŀ� ��Ʈ�� �ڷᰡ �����ϸ� WM_COMM_READ �޽����� ���� 
//   �����쿡 ���޵ȴ�. ON_MESSAGE ��ũ�θ� �̿�, �Լ��� �����ϰ�
//   m_ReadData String�� ����� �����͸� �̿� �б�
// ��Ʈ�� ���� : WriteComm(buff, 30)�� ���� ���ۿ� �� ũ�⸦ �ǳ׸� �ȴ�.
class CQueueSerial
{
private:
	BYTE buff[SERIAL_MAX_BUFFER];//ť ����
	int m_iHead, m_iTail;//ť ���ۿ� �����͸� �ְ� �� �ö� ����� ���� 
	char m_cStart, m_cEnd;	// ����, ���Ṯ��
public:
	CQueueSerial();
	void Clear();//���۸� �ʱ�ȭ ��Ŵ
	int GetSize();//���� ���ۿ� ����ִ� �������� size�� ����
	BOOL PutByte(BYTE b);//ť���ۿ� 1����Ʈ�� ����
	BOOL GetByte(BYTE *pb);//ť���ۿ��� 1����Ʈ�� �� ��
	BOOL GetWord(short *pb);
	int GetData(BYTE* pByte, int nSize);

	void SetStartChar(char cStart) { m_cStart = cStart; }
	void SetEndChar(char cEnd) { m_cEnd = cEnd; }
	int GetStartCharPos();
	int GetEndCharPos();
	int GetSizeStartChar_To_EndChar();
	int GetDataStartChar_To_EndChar(BYTE *pb, int nMaxBuffer);	// STX���� ETX���� ������ ����.

	//kjpark2017/09/01 Serial ���� - Temp �߰�, �� 2 ����, ��ũ ����, ���ο���
	void SetSTX(char cStart);
	void SetETX(char cEnd);
	int GetSTXPos();
	int GetETXPos();
	int GetDataSTX_To_ETX();
	int GetDataSTX_To_ETX(BYTE *pb, int nMaxBuffer);	// STX���� ETX���� ������ ����.



};

