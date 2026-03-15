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
	bool GetFinishSlow();

	void SetSwordLength(double dLength);
	void SetSwordAngle(double dAngle);
	void SetAttack(bool bAttack);
	void SetFinishSlow(bool bSlow);
	void SetSound(const char* pchSound);
	void StopSound();

	void DrawSword();
	void SwordAttack();
	void CalcSwordTipXY(float* pfx, float* pfy);

private:
	double m_dSwordLength;    // 剣の長さ→ヒットチェックの際、剣を線分として扱う
	double m_dSwordAngle;
	bool m_bAttack;
	bool m_bFinishSlow;
	int m_nSlashBGM;

};
