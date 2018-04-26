
// WorkspacePane.cpp : implementation of the WorkspacePane class
//
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WorkspacePane.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(WorkspacePane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

WorkspacePane::WorkspacePane()
{

}

WorkspacePane::~WorkspacePane()
{

}

/////////////////////////////////////////////////////////////////////////////
// WorkspacePane message handlers

int WorkspacePane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1) return -1;

	// create tabs control:
	if(!mTabCtrl.Create (CMFCTabCtrl::STYLE_3D, CRect(0,0,250,500), this, 1)) return -1;
	mTabCtrl.SetImageList(IDB_WORKSPACE_PANE_TAB_ICON, 16, RGB (192, 192, 192));
/*
	// create the object panel	
	if (!mObjectPanel.Create( AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_BTNSHADOW), NULL), 
		NULL, AFX_WS_DEFAULT_VIEW, CRect(0,0,250,500), &mTabCtrl, AFX_IDW_PANE_FIRST, NULL)) return -1;

	// create the object panel	
	if (!mTerrainPanel.Create( AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_BTNSHADOW), NULL), 
		NULL, AFX_WS_DEFAULT_VIEW, CRect(0,0,250,500), &mTabCtrl, AFX_IDW_PANE_FIRST, NULL)) return -1;	

	// add the panel to the tabs
	CString strPanelName;
	strPanelName.LoadString(IDS_OBJECT_PANEL);
	mTabCtrl.AddTab(&mObjectPanel, strPanelName, 0 );
	strPanelName.LoadString(IDS_TERRAIN_PANEL);
	mTabCtrl.AddTab(&mTerrainPanel, strPanelName, 1 );
*/
	return 0;
}

//////////////////////////////////////////////////////////////////////////

void WorkspacePane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// tab control should cover a whole client area:
	mTabCtrl.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);
}

//////////////////////////////////////////////////////////////////////////

