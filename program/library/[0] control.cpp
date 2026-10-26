#include <stdio.h>
#include "DxLib.h"
#include "[0] control.h"
#include "[4] define.h"
#include "[6] enemytype.h"
#include "[7] ExBoss_KOMY.h"
#include "[8] ExBoss_Suzu.h"

static const char *DiffName(const DIFFICULTY &d)
{
    // if (d.fumble)
    //     return "FUMBLE (MAX)";
    // if (d.roll <= 20)
    //     return "EASY";
    // if (d.roll <= 60)
    //     return "NORMAL";
    // if (d.roll <= 90)
    //     return "HARD";
    // return "LUNATIC";
    return DiffTierName(d.tier);
}

CONTROL::CONTROL()
    : player(new PLAYER), back(new BACK), bullets(new BULLETS),
      nextSpawn(0), boss(NULL), pendingEx(EXB_NONE),
      state(STATE_INTRO), stateCount(0), waveEndCount(0), stage(0), score(0)
{
    // 敵の出現データ読み込み (読めなければ代わりの配置を使う)
    if (!LoadEnemyCSV("Resource/Enemy.csv", data))
        MakeDefaultEnemyData(data);

    ctx.px = ctx.py = 0;
    ctx.pinv = 0;
    ctx.diff = &dice.Diff();
    ctx.bullets = bullets;

    Reset();
}

CONTROL::~CONTROL()
{
    ClearEnemies();
    delete boss;
    delete player;
    delete back;
    delete bullets;
}

void CONTROL::ClearEnemies()
{
    for (size_t i = 0; i < enemies.size(); ++i)
        delete enemies[i];
    enemies.clear();
}

void CONTROL::Reset()
{
    delete boss;
    boss = NULL;
    pendingEx = EXB_NONE;
    player->Reset();
    dice.Reset();
    stage = 0;
    score = 0;
    StartStage();
}

void CONTROL::StartStage()
{
    ++stage;
    dice.RollStage(); // 1d100 で難易度決定 (ファンブル後は最高難易度)
    ClearEnemies();
    bullets->Clear();
    nextSpawn = 0;
    waveEndCount = 0;
    ChangeState(STATE_INTRO);
}

void CONTROL::ChangeState(GAME_STATE s)
{
    state = s;
    stateCount = 0;
    if (s == STATE_WAVE)
        g_count = 0;
}

void CONTROL::SpawnBoss(EXBOSS_KIND kind)
{
    delete boss;
    switch (kind)
    {
    case EXB_KOMY:
        boss = new ExBoss_KOMY(dice.Diff());
        break;
    case EXB_SUZU:
        boss = new ExBoss_SUZU(dice.Diff());
        break;
    default:
        boss = new BOSS_NORMAL(dice.Diff());
        break;
    }
}

void CONTROL::GetPlayerPosition(double *x, double *y)
{
    player->GetPosition(x, y);
}

void CONTROL::GetEnemyPosition(int index, double *x, double *y)
{
    // 指定した添字の敵の座標を取得
    if (index < 0 || index >= (int)enemies.size())
    {
        *x = *y = 0;
        return;
    }
    enemies[index]->GetPosition(x, y);
}

void CONTROL::OnBossDead()
{
    const bool ex = (state == STATE_EX_BOSS);
    score += ex ? 20000 : 5000;
    delete boss;
    boss = NULL;
    bullets->Clear();
    if (ex)
    {
        ChangeState(STATE_EX_CLEAR);
    }
    else
    {
        dice.RollBossClear(); // 1d6 / 1d9 を振る
        ChangeState(STATE_BOSS_CLEAR);
    }
}

void CONTROL::Update()
{
    ++stateCount;
    ++g_count;

    if (state == STATE_GAMEOVER)
    {
        if (key[KEY_INPUT_RETURN] == 1)
            Reset();
        bullets->Update();
        return;
    }

#if VSC_DEBUG_KEYS
    static char prev[3] = {0, 0, 0};
    if (key[KEY_INPUT_F1] && !prev[0])
        dice.ForceExBoss(EXB_KOMY);
    if (key[KEY_INPUT_F2] && !prev[1])
        dice.ForceExBoss(EXB_SUZU);
    if (key[KEY_INPUT_F3] && !prev[2] && boss)
        boss->Kill();
    prev[0] = key[KEY_INPUT_F1];
    prev[1] = key[KEY_INPUT_F2];
    prev[2] = key[KEY_INPUT_F3];
#endif

    player->Update();

    // 敵・ボスが参照する状況を更新
    player->GetPosition(&ctx.px, &ctx.py);
    ctx.pinv = player->Invincible();

    switch (state)
    {
    case STATE_INTRO:
        if (stateCount >= 120)
            ChangeState(STATE_WAVE);
        break;

    case STATE_WAVE:
        // 出現時間になった敵を出す
        while (nextSpawn < data.size() && data[nextSpawn].in_time <= g_count)
            enemies.push_back(new ENEMY(data[nextSpawn++], dice.Diff()));

        for (size_t i = 0; i < enemies.size();)
        {
            if (enemies[i]->Update(ctx))
            {
                delete enemies[i];
                enemies.erase(enemies.begin() + i);
            }
            else
                ++i;
        }

        // 敵が全部いなくなって少ししたらボス
        if (nextSpawn >= data.size() && enemies.empty())
        {
            if (++waveEndCount > 90)
            {
                SpawnBoss(EXB_NONE);
                ChangeState(STATE_BOSS);
            }
        }
        break;

    case STATE_BOSS:
    case STATE_EX_BOSS:
        if (boss)
            boss->Update(ctx);
        break;

    case STATE_BOSS_CLEAR:
        if (stateCount >= 240)
        {
            pendingEx = dice.PopExBoss();
            if (pendingEx != EXB_NONE)
                ChangeState(STATE_EX_WARNING);
            else
                StartStage();
        }
        break;

    case STATE_EX_WARNING:
        if (stateCount >= 150)
        {
            SpawnBoss(pendingEx);
            ChangeState(STATE_EX_BOSS);
        }
        break;

    case STATE_EX_CLEAR:
        if (stateCount >= 180)
        {
            pendingEx = dice.PopExBoss(); // 666 と 999 が両方成立していたら続けて出る
            if (pendingEx != EXB_NONE)
                ChangeState(STATE_EX_WARNING);
            else
                StartStage();
        }
        break;

    default:
        break;
    }

    bullets->Update();
    Collide();

    if (boss && boss->IsDead())
        OnBossDead();
}

void CONTROL::Collide()
{
    if (state != STATE_WAVE && state != STATE_BOSS && state != STATE_EX_BOSS)
        return;

    // 自機弾 -> 敵
    for (size_t i = 0; i < enemies.size();)
    {
        double ex, ey;
        enemies[i]->GetPosition(&ex, &ey);
        const int hits = player->ConsumeShots(ex, ey, enemies[i]->Radius());
        if (hits > 0 && enemies[i]->Damage(hits))
        {
            score += 100;
            delete enemies[i];
            enemies.erase(enemies.begin() + i);
        }
        else
            ++i;
    }

    // 自機弾 -> ボス (消えている間は素通り)
    if (boss && boss->Vulnerable())
    {
        double bx, by;
        boss->GetPosition(&bx, &by);
        const int hits = player->ConsumeShots(bx, by, boss->Radius());
        if (hits > 0)
            boss->Damage(hits);
    }

    // 敵弾・敵の体 -> 自機
    if (!player->Alive() || player->Invincible() > 0)
        return;

    bool hit = bullets->Hit(ctx.px, ctx.py, PLAYER_HIT_R);
    for (size_t i = 0; i < enemies.size() && !hit; ++i)
    {
        double ex, ey;
        enemies[i]->GetPosition(&ex, &ey);
        const double dx = ex - ctx.px, dy = ey - ctx.py;
        const double r = enemies[i]->Radius() * 0.6 + PLAYER_HIT_R;
        hit = (dx * dx + dy * dy <= r * r);
    }
    if (!hit && boss && boss->Vulnerable())
    {
        double bx, by;
        boss->GetPosition(&bx, &by);
        const double dx = bx - ctx.px, dy = by - ctx.py;
        const double r = boss->Radius() * 0.6 + PLAYER_HIT_R;
        hit = (dx * dx + dy * dy <= r * r);
    }

    if (hit && player->Hit())
    {
        bullets->Clear();
        if (!player->Alive())
            ChangeState(STATE_GAMEOVER);
    }
}

void CONTROL::Draw()
{
    // 描画範囲をゲーム画面に限定
    SetDrawArea(FIELD_L, FIELD_T, FIELD_R, FIELD_B);

    back->All();
    for (size_t i = 0; i < enemies.size(); ++i)
        enemies[i]->Draw();
    if (boss)
    {
        boss->Draw();
        boss->DrawHpBar();
    }
    player->Draw();
    bullets->Draw();
    DrawBanner();

    // 描画範囲を画面全体に戻してHUD
    SetDrawArea(0, 0, WINDOW_W, WINDOW_H);
    DrawBox(FIELD_L - 1, FIELD_T - 1, FIELD_R + 1, FIELD_B + 1, GetColor(180, 180, 180), FALSE);
    DrawHud();
}

void CONTROL::DrawBanner() const
{
    const int white = GetColor(255, 255, 255);
    const int yellow = GetColor(255, 230, 80);
    const int red = GetColor(255, 80, 80);
    const int x = FIELD_L + 40;
    const int y = FIELD_T + 150;
    const DIFFICULTY &d = dice.Diff();

    SetFontSize(24);
    switch (state)
    {
    case STATE_INTRO:
        DrawFormatString(x, y, white, "STAGE %d", stage);
        SetFontSize(16);
        DrawFormatString(x, y + 40, yellow, "DICE 1d100 = %d", d.roll);
        DrawFormatString(x, y + 62, d.fumble ? red : white, "DIFFICULTY : %s", DiffName(d));
        if (d.fumble)
            DrawFormatString(x, y + 84, red, "FUMBLE! forced MAX difficulty");
        break;
    case STATE_BOSS_CLEAR:
        DrawFormatString(x, y, white, "BOSS CLEAR!");
        SetFontSize(16);
        DrawFormatString(x, y + 40, yellow, "DICE 1d6 = %d   1d9 = %d", dice.Last6(), dice.Last9());
        if (dice.IsFumble6())
            DrawFormatString(x, y + 62, red, "FUMBLE! next stage = MAX difficulty");
        DrawFormatString(x, y + 84, white, "666: %d/3   999: %d/3", dice.Streak6(), dice.Streak9());
        break;
    case STATE_EX_WARNING:
        if ((stateCount / 15) % 2 == 0)
            DrawFormatString(x + 40, y, red, "WARNING!");
        SetFontSize(16);
        DrawFormatString(x, y + 40, white, "EX BOSS : %s", pendingEx == EXB_KOMY ? "KOMY" : "SUZU");
        break;
    case STATE_EX_CLEAR:
        DrawFormatString(x, y, yellow, "EX BOSS CLEAR!");
        break;
    case STATE_GAMEOVER:
        DrawFormatString(x + 20, y, red, "GAME OVER");
        SetFontSize(16);
        DrawFormatString(x + 20, y + 40, white, "ENTER : retry");
        break;
    default:
        break;
    }
    SetFontSize(16);
}

void CONTROL::DrawHud() const
{
    const int white = GetColor(255, 255, 255);
    const int gray = GetColor(160, 160, 160);
    const DIFFICULTY &d = dice.Diff();
    const int x = FIELD_R + 16;

    DrawFormatString(x, 16, white, "VSCDXSTG");
    DrawFormatString(x, 50, white, "STAGE  %d", stage);
    DrawFormatString(x, 70, white, "SCORE  %d", score);
    DrawFormatString(x, 90, white, "LIFE   %d", player->Life());

    DrawFormatString(x, 130, white, "-- DICE --");
    DrawFormatString(x, 150, white, "1d100 : %d", d.roll);
    DrawFormatString(x, 170, d.fumble ? GetColor(255, 80, 80) : white, "%s", DiffName(d));
    DrawFormatString(x, 190, gray, "SPD x%.2f HP x%.2f", d.speedMul, d.hpMul);
    if (dice.Last6() > 0)
        DrawFormatString(x, 210, white, "1d6:%d 1d9:%d", dice.Last6(), dice.Last9());
    DrawFormatString(x, 230, gray, "666:%d/3 999:%d/3", dice.Streak6(), dice.Streak9());

    DrawFormatString(x, 380, gray, "Z:shot SHIFT:slow");
    DrawFormatString(x, 400, gray, "ARROW:move ESC:quit");
#if VSC_DEBUG_KEYS
    DrawFormatString(x, 430, gray, "F1:KOMY F2:SUZU");
    DrawFormatString(x, 450, gray, "F3:kill boss");
#endif
}

void CONTROL::All()
{
    Update();
    Draw();
}