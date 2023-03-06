#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
using namespace std;

const int maxn = 320;     //序列个数
const int max = maxn + 20;//数组大小
const int maxp = max / 10; //最大页数

int inst[max];//指令序列
int page[max];//页地址流
int size;      //内存能容纳的页数

bool in[maxp];  //该页是否在内存里，提高效率
int pin[maxp];  //现在在内存里的页

void welcome()
{
    printf("******************************************\n");
    printf("**            操作系统模拟实验\t\t**\n");
    printf("**             页式存储管理\t\t**\n");
    printf("******************************************\n\n");
}

void input_hint()
{
    printf("\n1--create new instruction sequence      2--set memory page number(4 to 32)\n");
    printf("3--solve by FIFO algorithm              4--solve by LRU algorithm\n");
    printf("5--solve by OPT algorithm               0--exit\n");
    printf("*********Please input Your choice:  ");
}

/*通过随机数产生一个指令序列，共320条指令*/
void produce_inst()
{
    int m, n;
    int num = 0;

    while (num < maxn)
    {
        m = rand() % maxn;
        inst[num++] = (m + 1) % maxn;
        //inst[num++] = (m + 2) % maxn;
        if (num == maxn) break;

        m = (m + 2) % maxn;
        if (m == 0) m = 160;
        n = rand() % m;
        inst[num++] = (n + 1) % maxn;
        //inst[num++] = (n + 2) % maxn;
        if (num == maxn) break;

        n = (n + 2) % maxn;
        m = maxn - n;
        if (m == 0) m = 160;
        m = rand() % m + n;
        inst[num++] = m;
        //inst[num++] = m + 1;
    }
}
/*将指令序列变换成为页地址流*/
void turn_page_address()
{
    for (int i = 0; i < maxn; i++) {
        page[i] = inst[i] / 10;
    }
}

void FIFO_solve()
{
    memset(in, false, sizeof(in));
    int fault_n = 0;//缺页率
    int ptr, i;

    //预调页填满空间    
    ptr = 0; //下一个要放的位置
    for (i = 0; i < maxn && ptr < size; i++)
        if (!in[page[i]])
        {
            pin[ptr++] = page[i];
            printf("调入内存%d\n", page[i]);
            in[page[i]] = true;
            fault_n++;
        }
    printf("\n\n******内存页已满进行页调度算法******\n\n");
    //继续执行剩下的指令
    ptr = 0;//队列里最先进来的位置，即下一个要被替换的位置
    for (; i < maxn; i++)
        if (!in[page[i]])
        {
            ptr %= size;
            in[pin[ptr]] = false;
            printf("调出内存%d\n", pin[ptr]);
            pin[ptr++] = page[i];
            printf("调入内存%d\n", page[i]);
            in[page[i]] = true;
            fault_n++;
            /*--------------*/
            //请补全该部分
            /*--------------*/
        }

    printf("\nBy FIFO algorithm, the fault-page number is:  %d\n", fault_n);
    printf("                   the hit ratio is :  %.2lf\n", (1 - (fault_n + 0.0) / 320.0));
}

void LRU_solve()
{
    int ltu[maxp]; //last_time_use
    int ti = 1;    //模拟时间
    int fault_n = 0;
    memset(ltu, 0, sizeof(ltu));
    memset(in, false, sizeof(in));
    memset(pin, -1, sizeof(pin));

    int min, ptr, i, j;
    for (i = 0; i < maxn; i++)
    {
        if (!in[page[i]])
        {
            //寻找lru
            min = 1000000; ptr = 0;
            for (j = 0; j < size; j++)
            {
                if (ltu[j] < min)
                {
                    ptr = j;
                    min = ltu[ptr];
                    /*--------------*/
                    //请补全该部分
                    /*--------------*/
                }
            }
            //替换或写入 
            if (pin[ptr] != -1) {
                in[pin[ptr]] = false;
                printf("调出内存%d\n", pin[ptr]);
            }
            pin[ptr] = page[i];
            printf("调入内存%d\n", page[i]);
            in[page[i]] = true;
            /*--------------*/
            //请补全该部分
            /*--------------*/
            fault_n++;
            ltu[ptr] = ti++;
        }
        else//已经在内存里则只需更改最近使用时间    
        {
            for (j = 0; j < size; j++)
                if (pin[j] == page[i])
                {
                    ltu[j] = ti++;
                    break;
                }
        }
    }

    printf("\nBy LRU algorithm, the fault-page number is:  %d\n", fault_n);
    printf("                   the hit ratio is :  %.2lf\n", (1 - (fault_n + 0.0) / 320.0));
}

void OPT_solve()
{
    int ntu[maxp];//next_time_use
    int fault_n = 0;
    int i, j;
    memset(in, false, sizeof(in));
    memset(ntu, -1, sizeof(ntu));

    //预调页填满
    int ptr = 0;
    for (i = 0; i < maxn && fault_n < size; i++)
    {
        if (!in[page[i]])
        {
            in[page[i]] = true;
            pin[ptr] = page[i];
            printf("调入内存%d\n", page[i]);
            fault_n++;
            ptr++;
        }
    }
    printf("\n\n******内存页已满进行页调度算法******\n\n");
    //初始化ntu数组
    ptr = 0;
    for (j = i; j < maxn && ptr < 32; j++)
    {
        if (ntu[page[j]] == -1)
        {
            ntu[page[j]] = j;
            ptr++;
            /*--------------*/
            //请补全该部分
            /*--------------*/
        }
    }

    int max;
    for (; i < maxn; i++)
    {
        if (!in[page[i]])
        {
            max = 0; ptr = 0;
            for (j = 0; j < size; j++)
            {
                if (ntu[pin[j]] == -1)
                {
                    ptr = j;
                    break;
                }
                if (ntu[pin[j]] > max)
                {
                    ptr = j;
                    max = ntu[pin[ptr]];
                    /*--------------*/
                    //请补全该部分
                    /*--------------*/
                }
            }
            in[pin[ptr]] = false;
            printf("调出内存%d\n", pin[ptr]);
            in[page[i]] = true;
            pin[ptr] = page[i];
            printf("调入内存%d\n", page[i]);
            fault_n++;
        }

        ntu[page[i]] = -1;
        for (j = i + 1; j < maxn; j++)
            if (page[j] == page[i])
            {
                ntu[page[i]] = j;
                break;
            }
    }

    printf("\nBy OPT algorithm, the fault-page number is:  %d\n", fault_n);
    printf("                   the hit ratio is :  %.2lf\n", (1 - (fault_n + 0.0) / 320.0));
}

int main()
{
    srand(time(NULL));
    welcome();

    int choice;
    while (1)
    {
        input_hint();
       
        scanf("%d", &choice);
        printf("\n");

        if (choice == 0)
        {
            printf("BYE-BYE!!!\n");
            break;
        }
        if (choice == 1)
        {
            produce_inst();
            turn_page_address();
            printf("指令 页地址\t指令 页地址\t指令 页地址\t指令 页地址\n");
            for (int i = 0; i < max - 20; i++)
            {
                printf("%4d %4d\t", inst[i], page[i]);
                if ((i + 1) % 4 == 0)
                    printf("第%d行\n", (i + 1) / 4);
            }
            printf("New page address sequence is set OK!!!\n");
        }
        else if (choice == 2)
        {
            printf("Please input the size of memory page number:  ");
            scanf("%d", &size);
        }
        else if (choice == 3)
            FIFO_solve();
        else if (choice == 4)
            LRU_solve();
        else if (choice == 5)
            OPT_solve();
        else
            printf("INPUT ERROR !!! \n");
    }

    return 0;
}
