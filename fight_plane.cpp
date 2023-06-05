/* Windows */
#include <stdio.h>

#include <stdlib.h>

#include <conio.h>

int main()

{

	int i, j;

	int x = 5;

	int y = 10;

	char input;

	int isFire = 0;

	int ny = 5; // 一个靶子，放在第一行，ny列上

	int isKilled = 0;

	int score = 0; // 增加一个分数变量

	while (1)

	{

		system("cls");   // 清屏函数

		if (!isKilled)  // 输出靶子

		{

			for (j = 0; j < ny; j++)

				printf(" ");

			printf("+\n");

		}

		if (isFire == 0) // 输出飞机上面的空行

		{

			for (i = 0; i < x; i++)

				printf("\n");

		}

		else   // 输出飞机上面的激光竖线

		{

			for (i = 0; i < x; i++)

			{

				for (j = 0; j < y; j++)

					printf(" ");

				printf("  |\n");

			}

			if (y + 2 == ny)  // +2是因为激光在飞机的正中间，距最左边2个坐标

			{

				isKilled = 1; // 击中靶子

				score++; // 击中靶子，分数加1

			}

			isFire = 0;

		}

		// 下面输出一个复杂的飞机图案

		for (j = 0; j < y; j++)

			printf(" ");

		printf("  *\n");

		for (j = 0; j < y; j++)

			printf(" ");

		printf("*****\n");

		for (j = 0; j < y; j++)

			printf(" ");

		printf(" * * \n");

		printf("\nScore: %d\n", score); // 输出当前分数

		if (_kbhit())  // 判断是否有输入

		{

			input = _getch();  // 根据用户的不同输入来移动，不必输入回车

			if (input == 'a')

				y--;  // 位置左移

			if (input == 'd')

				y++;  // 位置右移

			if (input == 'w')

				x--;  // 位置上移

			if (input == 's')

				x++;  // 位置下移

			if (input == ' ')

				isFire = 1;

		}

		// 移动靶子

		if (!isKilled) {

			ny++;

			if (ny >= 20) // 假设屏幕宽度为20，您可以根据实际情况修改

				ny = 0;

		}
		else { // 如果靶子被击中，重新设置位置并让它活动起来

			ny = 0;

			isKilled = 0;

		}
	}
	return 0;

}

/* Linux
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

char getch_()
{
	char buf = 0;
	struct termios old = {0};
	if (tcgetattr(0, &old) < 0)
		perror("tcsetattr()");
	old.c_lflag &= ~ICANON;
	old.c_lflag &= ~ECHO;
	old.c_cc[VMIN] = 1;
	old.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSANOW, &old) < 0)
		perror("tcsetattr ICANON");
	if (read(0, &buf, 1) < 0)
		perror("read()");
	old.c_lflag |= ICANON;
	old.c_lflag |= ECHO;
	if (tcsetattr(0, TCSADRAIN, &old) < 0)
		perror("tcsetattr ~ICANON");
	return buf;
}

int kbhit(void)
{
	struct termios term;
	tcgetattr(0, &term);

	struct termios term2 = term;
	term2.c_lflag &= ~ICANON;
	tcsetattr(0, TCSANOW, &term2);

	int byteswaiting;
	ioctl(0, FIONREAD, &byteswaiting);

	tcsetattr(0, TCSANOW, &term);

	return byteswaiting > 0;
}

void clrscr()
{
	printf("\e[1;1H\e[2J");
}

int main()
{
	int i, j;
	int x = 5;
	int y = 10;
	char input;
	int isFire = 0;
	int ny = 5; // 一个靶子，放在第一行，ny列上
	int isKilled = 0;
	int score = 0; // 增加一个分数变量

	while (1)
	{
		clrscr();   // 清屏函数

		if (!isKilled)    // 输出靶子
		{
			for (j = 0; j < ny; j++)
				printf(" ");
			printf("+\n");
		}

		if (isFire == 0)   // 输出飞机上面的空行
		{
			for (i = 0; i < x; i++)
				printf("\n");
		}
		else      // 输出飞机上面的激光竖线
		{
			for (i = 0; i < x; i++)
			{
				for (j = 0; j < y; j++)
					printf(" ");
				printf("  |\n");
			}

			if (y + 2 == ny)   // +2是因为激光在飞机的正中间，距最左边2个坐标
			{
				isKilled = 1; // 击中靶子
				score++; // 击中靶子，分数加1
			}

			isFire = 0;
		}

		// 下面输出一个复杂的飞机图案
		for (j = 0; j < y; j++)
			printf(" ");
		printf("  *\n");

		for (j = 0; j < y; j++)
			printf(" ");
		printf("*****\n");

		for (j = 0; j < y; j++)
			printf(" ");
		printf(" * * \n");

		printf("\nScore: %d\n", score); // 输出当前分数

		if (kbhit())    // 判断是否有输入
		{
			input = getch_();  // 根据用户的不同输入来移动，不必输入回车
			if (input == 'a')
				y--;  // 位置左移
			if (input == 'd')
				y++;  // 位置右移
			if (input == 'w')
				x--;  // 位置上移
			if (input == 's')
				x++;  // 位置下移
			if (input == ' ')
				isFire = 1;
		}

		// 移动靶子
		if (!isKilled)
		{
			ny++;
			if (ny >= 20) // 假设屏幕宽度为20，您可以根据实际情况修改
				ny = 0;
		}
		else      // 如果靶子被击中，重新设置位置并让它活动起来
		{
			ny = 0;
			isKilled = 0;
		}
	}
	return 0;
}
*/
