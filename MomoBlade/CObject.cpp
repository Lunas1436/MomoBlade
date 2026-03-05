#include "CObject.h"

// コンストラクタ
CObject::CObject() 
	: m_fx(0), m_fy(0), m_fvx(0), m_fvy(0),
m_nImg(-1), m_nWidth(0), m_nHeight(0),
m_bIsHit(false), m_nDmgTimer(0), m_nDmgImg(-1)
{

}

CObject::CObject(float fx, float fy, float fvx, float fvy, const char* pchImg)
{
	if (pchImg == nullptr) {
		// ログ出力
	}
}

// パラメータセット
void CObject::SetParameter(float fx, float fy, float fvx, float fvy, const char* pchImg)
{
	m_fx = fx;
	m_fy = fy;
	m_fvx = fvx;
	m_fvy = fvy;

	if (pchImg != nullptr) {
		m_nImg = LoadGraph(pchImg);
		if (m_nImg != -1) {
			GetGraphSize(m_nImg, &m_nWidth, &m_nHeight);
		}
	}
}

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
	return m_nImg;
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



