#ifndef MYDIALOG_H
#define MYDIALOG_H


#include "wxx_dialog.h"
#include "resource.h"


class CMyDialog : public CDialog
{
public:
	CMyDialog(UINT nResID);
	virtual ~CMyDialog();
	void AddToEdit();
	void AddToListBox();
	void AddToScrollBars();
	void AddToComboBox();
	void AddToButton();

private:
	virtual BOOL OnInitDialog();
	virtual INT_PTR DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void OnOK();

	int m_nCounter;
};

#endif //MYDIALOG_H
