#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

#define getpch(type) (type*)malloc(sizeof(type))
#define NOTHING 0

typedef struct pcb {//进程管理块
    char name[10];//进程名字
    char state;     //进程状态
    int ntime;      //进程需要运行的时间
    int rtime;      //进程已经运行的时间
    struct pcb* link;
}PCB;

PCB* ready = NOTHING, * pfend = NOTHING, * p = NOTHING, * stop = NOTHING, * stopend = NOTHING;      //就绪队列，进程插入位置的变量

int geti()  //使用户仅能输入整数
{
    char ch;
    int i = 0;
    fflush(stdin);
    ch = getchar();

    while (ch == '\n') {
        //printf("\tf输入不能为空..请重新输入\n");
        fflush(stdin);
        ch = getchar();
    }

    while (ch != '\n') {
        if (ch > '9' || ch < '0') {
            printf("\t输入有误!!输入只能为正整数，请重新输入...\n");
            fflush(stdin);
            i = 0;
            ch = getchar();
        }
        else {
            i = i * 10 + (ch - '0');
            ch = getchar();
        }
    }
    return i;
}

void fcfs()//插入进程
{
    if (!ready) {
        ready = p;
        pfend = p;
    }
    else {
        p->link = pfend->link;
        pfend->link = p;
        pfend = p;
    }
}

void input()/*建立进程控制块函数*/
{
    int i, num;
    printf("\n请输入进程的个数?");
    num = geti();
    for (i = 0; i < num; i++)
    {
        printf("\n进程号No.%d:\n", i + 1);
        p = getpch(PCB);
        printf("\n输入进程名:");
        scanf("%s", p->name);
        printf("\n输入进程运行时间:");
        p->ntime = geti();
        printf("\n");
        p->rtime = 0;
        p->state = 'w';
        p->link = NOTHING;
        fcfs();
    }
}

void disp(PCB* pr)/*建立进程现实函数，用于显示当前进程*/
{
    printf("\nname\t state\t ntime\t rtime\t \n");
    printf("|%s\t", pr->name);
    printf(" |%c\t", pr->state);
    printf(" |%d\t", pr->ntime);
    printf(" |%d\t", pr->rtime);
    printf("\n");
}

void check()/*建立进程查看函数*/
{
    PCB* pr;
    if (ready != NOTHING) {
        printf("\n ****当前正在运行的进程是:%s", ready->name);/*显示当前运行的进程*/
        disp(ready);
        pr = ready->link;
        printf("\n****当前就绪队列状态为:\n");/*显示就绪队列状态*/
        while (pr != NOTHING)
        {
            disp(pr);
            pr = pr->link;
        }
    }
    printf("\n****当前就阻塞队列状态为:\n");
    pr = stop;
    while (pr != NOTHING) {
        disp(pr);
        pr = pr->link;
    }
}

void destroy()/*建立进程撤销函数(进程运行结束，撤销进程)*/
{
    printf("\n进程[%s]已完成.\n", ready->name);
    p = ready;
    ready = ready->link;
    free(p);
}

void running()/*建立进程就绪函数(进程运行时间到，置就绪状态)*/
{
    (ready->rtime)++;
    check();
    if (ready->rtime == ready->ntime) {
        destroy();
        return;
    }
}

void block()
{
    char name[10];
    PCB* p1 = ready, * p2 = ready->link;
    printf("Enter the name !: ");
    scanf("%s", name);
    if (strcmp(ready->name, name) == 0) {
        ready->state = 'b';
        p = ready;
        ready = ready->link;
    }
    else
    {
        while (p2)
        {
            if (strcmp(p2->name, name) == 0)
            {
                p2->state = 'b';
                p = p2;
                p1->link = p2->link;
                break;
            }
            p1 = p1->link;
            p2 = p2->link;
        }
    }
    if (!stop)
    {
        p->link = NULL;
        stop = p;
        stopend = p;
    }
    else {
        p->link = stopend->link;
        stopend->link = p;
        stopend = p;
    }

}

void wake()
{
    char name[10];
    PCB* p1 = stop, * p2 = stop->link;
    printf("Enter the name !: ");
    scanf("%s", name);
    if (stop == NOTHING) {
        printf("\n阻塞队列为空，请进行其他操作");
        return;
    }
    else {
        if (strcmp(stop->name, name) == 0) {
            p = stop;
            stop = stop->link;
        }
        else
        {
            while (p2) {
                if (strcmp(p2->name, name) == 0)
                    p = p2;
                p1->link = p2->link;
                break;
            }
            p1 = p1->link;
            p2 = p2->link;
        }

    }
    if (ready != NOTHING) {
        p->link = pfend->link;
        pfend->link = p;
        pfend = p;
    }
    else
    {
        p->link = NOTHING;
        ready = p;
        pfend = p;
    }
    p->state = 'w';

}

int main()
{
    char ch;
    input();
    while (ready != NOTHING || stop != NOTHING)
    {
        if (ready != NOTHING) {
            printf("\nThe execute name:%s\n", ready->name);
            ready->state = 'R';
            //check();
            running();
            //check();
        }
        else {
            check();
        }
        printf("\n按i键添加新进程...s键阻塞进程...w键唤醒进程\n按其他任意键继续运行...");
        fflush(stdin);
        ch = getchar();
        if (ch == 'i' || ch == 'I')
            input();
        if (ch == 's' || ch == 'S')
            block();
        if (ch == 'w' || ch == 'W')
            wake();
    }
    printf("\n\n 进程已经完成\n");
    getchar();
    return 0;
}

