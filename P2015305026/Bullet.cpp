#include "stdafx.h"
#include "Bullet.h"
#include "resource.h"


CBullet::CBullet(int x, int y, int nBoundary, bool goLeft)
{
    m_Xpos = x;
    m_Ypos = y;
    m_nBoundary = nBoundary;
    m_bGoLeft = goLeft;
    m_nAniCount = 0;
    m_bIsHit = false;
}

CBullet::CBullet(const CBullet & c)
{
    m_bmBullet.LoadBitmapW(IDB_BULLET);
    m_bmBullet.GetBitmap(&m_bmBulletInfo);
    m_bmSplash.LoadBitmapW(IDB_SPLASH);
    m_bmSplash.GetBitmap(&m_bmSplashInfo);
    m_bmHitSplash.LoadBitmapW(IDB_HITSPLASH);
    m_bmHitSplash.GetBitmap(&m_bmHitSplashInfo);
    m_Xpos = c.m_Xpos;
    m_Ypos = c.m_Ypos;
    m_nBoundary = c.m_nBoundary;
    m_bGoLeft = c.m_bGoLeft;
    m_nAniCount = c.m_nAniCount;
    m_bIsHit = c.m_bIsHit;
}

CBullet & CBullet::operator=(const CBullet & c)
{
    // TODO: insert return statement here
    m_bmBullet.LoadBitmapW(IDB_BULLET); 
    m_bmBullet.GetBitmap(&m_bmBulletInfo);
    m_bmSplash.LoadBitmapW(IDB_SPLASH);
    m_bmSplash.GetBitmap(&m_bmSplashInfo);
    m_bmHitSplash.LoadBitmapW(IDB_HITSPLASH);
    m_bmHitSplash.GetBitmap(&m_bmHitSplashInfo);
    m_Xpos = c.m_Xpos;
    m_Ypos = c.m_Ypos;
    m_nBoundary = c.m_nBoundary;
    m_bGoLeft = c.m_bGoLeft;
    m_nAniCount = c.m_nAniCount;
    m_bIsHit = c.m_bIsHit;

    return *this;
}


CBullet::~CBullet()
{
}

void CBullet::DrawBullet(CDC *pDC)
{
    CDC memDC;
    memDC.CreateCompatibleDC(pDC);

    //	Splash
    if (m_nAniCount) {
        CSize size;

        if (m_bIsHit) {
            size.cx = size.cy = 33;

            memDC.SelectObject(&m_bmHitSplash);
        }
        else {
            size.cx = 47;
            size.cy = 50;

            memDC.SelectObject(&m_bmSplash);
        }

        int animateX, animateY;

        switch (m_nAniCount) {
        case 1: animateX = 0; animateY = 0; break;
        case 2: animateX = m_bIsHit ? 33 : 47; animateY = 0; break;
        case 3: animateX = m_bIsHit ? 66 : 94; animateY = 0; break;
        case 4: animateX = 0; animateY = m_bIsHit ? 33 : 50; break;
        case 5: animateX = m_bIsHit ? 33 : 47; animateY = m_bIsHit ? 33 : 50; break;
        case 6: animateX = m_bIsHit ? 66 : 94; animateY = m_bIsHit ? 33 : 50; break;
        default: animateX = m_bIsHit ? 66 : 94; animateY = m_bIsHit ? 33 : 50; break;
        }
        if (m_bGoLeft) {
            CDC tmpDC;
            CBitmap tmpDCbmp;
            tmpDC.CreateCompatibleDC(pDC);
            tmpDCbmp.CreateCompatibleBitmap(pDC, size.cx, size.cy);
            tmpDC.SelectObject(&tmpDCbmp);

            tmpDC.StretchBlt(0, 0, size.cx, size.cy,
                &memDC, animateX + size.cx - 1, animateY, -size.cx, size.cy, SRCCOPY);
            pDC->TransparentBlt(m_Xpos, m_Ypos - 20, size.cx, size.cy,
                &tmpDC, 0, 0, size.cx, size.cy, RGB(0, 255, 255));
        }
        else {
            pDC->TransparentBlt(m_Xpos - 24, m_Ypos - 20, size.cx, size.cy,
                &memDC, animateX, animateY, size.cx, size.cy, RGB(0, 255, 255));
        }
    }
    //	Bullet
    else {
        memDC.SelectObject(&m_bmBullet);
        //	�������� �߻�� �Ѿ�
        if (m_bGoLeft) {
            CDC tmpDC;
            CBitmap tmpDCbmp;
            tmpDC.CreateCompatibleDC(pDC);
            tmpDCbmp.CreateCompatibleBitmap(pDC, m_bmBulletInfo.bmWidth, m_bmBulletInfo.bmHeight);
            tmpDC.SelectObject(&tmpDCbmp);

            tmpDC.StretchBlt(0, 0, m_bmBulletInfo.bmWidth, m_bmBulletInfo.bmHeight,
                &memDC, m_bmBulletInfo.bmWidth - 1, 0, -m_bmBulletInfo.bmWidth, m_bmBulletInfo.bmHeight, SRCCOPY);
            pDC->TransparentBlt(m_Xpos, m_Ypos, m_bmBulletInfo.bmWidth, m_bmBulletInfo.bmHeight,
                &tmpDC, 0, 0, m_bmBulletInfo.bmWidth, m_bmBulletInfo.bmHeight, RGB(0, 255, 255));
        }
        //	���������� �߻�� �Ѿ�
        else {
            pDC->TransparentBlt(m_Xpos, m_Ypos, m_bmBulletInfo.bmWidth, m_bmBulletInfo.bmHeight,
                &memDC, 0, 0, m_bmBulletInfo.bmWidth, m_bmBulletInfo.bmHeight, RGB(0, 255, 255));
        }
    }
}
