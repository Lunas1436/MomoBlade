#pragma once
#include "DxLib.h"

// キャラクターの方向
enum {
	DIRECTION_L,
	DIRECTION_R
};

class CObject 
{
public:
	// コンストラクタ
	CObject();
	CObject(
		float fx,
		float fy,
		float fvx,
		float fvy
	);
	
public:
	// パラメータセット
	void SetParameter(float fx, float fy, float fvx, float fvy, const char* pchImg=nullptr);
	void SetImages(const char* pchImgL, const char* pchImgR, const char* pchDmgL, const char* pchDmgR);

 	virtual void Draw(int nCameraX);

	float GetX();
	float GetY();
	float GetVx();
	float GetVy();
	void AddX(float fx);
	void AddY();
	int GetDirection();
	int GetWidth();
	int GetHeight();
	int GetImg();
	int GetTimer();
	bool IsHit();
	bool IsAttack();
	bool IsDamaged();
	void AddTimer(int nTime);

	void SetX(float x);
	void SetY(float y);
	void SetVx(float vx);
	void SetVy(float vy);
	void SetImg(int nImg);
	void SetHit(bool bHit);
	void SetDirection(int nDirection);
	bool SetAttack(bool bAttack);
	bool SetDamaged(bool bDamaged);
	void SetTimer(int nTimer);

	bool DamagedCheck();
	void DrawDamaged(int nCameraX);

protected:
	// 座標(左上基準)
	float m_fx;
	float m_fy;
	
	// 速度
	float m_fvx;
	float m_fvy;
	
	// 画像情報
	int m_nCurrentImg;
	// 通常時の画像
	int m_nImg_L;
	int m_nImg_R;
	// 攻撃時を受けたときの画像
	int m_nDmgImg_L;
	int m_nDmgImg_R;

	int m_nDirection; // 方向
	int m_nWidth;     // 高さ
	int m_nHeight;    // 幅
	
	// ヒットチェック用
	bool m_bHit;
	bool m_bAttack;
	bool m_bDamaged;
	int m_nTimer;

};
