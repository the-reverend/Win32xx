#include "MainFrm.h"
#include "resource.h"



CMainFrame::CMainFrame() : m_PenColor(RGB(0,0,0))
{
	// Set the Resource IDs for the toolbar buttons
	AddToolBarButton( 0 );			// Separator
	AddToolBarButton( IDM_RED  );
	AddToolBarButton( IDM_BLUE );
	AddToolBarButton( IDM_GREEN );
	AddToolBarButton( IDM_BLACK );
}

void CMainFrame::DrawLine(short x, short y)
{
	CDC dc = GetDC();
	dc.CreatePen(PS_SOLID, 1, m_points.back().color);
	dc.MoveTo(m_points.back().x, m_points.back().y);
	dc.LineTo(x, y);
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM /*lParam*/)
{
	// Respond to menu and toolbar selections
	switch (LOWORD(wParam))
	{
	// Respond to menu items
	case IDM_NEW:
		m_points.clear();
		Invalidate();
		return TRUE;
	case IDM_HELP_ABOUT:
		{
			CDialog HelpDialog(IDW_ABOUT);
			HelpDialog.DoModal(*this);
		}
		return TRUE;

	// Respond to ToolBar buttons
	case IDM_RED:
		m_PenColor = RGB(255, 0, 0);
		TRACE("Red Pen Selected \n");
		return TRUE;
	case IDM_BLUE:
		m_PenColor = RGB(0, 0, 255);
		TRACE("Blue Pen Selected \n");
		return TRUE;
	case IDM_GREEN:
		m_PenColor = RGB(0, 191, 0);
		TRACE("Green Pen Selected \n");
		return TRUE;
	case IDM_BLACK:
		m_PenColor = RGB(0, 0, 0);
		TRACE("Black Pen Selected \n");
		return TRUE;

	// Respond to the accelerator key
	case IDW_QUIT:
		SendMessage(WM_CLOSE, 0L, 0L);
		return TRUE; 
	} 

	return FALSE;
}

void CMainFrame::OnDraw(CDC& dc)
{
	// Redraw our client area
	if (m_points.size() > 0)
	{
		bool bDraw = false;  //Start with the pen up
		for (unsigned int i = 0 ; i < m_points.size(); i++)
		{
			dc.CreatePen(PS_SOLID, 1, m_points[i].color);
			if (bDraw)
				dc.LineTo(m_points[i].x, m_points[i].y);
			else
				dc.MoveTo(m_points[i].x, m_points[i].y);
			
			bDraw = m_points[i].PenDown;
		}
	}
}

void CMainFrame::OnInitialUpdate()
{
	// Startup code goes here
}

LRESULT CMainFrame::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(uMsg);
	UNREFERENCED_PARAMETER(wParam);

	// Capture mouse input.
	SetCapture();

	StorePoint(LOWORD(lParam), HIWORD(lParam), true);

	return FinalWindowProc(uMsg, wParam, lParam);
}

LRESULT CMainFrame::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(uMsg);
	UNREFERENCED_PARAMETER(wParam);

	//Release the capture on the mouse
	ReleaseCapture();

	StorePoint(LOWORD(lParam), HIWORD(lParam), false);
	
	return FinalWindowProc(uMsg, wParam, lParam);
}

LRESULT CMainFrame::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(uMsg);

	// hold down the left mouse button and move mouse to draw lines.
	if (wParam & MK_LBUTTON)
	{	
		TCHAR str[80];
		::wsprintf(str, TEXT("Draw Point:  %hd, %hd\n"), LOWORD(lParam), HIWORD(lParam));
		TRACE(str);

		DrawLine(LOWORD(lParam), HIWORD(lParam));
		StorePoint(LOWORD(lParam), HIWORD(lParam), true);
	}

	return FinalWindowProc(uMsg, wParam, lParam);
}

void CMainFrame::SetPen(COLORREF color)
{
	m_PenColor = color;
}

void CMainFrame::StorePoint(int x, int y, bool PenDown)
{
	PlotPoint P1;
	P1.x = x;
	P1.y = y;
	P1.PenDown = PenDown;
	P1.color = m_PenColor;

	m_points.push_back(P1); //Add the point to the vector
}

LRESULT CMainFrame::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// handle left mouse button up/down and mouse move messages
	// a seperate function for each case keeps the code tidy.
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:	return 	OnLButtonDown(uMsg, wParam, lParam);
	case WM_MOUSEMOVE:		return 	OnMouseMove(uMsg, wParam, lParam);
    case WM_LBUTTONUP:		return  OnLButtonUp(uMsg, wParam, lParam);
	}

	// Pass unhandled messages on to WndProcDefault
	return WndProcDefault(uMsg, wParam, lParam);
}


