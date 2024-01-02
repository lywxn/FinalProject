#pragma once

#define ROW 10
#define COL 10
#define IMGW 40
void initMap(int map[ROW][COL]);
void drawMap(int map[ROW][COL], IMAGE img[]);
void mouseEvent(int map[ROW][COL]);
void openNull(int map[ROW][COL], int row, int col);
void judge(int map[ROW][COL], int row, int col);
void starbomb();