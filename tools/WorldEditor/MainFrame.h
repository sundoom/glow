
// MainFrm.h : interface of the MainFrame class
//
//////////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////////

#include "MainView.h"
#include "WorkspacePane.h"
#include "OutputPane.h"

//////////////////////////////////////////////////////////////////////////

class MainFrame : public CFrameWndEx
{	
public:
	MainFrame();
	virtual ~MainFrame();

protected: 
	DECLARE_DYNAMIC(MainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);	

	// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd *pOldWnd);	
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnViewWorkspacePane();
	afx_msg void OnUpdateViewWorkspacePane(CCmdUI *pCmdUI);
	DECLARE_MESSAGE_MAP()

public:
	MainView		mMainView;

protected:  
	// control bar embedded members
	CMFCMenuBar		mMenuBar;
	CMFCToolBar		mToolBar;
	CMFCStatusBar	mStatusBar;	
	WorkspacePane	mWorkspacePane;
};


