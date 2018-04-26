
// MainFrm.cpp : implementation of the MainFrame class
//
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainApp.h"
#include "MainFrame.h"

//////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////

// MainFrame
IMPLEMENT_DYNAMIC(MainFrame, CFrameWndEx)
BEGIN_MESSAGE_MAP(MainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_VIEW_WORKSPACE_PANE, &MainFrame::OnViewWorkspacePane)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WORKSPACE_PANE, &MainFrame::OnUpdateViewWorkspacePane)
	ON_WM_DESTROY()
	ON_WM_SIZING()
END_MESSAGE_MAP()

// status line indicator
static UINT indicators[] =
{
	ID_SEPARATOR,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// MainFrame construction/destruction
MainFrame::MainFrame()
{

}

MainFrame::~MainFrame()
{
}

int MainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1) return -1;

	// set the visual manager used to draw all user interface elements
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);
	CMFCToolBarComboBoxButton::SetFlatMode();

	{
		// create the menu bar
		if (!mMenuBar.Create(this)) return -1;
		mMenuBar.SetPaneStyle(mMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);
		CMFCPopupMenu::SetForceMenuFocus(FALSE);	

		// create the tool bar	
		if (!mToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD|WS_VISIBLE|CBRS_TOP|CBRS_GRIPPER|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC) ||
			!mToolBar.LoadToolBar(IDT_TOOLBAR, 0, 0, FALSE, 0, 0, IDB_TOOLBAR)) return -1;
		CString strToolBarName; strToolBarName.LoadString(IDS_TOOLBAR);	
		mToolBar.SetWindowText(strToolBarName);
		mToolBar.ReplaceButton(ID_EDIT_FIND, CMFCToolBarComboBoxButton(ID_EDIT_FIND, GetCmdMgr()->GetCmdImage(ID_EDIT_FIND), CBS_DROPDOWN));

		// create the status bar
		if (!mStatusBar.Create(this)) return -1;
		mStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

		// Delete these five lines if you don't want the toolbar and menubar to be dockable
		mMenuBar.EnableDocking(CBRS_ALIGN_ANY);
		mToolBar.EnableDocking(CBRS_ALIGN_ANY);
		EnableDocking(CBRS_ALIGN_ANY);
		DockPane(&mMenuBar);
		DockPane(&mToolBar);
	}	

	// create a view to occupy the client area of the frame
	if (!mMainView.Create( AfxRegisterWndClass(CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_BTNSHADOW), NULL), 
		NULL, AFX_WS_DEFAULT_VIEW, CRect(0,0,0,0), this, AFX_IDW_PANE_FIRST, NULL)) return -1;	

	// create the workspace pane		
	CString strWorkspacePaneName; strWorkspacePaneName.LoadString(IDS_WORKSPACE_PANE);
	if (!mWorkspacePane.Create(strWorkspacePaneName, this, CRect(0,0,250,500), TRUE, ID_WORKSPACE_PANE, 
		WS_CHILD|WS_VISIBLE|CBRS_RIGHT|CBRS_HIDE_INPLACE|WS_CAPTION))return -1;
	mWorkspacePane.SetFont(CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT)));	
	mWorkspacePane.EnableDocking(CBRS_ALIGN_LEFT|CBRS_ALIGN_RIGHT);
	DockPane(&mWorkspacePane);

	return 0;
}

BOOL MainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) ) return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

void MainFrame::OnDestroy()
{
	CFrameWndEx::OnDestroy();	
}


// MainFrame message handlers
void MainFrame::OnSetFocus(CWnd* pOldWnd)
{
	// forward focus to the view window
	mMainView.SetFocus();
}

BOOL MainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (mMainView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo)) return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void MainFrame::OnSizing(UINT fwSide, LPRECT pRect)
{
	if( pRect->right - pRect->left < 400 )
		pRect->right = pRect->left + 400;   
	if( pRect->bottom - pRect->top < 400 )
		pRect->bottom = pRect->top + 400; 
	CFrameWndEx::OnSizing(fwSide, pRect);	
}

void MainFrame::OnViewWorkspacePane()
{
	mWorkspacePane.ShowPane(!mWorkspacePane.IsVisible(), FALSE, TRUE);
}

void MainFrame::OnUpdateViewWorkspacePane(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(mWorkspacePane.IsVisible());
}

//////////////////////////////////////////////////////////////////////////



