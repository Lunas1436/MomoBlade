#pragma once
#include "CObject.h"

class CObjEnemy : public CObject
{
public:
	CObjEnemy();

	virtual void Update(float fx, float fy);
	virtual void Attack(float fx, float fy);

	bool DetectCheck(float fx, float fy);
	void InitEnmey(float fRangeL, float fRangeR, float fDetectW, const char* pchImg);

public:
	bool CheckPointInRect(float fx, float fy, int nCameraX);

protected:
	// X方向の移動可能範囲
	float m_fRangeL;
	float m_fRangeR;
	float m_fDetectW;  // 検知範囲。敵の位置から向いている方向にfDetectDistの範囲内にモモがいれば検知
	bool m_bDetect;    // 検知の有無
	int m_nImgDetect;  // 「!」マーク画像

};