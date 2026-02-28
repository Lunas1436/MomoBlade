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

        // 敵描画
        DrawEnemy();

        // 以下をうまいこと関数したい...
        // モモのヒットチェック
        if (bHitToMomo) { // ヒット中
            nMomoDamagedTimer++;
            DrawDamageToMomo(); // モモのダメージエフェクト
            if (nMomoDamagedTimer > 60) { // ダメージエフェクトは60フレーム
                nMomoDamagedTimer = 0;
                bHitToMomo = false;
            }
        }
        else {
            if (HitCheckToMomo()) { 
                bHitToMomo = true;
                nCurrentHP--;
            }
        }

        // 敵のヒットチェック
        if (bHitToEnemy) {
            nEnemyDamagedTimer++;
            DrawDamageToEnemy(); // 敵のダメージエフェクト
            if (nEnemyDamagedTimer > 60) {
                nEnemyDamagedTimer = 0;
                bHitToEnemy = false;
            }
        }
        else {
            if (HitCheckToEnemy()) { // 剣の先端が敵の矩形の中にあればヒット
                bHitToEnemy = true;
            }
        }

        ScreenFlip(); // 裏画面の内容を表画面に反映させる
        if (ProcessMessage() == -1) break; // Windowsから情報を受け取りエラーが起きたら終了
        if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break; // ESCキーが押されたら終了
    }

    DxLib_End(); // ＤＸライブラリ使用の終了処理
    return 0;
}

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
    nAttackingTimer = 0;
    bIsAttacking = false;
    dSwordAngle = 0;

    // 確認用
    // 敵1
    SetObjParameter(&ObjEnemy1, 1000, ObjMomo.y, 0.0f, 0.0f, "Image/Enemy/Enemy1.png");
    ObjEnemy1.y += ObjEnemy1.height / 2;
    // 敵2
    SetObjParameter(&ObjEnemy2, 1000, 100, 0.0f, 0.0f, "Image/Enemy/Enemy2.png");
    ObjEnemy2.y += ObjEnemy2.height / 2;
    // 敵3
    SetObjParameter(&ObjEnemy3, 1100, ObjMomo.y, 0.0f, 0.0f, "Image/Enemy/Enemy3.png");
    ObjEnemy3.y += ObjEnemy3.height / 2;
    // 敵1がダメージを受けたとき用
    nDamagedEnemy = LoadGraph("Image/Enemy/DamagedEnemy1.png");

    ObjEnemyList[0] = ObjEnemy1;
    ObjEnemyList[1] = ObjEnemy2;
    ObjEnemyList[2] = ObjEnemy3;

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

    // スペースキー
    if (CheckHitKey(KEY_INPUT_SPACE)) {
        //if (bIsAttacking == false) {
        //    bIsAttacking = true;
        //}

        if (bOnLand) {
            fMaxY = ObjMomo.y - 100; // とりあえず現在地から100ドットをジャンプの上限とする
            bOnLand = false;
            bOnGround = false;
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
    nAttackingTimer += 10;
    if (nAttackingTimer > 90) { // 斬撃モーション終了
        bIsAttacking = false;
        nAttackingTimer = 0;
        dSwordAngle = 0;
        StopSoundMem(nSlashBGM, DX_PLAYTYPE_LOOP);
        return;
    }

    PlaySoundMem(nSlashBGM, DX_PLAYTYPE_LOOP);
    dSwordAngle = nAttackingTimer * (3.14 / 180);
}

// 敵描画
void DrawEnemy()
{
    for (int i = 0; i < 3; i++) {
        if (nCameraX < ObjEnemyList[i].x && ObjEnemyList[i].x < nCameraX + SCREEN_WIDTH) { // ウィンドウ内にあるときに描画
            if (nEnemyDamagedTimer == 0) {
                DrawGraph(ObjEnemyList[i].x - nCameraX, ObjEnemyList[i].y, ObjEnemyList[i].img, TRUE);
            }
        }
    }

}

// モモと敵とのヒットチェック
bool HitCheckToMomo()
{
    int dx = abs((ObjMomo.x + ObjMomo.width / 2) - (ObjEnemy1.x + ObjEnemy1.width / 2));
    int dy = abs((ObjMomo.y + ObjMomo.height / 2) - (ObjEnemy1.y + ObjEnemy1.height / 2));
    if ((dx <= ObjMomo.width / 2 + ObjEnemy1.width / 2) && (dy <= ObjMomo.height / 2 + ObjEnemy1.height / 2)) {
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

// 敵と剣先とのヒットチェック
bool HitCheckToEnemy()
{
    // 剣先の座標
    // 剣の画像の左下からの座標
    double TipX = ObjSword.x + dSwordLength * cos(dSwordAngle);
    double TipY = ObjSword.y + ObjSword.height + dSwordLength * sin(dSwordAngle);

    // 剣先が敵矩形の中にあるか判定
    if (TipX >= ObjEnemy1.x - nCameraX && TipX <= ObjEnemy1.x - nCameraX + ObjEnemy1.width &&
        TipY >= ObjEnemy1.y && TipY <= ObjEnemy1.y + ObjEnemy1.height) {
        return true;
    }

    return false;
}

// 敵がダメージを受けたときの演出
void DrawDamageToEnemy()
{
    DrawGraph(ObjEnemy1.x - nCameraX, ObjEnemy1.y, nDamagedEnemy, TRUE); // 被ダメージの敵描画

    // ダメージ演出
    SetDrawBlendMode(DX_BLENDMODE_ADD, 255);   // 色を加算する設定
    int col = GetColor(rand() % 256, rand() % 256, rand() % 256); // 確認用
    DrawBox(ObjEnemy1.x - nCameraX, ObjEnemy1.y, ObjEnemy1.x - nCameraX + ObjEnemy1.width, ObjEnemy1.y + ObjEnemy1.height, col, TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // 通常の描画に戻す 
}


