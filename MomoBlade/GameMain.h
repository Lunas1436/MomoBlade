#pragma once

const int HEIGHT = 600;
const int WIDTH = 1000;


enum {
	GAME_START,
	GAME_PLAY,
	GAME_END,
};

int nGameState = GAME_START;

struct CHARACTER 
{
	// 中心座標
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

const float GRAVITEY = 0.8f;
const float JUMP_POWER = -10.0f;
bool IsGround = false;

const float JUMP_UP_POWER = 0.8f;
bool bJumpUp = false;
bool bJumpDown = false;


// モモタロー
CHARACTER Momo;

void Init();
void PlayerInput();