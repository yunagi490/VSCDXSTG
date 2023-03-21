#include <windows.h>
#define PLAYER_SPEED 4
#define SCROLL_SPEED 2  // 背景スクロールスピード
#define MARGIN 10
#define PSHOT_NUM 20


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

// extern宣言してkey配列に何処からでもアクセスできるようにする
extern char key[256];
