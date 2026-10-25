#ifndef _DEFINE_H
#define _DEFINE_H

#include <windows.h>
#include <stdlib.h>
#include <math.h>

// ===== 画面・フィールド =====
#define WINDOW_W 640
#define WINDOW_H 480
#define MARGIN 10
#define FIELD_W 380 // ゲーム画面の幅 (back1.png と同じ)
#define FIELD_H 460
#define FIELD_L MARGIN
#define FIELD_T MARGIN
#define FIELD_R (MARGIN + FIELD_W)
#define FIELD_B (MARGIN + FIELD_H)

// ===== ゲーム定数 =====
#define PLAYER_SPEED 4
#define PLAYER_SLOW_SPEED 2 // SHIFT押下中の低速移動
#define PLAYER_LIFE 3
#define PLAYER_HIT_R 3.0    // プレイヤーの当たり判定半径
#define PLAYER_INVINCIBLE 120
#define SCROLL_SPEED 2 // 背景スクロールスピード
#define PSHOT_NUM 40
#define PSHOT_SPEED 14
#define EBULLET_MAX 2048 // 敵弾の同時表示上限
#define EBULLET_R 4.0    // 敵弾の当たり判定半径

// 1: デバッグキー有効 (F1:KOMY F2:SUZU を次のボス撃破後に出す / F3:ボス即死)
#define VSC_DEBUG_KEYS 1

#define VS_PI 3.14159265358979323846
#define DEG2RAD(d) ((d) * VS_PI / 180.0)

// メッセージボックス
#define MSG(m) MessageBox(NULL, m, "MESSAGE", MB_OK)

// プレイヤーの弾
struct SHOT
{
    bool flag;     // 弾が発射中かどうか
    double x;      // x座標
    double y;      // y座標
};

// 敵の弾 (速度ベクトルを持つので毎フレームの三角関数が不要)
struct E_SHOT
{
    double x, y;   // 座標
    double vx, vy; // 1フレームあたりの移動量
};

struct ENEMYDATA
{
    int type;      // 敵種類
    int stype;     // 弾種類
    int m_pattern; // 移動パターン
    int s_pattern; // 発射パターン
    int in_time;   // 出現時間
    int stop_time; // 停止時間
    int shot_time; // 弾発射時間
    int out_time;  // 帰還時間
    int x;         // x座標
    int y;         // y座標
    int speed;     // 弾スピード
    int hp;        // HP
    int item;      // アイテム
};

// ダイスで決まる難易度 (ステージ開始時の 1d100 から作られる)
struct DIFFICULTY
{
    int roll;        // 1d100 の出目 (ファンブル時は100固定)
    bool fumble;     // ファンブルによる強制最高難易度か
    double speedMul; // 弾速・ボス弾速の倍率
    double hpMul;    // 敵HPの倍率
    int extra;       // 弾幕の追加段階 (0〜4) : way数・密度が増える
};

// EXボスの種類
enum EXBOSS_KIND
{
    EXB_NONE = 0,
    EXB_KOMY,
    EXB_SUZU
};

class BULLETS;

// 敵やボスが毎フレーム参照するゲーム状況
struct GAMECTX
{
    double px, py;           // プレイヤー座標
    int pinv;                // プレイヤーの無敵残りフレーム
    const DIFFICULTY *diff;  // 現在の難易度
    BULLETS *bullets;        // 敵弾プール
};

// リソースのパスを探す (実行ファイルの場所が違っても動くようにする)
// 例: "graphics/shot.png", "Resource/Enemy.csv"
bool ResolvePath(const char *name, char *out, size_t outSize);

inline double VS_Clamp(double v, double lo, double hi)
{
    return v < lo ? lo : (v > hi ? hi : v);
}
inline int VS_Rand(int n) { return rand() % n; } // 0〜n-1

// extern宣言してkey配列に何処からでもアクセスできるようにする
extern char key[256];
extern int g_count; // ウェーブ開始からのフレーム数

#endif