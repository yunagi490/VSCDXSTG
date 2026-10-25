#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <time.h>
#include "DxLib.h"
#include "[0] control.h"

// キー取得配列
char key[256];

// ウェーブ開始からのフレーム数 (charだと127で溢れるのでint)
int g_count = 0;

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    srand((unsigned int)time(NULL));

    SetOutApplicationLogValidFlag(FALSE); // Log.txt を作らない
    ChangeWindowMode(TRUE);
    SetGraphMode(WINDOW_W, WINDOW_H, 32);
    SetMainWindowText("VSCDXSTG");
    SetWaitVSyncFlag(TRUE);

    // DXライブラリ初期化処理
    if (DxLib_Init() == -1)
    {
        // エラーが起きたら直ちに終了
        return -1;
    }

    // 裏画面に描画して ScreenFlip で表示する (ちらつき防止)
    SetDrawScreen(DX_SCREEN_BACK);
    SetDrawMode(DX_DRAWMODE_BILINEAR); // 縮小描画をなめらかに

    CONTROL &control = CONTROL::GetInstance();
    while (ProcessMessage() == 0 && ClearDrawScreen() == 0 &&
           GetHitKeyStateAll(key) == 0 && key[KEY_INPUT_ESCAPE] == 0)
    {
        control.All();
        ScreenFlip();
    }

    // Dxライブラリ使用の終了処理
    DxLib_End();

    // ソフトの終了
    return 0;
}