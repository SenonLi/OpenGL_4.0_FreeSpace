#pragma once
#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

class CMfc3DPlayerApp : public CWinApp
{
public:
	CMfc3DPlayerApp();

public:
	virtual BOOL InitInstance() override;

// Implementation
	DECLARE_MESSAGE_MAP()
};

extern CMfc3DPlayerApp theApp;
