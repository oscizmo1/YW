// Ini.cpp: implementation of the CGxIniFile class.
// Author: Bjarke Viks?
//
// Description:
// Thin wrapper around the Win32 Windows Profile (Ini-file configuration)
// interface.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GxIniFile.h"


// CGxIniFile

CGxIniFile::CGxIniFile()
{
	m_IniFilename.Empty();
}

CGxIniFile::CGxIniFile(LPCTSTR IniFilename)
{
	SetIniFilename( IniFilename );
}

CGxIniFile::~CGxIniFile()
{
	// Flush .ini file
	// (This should perhaps not be here. We risk to slow
	//  down the system and this would be done at a more appropriate
	//  time by the OS scheduler anyway)
	::WritePrivateProfileString( NULL, NULL, NULL, m_IniFilename );
}


//////////////////////////////////////////////////////////////////////
// Methods
//////////////////////////////////////////////////////////////////////

#define MAX_INI_BUFFER 300   // Defines the maximum number of chars we can
// read from the ini file 

int CGxIniFile::SetIniFilename(LPCTSTR IniFilename)
{
	// ASSERT(AfxIsValidString(IniFilename));

	m_IniFilename = IniFilename;
	if( m_IniFilename.IsEmpty() ) return RET_INVALIDARGS;
	return RET_OK;
};

// 지정된 파일이름 조회
CString CGxIniFile::GetIniFilename()
{
	return m_IniFilename;
}



// BOOL CGxIniFile::SetIniFilename(LPCTSTR IniFilename)
// {
// 	ASSERT(AfxIsValidString(IniFilename));
// 	m_IniFilename = IniFilename;
// 	if( m_IniFilename.IsEmpty() ) return FALSE;
// 	return TRUE;
// };

int CGxIniFile::GetInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault)
{
//	ASSERT(AfxIsValidString(lpszSection));
//	ASSERT(AfxIsValidString(lpszEntry));

	if( m_IniFilename.IsEmpty() ) return 0; // error

	CString sDefault;
	sDefault.Format( _T("%d"), nDefault );
	CString s = GetString( lpszSection, lpszEntry, sDefault );

	return _ttoi( s );
};


long CGxIniFile::GetLong( LPCTSTR lpszSection, LPCTSTR lpszEntry, long lDefault/*=0*/ )
{
//	ASSERT(AfxIsValidString(lpszSection));
//	ASSERT(AfxIsValidString(lpszEntry));

	if( m_IniFilename.IsEmpty() ) return 0; // error
	CString sDefault;
	sDefault.Format( _T("%ld"), lDefault );
	CString s = GetString( lpszSection, lpszEntry, sDefault );

	return _ttol( s );
};

double CGxIniFile::GetDouble(LPCTSTR lpszSection, LPCTSTR lpszEntry, double dDefault)
{
//	ASSERT(AfxIsValidString(lpszSection));
//	ASSERT(AfxIsValidString(lpszEntry));

	if( m_IniFilename.IsEmpty() ) return 0; // error
	CString sDefault;
	sDefault.Format( _T("%f"), dDefault );
	CString s = GetString( lpszSection, lpszEntry, sDefault );
	return _tstof( s );
};

CString CGxIniFile::GetString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault)
{
//	ASSERT(AfxIsValidString(lpszSection));
//	ASSERT(AfxIsValidString(lpszEntry));

	if( m_IniFilename.IsEmpty() ) return CString();
	CString s;

	long ret = ::GetPrivateProfileString( lpszSection, lpszEntry, lpszDefault, s.GetBuffer( MAX_INI_BUFFER ), MAX_INI_BUFFER, m_IniFilename );
	s.ReleaseBuffer();

	if( ret==0 ) return CString(lpszDefault);
	return s;
};

BOOL CGxIniFile::GetBoolean(LPCTSTR lpszSection, LPCTSTR lpszEntry, BOOL bDefault)
{
	CString s = GetString(lpszSection,lpszEntry);
	if( s.IsEmpty() ) return bDefault;
	TCHAR c = _totupper( s[0] );
	switch( c ) {
   case _T('Y'): // YES
   case _T('1'): // 1 (binary)
   case _T('O'): // O (O,X)
	   return TRUE;
   case _T('N'): // NO
   case _T('0'): // 0 (binary)
   case _T('X'): // X (O,X)
	   return FALSE;
   default:
	   return bDefault;
	};
};

//? 기능 구현이 되지 않았음.
BOOL CGxIniFile::GetBinary(LPCTSTR lpszSection, LPCTSTR lpszEntry, BYTE** ppData, UINT* pBytes)
{
//	ASSERT(AfxIsValidString(lpszSection));
//	ASSERT(AfxIsValidString(lpszEntry));

	return FALSE;
};


// 정수형 기록
BOOL CGxIniFile::WriteInt( LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue )
{
//	ASSERT(AfxIsValidString(lpszSection));
//	ASSERT(AfxIsValidString(lpszEntry));

	CString s;
	s.Format( _T("%d"), nValue );
	return WriteString( lpszSection, lpszEntry, s );
};


BOOL CGxIniFile::WriteLong( LPCTSTR lpszSection, LPCTSTR lpszEntry, long lValue )
{
//	ASSERT(AfxIsValidString(lpszSection));
//	ASSERT(AfxIsValidString(lpszEntry));

	CString s;
	s.Format( _T("%ld"), lValue );
	return WriteString( lpszSection, lpszEntry, s );
};

BOOL CGxIniFile::WriteDouble( LPCTSTR lpszSection, LPCTSTR lpszEntry, double dValue, UINT nPoint )
{
//	ASSERT(AfxIsValidString(lpszSection));
//	ASSERT(AfxIsValidString(lpszEntry));

	CString s, sFmt;
	sFmt.Format(_T("%%.%df"), nPoint);				// 소숫점 자릿수 지정
	s.Format( sFmt, dValue );				// 문자열로 만들어 줌
	return WriteString( lpszSection, lpszEntry, s );
};

BOOL CGxIniFile::WriteBoolean(LPCTSTR lpszSection, LPCTSTR lpszEntry, BOOL bValue)
{
	CString s;
	// bValue ? s=_T("Y") : s=_T("N");
	s = bValue ? _T("1") : _T("0");
	return WriteString( lpszSection, lpszEntry, s );
};

BOOL CGxIniFile::WriteString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue)
{
//	ASSERT(AfxIsValidString(lpszSection));
//	ASSERT(AfxIsValidString(lpszEntry));

	if( m_IniFilename.IsEmpty() ) return RET_NOTINITIALIZED;
	return ::WritePrivateProfileString( lpszSection, lpszEntry, lpszValue, m_IniFilename );
}


//? 기능 구현이 되지 않았음
BOOL CGxIniFile::WriteBinary(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPBYTE pData, UINT nBytes)
{
//	ASSERT(AfxIsValidString(lpszSection));
//	ASSERT(AfxIsValidString(lpszEntry));

	return FALSE;
}


BOOL CGxIniFile::WriteExpandString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue)
{
//	ASSERT(AfxIsValidString(lpszSection));
//	ASSERT(AfxIsValidString(lpszEntry));

	return FALSE;
};

BOOL CGxIniFile::DeleteKey(LPCTSTR lpszSection, LPCTSTR lpszEntry)
{
//	ASSERT(AfxIsValidString(lpszSection));
//	ASSERT(AfxIsValidString(lpszEntry));

	if( m_IniFilename.IsEmpty() ) return RET_NOTINITIALIZED;
	return ::WritePrivateProfileString( lpszSection, lpszEntry, NULL, m_IniFilename );
};

BOOL CGxIniFile::DeleteSection(LPCTSTR lpszSection)
{
//	ASSERT(AfxIsValidString(lpszSection));

	if( m_IniFilename.IsEmpty() ) return RET_NOTINITIALIZED;
	return ::WritePrivateProfileString( lpszSection, NULL, NULL, m_IniFilename );
};
