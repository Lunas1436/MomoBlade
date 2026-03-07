#pragma once
#include "CObject.h"

class CObjSword : public CObject
{
public:
	// コンストラクタ
	CObjSword();

public:
	void SetIsAttacking(bool bIsAttaking);
	void SetSwordLength(double dLength);
	void SetSwordAngle(double dAngle);

	void DrawSword();
	void SwordAttack();
	void CalcSwordTipXY(float* pfx, float* pfy);

private:
	double m_dSwordLength;    // 剣の長さ→ヒットチェックの際、剣を線分として扱う
	double m_dSwordAngle = 0;
	bool m_bIsAttacking = false;

	int m_nSlashBGM;

};
