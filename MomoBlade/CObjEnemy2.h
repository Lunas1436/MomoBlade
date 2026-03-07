#pragma once
#include "CObjEnemy.h"

class CObjEnemy2 : public CObjEnemy
{
public:
	CObjEnemy2();

public:
	void InitEnmey(float fDetectW, const char* pchImg);
	void Update();
	void Draw(int nCameraX) override;

private:
	float m_fDetectW;  // 検知範囲。敵の位置から向いている方向にfDetectDistの範囲内にモモがいれば検知
	bool m_bDetect;    // 検知の有無
	int m_nImgDetect;  // 「!」マーク画像

};