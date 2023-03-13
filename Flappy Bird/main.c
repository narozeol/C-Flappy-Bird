#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

#define BARRIER_MAX 10
#define BARRIER_WIDTH 4
#define BIRDWIDTH 2
#define BIRDHEIGHT 2
#define SCREENHEIGHT 30
#define SCREENWIDTH 100


struct Object
{
    int x;
    int y;
    int live;
    int type;
};

struct Object bird;
struct Object barrier[BARRIER_MAX];

unsigned long score = 0;
unsigned long record = 0;

//计时器函数
int Timer (int ms, int id)
{
    static clock_t t[10];
    if (clock() - t[id] > ms)
    {
        t[id] = clock();
        return 1;
    }
    else
        return 0;
}
//移动光标函数
void gotoxy(int x, int y)
{
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
//隐藏光标函数
void HideCursor()
{
	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible = FALSE;
	cursor.dwSize = sizeof(cursor);
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(handle, &cursor);
}
//读取最高记录
void ReadRecord ()
{
    FILE *fp;
    fp = fopen ("record", "rb");
    fread (&record, sizeof (unsigned long), 1, fp);
    fclose (fp);
}
//储存最高记录
void SaveRecord ()
{
    FILE *fp;
    fp = fopen ("record", "wb");
    fwrite (&record, sizeof (unsigned long), 1, fp);
    fclose (fp);
}
//游戏初始化
void GameInit ()
{
    //初始化障碍物状态
    for (int i = 0; i < BARRIER_MAX; i++)
        barrier[i].live = 0;
    //初始化小鸟状态
    bird.x = 5;
    bird.y = 10;
    bird.live = 1;
    //读取最高记录
    ReadRecord ();
    //初始化计数
    score = 0;
}
//绘图函数
void Gamedraw ()
{
    //画鸟
    for (int i = bird.x; i < bird.x + BIRDWIDTH; i++)
        for (int j = bird.y; j < bird.y + BIRDHEIGHT; j++)
        {
            //移动光标
            gotoxy (i,j);
            //隐藏光标
            HideCursor ();
            printf ("*");
        }
    //画出障碍
    int tophalf_height, lowerhalf_height;
    for (int k = 0; k < BARRIER_MAX; k++)
    {
        if (barrier[k].live)
        {
            //选择要画出的障碍的类型
            switch (barrier[k].type)
            {
                case 0 :
                    tophalf_height = 5;//设置上半部分最低点
                    lowerhalf_height = 11;//设置下半部分最高点
                    break;
                case 1 :
                    tophalf_height = 11;
                    lowerhalf_height = 17;
                    break;
                case 2 :
                    tophalf_height = 17;
                    lowerhalf_height = 23;
                    break;
                case 3 :
                    tophalf_height = 23;
                    lowerhalf_height  = 29;
                    break;
            }

            for (int i = barrier[k].x; i < barrier[k].x + BARRIER_WIDTH; i++)
            {
                for (int j = 0; j < tophalf_height; j++)
                {
                    //移动光标
                    gotoxy (i,j);
                    //隐藏光标
                    HideCursor ();
                    printf ("#");
                }

                for (int j = lowerhalf_height; j < SCREENHEIGHT; j++)
                {
                    //移动光标
                    gotoxy (i,j);
                    //隐藏光标
                    HideCursor ();
                    printf ("#");
                }
            }
        }
    }
    //画出计分板
    gotoxy (SCREENWIDTH + 5, 4);//移动光标
    printf ("最高得分：%lu", record);
    gotoxy (SCREENWIDTH + 5, 5);
    printf ("当前得分：%lu", score);

    system ("cls");

}
//生成障碍物
void CreateBarrier ()
{
    for (int i = 0; i < BARRIER_MAX; i++)
       if (!barrier[i].live)
        {
            barrier[i].type = rand() % 4;
            barrier[i].live = 1;
            barrier[i].x = SCREENWIDTH;
            return;
        }
}
//移动障碍物
void BarrierMove (int speed)
{
    for (int i = 0; i < BARRIER_MAX; i++)
    {
        if (barrier[i].live)
        {
            barrier[i].x -= speed;
        }
        if (barrier[i].x <= 0)
        {
            barrier[i].live = 0;
        }
    }
}
//按空格控制起飞
void BirdJump (int speed)
{
    int a = GetKeyState (VK_SPACE);
    if (a < 0)
        if ( (bird.y - speed) > 0)
            bird.y -= speed;
}
//小鸟自然的坠落
void BirdDown (int speed)
{
    if (Timer (120,2))
        bird.y += speed;
}
//死亡检测
int DeathCheck ()
{
    int tophalf_height, lowerhalf_height;
    for (int k = 0; k < BARRIER_MAX; k++)
    {
        if (barrier[k].live)
        {
            //选择要检测的障碍的类型，这里只设置4种
            switch (barrier[k].type)
            {
                case 0 :
                    tophalf_height = 5;
                    lowerhalf_height = 11;
                    break;
                case 1 :
                    tophalf_height = 11;
                    lowerhalf_height = 17;
                    break;
                case 2 :
                    tophalf_height = 17;
                    lowerhalf_height = 23;
                    break;
                case 3 :
                    tophalf_height = 23;
                    lowerhalf_height  = 29;
                    break;
            }
            //其实这就是根据之前的绘图函数改的，如果觉得这样写浪费可以整合在一起
            for (int i = barrier[k].x; i < barrier[k].x + BARRIER_WIDTH; i++)
            {
                for (int j = 0; j < tophalf_height; j++)
                {
                    if ( bird.x == i && bird.y == j)
                        return 1;
                    else if ( bird.x == i && (bird.y + BIRDHEIGHT/2 ) == j)
                        return 1;
                    else if ( (bird.x + BIRDWIDTH/2 ) == i && bird.y == j)
                        return 1;
                    else if ( (bird.x + BIRDWIDTH/2 ) == i && (bird.y + BIRDHEIGHT/2 ) == j)
                        return 1;
                }

                for (int j = lowerhalf_height; j < SCREENHEIGHT; j++)
                {
                    if ( bird.x == i && bird.y == j)
                        return 1;
                    else if ( bird.x == i && (bird.y + 1) == j)
                        return 1;
                    else if ( (bird.x + 1) == i && bird.y == j)
                        return 1;
                    else if ( (bird.x + 1) == i && (bird.y + 1) == j)
                        return 1;
                }
            }
        }
    }
    if (bird.y >= SCREENHEIGHT)
        return 1;
    else
        return 0;
}
//记分板
void GameScore ()
{
    if ( Timer (1000,3) )
        score += 10;
    if (record <= score)
        record = score;
}
int main()
{
start:

    GameInit ();//游戏初始化

    while (1)
    {
        Gamedraw ();//画出图像
        if (Timer (2800, 0))
            CreateBarrier ();//生成障碍物
        BarrierMove (1);//障碍物移动
        BirdJump (1);//控制小鸟跳跃
        BirdDown (1);//小鸟自由落体
        GameScore ();//计分板
        if ( DeathCheck () )//死亡检测
        {
            //保存最高纪录
            SaveRecord ();

            gotoxy (SCREENWIDTH + 5, 4);//移动光标
            printf ("最高得分：%lu", record);
            gotoxy (SCREENWIDTH + 5, 5);
            printf ("得分：%lu", score);
            gotoxy (SCREENWIDTH + 5, 6);
            printf ("已结束");
            gotoxy (SCREENWIDTH + 5, 7);
            printf ("按Q退出");
            gotoxy (SCREENWIDTH + 5, 8);
            printf ("按P再来一次");
            break;
        }
    }

    char ch;
    while ( ch = getch () )
    {
        if (ch == 'q' || ch == 'Q')
            break;
        else if (ch == 'p' || ch == 'P')
            goto start;//虽然很不想用goto，但是不用的话太麻烦了。。。。
        else
            continue;
    }
    return 0;
}
