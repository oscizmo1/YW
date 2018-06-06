// ADOClass.cpp: implementation of the CADOClass class.

#include "stdafx.h"
#include "ADOClass.h"
#include <ATLCONV.H>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CADOClass::CADOClass()
{
	m_pConnection = NULL;
	m_pCommand = NULL;
	m_pRs = NULL;

	::CoInitialize(NULL);

}

CADOClass::~CADOClass()
{
	m_pConnection = NULL;
	m_pCommand = NULL;
	m_pRs = NULL;
	::CoUninitialize();
}


bool CADOClass::Connect(LPCSTR lpszProvider, ExecuteOptionEnum emOption)
{

//	TRACE0( "ADOClass::Connect() entered\n" );

	if(m_pConnection != NULL && m_pConnection->GetState() == adStateOpen)
		return FALSE;


	try
	{
		HRESULT hr = m_pConnection.CreateInstance(__uuidof( Connection ));
		
		m_pConnection->ConnectionTimeout = 5;
		_bstr_t btProvider=(_bstr_t)lpszProvider;
		_bstr_t btEmpty=(_bstr_t)"";
		
		m_pConnection->Open(btProvider,btEmpty,btEmpty,emOption);
		
	}
	catch( _com_error &e)
	{
		DumpError(e);
		return FALSE;
	}
	
	return TRUE;
}

bool CADOClass::ExecuteQryCmd(LPCSTR lpszQry, long lop)
{

////	TRACE0( "ADOClass::ExecuteQryCmd entered\n" );
	
	if( m_pConnection == NULL || m_pConnection->GetState()!=adStateOpen)
		return FALSE;

	if( m_pRs != NULL && m_pRs->GetState()!=adStateClosed)
	{
		m_pRs->Close();
		m_pRs.Release();
		m_pRs = NULL;
	}

	try
	{
		m_pCommand.CreateInstance(__uuidof( Command )); 
		m_pCommand->ActiveConnection = m_pConnection; 
		m_pCommand->CommandType = adCmdStoredProc; 

		m_pCommand->CommandText = _bstr_t(lpszQry); 

		_variant_t vNull; 
		vNull.vt = VT_ERROR; 
		vNull.scode = DISP_E_PARAMNOTFOUND; 
		
		m_pRs = m_pCommand->Execute(&vNull, &vNull, lop); 

	}
	catch(_com_error &e)
	{

		DumpError(e);
		return FALSE;
	}

	return TRUE;


}

bool CADOClass::ExecuteQryRs(LPCSTR lpszQry, CursorTypeEnum emCurType, LockTypeEnum emLockType, long lop)
{

//	TRACE0( "ADOClass::ExecuteQry entered\n" );
	
	if( m_pConnection == NULL || m_pConnection->GetState()!=adStateOpen)
		return FALSE;
	
	if( m_pRs != NULL && m_pRs->GetState()!=adStateClosed)
	{
		m_pRs->Close();
		m_pRs.Release();
		m_pRs = NULL;
	}

	m_pRs.CreateInstance(__uuidof(Recordset));
	m_pRs->CursorLocation = adUseClient;

	try
	{
		_bstr_t btQry=(_bstr_t)lpszQry;
        m_pRs->Open(btQry, _variant_t((IDispatch *) m_pConnection), emCurType, emLockType, lop);

	}
	catch(_com_error &e)
	{

		DumpError(e);
		return FALSE;

	}

	return TRUE;

}

bool CADOClass::InUpDelQryCmd(LPCSTR lpszQry)
{
//	TRACE0( "ADOClass::InUpDelQryCmd entered\n" );
	
	if( m_pConnection == NULL || m_pConnection->GetState()!=adStateOpen)
		return FALSE;


	try
	{
		m_pCommand.CreateInstance(__uuidof( Command )); 
		m_pCommand->ActiveConnection = m_pConnection; 
		m_pCommand->CommandType = adCmdStoredProc; 

		m_pCommand->CommandText = _bstr_t(lpszQry); 

		_variant_t vNull; 
		vNull.vt = VT_ERROR; 
		vNull.scode = DISP_E_PARAMNOTFOUND; 
		
		m_pCommand->Execute(&vNull, &vNull, adCmdUnknown); 

	}
	catch(_com_error &e)
	{

		DumpError(e);
		return FALSE;

	}

	return TRUE;

}

bool CADOClass::InUpDelQryConn(LPCSTR lpszQry)
{
//	TRACE0( "ADOClass::InUpDelQry entered\n" );
	
	if(m_pConnection == NULL || m_pConnection->GetState()!=adStateOpen )
		return FALSE;


	try 
	{
		_bstr_t btQry=(_bstr_t)lpszQry;
		m_pConnection->Execute(btQry,NULL,adOptionUnspecified);
	}
	catch(_com_error &e)
	{
		DumpError(e);
		return FALSE;
	}
		
	return TRUE;
}

bool CADOClass::DisConnect()
{
//	TRACE0( "ADOClass::DisConnect entered\n" );

	try{

		if(m_pRs != NULL && m_pRs->GetState()!=adStateClosed)
		{
			m_pRs->Close();
			m_pRs.Release();
			m_pRs = NULL;
		}		
		
		
		if(m_pConnection != NULL && m_pConnection->GetState()!=adStateClosed )
		{
			m_pConnection->Close();
			m_pConnection.Release();
			m_pConnection = NULL;
		}

		if(m_pCommand != NULL && m_pCommand->GetState()!=adStateClosed )
		{
			m_pCommand.Release();
			m_pCommand = NULL;
		}


	}
	catch(_com_error &e)
	{
		DumpError(e);
		return FALSE;
	}

	return TRUE;

}

void CADOClass::DumpError(const _com_error &e) const
{
	_bstr_t btSource(e.Source());
	_bstr_t btDescription(e.Description());
	CString strMsg;
	strMsg.Format(	"DATE= %s\n"
					"%08lx= %s\n"
					"Source= %s\n"
					"Description= %s"
				,CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S")
				,e.Error(),e.ErrorMessage()
				,(LPCSTR)btSource
				,(LPCSTR)btDescription);


#ifdef _DEBUG
	// 로그에 띄워주기 위해...
	//AfxMessageBox(strMsg);
#endif
	CStdioFile sdFile;
	if( !sdFile.Open("C:\\EZnetManagerDump.txt", CFile::shareDenyNone | CFile::modeWrite | CFile::typeText) )
	{
		if( !sdFile.Open("C:\\EZnetManagerDump.txt", CFile::shareDenyNone | CFile::modeCreate | CFile::modeWrite | CFile::typeText) )
		{
//			TRACE("C:\\EZnetManagerDump.txt Create Fail\n");
			return;
		}
	}

	sdFile.SeekToEnd();
	sdFile.WriteString(strMsg);
	sdFile.WriteString("\n\n");
	sdFile.Close();


}

BOOL CADOClass::IsEOF()
{
	if (NULL == m_pRs)
	{
		return TRUE;
	}
	return (m_pRs->adoEOF);
}

BOOL CADOClass::CloseRS()
{

	try
	{
		if(m_pRs != NULL && m_pRs->GetState()!=adStateClosed)
		{
			m_pRs->Close();
			m_pRs.Release();
			m_pRs = NULL;
		}						
	}
	catch(_com_error &e)
	{
		DumpError(e);
		return FALSE;
	}


	return TRUE;
}

void CADOClass::FirstRS()
{

	if( m_pRs != NULL )
	{
		m_pRs->MoveFirst();
	}

}

void CADOClass::NextRS()
{
	if( m_pRs != NULL )
	{
		m_pRs->MoveNext();
	}

}

void CADOClass::PrevRS()
{
	if( m_pRs != NULL )
	{
		m_pRs->MovePrevious();
	}
}

void CADOClass::LastRS()
{
	if( m_pRs != NULL )
	{
		m_pRs->MoveLast();
	}

}

BOOL CADOClass::GetRsDataLong( LPCTSTR lpszColumn, LONG& lValue )
{
	_variant_t var;

	try
	{
		if( m_pRs != NULL)
		{
			var = m_pRs->GetFields()->GetItem(lpszColumn)->GetValue();

			if( var.vt == VT_NULL )
				lValue = 0L;
			else
				lValue = var.ulVal;
		}

	}
	catch(_com_error &e)
	{
		DumpError(e);
		return FALSE;
	}

	return TRUE;

}

BOOL CADOClass::GetRsDataBool(LPCTSTR lpszColumn, BOOL &bValue)
{
	_variant_t var;

	try
	{
		if( m_pRs != NULL)
		{
			var = m_pRs->GetFields()->GetItem(lpszColumn)->GetValue();

			if( var.vt == VT_NULL )
				bValue = -1;
			else if (var.boolVal == -1)
			{
				bValue = TRUE;
			}
			else
			{
				bValue = FALSE;
			}
		}

	}
	catch(_com_error &e)
	{
		DumpError(e);
		return FALSE;
	}

	return TRUE;
}

BOOL CADOClass::GetRsDataInt(LPCTSTR lpszColumn, INT &nValue)
{
	_variant_t var;

	try
	{
		if( m_pRs != NULL)
		{
			var = m_pRs->GetFields()->GetItem(lpszColumn)->GetValue();

			if( var.vt == VT_NULL )
				nValue = 0;
			else
				nValue = var.intVal;
		}

	}
	catch(_com_error &e)
	{
		DumpError(e);
		return FALSE;
	}

	return TRUE;
}

BOOL CADOClass::GetRsDataFloat(LPCTSTR lpszColumn, float& fValue)
{
	_variant_t var;

	try
	{
		if( m_pRs != NULL)
		{

			var = m_pRs->GetFields()->GetItem(lpszColumn)->GetValue();
			if( var.vt == VT_NULL )
			{
				fValue = 0;
			}
			else
			{
				fValue = var.fltVal;
			}

		}
	}
	catch(_com_error &e)
	{
		DumpError(e);
		return FALSE;
	}

	return TRUE;
}

BOOL CADOClass::GetRsDataChar(LPCTSTR lpszColumn, CString& csValue)
{

	_variant_t var;

	try
	{
		if( m_pRs != NULL)
		{

			var = m_pRs->GetFields()->GetItem(lpszColumn)->GetValue();
			if( var.vt == VT_NULL )
			{
				csValue = "";
			}
			else
			{
				csValue = var.bstrVal;
			}

		}
	}
	catch(_com_error &e)
	{
		DumpError(e);
		return FALSE;
	}

	return TRUE;

}

BOOL CADOClass::GetRsDataTime(LPCTSTR lpszColumn, CTime &cTime)
{

	_variant_t var;

	SYSTEMTIME	SystemTime ;
	DATE		date;


	try
	{
		if( m_pRs != NULL)
		{
			var = m_pRs->GetFields()->GetItem(lpszColumn)->GetValue();
			if( var.vt == VT_NULL )
			{
				cTime = 0;
			}
			else
			{
				date = var.date;				
				if(VariantTimeToSystemTime(date, &SystemTime) == TRUE)
				{
					//CTime이 표현할수 있는 범위를 넘어서면
					if ((1970 > SystemTime.wYear) | (3000 < SystemTime.wYear))
					{
						cTime = 0;
					}
					else
					{
						CTime cDTime(SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,
							SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond );

						cTime = (CTime)cDTime;
					}
				}
			}
		}

	}
	catch(_com_error &e)
	{
		DumpError(e) ;
		return FALSE;
	}

	return TRUE;
}

BOOL CADOClass::GetRsDataChar(LPCTSTR lpszColumn, char *pszValue)
{
	_variant_t var;

	USES_CONVERSION;	// BSTR -> char을 위한 매크로를 쓰기 위해서 선언.


	try
	{
		if( m_pRs != NULL)
		{

			var = m_pRs->GetFields()->GetItem(lpszColumn)->GetValue();
			if( var.vt == VT_NULL )
			{
				strcpy_s( pszValue, 1 ,_T(""));
			}
			else
			{				
				int nLen = ::WideCharToMultiByte( CP_ACP, 0, var.bstrVal, -1, NULL, 0, NULL, NULL ); // get size.
				::WideCharToMultiByte( CP_ACP, 0, var.bstrVal, -1, pszValue, nLen, NULL, NULL );			
			}
		}
	}

	catch(_com_error &e)
	{
		DumpError(e);
		return FALSE;
	}
	
	return TRUE;
}

int CADOClass::NextRecord(void)
{
	DWORD_PTR lngRec = 0;
	m_pRs = m_pRs->NextRecordset((VARIANT *)lngRec);
	return 0;
}

int CADOClass::setFilter(_bstr_t strField, _bstr_t strFilter)
{
	m_pRs->Filter  = strField + " = '" + strFilter + "'";
	
	if (m_pRs->GetRecordCount() == 0)
		return 1;

	return 0;
}

long CADOClass::GetRecordCount(void)
{
	return m_pRs->RecordCount;
}
