// LaserDeicing.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "LaserDeicing.h"
#include "LaserDeicingDlg.h"
#include "GeneralDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//全局变量
CLaserDeicingDlg *g_pMainDlg = NULL;

/////////////////////////////////////////////////////////////////////////////
// CLaserDeicingApp

BEGIN_MESSAGE_MAP(CLaserDeicingApp, CWinApp)
	//{{AFX_MSG_MAP(CLaserDeicingApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLaserDeicingApp construction

CLaserDeicingApp::CLaserDeicingApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CLaserDeicingApp object

CLaserDeicingApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CLaserDeicingApp initialization

BOOL CLaserDeicingApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

    NET_DVR_Init();          //初始化设备

	CLaserDeicingDlg dlg;
	m_pMainWnd = &dlg;
	g_pMainDlg = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CLaserDeicingApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	NET_DVR_Cleanup();//释放SDK资源，在程序结束之前调用
	return CWinApp::ExitInstance();
}
