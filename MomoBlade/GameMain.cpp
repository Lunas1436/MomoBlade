#include "DxLib.h"
#include "GameMain.h"

using namespace std;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    SetWindowText("MOMO BLADE"); // ウィンドウのタイトル
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
            Momo.vy -= JUMP_UP_POWER;
        }
        if (bJumpDown) {
            Momo.vy += GRAVITEY;
        }

        Momo.y += Momo.vy;
        if (Momo.y - Momo.height / 2 <= 200) { // とりあえず天井を200に設定 
            bJumpUp = false;
            bJumpDown = true;
        }

        if (bJumpDown && Momo.y + Momo.height / 2 >= HEIGHT - 300) { // 着地
            Momo.y = HEIGHT - 300;
            Momo.vy = 0;
            IsGround = true;
            bJumpDown = false;
        }

        // モモ描画
        DrawGraph(Momo.x - Momo.width / 2, Momo.y - Momo.height / 2, Momo.img, TRUE);
        
        // 地面描画確認用
        DrawStage();

        ScreenFlip(); // 裏画面の内容を表画面に反映させる
        if (ProcessMessage() == -1) break; // Windowsから情報を受け取りエラーが起きたら終了
        if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break; // ESCキーが押されたら終了
    }

    DxLib_End(); // ＤＸライブラリ使用の終了処理
    return 0;
}

void Init()
{
    // モモタロー
    // 画像読込み
    Momo.img = LoadGraph("Image/Momo.png");
    if (Momo.img < 0) {
        // 読込み失敗
    }

    nImgGround = LoadGraph("Image/Ground.png");
    if (nImgGround < 0) {
        // 読込み失敗
    }

    // サイズ
    GetGraphSize(Momo.img, &Momo.width, &Momo.height);

    // 初期位置(画像の中心座標)
    Momo.x = 100;
    Momo.y = HEIGHT - 300;
    // 初期速度
    Momo.vx = 5;
    Momo.vy = 0;

}

void DrawStage()
{
    // 地面描画
    for (int i = 0; i < 30; i++) {
        DrawGraph(i * 64, HEIGHT - 300 + Momo.height / 2, nImgGround, TRUE);
    }
}


void PlayerInput()
{
    // →キー
    if (CheckHitKey(KEY_INPUT_RIGHT)) {
        Momo.x += Momo.vx;
        if (Momo.x + Momo.width / 2 > WIDTH) {
            Momo.x = WIDTH - Momo.width / 2;
        }
    }

    // ←キー
    if (CheckHitKey(KEY_INPUT_LEFT)) {
        Momo.x -= Momo.vx;
        if (Momo.x - Momo.width / 2 < 0 ) {
            Momo.x = Momo.width / 2;
        }
    }

    // スペースキー
    if (CheckHitKey(KEY_INPUT_SPACE)) {
        //if (IsGround) {
        //    Momo.y = JUMP_POWER;
        //    IsGround = false;
        //}
        if (!bJumpUp && !bJumpDown) {
            bJumpUp = true;
        }
    }

}

