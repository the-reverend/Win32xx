///////////////////////////////////////////////////////
// Mainfrm.h
//  Declaration of the CMainFrame class

#ifndef MAINFRM_H
#define MAINFRM_H


#include "View.h"
#include "Doc.h"
#include "wxx_ribbon.h"


// Declaration of the CMainFrame class
class CMainFrame : public CRibbonFrame
{
public:
	CMainFrame();
	virtual ~CMainFrame();

	CDoc& GetDoc() { return m_Doc; }

	void LoadFile(LPCTSTR str);
	void MRUFileOpen(UINT nMRUIndex);
	void OnFileExit();
	void OnFileNew();
	void OnFileOpen();
	void OnFileSave();
	void OnFileSaveAs();
	void OnFilePrint();
	void OnMRUList(const PROPERTYKEY* key, const PROPVARIANT* ppropvarValue);
	void OnPenColor(const PROPVARIANT* ppropvarValue, IUISimplePropertySet* pCmdExProp);
	void SetPenColor(COLORREF clr);

protected:
	virtual STDMETHODIMP Execute(UINT32 nCmdID, UI_EXECUTIONVERB verb, const PROPERTYKEY* key, const PROPVARIANT* ppropvarValue, IUISimplePropertySet* pCmdExProp);
	virtual STDMETHODIMP UpdateProperty(UINT32 nCmdID, __in REFPROPERTYKEY key, __in_opt  const PROPVARIANT *currentValue, __out PROPVARIANT *newValue);
	BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void SetupToolBar();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CView m_View;
	CDoc m_Doc;
	CString m_PathName;

};

#endif //MAINFRM_H

