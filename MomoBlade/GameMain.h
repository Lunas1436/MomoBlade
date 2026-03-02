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

	// ヒットチェック用
	// ↑の変数と命名規則を揃える
	bool bIsHit;
	int nDmgTimer;
	int nDmgImg;
};

// 敵
// 敵の向いている方向
enum {
	DIRECTION_L,
	DIRECTION_R
};
struct ENEMY
{
	OBJECT Obj;

	// X方向の移動範囲
	float fRangeL;
	float fRangeR;

	// 向いている方向
	int nDirection;

	// 通常時の画像
	int nImg_L;
	int nImg_R;

	// 被攻撃時の画像
	int nDmgImg_L;
	int nDmgImg_R;
};

// ステージ描画
OBJECT ObjSky;         // 空    
OBJECT ObjGround;      // 地面
OBJECT ObjUnderGround; // 地中
OBJECT ObjBlock;       // ブロック
std::vector<OBJECT> ObjBlockList;    // ブロックリスト
OBJECT ObjGoalFlag;    // ゴールフラッグ

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

// 剣
OBJECT ObjSword;
double dSwordLength;    // 剣の長さ→ヒットチェックの際、剣を線分として扱う
double dSwordAngle = 0;
bool bIsAttacking = false;
void SwordAttack();     // 斬撃モーション
void DrawDamageToMomo();

// 敵
//OBJECT ObjEnemyList[3];
ENEMY ObjEnemyList[3];
int nDmgIndex = -1;
void DrawDamageToEnemy(ENEMY ObjDmgEnemy);

void InitData();
void SetObjParameter(OBJECT* pObj, float fx, float fy, float fvx, float fvy, const char* pchImg);
void SetEnemyImgData();
void DrawStage();
void PlayerInput();

void DrawMomo();
void MoveEnemy();
void DrawEnemy();
void DrawHP();
const int HP_MAX = 3; // MAXのHPはハート(HP)3個
int nImgHP[5]; // ハートの画像(HP0～MAX)
int nCurrentHP = HP_MAX * 4; // モモのHP(初期値MAX)
int nHPX, nHPY, nHPWidth;

void CollisionCheck();
bool PointInRect(float fpx, float fpy, OBJECT ObjRect);
bool AABBOverlap(OBJECT ObjRect1, OBJECT ObjRect2);
