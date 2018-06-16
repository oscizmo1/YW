#include "StdAfx.h"
#include "MelsecAddress.h"

CMelsecAddress::CMelsecAddress(void)
{
}

CMelsecAddress::~CMelsecAddress(void)
{
}

CAdrAMMT1::CAdrAMMT1(void)
{
	LoadInputStart		=	0x2E0	;
	UnloadInputStart	=	0x2C0	;
	DataInputStart		=	0xB00	;

	LoadOutputStart		=	0x1E0	;
	UnloadOutputStart	=	0x1C0	;
	DataOutputStart		=	0x800	;

	PanelorChk			=	0x1E9	;

	NonStopInputWordStart	=	0xB70	;
	NonStopOutputWordStart	=	0x87A	;

	TiltInputWordStart		=	0xC80;		
	TiltOutputWordStart		=	0xCA0;		

	DryRunPanelExist = 0x2FF	;
}

CAdrAMMT1::~CAdrAMMT1(void)
{
}

CAdrAMMT2::CAdrAMMT2(void)
{
	LoadInputStart		=	0x320	;
	UnloadInputStart	=	0x300	;
	DataInputStart		=	0xB80	;

	LoadOutputStart		=	0x220	;
	UnloadOutputStart	=	0x200	;
	DataOutputStart		=	0x900	;

	PanelorChk			=	0x229	;

	NonStopInputWordStart	=	0xBF0	;
	NonStopOutputWordStart	=	0x97A	;

	TiltInputWordStart		=	0xCC0;		
	TiltOutputWordStart		=	0xCE0;		

	DryRunPanelExist = 0x33F	;
}

CAdrAMMT2::~CAdrAMMT2(void)
{
}

CAdrBMMT1::CAdrBMMT1(void)
{
	LoadInputStart		=	0x260	;
	UnloadInputStart	=	0x240	;
	DataInputStart		=	0xA00	;

	LoadOutputStart		=	0x160	;
	UnloadOutputStart	=	0x140	;
	DataOutputStart		=	0x600	;

	PanelorChk			=	0x169	;

	NonStopInputWordStart	=	0xA70	;
	NonStopOutputWordStart	=	0x67A	;

	TiltInputWordStart		=	0xC00;		// Address 영역 확인 후 수정 요...(PLC->PC)
	TiltOutputWordStart		=	0xC20;		

	DryRunPanelExist = 0x27F	;
}

CAdrBMMT1::~CAdrBMMT1(void)
{
}

CAdrBMMT2::CAdrBMMT2(void)
{
	LoadInputStart		=	0x2A0	;
	UnloadInputStart	=	0x280	;
	DataInputStart		=	0xA80	;

	LoadOutputStart		=	0x1A0	;
	UnloadOutputStart	=	0x180	;
	DataOutputStart		=	0x700	;

	PanelorChk			=	0x1A9	;

	NonStopInputWordStart	=	0xAF0	;
	NonStopOutputWordStart	=	0x77A	;

	TiltInputWordStart		=	0xC40;		
	TiltOutputWordStart		=	0xC60;		

	DryRunPanelExist = 0x3BF	;
}

CAdrBMMT2::~CAdrBMMT2(void)
{
}
