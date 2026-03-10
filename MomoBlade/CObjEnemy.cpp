#include "CObjEnemy.h"

CObjEnemy::CObjEnemy()
{
	m_nDirection = DIRECTION_L;

	m_fRangeL = 0.0f;
	m_fRangeR = 0.0f;
	m_fDetectW = 0.0f;
	m_bDetect = false;
	m_nImgDetect = -1;
}

// 敵初期化
void CObjEnemy::InitEnmey(float fRangeL, float fRangeR, float fDetectW, const char* pchImg)
{
	m_fRangeL = fRangeL;
	m_fRangeR = fRangeR;
	m_fDetectW = fDetectW;
	if (pchImg != nullptr) {
		m_nImgDetect = LoadGraph(pchImg);
	}
}

void CObjEnemy::Update(CObject* pTarget)
{
	if (m_bDamaged) {
		if (m_nDirection == DIRECTION_L) {
			m_nCurrentImg = m_nDmgImg_L;
		}
		else {
			m_nCurrentImg = m_nDmgImg_R;
		}
	}
	else {
		if (m_nDirection == DIRECTION_L) {
			m_nCurrentImg = m_nImg_L;
		}
		else {
			m_nCurrentImg = m_nImg_R;
		}
	}

	m_bDetect = false;
	if (m_fy - 50 <= pTarget->GetY() && pTarget->GetY() <= m_fy + m_nHeight + 20) {
		if (m_nDirection == DIRECTION_L) {
			if (m_fx - m_fDetectW <= pTarget->GetX() && pTarget->GetX() <= m_fx) {
				m_bDetect = true;
			}
		}
		else {
			if (m_fx <= pTarget->GetX() && pTarget->GetX() <= m_fx + m_fDetectW) {
				m_bDetect = true;
			}
		}
	}
}

void CObjEnemy::Attack(CObject* pTarget)
{

}

void CObjEnemy::Draw(int nCameraX)
{
	CObject::Draw(nCameraX);

	if (m_bDetect) {
		DrawGraph(m_fx - nCameraX - 30, m_fy - 35, m_nImgDetect, TRUE);
	}
}

bool CObjEnemy::IsPointInRect(float fx, float fy, int nCameraX)
{
	if (fx >= m_fx - nCameraX && fx <= m_fx - nCameraX + m_nWidth &&
		fy >= m_fy && fy <= m_fy + m_nHeight) 
	{
		// 剣先の座標が敵矩形内にある場合は敵が攻撃を受けたとみなす
		return true;
	}

	return false;
}




