#pragma once
#include "CObjEnemy.h"

class CObjEnemy1 : public CObjEnemy
{
public:
	CObjEnemy1();

public:
	void Update(float fx, float fy) override;
	void Attack(float fx, float fy) override;
	void Draw();



};