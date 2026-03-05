#include "GameMain.h"

#include <string>
#include <cmath>

using namespace std;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    SetWindowText("MOMO BLADE"); // ウィンドウのタイトル
    SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32); // ウィンドウの大きさとカラービット数の指定
    ChangeWindowMode(TRUE); // ウィンドウモードで起動
    if (DxLib_Init() == -1) return -1; // ライブラリ初期化 エラーが起きたら終了
    SetBackgroundColor(0, 0, 0); // 背景色の指定
    SetDrawScreen(DX_SCREEN_BACK); // 描画面を裏画面にする

    // 初期化
    InitData();

    // ステージのBGMをループ再生で流す
    PlaySoundMem(nStageBGM, DX_PLAYTYPE_LOOP);

    while (1)
    {
        // 画面クリア
        ClearDrawScreen();

        // ステージ描画
        DrawStage();

        // HP描画
        DrawHP();

        // プレイヤー入力
        PlayerInput();

        // カメラ座標(モモを中心)→モモクラスのメンバにnCameraXを追加
        //nCameraX = (ObjMomo.m_fx + ObjMomo.width / 2) - SCREEN_WIDTH / 2;
        //if (nCameraX < 0) {
        //    nCameraX = 0;
        //}
        //else if (nCameraX + SCREEN_WIDTH > STAGE_WIDTH) {
        //    nCameraX = STAGE_WIDTH - SCREEN_WIDTH;
        //}

        // モモと剣描画
        DrawMomo();

        // 敵移動
        MoveEnemy();

        // 敵描画
        DrawEnemy();
        
        // ヒットチェック
        CollisionCheck();

        ScreenFlip(); // 裏画面の内容を表画面に反映させる
        if (ProcessMessage() == -1) break; // Windowsから情報を受け取りエラーが起きたら終了
        if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break; // ESCキーが押されたら終了
    }

    DxLib_End(); // ＤＸライブラリ使用の終了処理
    return 0;
}

// 各オブジェクトの情報を外部のパラメータファイルで持つようにする
// 初期化
void InitData()
{
    // ステージ
    // 空
    // 雲のX軸の移動速度を0.5f
    ObjSky.SetParameter(0.0f, 0.0f, 0.5f, 0.0f, "Image/Stage/Sky.png");

    // 地面
    ObjGround.SetParameter(0.0f, 0.0f, 0.0f, 0.0f, "Image/Stage/Ground.png");
    ObjGround.SetY(SCREEN_HEIGHT - ObjGround.GetHeight() * 3);

    // 地中
    ObjUnderGround.SetParameter(ObjGround.GetX(), 0.0f, 0.0f, 0.0f, "Image/Stage/UnderGround.png");
    ObjUnderGround.SetY(ObjGround.GetY() + ObjUnderGround.GetHeight());

    // ブロック
    ObjBlock.SetParameter(500, ObjGround.GetY() - 150, 0, 0, "Image/Stage/Block.png");
    ObjBlockList.push_back(ObjBlock);

    // ゴールフラッグ
    ObjGoalFlag.SetParameter(STAGE_WIDTH - 200, 0.0f, 0.0f, 0.0f, "Image/Stage/GoalFlag.png");
    ObjGoalFlag.SetY(ObjGround.GetY() - ObjGoalFlag.GetHeight());

    // モモタロー
    ObjMomo.SetParameter(100, 0.0f, 5.0f, 0.0f, "Image/Momo/Momo.png");
    ObjMomo.SetY(ObjGround.GetY() - ObjMomo.GetHeight());
    
    // 剣
    ObjSword.SetParameter(0.0f, 0.0f, 0.0f, 0.0f, "Image/Momo/Sword.png");
    dSwordLength = ObjSword.GetWidth() * 1.41;
    dSwordAngle = 0;

    // HP
    string strHPpath = "";
    for (int i = 0; i < 5; i++) {
        strHPpath = "Image/Momo/HP_" + std::to_string(i) + ".png";
        nImgHP[i] = LoadGraph(strHPpath.c_str());
    }
    int nTempH = 0;
    GetGraphSize(nImgHP[0], &nHPWidth, &nTempH);
    nHPX = 20, nHPY = 20; // とりあえずの座標
    nCurrentHP = HP_MAX * 4;

    // 敵1   
    ObjEnemyList[0].InitEnmey(0.0f, 0.0f, 0.0f, 0.0f, "Image/Enemy/Enemy1_L.png", 500, 1000, 0);
    ObjEnemyList[0].SetY(ObjEnemyList[0].GetY() + ObjEnemyList[0].GetHeight() / 2);
    ObjEnemyList[0].SetImages(
        "Image/Enemy/Enemy1_L.png",
        "Image/Enemy/Enemy1_R.png",
        "Image/Enemy/Enemy1_Damaged_L.png",
        "Image/Enemy/Enemy1_Damaged_R.png",
        "Image/Enemy/DetectMark.png"
    );

    // 敵2
    ObjEnemyList[1].InitEnmey(1100, 50, 0.0f, 0.0f, "Image/Enemy/Enemy2_L.png", 0, 0, 300);
    ObjEnemyList[1].SetY(ObjEnemyList[1].GetY() + ObjEnemyList[1].GetHeight() / 2);
    ObjEnemyList[1].SetImages(
        "Image/Enemy/Enemy2_L.png",
        "Image/Enemy/Enemy2_R.png",
        "Image/Enemy/Enemy2_Damaged_L.png",
        "Image/Enemy/Enemy2_Damaged_R.png",
        "Image/Enemy/DetectMark.png"
    ); 

    // サウンド
    // ステージBGM
    nStageBGM = LoadSoundMem("Sound/Stage/StageBGM.wav");
    // 斬撃BGM
    nSlashBGM = LoadSoundMem("Sound/Momo/SlashBGM.wav");

}

// HP更新
void DrawHP()
{
    int nFullHP = nCurrentHP / 4;
    int nLeftHP = nCurrentHP % 4;
    int nNonHP = (HP_MAX * 4 - nCurrentHP) / 4;

    int i = 0;
    // フルHP
    for (i = 0; i < nFullHP; i++) {
        DrawGraph(nHPX + nHPWidth * i, nHPY, nImgHP[4], TRUE);
    }

    // 
    if (nLeftHP > 0) {
        DrawGraph(nHPX + nHPWidth * i, nHPY, nImgHP[nLeftHP], TRUE);
        i++;
    }

    for (int j = 0; j < nNonHP; j++) {
        DrawGraph(nHPX + nHPWidth * (i + j), nHPY, nImgHP[0], TRUE);
    }
}

// ステージ描画
void DrawStage()
{
    // 空描画
    float fnx = ObjSky.GetX() + ObjSky.GetVx();
    if (fnx >= SCREEN_WIDTH) {
        fnx -= SCREEN_WIDTH;
    }
    ObjSky.SetX(fnx);
    DrawGraph(fnx - ObjSky.GetWidth(), ObjSky.GetY(), ObjSky.GetImg(), TRUE);
    DrawGraph(fnx, ObjSky.GetY(), ObjSky.GetImg(), TRUE);

    // 地面描画
    int nGroundW = ObjGround.GetWidth();
    for (int i = 0; i < STAGE_WIDTH / nGroundW; i++) {
        DrawGraph(i * nGroundW - nCameraX, ObjGround.GetY(), ObjGround.GetImg(), TRUE); // 地面
        DrawGraph(i * nGroundW - nCameraX, ObjGround.GetY() + ObjGround.GetHeight(), ObjUnderGround.GetImg(), TRUE); // 地中
    }

    // ブロック描画
    for (int i = 0; i < ObjBlockList.size(); i++) {
        DrawGraph(ObjBlockList[i].GetX() - nCameraX, ObjBlockList[i].GetY(), ObjBlock.GetImg(), TRUE);
    }

    // ゴールフラッグ描画
    DrawGraph(ObjGoalFlag.GetX() - nCameraX, ObjGoalFlag.GetY(), ObjGoalFlag.GetImg(), TRUE);
}

// プレイヤー入力
void PlayerInput()
{
    // →キー
    if (CheckHitKey(KEY_INPUT_RIGHT)) {
        ObjMomo.x += ObjMomo.vx;
    }

    // ←キー
    if (CheckHitKey(KEY_INPUT_LEFT)) {
        ObjMomo.x -= ObjMomo.vx;
        if (ObjMomo.x < 0) {
            ObjMomo.x = 0;
        }
    }

    // ↑キー
    if (CheckHitKey(KEY_INPUT_UP)) {
        if (ObjMomo.IsOnLand()) {
            ObjMomo.SetDestY(ObjMomo.GetY() - 100); // とりあえず現在地から100ドットをジャンプの上限とする
            ObjMomo.SetOnLand(false);
            ObjMomo.SetOnGround(false);
        }
    }

    // スペースキー
    if (CheckHitKey(KEY_INPUT_SPACE)) {
        if (bIsAttacking == false) {
            bIsAttacking = true;
        }
    }
}

// モモと剣描画
void DrawMomo()
{
    // モモがオブジェクト上に乗っている
    if (ObjMomo.IsOnLand()) { 
        if (!ObjMomo.IsOnGround()) { // 地面以外のオブジェクト上にいる
            if (!ObjMomo.IsMomoInRangeObjectX(ObjBlockList[m_nOnIndex])) { // オブジェクトのX範囲外に出たとき
                m_nOnIndex = -1;
                for (int i = 0; i < ObjBlockList.size(); i++) {
                    if (ObjMomo.IsMomoInRangeObjectX(ObjBlockList[i])) {
                        // 他のオブジェクトのX範囲内に入ったとき
                        m_nOnIndex = i; 
                    }
                }
                if (m_nOnIndex < 0) { // 範囲外の空中に出たとき
                    ObjMomo.SetOnLand(false);
                    ObjMomo.SetOnGround(false);
                    ObjMomo.SetDestY(ObjGround.GetY());
                }
            }
        }
    }
    // 空中にいる
    else {
        float fPrevTop = ObjMomo.GetY();
        float fPrevBottom = fPrevTop + ObjMomo.GetHeight();

        ObjMomo.CalcMomoVy(); // ジャンプUP or DOWNの速度計算
        float fNextTop = ObjMomo.GetY() + ObjMomo.GetVy();
        float fNextBottom = fNextTop + ObjMomo.GetHeight();
        //ObjMomo.y += ObjMomo.vy;

        // 着地判定
        if (ObjMomo.GetVy() > 0) {
            // まず地面の上にいるか判定
            if (fPrevBottom <= ObjGround.GetY() && ObjGround.GetY() <= fNextBottom) {
                ObjMomo.SetY(ObjGround.GetY() - ObjMomo.GetHeight());
                ObjMomo.SetOnLand(true);
                ObjMomo.SetOnGround(true);
                ObjMomo.SetVy(0.0f);
            }
            // 地面以外のオブジェクトの上にいるかどうか判定
            if (!ObjMomo.IsOnGround()) {
                for (int i = 0; i < ObjBlockList.size(); i++) {
                    if (ObjMomo.IsMomoInRangeObjectX(ObjBlockList[i])) {
                        float fLandY = ObjBlockList[i].GetY();
                        if (fPrevBottom <= fLandY && fLandY <= fNextBottom) {
                            // 着地
                            ObjMomo.SetY(fLandY - ObjMomo.GetHeight());
                            ObjMomo.SetOnLand(true);
                            ObjMomo.SetVy(0.0f);
                            break;
                        }
                    }
                }
            }   
        }
        // 天井判定
        if (ObjMomo.GetVy() < 0) {
            // 画面外に出たか判定
            if (fPrevTop >= 0 && fNextTop < 0) {
                ObjMomo.SetY(0.0f);
                ObjMomo.SetDestY(ObjGround.GetY());
            }
            else {
                // オブジェクトにモモの頭が直撃するか判定
                for (int i = 0; i < ObjBlockList.size(); i++) {
                    float fLandY = ObjBlockList[i].GetY();
                    if (ObjMomo.IsMomoInRangeObjectX(ObjBlockList[i])) {
                        float fBlockTop = ObjBlockList[i].GetY() + ObjBlockList[i].GetHeight();
                        if (fBlockTop <= fPrevTop && fNextTop <= fBlockTop) {
                            ObjMomo.SetY(fBlockTop);
                            ObjMomo.SetDestY(ObjGround.GetY());
                            break;
                        }
                    }
                }
            }
        }
    }

    // モモ描画
    // モモ描画
    DrawGraph(ObjMomo.GetX() - nCameraX, ObjMomo.GetY(), ObjMomo.GetImg(), TRUE);

    // 剣描画
    ObjSword.x = ObjMomo.x - nCameraX + ObjMomo.width + 10;
    ObjSword.y = ObjMomo.y - 20;
    if (bIsAttacking) { // 攻撃中なら斬撃モーション
        SwordAttack();
    }
    DrawRotaGraph2(ObjSword.x, ObjSword.y + ObjSword.height, 0, ObjSword.height, 1.0, dSwordAngle, ObjSword.img, TRUE);
}

// 斬撃モーション
void SwordAttack()
{
    ObjSword.nDmgTimer += 10;
    if (ObjSword.nDmgTimer > 90) { // 斬撃モーション終了
        ObjSword.nDmgTimer = 0;
        ObjSword.bIsHit = false;
        dSwordAngle = 0;
        bIsAttacking = false;
        StopSoundMem(nSlashBGM, DX_PLAYTYPE_LOOP);
        return;
    }

    // 斬撃モーション中
    PlaySoundMem(nSlashBGM, DX_PLAYTYPE_LOOP);
    dSwordAngle = ObjSword.nDmgTimer * (3.14 / 180);
}

// 敵移動
void MoveEnemy()
{
    // まずは各敵の挙動を決める
    // 敵1→単純な往復
    // 敵2→モモを検知したら攻撃してくる→その場からは動かない
    // 敵3→モモを検知したら突進してくる
    MoveEnemy1();
    MoveEnemy2();

}

// 敵1の挙動
void MoveEnemy1()
{
    ENEMY* pEnemy = &ObjEnemyList[0];
    pEnemy->Obj.x += pEnemy->Obj.vx;
    if (pEnemy->Obj.x >= pEnemy->fRangeR) {
        pEnemy->Obj.x = pEnemy->fRangeR;
        pEnemy->Obj.vx *= -1;
        pEnemy->nDirection = DIRECTION_L;
    }
    else if (pEnemy->Obj.x < pEnemy->fRangeL) {
        pEnemy->Obj.x = pEnemy->fRangeL;
        pEnemy->Obj.vx *= -1;
        pEnemy->nDirection = DIRECTION_R;
    }
}

// 敵2の挙動
void MoveEnemy2()
{
    ENEMY* pEnemy = &ObjEnemyList[1];
    pEnemy->bDetect = false; // 確認用
    if (pEnemy->nDirection == DIRECTION_L) {
        if (pEnemy->Obj.y - 50 <= ObjMomo.y && ObjMomo.y <= pEnemy->Obj.y + pEnemy->Obj.height + 20) {
            if (pEnemy->Obj.x - pEnemy->fDetectW <= ObjMomo.x && ObjMomo.x <= pEnemy->Obj.x) {
                pEnemy->bDetect = true;
            }
        }
    }

    if (pEnemy->bDetect) { // モモがいる方向を向く
        if (pEnemy->Obj.x >= ObjMomo.x) {
            pEnemy->nDirection = DIRECTION_L;
        }
        else {
            pEnemy->nDirection = DIRECTION_R;
        }
    }
}

// 敵3の挙動
void MoveEnemy3()
{


}

// 敵描画
void DrawEnemy()
{
    for (int i = 0; i < 3; i++) {
        if (nCameraX < ObjEnemyList[i].Obj.x && ObjEnemyList[i].Obj.x < nCameraX + SCREEN_WIDTH) { // ウィンドウ内にあるときに描画
            if (i == nDmgIndex) continue;
            if (ObjEnemyList[i].nDirection == DIRECTION_L) {
                DrawGraph(ObjEnemyList[i].Obj.x - nCameraX, ObjEnemyList[i].Obj.y, ObjEnemyList[i].nImg_L, TRUE);
                if (ObjEnemyList[i].bDetect) { 
                    // モモを検知した場合、「!」マークを描画
                    DrawGraph(ObjEnemyList[i].Obj.x - nCameraX - 35, ObjEnemyList[i].Obj.y - 20, nDetectImg, TRUE);
                }
            }
            else {
                DrawGraph(ObjEnemyList[i].Obj.x - nCameraX, ObjEnemyList[i].Obj.y, ObjEnemyList[i].nImg_R, TRUE);
                if (ObjEnemyList[i].bDetect) {
                    // モモを検知した場合、「!」マークを描画
                    DrawGraph(ObjEnemyList[i].Obj.x - nCameraX + 35, ObjEnemyList[i].Obj.y - 20, nDetectImg, TRUE);
                }
            }
        }
    }
}

// ヒットチェック
void CollisionCheck()
{
    // 剣先と敵
    if (nDmgIndex >= 0 && ObjEnemyList[nDmgIndex].Obj.bIsHit) {
        ObjEnemyList[nDmgIndex].Obj.nDmgTimer++;
        DrawDamageToEnemy(ObjEnemyList[nDmgIndex]); // 敵のダメージエフェクト
        if (ObjEnemyList[nDmgIndex].Obj.nDmgTimer > 60) {
            ObjEnemyList[nDmgIndex].Obj.nDmgTimer = 0;
            ObjEnemyList[nDmgIndex].Obj.bIsHit = false;
            nDmgIndex = -1;
        }
    }
    else {
        if (bIsAttacking) {
            float fTipX = ObjSword.x + dSwordLength * cos(dSwordAngle);
            float fTipY = ObjSword.y + ObjSword.height + dSwordLength * sin(dSwordAngle);
            for (int i = 0; i < 3; i++) {
                if (PointInRect(fTipX, fTipY, ObjEnemyList[i].Obj)) {
                    ObjSword.bIsHit = true;
                    nDmgIndex = i;
                    ObjEnemyList[nDmgIndex].Obj.bIsHit = true;
                }
            }
        }
    }

    // モモと敵
    if (ObjMomo.bIsHit) {
        ObjMomo.nDmgTimer++;
        DrawDamageToMomo(); // モモのダメージエフェクト
        if (ObjMomo.nDmgTimer > 60) { // ダメージエフェクトは60フレーム
            ObjMomo.nDmgTimer = 0;
            ObjMomo.bIsHit = false;
        }
    }
    else {
        for (int i = 0; i < 3; i++) {
            if (ObjEnemyList[i].Obj.bIsHit) continue;
            if (AABBOverlap(ObjMomo, ObjEnemyList[i].Obj)) {
                ObjMomo.nDmgTimer = 0; // 一応初期化
                ObjMomo.bIsHit = true;
                nCurrentHP--;
            }
        }
    }
}

// 剣先(点)と敵(矩形)のヒットチェック
// 点が矩形の中に含まれるか判定
bool PointInRect(float fpx, float fpy, OBJECT ObjRect)
{
    if (fpx >= ObjRect.x - nCameraX && fpx <= ObjRect.x - nCameraX + ObjRect.width &&
        fpy >= ObjRect.y && fpy <= ObjRect.y + ObjRect.height) {
        return true;
    }

    return false;
}

// モモ(矩形)と敵(矩形)のヒットチェック
// 矩形どうしの重なり判定
bool AABBOverlap(OBJECT ObjRect1, OBJECT ObjRect2)
{
    float dx = abs((ObjMomo.x + ObjMomo.width / 2) - (ObjRect2.x + ObjRect2.width / 2));
    float dy = abs((ObjMomo.y + ObjMomo.height / 2) - (ObjRect2.y + ObjRect2.height / 2));
    if ((dx <= ObjMomo.width / 2 + ObjRect2.width / 2) && (dy <= ObjMomo.height / 2 + ObjRect2.height / 2)) {
        return true;
    }

    return false;
}

// モモがダメージを受けたときの演出
void DrawDamageToMomo()
{
    // ダメージ演出
    SetDrawBlendMode(DX_BLENDMODE_ADD, 255);   // 色を加算する設定
    int col = GetColor(rand() % 256, rand() % 256, rand() % 256); // 確認用
    DrawBox(ObjMomo.x - nCameraX, ObjMomo.y, ObjMomo.x - nCameraX + ObjMomo.width, ObjMomo.y + ObjMomo.height, col, TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // 通常の描画に戻す
}

// 敵がダメージを受けたときの演出
void DrawDamageToEnemy(ENEMY ObjDmgEnemy)
{
    // 被ダメージの敵描画
    if (ObjDmgEnemy.nDirection == DIRECTION_L) {
        DrawGraph(ObjDmgEnemy.Obj.x - nCameraX, ObjDmgEnemy.Obj.y, ObjDmgEnemy.nDmgImg_L, TRUE);
    }
    else {
        DrawGraph(ObjDmgEnemy.Obj.x - nCameraX, ObjDmgEnemy.Obj.y, ObjDmgEnemy.nDmgImg_R, TRUE);
    }

    // ダメージ演出
    SetDrawBlendMode(DX_BLENDMODE_ADD, 255);   // 色を加算する設定
    int col = GetColor(rand() % 256, rand() % 256, rand() % 256); // 確認用
    DrawBox(ObjDmgEnemy.Obj.x - nCameraX, 
            ObjDmgEnemy.Obj.y, 
            ObjDmgEnemy.Obj.x - nCameraX + ObjDmgEnemy.Obj.width, 
            ObjDmgEnemy.Obj.y + ObjDmgEnemy.Obj.height, 
            col, 
            TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // 通常の描画に戻す 
}


