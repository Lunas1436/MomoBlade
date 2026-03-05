#pragma once

#include "DxLib.h" // 冗長
#include "CObject.h"
#include "CObjMomo.h"
#include "CObjEnemy.h"
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

// ステージ
CObject ObjSky;         // 空    
CObject ObjGround;      // 地面
CObject ObjUnderGround; // 地中
CObject ObjBlock;       // ブロック
std::vector<CObject> ObjBlockList;    // ブロックリスト
CObject ObjGoalFlag;    // ゴールフラッグ

int m_nOnIndex;

// BGM
int nStageBGM; // ステージ
int nSlashBGM; // 斬撃

// モモタロー
CObjMomo ObjMomo;
const float GRAVITEY = 1.2f;
const float JUMP_UP_POWER = 0.8f;
int nCameraX = 0; // モモを中心とするカメラのX座標

// 剣
CObject ObjSword;
double dSwordLength;    // 剣の長さ→ヒットチェックの際、剣を線分として扱う
double dSwordAngle = 0;
bool bIsAttacking = false;
void SwordAttack();     // 斬撃モーション
void DrawDamageToMomo();

// 敵
CObjEnemy ObjEnemyList[3];
int nDmgIndex = -1;
void DrawDamageToEnemy(CObjEnemy ObjDmgEnemy);

void InitData();
void SetEnemyImgData();
void DrawStage();
void PlayerInput();

void DrawMomo();
void MoveEnemy();
void MoveEnemy1();
void MoveEnemy2();
void MoveEnemy3();

void DrawEnemy();
void DrawHP();
const int HP_MAX = 3; // MAXのHPはハート(HP)3個
int nImgHP[5]; // ハートの画像(HP0～MAX)
int nCurrentHP = HP_MAX * 4; // モモのHP(初期値MAX)
int nHPX, nHPY, nHPWidth;

void CollisionCheck();
bool PointInRect(float fpx, float fpy, CObject ObjRect);
bool AABBOverlap(CObject ObjRect1, CObject ObjRect2);
