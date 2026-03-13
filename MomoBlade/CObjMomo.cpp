#include "CObjMomo.h"
#include "CObjSword.h"


CObjMomo::CObjMomo()
{
    m_bOnLand = false;
    m_bOnGround = false;
}

// 剣初期化
void CObjMomo::InitSword(float fx, float fy, float fvx, float fvy, const char* pchImgL, const char* pchImgR)
{
    m_ObjSword.SetParameter(0.0f, 0.0f, 0.0f, 0.0f, pchImgR);
    m_ObjSword.SetImages(pchImgL, pchImgR);
    m_ObjSword.SetDirection(DIRECTION_R);
    m_ObjSword.SetSwordLength(m_ObjSword.GetWidth() * 1.41);
    m_ObjSword.SetSwordAngle(0.0f);
}

// HP初期化
void CObjMomo::InitHP(float fx, float fy, const char *pchImg, vector<filesystem::path> &pathList)
{
    m_ObjHP.SetParameter(fx, fy, 0.0f, 0.0f, pchImg);
    m_nHPX = fx;
    m_nHPY = fy;
    m_nHPW = m_ObjHP.GetWidth();
    m_nHP = HP_MAX * 4;

    int nCount = pathList.size();
    m_nImgHP.resize(nCount);

    for (int i = 0; i < nCount; i++) {
        string strPath = pathList[i].string();
        m_nImgHP[i] = LoadGraph(strPath.c_str());
    }
}

void CObjMomo::UpdateHP()
{
    if (m_bDamaged && m_nTimer == 0) {
        m_nHP--;
    }
}

void CObjMomo::DrawSword(int nCameraX)
{
    float fSwordX = m_fx - nCameraX + m_nWidth + 10;
    float fSwordY = m_fy - 20;
    if (m_nDirection == DIRECTION_L) {
        fSwordX = m_fx - nCameraX - 40;
    }

    m_ObjSword.SetX(fSwordX);
    m_ObjSword.SetY(fSwordY);
    m_ObjSword.SetDirection(m_nDirection);
    m_ObjSword.DrawSword();
}

void CObjMomo::DrawHP()
{
    int nFullHP = m_nHP / 4;
    int nLeftHP = m_nHP % 4;
    int nNonHP = (HP_MAX * 4 - m_nHP) / 4;

    int i = 0;
    // フルHP
    for (i = 0; i < nFullHP; i++) {
        DrawGraph(m_nHPX + m_nHPW * i, m_nHPY, m_nImgHP[4], TRUE);
    }

    // 
    if (nLeftHP > 0) {
        DrawGraph(m_nHPX + m_nHPW * i, m_nHPY, m_nImgHP[nLeftHP], TRUE);
        i++;
    }

    for (int j = 0; j < nNonHP; j++) {
        DrawGraph(m_nHPX + m_nHPW * (i + j), m_nHPY, m_nImgHP[0], TRUE);
    }
}

// Y方向の速度管理
void CObjMomo::CalcMomoVy()
{
    // ジャンプUP中
    if (m_fy > m_fDestY) {
        m_fvy -= JUMP_UP_POWER;
    }
    // ジャンプDOWN中
    else {
        m_fvy += GRAVITEY;
    }
}

void CObjMomo::MoveMomo(int nDirection)
{
    if (nDirection == DIRECTION_L) {
        m_fx -= m_fvx;
        if (m_fx < 0) {
            m_fx = 0.0f;
        }
    }
    else {
        m_fx += m_fvx;

    }
    
    SetDirection(nDirection);
}

void CObjMomo::CalcSwordTipXY(float* pfx, float* pfy)
{
    m_ObjSword.CalcSwordTipXY(pfx, pfy);
}

// モモがオブジェクトの上に乗っているか判定
bool CObjMomo::IsMomoInRangeObjectX(CObject Obj)
{
    if (Obj.GetX() <= m_fx + m_nWidth && m_fx <= Obj.GetX() + Obj.GetWidth()) {
        return true;
    }

    return false;
}

// モモ(矩形)と敵(矩形)のヒットチェック
// 矩形どうしの重なり判定
bool CObjMomo::AABBOverlap(CObject ObjEnemy)
{
    float fx = ObjEnemy.GetX();
    float fy = ObjEnemy.GetY();
    int nw = ObjEnemy.GetWidth();
    int nh = ObjEnemy.GetHeight();

    float dx = fabsf((m_fx + m_nWidth * 0.5f) - (fx + nw * 0.5f));
    float dy = fabsf((m_fy + m_nHeight * 0.5f) - (fy + nh * 0.5f));
    if ((dx <= (m_nWidth * 0.5f + nw * 0.5f)) && (dy <= (m_nHeight * 0.5f + nh * 0.5f))) {
        return true;
    }

    return false;
}

void CObjMomo::SetDestY(float fDestY)
{
    m_fDestY = fDestY;
}

float CObjMomo::GetDestY()
{
    return m_fDestY;
}

void CObjMomo::SetOnLand(bool bOnLand)
{
    m_bOnLand = bOnLand;
}

void CObjMomo::SetOnGround(bool bOnGround)
{
    m_bOnGround = bOnGround;
}

void CObjMomo::SetIsAttacking(bool bAttacking)
{
    m_ObjSword.SetAttack(bAttacking);
}

bool CObjMomo::IsOnLand()
{
    return m_bOnLand;
}

bool CObjMomo::IsOnGround()
{
    return m_bOnGround;
}

bool CObjMomo::IsAttacking()
{
    return m_ObjSword.IsAttack();
}





