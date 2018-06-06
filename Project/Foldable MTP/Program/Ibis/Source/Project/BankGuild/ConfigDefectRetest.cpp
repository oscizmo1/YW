#include "StdAfx.h"
#include "ConfigDefectRetest.h"
#include "Etc/Ini.h"
#include "Etc/FileSupport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CConfigDefectRetest::CConfigDefectRetest(void)
{	
}


CConfigDefectRetest::~CConfigDefectRetest(void)
{
}

void CConfigDefectRetest::ReadDefectRetestFile(CString strPath)
{
	m_listRetest.RemoveAll();

	// 파일이 존재하는지 점검
	if(CFileSupport::IsFile(strPath) == FALSE) 	
	{	
		return;
	}

	CString strLine;						// 파일에서 읽어온 데이터	

	FILE *fStream;
	errno_t e = _tfopen_s(&fStream, strPath.GetString(), _T("r"));	// UNICODE"));
	if (e != 0) return ;

	CStdioFile		file(fStream);
	// 파일에서 1줄씩 읽어와서 처리한다.
	int nIndex = 0;
	while ( file.ReadString(strLine) )
	{
		if (strLine.IsEmpty() )	continue;			// 공백일 경우 무시
		if (strLine.GetLength() < 2 )	continue;	// 길이가 너무 작아도 무시
		if (strLine.Left(1) == _T("?"))	continue;	// 처음 글자가 " ? " 일경우 다음 문장으로 넘어간다.;
		if (strLine.Left(1) == _T("/"))	continue;	// 처음 글자가 " / " 일경우 다음 문장으로 넘어간다.;
		if (strLine.Left(1) == _T("!"))	continue;	// 처음 글자가 " ! " 일경우 다음 문장으로 넘어간다.;
		if (strLine.Left(1) == _T(";"))	continue;	// 처음 글자가 " ; " 일경우 다음 문장으로 넘어간다.;

		m_listRetest.AddTail(strLine);
	}

	file.Close();

}

BOOL CConfigDefectRetest::FindRetestList(CString strDefectName)
{
	CString strListText;
	POSITION pos = m_listRetest.GetHeadPosition();
	while(pos)
	{
		strListText = m_listRetest.GetNext(pos);
		if(strListText == strDefectName)
		{
			return TRUE;
		}
	}
	return FALSE;
}
