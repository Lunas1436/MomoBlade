#pragma once
#include "CObjEnemy.h"

enum {
	DIRECTION_L,
	DIRECTION_R
};

class CObjEnemy1 : public CObjEnemy
{
public:
	CObjEnemy1();

public:
	void InitEnmey(float fRangeL, float fRangeR, float fDetectW, const char* pchImg);

private:
	// X方向の移動可能範囲
	float m_fRangeL;
	float m_fRangeR;
	float m_fDetectW;  // 検知範囲。敵の位置から向いている方向にfDetectDistの範囲内にモモがいれば検知
	bool m_bDetect;    // 検知の有無
	int m_nImgDetect;  // 「!」マーク画像

};