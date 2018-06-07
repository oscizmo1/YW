#pragma once
class CRecipeTeach
{
public:
	CRecipeTeach(void);
	~CRecipeTeach(void);

	double m_dShuttleY1_MCR;
	double m_dShuttleY2_MCR;

	double m_doldShuttleY1_MCR;
	double m_doldShuttleY2_MCR;

	void ReadFile(CString strPath);
	BOOL SaveFile(CString strPath, CString strBackupPath);
};

