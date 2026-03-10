
#include "CObjEnemy3.h"

// コンストラクタ
CObjEnemy3::CObjEnemy3()
{

}

void CObjEnemy3::SetStartX(float fx)
{
	m_fStartX = fx;
}

void CObjEnemy3::SetLoseMark(const char* pchImg)
{
	if (pchImg != nullptr) {
		m_nImgLoseTarget = LoadGraph(pchImg);
	}
}

void CObjEnemy3::Update(CObject* pTarget)
{
	CObjEnemy::Update(pTarget);

	// モモを検知
	if (m_bDetect) {
		m_bAttacking = true;
		m_nIdleTimer = 0;
		m_bLoseTarget = false;
		m_nTryTimer = 0;
		m_bReturn = false;
	}
	// 左右をキョロキョロ
	else {
		if (m_bAttacking) return;
		m_fvx = 5.0;
		if (m_bLoseTarget) {
			m_nTryTimer++;
			if (m_nTryTimer >= 300) {
				m_nTryTimer = 0;
				m_bReturn = true;
				m_bLoseTarget = false;
			}
		}
		if (m_bReturn) {
			ReturnToPlace();
		}
		else {
			SearchTarget();
		}
	}

}

// モモの座標まで突進
// モモを通り過ぎたら徐々に減速して見失う→サーチ開始→しばらく探していないようなら元の位置に戻ってキョロキョロ
void CObjEnemy3::Attack(CObject* pTarget)
{
	if (!m_bDetect && !m_bAttacking) return;

	if (m_nDirection == DIRECTION_L) {
		m_fx -= m_fvx;
		if (pTarget->GetX() >= m_fx) {
			m_fvx -= 0.5;
			if (m_fvx < 0) {
				m_bLoseTarget = true;
				m_bAttacking = false;
			}
		}
	}
	else {
		m_fx += m_fvx;
		if (pTarget->GetX() <= m_fx) {
			m_fvx -= 0.5;
			if (m_fvx < 0) {
				m_bLoseTarget = true;
				m_bAttacking = false;
			}
		}
	}
}

// 左右をキョロキョロする
void CObjEnemy3::SearchTarget()
{
	m_nIdleTimer++;
	if (m_nIdleTimer >= m_nIdleInterval) {
		m_nIdleTimer = 0;
		if (m_nDirection == DIRECTION_L) {
			m_nDirection = DIRECTION_R;
			m_nCurrentImg = m_nImg_R;
		}
		else {
			m_nDirection = DIRECTION_L;
			m_nCurrentImg = m_nImg_L;
		}
	}
}

void CObjEnemy3::ReturnToPlace()
{
	if (m_fStartX >= m_fx) {
		m_fx += 3.0;
		if (m_fx >= m_fStartX) {
			m_bReturn = false;
		}
	}
	else {
		m_fx -= 3.0;
		if (m_fx <= m_fStartX) {
			m_bReturn = false;
		}
	}

}

void CObjEnemy3::Draw(int nCameraX)
{
	CObjEnemy::Draw(nCameraX);

	if (m_bLoseTarget) {
		DrawGraph(m_fx - nCameraX - 25, m_fy - 25, m_nImgLoseTarget, TRUE);
	}
}





