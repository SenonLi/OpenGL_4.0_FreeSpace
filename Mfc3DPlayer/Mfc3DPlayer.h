
// Mfc3DPlayer.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMfc3DPlayerApp:
// See Mfc3DPlayer.cpp for the implementation of this class
//

class CMfc3DPlayerApp : public CWinApp
{
public:
	CMfc3DPlayerApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMfc3DPlayerApp theApp;
