#include "stdafx.h"
#include "Figure.h"


CFigure::CFigure()
{
}


CFigure::~CFigure()
{
}


void CFigure::DrawFigure(CDC *pDC)
{
    CDC memDC;
    memDC.CreateCompatibleDC(pDC);
    int animateX = 0, animateY = 0;
    //	Alive
    if (m_nHealth > 0) {
        memDC.SelectObject(&m_bmFigure);
        

        switch (m_currentState) {
        case STILL: animateX = 0; animateY = 0;	break;
        case MOVE1:	animateX = m_size.cx; animateY = 0; break;
        case MOVE2:	animateX = 2 * m_size.cx; animateY = 0; break;
        case MOVE3:	animateX = 0; animateY = m_size.cy; break;
        case MOVE4:	animateX = m_size.cx; animateY = m_size.cy; break;
        case MOVE5: animateX = 2 * m_size.cx; animateY = m_size.cy; break;
        case MOVE6:	animateX = 0; animateY = 2 * m_size.cy; break;
        case RISING: animateX = m_size.cx; animateY = 2 * m_size.cy; break;
        case FALLING: animateX = 2 * m_size.cx; animateY = 2 * m_size.cy; break;
        default: animateX = 0; animateY = 0; break;
        }
        //	���� �ٶ� ��
        if (m_bLookAtLeft) {
            CDC tmpDC;
            CBitmap tmpDCbmp;
            tmpDC.CreateCompatibleDC(pDC);
            tmpDCbmp.CreateCompatibleBitmap(pDC, m_size.cx, m_size.cy);
            tmpDC.SelectObject(&tmpDCbmp);

            tmpDC.StretchBlt(0, 0, m_size.cx, m_size.cy,
                &memDC, animateX + m_size.cx - 1, animateY, -m_size.cx, m_size.cy, SRCCOPY);
            pDC->TransparentBlt(m_Xpos, m_Ypos, m_size.cx, m_size.cy,
                &tmpDC, 0, 0, m_size.cx, m_size.cy, RGB(0, 255, 255));
        }
        //	������ �ٶ� ��
        else {
            pDC->TransparentBlt(m_Xpos, m_Ypos, m_size.cx, m_size.cy,
                &memDC, animateX, animateY, m_size.cx, m_size.cy, RGB(0, 255, 255));
        }
    }
    //	Dead
    else {
        CBitmap bmBlast;
        BITMAP bmBlastInfo;
        bmBlast.LoadBitmapW(IDB_BLAST);
        bmBlast.GetBitmap(&bmBlastInfo);
        memDC.SelectObject(&bmBlast);
        
        switch (m_nDeadAniCount) {
        case 1: animateX = 0; animateY = 0; break;
        case 2: animateX = 73; animateY = 0; break;
        case 3: animateX = 146; animateY = 0; break;
        case 4: animateX = 0; animateY = 63; break;
        case 5: animateX = 73; animateY = 63; break;
        case 6: animateX = 146; animateY = 63; break;
        default: animateX = 218; animateY = 125; break;
        }
        pDC->TransparentBlt(m_Xpos, m_Ypos, 73, 63,
            &memDC, animateX, animateY, 73, 63, RGB(0, 255, 255));
    }
}
