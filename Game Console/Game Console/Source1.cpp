#include <stdio.h>
#include <stdlib.h>
#include <easyx.h>
#include<time.h>
#include <conio.h>
#include <sstream>

//��a�p
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
            // �Ҧ��D�a�p����泣�w�g���} - ��ӡI
            isOver = true;
            int ret = MessageBox(GetHWnd(), "�AĹ�F,�A�Ӥ@��?", "����", MB_OKCANCEL);
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

//�����G
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
    double vx; // x �b���ʳt��
};
struct Fruit
{
    Sprite _super;
    double vy;
    int type; // ���Gø�s����
};// �w�q���G���c�A�]�t Sprite ���c�@����
struct Res
{
    IMAGE img_player;
    IMAGE img_fruit[3];
};// �w�q�귽���c�A�]�t���a�M���G���Ϥ�
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
}// ��l�ƹC���귽
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
}// ��l�Ƥ��G���c
void fruit_draw(Fruit* f)
{
    putimage(f->_super.x, f->_super.y, res.img_fruit + f->type);
}// ø�s���G
void fruit_move(Fruit* f)
{
    spr_moveBy(&f->_super, 0, f->vy);
    if (f->_super.y > GAME_HEIGHT)
    {
        f->_super.y = 0;
        f->type = rand() % 3;
        f->vy = (rand() % 80) / 100.0 + FRUIT_FALL_SPEED;
    }
}// ���ʤ��G
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
        // ���Ʀ��ܰʤ~��s���
        prevScore = *score;
    }
}// ������G�A��s����
void init()
{
    res_init(&res);
    spr_init(&player, PLAYER_INITIAL_X, GAME_HEIGHT - res.img_player.getheight() - PLAYER_INITIAL_Y_OFFSET, 0);
    for (int i = 0; i < MAX_FRUITS; i++)
    {
        fruit_init(fruit + i, rand() % GAME_WIDTH, rand() % 50);
    }
}// ��l�ƹC��
void countdown()
{
    for (int i = 3; i > 0; i--)
    {
        cleardevice();
        settextstyle(40, 0, _T("����"));

        std::stringstream ss;
        ss << i;
        outtextxy(GAME_WIDTH / 2 - 20, GAME_HEIGHT / 2 - 20, ss.str().c_str());

        Sleep(1000);
    }
}//�˼�3��

//���l��
#define O 1
#define X 2
char toSymbol(int num) {
    switch (num) {
    case 0:
        return '.';		//�ť�e�I
    case X:
        return 'X';		//�ѤlX
    case O:
        return 'O';		//�ѤlO
    case 9:
        return '+';		//�P��
    }
}
int check_hor(int panel[][15], int y, int x, int object) {	//�ˬdx�b��V�W�O�_�s�����l
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
int check_ver(int panel[][15], int y, int x, int object) {	//�ˬdy�b��V�W�O�_�s�����l
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
int check_slash(int panel[][15], int y, int x, int object) {	//�ˬd���y=-x���v�H�Ҧb���u��V�W�O�_�s�����l
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
int check_backslash(int panel[][15], int y, int x, int object) {	//�ˬd���y=x���v�H�Ҧb���u��V�W�O�_�s�����l
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
    printf("1.��a�p\t2.�����G\t3.���l��\t4.���}�C��\n��ܹC��:");
    while (scanf("%d", &choice) != 1) {
        printf("�п�J���Ī��Ʀr�C\n");
        while (getchar() != '\n'); // �M����J�w�İ�
        printf("1.��a�p\t2.�����G\t3.���l��\t4.���}�C��\n��ܹC��:");
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
                int ret = MessageBox(GetHWnd(), "�A���p�F,�A�Ӥ@��?", "hint", MB_OKCANCEL);
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
        initgraph(GAME_WIDTH, GAME_HEIGHT, EX_SHOWCONSOLE);// ��l�ƹϧε���
        countdown();// �C���˼�3��
        srand(time(NULL));

        init();

        // �C�����A�ܼ�
        int score = 0;
        bool canmoved = false;
        bool gameEnded = false;
        int gameTime = GAME_TIME_LIMIT;
        time_t startTime = time(NULL);

        // �I����
        IMAGE img_bk;
        loadimage(&img_bk, "./image/bk.jpg");

        // �D�C���j��
        while (true)
        {
            BeginBatchDraw();

            // ø�s�I��
            putimage(0, 0, &img_bk);

            //ø�s���a
            spr_draw(&player);

            // ø�s���G
            for (int i = 0; i < MAX_FRUITS; i++)
            {
                fruit_draw(fruit + i);
            }

            // ��ܥثe����
            char scoreText[20];
            settextstyle(20, 0, _T("����"));
            settextcolor(WHITE);
            sprintf(scoreText, "�ثe���ơG%d", score);
            outtextxy(20, 20, scoreText);

            // ��ܹC���Ѿl�ɶ�
            settextstyle(20, 0, _T("����"));
            settextcolor(WHITE);
            int elapsedTime = difftime(time(NULL), startTime);
            int remainingTime = (gameTime - elapsedTime) > 0 ? (gameTime - elapsedTime) : 0;
            char timeText[20];
            sprintf(timeText, "�Ѿl�ɶ��G%d ��", remainingTime);
            outtextxy(GAME_WIDTH - 200, 20, timeText);

            EndBatchDraw();

            // �P�_�O�_����
            if (remainingTime <= 0 && !gameEnded)
            {
                gameEnded = true;  // �]�m�C�������лx
            }

            if (!gameEnded)
            {
                // ���ʤ��G
                for (int i = 0; i < MAX_FRUITS; i++)
                {
                    fruit_move(fruit + i);
                }

                // �����ƹ�����
                static ExMessage msg;
                while (peekmessage(&msg, EX_MOUSE))
                {
                    if (msg.message == WM_MOUSEMOVE)
                    {
                        canmoved = true;
                    }
                }

                // ���k����
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

            Sleep(10);  // ���C CPU �ϥβv
        }

        // ��������
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

        int cus[] = { 7,7 };	//�C�Ъ�l��m
        int turn = O;		//��l���a
        int winner = 0;		//Ĺ�a�G�w�]�ȮɨS��

        while (1) {

            system("cls");
            printf("�{�b�O�i%c�j�訫�ѡK�K\n", toSymbol(turn));

            //��ܴѽL
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

            //�p�G���HĹ�F�A���Ĺ�a�O�֡A�õ����{��
            if (winner != 0) {
                printf("���l�s�]�I���a�i%c�j�ӡI\n", toSymbol(winner));
                return 0;
            }

            //������L�ƥ�
            key = _getch();

            //��WASD�i�汱��A�Ů��丨�l�AESC�䵲���C��
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
                    printf("�o�̤w�g���l�F�A���ӡI\n");
                    system("pause");
                }*/
                break;
            case 27:
                printf("�����{���K�K\n");
                system("pause");
                return 0;
            default:
                printf("�L�ī���I\n");
                system("pause");
                break;
            }
        }
        return 0;
    }
    break;
    case 4:
        printf("���}�C���C\n");
        break;
    default:
        printf("�п�J���Ī���ܡC\n");
        break;
    }
    return 0;
}
