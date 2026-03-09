#include "CObjEnemy2.h"

CObjEnemy2::CObjEnemy2()
{
}

void CObjEnemy2::InitWeapon(const char* pchImg1, const char* pchImg2)
{
    // 弓
    m_ObjBow.SetParameter(0.0f, 0.0f, 0.0f, 0.0f, pchImg1);
    m_ObjBow.SetX(m_fx - m_ObjBow.GetWidth() - 10);
    m_ObjBow.SetY(m_fy + m_nHeight / 2 - (m_ObjBow.GetHeight() / 2));

    // 矢
    m_ObjArrow.SetParameter(0.0f, 0.0f, 3.0f, 3.0f, pchImg2);
    m_ObjArrow.SetX(m_ObjBow.GetX());
    m_ObjArrow.SetY(m_ObjBow.GetY() + (m_ObjBow.GetHeight() / 2.0) - (m_ObjArrow.GetHeight() / 2.0));
}

float CObjEnemy2::CalcAngle(float fx, float fy, float ex, float ey)
{
    float dx = ex - fx;
    float dy = ey - fy;

    return atan2(dy, dx);
}

void CObjEnemy2::SetArrowRange(float fRangeX, float fRangeY)
{
    m_fArrowRngX = fRangeX;
    m_fArrowRngY = fRangeY;
}

void CObjEnemy2::Update(CObject* pTarget)
{
    // Updateで装填
    // Attackで矢の座標更新
    // Drawで描画

    CObjEnemy::Update(pTarget);

    m_dAngle = 0.0f;

    // モモがいる側を向く
    if (m_bDetect) {
        float fTargetX = pTarget->GetX() + pTarget->GetWidth() / 2;
        float fTargetY = pTarget->GetY() + pTarget->GetHeight() / 2;
        if (m_fx >= fTargetX) {
            SetDirection(DIRECTION_L);
        }
        else {
            SetDirection(DIRECTION_R);
        }

        // モモがいる角度を計算
        m_dAngle = CalcAngle(fTargetX, fTargetY, m_ObjBow.GetX(), m_ObjBow.GetY());

        // 矢を装填
        Loading(fTargetX, fTargetY);
    }
}

// 矢を装填
void CObjEnemy2::Loading(float fx, float fy)
{
    m_nFireTimer++;
    if (m_nFireTimer >= m_nFireInterval) {
        m_nFireTimer = 0;
        float sx = m_ObjBow.GetX() + m_ObjBow.GetWidth() / 2;
        float sy = m_ObjBow.GetY() + m_ObjBow.GetHeight() / 2;

        float dx = fx - sx;
        float dy = fy - sy;
        float len = sqrt(dx * dx + dy * dy);
        if (len <= 0.0001f) return;
        
        float vx = (dx / len) * m_fArrowSpd;
        float vy = (dy / len) * m_fArrowSpd;
        float angle = atan2(vy, vx);

        ARROW arrow;
        arrow.fx = sx;
        arrow.fy = sy;
        arrow.fvx = vx;
        arrow.fvy = vy;
        arrow.fAngle = angle;
        m_Arrows.push_back(arrow);
    }
}

void CObjEnemy2::Attack(CObject* pTarget)
{
    // m_Arrows内の矢を1本ずつ取り出して位置を更新
    for (int i = (int)m_Arrows.size() - 1; i >= 0; --i) {
        ARROW& a = m_Arrows[i];
        a.fx += a.fvx;
        a.fy += a.fvy;

        // 矢がステージ外に出たら削除
        if (a.fx < -100 || a.fx > m_fArrowRngX || a.fy < -100 || a.fy > m_fArrowRngY) {
            m_Arrows.erase(m_Arrows.begin() + i);
            continue;
        }

        // モモへのヒットチェック
        float left = pTarget->GetX();
        float right = left + pTarget->GetWidth();
        float top = pTarget->GetY();
        float bottom = top + pTarget->GetHeight();
        if (left <= a.fx && a.fx <= right) {
            if (top <= a.fy && a.fy <= bottom) {
                pTarget->SetDamaged(true);
            }
        }
    }
}

void CObjEnemy2::Draw(int nCameraX)
{
	CObjEnemy::Draw(nCameraX);
    
    // 弓矢描画
    DrawRotaGraph(m_ObjBow.GetX() - nCameraX + m_ObjBow.GetWidth() / 2, m_ObjBow.GetY() + m_ObjBow.GetHeight() / 2, 1.0, m_dAngle, m_ObjBow.GetImg(), TRUE);
    DrawRotaGraph(m_ObjArrow.GetX() - nCameraX + m_ObjArrow.GetWidth() / 2, m_ObjArrow.GetY() + m_ObjArrow.GetHeight() / 2, 1.0, m_dAngle, m_ObjArrow.GetImg(), TRUE);
    //DrawGraph(m_ObjArrow.GetX() - nCameraX, m_ObjArrow.GetY(), m_ObjArrow.GetImg(), TRUE);

    // 攻撃中なら矢を放つ
    for (int i = (int)m_Arrows.size() - 1; i >= 0; --i) {
        ARROW a = m_Arrows[i];
        DrawRotaGraph(a.fx - nCameraX, a.fy, 1.0, a.fAngle, m_ObjArrow.GetImg(), TRUE, TRUE);
    }
}







