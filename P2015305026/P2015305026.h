
// P2015305026.h : main header file for the P2015305026 application
//
#pragma once

#ifndef __AFXWIN_H__
    #error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CP2015305026App:
// See P2015305026.cpp for the implementation of this class
//

class CP2015305026App : public CWinApp
{
public:
    CP2015305026App();


// Overrides
public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();

// Implementation

public:
    UINT  m_nAppLook;
    afx_msg void OnAppAbout();
    DECLARE_MESSAGE_MAP()
};

extern CP2015305026App theApp;
