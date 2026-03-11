#pragma once
#include "CObjEnemy.h"

class CObjEnemy3 : public CObjEnemy
{
public:
	CObjEnemy3();

public:
	void SetStartX(float fx);
	void SetImagesEne3(const char* pchImgShield, const char* pchImgLose);

	void Update(CObject* pTarget) override;
	void Attack(CObject* pTarget) override;
	void Draw(int nCameraX) override;

private:
	void SearchTarget();
	void ReturnToPlace();

private:
	int m_nImgLoseMark;
	int m_nImgShield;

	bool m_bAttacking = false;
	bool m_bLoseTarget = false;
	bool m_bReturn = false;
	int m_nTryTimer = 0;
	float m_fStartX;
	int m_nIdleTimer = 0;
	int m_nIdleInterval = 240;
};