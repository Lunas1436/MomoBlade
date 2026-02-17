#pragma once

// ステージ
const int STAGE_WIDTH = 5000;
const int STAGE_HEIGHT = 600;

// スクリーン
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 600;

int nInputState;

// ゲームステート
enum {
	GAME_START, // ゲーム開始画面
	GAME_PLAY,  // ゲームプレイ画面
	GAME_OVER,   // ゲームオーバー画面
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

// BGM
int nStageBGM; // ステージ
int nSlashBGM; // 斬撃

// モモタロー
OBJECT ObjMomo;
const float GRAVITEY = 0.8f;
const float JUMP_UP_POWER = 0.8f;
bool bJumpUp = false;
bool bJumpDown = false;
int nCameraX = 0;

OBJECT ObjSword; // 剣

// 敵
OBJECT ObjEnemy;
OBJECT ObjEnemy2;
int nDamagedEnemy;
//OBJECT ObjDamagedEnemy;
void DamageToEnemy();
int nDamagingTimer;

void InitData();
void SetObjParameter(OBJECT* pObj, float fx, float fy, float fvx, float fvy, const char* pchImg);
void CheckJumpState();
void DrawStage();
void PlayerInput();

void DrawHP();
const int HP_MAX = 3; // MAXのHPはハート(HP)3個
int nImgHP[5]; // ハートの画像(HP0～MAX)
int nCurrentHP = HP_MAX * 4; // モモのHP(初期値MAX)
int nHPX, nHPY, nHPWidth;


void SwordAttack();     // 斬撃モーション
bool HitCheckToMomo();
bool HitCheckToEnemy(); // 敵と剣先とのヒットチェック
void DamageToMomo();
bool bIsHit = false;
bool bDamaging = false;
double dSwordLength;    // 剣の長さ→ヒットチェックの際、剣を線分として扱う
bool bIsAttacking;
int nAttackingTimer;
double dSwordAngle = 0;