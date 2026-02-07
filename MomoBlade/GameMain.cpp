#include "DxLib.h"
#include "GameMain.h"

using namespace std;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    SetWindowText("ObjMomo BLADE"); // ウィンドウのタイトル
    SetGraphMode(WIDTH, HEIGHT, 32); // ウィンドウの大きさとカラービット数の指定
    ChangeWindowMode(TRUE); // ウィンドウモードで起動
    if (DxLib_Init() == -1) return -1; // ライブラリ初期化 エラーが起きたら終了
    SetBackgroundColor(0, 0, 0); // 背景色の指定
    SetDrawScreen(DX_SCREEN_BACK); // 描画面を裏画面にする

    Init();

    while (1)
    {
        ClearDrawScreen(); // 画面クリア

        PlayerInput();

        if (bJumpUp) {
            ObjMomo.vy -= JUMP_UP_POWER;
        }
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
        
        // ステージ描画
        DrawStage();

        // モモ描画
        DrawGraph(ObjMomo.x, ObjMomo.y, ObjMomo.img, TRUE);
       

        ScreenFlip(); // 裏画面の内容を表画面に反映させる
        if (ProcessMessage() == -1) break; // Windowsから情報を受け取りエラーが起きたら終了
        if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break; // ESCキーが押されたら終了
    }

    DxLib_End(); // ＤＸライブラリ使用の終了処理
    return 0;
}

void Init()
{
    // ステージ
    // 空
    ObjSky.img = LoadGraph("Image/Sky.png");
    if (ObjSky.img < 0) {
        // 読込み失敗
    }
    GetGraphSize(ObjSky.img, &ObjSky.width, &ObjSky.height);
    ObjSky.x = 0;
    ObjSky.y = 0;
    ObjSky.vx = 0.5f; // 雲の移動速度
    ObjSky.vy = 0;

    // 地面
    ObjGround.img = LoadGraph("Image/Ground.png");
    if (ObjGround.img < 0) {
        // 読込み失敗
    }
    GetGraphSize(ObjGround.img, &ObjGround.width, &ObjGround.height);
    ObjGround.x = 0;
    ObjGround.y = HEIGHT - ObjGround.height * 3; // HEIGHT - 192

    // 地中
    ObjUnderGround.img = LoadGraph("Image/UnderGround.png");
    if (ObjUnderGround.img < 0) {
        // 読込み失敗
    }
    GetGraphSize(ObjUnderGround.img, &ObjUnderGround.width, &ObjUnderGround.height);
    ObjUnderGround.x = ObjGround.x;
    ObjUnderGround.y = ObjGround.y + ObjGround.height;
    

    // モモタロー
    ObjMomo.img = LoadGraph("Image/Momo.png");
    if (ObjMomo.img < 0) {
        // 読込み失敗
    }
    // サイズ
    GetGraphSize(ObjMomo.img, &ObjMomo.width, &ObjMomo.height);

    // 初期座標(左上基準)
    ObjMomo.x = 100;
    ObjMomo.y = ObjGround.y - ObjMomo.height;
    // 初期速度
    ObjMomo.vx = 5;
    ObjMomo.vy = 0;

}

// ステージ描画
void DrawStage()
{
    // 空描画
    ObjSky.x += ObjSky.vx;
    if (ObjSky.x >= WIDTH) {
        ObjSky.x -= WIDTH;
    }
    DrawGraph(ObjSky.x - ObjSky.width, ObjSky.y, ObjSky.img, TRUE);
    DrawGraph(ObjSky.x, ObjSky.y, ObjSky.img, TRUE);

    // 地面描画
    for (int i = 0; i < 30; i++) {
        DrawGraph(i * ObjGround.width, ObjGround.y, ObjGround.img, TRUE);
        for (int j = 0; j < 3; j++) {
            DrawGraph(i * ObjUnderGround.width, ObjUnderGround.y + j * ObjUnderGround.height, ObjUnderGround.img, TRUE); // 地中描画
        }
    }
}


void PlayerInput()
{
    // →キー
    if (CheckHitKey(KEY_INPUT_RIGHT)) {
        ObjMomo.x += ObjMomo.vx;
        if (ObjMomo.x + ObjMomo.width > WIDTH) {
            ObjMomo.x = WIDTH - ObjMomo.width;
        }
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
        if (!bJumpUp && !bJumpDown) {
            bJumpUp = true;
        }
    }

}

