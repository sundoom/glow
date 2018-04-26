
// WorldEditor.h : main header file for the WorldEditor application
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

//////////////////////////////////////////////////////////////////////////

#include "resource.h"       // main symbols

//////////////////////////////////////////////////////////////////////////

// MainApp:
// See MainApp.cpp for the implementation of this class

class MainApp : public CWinAppEx
{
public:
	MainApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern MainApp theApp;
