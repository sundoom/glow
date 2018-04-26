
// OutputPane.cpp : implementation of the OutputPane class
//
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OutputPane.h"
#include "resource.h"

//////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputBar

OutputPane::OutputPane()
{
	mBufferSize = 0;
}

OutputPane::~OutputPane()
{
}

BEGIN_MESSAGE_MAP(OutputPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int OutputPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	mOutputEdit.Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|ES_READONLY|ES_AUTOVSCROLL|ES_MULTILINE, CRect(0,0,0,0),this,ID_OUTPUT_PANE_OUTPUTEDIT);
	mOutputEdit.ModifyStyleEx( 0, WS_EX_CLIENTEDGE );
	mOutputEdit.SetFont(CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT)));

	if( !mCommandLine.Create( WS_CHILD | WS_VISIBLE | CBS_AUTOHSCROLL | CBS_DROPDOWN, CRect(0,0,0,0), this, ID_OUTPUT_PANE_COMMANDLINE ) )return -1;	
	mCommandLine.SetFont(CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT)));

	Print("Build output is being displayed here.\n");
	Print("The output is being displayed in rows of a list view.\n");
	Print("but you can change the way it is displayed as you wish...\n");
	
	return 0;
}

void OutputPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	mOutputEdit.SetWindowPos( NULL, 0, 0, cx, cy - 20, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER );
	mCommandLine.SetWindowPos( NULL, 0, cy - 20, cx, 20, SWP_NOACTIVATE | SWP_NOZORDER );
}

static char *TranslateString( const char *string )
{
	static char	buf[MAX_BUFFER_SIZE];
	int		i;
	int		len;
	char	*out;

	len = strlen( string );
	if( len > sizeof(buf)-1 ) {
		string = string+len-(sizeof(buf)-1);
	}

	out = buf;
	for( i=0; string[i]; i++ ) {
		if( string[i] == '\n' && string[i+1] == '\r' ) {
			*out++ = '\r';
			*out++ = '\n';
			i++;
		} else if( string[i] == '\r' || string[i] == '\n' ) {
			*out++ = '\r';
			*out++ = '\n';
		} else {
			*out++ = string[i];
		}
	}
	*out = 0;

	return buf;
}

void OutputPane::Print( const char* text ) 
{
	char *string = TranslateString( text );
	unsigned int len = strlen( string );
	mBufferSize += len;
	if( mBufferSize > MAX_BUFFER_SIZE ) 
	{
		mOutputEdit.SetSel( 0, 0x7FFFFFFF );
		mBufferSize = len;
	} 
	else 
	{
		mOutputEdit.SetSel( 0x7FFFFFFF, 0x7FFFFFFF );
	}
	mOutputEdit.LineScroll( 0xFFFF );
	mOutputEdit.SendMessage( EM_SCROLLCARET );
	mOutputEdit.ReplaceSel( string );
}

//////////////////////////////////////////////////////////////////////////