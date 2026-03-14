#include "CObjSword.h"
#include <math.h>


CObjSword::CObjSword()
{
	m_dSwordLength = 0.0;
	m_dSwordAngle = 0.0;
    m_nSlashBGM = LoadSoundMem("Sound/Momo/SlashBGM.wav");
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
    m_nTimer += 10;
    if (m_nTimer > 90) { // 斬撃モーション終了
        m_nTimer = 0;
        m_bAttack = false;
        m_dSwordAngle = 0;
        StopSoundMem(m_nSlashBGM, DX_PLAYTYPE_LOOP);
        return;
    }

    // 斬撃モーション中
    if (!CheckSoundMem(m_nSlashBGM)) { // 既に再生中か確認
        PlaySoundMem(m_nSlashBGM, DX_PLAYTYPE_LOOP);
    }
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


