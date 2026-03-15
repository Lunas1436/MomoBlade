#include "CObjSword.h"
#include <math.h>

CObjSword::CObjSword()
{
	m_dSwordLength = 0.0;
	m_dSwordAngle = 0.0;
    m_bAttack = false;
    m_bFinishSlow = false;
    m_nSlashBGM = -1;
}

// 剣描画
void CObjSword::DrawSword()
{
    if (m_bAttack) {
        SwordAttack();
    }
    DrawRotaGraph2(m_fx, m_fy + m_nHeight, 0, m_nHeight, 1.0, m_dSwordAngle, m_nCurrentImg, TRUE);
}

// 斬撃モーション
void CObjSword::SwordAttack()
{
    m_nTimer += m_bFinishSlow ? 1 : 10;
    if (m_nTimer > 90) { // 斬撃モーション終了
        m_nTimer = 0;
        m_bAttack = false;
        m_dSwordAngle = 0;
        StopSound();
        return;
    }

    // 斬撃モーション中
    // 再生中に呼び出した場合は最初から再生する
    PlaySoundMem(m_nSlashBGM, DX_PLAYTYPE_LOOP);
    m_dSwordAngle = m_nTimer * (3.14 / 180);
}

// 剣先の座標を計算
void CObjSword::CalcSwordTipXY(float* pfx, float* pfy)
{
    float fTipX = m_fx + m_dSwordLength * cos(m_dSwordAngle);
    float fTipY = m_fy + m_nHeight + m_dSwordLength * sin(m_dSwordAngle);

    *pfx = fTipX;
    *pfy = fTipY;
}

bool CObjSword::IsAttack()
{
    return m_bAttack;
}

bool CObjSword::GetFinishSlow()
{
    return m_bFinishSlow;
}

void CObjSword::SetSwordLength(double dLength)
{
    m_dSwordLength = dLength;
}

void CObjSword::SetSwordAngle(double dAngle)
{
    m_dSwordAngle = dAngle;
}

void CObjSword::SetAttack(bool bAttack)
{
    m_bAttack = bAttack;
}

void CObjSword::SetFinishSlow(bool bSlow)
{
    m_bFinishSlow = bSlow;
}

void CObjSword::SetSound(const char* pchSound)
{
    m_nSlashBGM = LoadSoundMem(pchSound);
}

void CObjSword::StopSound()
{
    StopSoundMem(m_nSlashBGM, DX_PLAYTYPE_LOOP);
}

