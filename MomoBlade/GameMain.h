#pragma once

#include <vector>

// ステージ
const int STAGE_WIDTH = 3000;
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

// class化すればよかった...
struct ENEMY
{
	OBJECT obj;
	bool bIsHit;
	int nDmgTimer;
};

// ステージ描画
OBJECT ObjSky;         // 空    
OBJECT ObjGround;      // 地面
OBJECT ObjUnderGround; // 地中
OBJECT ObjBlock;       // ブロック
OBJECT ObjGoalFlag;    // ゴールフラッグ
std::vector<OBJECT> ObjBlockList;    // ブロックリスト

// BGM
int nStageBGM; // ステージ
int nSlashBGM; // 斬撃

// モモタロー
OBJECT ObjMomo;
const float GRAVITEY = 1.2f;
const float JUMP_UP_POWER = 0.8f;
float fMaxY = 0.0f;
bool bOnLand = true;
bool bOnGround = true;
int nCameraX = 0; // モモを中心とするカメラのX座標
bool bHitToMomo = false;
int nMomoDamagedTimer = 0;

// 剣
OBJECT ObjSword;
void SwordAttack();     // 斬撃モーション
bool HitCheckToMomo();
bool HitCheckToEnemy(); // 敵と剣先とのヒットチェック
void DrawDamageToMomo();
double dSwordLength;    // 剣の長さ→ヒットチェックの際、剣を線分として扱う
bool bIsAttacking;
int nAttackingTimer;
double dSwordAngle = 0;

// 敵
OBJECT ObjEnemy1;
OBJECT ObjEnemy2;
OBJECT ObjEnemy3;
OBJECT ObjEnemyList[3];
int nDamagedEnemy;
bool bHitToEnemy = false;
int nEnemyDamagedTimer = 0;
void DrawDamageToEnemy();

void InitData();
void SetObjParameter(OBJECT* pObj, float fx, float fy, float fvx, float fvy, const char* pchImg);
void DrawStage();
void PlayerInput();

void DrawMomo();
void DrawEnemy();
void DrawHP();
const int HP_MAX = 3; // MAXのHPはハート(HP)3個
int nImgHP[5]; // ハートの画像(HP0～MAX)
int nCurrentHP = HP_MAX * 4; // モモのHP(初期値MAX)
int nHPX, nHPY, nHPWidth;

