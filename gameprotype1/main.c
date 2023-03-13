#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#define MAX 21
#define CLEANCOMMOND "cls"
int main()
{
    char ch;
    int num[MAX][MAX] = {0};
    int row=(MAX-1)/2,col=(MAX-1)/2,flag = 0;
    int rowtemp , coltemp;
    num[(MAX-1)/2][(MAX-1)/2] = 1;
    for (int i = 0; i < MAX; i++)
    {
        for (int j = 0; j < MAX; j++)
            printf ("%d",num[i][j]);
        printf ("\n");
    }
    printf ("按wasd移动，按p退出\n");
    while ((ch = getch()) != 'P' && ch != 'p')
    {
        num[row][col] = 0;
        rowtemp = row;
        coltemp = col;
        switch (ch)
        {
            case 'w':
            case 'W':
                row-=1;
                break;
            case 's':
            case 'S':
                row+=1;
                break;
            case 'a':
            case 'A':
                col-=1;
                break;
            case 'd':
            case 'D':
                col+=1;
                break;
            default :
                printf ("请按wasd中的一个，退出按p\n");
                flag = 1;
                break;
        }
        if (flag == 1)
        {
            flag = 0;
            continue;
        }
        if (row<0 || row>MAX-1 || col<0 || col >MAX-1 )
        {
            row = rowtemp;
            col = coltemp;
            continue;
        }
        num [row][col] = 1;
        system (CLEANCOMMOND);
        for (int i = 0; i < MAX; i++)
        {
            for (int j = 0; j < MAX; j++)
                printf ("%d",num[i][j]);
            printf ("\n");
        }
        printf ("按wasd移动，按p退出\n");

    }
    system (CLEANCOMMOND);
    printf ("game over!");
    return 0;
}
