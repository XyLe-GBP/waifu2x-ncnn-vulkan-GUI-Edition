#pragma once
#include <afxwin.h>

class CThreadWaitDlgThread : public CWinThread
{
    DECLARE_DYNCREATE(CThreadWaitDlgThread);

public:
    CThreadWaitDlgThread();
    virtual BOOL InitInstance();
    void UpdateStatus(UINT Curpos);

    THREADWAITDIALOG m_Dlg;
    bool m_Valid;

protected:
    DECLARE_MESSAGE_MAP()
};

