#include "CObjMomo.h"
#include <cmath> // fabsf を使うために追加

CObjMomo::CObjMomo()
{
    m_bOnLand = false;
    m_bOnGround = false;
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

bool CObjMomo::IsOnLand()
{
    return m_bOnLand;
}

bool CObjMomo::IsOnGround()
{
    return m_bOnGround;
}






