#include "CObjEnemy2.h"

CObjEnemy2::CObjEnemy2()
{
}

void CObjEnemy2::InitWeapon(const char* pchImg1, const char* pchImg2)
{
    // 弓
    m_ObjBow.SetParameter(0.0f, 0.0f, 0.0f, 0.0f, pchImg1);
    m_ObjBow.SetX(m_fx - m_ObjBow.GetWidth());
    m_ObjBow.SetY(m_fy + m_nHeight / 2 - (m_ObjBow.GetHeight() / 2.0));

    // 矢
    m_ObjArrow.SetParameter(0.0f, 0.0f, 0.0f, 0.0f, pchImg2);
    m_ObjArrow.SetX(m_ObjBow.GetX());
    m_ObjArrow.SetY(m_ObjBow.GetY() + (m_ObjBow.GetHeight() / 2.0) - (m_ObjArrow.GetHeight() / 2.0));
}

void CObjEnemy2::Update(float fx, float fy)
{
    CObjEnemy::Update(fx, fy);

    // モモがいる方向を向く
    if (m_bDetect) {
        if (m_fx >= fx) {
            SetDirection(DIRECTION_L);
        }
        else {
            SetDirection(DIRECTION_R);
        }
    }
}

void CObjEnemy2::Attack(float fx, float fy)
{
    // 弓矢の座標とモモの座標を用いて矢を放つ
    Arrow(fx, fy);

}

void CObjEnemy2::Arrow(float fx, float fy)
{

}

void CObjEnemy2::Draw(int nCameraX)
{
	CObject::Draw(nCameraX);
    
    // 弓矢描画
    DrawGraph(m_ObjBow.GetX() - nCameraX, m_ObjBow.GetY(), m_ObjBow.GetImg(), TRUE);
    DrawGraph(m_ObjArrow.GetX() - nCameraX, m_ObjArrow.GetY(), m_ObjArrow.GetImg(), TRUE);

}





