//
// GausGUI.h : GausGUI DLL�� �⺻ ��� �����Դϴ�. ��� GausGUI library header file�� ���� ��Ű�ʽÿ�.
//
// 2015-10-07, jhLee, DLL�� ��ȯ
//

#pragma once

//
// #define _DLL_EXPORTS				// DLL ����
// #define _ORIGIN_SOURCE			// ���� �ҽ��� ���� ������
//

#ifndef _GAUS_GUI

	#ifndef _ORIGIN_SOURCE

		#ifdef _DLL_EXPORTS
			#define _GAUS_GUI	__declspec(dllexport)
		#else
			#ifdef _DEBUG
				#pragma comment(lib, "GausGUI.LIB")		// Gaus Lib for Debug mode 
			#else
				#pragma comment(lib, "GausGUI.LIB")		// Gaus Lib for Release mode 
			#endif

			#define _GAUS_GUI	__declspec(dllimport)
		#endif //of #ifdef _DLL_EXPORTS

	#else //of #ifndef _ORIGIN_SOURCE
		#define _GAUS_GUI									// �ҽ� ��ü�� ����Ѵ�.
	#endif //of #ifndef _ORIGIN_SOURCE

#endif //of #ifndef _GAUS_LIB