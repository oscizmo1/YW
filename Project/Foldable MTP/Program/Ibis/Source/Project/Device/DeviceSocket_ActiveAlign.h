#pragma once
#include "DeviceSocket_Base.h"

#define AVTIVE_ALIGN_ALIGN_REQ			_T("ALIGN_REQ")
#define AVTIVE_ALIGN_RECIPE_REQ			_T("RECIPE_REQ")
#define AVTIVE_ALIGN_TEACHING_SAVE		_T("ALIGN_TEACHING_SAVE")

#define AVTIVE_ALIGN_CURRENT_STATE		_T("CURRENT_STATE")
#define AVTIVE_ALIGN_ALIGN_RES			_T("ALIGN_RES")
#define AVTIVE_ALIGN_RECIPE_RES			_T("RECIPE_RES")
#define AVTIVE_ALIGN_TEACHING_RES		_T("ALIGN_TEACHING_RES")


#define SOCKET_ACTIVEALIGN_PC	_T("ACTIVEALIGN")
#define ALIGN_PC_IP				_T("192.168.10.130")
#define ALIGN_PC_ACTIVE_PORT	7777

class CDeviceSocket_ActiveAlign : 	public CDeviceSocket_Base
{
public:
	CDeviceSocket_ActiveAlign(void);
	~CDeviceSocket_ActiveAlign(void);

	void CreateSocket();
	void PreparseMessage(CDataSocket* pSocket = NULL);
	CString ParseActiveAlignMessage(CString strReadingText);
};