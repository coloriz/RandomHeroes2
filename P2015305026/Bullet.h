#pragma once
class CBullet
{
public:
    CBullet() { }
    CBullet(int x, int y, int nBoundary, bool goLeft);
    CBullet(const CBullet& c);
    CBullet& operator=(const CBullet& c);
    ~CBullet();
    void DrawBullet(CDC * pDC);
public:
    CBitmap m_bmBullet, m_bmSplash, m_bmHitSplash;
    BITMAP m_bmBulletInfo, m_bmSplashInfo, m_bmHitSplashInfo;
    int m_Xpos;
    int m_Ypos;
    int m_nBoundary;
    bool m_bGoLeft;
    int m_nAniCount;
    bool m_bIsHit;
};

