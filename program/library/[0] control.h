#ifndef _CONTROL_H
#define _CONTROL_H

#include <vector>
#include "[4] define.h"
#include "[1] player.h"
#include "[2] enemy.h"
#include "[3] shot.h"
#include "[5] back.h"
#include "[6] BossEnemy.h"
#include "[1] dice_control.h"

// ゲームの流れ
enum GAME_STATE
{
    STATE_INTRO,      // ステージ開始 (1d100 で難易度決定)
    STATE_WAVE,       // 道中 (Enemy.csv の敵)
    STATE_BOSS,       // ボス戦
    STATE_BOSS_CLEAR, // ボス撃破 (1d6 / 1d9)
    STATE_EX_WARNING, // EXボス出現演出
    STATE_EX_BOSS,    // EXボス戦
    STATE_EX_CLEAR,   // EXボス撃破
    STATE_GAMEOVER
};

class CONTROL
{
private:
    // プレイヤークラス
    PLAYER *player;

    // 背景クラス
    BACK *back;

    // 敵弾
    BULLETS *bullets;

    // 敵クラス
    std::vector<ENEMY *> enemies;
    // 敵の出現データ (出現時間順)
    std::vector<ENEMYDATA> data;
    size_t nextSpawn;

    // ボス (通常 / EX)
    BOSS *boss;
    EXBOSS_KIND pendingEx;

    // ダイス
    DICE_CTL dice;

    GAMECTX ctx;
    GAME_STATE state;
    int stateCount;
    int waveEndCount;
    int stage;
    int score;

private:
    CONTROL();
    ~CONTROL();

    void Reset();
    void StartStage();
    void ChangeState(GAME_STATE s);
    void SpawnBoss(EXBOSS_KIND kind);
    void ClearEnemies();
    void OnBossDead();

    void Update();
    void Collide();
    void Draw();
    void DrawBanner() const;
    void DrawHud() const;

public:
    void All();
    void GetPlayerPosition(double *x, double *y);
    void GetEnemyPosition(int index, double *x, double *y);
    static CONTROL &GetInstance()
    {
        static CONTROL control;
        return control;
    }
};
#endif