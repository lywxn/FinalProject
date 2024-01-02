#include "catchfruit.h"

#include <stdio.h>
#include <easyx.h>
#include <time.h>
#include <sstream>
#include <conio.h>
#include "catchfruit.h"


struct Sprite
{
    double x, y;
    double vx; // x 軸移動速度
};
struct Fruit
{
    Sprite _super;
    double vy;
    int type; // 水果繪製種類
};// 定義水果結構，包含 Sprite 結構作為基底
struct Res
{
    IMAGE img_player;
    IMAGE img_fruit[3];
};// 定義資源結構，包含玩家和水果的圖片

void res_init(Res* res)
{
    loadimage(&res->img_player, "./image/player");
    for (int i = 0; i < 3; i++)
    {
        char filePath[50] = { 0 };
        sprintf(filePath, "./image/fruit%d.jpg", i);
        loadimage(res->img_fruit + i, filePath);
    }
}// 初始化遊戲資源
void spr_init(Sprite* spr, double x, double y, double vx)
{
    spr->x = x;
    spr->y = y;
    spr->vx = vx;
}
void spr_draw(Sprite* spr)
{
    putimage(spr->x, spr->y, &res.img_player);
}
void spr_moveBy(Sprite* spr, double dx, double dy)
{
    spr->x += dx;
    spr->y += dy;
}
void fruit_init(Fruit* f, double x, double y)
{
    spr_init(&f->_super, x, y, 0);
    f->type = rand() % 3;
    f->vy = (rand() % 50) / 100.0 + FRUIT_FALL_SPEED;
}// 初始化水果結構
void fruit_draw(Fruit* f)
{
    putimage(f->_super.x, f->_super.y, res.img_fruit + f->type);
}// 繪製水果
void fruit_move(Fruit* f)
{
    spr_moveBy(&f->_super, 0, f->vy);
    if (f->_super.y > GAME_HEIGHT)
    {
        f->_super.y = 0;
        f->type = rand() % 3;
        f->vy = (rand() % 80) / 100.0 + FRUIT_FALL_SPEED;
    }
}// 移動水果
void catchfruit(Sprite* player, Fruit* fruit, int* score)
{
    static int prevScore = *score;

    for (int i = 0; i < MAX_FRUITS; i++)
    {
        if (fruit[i]._super.x >= player->x && fruit[i]._super.x <= player->x + res.img_player.getwidth() &&
            fruit[i]._super.y >= player->y && fruit[i]._super.y <= player->y + res.img_player.getheight())
        {
            *score += 1;
            fruit_init(&fruit[i], rand() % GAME_WIDTH, rand() % 50);
        }
    }

    if (*score != prevScore)
    {
        // 分數有變動才更新顯示
        prevScore = *score;
    }
}// 接住水果，更新分數
void init()
{
    res_init(&res);
    spr_init(&player, PLAYER_INITIAL_X, GAME_HEIGHT - res.img_player.getheight() - PLAYER_INITIAL_Y_OFFSET, 0);
    for (int i = 0; i < MAX_FRUITS; i++)
    {
        fruit_init(fruit + i, rand() % GAME_WIDTH, rand() % 50);
    }
}// 初始化遊戲
void countdown()
{
    for (int i = 3; i > 0; i--)
    {
        cleardevice();
        settextstyle(40, 0, _T("楷體"));

        std::stringstream ss;
        ss << i;
        outtextxy(GAME_WIDTH / 2 - 20, GAME_HEIGHT / 2 - 20, ss.str().c_str());

        Sleep(1000);
    }
}//倒數3秒
void starcatchfruit()
{
    initgraph(GAME_WIDTH, GAME_HEIGHT, EX_SHOWCONSOLE);

    srand(time(NULL));
    countdown();

    init();
    bool canmoved = false;

    IMAGE img_bk;
    loadimage(&img_bk, "./image/bk.jpg");

    int score = 0;
    int gameTime = GAME_TIME_LIMIT;
    time_t startTime = time(NULL);

    bool gameEnded = false;

    while (true)
    {
        BeginBatchDraw();
        putimage(0, 0, &img_bk);
        spr_draw(&player);
        for (int i = 0; i < MAX_FRUITS; i++)
        {
            fruit_draw(fruit + i);
        }

        // 顯示目前分數
        settextstyle(20, 0, _T("楷體"));
        settextcolor(WHITE);

        char scoreText[20];
        sprintf(scoreText, "目前分數：%d", score);
        outtextxy(20, 20, scoreText);

        // 顯示遊戲剩餘時間的數字
        settextstyle(20, 0, _T("楷體"));
        settextcolor(WHITE);

        int elapsedTime = difftime(time(NULL), startTime);
        int remainingTime = (gameTime - elapsedTime) > 0 ? (gameTime - elapsedTime) : 0;
        char timeText[20];
        sprintf(timeText, "剩餘時間：%d 秒", remainingTime);
        outtextxy(GAME_WIDTH - 200, 20, timeText);

        EndBatchDraw();

        if (remainingTime <= 0 && !gameEnded)
        {
            gameEnded = true;  // 設置遊戲結束標誌
        }

        if (!gameEnded)
        {
            for (int i = 0; i < MAX_FRUITS; i++)
            {
                fruit_move(fruit + i);
            }

            static ExMessage msg;
            while (peekmessage(&msg, EX_MOUSE))
            {
                if (msg.message == WM_MOUSEMOVE)
                {
                    canmoved = true;
                }
            }

            if (canmoved)
            {
                if (msg.x < player.x - PLAYER_MOVE_SPEED)
                {
                    spr_moveBy(&player, -PLAYER_MOVE_SPEED, 0);
                }
                else if (msg.x > player.x + PLAYER_MOVE_SPEED)
                {
                    spr_moveBy(&player, PLAYER_MOVE_SPEED, 0);
                }

                catchfruit(&player, fruit, &score);
            }
        }

        Sleep(10);  // 降低 CPU 使用率
    }

    closegraph();
    return 0;
}

