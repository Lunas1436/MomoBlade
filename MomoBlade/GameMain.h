#pragma once

#include "DxLib.h" // 冗長
#include "CObject.h"
#include "CObjMomo.h"
#include "CObjSword.h"
#include "CObjEnemy1.h"
#include "CObjEnemy2.h"
//#include "CObjEnemy3.h"
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

// モモタロー
CObjMomo ObjMomo;
int nCameraX = 0; // モモを中心とするカメラのX座標

// 剣
CObjSword ObjSword;

// 敵
CObjEnemy ObjEnemyList[3];
CObjEnemy1 ObjEnemy1;

void DrawDamageToEnemy(CObjEnemy ObjDmgEnemy);

void InitData();
void DrawStage();
void PlayerInput();

void DrawMomo();
void UpdateEnemy();

void DrawEnemy();
void DrawHP();
const int HP_MAX = 3; // MAXのHPはハート(HP)3個
int nImgHP[5]; // ハートの画像(HP0～MAX)
int nCurrentHP = HP_MAX * 4; // モモのHP(初期値MAX)
int nHPX, nHPY, nHPWidth;

void CollisionCheck();
