
// SLLearnMfc_ViewOnly.h : main header file for the SLLearnMfc_ViewOnly application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CSLLearnMfcViewOnlyApp:
// See SLLearnMfc_ViewOnly.cpp for the implementation of this class
//

class CSLLearnMfcViewOnlyApp : public CWinApp
{
public:
	CSLLearnMfcViewOnlyApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CSLLearnMfcViewOnlyApp theApp;
