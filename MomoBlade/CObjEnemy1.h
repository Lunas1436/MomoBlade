#pragma once
#include "CObjEnemy.h"


class CObjEnemy1 : public CObjEnemy
{
public:
	CObjEnemy1();

public:
	void Update(CObject* pTarget) override;
	void Attack(CObject* pTarget) override;

};