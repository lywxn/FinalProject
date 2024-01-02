#include <stdio.h>
#include <stdlib.h>
#include <easyx.h>
#include<time.h>
#include <conio.h>
#include <sstream>

//踩地雷
#define ROW 10
#define COL 10
#define IMGW 40
void initMap(int map[ROW][COL]);
void drawMap(int map[ROW][COL], IMAGE img[]);
void mouseEvent(int map[ROW][COL]);
void openNull(int map[ROW][COL], int row, int col);
void judge(int map[ROW][COL], int row, int col);
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

//接水果
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
Res res;
Sprite player;
Fruit fruit[MAX_FRUITS];
void res_init(Res* res)
{
    loadimage(&res->img_player, "./image/player.jpg");
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

//五子棋
#define O 1
#define X 2
char toSymbol(int num) {
    switch (num) {
    case 0:
        return '.';		//空交叉點
    case X:
        return 'X';		//棋子X
    case O:
        return 'O';		//棋子O
    case 9:
        return '+';		//星位
    }
}
int check_hor(int panel[][15], int y, int x, int object) {	//檢查x軸方向上是否連成五子
    int stoneCount = 0;
    for (int i = x - 4; i <= x + 4; i++) {
        if (panel[y][i] == object) {
            stoneCount++;
            if (stoneCount == 5) {
                break;
            }
        }
        else {
            stoneCount = 0;
        }
    }
    if (stoneCount >= 5) {
        return 1;
    }
    else {
        return 0;
    }
}
int check_ver(int panel[][15], int y, int x, int object) {	//檢查y軸方向上是否連成五子
    int stoneCount = 0;
    for (int i = y - 4; i <= y + 4; i++) {
        if (panel[i][x] == object) {
            stoneCount++;
            if (stoneCount == 5) {
                break;
            }
        }
        else {
            stoneCount = 0;
        }
    }
    if (stoneCount >= 5) {
        return 1;
    }
    else {
        return 0;
    }
}
int check_slash(int panel[][15], int y, int x, int object) {	//檢查函數y=-x的影象所在直線方向上是否連成五子
    int stoneCount = 0;
    for (int i = y - 4, j = x - 4; i <= y + 4; i++, j++) {
        if (panel[i][j] == object) {
            stoneCount++;
            if (stoneCount == 5) {
                break;
            }
        }
        else {
            stoneCount = 0;
        }
    }
    if (stoneCount >= 5) {
        return 1;
    }
    else {
        return 0;
    }
}
int check_backslash(int panel[][15], int y, int x, int object) {	//檢查函數y=x的影象所在直線方向上是否連成五子
    int stoneCount = 0;
    for (int i = y + 4, j = x - 4; i >= y - 4; i--, j++) {
        if (panel[i][j] == object) {
            stoneCount++;
            if (stoneCount == 5) {
                break;
            }
        }
        else {
            stoneCount = 0;
        }
    }
    if (stoneCount >= 5) {
        return 1;
    }
    else {
        return 0;
    }
}

int main()
{
    int choice;
    printf("1.踩地雷\t2.接水果\t3.五子棋\t4.離開遊戲\n選擇遊戲:");
    while (scanf("%d", &choice) != 1) {
        printf("請輸入有效的數字。\n");
        while (getchar() != '\n'); // 清除輸入緩衝區
        printf("1.踩地雷\t2.接水果\t3.五子棋\t4.離開遊戲\n選擇遊戲:");
    }

    switch (choice)
    {
    case 1:

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
    break;
    case 2:

    {
        initgraph(GAME_WIDTH, GAME_HEIGHT, EX_SHOWCONSOLE);// 初始化圖形視窗
        countdown();// 遊戲倒數3秒
        srand(time(NULL));

        init();

        // 遊戲狀態變數
        int score = 0;
        bool canmoved = false;
        bool gameEnded = false;
        int gameTime = GAME_TIME_LIMIT;
        time_t startTime = time(NULL);

        // 背景圖
        IMAGE img_bk;
        loadimage(&img_bk, "./image/bk.jpg");

        // 主遊戲迴圈
        while (true)
        {
            BeginBatchDraw();

            // 繪製背景
            putimage(0, 0, &img_bk);

            //繪製玩家
            spr_draw(&player);

            // 繪製水果
            for (int i = 0; i < MAX_FRUITS; i++)
            {
                fruit_draw(fruit + i);
            }

            // 顯示目前分數
            char scoreText[20];
            settextstyle(20, 0, _T("楷體"));
            settextcolor(WHITE);
            sprintf(scoreText, "目前分數：%d", score);
            outtextxy(20, 20, scoreText);

            // 顯示遊戲剩餘時間
            settextstyle(20, 0, _T("楷體"));
            settextcolor(WHITE);
            int elapsedTime = difftime(time(NULL), startTime);
            int remainingTime = (gameTime - elapsedTime) > 0 ? (gameTime - elapsedTime) : 0;
            char timeText[20];
            sprintf(timeText, "剩餘時間：%d 秒", remainingTime);
            outtextxy(GAME_WIDTH - 200, 20, timeText);

            EndBatchDraw();

            // 判斷是否結束
            if (remainingTime <= 0 && !gameEnded)
            {
                gameEnded = true;  // 設置遊戲結束標誌
            }

            if (!gameEnded)
            {
                // 移動水果
                for (int i = 0; i < MAX_FRUITS; i++)
                {
                    fruit_move(fruit + i);
                }

                // 偵測滑鼠移動
                static ExMessage msg;
                while (peekmessage(&msg, EX_MOUSE))
                {
                    if (msg.message == WM_MOUSEMOVE)
                    {
                        canmoved = true;
                    }
                }

                // 左右移動
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

        // 關閉視窗
        closegraph();
        return 0;
    }
    break;
    case 3:
    {
        int key = 0;

        system("color E0");


        int panel[15][15] =
        { {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,9,0,0,0,0,0,0,0,9,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,9,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,9,0,0,0,0,0,0,0,9,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} };

        int cus[] = { 7,7 };	//遊標初始位置
        int turn = O;		//初始玩家
        int winner = 0;		//贏家：預設暫時沒有

        while (1) {

            system("cls");
            printf("現在是【%c】方走棋……\n", toSymbol(turn));

            //顯示棋盤
            for (int i = 0; i < 15; i++) {
                for (int j = 0; j < 15; j++) {
                    if (i == cus[0] && j == cus[1]) {
                        printf("[ %c ]", toSymbol(panel[i][j]));
                    }
                    else {
                        printf("  %c  ", toSymbol(panel[i][j]));
                    }
                }
                printf("\n\n");
            }

            //如果有人贏了，顯示贏家是誰，並結束程式
            if (winner != 0) {
                printf("五子連珠！玩家【%c】勝！\n", toSymbol(winner));
                return 0;
            }

            //接收鍵盤事件
            key = _getch();

            //按WASD進行控制，空格鍵落子，ESC鍵結束遊戲
            switch (key) {
            case 'w':
            case 'W':
                if (cus[0] == 0) cus[0] = 14;
                else cus[0]--;
                break;
            case 'a':
            case 'A':
                if (cus[1] == 0) cus[1] = 14;
                else cus[1]--;
                break;
            case 's':
            case 'S':
                if (cus[0] == 14) cus[0] = 0;
                else cus[0]++;
                break;
            case 'd':
            case 'D':
                if (cus[1] == 14) cus[1] = 0;
                else cus[1]++;
                break;
            case 32:
                if (panel[cus[0]][cus[1]] != X && panel[cus[0]][cus[1]] != O)
                {
                    panel[cus[0]][cus[1]] = turn;
                    if (check_hor(panel, cus[0], cus[1], turn) == 1
                        || check_ver(panel, cus[0], cus[1], turn) == 1
                        || check_slash(panel, cus[0], cus[1], turn) == 1
                        || check_backslash(panel, cus[0], cus[1], turn) == 1)
                    {
                        winner = turn;
                        break;
                    }
                    if (turn == O) turn = X;
                    else turn = O;
                }
                /*else
                {
                    printf("這裡已經有子了，重來！\n");
                    system("pause");
                }*/
                break;
            case 27:
                printf("結束程式……\n");
                system("pause");
                return 0;
            default:
                printf("無效按鍵！\n");
                system("pause");
                break;
            }
        }
        return 0;
    }
    break;
    case 4:
        printf("離開遊戲。\n");
        break;
    default:
        printf("請輸入有效的選擇。\n");
        break;
    }
    return 0;
}
