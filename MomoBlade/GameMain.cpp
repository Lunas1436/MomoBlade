#include "DxLib.h"
#include "GameMain.h"

#include <string>
#include <cmath>

using namespace std;

// 確認用
bool bIsHitToMomo = false;
int nMomoDamagedTimer = 0;

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

        // HP描画
        DrawHP();

        // ゴールフラッグ描画
        DrawGraph(ObjGoalFlag.x - nCameraX, ObjGoalFlag.y, ObjGoalFlag.img, TRUE);

        // 敵描画確認用
        DrawEnemy();
        if (nCameraX < ObjEnemy1.x && ObjEnemy1.x < nCameraX + SCREEN_WIDTH) { // ウィンドウ内にあるときに描画
            if (nDamagingTimer == 0) {
                DrawGraph(ObjEnemy1.x - nCameraX, ObjEnemy1.y, ObjEnemy1.img, TRUE);
            }
        }

        if (nCameraX < ObjEnemy2.x && ObjEnemy2.x < nCameraX + SCREEN_WIDTH) { // ウィンドウ内にあるときに描画
            if (nDamagingTimer == 0) {
                DrawGraph(ObjEnemy2.x - nCameraX, ObjEnemy2.y, ObjEnemy2.img, TRUE);
            }
        }

        // ヒットチェック
        if (bIsHit == false) { // 剣の先端が敵の矩形の中にあればヒット
            if (HitCheckToEnemy()) {
                bIsHit = true;
            }
        }

        // ダメージ演出
        if (bIsHit) {
            nDamagingTimer++;
            DamageToEnemy();
            if (nDamagingTimer > 60) {
                nDamagingTimer = 0;
                bIsHit = false;
            }
        }

        // モモのダメージ計算
        if (bIsHitToMomo == false) {
            if (HitCheckToMomo()) {
                bIsHitToMomo = true;
                nCurrentHP--;
            }
        }

        if (bIsHitToMomo) {
            nMomoDamagedTimer++;
            DamageToMomo();
            if (nMomoDamagedTimer > 60) {
                nMomoDamagedTimer = 0;
                bIsHitToMomo = false;
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
    SetObjParameter(&ObjSky, 0.0f, 0.0f, 0.5f, 0.0f, "Image/Sky.png");

    // 地面
    SetObjParameter(&ObjGround, 0.0f, 0.0f, 0.0f, 0.0f, "Image/Ground_1.png");
    ObjGround.y = SCREEN_HEIGHT - ObjGround.height * 3;

    // 地中
    SetObjParameter(&ObjUnderGround, ObjGround.x, 0.0f, 0.0f, 0.0f, "Image/UnderGround.png");
    ObjUnderGround.y = ObjGround.y + ObjUnderGround.height;

    // ゴールフラッグ
    SetObjParameter(&ObjGoalFlag, 1500, 0.0f, 0.0f, 0.0f, "Image/GoalFlag.png");
    ObjGoalFlag.y = ObjGround.y - ObjGoalFlag.height;

    // モモタロー
    SetObjParameter(&ObjMomo, 100, 0.0f, 5.0f, 0.0f, "Image/Momo2.png");
    ObjMomo.y = ObjGround.y - ObjMomo.height;

    // HP
    string strHPpath = "";
    for (int i = 0; i < 5; i++) {
        strHPpath = "Image/HP_" + std::to_string(i) + ".png";
        nImgHP[i] = LoadGraph(strHPpath.c_str());
    }
    int nTempH = 0; 
    GetGraphSize(nImgHP[0], &nHPWidth, &nTempH);
    nHPX = 20, nHPY = 20; // とりあえずの座標
    nCurrentHP = HP_MAX * 4;
    
    // 剣
    SetObjParameter(&ObjSword, 0.0f, 0.0f, 0.0f, 0.0f, "Image/Sword2.png");
    dSwordLength = ObjSword.width * 1.41;
    nAttackingTimer = 0;
    bIsAttacking = false;
    dSwordAngle = 0;

    // 確認用
    // 敵1
    SetObjParameter(&ObjEnemy1, 1000, ObjMomo.y, 0.0f, 0.0f, "Image/Enemy1.png");
    ObjEnemy1.y += ObjEnemy1.height / 2;
    // 敵2
    SetObjParameter(&ObjEnemy2, 1000, 100, 0.0f, 0.0f, "Image/Enemy2.png");
    ObjEnemy2.y += ObjEnemy2.height / 2;
    // 敵3
    SetObjParameter(&ObjEnemy3, 1100, ObjMomo.y, 0.0f, 0.0f, "Image/Enemy3.png");
    ObjEnemy3.y += ObjEnemy3.height / 2;
    // 敵1がダメージを受けたとき用
    nDamagedEnemy = LoadGraph("Image/DamagedEnemy1.png");
    bIsHit = false;
    nDamagingTimer = 0;

    ObjEnemyList[0] = ObjEnemy1;
    ObjEnemyList[1] = ObjEnemy2;
    ObjEnemyList[2] = ObjEnemy3;

    // ステージBGM
    nStageBGM = LoadSoundMem("Sound/StageBGM.wav");

    // 斬撃BGM
    nSlashBGM = LoadSoundMem("Sound/SlashBGM.wav");

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

// モモと剣描画
void DrawMomo()
{
    // ジャンプ上昇中か落下中か判定
    CheckJumpState();

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

// 敵描画
void DrawEnemy()
{
    for (int i = 0; i < 3; i++) {
        if (nCameraX < ObjEnemyList[i].x && ObjEnemyList[i].x < nCameraX + SCREEN_WIDTH) { // ウィンドウ内にあるときに描画
            if (nDamagingTimer == 0) {
                DrawGraph(ObjEnemyList[i].x - nCameraX, ObjEnemyList[i].y, ObjEnemyList[i].img, TRUE);
            }
        }
    }

}

// ジャンプ状態チェック
void CheckJumpState()
{
    // ジャンプアップ中
    if (bJumpUp) {
        ObjMomo.vy -= JUMP_UP_POWER;
    }
    // ジャンプ落下中
    if (bJumpDown) {
        ObjMomo.vy += GRAVITEY;
    }

    ObjMomo.y += ObjMomo.vy;
    if (ObjMomo.y - ObjMomo.height / 2 <= 200) { // とりあえず天井を200に設定 
        bJumpUp = false;
        bJumpDown = true;
    }

    if (bJumpDown && ObjMomo.y + ObjMomo.height >= ObjGround.y) { // 着地
        ObjMomo.y = ObjGround.y - ObjMomo.height;
        ObjMomo.vy = 0;
        bJumpDown = false;
    }
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
void DamageToMomo()
{
    // ダメージ演出
    SetDrawBlendMode(DX_BLENDMODE_ADD, 255);   // 色を加算する設定
    int col = GetColor(rand() % 256, rand() % 256, rand() % 256); // 確認用
    DrawBox(ObjMomo.x - nCameraX, ObjMomo.y, ObjMomo.x - nCameraX + ObjMomo.width, ObjMomo.y + ObjMomo.height, col, TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // 通常の描画に戻す
}

// 敵がダメージを受けたときの演出
void DamageToEnemy()
{
    DrawGraph(ObjEnemy1.x - nCameraX, ObjEnemy1.y, nDamagedEnemy, TRUE); // 被ダメージの敵描画

    // ダメージ演出
    SetDrawBlendMode(DX_BLENDMODE_ADD, 255);   // 色を加算する設定
    int col = GetColor(rand() % 256, rand() % 256, rand() % 256); // 確認用
    DrawBox(ObjEnemy1.x - nCameraX, ObjEnemy1.y, ObjEnemy1.x - nCameraX + ObjEnemy1.width, ObjEnemy1.y + ObjEnemy1.height, col, TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // 通常の描画に戻す 
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

}

// プレイヤー入力
void PlayerInput()
{
    //XINPUT_STATE input;

    //float lx, ly;
    //if (GetJoypadXInputState(DX_INPUT_PAD1, &input) == 0) {
    //    lx = (float)input.ThumbLX;
    //    lx /= 32767.0f;
    //    ObjMomo.x += (int)(lx * ObjMomo.vx);

    //    // Yボタンで斬撃
    //    if (input.Buttons & XINPUT_BUTTON_Y) {
    //        if (bIsAttacking == false) {
    //            bIsAttacking = true;
    //        }
    //    }

    //    // Bボタンでジャンプ
    //    if (input.Buttons & XINPUT_BUTTON_B) {
    //        if (!bJumpUp && !bJumpDown) {
    //            bJumpUp = true;
    //        }
    //    }
    //}

    // →キー
    if (CheckHitKey(KEY_INPUT_RIGHT)) {
        ObjMomo.x += ObjMomo.vx;
    }

    // ←キー
    if (CheckHitKey(KEY_INPUT_LEFT)) {
        ObjMomo.x -= ObjMomo.vx;
        if (ObjMomo.x < 0 ) { 
            ObjMomo.x = 0;
        }
    }

    // スペースキー
    if (CheckHitKey(KEY_INPUT_SPACE)) {
        if (bIsAttacking == false) {
            bIsAttacking = true;
        }

        // スペースキーを攻撃モーションに割り当ててるのでいったんコメントアウト
        //if (!bJumpUp && !bJumpDown) {
        //    bJumpUp = true;
        //}
    }
}


