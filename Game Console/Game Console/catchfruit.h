#pragma once

#define GAME_H

const int MAX_FRUITS = 6;
const double PLAYER_INITIAL_X = 200.0;
const double PLAYER_INITIAL_Y_OFFSET = 70.0;
const double PLAYER_MOVE_SPEED = 12;
const double FRUIT_FALL_SPEED = 3;
const int GAME_WIDTH = 800;
const int GAME_HEIGHT = 600;
const int GAME_TIME_LIMIT = 30;

Res res;
Sprite player;
Fruit fruit[MAX_FRUITS];

void res_init(Res* res);
void spr_init(Sprite* spr, double x, double y, double vx);
void spr_draw(Sprite* spr);
void spr_moveBy(Sprite* spr, double dx, double dy);
void fruit_init(Fruit* f, double x, double y);
void fruit_draw(Fruit* f);
void fruit_move(Fruit* f);
void catchfruit(Sprite* player, Fruit* fruit, int* score);
void init();
void countdown();
void starcatchfruit();
void starcatchfruit();
