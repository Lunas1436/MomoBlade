#pragma once

#include "CObject.h"

class CObjMomo : public CObject
{
public:
	CObjMomo();

public:
	void CalcMomoVy();
	void SetDestY(float fDestY);
	float GetDestY();
	void SetOnLand(bool bOnLand);
	void SetOnGround(bool bOnGround);
	bool IsOnLand();
	bool IsOnGround();
	bool IsMomoInRangeObjectX(CObject Obj); // モモがオブジェクトの上に乗っているか判定
	bool AABBOverlap(CObject ObjEnemy);

private:
	// ジャンプ関係
	const float JUMP_UP_POWER = 0.8f;
	const float GRAVITEY = 1.2f;
	float m_fDestY;
	bool m_bOnLand;
	bool m_bOnGround;

	// 攻撃関係
	bool m_bAttacking;
	int m_nSwordTimer;
	double m_dSwordLength;    // 剣の長さ→ヒットチェックの際、剣を線分として扱う
	double m_dSwordAngle = 0;

};