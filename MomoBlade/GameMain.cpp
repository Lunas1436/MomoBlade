#include "GameMain.h"
#include <string>
#include <cmath>


// ステージ
const int STAGE_WIDTH = 3000;
const int STAGE_HEIGHT = 600;

// スクリーン
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 600;

// ゲームステート
enum {
    GAME_START, // ゲームスタート
    GAME_PLAY,  // ゲームプレイ
    GAME_CLEAR, // ゲームクリア
    GAME_OVER,  // ゲームオーバー
};
int nGameState = GAME_START;

// 確認用
int nCnt = 0;

const char* pchTitle = "MOMO BLADE";
int nTitleWidth = 0, nTitleHeight = 0;
int nTitleFont = 0;

const char* pchText = "GAME START";
int nTextWidth = 0, nTextHeight = 0;
int nTextFont = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    SetWindowText("MOMO BLADE"); // ウィンドウのタイトル
    SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32); // ウィンドウの大きさとカラービット数の指定
    ChangeWindowMode(TRUE); // ウィンドウモードで起動
    if (DxLib_Init() == -1) return -1; // ライブラリ初期化 エラーが起きたら終了
    SetBackgroundColor(0, 0, 0); // 背景色の指定
    SetDrawScreen(DX_SCREEN_BACK); // 描画面を裏画面にする

    // 初期化
    InitGameString();
    InitGamePlay();

    while (1)
    {
        // 画面クリア
        ClearDrawScreen();

        // GAME_PLAY に入ったら BGM を再生、離れたら停止する
        static int nPrevState = -1;
        if (nGameState != nPrevState) {
            if (nGameState == GAME_PLAY) {
                PlaySoundMem(nStageBGM, DX_PLAYTYPE_LOOP);
            }
            else if (nPrevState == GAME_PLAY) {
                StopSoundMem(nStageBGM);
            }
            nPrevState = nGameState;
        }

        switch (nGameState) {
        case GAME_START: // ゲームスタート
            GameStart();
            break;

        case GAME_PLAY:  // ゲームプレイ           
            GamePlay();
            break;
        
        case GAME_CLEAR: // ゲームクリア
            GameClear();
            break;

        case GAME_OVER:  // ゲームオーバー
            GameOver();
            break;
        }

        if (nGameState == GAME_CLEAR || nGameState == GAME_OVER) {
            ObjMomo.StopSound();
        }

        ScreenFlip(); // 裏画面の内容を表画面に反映させる
        if (ProcessMessage() == -1) break; // Windowsから情報を受け取りエラーが起きたら終了
        if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break; // ESCキーが押されたら終了
    }

    // 動的確保した敵オブジェクトの解放
    for (int i = 0; i < m_ObjEnemyList.size(); i++) {
        delete m_ObjEnemyList[i];
    }
    m_ObjEnemyList.clear();

    DxLib_End(); // ＤＸライブラリ使用の終了処理
    return 0;
}

// ゲームスタート
void GameStart()
{
    // タイトル画面表示
    DrawStringToHandle(SCREEN_WIDTH / 2 - nTitleWidth / 2, 125, pchTitle, GetColor(255, 255, 255), nTitleFont);

    // ゲームスタート表示（ホバーで色を変え、クリックでゲーム開始）
    int nTextX = SCREEN_WIDTH / 2 - nTextWidth / 2;
    int nTextY = 400;
    int nTextW = nTextWidth;
    int nTextH = nTextHeight;

    int mx, my;
    GetMousePoint(&mx, &my);

    bool bHover = (mx >= nTextX && mx <= nTextX + nTextW && my >= nTextY && my <= nTextY + nTextH);

    int col = bHover ? GetColor(255, 220, 0) : GetColor(255, 255, 255);
    DrawStringToHandle(nTextX, nTextY, pchText, col, nTextFont);

    // クリックでゲーム開始（左ボタンのエッジ検出）
    static bool s_prevLeftDown = false;
    int mouse = GetMouseInput();
    bool leftDown = (mouse & MOUSE_INPUT_LEFT) != 0;
    if (bHover && leftDown && !s_prevLeftDown) {
        nGameState = GAME_PLAY;
    }
    s_prevLeftDown = leftDown;
}

void InitGameString()
{
    // ゲームタイトル
    nTitleFont = CreateFontToHandle("メイリオ", 80, 6);
    nTitleWidth = GetDrawStringWidthToHandle(pchTitle, strlen(pchTitle), nTitleFont);
    nTitleHeight = GetFontSizeToHandle(nTitleFont);
    
    // ゲームスタート
    nTextFont = CreateFontToHandle("メイリオ", 40, 4);
    nTextWidth = GetDrawStringWidthToHandle(pchText, strlen(pchText), nTextFont);
    nTextHeight = GetFontSizeToHandle(nTextFont);
}

// ゲームプレイ
void GamePlay()
{
    // ステージ描画
    DrawStage();

    // プレイヤー入力
    PlayerInput();

    // カメラ座標(モモを中心)
    nCameraX = (ObjMomo.GetX() + ObjMomo.GetWidth() / 2) - SCREEN_WIDTH / 2;
    if (nCameraX < 0) {
        nCameraX = 0;
    }
    else if (nCameraX + SCREEN_WIDTH > STAGE_WIDTH) {
        nCameraX = STAGE_WIDTH - SCREEN_WIDTH;
    }

    // モモ描画
    DrawMomo();

    // ゲームクリア判定
    if (!ObjMomo.IsAttacking() && ObjMomo.GetFinishSlow()) {
        nGameState = GAME_CLEAR;
    }

    // 剣描画
    ObjMomo.DrawSword(nCameraX);

    // 敵更新
    UpdateEnemy();

    // HP
    if (!ObjMomo.UpdateHP()) {
        nGameState = GAME_OVER;
    }
    ObjMomo.DrawHP();

    // 敵描画
    DrawEnemy();

    // ヒットチェック
    CollisionCheck();
}

// ゲームクリア
void GameClear()
{
    pchText = "GAME CLEAR!!";
    int nTextX = SCREEN_WIDTH / 2 - nTextWidth / 2;
    DrawStringToHandle(nTextX, SCREEN_HEIGHT / 2 - nTextHeight / 2, pchText, GetColor(255, 220, 0), nTextFont);

    ObjMomo.StopSound();

}

// ゲームオーバー
void GameOver()
{
    pchText = "GAME OVER";
    int nTextX = SCREEN_WIDTH / 2 - nTextWidth;
    DrawStringToHandle(nTextX, SCREEN_HEIGHT / 2 - -nTextHeight / 2, pchText, GetColor(255, 0, 0), nTextFont);
}

// 各オブジェクトの情報を外部のパラメータファイルで持つようにする
// 初期化
void InitGamePlay()
{
    // ステージ
    // 空
    // 雲のX軸の移動速度を0.5f
    ObjSky.SetParameter(0.0f, 0.0f, 0.5f, 0.0f, "Image/Stage/Sky.png");

    // 地面
    ObjGround.SetParameter(0.0f, 0.0f, 0.0f, 0.0f, "Image/Stage/Ground.png");
    ObjGround.SetY(SCREEN_HEIGHT - ObjGround.GetHeight() * 3);
    nGroundY = ObjGround.GetY();

    // 地中
    ObjUnderGround.SetParameter(ObjGround.GetX(), 0.0f, 0.0f, 0.0f, "Image/Stage/UnderGround.png");
    ObjUnderGround.SetY(nGroundY + ObjUnderGround.GetHeight());

    // ブロック
    ObjBlock.SetParameter(500, nGroundY - 150, 0, 0, "Image/Stage/Block.png");
    ObjBlockList.push_back(ObjBlock);

    CObject Block = ObjBlockList[0];
    Block.SetX(1300);
    Block.SetY(nGroundY - 150);
    ObjBlockList.push_back(Block);

    // ゴールフラッグ
    ObjGoalFlag.SetParameter(STAGE_WIDTH - 200, 0.0f, 0.0f, 0.0f, "Image/Stage/GoalFlag.png");
    ObjGoalFlag.SetY(nGroundY - ObjGoalFlag.GetHeight());
    nBrokenGoal = LoadGraph("Image/Stage/GoalFlag_Broken.png");

    // モモタロー
    ObjMomo.SetParameter(100, 0.0f, 5.0f, 0.0f, "Image/Momo/Momo_R.png");
    ObjMomo.SetImages(
        "Image/Momo/Momo_L.png",
        "Image/Momo/Momo_R.png",
        "Image/Momo/Momo_L.png",
        "Image/Momo/Momo_R.png"
    );
    ObjMomo.SetY(nGroundY - ObjMomo.GetHeight());
    ObjMomo.SetOnLand(true);
    ObjMomo.SetOnGround(true);
    ObjMomo.SetDirection(DIRECTION_R);
    
    // 剣
    ObjMomo.InitSword(0.0f, 0.0f, 0.0f, 0.0f, 
        "Image/Momo/Sword_L.png", 
        "Image/Momo/Sword_R.png", 
        "Sound/Momo/SlashBGM.wav"
    );

    // HP
    vector<filesystem::path> pathList;
    pathList.push_back("Image/Momo/HP_0.png");
    pathList.push_back("Image/Momo/HP_1.png");
    pathList.push_back("Image/Momo/HP_2.png");
    pathList.push_back("Image/Momo/HP_3.png");
    pathList.push_back("Image/Momo/HP_4.png");
    ObjMomo.InitHP(20.0, 20.0, "Image/Momo/HP.png", pathList);

    // 敵1
    CObjEnemy1* pEnemy1 = new CObjEnemy1();
    pEnemy1->SetParameter(1000.0f, 0.0f, 3.0f, 0.0f, "Image/Enemy/Enemy1_L.png");
    pEnemy1->SetY(nGroundY - pEnemy1->GetHeight());
    pEnemy1->SetImages(
        "Image/Enemy/Enemy1_L.png",
        "Image/Enemy/Enemy1_R.png",
        "Image/Enemy/Enemy1_Damaged_L.png",
        "Image/Enemy/Enemy1_Damaged_R.png"
    );
    pEnemy1->InitEnemy(500.0f, 1000.0f, 300, nullptr);
    m_ObjEnemyList.push_back(pEnemy1);

    // 敵2
    CObjEnemy2* pEnemy2 = new CObjEnemy2();
    pEnemy2->SetParameter(1500, 150, 0.0f, 0.0f, "Image/Enemy/Enemy2_L.png");
    pEnemy2->SetImages(
        "Image/Enemy/Enemy2_L.png",
        "Image/Enemy/Enemy2_R.png",
        "Image/Enemy/Enemy2_Damaged_L.png",
        "Image/Enemy/Enemy2_Damaged_R.png"
    );
    pEnemy2->InitEnemy(0.0f, 0.0f, 400.0f, "Image/Enemy/DetectMark.png");
    pEnemy2->InitWeapon(
        "Image/Enemy/Bow.png",
        "Image/Enemy/Arrow.png"
    );
    pEnemy2->SetArrowRange(STAGE_WIDTH, STAGE_HEIGHT);
    m_ObjEnemyList.push_back(pEnemy2);

    // 敵3
    CObjEnemy3* pEnemy3 = new CObjEnemy3();
    pEnemy3->SetParameter(2500, 0.0f, 5.0f, 0.0f, "Image/Enemy/Enemy3_L.png");
    pEnemy3->SetY(nGroundY - pEnemy3->GetHeight());
    pEnemy3->SetImages(
        "Image/Enemy/Enemy3_L.png",
        "Image/Enemy/Enemy3_R.png",
        "Image/Enemy/Enemy3_Damaged_L.png",
        "Image/Enemy/Enemy3_Damaged_R.png"
    );
    pEnemy3->SetStartX(pEnemy3->GetX());
    pEnemy3->SetImagesEne3(
        "Image/Enemy/Shield.png",
        "Image/Enemy/LoseMark.png"
    );
    pEnemy3->InitEnemy(0.0f, 0.0f, 300, "Image/Enemy/DetectMark.png");
    m_ObjEnemyList.push_back(pEnemy3);

    // サウンド
    // ステージBGM
    nStageBGM = LoadSoundMem("Sound/Stage/StageBGM.wav");

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
        ObjMomo.MoveMomo(DIRECTION_R);
    }

    // ←キー
    if (CheckHitKey(KEY_INPUT_LEFT)) {
        ObjMomo.MoveMomo(DIRECTION_L);
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
        if (!ObjMomo.IsAttacking()) {
            ObjMomo.SetIsAttacking(true);
        }
    }
}

// モモと剣描画
void DrawMomo()
{
    // モモがオブジェクト上に乗っている
    if (ObjMomo.IsOnLand()) {
        if (!ObjMomo.IsOnGround()) { // 地面以外のオブジェクト上にいる
            if (m_nOnIndex >= 0 && !ObjMomo.IsMomoInRangeObjectX(ObjBlockList[m_nOnIndex])) { // オブジェクトのX範囲外に出たとき
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

        // ジャンプDOWN
        if (ObjMomo.GetVy() > 0) { // 着地判定
            // まず地面の上にいるか判定
            if (fPrevBottom <= ObjGround.GetY() && ObjGround.GetY() <= fNextBottom) {
                //ObjMomo.SetY(ObjGround.GetY() - ObjMomo.GetHeight());
                fNextTop = ObjGround.GetY() - ObjMomo.GetHeight();
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
                            fNextTop = fLandY - ObjMomo.GetHeight();
                            ObjMomo.SetOnLand(true);
                            ObjMomo.SetVy(0.0f);
                            m_nOnIndex = i;
                            break;
                        }
                    }
                }
            }   
        }
        // ジャンプUP
        if (ObjMomo.GetVy() < 0) { // 天井判定
            // 画面外に出たか判定
            if (fPrevTop >= 0 && fNextTop < 0) {
                //ObjMomo.SetY(0.0f);
                fNextTop = 0.0f;
                ObjMomo.SetDestY(ObjGround.GetY());
            }
            else {
                // オブジェクトにモモの頭が直撃するか判定
                bool bHeadHit = false;
                for (int i = 0; i < ObjBlockList.size(); i++) {
                    if (ObjMomo.IsMomoInRangeObjectX(ObjBlockList[i])) {
                        float fBlockBottom = ObjBlockList[i].GetY() + ObjBlockList[i].GetHeight();
                        if (fNextTop <= fBlockBottom && fBlockBottom <= fPrevTop) { // i番目のブロックに頭直撃
                            bHeadHit = true;
                            fNextTop = fBlockBottom;
                            ObjMomo.SetDestY(nGroundY);
                            break;
                        }
                    }
                }
                if (!bHeadHit) {
                    float fDestY = ObjMomo.GetDestY();
                    if (fDestY <= fPrevTop && fNextTop <= fDestY) { // 最高点までジャンプした
                        fNextTop = fDestY;
                        ObjMomo.SetDestY(ObjGround.GetY());
                    }
                }

            }
        }

        // モモのY座標更新
        ObjMomo.SetY(fNextTop);
    }

    // モモ描画
    ObjMomo.Draw(nCameraX);
}

// 敵更新
void UpdateEnemy()
{
    // まずは各敵の挙動を決める
    // 敵1→単純な往復
    // 敵2→モモを検知したら攻撃してくる→その場からは動かない
    // 敵3→モモを検知したら突進してくる
    for (int i = 0; i < m_ObjEnemyList.size(); i++) {
        m_ObjEnemyList[i]->Update(&ObjMomo);
        m_ObjEnemyList[i]->Attack(&ObjMomo);
    }
}

// 敵描画
void DrawEnemy()
{
    for (int i = 0; i < m_ObjEnemyList.size(); i++) {
        float fEnemyX = m_ObjEnemyList[i]->GetX();
        if (nCameraX < fEnemyX && fEnemyX < nCameraX + SCREEN_WIDTH) { // ウィンドウ内にあれば描画
            m_ObjEnemyList[i]->Draw(nCameraX);
        }
    }
}

// ヒットチェック
void CollisionCheck()
{
    // 剣先と敵
    for (int i = 0; i < m_ObjEnemyList.size(); i++) {
        if (m_ObjEnemyList[i]->IsDamaged()) {
            if (m_ObjEnemyList[i]->DamagedCheck()) {
                m_ObjEnemyList[i]->DrawDamaged(nCameraX);
            }
        }
    }

    if (ObjMomo.IsAttacking()) {
        float fTipX, fTipY;
        ObjMomo.CalcSwordTipXY(&fTipX, &fTipY);
        for (int i = 0; i < m_ObjEnemyList.size(); i++) {
            if (!m_ObjEnemyList[i]->IsDamaged()) {
                if (m_ObjEnemyList[i]->IsPointInRect(fTipX, fTipY, nCameraX)) {
                    m_ObjEnemyList[i]->SetDamaged(true);
                }
            }
        }

        // ゴールフラッグ破壊
        int nGoalX = ObjGoalFlag.GetX() - nCameraX;
        int nGoalY = ObjGoalFlag.GetY();
        if (nGoalY <= fTipY && nGoalX <= fTipX) {
            if (!ObjGoalFlag.IsDamaged()) {
                ObjGoalFlag.SetDamaged(true);
                nCnt++;
                if (nCnt >= 10) {
                    ObjMomo.SetFinishSlow(true);
                    ObjGoalFlag.SetImg(nBrokenGoal);
                    //nGameState = GAME_CLEAR;
                }
            }
        }
    }
    else {
        // 確認用
        ObjGoalFlag.SetDamaged(false);
    }

    // モモと敵のヒットチェック
    if (ObjMomo.DamagedCheck()) {
        ObjMomo.DrawDamaged(nCameraX);
    }
    else {
        for (int i = 0; i < m_ObjEnemyList.size(); i++) {
            if (ObjMomo.AABBOverlap(*m_ObjEnemyList[i])) {
                ObjMomo.SetDamaged(true);
            }
        }
    }
}

