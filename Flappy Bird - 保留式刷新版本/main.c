#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <string.h>
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

char buffer_print[SCREENWIDTH][SCREENHEIGHT] = {0};
char buffer_front[SCREENWIDTH][SCREENHEIGHT] = {0};
char buffer_temp[SCREENWIDTH][SCREENHEIGHT] = {0};

unsigned long score = 0;
unsigned long record = 0;

//��ʱ������
int Timer (int ms, int id)
{
    static clock_t t[10];//��ʹ��0,1,2,3,4��
    if (clock() - t[id] > ms)
    {
        t[id] = clock();
        return 1;
    }
    else
        return 0;
}
//�ƶ���꺯��
void gotoxy(int x, int y)
{
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
//���ع�꺯��
void HideCursor()
{
	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible = FALSE;
	cursor.dwSize = sizeof(cursor);
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(handle, &cursor);
}
//��ȡ��߼�¼
void ReadRecord ()
{
    FILE *fp;
    fp = fopen ("record", "rb");
    fread (&record, sizeof (unsigned long), 1, fp);
    fclose (fp);
}
//������߼�¼
void SaveRecord ()
{
    FILE *fp;
    fp = fopen ("record", "wb");
    fwrite (&record, sizeof (unsigned long), 1, fp);
    fclose (fp);
}
//��Ϸ��ʼ��
void GameInit ()
{
    //��ʼ���ϰ���״̬
    for (int i = 0; i < BARRIER_MAX; i++)
        barrier[i].live = 0;
    //��ʼ��С��״̬
    bird.x = 5;
    bird.y = 10;
    bird.live = 1;
    //��ȡ��߼�¼
    ReadRecord ();
    //��ʼ������
    score = 0;
}
//���û�����1
void SetBuffer1 ()
{
    for (int i = 0; i < SCREENWIDTH; i++)
        for (int j = 0; j < SCREENHEIGHT; j++)
            buffer_print[i][j] = 0;
    //����
    for (int i = bird.x; i < bird.x + BIRDWIDTH; i++)
        for (int j = bird.y; j < bird.y + BIRDHEIGHT; j++)
        {
            buffer_print[i][j] = 1;
        }
    //�����ϰ�
    int tophalf_height, lowerhalf_height;
    for (int k = 0; k < BARRIER_MAX; k++)
    {
        if (barrier[k].live)
        {
            //ѡ��Ҫ�������ϰ�������
            switch (barrier[k].type)
            {
                case 0 :
                    tophalf_height = 5;//�����ϰ벿����͵�
                    lowerhalf_height = 11;//�����°벿����ߵ�
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
                    buffer_print[i][j] = 2;
                }

                for (int j = lowerhalf_height; j < SCREENHEIGHT; j++)
                {
                    buffer_print[i][j] = 2;
                }
            }
        }
    }
    //�����Ʒְ�
    gotoxy (SCREENWIDTH + 5, 4);//�ƶ����
    printf ("��ߵ÷֣�%lu", record);
    gotoxy (SCREENWIDTH + 5, 5);
    printf ("��ǰ�÷֣�%lu", score);
}
//�Ƚϻ�����
void BufferCamp ()
{
    for (int i = 0; i < SCREENWIDTH; i++)
        for (int j = 0; j < SCREENHEIGHT; j++)
        {
            if (buffer_print[i][j] != buffer_front[i][j])
                continue;
            else
                buffer_print[i][j] = 3;
        }
}
//��ͼ
void GameDraw ()
{
    for (int i = 0; i < SCREENWIDTH; i++)
        for (int j = 0; j < SCREENHEIGHT; j++)
        {
            if (buffer_print[i][j] == 3)
                continue;
            else
            {
                gotoxy (i, j);
                HideCursor();
                switch (buffer_print[i][j])
                {
                case 1:
                    printf ("*");
                    break;
                case 2:
                    printf ("#");
                    break;
                case 0:
                    printf (" ");
                    break;
                }
            }
        }
}
//�����ϰ���
void CreateBarrier ()
{
    for (int i = 0; i < BARRIER_MAX; i++)
       if (!barrier[i].live)
        {
            barrier[i].type = rand() % 4;
            barrier[i].live = 1;
            barrier[i].x = SCREENWIDTH - BARRIER_WIDTH;
            return;
        }
}
//�ƶ��ϰ���
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
//���ո�������
void BirdJump (int speed)
{
    int a = GetKeyState (VK_SPACE);
    if (a < 0)
        if ( (bird.y - speed) > 0)
            bird.y -= speed;
}
//С����Ȼ��׹��
void BirdDown (int speed)
{
    if (Timer (120,2))
        bird.y += speed;
}
//�������
int DeathCheck ()
{
    int tophalf_height, lowerhalf_height;
    for (int k = 0; k < BARRIER_MAX; k++)
    {
        if (barrier[k].live)
        {
            //ѡ��Ҫ�����ϰ������ͣ�����ֻ����4��
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
            //��ʵ����Ǹ���֮ǰ�Ļ�ͼ�����ĵģ������������д�˷ѿ���������һ��
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
    if (bird.y > SCREENHEIGHT - BIRDHEIGHT)
        return 1;
    else
        return 0;
}
//�Ƿְ�
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

    GameInit ();//��Ϸ��ʼ��

    while (1)
    {
        SetBuffer1 ();//���û�����1
        memcpy (buffer_temp, buffer_print, sizeof (buffer_front));
        BufferCamp ();//�Ƚϻ�����
        memcpy (buffer_front, buffer_temp, sizeof (buffer_front));
        GameDraw ();//��ͼ
        if (Timer (2800, 0))
            CreateBarrier ();//�����ϰ���
        if (Timer (30, 4))
        {
            BarrierMove (1);//�ϰ����ƶ�
            BirdJump (1);//����С����Ծ
            BirdDown (1);//С����������
        }
        GameScore ();//�Ʒְ�

        if ( DeathCheck () )//�������
        {
            //������߼�¼
            SaveRecord ();

            gotoxy (SCREENWIDTH + 5, 4);//�ƶ����
            printf ("��ߵ÷֣�%lu", record);
            gotoxy (SCREENWIDTH + 5, 5);
            printf ("�÷֣�%lu       ", score);
            gotoxy (SCREENWIDTH + 5, 6);
            printf ("�ѽ���");
            gotoxy (SCREENWIDTH + 5, 7);
            printf ("��Q�˳�");
            gotoxy (SCREENWIDTH + 5, 8);
            printf ("��P����һ��");
            break;
        }
    }

    char ch;
    while ( ch = getch () )
    {
        if (ch == 'q' || ch == 'Q')
            break;
        else if (ch == 'p' || ch == 'P')
        {
            system("cls");
            goto start;//��Ȼ�ܲ�����goto�����ǲ��õĻ�̫�鷳�ˡ�������
        }
        else
            continue;
    }
    return 0;
}
