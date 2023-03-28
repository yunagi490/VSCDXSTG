#include <windows.h>
#define PLAYER_SPEED 4
#define SCROLL_SPEED 2  // 背景スクロールスピード
#define MARGIN 10
#define PSHOT_NUM 20
#define ENEMY_SNUM 50
#define PSHOT_SPEED 14

// メッセージボックス
#define MSG(m)                                   \
    {                                            \
        \ MessageBox(NULL, m, "MESSAGE", MB_OK); \
    }

struct SHOT{
    bool flag;          // 弾が発射中かどうか
    double x;           // x座標
    double y;           // y座標
    int gh;             // グラフィックハンドル
    int width,height;   // 画像の幅と高さ
};

struct E_SHOT{
    bool flag;          // 弾が発射中かどうか
    double x;           // x座標
    double y;           // y座標
    double rad;         // 角度(ラジアン) : ラジアン= 角度 *n(3.14) / 180
    int gh;             // グラフィックハンドル
    int width,height;   // 画像の幅と高さ
    int pattern;        // ショットPattern
    int speed;          // スピード
};

struct ENEMYDATA{
    int type;   // 敵種類
    int stype;  // 弾種類
    int m_pattern;  // 移動パターン
    int s_pattern;  // 発射パターン
    int in_time;    // 出現時間
    int stop_time;  // 停止時間
    int shot_time;  // 弾発射時間
    int out_time;   // 帰還時間
    int x;          // x座標
    int y;          // y座標
    int speed;      // 弾スピード
    int hp;         // HP
    int item;       // アイテム
};

#define ENEMY_NUM 5
// extern宣言してkey配列に何処からでもアクセスできるようにする
extern char key[256];
extern char g_count;
