#ifndef SIMPLEAPP_H
#define SIMPLEAPP_H

#include "wxx_wincore.h"
#include "MainFrm.h"


class CWceFrameApp : public CWinApp
{
public:
    CWceFrameApp();
    virtual ~CWceFrameApp() {}
	virtual BOOL InitInstance();
	CMainFrame& GetMainFrame() { return m_Frame; }

private:
    CMainFrame m_Frame;
};


// returns a reference to the CWceFrameApp object
inline CWceFrameApp& GetFrameApp() { return static_cast<CWceFrameApp&>(GetApp()); }


#endif //SIMPLEAPP_H

