////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "TabDemoApp.h"
#include "mainfrm.h"
#include "resource.h"


// Definitions for the CMainFrame class
CMainFrame::CMainFrame()
{
	//Set the tab control as the fram's view window
	SetView(m_View);

	// Set the registry key name, and load the initial window position
	// Use a registry key name like "CompanyName\\Application"
	LoadRegistrySettings(_T("Win32++\\Container Demo"));
}

CMainFrame::~CMainFrame()
{
	// Destructor for CMainFrame.
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// OnCommand responds to menu and and toolbar input

	UNREFERENCED_PARAMETER(lParam);

	switch(LOWORD(wParam))
	{
	case IDM_CLOSE_TAB:			return OnTabClose();
	case IDM_FILE_EXIT:			return OnFileExit();
	case IDM_NEW_FILES:			return OnNewFilesTab();
	case IDM_NEW_CLASSES:		return OnNewClassesTab();
	case IDM_NEW_DIALOG:		return OnNewDialogTab();
	case IDM_NEW_TAB:			return OnNewTab();
	case IDM_TAB_FIXED:         return OnTabFixedWidth();
	case IDM_TAB_DRAW:          return OnTabOwnerDraw();
	case IDM_TAB_TOP:			return OnTabsAtTop();
	case IDM_TAB_BUTTONS:		return OnShowButtons();
	case IDW_VIEW_STATUSBAR:	return OnViewStatusBar();
	case IDW_VIEW_TOOLBAR:		return OnViewToolBar();
	case IDM_HELP_ABOUT:		return OnHelp();
	}

	return FALSE;
}

int CMainFrame::OnCreate(CREATESTRUCT& cs)
{
	// OnCreate controls the way the frame is created.
	// Overriding CFrame::OnCreate is optional.
	// Uncomment the lines below to change frame options.

	// SetUseIndicatorStatus(FALSE);	// Don't show keyboard indicators in the StatusBar
	// SetUseMenuStatus(FALSE);			// Don't show menu descriptions in the StatusBar
	// SetUseReBar(FALSE);				// Don't use a ReBar
	// SetUseThemes(FALSE);				// Don't use themes
	// SetUseToolBar(FALSE);			// Don't use a ToolBar

	// call the base class function
	CFrame::OnCreate(cs);
	
	// Turn on the tab's owner draw and fixed width tabs
	m_View.SetFixedWidth(TRUE);
	m_View.SetOwnerDraw(TRUE);
	m_View.SetShowButtons(TRUE);

	return 0;
}

BOOL CMainFrame::OnFileExit()
{
	// Issue a close request to the frame
	PostMessage(WM_CLOSE);

	return TRUE;
}

void CMainFrame::OnInitialUpdate()
{	
	// Add some tabs to the tab control
	m_View.AddTabPage(new CViewClasses, _T("Classes"), IDI_CLASSVIEW);
	m_View.AddTabPage(new CViewFiles, _T("Files"), IDI_FILEVIEW);
	m_View.AddTabPage(new CViewClasses, _T("Classes"), IDI_CLASSVIEW);
	m_View.AddTabPage(new CViewFiles, _T("Files"), IDI_FILEVIEW);
	m_View.AddTabPage(new CViewDialog(IDD_MYDIALOG), _T("Dialog"), IDI_DIALOGVIEW);

	m_View.SelectPage(0);

	// PreCreate initially set the window as invisible, so show it now.
	ShowWindow();
}

void CMainFrame::OnMenuUpdate(UINT nID)
{
	// Update the check state of the various menu items
	
	BOOL bDraw  = m_View.GetWindowLongPtr(GWL_STYLE) & TCS_OWNERDRAWFIXED;
	BOOL bFixed = m_View.GetWindowLongPtr(GWL_STYLE) & TCS_FIXEDWIDTH;

	switch (nID) 
	{

	case IDM_TAB_TOP:
		{
			BOOL bTop = m_View.GetTabsAtTop();
			UINT uCheck = (bTop)? MF_CHECKED : MF_UNCHECKED;
			GetFrameMenu().CheckMenuItem(IDM_TAB_TOP, uCheck);
		}
		break;
	case IDM_TAB_BUTTONS:
		{
			BOOL bShow = m_View.GetShowButtons();
			UINT uCheck = (bShow)? MF_CHECKED : MF_UNCHECKED;
			GetFrameMenu().CheckMenuItem(IDM_TAB_BUTTONS, uCheck);
			UINT uEnable = (bDraw && bFixed)? MF_ENABLED : MF_GRAYED;
			GetFrameMenu().EnableMenuItem(IDM_TAB_BUTTONS, uEnable);
		}
		break;
	case IDM_TAB_DRAW:
		{
			UINT uCheck = (bDraw)? MF_CHECKED : MF_UNCHECKED;
			GetFrameMenu().CheckMenuItem(IDM_TAB_DRAW, uCheck);
		}
	case IDM_TAB_FIXED:
		{
			UINT uCheck = (bFixed)? MF_CHECKED : MF_UNCHECKED;
			GetFrameMenu().CheckMenuItem(IDM_TAB_FIXED, uCheck);
		}
	}

	CFrame::OnMenuUpdate(nID);
}

BOOL CMainFrame::OnNewFilesTab()
{
	m_View.AddTabPage(new CViewFiles, _T("Files"), IDI_FILEVIEW);
	return TRUE;
}

BOOL CMainFrame::OnNewClassesTab()
{
	m_View.AddTabPage(new CViewClasses, _T("Classes"), IDI_CLASSVIEW);
	return TRUE;
}

BOOL CMainFrame::OnNewDialogTab()
{
	m_View.AddTabPage(new CViewDialog(IDD_MYDIALOG), _T("Dialog"), IDI_DIALOGVIEW);
	return TRUE;
}

BOOL CMainFrame::OnNewTab()
{
	// Creates the popup menu when the "New" toolbar button is pressed

	// Position the popup menu
	CToolBar& TB = GetToolBar();
	RECT rc = TB.GetItemRect(TB.CommandToIndex(IDM_NEW_TAB));
	TB.MapWindowPoints(NULL, (LPPOINT)&rc, 2);

	TPMPARAMS tpm;
	tpm.cbSize = sizeof(TPMPARAMS);
	tpm.rcExclude = rc;

	// Load the popup menu
	CMenu TopMenu(IDM_NEWMENU);
	CMenu PopupMenu = TopMenu.GetSubMenu(0);

	// Start the popup menu
	PopupMenu.TrackPopupMenuEx(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL, rc.left, rc.bottom, *this, &tpm);
	return TRUE;
}

BOOL CMainFrame::OnShowButtons()
{
	BOOL bShow = m_View.GetShowButtons();
	m_View.SetShowButtons(!bShow);
	m_View.RedrawWindow();
	return TRUE;
}

BOOL CMainFrame::OnTabClose()
{
	int iTab = m_View.GetCurSel();

	if (iTab >= 0)
		m_View.RemoveTabPage(iTab);

	return TRUE;
}

BOOL CMainFrame::OnTabFixedWidth()
{
	BOOL bFixed = m_View.GetWindowLongPtr(GWL_STYLE) & TCS_FIXEDWIDTH;
	m_View.SetFixedWidth(!bFixed);
	return TRUE;
}

BOOL CMainFrame::OnTabOwnerDraw()
{
	BOOL bDraw = m_View.GetWindowLongPtr(GWL_STYLE) & TCS_OWNERDRAWFIXED;
	m_View.SetOwnerDraw(!bDraw);
	return TRUE;
}

BOOL CMainFrame::OnTabsAtTop()
{
	BOOL bTop = m_View.GetTabsAtTop();
	m_View.SetTabsAtTop(!bTop);
	return TRUE;
}

void CMainFrame::PreCreate(CREATESTRUCT& cs)
{
	// Call the base class function first
	CFrame::PreCreate(cs);
	
	// Hide the window initially by removing the WS_VISIBLE style
	cs.style &= ~WS_VISIBLE;
}

void CMainFrame::SetupToolBar()
{
	// Set the Resource IDs for the toolbar buttons
	AddToolBarButton( IDM_NEW_TAB           );
	AddToolBarButton( IDM_FILE_OPEN,  FALSE );
	AddToolBarButton( IDM_FILE_SAVE,  FALSE );
	
	AddToolBarButton( 0 );	// Separator
	AddToolBarButton( IDM_EDIT_CUT,   FALSE );
	AddToolBarButton( IDM_EDIT_COPY,  FALSE );
	AddToolBarButton( IDM_EDIT_PASTE, FALSE );
	
	AddToolBarButton( 0 );	// Separator
	AddToolBarButton( IDM_FILE_PRINT, FALSE );
	
	AddToolBarButton( 0 );	// Separator
	AddToolBarButton( IDM_HELP_ABOUT        );

	// Set the Checkmarks in the menu
	GetFrameMenu().CheckMenuItem(IDM_TAB_BUTTONS, MF_UNCHECKED);

	// Add some extra icons for menu items
	AddMenuIcon(IDM_NEW_FILES, GetApp().LoadIcon(IDI_FILEVIEW));
	AddMenuIcon(IDM_NEW_CLASSES, GetApp().LoadIcon(IDI_CLASSVIEW));
	AddMenuIcon(IDM_NEW_DIALOG, GetApp().LoadIcon(IDI_DIALOGVIEW));
}

LRESULT CMainFrame::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{
//		Add case statements for each messages to be handled here
//	}

	// pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}

