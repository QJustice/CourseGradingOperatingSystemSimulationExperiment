#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

#define getpch(type) (type*)malloc(sizeof(type))
#define NOTHING 0
void check();

int signal=0;

typedef struct pcb{//进程管理块
	char name[10];//进程名字
	char state;		//进程状态
	int ntime;		//进程需要运行的时间
	int rtime;		//进程已经运行的时间
	int oncerun;
	struct pcb *link;
}PCB;

PCB	*ready = NOTHING, *pfend = NOTHING,*p =NOTHING;		//就绪队列，进程插入位置的变量

int geti()	//使用户仅能输入整数
{
	char ch;
	int i = 0;
	fflush(stdin);
	ch = getchar();
	
	while(ch == '\n'){
		//printf("\tf输入不能为空..请重新输入\n");
		fflush(stdin);
		ch = getchar();
	}

	while(ch != '\n'){
		if(ch > '9' || ch < '0'){
			//printf("\t输入有误!!输入只能为正整数，请重新输入...\n");
			fflush(stdin);
			i = 0;
			ch = getchar();
		}else{
			i =  i*10 + (ch - '0');
			ch = getchar();
		}
	}
	return i;
}

void SJF()//Shortest Job First
{
	if(!ready ){
		ready = p;
		pfend = p;
		pfend->link = NULL;
		//待补全
	}
	else{
		PCB *tp=ready,*tempp = NULL;
		if(ready->oncerun == 1)
			tp = tp->link;
		if (ready && !tp)
		{
			ready->link = p;
			p->link = NULL;
			return;
		}
		while(p->ntime >= tp->ntime){
			tempp = tp;			
			tp = tp->link;
			if(tp==NOTHING)
				break;
		}
		if(tp == ready){
			if(ready->oncerun == 0){
				p->link = tp;
				ready = p;
				//待补全
			}
			else{
				tp = tp->link;
				ready->link = p;
				p->link = tp;
				//待补全
			}
		}
		else if(tp == NOTHING){
			tempp->link	= p;
			p->link = NULL;

			//待补全
		}
		else{
			if (!tempp)
			{
				tempp = ready->link;
				ready->link = p;
				p->link = tempp;
			} 
			else
			{
				tempp->link = p;
				p->link = tp;
			}
			//待补全
		}
	}
}

void input()/*建立进程控制块函数*/
{
	int i,num;
	printf("\n请输入进程的个数?\n");
	num = geti();
	for(i=0; i < num; i++)
	{
		printf("\n进程号No.%d:\n",i+1);
		p=getpch(PCB);
		printf("\n输入进程名:");
		scanf("%s",p->name);
		printf("\n输入进程运行时间:");
		p ->ntime = geti();
		printf("\n");
		p->rtime=0;
		p->state='w';
		p->link=NOTHING;
		p->oncerun=0;
		SJF();
	}
}

void disp(PCB *pr)/*建立进程现实函数，用于显示当前进程*/
{
	printf("\nname\t state\t ntime\t rtime\t \n");
	printf("|%s\t",pr->name);
	printf(" |%c\t",pr->state);
	printf(" |%d\t",pr->ntime);
	printf(" |%d\t",pr->rtime);
	printf("\n");
}

void check()/*建立进程查看函数*/
{
	PCB *pr;
	printf("\n ****当前正在运行的进程是:%s",ready->name);/*显示当前运行的进程*/
	disp(ready);
	pr= ready ->link;
	printf("\n****当前就绪队列状态为:\n");/*显示就绪队列状态*/
	while(pr!=NOTHING)
	{
		disp(pr);
		pr=pr->link;
	}
}

void destroy()/*建立进程撤销函数(进程运行结束，撤销进程)*/
{
	printf("\n进程[%s]已完成.\n",ready->name);
	p = ready;
	ready = ready->link;
	free(p);
}

void running()/*建立进程就绪函数(进程运行时间到，置就绪状态)*/
{
	ready->oncerun = 1;
	(ready -> rtime)++;
	check();
	if(ready->rtime >= ready->ntime){
		destroy();
		return;
	}
}

void main()
{
	char ch;
	input();
	while(ready != NOTHING)
	{
		printf("\nThe execute name:%s\n",ready ->name);
		ready ->state = 'R';
		//check();
		running();
		printf("\n按i键添加新进程....按其他任意键继续运行...");
		fflush(stdin);
		ch = getchar();
		if (ch == 'i'|| ch=='I')
			input();
	}
	printf("\n\n 进程已经完成\n");
	getchar();
}
