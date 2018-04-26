
// MainView.cpp : implementation of the MainView class
//
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainApp.h"
#include "MainView.h"
#include "editor/Editor.h"

//////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// MainView
//////////////////////////////////////////////////////////////////////////

#define TIMER_PAINT	100

//////////////////////////////////////////////////////////////////////////

MainView::MainView()
{
	mLButtonPress = FALSE;
	mRButtonPress = FALSE;
}

MainView::~MainView()
{
}


BEGIN_MESSAGE_MAP(MainView, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_FILE_NEW, &MainView::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &MainView::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &MainView::OnFileSave)	
	ON_COMMAND(ID_EDIT_COPY, &MainView::OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, &MainView::OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, &MainView::OnEditPaste)	
	ON_COMMAND(ID_EDIT_DELETE, &MainView::OnEditDelete)	
	ON_COMMAND(ID_EDIT_UNDO, &MainView::OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, &MainView::OnEditRedo)
	ON_COMMAND(ID_EDIT_FIND, &MainView::OnEditFind)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// MainView message handlers
BOOL MainView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

int MainView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1) return -1;

	SetTimer(TIMER_PAINT, 10, NULL);

	// init the world editor
	Editor::Instance().Init(this->GetSafeHwnd());

	return 0;
}

void MainView::OnDestroy()
{
	// exit the world editor
	Editor::Instance().Exit();

	CWnd::OnDestroy();
}


void MainView::OnTimer(UINT_PTR nIDEvent)
{
	switch( nIDEvent )
	{
	case TIMER_PAINT:
		this->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		break;
	}

	CWnd::OnTimer(nIDEvent);
}

void MainView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	this->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

void MainView::OnPaint() 
{
	CPaintDC dc(this);
	Editor::Instance().Run();
}

BOOL MainView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	// return CWnd::OnEraseBkgnd(pDC);
	return TRUE;
}

void MainView::OnFileNew()
{
/*	NewMapDlg dlg( this );
	int id = (int) dlg.DoModal();
	if (id == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (id == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	*/
	Editor::Instance().New(10,10,"map/grass.tga");
}

void MainView::OnFileOpen()
{

}
void MainView::OnFileSave()
{

}

void MainView::OnEditUndo()
{

}

void MainView::OnEditRedo()
{

}

void MainView::OnEditCopy()
{

}

void MainView::OnEditCut()
{

}

void MainView::OnEditPaste()
{

}

void MainView::OnEditDelete()
{

}

void MainView::OnEditFind()
{

}

//////////////////////////////////////////////////////////////////////////

void MainView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	this->SetFocus();

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void MainView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	this->SetFocus();

	CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

void MainView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	this->SetFocus();

	mLButtonPress = TRUE;

	CWnd::OnLButtonDown(nFlags, point);
}

void MainView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	this->SetFocus();

	mLButtonPress = FALSE;

	CWnd::OnLButtonUp(nFlags, point);
}

void MainView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	this->SetFocus();

	mRButtonPress = TRUE;

	GetCursorPos(&mPtMouse);
	SetCapture();
	while(ShowCursor(FALSE) >= 0);

	RECT wnd_rc; this->GetClientRect(&wnd_rc);
	I32 wnd_width = wnd_rc.right-wnd_rc.left;
	I32 wnd_height = wnd_rc.bottom-wnd_rc.top;

	Vector3 pt(0, 0, 1.0f);
	Editor::Instance().project(pt,pt);
	mOffset.x = point.x - (I32)(pt[0]);
	mOffset.y = (wnd_height - point.y) - (I32)(pt[1]);

//	mOffset.x = point.x;
//	mOffset.y = wnd_height - point.y;

//	mOffset = point;

	CWnd::OnRButtonDown(nFlags, point);
}

void MainView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	this->SetFocus();

	SetCursorPos(mPtMouse.x, mPtMouse.y);
	ReleaseCapture();
	while(ShowCursor(TRUE) < 0);

	mRButtonPress = FALSE;
	mOffset.x = 0;
	mOffset.y = 0;

	CWnd::OnRButtonUp(nFlags, point);
}

BOOL MainView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	F32 scale = Editor::Instance().GetScale();
	if(zDelta > 0) scale += 0.4f;
	else if(zDelta < 0) scale -= 0.4f;
	Editor::Instance().SetScale(GMath::clamp(scale, 10.0f, 1.0f));
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void MainView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	this->SetFocus();

	RECT wnd_rc; this->GetClientRect(&wnd_rc);
	I32 wnd_width = wnd_rc.right-wnd_rc.left;
	I32 wnd_height = wnd_rc.bottom-wnd_rc.top;

	if(mLButtonPress)
	{
		Vector3 pt(point.x, wnd_height-point.y, 0.0f);
		Editor::Instance().unproject(pt,pt);
		Editor::Instance().DoDrag(Vector3(pt[0], pt[1], 0.0f));
	}
	else
	{
		Vector3 pt(point.x, wnd_height-point.y, 0.0f);
		Editor::Instance().unproject(pt,pt);
		Editor::Instance().DoMove(Vector3(pt[0], pt[1], 0.0f));
	}

	if(mRButtonPress)
	{
/*		Vector3 pt((point.x-mOffset.x), (mOffset.y-point.y), 0);
		Editor::Instance().unproject(pt,pt);
		Vector3 eye = Editor::Instance().GetEye();		
		eye -= pt;
		Editor::Instance().SetEye(eye);
*/
		Vector3 pt(point.x - mOffset.x, (wnd_height-point.y) - mOffset.y, 1.0f);
		Editor::Instance().unproject(pt,pt);
		Vector3 eye = Editor::Instance().GetEye();
		eye[0] = eye[0]-pt[0];
		eye[1] = eye[1]-pt[1];
		Editor::Instance().SetEye(eye);
	}

	CWnd::OnMouseMove(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////