#pragma once

const int HEIGHT = 600;
const int WIDTH = 1000;

// ゲームステート
enum {
	GAME_START,
	GAME_PLAY,
	GAME_END,
};
int nGameState = GAME_START;

struct OBJECT
{
	// 座標(左上基準)
	float x;       
	float y;
	// 速度
	float vx;
	float vy;
	// 画像情報
	int img;
	int width;
	int height;
};

// ステージ描画
OBJECT ObjSky;
OBJECT ObjGround;      // 地面
OBJECT ObjUnderGround; // 地中

// モモタロー
OBJECT ObjMomo;
const float GRAVITEY = 0.8f;
const float JUMP_UP_POWER = 0.8f;
bool bJumpUp = false;
bool bJumpDown = false;


void Init();
void DrawStage();
void PlayerInput();
