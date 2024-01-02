#include <stdio.h>
#include <stdlib.h>	
#include <conio.h>	

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

int main(void) {
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