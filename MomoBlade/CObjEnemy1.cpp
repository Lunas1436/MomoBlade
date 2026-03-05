
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



