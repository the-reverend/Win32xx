/* (10-08-2014) [Tab/Indent: 8/8][Line/Box: 80/74]          (CColorChoice.cpp) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2016, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                          robert.c.tausworthe@ieee.org                        |
|                                                                              |
===============================================================================*

	Contents Description: Implementation of the CColorChoice class for
	applications using the Win32++ Windows interface classes, Copyright
	(c) 2005-2016 David Nash, under permissions granted therein. This class
	extends the CColoDialog class to select colors for a number of items
	whose usage descriptors are displayed in a ClistBoxDlg. Instructions
	on the application of this class appears below.

        Caveats: The copyright displayed above extends only to the author's
	original contributions to the subject class, and to the alterations,
	additions, deletions, and other treatments of materials that may have
	been extracted from the cited sources.  Unaltered portions of those
	materials retain their original copyright status. The author hereby
	grants permission to any person obtaining a copy of this treatment
	of the subject class and any associated documentation composed by
	the author, to utilize this material, free of charge and without
	restriction or limitation, subject to the following conditions:

		The above copyright notice, as well as that of David Nash
		and Win32++, together with the respective permission
		conditions shall be included in all copies or substantial
		portions of this material so copied, modified, merged,
		published, distributed, or otherwise held by others.

	These materials are provided "as is", without warranty of any kind,
	express or implied, including but not limited to: warranties of
	merchantability, fitness for a particular purpose, and non-infringement.
	In no event shall the authors or copyright holders be liable for any
	claim, damages, or other liability, whether in an action of contract,
	tort or otherwise, arising from, out of, or in connection with, these
	materials, the use thereof, or any other other dealings therewith.

	Usage Information: This class populates a table of usage-color pairs
	that may be used by an application or a derived class.  The class then
	utilizes the CListBoxDlg class to display the usage name strings in
	a list box and the CColorDialog class (from which this class derives)
	to assign colors to a selected usages displayed in the list box. The
	user populates the color by table using the ClearColorTable() to
	empty any current content and then calling the AddColorChoice() method
	to add usage-color structs to the color table.  The usage names appear
	in the list box for selection when the DoModal() member is engaged.

	The user calls upon the DoModal() method to display the color selection
	ListBox and thence the color choice dialog to supply the color for that
	selection.  Access to a particular element in the color table is made
	using its index in the DeleteEntry(), GetColor(), GetBrush(),
	GetUsage(), SetColorTable(), and SetUsage() methods.

	Programming Notes:
               The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
		Planning and Preparation Subsystem project for C++ programming.

********************************************************************************

	Implementation of the CColorChoice class

*******************************************************************************/

#include "stdafx.h"
#include "CColorChoice.h"
#include "resource.h"

/*============================================================================*/
	CColorChoice::
CColorChoice()						   /*

	Construct a color choice object with or without initial values.
*-----------------------------------------------------------------------------*/
{
	ClearColorTable();
	SetBoxTitle(_T("Color"));
}

/*============================================================================*/
	BOOL CColorChoice::
AddColorChoice(const CString& usage, COLORREF color) 			/*

	Add the (name, color) pair to the color table, return TRUE on success,
	FALSE otherwise.
*-----------------------------------------------------------------------------*/
{
	UINT size = m_ColorTable.size();
	ctl_color pair;
	pair.usage = usage;
	pair.color = color;
	m_ColorTable.push_back(pair);
	return (UINT)m_ColorTable.size() == size + 1;
}

/*============================================================================*/
	UINT CColorChoice::
DeleteEntry(UINT index)							/*

	Description and explanation of parameters
*-----------------------------------------------------------------------------*/
{
	return (m_ColorTable.erase(m_ColorTable.begin() + index) -
	     m_ColorTable.begin());
}

/*============================================================================*/
	INT_PTR CColorChoice::
DoModal(HWND hWndOwner /* = 0 */) 					/*

        Show the base class color dialog box and select an app control
	color.
*-----------------------------------------------------------------------------*/
{
	  // determine a common window for the two dialog boxes
	if (hWndOwner == 0)
		hWndOwner = GetParameters().hwndOwner;
	if (hWndOwner == 0)
		hWndOwner = GetApp().GetMainWnd();
	  // prepare the list box dialog: load the color table choice items
	m_LBDlg.ClearList();
	for (UINT i = 0; i < m_ColorTable.size(); i++)
		m_LBDlg.AddListItem(m_ColorTable[i].usage.c_str());
	  // Make the control be modal so the choice is returned at
	  // termination. Further, name the parent that cannot be accessed
	  // during the modal operation.
	int selection = m_LBDlg.DoModal(hWndOwner);
	  // if invalid, go no further
	if (selection < 0)
		return IDCANCEL;

	  // register the current control color in the color choice struct
	SetColor(m_ColorTable[selection].color);
	  // invoke the control and save the result on success
	if(CColorDialog::DoModal(hWndOwner) ==  IDOK)
	{
		m_ColorTable[selection].color = CColorDialog::GetColor();
		return IDOK;
	}
	return IDCANCEL;
}

/*============================================================================*/
	CBrush& CColorChoice::
GetBrush(UINT index) 							/*

	Return a brush with the color found in the color table at the given
	index. The returned reference to a permanent object is required.
*-----------------------------------------------------------------------------*/
{
   	m_br.CreateSolidBrush(GetColor(index));
    	return m_br;
}

/*============================================================================*/
	void CColorChoice::
InitCustomColors()                                                      /*

	Initialize the custom color table entries to default values. Override
	this member to provide an application-dependent array.
*-----------------------------------------------------------------------------*/
{
	// initial values for custom colors in color dialog
	COLORREF rgbCustomColors[16];
	rgbCustomColors[0]  = RGB(0,     5,   5);
	rgbCustomColors[1]  = RGB(0,    15,  55);
	rgbCustomColors[2]  = RGB(0,    25, 155);
	rgbCustomColors[3]  = RGB(0,    35, 255);
	rgbCustomColors[4]  = RGB(10,    0,   5);
	rgbCustomColors[5]  = RGB(10,   20,  55);
	rgbCustomColors[6]  = RGB(10,   40, 155);
	rgbCustomColors[7]  = RGB(10,   60, 255);
	rgbCustomColors[8]  = RGB(100,   5,   5);
	rgbCustomColors[9]  = RGB(100,  25,  55);
	rgbCustomColors[10] = RGB(100,  50, 155);
	rgbCustomColors[11] = RGB(100, 125, 255);
	rgbCustomColors[12] = RGB(200, 120,   5);
	rgbCustomColors[13] = RGB(200, 150,  55);
	rgbCustomColors[14] = RGB(200, 200, 155);
	rgbCustomColors[15] = RGB(200, 250, 255);

	SetCustomColors(rgbCustomColors);
}

/*============================================================================*/
	void CColorChoice::
OnHelpButton()	                                                        /*

	Override base class member to send the font context help identifier to
	the window whose handle is the object's hwndOwner.
-----------------------------------------------------------------------------*/
{
	  // route the help message to the owner window
	HWND hwndOwner = GetParameters().hwndOwner;
	if (::IsWindow(hwndOwner))
		SendMessage(hwndOwner, WM_COMMAND, IDC_HELP_COMDLG,
			IDM_HELP_COLORDLG);
}

/*============================================================================*/
	BOOL CColorChoice::
OnInitDialog()                                                          /*

	Perform any actions required for initialization of this object when
	the color choice is being initialized.
*-----------------------------------------------------------------------------*/
{
	  // do the base class initialization first (it currently does nothing)
	CDialog::OnInitDialog();
	SetWindowTitle();
	InitCustomColors();
	return TRUE;
}


/*============================================================================*/
        void CColorChoice::
Serialize(CArchive &ar)                                               	/*

        Called to serialize the current color choice table to or from the
	archive ar, depending on the sense of IsStoring(). Leaves the archive
	open for for further operations.
*-----------------------------------------------------------------------------*/
{
	  // perform loading or storing
    if (ar.IsStoring())
    {
		  // save the current color
		ar << GetParameters().rgbResult;
        	  // save the custom colors
		ArchiveObject ao(GetCustomColors(), 16 * sizeof(COLORREF) );
		ar << ao;
		  // save the color table entries
		UINT n = m_ColorTable.size();
		ar << n;
		std::vector<ctl_color>::iterator it;
		for (it = m_ColorTable.begin(); it < m_ColorTable.end(); ++it)
		{
			ar << (*it).usage;
			ar << (*it).color;
		}

	}
    else    // recovering
    {
		  // recover current color
		COLORREF rgbResult;
		ar >> rgbResult;
		SetColor(rgbResult);
        	  // read in the custom colors
		COLORREF cr[16];
		ArchiveObject ao(cr, 16 * sizeof(COLORREF) );
		ar >> ao;
		SetCustomColors(cr);
		  // recover the color table entries
		UINT n;
		ar >> n;
		std::vector<ctl_color>::iterator it;
		for (it = m_ColorTable.begin(); it < m_ColorTable.begin() + n; ++it)
		{
			ar >> (*it).usage;
			ar >> (*it).color;
		}

	}

	CColorDialog::Serialize(ar);
}

