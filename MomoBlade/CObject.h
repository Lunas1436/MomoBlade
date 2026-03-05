#pragma once
#include "DxLib.h"

class CObject 
{
public:
	// コンストラクタ
	CObject();
	CObject(
		float fx,
		float fy,
		float fvx,
		float fvy,
		const char* pchImg = nullptr
	);
	
public:
	// パラメータセット
	void SetParameter(float fx, float fy, float fvx, float fvy, const char* pchImg);

	float GetX();
	float GetY();
	float GetVx();
	float GetVy();
	int GetWidth();
	int GetHeight();
	int GetImg();

	void SetX(float x);
	void SetY(float y);
	void SetVx(float vx);
	void SetVy(float vy);

protected:
	// 座標(左上基準)
	float m_fx;
	float m_fy;
	
	// 速度
	float m_fvx;
	float m_fvy;
	
	// 画像情報
	int m_nImg;
	int m_nWidth;
	int m_nHeight;
	
	// ヒットチェック用
	bool m_bIsHit;
	int m_nDmgTimer;
	int m_nDmgImg;

};
