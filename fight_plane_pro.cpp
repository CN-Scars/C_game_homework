/* Windows */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

#define High 15  // 游戏画面尺寸
#define Width 25
#define EnemyNum 5 // 敌机个数
#define MaxBulletNum 50 // 最大子弹数量
#define EnemyHealth 2	// 敌机血量

// 全局变量
int position_x, position_y; // 飞机位置
int enemy_x[EnemyNum], enemy_y[EnemyNum];  // 敌机位置
int enemy_health[EnemyNum]; // 敌机血量
int player_bullet_x[MaxBulletNum], player_bullet_y[MaxBulletNum]; // 玩家子弹位置
int player_bullet_num = 0; // 玩家子弹数量
int enemy_bullet_x[MaxBulletNum], enemy_bullet_y[MaxBulletNum]; // 敌机子弹位置
int enemy_bullet_num = 0; // 敌机子弹数量
int canvas[High][Width] = { 0 }; // 二维数组存储游戏画布中对应的元素
// 0为空格，1为飞机*，2为子弹|，3为敌机@
int score; // 得分
int BulletWidth; // 子弹宽度
int EnemyMoveSpeed; // 敌机移动速度
bool is_paused = false; // 游戏是否暂停的标志

void gotoxy(int x, int y)  //光标移动到(x,y)位置
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(handle, pos);
}

void set_input_mode()
{
    HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(handle, &mode);
    SetConsoleMode(handle, mode & (~ENABLE_ECHO_INPUT) & (~ENABLE_LINE_INPUT));
}

void set_output_mode()
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursor_info;
    GetConsoleCursorInfo(handle, &cursor_info);
    cursor_info.bVisible = false;
    SetConsoleCursorInfo(handle, &cursor_info);
}

void startup() // 数据初始化
{
    position_x = High - 1;
    position_y = Width / 2;
    
    canvas[position_x][position_y] = 1;
    int k;
    for (k = 0; k < EnemyNum; k++)
    {
        enemy_x[k] = rand() % 2;
        enemy_y[k] = rand() % Width;
        int dy[] = { 0, 1, 0, -1, 0 };
        int dx[] = { 1, 0, -1, 0, 0 };
        for (int i = 0; i < 5; i++)
        {
            int new_X = enemy_x[k] + dx[i], new_Y = enemy_y[k] + dy[i];
            if (new_X >= 0 && new_Y >= 0)
                canvas[new_X][new_Y] = 3;
        }
        enemy_health[k] = EnemyHealth;
    }
    score = 0;
    BulletWidth = 0;
    EnemyMoveSpeed = 20;
}

void show()  // 显示画面
{
    gotoxy(0, 0);  // 光标移动到原点位置，以下重画清屏
    int i, j;
    for (i = 0; i < High; i++)
    {
        for (j = 0; j < Width; j++)
        {
            if (canvas[i][j] == 0)
                printf(" ");   //   输出空格
            else if (canvas[i][j] == 1)
                printf("*");   //   输出飞机*
            else if (canvas[i][j] == 2)
                printf("|");   //   输出子弹|
            else if (canvas[i][j] == 3)
                printf("@");   //  输出飞机@
        }
        printf("\n");
    }
    printf("得分：%d\n", score);
    Sleep(20);
}

void updateWithoutInput()  // 与用户输入无关的更新
{
    int i, j, k;

    // 敌机发射子弹
    for (k = 0; k < EnemyNum; k++)
    {
        int shoot = rand() % 75; // 随机决定是否发射子弹
        if (shoot == 0 && enemy_bullet_num < MaxBulletNum)
        {
            enemy_bullet_x[enemy_bullet_num] = enemy_x[k] + 1; // 子弹初始位置在敌机的正下方
            enemy_bullet_y[enemy_bullet_num] = enemy_y[k];
            enemy_bullet_num++;
        }
    }

    // 更新玩家子弹位置
    for (int i = 0; i < player_bullet_num; i++)
    {
        canvas[player_bullet_x[i]][player_bullet_y[i]] = 0; // 清除之前的位置

        // 子弹向上移动
        player_bullet_x[i]--;
        if (player_bullet_x[i] >= 0)
        {
            // 判断子弹是否击中敌机
            for (k = 0; k < EnemyNum; k++)
            {
                int dy[] = { 0, 1, 0, -1, 0 };
                int dx[] = { 1, 0, -1, 0, 0 };
                for (int I = 0; I < 5; I++)
                {
                    int new_X = enemy_x[k] + dx[I], new_Y = enemy_y[k] + dy[I];
                    if ((player_bullet_x[i] == new_X) && (player_bullet_y[i] == new_Y))
                    {
                        canvas[player_bullet_x[i]][player_bullet_y[i]] = 0; // 子弹消失
                        enemy_health[k]--; // 减少敌机血量
                        if (enemy_health[k] <= 0)
                        {
                            score++; // 分数加1
                            if (score % 5 == 0 && EnemyMoveSpeed > 3) // 达到一定积分后，敌机变快
                                EnemyMoveSpeed--;
                            if (score % 5 == 0) // 达到一定积分后，子弹变厉害
                                BulletWidth++;
                            for (j = 0; j < 5; j++)
                            {
                                new_X = enemy_x[k] + dx[j], new_Y = enemy_y[k] + dy[j];
                                if (new_X >= 0 && new_Y >= 0)
                                    canvas[new_X][new_Y] = 0;
                            }
                            enemy_x[k] = rand() % 2; // 产生新的飞机
                            enemy_y[k] = rand() % Width;
                            canvas[enemy_x[k]][enemy_y[k]] = 3;
                            enemy_health[k] = EnemyHealth; // 重置敌机血量
                        }
                        break;
                    }
                }
            }

            canvas[player_bullet_x[i]][player_bullet_y[i]] = 2; // 更新新的位置
        }
        else
        {
            // 子弹超出边界，移除子弹
            for (j = i; j < player_bullet_num - 1; j++)
            {
                player_bullet_x[j] = player_bullet_x[j + 1];
                player_bullet_y[j] = player_bullet_y[j + 1];
            }
            player_bullet_num--;
        }
    }

    // 更新敌机子弹位置
    for (i = 0; i < enemy_bullet_num; i++)
    {
        canvas[enemy_bullet_x[i]][enemy_bullet_y[i]] = 0; // 清除之前的位置

        // 子弹向下移动
        enemy_bullet_x[i]++;
        if (enemy_bullet_x[i] < High)
        {
            // 判断子弹是否击中玩家
            if ((enemy_bullet_x[i] == position_x) && (enemy_bullet_y[i] == position_y))
            {
                printf("失败！\n");
                Sleep(3000);
                system("pause");
                exit(0);
            }

            canvas[enemy_bullet_x[i]][enemy_bullet_y[i]] = 2; // 更新新的位置
        }
        else
        {
            // 子弹超出边界，移除子弹
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
        int dy[] = { 0, 1, 0, -1, 0 };
        int dx[] = { 1, 0, -1, 0, 0 };
        for (int i = 0; i < 5; i++)
        {
            int new_X = enemy_x[k] + dx[i], new_Y = enemy_y[k] + dy[i];
            if ((position_x == new_X) && (position_y == new_Y))  // 敌机撞到我机
            {
                printf("失败！\n");
                Sleep(3000);
                system("pause");
                exit(0);
            }       
        }
        

        if (enemy_x[k] - 1 > High)   // 敌机跑出显示屏幕
        {
            canvas[enemy_x[k]][enemy_y[k]] = 0;
            enemy_x[k] = rand() % 2;           // 产生新的飞机
            enemy_y[k] = rand() % Width;
            canvas[enemy_x[k]][enemy_y[k]] = 3;
            score--;  // 减分
            enemy_health[k] = EnemyHealth;    // 重置敌机血量
        }

        if (speed == EnemyMoveSpeed)
        {
            // 敌机下落
            for (k = 0; k < EnemyNum; k++)
            {
                int dy[] = { 0, 1, 0, -1, 0 };
                int dx[] = { 1, 0, -1, 0, 0 };
                for (int i = 0; i < 5; i++)
                {
                    int new_X = enemy_x[k] + dx[i], new_Y = enemy_y[k] + dy[i];
                    if (new_X >= 0 && new_Y >= 0)
                        canvas[new_X][new_Y] = 0;
                }
                enemy_x[k]++;
                speed = 0;
                for (int i = 0; i < 5; i++)
                {
                    int new_X = enemy_x[k] + dx[i], new_Y = enemy_y[k] + dy[i];
                    if (new_X >= 0 && new_Y >= 0)
                        canvas[enemy_x[k] + dx[i]][enemy_y[k] + dy[i]] = 3;
                }
            }
        }
    }
}

void updateWithInput()  // 与用户输入有关的更新
{
    char input;
    if (_kbhit())  // 判断是否有输入
    {
        input = _getch();  // 根据用户的不同输入来移动，不必输入回车
        if (input == 'a' && position_y > 0)
        {
            canvas[position_x][position_y] = 0;
            position_y--;  // 位置左移
            canvas[position_x][position_y] = 1;
        }
        else if (input == 'd' && position_y < Width - 1)
        {
            canvas[position_x][position_y] = 0;
            position_y++;  // 位置右移
            canvas[position_x][position_y] = 1;
        }
        else if (input == 'w' && position_x > 0)
        {
            canvas[position_x][position_y] = 0;
            position_x--;  // 位置上移
            canvas[position_x][position_y] = 1;
        }
        else if (input == 's' && position_x < High - 1)
        {
            canvas[position_x][position_y] = 0;
            position_x++;  // 位置下移
            canvas[position_x][position_y] = 1;
        }
        else if (input == ' ')  // 发射子弹
        {
            int left = position_y - BulletWidth;
            int right = position_y + BulletWidth;
            if (left < 0)
                left = 0;
            if (right > Width - 1)
                right = Width - 1;
            int k;
            for (k = left; k <= right; k++) // 发射闪弹
            {
                // 判断是否与玩家子弹重叠
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
                    canvas[position_x - 1][k] = 2; // 发射子弹的初始位置在飞机的正上方
                    player_bullet_x[player_bullet_num] = position_x - 1;
                    player_bullet_y[player_bullet_num] = k;
                    player_bullet_num++;
                }
            }
        }
        else if (input == 27) {  // 按下Esc键
            is_paused = !is_paused;  // 切换暂停状态
        }
    }
}

int main()
{
    set_input_mode();  // 设置输入模式
    set_output_mode();  // 设置输出模式
    startup();  // 数据初始化
    while (1) //  游戏循环执行
    {
        if (!is_paused) {
            show();  // 显示画面
            updateWithoutInput();  // 与用户输入无关的更新
            updateWithInput();  // 与用户输入有关的更新
        }

        if (is_paused) {
            while (is_paused) {
                if (_kbhit() && _getch() == 27) {
                    is_paused = !is_paused;  // 切换暂停状态
                }
                Sleep(50);
            }
        }

        Sleep(50);  // 添加适当的延迟
    }
    return 0;
}

/* Linux
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

#define High 15  // 游戏画面尺寸
#define Width 25
#define EnemyNum 5 // 敌机个数
#define MaxBulletNum 50 // 最大子弹数量
#define EnemyHealth 2 // 敌机血量

// 全局变量
int position_x, position_y; // 飞机位置
int enemy_x[EnemyNum], enemy_y[EnemyNum]; // 敌机位置
int enemy_health[EnemyNum]; // 敌机血量
int player_bullet_x[MaxBulletNum], player_bullet_y[MaxBulletNum]; // 玩家子弹位置
int player_bullet_num = 0; // 玩家子弹数量
int enemy_bullet_x[MaxBulletNum], enemy_bullet_y[MaxBulletNum]; // 敌机子弹位置
int enemy_bullet_num = 0; // 敌机子弹数量
int canvas[High][Width] = {0}; // 二维数组存储游戏画布中对应的元素
// 0为空格，1为飞机*，2为子弹|，3为敌机@
int score; // 得分
int BulletWidth; // 子弹宽度
int EnemyMoveSpeed; // 敌机移动速度
int is_paused = 0; // 是否暂停

void gotoxy(int x, int y) //光标移动到(x,y)位置
{
    printf("\033[%d;%dH", x + 1, y + 1);
    fflush(stdout);
}

void set_input_mode(void) {
    struct termios tattr;
    tcgetattr(STDIN_FILENO, &tattr);
    tattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &tattr);
    setbuf(stdin, NULL);
}

void set_output_mode(void) {
    struct termios tattr;
    tcgetattr(STDOUT_FILENO, &tattr);
    tattr.c_lflag &= ~(ICANON);
    tcsetattr(STDOUT_FILENO, TCSANOW, &tattr);
}

void reset_input_mode(void) {
    struct termios tattr;
    tcgetattr(STDIN_FILENO, &tattr);
    tattr.c_lflag |= ICANON | ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &tattr);
}

int kbhit(void) {
    struct timeval tv = {0L, 0L};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}

void startup() // 数据初始化
{
    position_x = High - 1;
    position_y = Width / 2;

    canvas[position_x][position_y] = 1;
    int k;
    for (k = 0; k < EnemyNum; k++) {
        enemy_x[k] = rand() % 2;
        enemy_y[k] = rand() % Width;
        int dy[] = {0, 1, 0, -1, 0};
        int dx[] = {1, 0, -1, 0, 0};
        for (int i = 0; i < 5; i++) {
            int new_X = enemy_x[k] + dx[i], new_Y = enemy_y[k] + dy[i];
            if (new_X >= 0 && new_Y >= 0)
                canvas[new_X][new_Y] = 3;
        }
        enemy_health[k] = EnemyHealth;
    }
    score = 0;
    BulletWidth = 0;
    EnemyMoveSpeed = 20;
}

void show() // 显示画面
{
    system("clear"); // 清屏
    int i, j;
    for (i = 0; i < High; i++) {
        for (j = 0; j < Width; j++) {
            if (canvas[i][j] == 0)
                printf(" "); // 输出空格
            else if (canvas[i][j] == 1)
                printf("*"); // 输出飞机*
            else if (canvas[i][j] == 2)
                printf("|"); // 输出子弹|
            else if (canvas[i][j] == 3)
                printf("@"); // 输出飞机@
        }
        printf("\n");
    }
    printf("得分：%d\n", score);
    usleep(50000);
}

void updateWithoutInput() // 与用户输入无关的更新
{
    if (is_paused) {
        return; // 如果游戏暂停，不进行更新
    }

    int i, j, k;

    // 敌机发射子弹
    for (k = 0; k < EnemyNum; k++) {
        int shoot = rand() % 75; // 随机决定是否发射子弹
        if (shoot == 0 && enemy_bullet_num < MaxBulletNum) {
            enemy_bullet_x[enemy_bullet_num] = enemy_x[k] + 1; // 子弹初始位置在敌机的正下方
            enemy_bullet_y[enemy_bullet_num] = enemy_y[k];
            enemy_bullet_num++;
        }
    }

    // 更新玩家子弹位置
    for (int i = 0; i < player_bullet_num; i++) {
        canvas[player_bullet_x[i]][player_bullet_y[i]] = 0; // 清除之前的位置

        // 子弹向上移动
        player_bullet_x[i]--;
        if (player_bullet_x[i] >= 0) {
            // 判断子弹是否击中敌机
            for (k = 0; k < EnemyNum; k++) {
                int dy[] = {0, 1, 0, -1, 0};
                int dx[] = {1, 0, -1, 0, 0};
                for (int I = 0; I < 5; I++) {
                    int new_X = enemy_x[k] + dx[I], new_Y = enemy_y[k] + dy[I];
                    if ((player_bullet_x[i] == new_X) && (player_bullet_y[i] == new_Y)) {
                        canvas[player_bullet_x[i]][player_bullet_y[i]] = 0; // 子弹消失
                        enemy_health[k]--; // 减少敌机血量
                        if (enemy_health[k] <= 0) {
                            score++; // 分数加1
                            if (score % 5 == 0 && EnemyMoveSpeed > 3) // 达到一定积分后，敌机变快
                                EnemyMoveSpeed--;
                            if (score % 5 == 0) // 达到一定积分后，子弹变厉害
                                BulletWidth++;
                            for (j = 0; j < 5; j++) {
                                new_X = enemy_x[k] + dx[j], new_Y = enemy_y[k] + dy[j];
                                if (new_X >= 0 && new_Y >= 0)
                                    canvas[new_X][new_Y] = 0;
                            }
                            enemy_x[k] = rand() % 2; // 产生新的飞机
                            enemy_y[k] = rand() % Width;
                            canvas[enemy_x[k]][enemy_y[k]] = 3;
                            enemy_health[k] = EnemyHealth; // 重置敌机血量
                        }
                        break;
                    }
                }
            }

            canvas[player_bullet_x[i]][player_bullet_y[i]] = 2; // 更新新的位置
        } else {
            // 子弹超出边界，移除子弹
            for (j = i; j < player_bullet_num - 1; j++) {
                player_bullet_x[j] = player_bullet_x[j + 1];
                player_bullet_y[j] = player_bullet_y[j + 1];
            }
            player_bullet_num--;
        }
    }

    // 更新敌机子弹位置
    for (i = 0; i < enemy_bullet_num; i++) {
        canvas[enemy_bullet_x[i]][enemy_bullet_y[i]] = 0; // 清除之前的位置

        // 子弹向下移动
        enemy_bullet_x[i]++;
        if (enemy_bullet_x[i] < High) {
            // 判断子弹是否击中玩家
            if ((enemy_bullet_x[i] == position_x) && (enemy_bullet_y[i] == position_y)) {
                printf("失败！\n");
                reset_input_mode(); // 恢复终端的输入模式
                usleep(3000000);
                exit(0);
            }

            canvas[enemy_bullet_x[i]][enemy_bullet_y[i]] = 2; // 更新新的位置
        } else {
            // 子弹超出边界，移除子弹
            for (j = i; j < enemy_bullet_num - 1; j++) {
                enemy_bullet_x[j] = enemy_bullet_x[j + 1];
                enemy_bullet_y[j] = enemy_bullet_y[j + 1];
            }
            enemy_bullet_num--;
        }
    }

    static int speed = 0;
    if (speed < EnemyMoveSpeed)
        speed++;

    for (k = 0; k < EnemyNum; k++) {
        int dy[] = {0, 1, 0, -1, 0};
        int dx[] = {1, 0, -1, 0, 0};
        for (int i = 0; i < 5; i++) {
            int new_X = enemy_x[k] + dx[i], new_Y = enemy_y[k] + dy[i];
            if ((position_x == new_X) && (position_y == new_Y)) // 敌机撞到我机
            {
                printf("失败！\n");
                reset_input_mode(); // 恢复终端的输入模式
                usleep(3000000);
                exit(0);
            }
        }

        if (enemy_x[k] - 1 > High) // 敌机跑出显示屏幕
        {
            canvas[enemy_x[k]][enemy_y[k]] = 0;
            enemy_x[k] = rand() % 2; // 产生新的飞机
            enemy_y[k] = rand() % Width;
            canvas[enemy_x[k]][enemy_y[k]] = 3;
            score--; // 减分
            enemy_health[k] = EnemyHealth; // 重置敌机血量
        }

        if (speed == EnemyMoveSpeed) {
            // 敌机下落
            for (k = 0; k < EnemyNum; k++) {
                int dy[] = {0, 1, 0, -1, 0};
                int dx[] = {1, 0, -1, 0, 0};
                for (int i = 0; i < 5; i++) {
                    int new_X = enemy_x[k] + dx[i], new_Y = enemy_y[k] + dy[i];
                    if (new_X >= 0 && new_Y >= 0)
                        canvas[new_X][new_Y] = 0;
                }
                enemy_x[k]++;
                speed = 0;
                for (int i = 0; i < 5; i++) {
                    int new_X = enemy_x[k] + dx[i], new_Y = enemy_y[k] + dy[i];
                    if (new_X >= 0 && new_Y >= 0)
                        canvas[enemy_x[k] + dx[i]][enemy_y[k] + dy[i]] = 3;
                }
            }
        }
    }
}

void updateWithInput() // 与用户输入有关的更新
{
    char input;
    if (kbhit()) // 判断是否有输入
    {
        input = getchar(); // 根据用户的不同输入来移动，不必输入回车
        if (input == 27) { // 按下Esc键，切换暂停状态
            is_paused = !is_paused;
        } else if (!is_paused) { // 只处理非暂停状态下的输入
            if (input == 'a' && position_y > 0) {
                canvas[position_x][position_y] = 0;
                position_y--; // 位置左移
                canvas[position_x][position_y] = 1;
            } else if (input == 'd' && position_y < Width - 1) {
                canvas[position_x][position_y] = 0;
                position_y++; // 位置右移
                canvas[position_x][position_y] = 1;
            } else if (input == 'w' && position_x > 0) {
                canvas[position_x][position_y] = 0;
                position_x--; // 位置上移
                canvas[position_x][position_y] = 1;
            } else if (input == 's' && position_x < High - 1) {
                canvas[position_x][position_y] = 0;
                position_x++; // 位置下移
                canvas[position_x][position_y] = 1;
            } else if (input == ' ') // 发射子弹
            {
                int left = position_y - BulletWidth;
                int right = position_y + BulletWidth;
                if (left < 0)
                    left = 0;
                if (right > Width - 1)
                    right = Width - 1;
                int k;
                for (k = left; k <= right; k++) // 发射闪弹
                {
                    // 判断是否与玩家子弹重叠
                    int overlap = 0;
                    for (int i = 0; i < player_bullet_num; i++) {
                        if (position_x - 1 == player_bullet_x[i] && k == player_bullet_y[i]) {
                            overlap = 1;
                            break;
                        }
                    }
                    if (!overlap) {
                        canvas[position_x - 1][k] = 2; // 发射子弹的初始位置在飞机的正上方
                        player_bullet_x[player_bullet_num] = position_x - 1;
                        player_bullet_y[player_bullet_num] = k;
                        player_bullet_num++;
                    }
                }
            }
        }
    }
}

int main()
{
    set_input_mode(); // 设置输入模式
    set_output_mode(); // 设置输出模式
    startup(); // 数据初始化
    while (1) // 游戏循环执行
    {
        show(); // 显示画面
        updateWithoutInput(); // 与用户输入无关的更新
        updateWithInput(); // 与用户输入有关的更新
    }
    return 0;
}
*/
