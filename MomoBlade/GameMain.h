#pragma once
#include "DxLib.h"
#include "CObject.h"
#include "CObjMomo.h"
#include "CObjSword.h"
#include "CObjEnemy1.h"
#include "CObjEnemy2.h"
#include "CObjEnemy3.h"
#include <vector>

using namespace std;

// ステージ
CObject ObjSky;         // 空    
CObject ObjGround;      // 地面
CObject ObjUnderGround; // 地中
CObject ObjBlock;       // ブロック
vector<CObject> ObjBlockList;    // ブロックリスト
CObject ObjGoalFlag;    // ゴールフラッグ

int nGroundY;    // 地面のY座標
int nBrokenGoal; // 破壊時のゴールフラッグ

// BGM
int nStageBGM; // ステージBGM

// モモタロー
CObjMomo ObjMomo;
int nCameraX = 0; // モモを中心とするカメラのX座標
int m_nOnIndex = -1;

// 剣
CObjSword ObjSword;

// 敵
vector<CObjEnemy*> m_ObjEnemyList;

void GameStart();
void GamePlay();
void GameClear();
void GameOver();

void InitGameString();
void InitGamePlay();

void DrawStage();
void PlayerInput();

void DrawMomo();

void UpdateEnemy();
void DrawEnemy();

void CollisionCheck();
