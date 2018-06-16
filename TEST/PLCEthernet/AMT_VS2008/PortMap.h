#pragma once

class CPortMap
{
public:
	CPortMap(void);
	~CPortMap(void);

	//////////////////////////////////////////////////////////////////////////
	// Input
	BYTE iStartSwitch			; 
	BYTE iStopSwitch			; 
	BYTE iResetSwitch			; 
	BYTE iAreaSensor			;
	BYTE iEmoSwitch				; 	// NOTE : Suspense
	BYTE iPowerOnCheck				; 	// NOTE : Suspense
	BYTE iDoorSensorRCover1			; 

	BYTE iPanelCheckSensor1		; 
	BYTE iContactY1Up		; 
	BYTE iContactY2Up		;	// SESL
	BYTE iContactY3Up	; 
	BYTE iContactY4Up	;	// SESL
	BYTE iPaneCheckSensor2		; 
	BYTE iTopClampOff		; 
	BYTE iTopClampOn			; 
	BYTE iDDKNetReady			; 
	BYTE iDDKArmStretch			; 

	//////////////////////////////////////////////////////////////////////////
	// Output
	BYTE oStartLamp 			;
	BYTE oStopLamp 				;
	BYTE oResetLamp 			;
	BYTE oContactY1Y4AirBlower 			;
	BYTE oContactY2Y3AirBlower 			;
	BYTE oTowerLampRed 			;
	BYTE oTowerLampYellow 		;
	BYTE oTowerLampGreen 		;
	BYTE oLightCurtainOff				;	// SESL
	BYTE oBackLignt1Set 		;	// NOTE : Suspense
	BYTE oBackLignt2Set 		;	// NOTE : Suspense
	BYTE oBackLignt3Set 		;	// NOTE : Suspense
	BYTE oBackLignt4Set 		;	// NOTE : Suspense
	BYTE oBackLignt5Set 		;	// NOTE : Suspense
	BYTE oBackLignt6Set 		;	// NOTE : Suspense
	BYTE oBackLignt7Set 		;	// NOTE : Suspense
	BYTE oBackLignt8Set 		;	// NOTE : Suspense
								
	BYTE oContactY1Up 	;
	BYTE oContactY1Down 	;
	BYTE oContactY2Up 	;
	BYTE oContactY2Down 	;
	BYTE oContactY3Up 	;
	BYTE oContactY3Down 	;
	BYTE oContactY4Up 	;
	BYTE oContactY4Down 	;
	BYTE oClampCylOff 		;
	BYTE oClampCylOn 		;
	BYTE oBackLightOnOff 	;
	BYTE oBackLightStrobe	;
	BYTE oBackLightCS1 		;
	BYTE oBackLigntCS2		;
	BYTE oFrontLight		;
	BYTE oDDKNetReady 		;
	BYTE oDDKTiltOut 		;

	BYTE oOmitOnOff 		;		// SESL용 상부조명
};

class CPortMapDemo : public CPortMap
{
public:
	CPortMapDemo(void);
	~CPortMapDemo(void);
};

// SESL IO Map
class CPortMapSESL : public CPortMap
{
public:
	CPortMapSESL(void);
	~CPortMapSESL(void);
};
