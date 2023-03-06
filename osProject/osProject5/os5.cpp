#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
using namespace std;

const int maxn = 320;     //���и���
const int max = maxn + 20;//�����С
const int maxp = max / 10; //���ҳ��

int inst[max];//ָ������
int page[max];//ҳ��ַ��
int size;      //�ڴ������ɵ�ҳ��

bool in[maxp];  //��ҳ�Ƿ����ڴ�����Ч��
int pin[maxp];  //�������ڴ����ҳ

void welcome()
{
    printf("******************************************\n");
    printf("**            ����ϵͳģ��ʵ��\t\t**\n");
    printf("**             ҳʽ�洢����\t\t**\n");
    printf("******************************************\n\n");
}

void input_hint()
{
    printf("\n1--create new instruction sequence      2--set memory page number(4 to 32)\n");
    printf("3--solve by FIFO algorithm              4--solve by LRU algorithm\n");
    printf("5--solve by OPT algorithm               0--exit\n");
    printf("*********Please input Your choice:  ");
}

/*ͨ�����������һ��ָ�����У���320��ָ��*/
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
/*��ָ�����б任��Ϊҳ��ַ��*/
void turn_page_address()
{
    for (int i = 0; i < maxn; i++) {
        page[i] = inst[i] / 10;
    }
}

void FIFO_solve()
{
    memset(in, false, sizeof(in));
    int fault_n = 0;//ȱҳ��
    int ptr, i;

    //Ԥ��ҳ�����ռ�    
    ptr = 0; //��һ��Ҫ�ŵ�λ��
    for (i = 0; i < maxn && ptr < size; i++)
        if (!in[page[i]])
        {
            pin[ptr++] = page[i];
            printf("�����ڴ�%d\n", page[i]);
            in[page[i]] = true;
            fault_n++;
        }
    printf("\n\n******�ڴ�ҳ��������ҳ�����㷨******\n\n");
    //����ִ��ʣ�µ�ָ��
    ptr = 0;//���������Ƚ�����λ�ã�����һ��Ҫ���滻��λ��
    for (; i < maxn; i++)
        if (!in[page[i]])
        {
            ptr %= size;
            in[pin[ptr]] = false;
            printf("�����ڴ�%d\n", pin[ptr]);
            pin[ptr++] = page[i];
            printf("�����ڴ�%d\n", page[i]);
            in[page[i]] = true;
            fault_n++;
            /*--------------*/
            //�벹ȫ�ò���
            /*--------------*/
        }

    printf("\nBy FIFO algorithm, the fault-page number is:  %d\n", fault_n);
    printf("                   the hit ratio is :  %.2lf\n", (1 - (fault_n + 0.0) / 320.0));
}

void LRU_solve()
{
    int ltu[maxp]; //last_time_use
    int ti = 1;    //ģ��ʱ��
    int fault_n = 0;
    memset(ltu, 0, sizeof(ltu));
    memset(in, false, sizeof(in));
    memset(pin, -1, sizeof(pin));

    int min, ptr, i, j;
    for (i = 0; i < maxn; i++)
    {
        if (!in[page[i]])
        {
            //Ѱ��lru
            min = 1000000; ptr = 0;
            for (j = 0; j < size; j++)
            {
                if (ltu[j] < min)
                {
                    ptr = j;
                    min = ltu[ptr];
                    /*--------------*/
                    //�벹ȫ�ò���
                    /*--------------*/
                }
            }
            //�滻��д�� 
            if (pin[ptr] != -1) {
                in[pin[ptr]] = false;
                printf("�����ڴ�%d\n", pin[ptr]);
            }
            pin[ptr] = page[i];
            printf("�����ڴ�%d\n", page[i]);
            in[page[i]] = true;
            /*--------------*/
            //�벹ȫ�ò���
            /*--------------*/
            fault_n++;
            ltu[ptr] = ti++;
        }
        else//�Ѿ����ڴ�����ֻ��������ʹ��ʱ��    
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

    //Ԥ��ҳ����
    int ptr = 0;
    for (i = 0; i < maxn && fault_n < size; i++)
    {
        if (!in[page[i]])
        {
            in[page[i]] = true;
            pin[ptr] = page[i];
            printf("�����ڴ�%d\n", page[i]);
            fault_n++;
            ptr++;
        }
    }
    printf("\n\n******�ڴ�ҳ��������ҳ�����㷨******\n\n");
    //��ʼ��ntu����
    ptr = 0;
    for (j = i; j < maxn && ptr < 32; j++)
    {
        if (ntu[page[j]] == -1)
        {
            ntu[page[j]] = j;
            ptr++;
            /*--------------*/
            //�벹ȫ�ò���
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
                    //�벹ȫ�ò���
                    /*--------------*/
                }
            }
            in[pin[ptr]] = false;
            printf("�����ڴ�%d\n", pin[ptr]);
            in[page[i]] = true;
            pin[ptr] = page[i];
            printf("�����ڴ�%d\n", page[i]);
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
            printf("ָ�� ҳ��ַ\tָ�� ҳ��ַ\tָ�� ҳ��ַ\tָ�� ҳ��ַ\n");
            for (int i = 0; i < max - 20; i++)
            {
                printf("%4d %4d\t", inst[i], page[i]);
                if ((i + 1) % 4 == 0)
                    printf("��%d��\n", (i + 1) / 4);
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
