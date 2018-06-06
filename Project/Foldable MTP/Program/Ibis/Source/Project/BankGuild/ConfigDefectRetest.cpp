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

	// ������ �����ϴ��� ����
	if(CFileSupport::IsFile(strPath) == FALSE) 	
	{	
		return;
	}

	CString strLine;						// ���Ͽ��� �о�� ������	

	FILE *fStream;
	errno_t e = _tfopen_s(&fStream, strPath.GetString(), _T("r"));	// UNICODE"));
	if (e != 0) return ;

	CStdioFile		file(fStream);
	// ���Ͽ��� 1�پ� �о�ͼ� ó���Ѵ�.
	int nIndex = 0;
	while ( file.ReadString(strLine) )
	{
		if (strLine.IsEmpty() )	continue;			// ������ ��� ����
		if (strLine.GetLength() < 2 )	continue;	// ���̰� �ʹ� �۾Ƶ� ����
		if (strLine.Left(1) == _T("?"))	continue;	// ó�� ���ڰ� " ? " �ϰ�� ���� �������� �Ѿ��.;
		if (strLine.Left(1) == _T("/"))	continue;	// ó�� ���ڰ� " / " �ϰ�� ���� �������� �Ѿ��.;
		if (strLine.Left(1) == _T("!"))	continue;	// ó�� ���ڰ� " ! " �ϰ�� ���� �������� �Ѿ��.;
		if (strLine.Left(1) == _T(";"))	continue;	// ó�� ���ڰ� " ; " �ϰ�� ���� �������� �Ѿ��.;

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
