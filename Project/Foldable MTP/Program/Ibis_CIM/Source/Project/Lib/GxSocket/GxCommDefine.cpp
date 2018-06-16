#include "stdafx.h"
#include "GxSocket\GxCommDefine.h"


//=============================================================================
// COMMON 
//=============================================================================

static const BYTE CRC8_Table1[256] = 
{
	0x00, 0x5e, 0xbc, 0xe2, 0x61, 0x3f, 0xdd, 0x83,
	0xc2, 0x9c, 0x7e, 0x20, 0xa3, 0xfd, 0x1f, 0x41,
	0x9d, 0xc3, 0x21, 0x7f, 0xfc, 0xa2, 0x40, 0x1e,
	0x5f, 0x01, 0xe3, 0xbd, 0x3e, 0x60, 0x82, 0xdc,
	0x23, 0x7d, 0x9f, 0xc1, 0x42, 0x1c, 0xfe, 0xa0,
	0xe1, 0xbf, 0x5d, 0x03, 0x80, 0xde, 0x3c, 0x62,
	0xbe, 0xe0, 0x02, 0x5c, 0xdf, 0x81, 0x63, 0x3d,
	0x7c, 0x22, 0xc0, 0x9e, 0x1d, 0x43, 0xa1, 0xff,
	0x46, 0x18, 0xfa, 0xa4, 0x27, 0x79, 0x9b, 0xc5,
	0x84, 0xda, 0x38, 0x66, 0xe5, 0xbb, 0x59, 0x07,
	0xdb, 0x85, 0x67, 0x39, 0xba, 0xe4, 0x06, 0x58,
	0x19, 0x47, 0xa5, 0xfb, 0x78, 0x26, 0xc4, 0x9a,
	0x65, 0x3b, 0xd9, 0x87, 0x04, 0x5a, 0xb8, 0xe6,
	0xa7, 0xf9, 0x1b, 0x45, 0xc6, 0x98, 0x7a, 0x24,
	0xf8, 0xa6, 0x44, 0x1a, 0x99, 0xc7, 0x25, 0x7b,
	0x3a, 0x64, 0x86, 0xd8, 0x5b, 0x05, 0xe7, 0xb9,
	0x8c, 0xd2, 0x30, 0x6e, 0xed, 0xb3, 0x51, 0x0f,
	0x4e, 0x10, 0xf2, 0xac, 0x2f, 0x71, 0x93, 0xcd,
	0x11, 0x4f, 0xad, 0xf3, 0x70, 0x2e, 0xcc, 0x92,
	0xd3, 0x8d, 0x6f, 0x31, 0xb2, 0xec, 0x0e, 0x50,
	0xaf, 0xf1, 0x13, 0x4d, 0xce, 0x90, 0x72, 0x2c,
	0x6d, 0x33, 0xd1, 0x8f, 0x0c, 0x52, 0xb0, 0xee,
	0x32, 0x6c, 0x8e, 0xd0, 0x53, 0x0d, 0xef, 0xb1,
	0xf0, 0xae, 0x4c, 0x12, 0x91, 0xcf, 0x2d, 0x73,
	0xca, 0x94, 0x76, 0x28, 0xab, 0xf5, 0x17, 0x49,
	0x08, 0x56, 0xb4, 0xea, 0x69, 0x37, 0xd5, 0x8b,
	0x57, 0x09, 0xeb, 0xb5, 0x36, 0x68, 0x8a, 0xd4,
	0x95, 0xcb, 0x29, 0x77, 0xf4, 0xaa, 0x48, 0x16,
	0xe9, 0xb7, 0x55, 0x0b, 0x88, 0xd6, 0x34, 0x6a,
	0x2b, 0x75, 0x97, 0xc9, 0x4a, 0x14, 0xf6, 0xa8,
	0x74, 0x2a, 0xc8, 0x96, 0x15, 0x4b, 0xa9, 0xf7,
	0xb6, 0xe8, 0x0a, 0x54, 0xd7, 0x89, 0x6b, 0x35
};

static const BYTE CRC8_Table2[256] = 
{
	0x00, 0x07, 0x0e, 0x09, 0x1c, 0x1b, 0x12, 0x15,
	0x38, 0x3f, 0x36, 0x31, 0x24, 0x23, 0x2a, 0x2d,
	0x70, 0x77, 0x7E, 0x79, 0x6C, 0x6B, 0x62, 0x65,
	0x48, 0x4F, 0x46, 0x41, 0x54, 0x53, 0x5A, 0x5D,
	0xE0, 0xE7, 0xEE, 0xE9, 0xFC, 0xFB, 0xF2, 0xF5,
	0xD8, 0xDF, 0xD6, 0xD1, 0xC4, 0xC3, 0xCA, 0xCD,
	0x90, 0x97, 0x9E, 0x99, 0x8C, 0x8B, 0x82, 0x85,
	0xA8, 0xAF, 0xA6, 0xA1, 0xB4, 0xB3, 0xBA, 0xBD,
	0xC7, 0xC0, 0xC9, 0xCE, 0xDB, 0xDC, 0xD5, 0xD2,
	0xFF, 0xF8, 0xF1, 0xF6, 0xE3, 0xE4, 0xED, 0xEA,
	0xB7, 0xB0, 0xB9, 0xBE, 0xAB, 0xAC, 0xA5, 0xA2,
	0x8F, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9D, 0x9A,
	0x27, 0x20, 0x29, 0x2E, 0x3B, 0x3C, 0x35, 0x32,
	0x1F, 0x18, 0x11, 0x16, 0x03, 0x04, 0x0D, 0x0A,
	0x57, 0x50, 0x59, 0x5E, 0x4B, 0x4C, 0x45, 0x42,
	0x6F, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7D, 0x7A,
	0x89, 0x8E, 0x87, 0x80, 0x95, 0x92, 0x9B, 0x9C,
	0xB1, 0xB6, 0xBF, 0xB8, 0xAD, 0xAA, 0xA3, 0xA4,
	0xF9, 0xFE, 0xF7, 0xF0, 0xE5, 0xE2, 0xEB, 0xEC,
	0xC1, 0xC6, 0xCF, 0xC8, 0xDD, 0xDA, 0xD3, 0xD4,
	0x69, 0x6E, 0x67, 0x60, 0x75, 0x72, 0x7B, 0x7C,
	0x51, 0x56, 0x5F, 0x58, 0x4D, 0x4A, 0x43, 0x44,
	0x19, 0x1E, 0x17, 0x10, 0x05, 0x02, 0x0B, 0x0C,
	0x21, 0x26, 0x2F, 0x28, 0x3D, 0x3A, 0x33, 0x34,
	0x4E, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5C, 0x5B,
	0x76, 0x71, 0x78, 0x7F, 0x6A, 0x6D, 0x64, 0x63,
	0x3E, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2C, 0x2B,
	0x06, 0x01, 0x08, 0x0F, 0x1A, 0x1D, 0x14, 0x13,
	0xAE, 0xA9, 0xA0, 0xA7, 0xB2, 0xB5, 0xBC, 0xBB,
	0x96, 0x91, 0x98, 0x9F, 0x8A, 0x8D, 0x84, 0x83,
	0xDE, 0xD9, 0xD0, 0xD7, 0xC2, 0xC5, 0xCC, 0xCB,
	0xE6, 0xE1, 0xE8, 0xEF, 0xFA, 0xFD, 0xF4, 0xF3
};

static const UINT CRC16_Table1[256]= 
{
	0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
	0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
	0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
	0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
	0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
	0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
	0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
	0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
	0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
	0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
	0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
	0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
	0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
	0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
	0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
	0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
	0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
	0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
	0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
	0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
	0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
	0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
	0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
	0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
	0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
	0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
	0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
	0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
	0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
	0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
	0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
	0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0
};

BYTE CRC8_Packet1(BYTE* packet, int size)
{
	BYTE CRC = 0x00;

	if( !packet || size <= 0 ) return CRC;

	while( size-- )
	{
		CRC = CRC8_Table1[CRC ^ *packet++];
	}

	return CRC;
}

BYTE CRC8_Packet2(BYTE* packet, int size)
{
	BYTE CRC = 0x00;

	if( !packet || size <= 0 ) return CRC;

	while( size-- )
	{
		CRC = CRC8_Table2[CRC ^ *packet++];
	}

	return CRC;
}

WORD CRC16_Packet1(BYTE *packet, int size)
{
	USHORT CRC = 0x0000;

	if( !packet || size <= 0 ) return CRC;
	
	while( size-- )
	{
		CRC = (CRC<<8) ^ CRC16_Table1[((CRC>>8) ^ *packet++)&0x00FF];
	}

	return CRC;
}

// MODBUS RTU
WORD CRC16_Packet2(BYTE *packet, int size)
{
	USHORT CRC=0xFFFF;
	
	while( size-- )
	{
		CRC ^= ((*packet++) & 0x00FF);
		
		for( int i=0; i<8; i++ )
		{
			if( CRC & 0x01 )
			{
				CRC >>= 1;
				CRC ^= 0xA001;
			}
			else
			{
				CRC >>= 1;
			}
		}
	}
	
	return CRC;
}


// for SanKyo Motor, 2015-11-05, jhLee
WORD CRC16_CCITT(BYTE *packet, int size )
{
	unsigned short tmp;
	unsigned short crc = 0xffff;

	for (int i = 0; i < size; i++)
	{
		tmp = (crc >> 8) ^ packet[i];
		crc = (crc << 8) ^ CRC16_Table1[tmp];
	}

	return crc;
}


// 
BYTE BCC_Packet(BYTE* packet, int size)
{
	BYTE BCC = 0x00;

	if( !packet || size <= 0 ) return BCC;

	while( size-- )
	{
		BCC ^= *packet++;
	}

	return BCC;
}

//=============================================================================
// COMMUNICATION QUEUE
//=============================================================================

CGxCommQueue::CGxCommQueue()
{
	m_nMode   = eGxCOMM_PKT_NONE;		// 특정 패킷 구분 방법을 지정하지 않고 수신되는 대로 패킷으로 인정한다.
	m_nIndex  = 0;
	m_nLen    = GxCOMM_LEN_SIZE;		// 기본적인 패킷 길이 정보 크기
}

CGxCommQueue::~CGxCommQueue()
{
}


// 지정한 길이만큼 큐에 삽입한다.
UINT CGxCommQueue::PutData(BYTE* pbyData, int nSize)
{
	for( int i = 0; i < nSize; i++ )
	{
		// 크기를 넘어서거나 삽입에 실패하면 삽입된 크기만큼만 인정
		if ( IsFull() ) return i;						
		if ( !Push(*(pbyData + i)) ) return i;
	}

	return nSize;
}


// 지정한 길이만큼 선두에서 데이터를 인출하여 회신한다.
// 원본 버퍼에서는 삭제된다.
UINT CGxCommQueue::GetData(BYTE* pbyData, int nSize)
{
	for( int i = 0; i < nSize; i++ )
	{
		if ( IsEmpty() ) return i;			// 인출한 크기만큼

		*(pbyData + i) = Pop();				// 한 Byte씩 꺼내어 포인터에 복사한다.
	}

	return nSize;
}

// 버퍼 초기화
void CGxCommQueue::Clear()
{
	RemoveAll();
}

// 비어있는가 ?
BOOL CGxCommQueue::IsEmpty()
{
	return CList<BYTE,BYTE>::IsEmpty();
}

// 큐가 지정 크기에 이르렀는가 ?
BOOL CGxCommQueue::IsFull()
{
	return ( GetCount() >= GxCOMM_MAX_QUEUE );
}

// 패킷 구분자 선정 모드 조회
UINT CGxCommQueue::GetPacketMode(int& nIdx, int& nLen)
{
	nIdx = m_nIndex;						// 패킷 구분자 시작 위치
	nLen = m_nLen;							// 패킷 구분자 길이

	return m_nMode;						// 패킷 구분 모드 반환
}

//
// 온전한 패킷이 수신되었는가 ?
//
// return = 0 (Not Found)
//			그 외는 패킷의 끝 위치 
UINT CGxCommQueue::IsPacket()
{
	int i;
	UINT nPacketSize = 0;

	// 지정한 길이의 데이터가 수신되면 패킷이 수신되었다고 인정
	if( m_nMode == eGxCOMM_PKT_COUNT )
	{
		if ( m_nLen <= (UINT)GetCount() )					// 수신된 데이터 길이가 지정 길이 이상인가 ?
		{
			nPacketSize = m_nLen;					// 길이 만큼의 패킷이 수신되었다.
		}

		// 수신 데이터 길이가 지정 길이 미만이라면 아직 패킷이 수신되지 않았다. nPacketSize = 0; 값 유지
	}
	// 특정 토큰 문자로 패킷을 나눈다.
	else if( m_nMode == eGxCOMM_PKT_TOKEN )
	{
		POSITION pos = GetHeadPosition();
		int  nIndex  = 0; 
		BOOL bFind   = FALSE;
		int  nSize   = m_arrToken.GetSize();		// 지정된 토큰의 수량
		BYTE byData  = 0x00;

		// 끝까지 검색 대상으로 한다. 더이상의 데이터가 없거나 토큰을 찾게되면 반복 종료
		for( ; pos!=NULL && !bFind; nIndex++ )		
		{
			byData = GetNext( pos );				// 한 Byte를 취득한다.
			for( i = 0; i < nSize; i++ )			// 토큰의 수량만큼 반복
			{
				if( m_arrToken[i] == byData )		// 지정한 토큰과 일치하는 값이 있는가 ?
				{
					bFind = TRUE;					// 패킷 구분 토큰을 찾았다.
					break;							// 내부 for i 를 중지
				}
			}
		}

		// 패킷 구분 토큰을 찾았다. 오류감지 코드를 포함한 모든 데이터를 수신받았는가 ?
		if( bFind )
		{
			// 오류감지 CRC/BCC 가 Token 다음에 있으면, 양수
			if( m_nIndex > 0 )
			{
				int nCount = GetCount();				// 현재까지 수신된 데이터의 길이
				if( nCount < (int)(nIndex + m_nLen) )	// 오류감지 코드까지 수신되지 않았다.
				{
					bFind = FALSE;						// 온전한 패킷이 아직 수신되지 않았다.
				}
				else
					nIndex += m_nLen;					// 패킷의 끝 위치 = 토큰위치 + 오류감지코드 길이
			}
			// CRC 가 Token 앞에 있으면 , (m_nIndex 값이 음수일 경우)
			else if( m_nIndex < 0 )						// 일단, 패킷은 모두 수신되었다.
			{
				// 남아있는 데이터 중에 실제 토큰이 존재할 가능성 점검
				// 이유 : 오류 감지코드 CRC / BCC는 0x00 ~ 0xFF의 값을 가질 수 있다. 즉 토큰과 동일한 값이 나올수 있는데,
				//  이러한 오류감지코드를 패킷 구분자로 인식할 수도 있으므로 점검 필요
				int nLoop = min((int)(GetCount() - nIndex), (int)m_nLen);		// 남아있는 데이터 잔량과 오류코드 길이 중 작은값
				int nIndexTemp = nIndex;					// 현재 찾은 토큰의 위치

				for(  i = 0; (pos != NULL) && (i < nLoop); i++ )	// 검색할 길이만큼 반복
				{
					byData = GetNext( pos );				// 수신된 데이터를 1 Byte 읽는다.

					// 토큰에 해당되는 값인가 ?
					for( int j = 0; j < nSize; j++ )
					{
						if( m_arrToken[j] == byData )		// 토큰 문자이다.
						{
							nIndexTemp = nIndex + i + 1;	// 실제 토큰으로 인정한다. i값은 0 부터 시작하기에 + 1을 해준다.
							break;
						}
					}
				}
				nIndex = nIndexTemp;						// 실제 토큰 위치
			}
		}

		// 토큰을 찾았는가 ?
		if( bFind )
		{
			nPacketSize = nIndex;							// 패킷의 크기
		}
		else
			nPacketSize = 0;								// 못찾았다면 패킷의 크기는 0이다.
	}
	// 길이 정보가 ASCII (문자열) 10진수인 크기에 의한 가변길이로 패킷 구분
	else if( m_nMode == eGxCOMM_PKT_SIZE_AD )
	{
		int nCount = GetCount();							// 수신된 길이
		if( m_nIndex + m_nLen > (UINT)nCount ) return 0;	// 길이정보가 아직 수신되지 않았다.

		POSITION pos = NULL;
		BYTE* bySize = NULL;
		bySize = new BYTE[m_nLen + 1];						// 길이 정보

		for( i = 0; i < (int)m_nLen; i++ )
		{
			pos = FindIndex(m_nIndex + i);				// 지정 Index의 위치를 검색
			bySize[i] = GetAt( pos );						// 데이터를 취득
		}
		bySize[m_nLen] = '\0';								// 문자열 마무리

		int nSize = atoi((const char*)bySize);			// 정수형으로 변환
		delete[] bySize;

		// 수신된 길이가 지정 길이 이상인가 ?
		if( nCount >= nSize )								// 변환된 길이 정보 사용
		{
			nPacketSize = nSize;								// 지정된 길이 만큼 수신되었다.
		}
		else
			nPacketSize = 0;									// 아직 미수신
	}
	// 길이 정보가 ASCII (문자열) 16진수인 크기에 의한 가변길이로 패킷 구분
	else if( m_nMode == eGxCOMM_PKT_SIZE_AH )
	{
		int nCount = GetCount();							// 수신된 길이
		if( m_nIndex + m_nLen > (UINT)nCount ) return 0;	// 길이정보가 아직 수신되지 않았다.

		POSITION pos = NULL;
		BYTE* bySize = NULL;
		char* pEnd;
		bySize = new BYTE[m_nLen + 3];						// 길이 정보

		// 16진수 문자열로 만들기 위한 선두 2 글자
		bySize[0] = '0';
		bySize[1] = 'x';

		for( i = 0; i < (int)m_nLen; i++ )
		{
			pos = FindIndex(m_nIndex + i);					// 지정 Index의 위치를 검색
			bySize[i+2] = GetAt( pos );						// 데이터를 취득
		}
		bySize[m_nLen+2] = '\0';								// 문자열 마무리

		int nSize = strtol((const char*)bySize, &pEnd, 16);	// 16진수 정수형으로 변환
		delete[] bySize;

		// 수신된 길이가 지정 길이 이상인가 ?
		if( nCount >= nSize )								// 변환된 길이 정보 사용
		{
			nPacketSize = nSize;								// 지정된 길이 만큼 수신되었다.
		}
		else
			nPacketSize = 0;									// 아직 미수신
	}
	// 길이 정보가 Binary 값인 크기에 의한 가변길이로 패킷 구분
	else if( m_nMode == eGxCOMM_PKT_SIZE_B )
	{
		int nCount = GetCount();
		if( m_nIndex + m_nLen > (UINT)nCount ) return 0;	// 길이 정보가 아직 수신되지 않았다.

		POSITION pos = NULL;
		BYTE byData = NULL;									// 임시
		DWORD dwSize = 0;										// 길이 크기

		for( i = 0; i < (int)m_nLen; i++ )
		{
			pos = FindIndex(m_nIndex + i);				// 지정 Index의 위치를 검색
			byData = GetAt( pos );							// 데이터를 취득

			// Binary 값으로 지정된 길이 데이터를 수치형으로 변환한다.
			dwSize = dwSize + ( byData << (8 * (m_nLen - (i + 1))) );		// 8 * 3, 8 * 2, 8 * 1, 8 * 0  순서로 적용된다.
		}

		// 지정한 크기 이상으로 수신되었는가 ?
		if( (DWORD)nCount >= dwSize ) 
		{
			nPacketSize = (UINT)dwSize;					// 지정된 크기를 되돌린다.
		}
		else
			nPacketSize = 0;									// 아직 미수신
	}
	// eGxCOMM_PKT_NONE 혹은 잘못된 지정값인 경우
	else	// 그 외에는 읽어들인 데이터 길이 그대로를 사용한다.
	{
		nPacketSize = GetCount();
	}

	return nPacketSize;
}


// 패킷을 자동으로 검색하여 데이터를 취득한다.
//
// return = 0 (Not Found)
//   그외 : 취득한 데이터 길이
UINT CGxCommQueue::GetPacket(BYTE* pbyData)
{
	// 패킷이 온전히 수신되었다면 패킷 크기만큼 데이터를 인출하여 되돌려준다.
	UINT nPacketSize = IsPacket();
	if( nPacketSize <= 0 ) return 0;		// 아직 온전히 수신되지 않았다.

	UINT nGetSize = GetData(pbyData, nPacketSize);

	return nGetSize;							// 읽어들인 데이터 수량을 리턴
}


// 지정길이로 데이터를 취득한다.
//
// return = 0 : 지정한 길이보다 적게 수신이 되었다.
//     그외 : 취득한 데이터 길이
UINT CGxCommQueue::GetPacket(BYTE* pbyData, UINT nLen)
{
	// 현재 수신된 데이터 길이가 지정한 길이보다 작다면 수행 포기
	UINT nPacketSize = GetCount();
	if( nPacketSize < nLen ) return 0;
	
	// 지정한 길이보다 같거나 더 많은 데이터가 수신되었다면 지정한 길이만큼 데이터를 인출하여 되돌려준다.
	UINT nGetSize = GetData(pbyData, nLen);		// nPacketSize);
	
	return nGetSize;							// 읽어들인 데이터 수량을 리턴
}


// 패킷의 구분을 정하는 기준을 설정한다.
//
// UINT m_nMode : 패킷을 구분하는 방법
//	eGxCOMM_PKT_NONE	= 0,		// 수신된 데이터 그대로를 패킷으로 인정한다.
//	eGxCOMM_PKT_COUNT,				// 지정한 데이터 수신 수량으로 패킷의 기준을 삼는다.
//	eGxCOMM_PKT_TOKEN,				// 지정한 데이터를 패킷의 기준으로 삼는다.
//	eGxCOMM_PKT_SIZE_A,				// 가변길이, 선두 길이 정보가 ASCII 형식인 패킷   '0010' -> 길이 10 Bytes
//	eGxCOMM_PKT_SIZE_B,				// 가변길이, 선두 길이 정보가 Binary 데이터인 패킷 0x0000000a -> 길이 10 Bytes
//
// CByteArray* m_arrToken : eGxCOMM_PKT_TOKEN 모든인 경우 토큰 문자들 배열 포인터
//
// int m_nIndex  = 기준 위치
//			eGxCOMM_PKT_TOKEN : BCC/CRC가 토큰 문자 기준으로 어느 위치에 존재하는가 ?
//								0 : BCC/CRC가 존재하지 않는다.								
//								음수 : 토큰 문자 이전에 값이 온다.
//								양수 : 토큰 문자 이후에 값이 온다.
//			eGxCOMM_PKT_SIZE_A, eGxCOMM_PKT_SIZE_A : 길이 정보의 시작 위치
//
// UINT m_nLen  = 정보 길이
//			eGxCOMM_PKT_COUNT : 패킷으로 인정할 데이터의 길이
//			eGxCOMM_PKT_TOKEN : BCC/CRC 의 길이
//			eGxCOMM_PKT_SIZE_AD / AH, eGxCOMM_PKT_SIZE_A : 길이 정보의 크기
//
void CGxCommQueue::SetPacketInfo(UINT nMode, CByteArray* parrToken, int nIndex, UINT nLen)
{
	if( nMode == eGxCOMM_PKT_TOKEN )			// 특정문자로 패킷을 구분한다.
	{
		// 토큰 문자가 지정된 경우에만 유효하다
		if ( parrToken && (parrToken->GetSize() > 0) )
		{
			m_nMode  = nMode;
			m_nIndex = nIndex;				// Token 을 기준으로 BCC/CRC 의 위치 (앞:음수, 뒤:양수)
			m_nLen   = nLen;					// BCC/CRC Size

			m_arrToken.Copy(*parrToken);
		}
	}
	else if( (nMode == eGxCOMM_PKT_SIZE_AD) || (nMode == eGxCOMM_PKT_SIZE_AH) || (nMode == eGxCOMM_PKT_SIZE_B) )		// 가변길이로 패킷을 구분
	{
		// 크기 정보는 첫글자 부터 시작할 수 있고, 지정 길이는 1 ~ 4 사이 값을 가진다.
		if ( ( nIndex >= 0 ) && (nLen > 0) && (nLen <= GxCOMM_LEN_SIZE) )			// 지정 길이 이내이어야 사용가능
		{
			m_nMode  = nMode;
			m_nIndex = nIndex; // Size 정보가 들어있는 Block 의 위치
			m_nLen   = nLen;   // Size 정보가 들어있는 Block 의 크기 
		}
	}
	else if( nMode == eGxCOMM_PKT_COUNT )		// 크기 지정으로 패킷을 구분, m_nIndex는 미사용
	{
		if ( nLen > 0 )
		{
			m_nMode	= nMode;
			m_nLen	= nLen;
		}
	}
	else
	{
		//! ASSERT( FALSE );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// eGxCOMM_PKT_SIZE_A, eGxCOMM_PKT_SIZE_B의 적용을 위한 Utility 함수

// 지정 크기를 해당 Byte 배열에 ASCII 10진수 형태로 저장한다.
void CGxCommQueue::SetAsciiValueDec(BYTE* pArray, UINT uValue, int nLen)
{
	if ( !pArray ) return;				// 포인터에 문제가 있다면 수행 불가

	// 수치형 -> ASCII
	char szFmt[10];
	char szValue[20];

	// 수치형 -> 문자열로 변환시킨다.
	sprintf_s(szFmt, sizeof(szFmt), "%%0%uu", nLen);
	sprintf_s(szValue, sizeof(szValue),  szFmt, uValue);

	for (int i = 0; i < nLen; i++)
	{
		pArray[i] = szValue[i];
	}

}


// 지정 크기를 해당 Byte 배열에 ASCII 16진수 형태로 저장한다.
void CGxCommQueue::SetAsciiValueHex(BYTE* pArray, UINT uValue, int nLen)
{
	if ( !pArray ) return;				// 포인터에 문제가 있다면 수행 불가

	// 수치형 -> ASCII
	char szFmt[10];
	char szValue[20];

	// 수치형 -> 문자열로 변환시킨다.
	sprintf_s(szFmt, sizeof(szFmt), "%%0%uX", nLen);
	sprintf_s(szValue, sizeof(szValue),  szFmt, uValue);

	for (int i = 0; i < nLen; i++)
	{
		pArray[i] = szValue[i];
	}

}


// 지정 크기를 해당 Byte 배열에 Binary 형태로 저장한다.
void CGxCommQueue::SetBinaryValue(BYTE* pArray, UINT uSize, int nLen)
{
	if ( !pArray ) return;				// 포인터에 문제가 있다면 수행 불가

	DWORD dwValue = (DWORD)uSize;		// 가장 큰 DWORD 형태로 전환한다.

	for (int i = 0; i < nLen; i++)
	{
		pArray[i] = (dwValue >> (8 * (nLen - (i + 1))) & 0xff);
	}
}

// 지정 Byte 배열에서 Ascii 10진수 형태의 수치형 값을 얻는다.
UINT CGxCommQueue::GetAsciiValueDec(BYTE* pArray, int nLen)
{
	if ( !pArray ) return 0;				// 포인터에 문제가 있다면 수행 불가

	BYTE* byBuff = new BYTE[nLen + 1];					// 길이 정보

	for (int i = 0; i < nLen; i++ )
	{
		byBuff[i] = pArray[i];								// 데이터를 취득
	}
	byBuff[nLen] = '\0';										// 문자열 마무리

	UINT uValue = atoi((const char*)byBuff);			// 정수형으로 변환
	delete[] byBuff;

	return uValue;
}

// 지정 Byte 배열에서 Ascii 16진수 형태의 수치형 값을 얻는다.
UINT CGxCommQueue::GetAsciiValueHex(BYTE* pArray, int nLen)
{
	if ( !pArray ) return 0;				// 포인터에 문제가 있다면 수행 불가

	BYTE* byBuff = new BYTE[nLen + 3];					// 길이 정보
	char* pEnd;

	// 16진수 임을 표시
	byBuff[0] = '0';
	byBuff[1] = 'x';

	for (int i = 0; i < nLen; i++ )
	{
		byBuff[i+2] = pArray[i];								// 데이터를 취득
	}
	byBuff[nLen+2] = '\0';										// 문자열 마무리

	UINT uValue = strtol((const char*)byBuff, &pEnd, 16);			// 16진수 정수형으로 변환
	delete[] byBuff;

	return uValue;
}

// 지정 Byte 배열에서 Binay 형태의 수치형 값을 얻는다.
UINT CGxCommQueue::GetBinaryValue(BYTE* pArray, int nLen)
{
	if ( !pArray ) return 0;				// 포인터에 문제가 있다면 수행 불가

	DWORD dwValue = 0;

	for (int i = 0; i < nLen; i++)
	{
		// 1 Byte씩 값으로 변형시킨다.
		dwValue = dwValue + ( pArray[i] << (8 * (nLen - (i + 1))) );
	}

	return (UINT)dwValue;
}



