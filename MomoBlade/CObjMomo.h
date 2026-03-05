#pragma once

#include "CObject.h"

class CObjMomo : public CObject
{
public:
	CObjMomo();

public:
	void Update(); // GameMainのMoveEnemy
	void Draw();

	void CalcMomoVy();
	void SetDestY(float fDestY);
	float GetDestY();
	void SetOnLand(bool bOnLand);
	void SetOnGround(bool bOnGround);
	bool IsOnLand();
	bool IsOnGround();

	bool IsMomoInRangeObjectX(CObject Obj); // モモがオブジェクトの上に乗っているか判定

private:
	const float JUMP_UP_POWER = 0.8f;
	const float GRAVITEY = 1.2f;
	float m_fDestY;
	bool m_bOnLand;
	bool m_bOnGround;

};