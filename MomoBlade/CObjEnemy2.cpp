#include "CObjEnemy2.h"

CObjEnemy2::CObjEnemy2()
{
	m_fDetectW = 0.0f;
	m_bDetect = false;
	m_nImgDetect = -1;

}

void CObjEnemy2::InitEnmey(float fDetectW, const char* pchImg)
{
	m_fDetectW = fDetectW;
	if (pchImg != nullptr) {
		m_nImgDetect = LoadGraph(pchImg);
	}

}

void CObjEnemy2::Update()
{
    //if (pEnemy->Obj.y - 50 <= ObjMomo.y && ObjMomo.y <= pEnemy->Obj.y + pEnemy->Obj.height + 20) {
    //    if (pEnemy->Obj.x - pEnemy->fDetectW <= ObjMomo.x && ObjMomo.x <= pEnemy->Obj.x) {
    //        pEnemy->bDetect = true;
    //    }
    //}

    //if (m_bDetect) { // モモがいる方向を向く
    //    if (pEnemy->Obj.x >= ObjMomo.x) {
    //        m_nDirection = DIRECTION_L;
    //    }
    //    else {
    //        m_nDirection = DIRECTION_R;
    //    }
    //}
}

void CObjEnemy2::Draw(int nCameraX)
{
	CObject::Draw(nCameraX);

	// ！マーク描画

}





