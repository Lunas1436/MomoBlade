#pragma once

#include "DxLib.h"
#include "CObject.h"
#include "CObjMomo.h"
#include "CObjSword.h"
#include "CObjEnemy1.h"
#include "CObjEnemy2.h"
//#include "CObjEnemy3.h"
#include <vector>

using namespace std;

// ステージ
const int STAGE_WIDTH = 3000;
const int STAGE_HEIGHT = 600;

// スクリーン
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 600;

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
vector<CObject> ObjBlockList;    // ブロックリスト
CObject ObjGoalFlag;    // ゴールフラッグ

int nGroundY;

// BGM
int nStageBGM; // ステージBGM

// モモタロー
CObjMomo ObjMomo;
int nCameraX = 0; // モモを中心とするカメラのX座標
int m_nOnIndex;

// 剣
CObjSword ObjSword;

// 敵
vector<CObjEnemy*> m_ObjEnemyList;

void InitData();
void DrawStage();
void PlayerInput();

void DrawMomo();

void UpdateEnemy();
void DrawEnemy();

void CollisionCheck();
