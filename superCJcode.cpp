#include<stdlib.h>
#include<windows.h>
#include<conio.h>
#include<graphics.h>
#include<stdio.h>
#define High 480
#define Width 640
#define Bondary 99
int main()
{
	initgraph(Width,High);

	float ball_x,ball_y;
	float ball_vx,ball_vy;
	float radius;

	ball_x = Width/2;
	ball_y = High/2;
	ball_vx = 1;
	ball_vy = 1;
	radius = 20;

	//3. 为反弹球游戏制作边框
	setcolor(YELLOW);
	line(Bondary,Bondary,Width-Bondary,Bondary);
	line(Bondary,Bondary,Bondary,High-Bondary);
	line(Bondary,High-Bondary,Width-Bondary,High-Bondary);
	line(Width-Bondary,Bondary,Width-Bondary,High-Bondary);

	BeginBatchDraw();
	while(1)
	{
		setcolor(BLACK);
		setfillcolor(BLACK);
		fillcircle(ball_x,ball_y,radius);

		ball_x += ball_vx;
		ball_y += ball_vy;

		if( ball_x <= radius + Bondary +1 || ball_x >= Width - radius - Bondary -1 )
		{
			ball_vx *= -1;
			//2.实现小球碰到边界发声
			printf("\a");
		}
		if( ball_y <= radius + Bondary +1 || ball_y >= High - radius - Bondary -1 )
		{
			ball_vy *= -1;
			//2.实现小球碰到边界发声
			printf("\a");
		}

		setcolor(BLUE);
		setfillcolor(GREEN);
		fillcircle(ball_x,ball_y,radius);

		FlushBatchDraw();

		//1.代替Sleep函数
		//Sleep(3);
		for(int i = 0; i <1e6 ; i++);
	}
	EndBatchDraw();
	closegraph();
	return 0;
}
