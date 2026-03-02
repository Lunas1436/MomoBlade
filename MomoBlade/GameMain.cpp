#include "DxLib.h"
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

        // ゴールフラッグ描画
        DrawGraph(ObjGoalFlag.x - nCameraX, ObjGoalFlag.y, ObjGoalFlag.img, TRUE);

        // HP描画
        DrawHP();

        // プレイヤー入力
        PlayerInput();

        // カメラ座標(モモを中心)
        nCameraX = (ObjMomo.x + ObjMomo.width / 2) - SCREEN_WIDTH / 2;
        if (nCameraX < 0) {
            nCameraX = 0;
        }
        else if (nCameraX + SCREEN_WIDTH > STAGE_WIDTH) {
            nCameraX = STAGE_WIDTH - SCREEN_WIDTH;
        }

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
    SetObjParameter(&ObjSky, 0.0f, 0.0f, 0.5f, 0.0f, "Image/Stage/Sky.png");

    // 地面
    SetObjParameter(&ObjGround, 0.0f, 0.0f, 0.0f, 0.0f, "Image/Stage/Ground.png");
    ObjGround.y = SCREEN_HEIGHT - ObjGround.height * 3;

    // 地中
    SetObjParameter(&ObjUnderGround, ObjGround.x, 0.0f, 0.0f, 0.0f, "Image/Stage/UnderGround.png");
    ObjUnderGround.y = ObjGround.y + ObjUnderGround.height;

    // ブロック
    SetObjParameter(&ObjBlock, 0.0f, 0.0f, 0.0f, 0.0f, "Image/Stage/Block.png");
    ObjBlock.x = 500;
    ObjBlock.y = ObjGround.y - 150;
    ObjBlockList.push_back(ObjBlock);

    // ブロック配置
    ObjBlock.x = 700;
    ObjBlock.y -= 150;
    ObjBlockList.push_back(ObjBlock);

    ObjBlock.x = 800;
    ObjBlock.y -= 100;
    ObjBlockList.push_back(ObjBlock);

    // ゴールフラッグ
    SetObjParameter(&ObjGoalFlag, STAGE_WIDTH - 200, 0.0f, 0.0f, 0.0f, "Image/Stage/GoalFlag.png");
    ObjGoalFlag.y = ObjGround.y - ObjGoalFlag.height;

    // モモタロー
    SetObjParameter(&ObjMomo, 100, 0.0f, 5.0f, 0.0f, "Image/Momo/Momo.png");
    ObjMomo.y = ObjGround.y - ObjMomo.height;

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
    
    // 剣
    SetObjParameter(&ObjSword, 0.0f, 0.0f, 0.0f, 0.0f, "Image/Momo/Sword.png");
    dSwordLength = ObjSword.width * 1.41;
    dSwordAngle = 0;

    // 敵
    SetObjParameter(&ObjEnemyList[0].Obj, 1000, ObjMomo.y, -2.5f, 0.0f, "Image/Enemy/Enemy1_L.png");
    ObjEnemyList[0].Obj.y += ObjEnemyList[0].Obj.height / 2;
    ObjEnemyList[0].nDirection = DIRECTION_L;
    ObjEnemyList[0].fRangeL = 500;
    ObjEnemyList[0].fRangeR = 1000;

    SetObjParameter(&ObjEnemyList[1].Obj, 1000, 100, 0.0f, 0.0f, "Image/Enemy/Enemy2_L.png");
    ObjEnemyList[1].Obj.y += ObjEnemyList[1].Obj.height / 2;
    SetObjParameter(&ObjEnemyList[2].Obj, 1100, ObjMomo.y, 0.0f, 0.0f, "Image/Enemy/Enemy3_L.png");
    ObjEnemyList[2].Obj.y += ObjEnemyList[2].Obj.height / 2;
    
    // 敵の状態画像読込み
    SetEnemyImgData();

    // ステージBGM
    nStageBGM = LoadSoundMem("Sound/Stage/StageBGM.wav");

    // 斬撃BGM
    nSlashBGM = LoadSoundMem("Sound/Momo/SlashBGM.wav");

}

// OBJECTのパラメータセット
// 座標、速度、画像のファイルパス
void SetObjParameter(OBJECT *pObj, float fx, float fy, float fvx, float fvy, const char *pchImg)
{
    pObj->img = LoadGraph(pchImg);
    if (pObj->img < 0) {
        // 読込み失敗
    }
    GetGraphSize(pObj->img, &pObj->width, &pObj->height);
    pObj->x = fx;
    pObj->y = fy;
    pObj->vx = fvx;
    pObj->vy = fvy;
}

// 敵の状態画像読込み
void SetEnemyImgData()
{
    string strPath = "";
    string strTemp = "";
    for (int i = 0; i < 3; i++) {
        strTemp = "Image/Enemy/Enemy" + std::to_string(i + 1) + "_";

        // 通常時
        // 左向き
        strPath = strTemp + "L.png";
        ObjEnemyList[i].nImg_L = LoadGraph(strPath.c_str());
        // 右向き
        strPath = strTemp + "R.png";
        ObjEnemyList[i].nImg_R = LoadGraph(strPath.c_str());
        
        // 被攻撃時
        // 左向き
        strPath = strTemp + "Damaged_L.png";
        ObjEnemyList[i].nDmgImg_L = LoadGraph(strPath.c_str());
        // 右向き
        strPath = strTemp + "Damaged_R.png";
        ObjEnemyList[i].nDmgImg_R = LoadGraph(strPath.c_str());
    }
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
    ObjSky.x += ObjSky.vx;
    if (ObjSky.x >= SCREEN_WIDTH) {
        ObjSky.x -= SCREEN_WIDTH;
    }
    DrawGraph(ObjSky.x - ObjSky.width, ObjSky.y, ObjSky.img, TRUE);
    DrawGraph(ObjSky.x, ObjSky.y, ObjSky.img, TRUE);

    // 地面描画
    for (int i = 0; i < STAGE_WIDTH / ObjGround.width; i++) {
        DrawGraph(i * ObjGround.width - nCameraX, ObjGround.y, ObjGround.img, TRUE); // 地面
        DrawGraph(i * ObjUnderGround.width - nCameraX, ObjGround.y + ObjGround.height, ObjUnderGround.img, TRUE); // 地中
    }

    // ブロック描画
    for (int i = 0; i < ObjBlockList.size(); i++) {
        DrawGraph(ObjBlockList[i].x - nCameraX, ObjBlockList[i].y, ObjBlock.img, TRUE);
    }
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
        if (bOnLand) {
            fMaxY = ObjMomo.y - 100; // とりあえず現在地から100ドットをジャンプの上限とする
            bOnLand = false;
            bOnGround = false;
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
    float fPrevTop = ObjMomo.y;
    float fPrevBottom = ObjMomo.y + ObjMomo.height;

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

    for (int i = 0; i < 3; i++) {
        ENEMY* pEnemy = &ObjEnemyList[i];
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

}

// 敵描画
void DrawEnemy()
{
    for (int i = 0; i < 3; i++) {
        if (nCameraX < ObjEnemyList[i].Obj.x && ObjEnemyList[i].Obj.x < nCameraX + SCREEN_WIDTH) { // ウィンドウ内にあるときに描画
            if (i == nDmgIndex) continue;
            if (ObjEnemyList[i].nDirection == DIRECTION_L) {
                DrawGraph(ObjEnemyList[i].Obj.x - nCameraX, ObjEnemyList[i].Obj.y, ObjEnemyList[i].nImg_L, TRUE);
            }
            else {
                DrawGraph(ObjEnemyList[i].Obj.x - nCameraX, ObjEnemyList[i].Obj.y, ObjEnemyList[i].nImg_R, TRUE);
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


