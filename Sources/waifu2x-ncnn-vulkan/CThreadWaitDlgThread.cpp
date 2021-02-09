#include "pch.h"
#include "THREADWAITDIALOG.h"
#include "CThreadWaitDlgThread.h"

IMPLEMENT_DYNCREATE(CThreadWaitDlgThread, CWinThread)

CThreadWaitDlgThread::CThreadWaitDlgThread() : m_Valid(true)
{
}

BOOL CThreadWaitDlgThread::InitInstance()
{
	m_pMainWnd = &m_Dlg;
	m_Dlg.DoModal();
	m_Valid = false;

	return TRUE;
}

void CThreadWaitDlgThread::UpdateStatus(UINT Curpos)
{
	if (m_Dlg.m_hWnd) { // Statusダイアログが閉じられてしまっていないかチェック後post
		m_Dlg.PostMessage(WM_COMMAND, ID_UPDATE_STATUS);
	}
}

BEGIN_MESSAGE_MAP(CThreadWaitDlgThread, CWinThread)
END_MESSAGE_MAP()
