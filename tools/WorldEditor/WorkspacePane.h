//
// WorkspacePane.h : interface of the OutputPane class
//
//////////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////////

class WorkspacePane : public CDockablePane
{
public:
	WorkspacePane();
	virtual ~WorkspacePane();

protected:
	CMFCTabCtrl	mTabCtrl;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////