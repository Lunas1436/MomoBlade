#include "CObjEnemy.h"

CObjEnemy::CObjEnemy()
{
	m_nDirection = DIRECTION_L;
	m_nImg_L = -1;
	m_nImg_R = -1;
	m_nDmgImg_L = -1;
	m_nDmgImg_R = -1;
}

void CObjEnemy::Update()
{

}

void CObjEnemy::CheckPointInRect(float fx, float fy, int nCameraX)
{
	if (fx >= m_fx - nCameraX && fx <= m_fx - nCameraX + m_nWidth &&
		fy >= m_fy && fy <= m_fy + m_nHeight) 
	{
		// 剣先の座標が敵矩形内にある場合は敵が攻撃を受けたとみなす
		m_bDamaged = true;
	}
}




