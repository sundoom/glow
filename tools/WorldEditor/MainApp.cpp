
// MainApp.cpp : Defines the class behaviors for the application.
//
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "afxwinappex.h"
#include "MainApp.h"
#include "MainFrame.h"

//////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////

// MainApp
BEGIN_MESSAGE_MAP(MainApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &MainApp::OnAppAbout)
END_MESSAGE_MAP()

// MainApp construction
MainApp::MainApp()
{

}

// The one and only MainApp object

MainApp theApp;

// MainApp initialization

BOOL MainApp::InitInstance()
{
	CWinAppEx::InitInstance();	

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	Unregister();

	InitContextMenuManager();
	InitKeyboardManager();
	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object
	MainFrame* pFrame = new MainFrame;
	if (!pFrame) return FALSE;
	m_pMainWnd = pFrame;
	// create and load the frame with its resources
	pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,NULL);
	// The one and only window has been initialized, so show and update it
	pFrame->ShowWindow(SW_SHOWMAXIMIZED);
	pFrame->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand	

	return TRUE;
}

int MainApp::ExitInstance()
{
	return CWinAppEx::ExitInstance();
}

// MainApp message handlers

//////////////////////////////////////////////////////////////////////////

// AboutDlg dialog used for App About

class AboutDlg : public CDialog
{
public:
	AboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

AboutDlg::AboutDlg() : CDialog(AboutDlg::IDD)
{
}

void AboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(AboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void MainApp::OnAppAbout()
{
	AboutDlg aboutDlg;
	aboutDlg.DoModal();
}

//////////////////////////////////////////////////////////////////////////
