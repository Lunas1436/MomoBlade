
#include "CObjMomo.h"

CObjMomo::CObjMomo()
{
    m_bOnLand = false;
    m_bOnGround = false;
}

void CObjMomo::CalcMomoVy()
{
    // ジャンプUP中
    if (m_fy > m_fDestY) {
        m_fvy -= JUMP_UP_POWER;
    }
    // ジャンプDOWN中
    else {
        m_fvy += GRAVITEY;
    }
}

void CObjMomo::SetDestY(float fDestY)
{
    m_fDestY = fDestY;
}

float CObjMomo::GetDestY()
{
    return m_fDestY;
}

void CObjMomo::SetOnLand(bool bOnLand)
{
    m_bOnLand = bOnLand;
}

void CObjMomo::SetOnGround(bool bOnGround)
{
    m_bOnGround = bOnGround;
}

bool CObjMomo::IsOnLand()
{
    return m_bOnLand;
}

bool CObjMomo::IsOnGround()
{
    return m_bOnGround;
}

// モモがオブジェクトの上に乗っているか判定
bool CObjMomo::IsMomoInRangeObjectX(CObject Obj)
{
    if (Obj.GetX() <= m_fx + m_nWidth && m_fx <= Obj.GetX() + Obj.GetWidth()) {
        return true;
    }

    return false;
}

void CObjMomo::Draw()
{
    float fPrevTop = m_fx;
    float fPrevBottom = m_fy + m_nHeight;

    // オブジェクト上にいる
    if (bOnLand) {
        if (!bOnGround) {
            bool bInRange = false;
            for (int i = 0; i < ObjBlockList.size(); i++) {
                float fLandX = ObjBlockList[i].x;
                float fLandY = ObjBlockList[i].y;
                if (fPrevBottom <= fLandY && fLandY <= ObjMomo.y + ObjMomo.height) {
                    if (fLandX <= ObjMomo.x + ObjMomo.width && ObjMomo.x <= fLandX + ObjBlockList[i].width) {
                        bInRange = true;
                        break;
                    }
                }
            }
            if (!bInRange) {
                bOnLand = false;
                bOnGround = false;
                fMaxY = ObjGround.y;
            }
        }
    }
    // 空中にいる
    else {
        // ジャンプUP中
        if (ObjMomo.y > fMaxY) {
            ObjMomo.vy -= JUMP_UP_POWER;
        }
        // ジャンプDOWN中
        else {
            ObjMomo.vy += GRAVITEY;
        }

        ObjMomo.y += ObjMomo.vy;

        // 着地判定
        if (ObjMomo.vy > 0) {
            // 地面の上にいるかどうか判定
            if (fPrevBottom <= ObjGround.y && ObjGround.y <= ObjMomo.y + ObjMomo.height) {
                ObjMomo.y = ObjGround.y - ObjMomo.height;
                bOnLand = true;
                bOnGround = true;
                ObjMomo.vy = 0;
            }
            // 地面以外のオブジェクトの上にいるかどうか判定
            if (!bOnGround) {
                for (int i = 0; i < ObjBlockList.size(); i++) {
                    float fLandX = ObjBlockList[i].x;
                    float fLandY = ObjBlockList[i].y;
                    if (fLandX <= ObjMomo.x + ObjMomo.width && ObjMomo.x <= fLandX + ObjBlockList[i].width) {
                        if (fPrevBottom <= fLandY && fLandY <= ObjMomo.y + ObjMomo.height) {
                            // 着地
                            ObjMomo.y = fLandY - ObjMomo.height;
                            bOnLand = true;
                            ObjMomo.vy = 0;
                            break;
                        }
                    }
                }
            }
        }
        // 天井判定
        if (ObjMomo.vy < 0) {
            // 画面外に出たか判定
            if (fPrevTop >= 0 && ObjMomo.y < 0) {
                ObjMomo.y = 0;
                fMaxY = ObjGround.y;
            }
            else {
                for (int i = 0; i < ObjBlockList.size(); i++) {
                    float fLandX = ObjBlockList[i].x;
                    float fLandY = ObjBlockList[i].y;
                    if (fLandX <= ObjMomo.x + ObjMomo.width && ObjMomo.x <= fLandX + ObjBlockList[i].width) {
                        if (ObjBlockList[i].y + ObjBlockList[i].height <= fPrevTop && ObjMomo.y <= ObjBlockList[i].y + ObjBlockList[i].height) {
                            ObjMomo.y = ObjBlockList[i].y + ObjBlockList[i].height;
                            fMaxY = ObjGround.y;
                            break;
                        }
                    }
                }
            }
        }
    }

    // モモ描画
    DrawGraph(ObjMomo.x - nCameraX, ObjMomo.y, ObjMomo.img, TRUE);
}




