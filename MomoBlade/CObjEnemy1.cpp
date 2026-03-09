
#include "CObjEnemy1.h"

// コンストラクタ
CObjEnemy1::CObjEnemy1()
{

}

void CObjEnemy1::Update(CObject* pTarget)
{
	CObjEnemy::Update(pTarget);

	// 左右の往復移動
	m_fx += m_fvx;
	if (m_fx >= m_fRangeR) {
		m_fx = m_fRangeR;
		m_fvx *= -1;
		m_nDirection = DIRECTION_L;
		m_nCurrentImg = m_nImg_L;
	}
	else if (m_fx < m_fRangeL) {
		m_fx = m_fRangeL;
		m_fvx *= -1;
		m_nDirection = DIRECTION_R;
		m_nCurrentImg = m_nImg_R;
	}
}

void CObjEnemy1::Attack(CObject* pTarget)
{

}

void CObjEnemy1::Draw()
{
	
	

}





