#pragma once
#include "CObject.h"

enum {
	DIRECTION_L,
	DIRECTION_R
};

class CObjEnemy : public CObject
{
public:
	CObjEnemy();

public:
	void SetImages(const char* pchImgL, const char* pchImgR, const char* pchDmgL, const char* pchDmgR);

private:
	// 向いている方向
	int m_nDirection;

	// 通常時の画像
	int m_nImg_L;
	int m_nImg_R;

	// 攻撃時を受けたときの画像
	int m_nDmgImg_L;
	int m_nDmgImg_R;


};