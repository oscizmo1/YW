#pragma once
class CRecipeCell
{
public:
	CRecipeCell(void);
	~CRecipeCell(void);
	BOOL m_bFPCBuse;
	double m_dWidth;
	double m_dHeight;

	void ReadFile(CString strPath);
	void SaveFile(CString strPath, CString strBackupPath);
};

