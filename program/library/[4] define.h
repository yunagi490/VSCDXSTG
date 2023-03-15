#define PLAYER_SPEED 4
#include <windows.h>

// メッセージボックス
#define MSG(m)                                   \
    {                                            \
        \ MessageBox(NULL, m, "MESSAGE", MB_OK); \
    }

// extern宣言してkey配列に何処からでもアクセスできるようにする
extern char key[256];
