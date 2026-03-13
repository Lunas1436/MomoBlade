#include "CObject.h"


// コンストラクタ
CObject::CObject() 
	: m_fx(0), m_fy(0), m_fvx(0), m_fvy(0),
	m_nWidth(0), m_nHeight(0), m_bDamaged(false), m_nTimer(0)
{
	m_nImg_L = -1;
	m_nImg_R = -1;
	m_nDmgImg_L = -1;
	m_nDmgImg_R = -1;
}

CObject::CObject(float fx, float fy, float fvx, float fvy)
{

}

// パラメータセット
void CObject::SetParameter(float fx, float fy, float fvx, float fvy, const char *pchImg)
{
	m_fx = fx;
	m_fy = fy;
	m_fvx = fvx;
	m_fvy = fvy;

	if (pchImg != nullptr) {
		m_nCurrentImg = LoadGraph(pchImg);
		GetGraphSize(m_nCurrentImg, &m_nWidth, &m_nHeight);
	}

}

// 画像セット
void CObject::SetImages(const char* pchImgL, const char* pchImgR, const char* pchDmgL, const char* pchDmgR)
{
	if (pchImgL) {
		m_nImg_L = LoadGraph(pchImgL);
	}
	if (pchImgL) {
		m_nImg_R = LoadGraph(pchImgR);
	}
	if (pchImgL) {
		m_nDmgImg_L = LoadGraph(pchDmgL);
	}
	if (pchImgL) {
		m_nDmgImg_R = LoadGraph(pchDmgR);
	}

	if (m_nImg_L != -1) {
		m_nCurrentImg = m_nImg_L;
	}
}

// キャラクター描画
void CObject::Draw(int nCameraX)
{
	DrawGraph(m_fx - nCameraX, m_fy, m_nCurrentImg, TRUE);
}

// ダメージを受けているかチェック
bool CObject::DamagedCheck()
{
	if (!m_bDamaged) return false;
	
	m_nTimer++;
	if (m_nTimer > 45) {
		m_nTimer = 0;
		m_bDamaged = false;
	}
	return true;
}

// 被ダメージエフェクト
void CObject::DrawDamaged(int nCameraX)
{
	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);   // 色を加算する設定
	int nCol = GetColor(rand() % 256, rand() % 256, rand() % 256);
	DrawBox(m_fx - nCameraX, m_fy, m_fx - nCameraX + m_nWidth, m_fy + m_nHeight, nCol, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // 通常の描画に戻す
}


// ゲッター
float CObject::GetX()
{
	return m_fx;
}

float CObject::GetY()
{
	return m_fy;
}

float CObject::GetVx()
{
	return m_fvx;
}

float CObject::GetVy()
{
	return m_fvy;
}

void CObject::AddX(float fx)
{
	m_fx += fx;
}

int CObject::GetDirection()
{
	return m_nDirection;
}

int CObject::GetWidth()
{
	return m_nWidth;
}

int CObject::GetHeight()
{
	return m_nHeight;
}

int CObject::GetImg()
{
	return m_nCurrentImg;
}

bool CObject::IsDamaged()
{
	return m_bDamaged;
}

int CObject::GetTimer()
{
	return m_nTimer;
}

void CObject::AddTimer(int nTime)
{
	m_nTimer += nTime;
}

// セッター
void CObject::SetDamaged(bool bDamaged)
{
	m_bDamaged = bDamaged;
}
void CObject::SetTimer(int nTimer)
{
	m_nTimer = nTimer;
}

void CObject::SetX(float x)
{
	m_fx = x;
}

void CObject::SetY(float y)
{
	m_fy = y;
}

void CObject::SetVx(float vx)
{
	m_fvx = vx;
}

void CObject::SetVy(float vy)
{
	m_fvy = vy;
}

void CObject::SetImg(int nImg)
{
	m_nCurrentImg = nImg;
}

void CObject::SetDirection(int nDirection)
{
	m_nDirection = nDirection;

	if (m_nDirection == DIRECTION_L) {
		m_nCurrentImg = m_nImg_L;
	}
	else {
		m_nCurrentImg = m_nImg_R;
	}
}

