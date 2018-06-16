#pragma once

class CMelsecAddress
{
public:
	CMelsecAddress(void);
	~CMelsecAddress(void);

	//////////////////////////////////////////////////////////////////////////
	// �⺻ ������ �ٸ� Address�� ����ϴ� ��� ���� Load/Save �Լ�
	// ���� �� Class(�� : CAdrCustom)�� �����ϰ� �Ʒ� �� �Լ��� ������ �ؼ� ����.
	virtual void LoadAddress() { return; }
	virtual void SaveAddress() { return; }

	WORD LoadInputStart;
	WORD UnloadInputStart;
	WORD DataInputStart;

	WORD LoadOutputStart;
	WORD UnloadOutputStart;
	WORD DataOutputStart;

	WORD PanelorChk;

	WORD NonStopInputWordStart;
	WORD NonStopOutputWordStart;	// 2009.05.31 Nonstop Word ����.

	WORD TiltInputWordStart;		// 2009.06.07 Tilting Word ����.
	WORD TiltOutputWordStart;		// 2009.06.07 Tilting Word ����.

	WORD DryRunPanelExist;			// 2009.06.09 ����� ���� ���� �߰�.

};

class CAdrAMMT1 : public CMelsecAddress
{
public:
	CAdrAMMT1(void);
	~CAdrAMMT1(void);
};
class CAdrAMMT2 : public CMelsecAddress
{
public:
	CAdrAMMT2(void);
	~CAdrAMMT2(void);
};
class CAdrBMMT1 : public CMelsecAddress
{
public:
	CAdrBMMT1(void);
	~CAdrBMMT1(void);
};
class CAdrBMMT2 : public CMelsecAddress
{
public:
	CAdrBMMT2(void);
	~CAdrBMMT2(void);
};
