#pragma once
#include "CObjEnemy.h"
#include <iostream>

using namespace std;

class CObjEnemy2 : public CObjEnemy
{
public:
	CObjEnemy2();

public:
	void InitWeapon(const char* pchImg1, const char* pchImg2);

	void Update(CObject* pTarget) override;
	void Attack(CObject* pTarget) override;
	void Draw(int nCameraX) override;

	void Loading(float fx, float fy);
	
	float CalcAngle(float fx, float fy, float ex, float ey);
	void SetArrowRange(float fRangeX, float fRangeY);

private:
	CObject m_ObjBow;
	CObject m_ObjArrow;
	struct ARROW {
		float fx;
		float fy;
		float fvx;
		float fvy;
		float fAngle;
	};
	vector<ARROW> m_Arrows;
	double m_dLength;
	double m_dAngle;

	int m_nFireTimer = 0;
	int m_nFireInterval = 120;
	float m_fArrowSpd = 6.0f;

	float m_fArrowRngX = 0;
	float m_fArrowRngY = 0;

};