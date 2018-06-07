#pragma once
class CRecipeOption
{
public:
	CRecipeOption(void);
	~CRecipeOption(void);

	void ReadFile(CString strPath);
	void SaveFile(CString strPath, CString strBackupPath);
};

