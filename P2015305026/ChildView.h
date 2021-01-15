
// ChildView.h : interface of the CChildView class
//


#pragma once
#include "Figure.h"
#include "Bullet.h"

#include <mmsystem.h>
#include <Digitalv.h>
#pragma comment(lib, "winmm.lib")

enum _State { PLAYING, YOUDIED, ENDING };
// CChildView window

class CChildView : public CWnd
{
    // Construction
public:
    CChildView();

    // Attributes
public:
    enum _State State;

    CFigure Agent;
    CList<CBullet, CBullet&> Bullets;
    bool m_bFireCoolTime;

    CArray<CFigure *, CFigure *> Alien;

    CBitmap bmHPbar;
    BITMAP bmHPbarInfo;

    CBitmap bmBackGround;
    BITMAP bmBackGroundInfo;

    CBitmap bmTerrain;
    BITMAP bmTerrainInfo;

    int velocityY;
    int gravity;

    int nGroundLevel;

    CPoint Camera;

    CRgn rgnTerrain;
    CRgn rgnPartial[16];

    int iAlienFlickerIndex;

    //	For Background Music
    MCI_OPEN_PARMS mciOpenParms;
    MCI_PLAY_PARMS mciPlayParms;
    UINT wDeviceID;

    //	Sound pathes
    CString szBGM;
    CString szEndingBGM;
    CString szShoot;
    CString szBlow;

    //	Miscellaneous BITMAPS
    CBitmap bmYouDied, bmEnding;
    BITMAP bmYouDiedInfo, bmEndingInfo;

// Operations
public:
    DWORD LoadWAV(HWND hWnd, LPCTSTR lpszWave) {
        DWORD Result;

        mciOpenParms.lpstrDeviceType = L"MPEGVideo";
        //WaveAudio ��� MPEGVideo�� ����ϸ� mp3 ������ ����մϴ�.

        mciOpenParms.lpstrElementName = lpszWave;

        Result = mciSendCommand(wDeviceID, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)(LPVOID)&mciOpenParms);

        if (Result)
            return Result;

        wDeviceID = mciOpenParms.wDeviceID;

        mciPlayParms.dwCallback = (DWORD)hWnd;

        if (Result)
            return Result;

        return 0;
    }
    // Overrides
protected:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

    // Implementation
public:
    virtual ~CChildView();

    // Generated message map functions
protected:
    afx_msg void OnPaint();
    DECLARE_MESSAGE_MAP()
public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnDestroy();
    afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
    void InitializeObjects();
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

