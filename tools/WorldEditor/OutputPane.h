//
// OutputPane.h : interface of the OutputPane class
//
//////////////////////////////////////////////////////////////////////////

#pragma once

/////////////////////////////////////////////////////////////////////////////
// OutputPane window

class OutputPane : public CDockablePane
{
// Construction
public:
	OutputPane();
	virtual ~OutputPane();

	void Print( const char* text );

// Attributes
private:
	CEdit mOutputEdit;
	CComboBox mCommandLine;

	#define MAX_BUFFER_SIZE 0x7FFF
	unsigned int mBufferSize;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////