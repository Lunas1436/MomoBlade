
#include "CObjEnemy1.h"

// コンストラクタ
CObjEnemy1::CObjEnemy1()
{
	m_fRangeL = 0.0f;
	m_fRangeR = 0.0f;
	m_fDetectW = 0.0f;
	m_bDetect = false;
	m_nImgDetect = -1;
}

// 敵1初期化
void CObjEnemy1::InitEnmey(float fRangeL, float fRangeR, float fDetectW, const char *pchImg)
{
	m_fRangeL = fRangeL;
	m_fRangeR = fRangeR;
	m_fDetectW = fDetectW;
	if (pchImg != nullptr) {
		m_nImgDetect = LoadGraph(pchImg);
	}
}

void CObjEnemy1::Update()
{
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

void CObjEnemy1::Draw()
{


}





