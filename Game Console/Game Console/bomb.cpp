#include "bomb.h"

#include <stdio.h>
#include <stdlib.h>
#include <easyx.h>
#include<time.h>
void showMap(int map[ROW][COL])
{
    for (int i = 0; i < ROW; i++)
    {
        for (int k = 0; k < COL; k++)
        {
            printf("%3d", map[i][k]);
        }
        printf("\n");
    }
    printf("\n");
}
bool isOver = false;
void initMap(int map[ROW][COL])
{
    memset(map, 0, ROW * COL * sizeof(int));

    for (int i = 0; i < 10;)
    {
        int r = rand() % ROW;
        int c = rand() % COL;
        if (map[r][c] == 0)
        {
            map[r][c] = -1;
            i++;
        }
    }


    for (int i = 0; i < ROW; i++)
    {
        for (int k = 0; k < COL; k++)
        {
            if (map[i][k] == -1)
            {
                for (int r = i - 1; r <= i + 1; r++)
                {
                    for (int c = k - 1; c <= k + 1; c++)
                    {
                        if ((r >= 0 && r < ROW && c >= 0 && c < COL) && map[r][c] != -1)
                            map[r][c]++;
                    }
                }
            }
        }
    }

    for (int i = 0; i < ROW; i++)
    {
        for (int k = 0; k < COL; k++)
        {
            map[i][k] += 20;
        }
    }
}
void drawMap(int map[ROW][COL], IMAGE img[])
{
    for (int i = 0; i < ROW; i++)
    {
        for (int k = 0; k < COL; k++)
        {
            if (map[i][k] >= 0 && map[i][k] <= 8)
            {
                putimage(k * IMGW, i * IMGW, img + map[i][k]);
            }
            else if (map[i][k] == -1)
            {
                putimage(k * IMGW, i * IMGW, img + 9);
            }
            else if (map[i][k] >= 19 && map[i][k] <= 28)
            {
                putimage(k * IMGW, i * IMGW, img + 10);
            }
        }
    }
}
void mouseEvent(int map[ROW][COL])
{
    ExMessage msg;
    if (peekmessage(&msg, EX_MOUSE))//EM
    {
        int r = msg.y / IMGW;
        int c = msg.x / IMGW;
        if (msg.message == WM_LBUTTONDOWN)
        {
            if (map[r][c] >= 19 && map[r][c] <= 28)
            {
                map[r][c] -= 20;
                openNull(map, r, c);
                judge(map, r, c);
                showMap(map);
            }

        }
    }
}
void openNull(int map[ROW][COL], int row, int col)
{
    if (map[row][col] == 0)
    {
        for (int i = row - 1; i <= row + 1; i++)
        {
            for (int k = col - 1; k <= col + 1; k++)
            {
                if ((i >= 0 && i < ROW && k >= 0 && k < COL) && map[i][k] >= 19 && map[i][k] <= 28)
                {
                    map[i][k] -= 20;
                    openNull(map, i, k);

                }
            }
        }
    }
}
void judge(int map[ROW][COL], int row, int col)
{
    if (map[row][col] == -1)
    {
        for (int i = 0; i <= ROW; i++)
        {
            for (int k = 0; k <= COL; k++)
            {
                if (map[i][k] == 19)
                {
                    map[i][k] -= 20;
                    openNull(map, i, k);

                }
            }
        }
        isOver = true;
    }
    else
    {
        int nonMineCount = 0;
        for (int i = 0; i < ROW; i++)
        {
            for (int k = 0; k < COL; k++)
            {
                if (map[i][k] >= 0 && map[i][k] <= 8 && map[i][k] != 20)
                {
                    nonMineCount++;
                }
            }
        }

        if (nonMineCount == ROW * COL - 10)
        {
            // 所有非地雷的方格都已經打開 - 獲勝！
            isOver = true;
            int ret = MessageBox(GetHWnd(), "你贏了,再來一把?", "提示", MB_OKCANCEL);
            if (ret == IDOK)
            {
                initMap(map);
                showMap(map);
                isOver = false;
            }
            else if (ret == IDCANCEL)
            {
                exit(666);
            }
        }
    }
}
void starbomb() 
{
    initgraph(COL * IMGW, ROW * IMGW, EX_SHOWCONSOLE);

    srand((unsigned)time(NULL));

    int map[ROW][COL] = { 0 };

    initMap(map);

    IMAGE img[12];

    for (int i = 0; i < 12; i++)
    {
        char fileName[50] = { 0 };
        sprintf_s(fileName, "./images/%d.jpg", i);
        loadimage(img + i, fileName, IMGW, IMGW);
        putimage(i * IMGW, 0, img + i);
    }

    while (true)
    {
        mouseEvent(map);
        drawMap(map, img);


        if (isOver)
        {
            int ret = MessageBox(GetHWnd(), "你踩到雷了,再來一把?", "hint", MB_OKCANCEL);
            if (ret == IDOK)
            {
                printf("IDOK\n");
                initMap(map);
                showMap(map);
                isOver = false;
            }
            else if (ret == IDCANCEL)
            {
                exit(666);
            }

        }
    }

    showMap(map);

    getchar();
    return 0;
}
