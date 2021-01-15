
// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "P2015305026.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define FRAME_SYNC_TIMER 0
#define KEY_STATE_TIMER 1
#define ANIMATION_TIMER 2
#define DAMAGE_COOLTIME_TIMER 3
#define AGENT_FLICKER_TIMER 4
#define FIRE_COOLTIME_TIMER 5
#define ALIEN_FLICKER_TIMER 6
#define ALIEN_FLICKER_DURATION_TIMER 7

#define AGENT_SPEED 6
#define ALIEN_SPEED 4
#define BULLET_SPEED 25
// CChildView

CChildView::CChildView()
{
    State = PLAYING;

    Agent.m_bmFigure.LoadBitmapW(IDB_AGENT);
    Agent.m_bmFigure.GetBitmap(&Agent.m_bmInfo);
    Agent.m_nHealth = 3;
    Agent.m_bDamageCoolTime = false;
    Agent.m_bDamageFlicker = false;
    Agent.m_size.cx = 95;
    Agent.m_size.cy = 65;
    Agent.m_Xpos = 150;
    Agent.m_Ypos = 1349;
    Agent.m_Xrange1 = Agent.m_Xrange2 = 0;
    Agent.m_bIsMoving = false;
    Agent.m_bLookAtLeft = Agent.m_bPrevLookAtLeft = false;
    Agent.m_ValidRect.SetRect(Agent.m_Xpos + 5, Agent.m_Ypos, Agent.m_Xpos + 60, Agent.m_Ypos + 65);
    Agent.m_bIsOnGround = true;
    Agent.m_currentState = STILL;
    Agent.m_nDeadAniCount = 0;

    m_bFireCoolTime = false;

    Alien.SetSize(4);
    for (int i = 0; i < 4; i++) {
        Alien[i] = new CFigure();
        Alien[i]->m_bmFigure.LoadBitmapW(IDB_ALIEN1);
        Alien[i]->m_bmFigure.GetBitmap(&Agent.m_bmInfo);
        Alien[i]->m_nHealth = 6;
        Alien[i]->m_bDamageCoolTime = false;
        Alien[i]->m_bDamageFlicker = false;
        Alien[i]->m_size.cx = 52;
        Alien[i]->m_size.cy = 63;
        Alien[i]->m_bIsMoving = true;
        Alien[i]->m_bLookAtLeft = Alien[i]->m_bPrevLookAtLeft = true;
        Alien[i]->m_bIsOnGround = true;
        Alien[i]->m_currentState = MOVE1;
        Alien[i]->m_nDeadAniCount = 0;
    }
    Alien[0]->m_Xpos = 1000;	Alien[0]->m_Ypos = 1189;
    Alien[0]->m_Xrange1 = 865 - 20;	Alien[0]->m_Xrange2 = 1297 + 20;

    Alien[1]->m_Xpos = 1130;	Alien[1]->m_Ypos = 649;
    Alien[1]->m_Xrange1 = 1026 - 20;	Alien[1]->m_Xrange2 = 1297 + 20;

    Alien[2]->m_Xpos = 3400;	Alien[2]->m_Ypos = 1243;
    Alien[2]->m_Xrange1 = 3130 - 20;	Alien[2]->m_Xrange2 = 3944 + 20;

    Alien[3]->m_Xpos = 2400;	Alien[3]->m_Ypos = 757;
    Alien[3]->m_Xrange1 = 2161 - 20;	Alien[3]->m_Xrange2 = 2647 + 20;

    bmHPbar.LoadBitmapW(IDB_HPBAR);
    bmHPbar.GetBitmap(&bmHPbarInfo);

    bmBackGround.LoadBitmapW(IDB_BG);
    bmBackGround.GetBitmap(&bmBackGroundInfo);

    bmTerrain.LoadBitmapW(IDB_TERRAIN);
    bmTerrain.GetBitmap(&bmTerrainInfo);

    velocityY = 0;
    gravity = 1;

    nGroundLevel = 1414;

    Camera.x = 0;
    Camera.y = 1111;

    //	Initialize Terrain
    rgnTerrain.CreateRectRgn(53, 1252, 486, 1414);
    rgnPartial[0].CreateRectRgn(486, 1144, 756, 1468);
    rgnPartial[1].CreateRectRgn(539, 928, 865, 1360);
    rgnPartial[2].CreateRectRgn(865, 1036, 1945, 1252);
    rgnPartial[3].CreateRectRgn(1349, 1252, 1459, 1360);
    rgnPartial[4].CreateRectRgn(1672, 820, 1945, 1036);
    rgnPartial[5].CreateRectRgn(1349, 172, 2268, 820);
    rgnPartial[6].CreateRectRgn(1945, 820, 2161, 874);
    rgnPartial[7].CreateRectRgn(918, 495, 1026, 766);
    rgnPartial[8].CreateRectRgn(1026, 495, 1349, 712);
    rgnPartial[9].CreateRectRgn(2268, 603, 2699, 820);
    rgnPartial[10].CreateRectRgn(2699, 172, 3186, 603);
    rgnPartial[11].CreateRectRgn(2699, 603, 3239, 928);
    rgnPartial[12].CreateRectRgn(2806, 657, 3294, 1036);
    rgnPartial[13].CreateRectRgn(2860, 1036, 3294, 1090);
    rgnPartial[14].CreateRectRgn(3022, 1090, 4050, 1198);
    rgnPartial[15].CreateRectRgn(3130, 1198, 4050, 1306);
    for (int i = 0; i < 16; i++) {
        rgnTerrain.CombineRgn(&rgnTerrain, &rgnPartial[i], RGN_OR);
    }
    wDeviceID = 0;
    szBGM = ".\\sfx\\bgm_audio.mp3";
    szEndingBGM = ".\\sfx\\Ending_audio.mp3";
    szShoot = ".\\sfx\\Shoot.wav";
    szBlow = ".\\sfx\\Blow.wav";

    bmYouDied.LoadBitmapW(IDB_YOUDIED);
    bmYouDied.GetBitmap(&bmYouDiedInfo);
    bmEnding.LoadBitmapW(IDB_ENDING);
    bmEnding.GetBitmap(&bmEndingInfo);

}

CChildView::~CChildView()
{
    for (int i = 0; i < Alien.GetSize(); i++)
        delete Alien[i];
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
    ON_WM_PAINT()
    ON_WM_CREATE()
    ON_WM_TIMER()
    ON_WM_DESTROY()
    ON_WM_KEYUP()
    ON_WM_KEYDOWN()
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs)
{
    if (!CWnd::PreCreateWindow(cs))
        return FALSE;

    cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
    cs.style &= ~WS_BORDER;
    cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
        ::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), NULL);

    return TRUE;
}

void CChildView::OnPaint()
{

    CPaintDC dc(this); // device context for painting

    CDC memDC;
    CDC mdcOffScreen;
    CBitmap bmpOffScreen;

    mdcOffScreen.CreateCompatibleDC(&dc);
    memDC.CreateCompatibleDC(&dc);
    bmpOffScreen.CreateCompatibleBitmap(&dc, bmTerrainInfo.bmWidth, bmTerrainInfo.bmHeight);
    mdcOffScreen.SelectObject(&bmpOffScreen);
#if 0
    //	Draw BG
    memDC.SelectObject(&bmBackGround);
    mdcOffScreen.StretchBlt(0, 0, bmTerrainInfo.bmWidth, bmTerrainInfo.bmHeight,
        &memDC, bkX - 400, 0, 600, bmBackGroundInfo.bmHeight, SRCCOPY);
#endif
#if 1
    //	Draw Terrain
    memDC.SelectObject(&bmTerrain);
    mdcOffScreen.BitBlt(0, 0, bmTerrainInfo.bmWidth, bmTerrainInfo.bmHeight,
        &memDC, 0, 0, SRCCOPY);
    //mdcOffScreen.TransparentBlt(0, 0, bmTerrainInfo.bmWidth, bmTerrainInfo.bmHeight,
        //&memDC, 0, 0, bmTerrainInfo.bmWidth, bmTerrainInfo.bmHeight, RGB(0, 255, 255));
#endif
    //	Draw Aliens
    for (int i = 0; i < Alien.GetSize(); i++) {
        if (Alien[i]->m_bDamageFlicker) {
            CFigure damagedAlien;
            damagedAlien.m_bmFigure.LoadBitmapW(IDB_ALIEN1_DAMAGED);
            damagedAlien.m_size = Alien[i]->m_size;
            damagedAlien.m_bLookAtLeft = Alien[i]->m_bLookAtLeft;
            damagedAlien.m_Xpos = Alien[i]->m_Xpos;
            damagedAlien.m_Ypos = Alien[i]->m_Ypos;
            damagedAlien.m_currentState = Alien[i]->m_currentState;
            damagedAlien.m_nHealth = Alien[i]->m_nHealth;
            damagedAlien.m_nDeadAniCount = Alien[i]->m_nDeadAniCount;
            damagedAlien.DrawFigure(&mdcOffScreen);
        }
        else
            Alien[i]->DrawFigure(&mdcOffScreen);
    }

    //	Draw Agent
    if (Agent.m_bDamageFlicker) {
        CFigure damagedAgent;
        damagedAgent.m_bmFigure.LoadBitmapW(IDB_AGENT_DAMAGED);
        damagedAgent.m_size = Agent.m_size;
        damagedAgent.m_bLookAtLeft = Agent.m_bLookAtLeft;
        damagedAgent.m_Xpos = Agent.m_Xpos;
        damagedAgent.m_Ypos = Agent.m_Ypos;
        damagedAgent.m_currentState = Agent.m_currentState;
        damagedAgent.m_nHealth = Agent.m_nHealth;
        damagedAgent.m_nDeadAniCount = Agent.m_nDeadAniCount;
        damagedAgent.DrawFigure(&mdcOffScreen);
    }
    else
        Agent.DrawFigure(&mdcOffScreen);

    //	Draw Bullets
    POSITION pos = Bullets.GetHeadPosition();
    while (pos != NULL) {
        Bullets.GetNext(pos).DrawBullet(&mdcOffScreen);
    }

    //	Draw HP bar
    memDC.SelectObject(&bmHPbar);
    mdcOffScreen.TransparentBlt(Camera.x + 44, Camera.y + 33, 84, 41,
        &memDC, 0, Agent.m_nHealth <= 0 ? 0 : Agent.m_nHealth * 41, 84, 41, RGB(0, 255, 255));

    if (State == YOUDIED) {
        memDC.SelectObject(&bmYouDied);
        mdcOffScreen.BitBlt(Camera.x + 136, Camera.y + 117, bmYouDiedInfo.bmWidth, bmYouDiedInfo.bmHeight,
            &memDC, 0, 0, SRCCOPY);
    }
    else if (State == ENDING) {
        memDC.SelectObject(&bmEnding);
        mdcOffScreen.BitBlt(Camera.x, Camera.y, bmEndingInfo.bmWidth, bmEndingInfo.bmHeight,
            &memDC, 0, 0, SRCCOPY);
    }
#ifdef DEBUG
    CPen pen(PS_SOLID, 1, RGB(255, 0, 0));
    mdcOffScreen.SelectObject(&pen);
    mdcOffScreen.SelectStockObject(NULL_BRUSH);
    mdcOffScreen.Rectangle(&Agent.m_ValidRect);
    CRect agentRect(CPoint(Agent.m_Xpos, Agent.m_Ypos), Agent.m_size);
    mdcOffScreen.Rectangle(&agentRect);
    //mdcOffScreen.Rectangle(&Alien[0]->m_ValidRect);
    CBrush brush(RGB(0, 0, 255));
    CBrush brush2(RGB(255, 0, 0));
    mdcOffScreen.FrameRgn(&rgnTerrain, &brush, 1, 1);

#endif

    dc.BitBlt(0, 0, 960, 540, &mdcOffScreen, Camera.x, Camera.y, SRCCOPY);

    // Do not call CWnd::OnPaint() for painting messages
}



int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  Add your specialized creation code here
    SetTimer(FRAME_SYNC_TIMER, 16, NULL);	//	60Frame Sync
    SetTimer(KEY_STATE_TIMER, 1, NULL);		//	KeyState
    SetTimer(ANIMATION_TIMER, 50, NULL);	//	Animation

    LoadWAV(NULL, szBGM);		//	1
    LoadWAV(NULL, szEndingBGM);	//	2
    mciSendCommand(1, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)&mciPlayParms);
    return 0;
}


void CChildView::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: Add your message handler code here and/or call default
    POSITION pos;

    switch (nIDEvent) {
    case FRAME_SYNC_TIMER:
        //	60Frame Sync, Camera Movement
        InvalidateRect(NULL, FALSE);
        if ((Agent.m_ValidRect.left - 460) <= 0
            || (Agent.m_ValidRect.right + 460) >= 3990) {
            Camera.y = Agent.m_ValidRect.top - 237;
        }
        else if ((Agent.m_ValidRect.top - 237) <= 0
            || (Agent.m_ValidRect.bottom + 237) >= 1720) {
            Camera.x = Agent.m_ValidRect.left - 452;
        }
        else {
            Camera.x = Agent.m_ValidRect.left - 452;
            Camera.y = Agent.m_ValidRect.top - 237;
        }

        break;
    case KEY_STATE_TIMER:
        //	ValidRect ����
        if (Agent.m_bLookAtLeft)
            Agent.m_ValidRect.SetRect(Agent.m_Xpos + 43, Agent.m_Ypos, Agent.m_Xpos + 82, Agent.m_Ypos + 65);
        else
            Agent.m_ValidRect.SetRect(Agent.m_Xpos + 13, Agent.m_Ypos, Agent.m_Xpos + 52, Agent.m_Ypos + 65);

        for (int i = 0; i < Alien.GetSize(); i++) {
            Alien[i]->m_ValidRect.SetRect(Alien[i]->m_Xpos, Alien[i]->m_Ypos, Alien[i]->m_Xpos + 52, Alien[i]->m_Ypos + 63);
        }

        //////////////////////////////////////////
        //		BULLETS Collision Process		//
        //////////////////////////////////////////

        //	���� Alien�� �浹 üũ
        pos = Bullets.GetHeadPosition();
        while (pos != NULL) {
            CBullet &bullet = Bullets.GetAt(pos);
            POSITION prevPos = pos;
            Bullets.GetNext(pos);
            CRect bulletRect(CPoint(bullet.m_Xpos, bullet.m_Ypos), CSize(23, 9));
            for (int i = 0; i < Alien.GetSize(); i++) {
                if (bullet.m_bIsHit || Alien[i]->m_ValidRect.PtInRect(bulletRect.CenterPoint())) {
                    //	First Hit!
                    if (bullet.m_bIsHit == false) {
                        Alien[i]->m_nHealth--;
                        iAlienFlickerIndex = i;
                        SetTimer(ALIEN_FLICKER_TIMER, 70, NULL);
                        SetTimer(ALIEN_FLICKER_DURATION_TIMER, 650, NULL);
                    }
                    bullet.m_bIsHit = true;
                    if (bullet.m_nAniCount >= 6) {
                        Bullets.RemoveAt(prevPos);
                        break;
                    }
                }
            }	// End Check every Aliens
        }
        //	���̶� �浹 üũ
        pos = Bullets.GetHeadPosition();
        while (pos != NULL) {
            CBullet &bullet = Bullets.GetAt(pos);
            //	Alien�� �浹�� �Ѿ��� ���⼭ ó������ ����.
            if (!bullet.m_bIsHit) {
                bullet.m_Xpos += bullet.m_bGoLeft ? -BULLET_SPEED : BULLET_SPEED;

                //	������ ���ϴ� �Ѿ��� ���
                if (bullet.m_bGoLeft) {
                    if (bullet.m_Xpos <= bullet.m_nBoundary) {
                        bullet.m_Xpos = bullet.m_nBoundary;
                        if (bullet.m_nAniCount >= 6) {
                            POSITION removePos = pos;
                            Bullets.GetNext(pos);
                            Bullets.RemoveAt(removePos);
                            continue;
                        }
                    }
                }
                //	�������� ���ϴ� �Ѿ��� ���
                else {
                    if (bullet.m_Xpos + 23 >= bullet.m_nBoundary) {
                        bullet.m_Xpos = bullet.m_nBoundary - 23;
                        if (bullet.m_nAniCount >= 6) {
                            POSITION removePos = pos;
                            Bullets.GetNext(pos);
                            Bullets.RemoveAt(removePos);
                            continue;
                        }
                    }
                }
            }
            Bullets.GetNext(pos);
        }	//	End Check every Bullets

            //	���� ó��, �߷�
            //	���鿡 ����ִٸ�
        if (Agent.m_bIsOnGround) {
            //	x : 1672 ~ 1945 ���� �� ���� ����
            if (!(nGroundLevel == 928 && Agent.m_ValidRect.left >= 1672 && Agent.m_ValidRect.left <= 1890
                || nGroundLevel == 1036 && Agent.m_ValidRect.right >= 1783 && Agent.m_ValidRect.right <= 1945
                || nGroundLevel == 1144 && Agent.m_ValidRect.right >= 1730 && Agent.m_ValidRect.left <= 1890))
            {
                CRgn rgnValidRect;
                rgnValidRect.CreateRectRgn(Agent.m_ValidRect.left, Agent.m_ValidRect.top, Agent.m_ValidRect.right, Agent.m_ValidRect.bottom);
                rgnValidRect.OffsetRgn(0, 1);
                rgnValidRect.CombineRgn(&rgnValidRect, &rgnTerrain, RGN_OR);
                if (rgnValidRect.EqualRgn(&rgnTerrain)) {
                    Agent.m_bIsOnGround = false;
                }
            }
        }
        //	���߿� �ִ� ���
        else {
            //	x : 1672 ~ 1945
            if (Agent.m_ValidRect.bottom <= 928 && Agent.m_ValidRect.left >= 1672 && Agent.m_ValidRect.left <= 1890)
                nGroundLevel = 928;
            else if (Agent.m_ValidRect.bottom <= 1036 && Agent.m_ValidRect.right >= 1783 && Agent.m_ValidRect.right <= 1945)
                nGroundLevel = 1036;
            else if (Agent.m_ValidRect.bottom <= 1144 && Agent.m_ValidRect.right >= 1730 && Agent.m_ValidRect.left <= 1890)
                nGroundLevel = 1144;

            else {
                CRgn rgnValidRect, rgnCombined;
                CRect tmpRect;
                rgnValidRect.CreateRectRgn(Agent.m_ValidRect.left, Agent.m_ValidRect.top, Agent.m_ValidRect.right, Agent.m_ValidRect.bottom);
                rgnCombined.CreateRectRgn(0, 0, 1, 1);
                //	��� ��
                if (velocityY < 0) {
                    rgnValidRect.OffsetRgn(0, -1);
                    rgnCombined.CombineRgn(&rgnValidRect, &rgnTerrain, RGN_OR);
                    if (!rgnCombined.EqualRgn(&rgnTerrain)) {
                        velocityY = 1;
                    }
                }
                //	�������� ��
                else {
                    do {
                        rgnValidRect.OffsetRgn(0, 1);
                        rgnCombined.CombineRgn(&rgnValidRect, &rgnTerrain, RGN_OR);
                    } while (rgnCombined.EqualRgn(&rgnTerrain));
                    rgnValidRect.GetRgnBox(&tmpRect);
                    nGroundLevel = tmpRect.bottom - 1;
                }
            }
            if (velocityY < 15) velocityY += gravity;
            Agent.m_Ypos += velocityY;
        }

        //	Groundlevel�� �����ϸ�
        if (Agent.m_Ypos > nGroundLevel - 65) {
            Agent.m_Ypos = nGroundLevel - 65;	//	�ٴ��� y��ǥ
            velocityY = 0;
            Agent.m_bIsOnGround = true;
        }


        //	���� ����Ű ������ ��
        if ((GetAsyncKeyState(VK_LEFT) & 0x8000) && State == PLAYING) {
            Agent.m_bIsMoving = true;
            Agent.m_bLookAtLeft = true;
            CRgn rgnValidRect;
            rgnValidRect.CreateRectRgn(Agent.m_ValidRect.left, Agent.m_ValidRect.top, Agent.m_ValidRect.right, Agent.m_ValidRect.bottom);
            //	���� ���� �������� �ٶ󺸰� �־��ٸ�
            if (!Agent.m_bPrevLookAtLeft) {
                Agent.m_Xpos -= 30;	//	AgentBox ��ġ ����
                Agent.m_bPrevLookAtLeft = true;
            }
            //	�浹 ���� -> �̵��ӵ� ��ŭ�� ���� �̸� ���� ���� �Ѿ���� �Ǻ�
            rgnValidRect.OffsetRgn(-AGENT_SPEED, 0);
            rgnValidRect.CombineRgn(&rgnValidRect, &rgnTerrain, RGN_OR);

            if (rgnValidRect.EqualRgn(&rgnTerrain)) {
                Agent.m_Xpos -= AGENT_SPEED;
            }
        }
        //	������ ����Ű ������ ��
        else if ((GetAsyncKeyState(VK_RIGHT) & 0x8000) && State == PLAYING) {
            Agent.m_bIsMoving = true;
            Agent.m_bLookAtLeft = false;
            CRgn rgnValidRect;
            rgnValidRect.CreateRectRgn(Agent.m_ValidRect.left, Agent.m_ValidRect.top, Agent.m_ValidRect.right, Agent.m_ValidRect.bottom);
            //	���� ���� ������ �ٶ󺸰� �־��ٸ�
            if (Agent.m_bPrevLookAtLeft) {
                Agent.m_Xpos += 30;	//	AgentBox ��ġ ����
                Agent.m_bPrevLookAtLeft = false;
            }
            //	�浹 ����
            rgnValidRect.OffsetRgn(AGENT_SPEED, 0);
            rgnValidRect.CombineRgn(&rgnValidRect, &rgnTerrain, RGN_OR);

            if (rgnValidRect.EqualRgn(&rgnTerrain)) {
                Agent.m_Xpos += AGENT_SPEED;
            }
        }
        //	�����̽��� ������ ��
        if ((GetAsyncKeyState(VK_SPACE) & 0x8000) && State == PLAYING) {
            if (Agent.m_bIsOnGround) {
                velocityY = -17;
                Agent.m_bIsOnGround = false;
            }
        }
        //	��Ÿ���� �ƴϰ� ����Ű ������ ��
        if ((GetAsyncKeyState('Z') & 0x8000) && !m_bFireCoolTime && State == PLAYING) {
            m_bFireCoolTime = true;
            //	Cheat Bullet Infinity
            if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
                SetTimer(FIRE_COOLTIME_TIMER, 70, NULL);
            }
            else {
                SetTimer(FIRE_COOLTIME_TIMER, 700, NULL);
            }
            CRgn rgnAgentRect;
            rgnAgentRect.CreateRectRgn(Agent.m_Xpos, Agent.m_Ypos, Agent.m_Xpos + 95, Agent.m_Ypos + 65);
            //	���� �ٶ󺸰� ���� ���
            if (Agent.m_bLookAtLeft) {
                rgnAgentRect.OffsetRgn(-23, 0);
                rgnAgentRect.CombineRgn(&rgnAgentRect, &rgnTerrain, RGN_OR);
                if (rgnAgentRect.EqualRgn(&rgnTerrain)) {
                    PlaySound(szShoot, NULL, SND_FILENAME | SND_ASYNC);
                    //	�������� ���� �Ѿ� ����
                    CRgn rgnBullet, rgnCombined;
                    CRect tmpRect;
                    rgnCombined.CreateRectRgn(0, 0, 1, 1);
                    rgnBullet.CreateRectRgn(Agent.m_Xpos - 22, Agent.m_Ypos + 26, Agent.m_Xpos + 1, Agent.m_Ypos + 35);
                    //	�Ѿ��� �ε����� ���� �˾Ƴ���
                    do {
                        rgnBullet.OffsetRgn(-1, 0);
                        rgnCombined.CombineRgn(&rgnBullet, &rgnTerrain, RGN_OR);
                    } while (rgnCombined.EqualRgn(&rgnTerrain));
                    rgnBullet.GetRgnBox(&tmpRect);
                    int nBoundary = tmpRect.left + 2;

                    Bullets.AddTail(CBullet(Agent.m_Xpos - 22, Agent.m_Ypos + 26, nBoundary, true));
                }
            }
            //	������ �ٶ󺸰� ���� ���
            else {
                rgnAgentRect.OffsetRgn(23, 0);
                rgnAgentRect.CombineRgn(&rgnAgentRect, &rgnTerrain, RGN_OR);
                if (rgnAgentRect.EqualRgn(&rgnTerrain)) {
                    PlaySound(szShoot, NULL, SND_FILENAME | SND_ASYNC);
                    //	���������� ���� �Ѿ� ����
                    CRgn rgnBullet, rgnCombined;
                    CRect tmpRect;
                    rgnCombined.CreateRectRgn(0, 0, 1, 1);
                    rgnBullet.CreateRectRgn(Agent.m_Xpos + 94, Agent.m_Ypos + 26, Agent.m_Xpos + 1, Agent.m_Ypos + 35);
                    //	�Ѿ��� �ε����� ���� �˾Ƴ���
                    do {
                        rgnBullet.OffsetRgn(1, 0);
                        rgnCombined.CombineRgn(&rgnBullet, &rgnTerrain, RGN_OR);
                    } while (rgnCombined.EqualRgn(&rgnTerrain));
                    rgnBullet.GetRgnBox(&tmpRect);
                    int nBoundary = tmpRect.right - 1;

                    Bullets.AddTail(CBullet(Agent.m_Xpos + 94, Agent.m_Ypos + 26, nBoundary, false));
                }
            }

        }
        //////////////////////////////
        //		Aliens Moving		//
        //////////////////////////////
        for (int i = 0; i < Alien.GetSize(); i++) {
            if (Alien[i]->m_nHealth > 0) {
                if (Alien[i]->m_Xpos <= Alien[i]->m_Xrange1) {
                    Alien[i]->m_bLookAtLeft = false;
                }
                else if (Alien[i]->m_Xpos >= Alien[i]->m_Xrange2) {
                    Alien[i]->m_bLookAtLeft = true;
                }
                Alien[i]->m_Xpos += Alien[i]->m_bLookAtLeft ? -ALIEN_SPEED : ALIEN_SPEED;
            }
        }

        //	Agent Getting Damaged
        if (!Agent.m_bDamageCoolTime) {
            CRect rectInter;
            for (int i = 0; i < Alien.GetSize(); i++) {
                rectInter.IntersectRect(Agent.m_ValidRect, Alien[i]->m_ValidRect);
                //	Alien�� ����� �� OR ���ÿ� �������� ��
                if (!rectInter.IsRectEmpty()
                    || (Agent.m_ValidRect.bottom == 1360 && Agent.m_ValidRect.left >= 1349 && Agent.m_ValidRect.right <= 1459)) {
                    Agent.m_bDamageCoolTime = true;
                    SetTimer(DAMAGE_COOLTIME_TIMER, 1000, NULL);
                    SetTimer(AGENT_FLICKER_TIMER, 70, NULL);
                    Agent.m_nHealth--;
                    break;
                }
            }
        }

        //////////////////////////
        //		Game Clear		//
        //////////////////////////
        if (Agent.m_Xpos >= 3785) {
            State = ENDING;
            mciSendCommand(1, MCI_SEEK, MCI_SEEK_TO_START, (DWORD)(LPVOID)NULL);
            mciSendCommand(2, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)&mciPlayParms);
            
        }
        //////////////////////
        //		Death		//
        //////////////////////
        for (int i = 0; i < Alien.GetSize(); i++) {
            if (Alien[i]->m_nHealth <= 0 && Alien[i]->m_nDeadAniCount > 6) {
                PlaySound(szBlow, NULL, SND_FILENAME | SND_ASYNC);
                delete Alien[i];
                Alien.RemoveAt(i);
                KillTimer(ALIEN_FLICKER_TIMER);
                KillTimer(ALIEN_FLICKER_DURATION_TIMER);
            }
        }
        //	Agent Dead
        if (Agent.m_nHealth <= 0 && Agent.m_nDeadAniCount > 6 && State == PLAYING) {
            PlaySound(szBlow, NULL, SND_FILENAME | SND_ASYNC);
            State = YOUDIED;
        }

        break;

    case ANIMATION_TIMER:
        //////////////////////
        //		Agent		//
        //////////////////////
        if (Agent.m_nHealth > 0) {
            //	���� ���߿� ���ִٸ�
            if (!Agent.m_bIsOnGround) {
                if (velocityY <= 2)
                    Agent.m_currentState = RISING;
                else
                    Agent.m_currentState = FALLING;
            }
            //	���� ���߿� �������ʰ� �����̴� ���¶��
            else if (Agent.m_bIsMoving) {
                switch (Agent.m_currentState) {
                case STILL:	Agent.m_currentState = MOVE1; break;
                case MOVE1:	Agent.m_currentState = MOVE2; break;
                case MOVE2:	Agent.m_currentState = MOVE3; break;
                case MOVE3:	Agent.m_currentState = MOVE4; break;
                case MOVE4:	Agent.m_currentState = MOVE5; break;
                case MOVE5:	Agent.m_currentState = MOVE6; break;
                case RISING:
                case FALLING:
                case MOVE6:	Agent.m_currentState = MOVE1; break;
                }
            }
            //	��������
            else {
                Agent.m_currentState = STILL;
            }
        }
        else {
            Agent.m_nDeadAniCount++;
        }

        //////////////////////
        //		Aliens		//
        //////////////////////
        for (int i = 0; i < Alien.GetSize(); i++) {
            if (Alien[i]->m_nHealth > 0) {
                switch (Alien[i]->m_currentState) {
                case MOVE1:	Alien[i]->m_currentState = MOVE2; break;
                case MOVE2:	Alien[i]->m_currentState = MOVE3; break;
                case MOVE3:	Alien[i]->m_currentState = MOVE4; break;
                case MOVE4:	Alien[i]->m_currentState = MOVE5; break;
                case MOVE5:	Alien[i]->m_currentState = MOVE6; break;
                case MOVE6: Alien[i]->m_currentState = MOVE1; break;
                }
            }
            else {
                Alien[i]->m_nDeadAniCount++;
            }
        }

        //////////////////////
        //		Bullets		//
        //////////////////////
        pos = Bullets.GetHeadPosition();
        while (pos != NULL) {
            CBullet &bullet = Bullets.GetNext(pos);
            if (bullet.m_bIsHit) {
                bullet.m_nAniCount++;
                continue;
            }
            if (bullet.m_bGoLeft) {
                if (bullet.m_Xpos <= bullet.m_nBoundary) {
                    bullet.m_nAniCount++;
                }
            }
            else {
                if (bullet.m_Xpos + 23 >= bullet.m_nBoundary) {
                    bullet.m_nAniCount++;
                }
            }
        }

        break;
    case DAMAGE_COOLTIME_TIMER:
        Agent.m_bDamageCoolTime = false;
        Agent.m_bDamageFlicker = false;
        KillTimer(DAMAGE_COOLTIME_TIMER);
        KillTimer(AGENT_FLICKER_TIMER);
        break;
    case AGENT_FLICKER_TIMER:
        Agent.m_bDamageFlicker = !Agent.m_bDamageFlicker;
        break;
    case FIRE_COOLTIME_TIMER:
        m_bFireCoolTime = false;
        KillTimer(FIRE_COOLTIME_TIMER);
        break;
    case ALIEN_FLICKER_TIMER:
        Alien[iAlienFlickerIndex]->m_bDamageFlicker = !Alien[iAlienFlickerIndex]->m_bDamageFlicker;
        break;
    case ALIEN_FLICKER_DURATION_TIMER:
        Alien[iAlienFlickerIndex]->m_bDamageFlicker = false;
        KillTimer(ALIEN_FLICKER_TIMER);
        KillTimer(ALIEN_FLICKER_DURATION_TIMER);
        break;
    }

    CWnd::OnTimer(nIDEvent);
}


void CChildView::OnDestroy()
{
    CWnd::OnDestroy();

    // TODO: Add your message handler code here
    //mciSendCommand(1, MCI_CLOSE, 0, (DWORD)(LPVOID)NULL);
    mciSendCommand(1, MCI_CLOSE, 0, (DWORD)(LPVOID)NULL);
    KillTimer(FRAME_SYNC_TIMER);
    KillTimer(KEY_STATE_TIMER);
    KillTimer(ANIMATION_TIMER);
}


void CChildView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // TODO: Add your message handler code here and/or call default
    switch (nChar) {
    case VK_LEFT:
    case VK_RIGHT:
        Agent.m_bIsMoving = false;
        break;
    }
    CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CChildView::InitializeObjects()
{
    Agent.m_nHealth = 3;
    Agent.m_bDamageCoolTime = false;
    Agent.m_bDamageFlicker = false;
    Agent.m_size.cx = 95;
    Agent.m_size.cy = 65;
    Agent.m_Xpos = 150;
    Agent.m_Ypos = 1349;
    Agent.m_Xrange1 = Agent.m_Xrange2 = 0;
    Agent.m_bIsMoving = false;
    Agent.m_bLookAtLeft = Agent.m_bPrevLookAtLeft = false;
    Agent.m_ValidRect.SetRect(Agent.m_Xpos + 5, Agent.m_Ypos, Agent.m_Xpos + 60, Agent.m_Ypos + 65);
    Agent.m_bIsOnGround = true;
    Agent.m_currentState = STILL;
    Agent.m_nDeadAniCount = 0;

    m_bFireCoolTime = false;

    for (int i = 0; i < Alien.GetSize(); i++) {
        delete Alien[i];
    }
    Alien.RemoveAll();
    Alien.SetSize(4);
    for (int i = 0; i < 4; i++) {
        Alien[i] = new CFigure();
        Alien[i]->m_bmFigure.LoadBitmapW(IDB_ALIEN1);
        Alien[i]->m_bmFigure.GetBitmap(&Agent.m_bmInfo);
        Alien[i]->m_nHealth = 6;
        Alien[i]->m_bDamageCoolTime = false;
        Alien[i]->m_bDamageFlicker = false;
        Alien[i]->m_size.cx = 52;
        Alien[i]->m_size.cy = 63;
        Alien[i]->m_bIsMoving = true;
        Alien[i]->m_bLookAtLeft = Alien[i]->m_bPrevLookAtLeft = true;
        Alien[i]->m_bIsOnGround = true;
        Alien[i]->m_currentState = MOVE1;
        Alien[i]->m_nDeadAniCount = 0;
    }
    Alien[0]->m_Xpos = 1000;	Alien[0]->m_Ypos = 1189;
    Alien[0]->m_Xrange1 = 865 - 20;	Alien[0]->m_Xrange2 = 1297 + 20;

    Alien[1]->m_Xpos = 1130;	Alien[1]->m_Ypos = 649;
    Alien[1]->m_Xrange1 = 1026 - 20;	Alien[1]->m_Xrange2 = 1297 + 20;

    Alien[2]->m_Xpos = 3400;	Alien[2]->m_Ypos = 1243;
    Alien[2]->m_Xrange1 = 3130 - 20;	Alien[2]->m_Xrange2 = 3944 + 20;

    Alien[3]->m_Xpos = 2400;	Alien[3]->m_Ypos = 757;
    Alien[3]->m_Xrange1 = 2161 - 20;	Alien[3]->m_Xrange2 = 2647 + 20;

    velocityY = 0;
    gravity = 1;

    nGroundLevel = 1414;

    Camera.x = 0;
    Camera.y = 1111;
}


void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // TODO: Add your message handler code here and/or call default
    switch (State) {
    case YOUDIED:
    case ENDING:
        if (nChar == VK_RETURN || nChar == VK_SPACE) {
            mciSendCommand(2, MCI_SEEK, MCI_SEEK_TO_START, (DWORD)(LPVOID)NULL);
            mciSendCommand(1, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)&mciPlayParms);
            InitializeObjects();
            State = PLAYING;
        }
        break;
    }
    CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}
