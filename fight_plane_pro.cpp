/* Windows */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

#define High 15  // ��Ϸ����ߴ�
#define Width 25
#define EnemyNum 5 // �л�����
#define MaxBulletNum 50 // ����ӵ�����
#define EnemyHealth 2	// �л�Ѫ�� 

// ȫ�ֱ���
int position_x, position_y; // �ɻ�λ��
int enemy_x[EnemyNum], enemy_y[EnemyNum];  // �л�λ��
int enemy_health[EnemyNum]; // �л�Ѫ��
int player_bullet_x[MaxBulletNum], player_bullet_y[MaxBulletNum]; // ����ӵ�λ��
int player_bullet_num = 0; // ����ӵ�����
int enemy_bullet_x[MaxBulletNum], enemy_bullet_y[MaxBulletNum]; // �л��ӵ�λ��
int enemy_bullet_num = 0; // �л��ӵ�����
int canvas[High][Width] = { 0 }; // ��ά����洢��Ϸ�����ж�Ӧ��Ԫ��
// 0Ϊ�ո�1Ϊ�ɻ�*��2Ϊ�ӵ�|��3Ϊ�л�@
int score; // �÷�
int BulletWidth; // �ӵ����
int EnemyMoveSpeed; // �л��ƶ��ٶ�

void gotoxy(int x, int y)  //����ƶ���(x,y)λ��
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(handle, pos);
}

void startup() // ���ݳ�ʼ��
{
	position_x = High - 1;
	position_y = Width / 2;
	canvas[position_x][position_y] = 1;
	int k;
	for (k = 0; k < EnemyNum; k++)
	{
		enemy_x[k] = rand() % 2;
		enemy_y[k] = rand() % Width;
		canvas[enemy_x[k]][enemy_y[k]] = 3;
		enemy_health[k] = EnemyHealth;
	}
	score = 0;
	BulletWidth = 0;
	EnemyMoveSpeed = 20;
}

void show()  // ��ʾ����
{
	gotoxy(0, 0);  // ����ƶ���ԭ��λ�ã������ػ�����
	int i, j;
	for (i = 0; i < High; i++)
	{
		for (j = 0; j < Width; j++)
		{
			if (canvas[i][j] == 0)
				printf(" ");   //   ����ո�
			else if (canvas[i][j] == 1)
				printf("*");   //   ����ɻ�*
			else if (canvas[i][j] == 2)
				printf("|");   //   ����ӵ�|
			else if (canvas[i][j] == 3)
				printf("@");   //  ����ɻ�@
		}
		printf("\n");
	}
	printf("�÷֣�%d\n", score);
	Sleep(20);
}

void updateWithoutInput()  // ���û������޹صĸ���
{
	int i, j, k;

	// �л������ӵ�
	for (k = 0; k < EnemyNum; k++)
	{
		int shoot = rand() % 75; // ��������Ƿ����ӵ�
		if (shoot == 0 && enemy_bullet_num < MaxBulletNum)
		{
			enemy_bullet_x[enemy_bullet_num] = enemy_x[k] + 1; // �ӵ���ʼλ���ڵл������·�
			enemy_bullet_y[enemy_bullet_num] = enemy_y[k];
			enemy_bullet_num++;
		}
	}

	// ��������ӵ�λ��
	for (i = 0; i < player_bullet_num; i++)
	{
		canvas[player_bullet_x[i]][player_bullet_y[i]] = 0; // ���֮ǰ��λ��

		// �ӵ������ƶ�
		player_bullet_x[i]--;
		if (player_bullet_x[i] >= 0)
		{
			// �ж��ӵ��Ƿ���ел�
			for (k = 0; k < EnemyNum; k++)
			{
				if ((player_bullet_x[i] == enemy_x[k]) && (player_bullet_y[i] == enemy_y[k]))
				{
					canvas[player_bullet_x[i]][player_bullet_y[i]] = 0; // �ӵ���ʧ
					enemy_health[k]--; // ���ٵл�Ѫ��
					if (enemy_health[k] <= 0)
					{
						score++; // ������1
						if (score % 5 == 0 && EnemyMoveSpeed > 3) // �ﵽһ�����ֺ󣬵л����
							EnemyMoveSpeed--;
						if (score % 5 == 0) // �ﵽһ�����ֺ��ӵ�������
							BulletWidth++;
						canvas[enemy_x[k]][enemy_y[k]] = 0;
						enemy_x[k] = rand() % 2; // �����µķɻ�
						enemy_y[k] = rand() % Width;
						canvas[enemy_x[k]][enemy_y[k]] = 3;
						enemy_health[k] = EnemyHealth; // ���õл�Ѫ��
					}
					break;
				}
			}

			canvas[player_bullet_x[i]][player_bullet_y[i]] = 2; // �����µ�λ��
		}
		else
		{
			// �ӵ������߽磬�Ƴ��ӵ�
			for (j = i; j < player_bullet_num - 1; j++)
			{
				player_bullet_x[j] = player_bullet_x[j + 1];
				player_bullet_y[j] = player_bullet_y[j + 1];
			}
			player_bullet_num--;
		}
	}

	// ���µл��ӵ�λ��
	for (i = 0; i < enemy_bullet_num; i++)
	{
		canvas[enemy_bullet_x[i]][enemy_bullet_y[i]] = 0; // ���֮ǰ��λ��

		// �ӵ������ƶ�
		enemy_bullet_x[i]++;
		if (enemy_bullet_x[i] < High)
		{
			// �ж��ӵ��Ƿ�������
			if ((enemy_bullet_x[i] == position_x) && (enemy_bullet_y[i] == position_y))
			{
				printf("ʧ�ܣ�\n");
				Sleep(3000);
				system("pause");
				exit(0);
			}

			canvas[enemy_bullet_x[i]][enemy_bullet_y[i]] = 2; // �����µ�λ��
		}
		else
		{
			// �ӵ������߽磬�Ƴ��ӵ�
			for (j = i; j < enemy_bullet_num - 1; j++)
			{
				enemy_bullet_x[j] = enemy_bullet_x[j + 1];
				enemy_bullet_y[j] = enemy_bullet_y[j + 1];
			}
			enemy_bullet_num--;
		}
	}

	static int speed = 0;
	if (speed < EnemyMoveSpeed)
		speed++;

	for (k = 0; k < EnemyNum; k++)
	{
		if ((position_x == enemy_x[k]) && (position_y == enemy_y[k]))  // �л�ײ���һ�
		{
			printf("ʧ�ܣ�\n");
			Sleep(3000);
			system("pause");
			exit(0);
		}

		if (enemy_x[k] > High)   // �л��ܳ���ʾ��Ļ
		{
			canvas[enemy_x[k]][enemy_y[k]] = 0;
			enemy_x[k] = rand() % 2;           // �����µķɻ�
			enemy_y[k] = rand() % Width;
			canvas[enemy_x[k]][enemy_y[k]] = 3;
			score--;  // ����
			enemy_health[k] = EnemyHealth;    // ���õл�Ѫ��
		}

		if (speed == EnemyMoveSpeed)
		{
			// �л�����
			for (k = 0; k < EnemyNum; k++)
			{
				canvas[enemy_x[k]][enemy_y[k]] = 0;
				enemy_x[k]++;
				speed = 0;
				canvas[enemy_x[k]][enemy_y[k]] = 3;
			}
		}
	}
}

void updateWithInput()  // ���û������йصĸ���
{
	char input;
	if (kbhit())  // �ж��Ƿ�������
	{
		input = getch();  // �����û��Ĳ�ͬ�������ƶ�����������س�
		if (input == 'a' && position_y > 0)
		{
			canvas[position_x][position_y] = 0;
			position_y--;  // λ������
			canvas[position_x][position_y] = 1;
		}
		else if (input == 'd' && position_y < Width - 1)
		{
			canvas[position_x][position_y] = 0;
			position_y++;  // λ������
			canvas[position_x][position_y] = 1;
		}
		else if (input == 'w')
		{
			canvas[position_x][position_y] = 0;
			position_x--;  // λ������
			canvas[position_x][position_y] = 1;
		}
		else if (input == 's')
		{
			canvas[position_x][position_y] = 0;
			position_x++;  // λ������
			canvas[position_x][position_y] = 1;
		}
		else if (input == ' ')  // �����ӵ�
		{
			int left = position_y - BulletWidth;
			int right = position_y + BulletWidth;
			if (left < 0)
				left = 0;
			if (right > Width - 1)
				right = Width - 1;
			int k;
			for (k = left; k <= right; k++) // ��������
			{
				// �ж��Ƿ�������ӵ��ص�
				int overlap = 0;
				for (int i = 0; i < player_bullet_num; i++)
				{
					if (position_x - 1 == player_bullet_x[i] && k == player_bullet_y[i])
					{
						overlap = 1;
						break;
					}
				}
				if (!overlap)
				{
					canvas[position_x - 1][k] = 2; // �����ӵ��ĳ�ʼλ���ڷɻ������Ϸ�
					player_bullet_x[player_bullet_num] = position_x - 1;
					player_bullet_y[player_bullet_num] = k;
					player_bullet_num++;
				}
			}
		}
	}
}

int main()
{
	startup();  // ���ݳ�ʼ��
	while (1) //  ��Ϸѭ��ִ��
	{
		show();  // ��ʾ����
		updateWithoutInput();  // ���û������޹صĸ���
		updateWithInput();  // ���û������йصĸ���
	}
	return 0;
}

