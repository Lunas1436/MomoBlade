#pragma once

#include "CObject.h"
#include "CObjSword.h"

using namespace std;

class CObjMomo : public CObject
{
public:
	CObjMomo();

public:
	void InitSword(float fx, float fy, float fvx, float fvy, const char* pchImgL, const char* pchImgR);
	void InitHP(float fx, float fy, const char *pchImg, vector<filesystem::path> &pathList);	

	void DrawSword(int nCameraX);
	void DrawHP();
	bool SubstractHP(int nDmg);

	void CalcMomoVy();
	void MoveMomo(int nDirection);

	void CalcSwordTipXY(float* pfx, float* pfy);
	bool IsMomoInRangeObjectX(CObject Obj); // モモがオブジェクトの上に乗っているか判定
	bool AABBOverlap(CObject ObjEnemy);
	
	void SetDestY(float fDestY);
	float GetDestY();
	void SetOnLand(bool bOnLand);
	void SetOnGround(bool bOnGround);
	void SetIsAttacking(bool bAttacking);
	
	bool IsOnLand();
	bool IsOnGround();
	bool IsAttacking();

private:
	// 剣
	CObjSword m_ObjSword;
	
	// HP
	CObject m_ObjHP;
	const int HP_MAX = 3; // MAXのHPはハート(HP)3個
	int m_nHP; // モモのHP(初期値MAX)
	vector<int> m_nImgHP; // ハートの画像(HP0～MAX)
	int m_nHPW;
	int m_nHPX, m_nHPY;

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