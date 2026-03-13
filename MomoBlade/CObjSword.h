#pragma once
#include "CObject.h"
#include <DxLib.h>


class CObjSword : public CObject
{
public:
	// コンストラクタ
	CObjSword();

public:
	bool IsAttack();

	void SetSwordLength(double dLength);
	void SetSwordAngle(double dAngle);
	void SetAttack(bool bAttack);

	void DrawSword();
	void SwordAttack();
	void CalcSwordTipXY(float* pfx, float* pfy);

private:
	double m_dSwordLength;    // 剣の長さ→ヒットチェックの際、剣を線分として扱う
	double m_dSwordAngle = 0;
	bool m_bAttack = false;
	int m_nSlashBGM;

};
