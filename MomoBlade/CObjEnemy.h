#pragma once
#include "CObject.h"

class CObjEnemy : public CObject
{
public:
	CObjEnemy();

	virtual void Update();
public:
	void CheckPointInRect(float fx, float fy, int nCameraX);

};