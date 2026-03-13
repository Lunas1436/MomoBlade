#pragma once
#include "CObject.h"


class CObjEnemy : public CObject
{
public:
	CObjEnemy();

	virtual void Update(CObject* pTarget);
	virtual void Attack(CObject *pTarget);
	virtual void Draw(int nCameraX);
	void InitEnmey(float fRangeL, float fRangeR, float fDetectW, const char* pchImg);

public:
	bool IsPointInRect(float fx, float fy, int nCameraX); // nCameraXを考慮しなくていいように修正する→GameMainのUpdateEnemyでEnemyのX座標をカメラ考慮版に更新するとか

protected:
	// X方向の移動可能範囲
	float m_fRangeL;
	float m_fRangeR;
	float m_fDetectW;  // 検知範囲。敵の位置から向いている方向にfDetectDistの範囲内にモモがいれば検知
	bool m_bDetect;    // 検知の有無
	int m_nImgDetect;  // 「!」マーク画像

	//bool m_bIsAttacking; // モモと共通ならCObjectに定義する


};