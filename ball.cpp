#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<windows.h>
void gotoxy(int x, int y)//光标定位函数
{
	COORD pos;//定义结构体变量p
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);//获取当前函数句柄
	pos.X = x;
	pos.Y = y;//将光标的目标移动位置传递给结构体
	SetConsoleCursorPosition(handle, pos);//移动光标
}
void ShowCursor(bool visible)
{            //显示或隐藏光标
	CONSOLE_CURSOR_INFO cursor_info = { 20, visible };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

int main() {
	int i, j;
	int x = 1;
	int y = 5;

	int left = 0;
	int right = 20;
	int top = 0;
	int bottom = 10;

	ShowCursor(false);

	int velocity_x = 5;
	int velocity_y = 5;

	while (true)
	{
		x += velocity_x;
		y += velocity_y;

		system("cls");

		for (j = 0; j <= right; j++) printf("_"); printf("\n");
		for (i = 1; i < bottom; i++)
		{
			printf("|");
			for (j = 0; j < right - 1; j++) printf(" ");
			printf("|\n");
		}
		for (j = 0; j <= right; j++) printf("=");
		gotoxy(0, 0);

		for (i = 0; i < x; i++)
			printf("\n");
		for (j = 0; j < y; j++) {
			if (j == 0)printf("|");
			else printf(" ");
		}
		printf("o");
		printf("\n");
		Sleep(500);

		if (x <= top + 1 || x >= bottom - 1) {
			velocity_x = -velocity_x;
			printf("\a");
		}
		if (y <= left + 1 || y >= right - 1) {
			velocity_y = -velocity_y;
			printf("\a");
		}

	}
	return 0;
}
