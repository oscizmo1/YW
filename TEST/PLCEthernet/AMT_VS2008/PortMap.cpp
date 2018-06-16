#include "StdAfx.h"
#include "PortMap.h"

CPortMap::CPortMap(void)
{
}

CPortMap::~CPortMap(void)
{
}

CPortMapDemo::CPortMapDemo( void )
{
	iStartSwitch			=	0  ;
	iStopSwitch				=	1  ;
	iResetSwitch			=	2  ;		
	iEmoSwitch				=	6  ;	// NOTE : Suspense
	iPowerOnCheck			=	7  ;	// NOTE : Suspense
	iDoorSensorRCover1		=	12 ;

	iPanelCheckSensor1		=	16 ;		
	iContactY1Up			=	17 ;		
	iContactY3Up			=	18 ;		
	iPaneCheckSensor2		=	21 ;		
	iTopClampOff			=	24 ;		
	iTopClampOn				=	25 ;		

	oStartLamp 				=	0  ;
	oStopLamp 				=	1  ;
	oResetLamp 				=	2  ;
	oContactY1Y4AirBlower 	=	3  ;
	oContactY2Y3AirBlower 	=	4  ;
	oTowerLampRed 			=	5  ;
	oTowerLampYellow 		=	6  ;
	oTowerLampGreen 		=	7  ;
	oBackLignt1Set 			=	8  ;	// NOTE : Suspense
	oBackLignt2Set 			=	9  ;	// NOTE : Suspense
	oBackLignt3Set 			=	10 ;	// NOTE : Suspense
	oBackLignt4Set 			=	11 ;	// NOTE : Suspense
	oBackLignt5Set 			=	12 ;	// NOTE : Suspense
	oBackLignt6Set 			=	13 ;	// NOTE : Suspense
	oBackLignt7Set 			=	14 ;	// NOTE : Suspense
	oBackLignt8Set 			=	15 ;	// NOTE : Suspense

	oContactY2Up 			=	16 ;
	oContactY2Down 			=	17 ;
	oContactY3Up 			=	19 ;
	oContactY3Down 			=	20 ;
	oClampCylOn 			=	22 ;
	oClampCylOff 			=	23 ;
	oBackLightOnOff 		=	26 ;
	oFrontLight 			=	30 ;
}									  

CPortMapDemo::~CPortMapDemo( void )
{

}

CPortMapSESL::CPortMapSESL( void )
{
	iStartSwitch			=	0x00  	;
	iStopSwitch				=	0x01  	;
	iResetSwitch			=	0x02  	;	
	iAreaSensor				=	0x04  	;	// B접
	iEmoSwitch				=	0x06  	;	
	iPowerOnCheck			=	0x07  	;	
	iDoorSensorRCover1		=	0x0C 	;

	iPanelCheckSensor1		=	0x10 	;		
	iContactY1Up			=	0x11 	;		
	iContactY2Up			=	0x12 	;		
	iContactY3Up			=	0x13 	;		
	iContactY4Up			=	0x14 	;		
	iPaneCheckSensor2		=	0x15 	;		
	iTopClampOff			=	0x18 	;		
	iTopClampOn				=	0x19 	;		
	iDDKNetReady			=	0x1E 	;	// 미사용		
	iDDKArmStretch			=	0x1F 	;	// B접		

	oStartLamp 				=	0x00  	;
	oStopLamp 				=	0x01  	;
	oResetLamp 				=	0x02  	;
	oContactY1Y4AirBlower 	=	0x03  	;
	oContactY2Y3AirBlower 	=	0x04  	;
	oTowerLampRed 			=	0x05  	;
	oTowerLampYellow 		=	0x06  	;
	oTowerLampGreen 		=	0x07  	;
	oLightCurtainOff		=	0x08	;

	oContactY1Up 			=	0x10 	;
	oContactY1Down 			=	0x11 	;
	oContactY2Up  			=	0x12 	;
	oContactY2Down  		=	0x13 	;
	oContactY3Up 			=	0x14 	;
	oContactY3Down			=	0x15 	;
	oClampCylOff 			=	0x16 	;
	oClampCylOn 			=	0x17 	;
	oContactY4Up 			=	0x18 	;
	oContactY4Down 			=	0x19 	;
	oOmitOnOff 				=	0x1A 	;
	//oBackLightStrobe		=	0x1B 	;
	//oBackLightCS1 		=	0x1C 	;
	//oBackLigntCS2	 		=	0x1D 	;
	oDDKNetReady 			=	0x1E 	;
	oDDKTiltOut 			=	0x1F 	;	// B접
}									  

CPortMapSESL::~CPortMapSESL( void )
{

}