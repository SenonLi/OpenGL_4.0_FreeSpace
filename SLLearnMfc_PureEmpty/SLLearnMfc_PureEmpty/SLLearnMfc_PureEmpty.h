// SLLearnMfc_PureEmpty.h : main header file for the SLLearnMfc_PureEmpty DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CSLLearnMfcPureEmptyApp
// See SLLearnMfc_PureEmpty.cpp for the implementation of this class
//

class CSLLearnMfcPureEmptyApp : public CWinApp
{
public:
	CSLLearnMfcPureEmptyApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
