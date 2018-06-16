#pragma once

class CMelsecAddress
{
public:
	CMelsecAddress(void);
	~CMelsecAddress(void);

	//////////////////////////////////////////////////////////////////////////
	// 기본 컨셉과 다른 Address를 사용하는 장비를 위한 Load/Save 함수
	// 사용시 새 Class(예 : CAdrCustom)를 생성하고 아래 두 함수를 재정의 해서 쓴다.
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
	WORD NonStopOutputWordStart;	// 2009.05.31 Nonstop Word 영역.

	WORD TiltInputWordStart;		// 2009.06.07 Tilting Word 영역.
	WORD TiltOutputWordStart;		// 2009.06.07 Tilting Word 영역.

	WORD DryRunPanelExist;			// 2009.06.09 드라이 런을 위해 추가.

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
