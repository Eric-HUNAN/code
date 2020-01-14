// LaserDeicing.h : main header file for the LaserDeicing application
//

#if !defined(AFX_LaserDeicing_H__7C80A991_2BBF_43C0_919C_02C3E47A0EEF__INCLUDED_)
#define AFX_LaserDeicing_H__7C80A991_2BBF_43C0_919C_02C3E47A0EEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "LaserDeicingDlg.h"

extern CLaserDeicingDlg *g_pMainDlg;


/////////////////////////////////////////////////////////////////////////////
// CLaserDeicingApp:
// See LaserDeicing.cpp for the implementation of this class
//

class CLaserDeicingApp : public CWinApp
{
public:
	CLaserDeicingApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLaserDeicingApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CLaserDeicingApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LaserDeicing_H__7C80A991_2BBF_43C0_919C_02C3E47A0EEF__INCLUDED_)
