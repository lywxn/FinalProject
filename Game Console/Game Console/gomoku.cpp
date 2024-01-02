#include "gomoku.h"

#include <stdio.h>
#include <stdlib.h>	
#include <conio.h>	

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
void stargomoku()
{
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
}