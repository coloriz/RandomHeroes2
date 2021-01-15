#pragma once
#include "resource.h"
enum BitmapAni { STILL, MOVE1, MOVE2, MOVE3, MOVE4, MOVE5, MOVE6, RISING, FALLING };

class CFigure
{
public:
    CFigure();
    ~CFigure();
//	Attributes
public:
    CBitmap m_bmFigure;
    BITMAP m_bmInfo;
    int m_nHealth;
    bool m_bDamageCoolTime;
    bool m_bDamageFlicker;
    CSize m_size;
    int m_Xpos;
    int m_Ypos;
    int m_Xrange1;
    int m_Xrange2;
    bool m_bIsMoving;
    bool m_bLookAtLeft;
    bool m_bPrevLookAtLeft;
    CRect m_ValidRect;
    bool m_bIsOnGround;
    enum BitmapAni m_currentState;

    int m_nDeadAniCount;

//	Operations
public:
    void DrawFigure(CDC *pDC);
};

