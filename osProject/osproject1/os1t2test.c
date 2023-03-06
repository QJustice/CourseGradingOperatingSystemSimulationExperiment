#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

#define getpch(type) (type*)malloc(sizeof(type))
#define NOTHING 0

typedef struct pcb {//���̹����
    char name[10];//��������
    char state;     //����״̬
    int ntime;      //������Ҫ���е�ʱ��
    int rtime;      //�����Ѿ����е�ʱ��
    struct pcb* link;
}PCB;

PCB* ready = NOTHING, * pfend = NOTHING, * p = NOTHING, * stop = NOTHING, * stopend = NOTHING;      //�������У����̲���λ�õı���

int geti()  //ʹ�û�������������
{
    char ch;
    int i = 0;
    fflush(stdin);
    ch = getchar();

    while (ch == '\n') {
        //printf("\tf���벻��Ϊ��..����������\n");
        fflush(stdin);
        ch = getchar();
    }

    while (ch != '\n') {
        if (ch > '9' || ch < '0') {
            printf("\t��������!!����ֻ��Ϊ������������������...\n");
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

void fcfs()//�������
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

void input()/*�������̿��ƿ麯��*/
{
    int i, num;
    printf("\n��������̵ĸ���?");
    num = geti();
    for (i = 0; i < num; i++)
    {
        printf("\n���̺�No.%d:\n", i + 1);
        p = getpch(PCB);
        printf("\n���������:");
        scanf("%s", p->name);
        printf("\n�����������ʱ��:");
        p->ntime = geti();
        printf("\n");
        p->rtime = 0;
        p->state = 'w';
        p->link = NOTHING;
        fcfs();
    }
}

void disp(PCB* pr)/*����������ʵ������������ʾ��ǰ����*/
{
    printf("\nname\t state\t ntime\t rtime\t \n");
    printf("|%s\t", pr->name);
    printf(" |%c\t", pr->state);
    printf(" |%d\t", pr->ntime);
    printf(" |%d\t", pr->rtime);
    printf("\n");
}

void check()/*�������̲鿴����*/
{
    PCB* pr;
    if (ready != NOTHING) {
        printf("\n ****��ǰ�������еĽ�����:%s", ready->name);/*��ʾ��ǰ���еĽ���*/
        disp(ready);
        pr = ready->link;
        printf("\n****��ǰ��������״̬Ϊ:\n");/*��ʾ��������״̬*/
        while (pr != NOTHING)
        {
            disp(pr);
            pr = pr->link;
        }
    }
    printf("\n****��ǰ����������״̬Ϊ:\n");
    pr = stop;
    while (pr != NOTHING) {
        disp(pr);
        pr = pr->link;
    }
}

void destroy()/*�������̳�������(�������н�������������)*/
{
    printf("\n����[%s]�����.\n", ready->name);
    p = ready;
    ready = ready->link;
    free(p);
}

void running()/*�������̾�������(��������ʱ�䵽���þ���״̬)*/
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
        printf("\n��������Ϊ�գ��������������");
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
        printf("\n��i������½���...s����������...w�����ѽ���\n�������������������...");
        fflush(stdin);
        ch = getchar();
        if (ch == 'i' || ch == 'I')
            input();
        if (ch == 's' || ch == 'S')
            block();
        if (ch == 'w' || ch == 'W')
            wake();
    }
    printf("\n\n �����Ѿ����\n");
    getchar();
    return 0;
}

