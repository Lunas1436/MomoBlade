#include "DxLib.h"
#include "GameMain.h"

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

        // 確認用
        DrawGraph(20, 20, nHP, TRUE);
        DrawGraph(20 + 32*1 + 5, 20, nHP, TRUE);
        DrawGraph(20 + 32*2 + 10, 20, nHP, TRUE);

        // プレイヤー入力
        PlayerInput();

        // ジャンプ上昇中か落下中か判定
        CheckJumpState();
        
        nCameraX = (ObjMomo.x + ObjMomo.width / 2) - SCREEN_WIDTH / 2;
        if (nCameraX < 0) {
            nCameraX = 0;
        }
        else if (nCameraX + SCREEN_WIDTH > STAGE_WIDTH) {
            nCameraX = STAGE_WIDTH - SCREEN_WIDTH;
        }

        // モモ描画
        DrawGraph(ObjMomo.x - nCameraX, ObjMomo.y, ObjMomo.img, TRUE);

        // 敵描画確認用
        if (nCameraX < ObjEnemy.x && ObjEnemy.x < nCameraX + SCREEN_WIDTH) { // ウィンドウ内にあるときに描画
            if (nDamagingTimer == 0) {
                DrawGraph(ObjEnemy.x - nCameraX, ObjEnemy.y, ObjEnemy.img, TRUE);
            }
        }

        // 剣描画
        ObjSword.x = ObjMomo.x - nCameraX + ObjMomo.width + 10;
        ObjSword.y = ObjMomo.y - 20;
        if (bIsAttacking) { // 斬撃モーション
            SwordAttack();
        }
        DrawRotaGraph2(ObjSword.x, ObjSword.y + ObjSword.height, 0, ObjSword.height, 1.0, dSwordAngle, ObjSword.img, TRUE);

        // ヒットチェック
        if (bIsHit == false) { // 剣の先端が敵の矩形の中にあればヒット
            if (HitCheckToEnemy()) {
                bIsHit = true;
                //bDamaging = true;
            }
        }

        // ダメージ演出
        if (bIsHit) {
            nDamagingTimer++;
            DamageEnemy();
            if (nDamagingTimer > 60) {
                nDamagingTimer = 0;
                bIsHit = false;
                //bDamaging = false;
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

    // モモタロー
    SetObjParameter(&ObjMomo, 100, 0.0f, 5.0f, 0.0f, "Image/Momo2.png");
    ObjMomo.y = ObjGround.y - ObjMomo.height;

    nHP = LoadGraph("Image/HP.png");
    

    // 剣
    SetObjParameter(&ObjSword, 0.0f, 0.0f, 0.0f, 0.0f, "Image/Sword2.png");
    dSwordLength = ObjSword.width * 1.41;
    nAttackingTimer = 0;
    bIsAttacking = false;
    dSwordAngle = 0;

    // 敵1
    SetObjParameter(&ObjEnemy, 1000, ObjMomo.y, 0.0f, 0.0f, "Image/Enemy1.png");
    ObjEnemy.y += ObjEnemy.height / 2;
    // 敵1がダメージを受けたとき用
    nDamagedEnemy = LoadGraph("Image/DamagedEnemy1.png");
    bIsHit = false;
    nDamagingTimer = 0;

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
    if (TipX >= ObjEnemy.x - nCameraX && TipX <= ObjEnemy.x - nCameraX + ObjEnemy.width &&
        TipY >= ObjEnemy.y && TipY <= ObjEnemy.y + ObjEnemy.height) {
        return true;
    }

    return false;
}

// 敵がダメージを受けたときの演出
void DamageEnemy()
{
    DrawGraph(ObjEnemy.x - nCameraX, ObjEnemy.y, nDamagedEnemy, TRUE); // 被ダメージの敵描画

    // ダメージ演出
    SetDrawBlendMode(DX_BLENDMODE_ADD, 255);   // 色を加算する設定
    int col = GetColor(rand() % 256, rand() % 256, rand() % 256); // 確認用
    DrawBox(ObjEnemy.x - nCameraX, ObjEnemy.y, ObjEnemy.x - nCameraX + ObjEnemy.width, ObjEnemy.y + ObjEnemy.height, col, TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // 通常の描画に戻す 
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

