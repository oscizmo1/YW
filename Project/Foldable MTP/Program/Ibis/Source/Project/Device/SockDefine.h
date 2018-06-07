#pragma once
#define MCR_CAMERA_IP		 _T("192.168.10.150")
#define MCR_CAMERA_PORT			23

#define ACITVE_ALIGN_PC_IP		 _T("192.168.10.170")	// 160은 전력량계 영역
#define ACITVE_ALIGN_PC_PORT	9999

#define SOCKET_MAIN		_T("MAIN")		// Socket Index
//kjpark 20161006 MCR 상하 수정
#define SOCKET_UVMCR_UP		_T("UVMCR_UP")		// 1
#define SOCKET_UVMCR_DOWN   _T("UVMCR_DOWN")		// 1
//////////////////////////////////////////////////////////////////////////

enum
{
	TIMER_RE_CONNECT = 0,
	TIMER_MAX
};

struct SQUARE_POINT
{
	int		nThickness;	// 두께
	int		nSize;		// 크기
	COLORREF colorLine;	// 선 색상
	int		nX, nY;		// 중심위치

	SQUARE_POINT()
	{
		nThickness = 5;
		nSize = 100;
		colorLine = RGB(0,0,0);
		nX = nY = 0;
	}
};
