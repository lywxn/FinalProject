#include <stdio.h>
#include <easyx.h>
#include <time.h>
#include <sstream>
#include <conio.h>

#ifndef GAME_H
#define GAME_H
Res res;
Sprite player;
Fruit fruit[MAX_FRUITS];

const int MAX_FRUITS = 6;
const double PLAYER_INITIAL_X = 200.0;
const double PLAYER_INITIAL_Y_OFFSET = 70.0;
const double PLAYER_MOVE_SPEED = 12;
const double FRUIT_FALL_SPEED = 3;
const int GAME_WIDTH = 800;
const int GAME_HEIGHT = 600;
const int GAME_TIME_LIMIT = 30;

struct Sprite
{
    double x, y;
    double vx; // x 軸移動速度
};

// 定義水果結構，包含 Sprite 結構作為基底
struct Fruit
{
    Sprite _super;
    double vy;
    int type; // 水果繪製種類
};

struct Res
{
    IMAGE img_player;
    IMAGE img_fruit[3];
};

// 初始化遊戲資源
void res_init(Res* res)
{
    loadimage(&res->img_player, "./image/player.jpg");
    for (int i = 0; i < 3; i++)
    {
        char filePath[50] = { 0 };
        sprintf(filePath, "./image/fruit%d.jpg", i);
        loadimage(res->img_fruit + i, filePath);
    }
}

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

// 初始化水果結構
void fruit_init(Fruit* f, double x, double y)
{
    spr_init(&f->_super, x, y, 0);
    f->type = rand() % 3;
    f->vy = (rand() % 50) / 100.0 + FRUIT_FALL_SPEED;
}

// 繪製水果
void fruit_draw(Fruit* f)
{
    putimage(f->_super.x, f->_super.y, res.img_fruit + f->type);
}

// 移動水果
void fruit_move(Fruit* f)
{
    spr_moveBy(&f->_super, 0, f->vy);
    if (f->_super.y > GAME_HEIGHT)
    {
        f->_super.y = 0;
        f->type = rand() % 3;
        f->vy = (rand() % 80) / 100.0 + FRUIT_FALL_SPEED;
    }
}

// 初始化遊戲
void init()
{
    res_init(&res);
    spr_init(&player, PLAYER_INITIAL_X, GAME_HEIGHT - res.img_player.getheight() - PLAYER_INITIAL_Y_OFFSET, 0);
    for (int i = 0; i < MAX_FRUITS; i++)
    {
        fruit_init(fruit + i, rand() % GAME_WIDTH, rand() % 50);
    }
}

int main()
{
    initgraph(GAME_WIDTH, GAME_HEIGHT, EX_SHOWCONSOLE);

    init();
    bool canmoved = false;

    IMAGE img_bk;
    loadimage(&img_bk, "./image/bk.jpg");

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
        EndBatchDraw();

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
        }
        Sleep(10);
    }

    closegraph();
    return 0;
}
#endif // GAME_H