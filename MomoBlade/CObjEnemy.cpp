#include "CObjEnemy.h"

CObjEnemy::CObjEnemy()
{
	m_nDirection = DIRECTION_L;
	m_nImg_L = -1;
	m_nImg_R = -1;
	m_nDmgImg_L = -1;
	m_nDmgImg_R = -1;
}

// 敵の画像セット
void CObjEnemy::SetImages(const char* pchImgL, const char* pchImgR, const char* pchDmgL, const char* pchDmgR)
{
	if (pchImgL) {
		m_nImg_L = LoadGraph(pchImgL);
	}
	if (pchImgL) {
		m_nImg_R = LoadGraph(pchImgR);
	}
	if (pchImgL) {
		m_nDmgImg_L = LoadGraph(pchDmgL);
	}
	if (pchImgL) {
		m_nDmgImg_R = LoadGraph(pchDmgR);
	}
}




