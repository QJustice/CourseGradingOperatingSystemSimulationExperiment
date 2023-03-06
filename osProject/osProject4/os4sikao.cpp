#include <iostream>
#include"stdlib.h"
#include "string.h"
#include"stdio.h"
#include <iomanip>

using namespace std;

struct duan
{
	long capacity;//该段的长度
	long addr;//起始逻辑地址
	long realaddr;//该段实际起始地址
	bool inMemory;// 是否在内存
	long year;    // 该段年龄
};
struct duanbiao
{
	duan duans[10];
	char processname[20]; // 进程的名字
	int Isdiaoyong;     //是否被调用
	int num;            //段的数目
	long total;         //该进程的总大小
	long now_occupy;    //该进程的当前总占用量
};

#define Free 0 //空闲状态
#define Busy 1 //已用状态
#define OK 1    //完成
#define ERROR 0 //出错
typedef int Status;
int flag;

typedef struct freearea//定义一个空闲区说明表结构
{
	long size;   //分区大小
	long address; //分区地址
	int state;   //状态
}ElemType;

// 线性表的双向链表存储结构
typedef struct DuLNode
{
	ElemType data;
	struct DuLNode* prior; //前趋指针
	struct DuLNode* next;  //后继指针
}

DuLNode, * DuLinkList;
DuLinkList block_first; //头结点
DuLinkList block_last;  //尾结点

long neicun = 0;
int duanbiaonum = 0;   //段表的数目
long zhanyong = 0;    // 占用的内存
long baseaddr = 0;
int funsignal = 0;    // 内存分配算法标记
duanbiao duanbiaos[10];
Status First_fit(int);//首次适应算法
Status Best_fit(int); //最佳适应算法
Status Worst_fit(int); //最差适应算法
int find_duan_num(long);
Status free_biao(int); //内存回收
Status Initblock(int);//开创空间表
Status allocat_memory(int, int);
void year_add();

void show1(int i);
void show();
void showtest();


Status Initblock(int MAX_length)//开创带头结点的内存空间链表
{
	block_first = (DuLinkList)malloc(sizeof(DuLNode));
	block_last = (DuLinkList)malloc(sizeof(DuLNode));
	block_first->prior = NULL;
	block_first->next = block_last;
	block_last->prior = block_first;
	block_last->next = NULL;
	block_last->data.address = 0;
	block_last->data.size = MAX_length;
	block_last->data.state = Free;
	return OK;
}

Status allocat_memory(int res_size, int signal)
{
	Status(*pFunc)(int) = NULL;
	if (1 == signal)
		pFunc = First_fit;
	else if (2 == signal)
		pFunc = Worst_fit;
	else
		pFunc = Best_fit;
	return (*pFunc)(res_size);
}

void year_add()
{
	for (int i = 0; i < duanbiaonum; i++)
	{
		for (int j = 0; j < duanbiaos[i].num; j++)
		{
			if (duanbiaos[i].duans[j].inMemory)
			{
				duanbiaos[i].duans[j].year++;
				/*cout << "----------------------------\n";
				cout << duanbiaos[i].duans[j].year << endl;
				cout << "----------------------------\n";*/
			}
		}
	}
}

void diaodu()
{
	cout<<"您要调度的进程是：p";
	int n;
	cin>>n;
	if(n>duanbiaonum || n <= 0)
		cout<<"段表不存在!"<<endl;
	else
	{
		int num;
		show1(n - 1);
		cout << "要调入第几段？";
		cin >> num;
		num--;
		if (num + 1 > duanbiaos[n - 1].num || num < 0)
		{
			cout << "段号超出范围\n调度失败！" << endl;
		}
		else if (duanbiaos[n - 1].duans[num].inMemory)
		{
			cout << "操作错误，该段已经被调入到内存！" << endl;
		}
		else if (duanbiaos[n - 1].duans[num].capacity > neicun - zhanyong)
		{
			cout << "内存空间不足，调度不成功！" << endl;
		}
		else
		{
			cout << "正在进行第" << num + 1 << "段的地址转换，请稍后......" << endl;
			duanbiaos[n - 1].duans[num].realaddr = allocat_memory(duanbiaos[n - 1].duans[num].capacity, funsignal);
			duanbiaos[n - 1].duans[num].inMemory = true;
			duanbiaos[n - 1].now_occupy += duanbiaos[n - 1].duans[num].capacity;
			zhanyong += duanbiaos[n - 1].duans[num].capacity;
			cout << "第" << num + 1 << "段的地址转换成功!" << endl;
			year_add();
			duanbiaos[n - 1].Isdiaoyong = 1;
			cout << "调度后的结果是：" << endl;
			show1(n - 1);
			showtest();
		}
	}
}

void zhuanhuan()
{
     int n,i,j;
	cout<<"请输入进程：p";
	cin>>n;
    if(n>duanbiaonum || n <= 0)
		cout<<"进程不存在或没被调度!"<<endl;
	else 
	{
		
		  cout<<"请输入段号："<<endl;
		  cin>>i;
		  cout<<"输入偏移地址："<<endl;
		  cin>>j;
		  if(i>duanbiaos[n-1].num || i <= 0)
		  {
			  cout<<"段号超出范围"<<endl;
		  }
		  else if (!duanbiaos[n - 1].duans[i - 1].inMemory)
		  {
			  cout << "该段还未被调入内存" << endl;
		  }
		  else if(j>duanbiaos[n-1].duans[i-1].capacity || j <= 0)
		  {
			  cout<<"段内偏移地址超出范围"<<endl;
		  }
		  else
		  {
			cout<<"转换后的地址为"<<endl;
		    cout<</*待补全*/duanbiaos[n - 1].duans[i - 1].realaddr + j << endl;
		  }
	}
}

void apply()
{
	printf("请输入进程的名字：");
	cin>>duanbiaos[duanbiaonum].processname;
	printf("请输入该进程的段数：");
	cin>>duanbiaos[duanbiaonum].num;
	if(duanbiaos[duanbiaonum].num>10)
	{
		cout<<"段数太多，申请失败\n";
		return;
	}
	int paddr=0;
	for(int j=0;j<duanbiaos[duanbiaonum].num;j++)
	{
		cout<<"进程第"<<j+1<<"段的大小是：";
		cin>>duanbiaos[duanbiaonum].duans[j].capacity ;
		duanbiaos[duanbiaonum].duans[j].realaddr = -1;
		duanbiaos[duanbiaonum].duans[j].inMemory = false;
		duanbiaos[duanbiaonum].now_occupy = 0;
		duanbiaos[duanbiaonum].duans[j].year = 0;
		//////
		if (j)
			duanbiaos[duanbiaonum].duans[j].addr = duanbiaos[duanbiaonum].duans[j - 1].addr + duanbiaos[duanbiaonum].duans[j - 1].capacity;
		paddr += duanbiaos[duanbiaonum].duans[j].capacity;
		//待补全
		//////

	}
	duanbiaos[duanbiaonum].Isdiaoyong=0;
	duanbiaos[duanbiaonum].total=paddr;
	duanbiaonum++;
}

void show1(int i)
{
		cout<<"进程p"<<i+1<<":"<<endl;
		cout<<"名字:"<<duanbiaos[i].processname<<endl;
		cout<<"该进程的总大小:"<<duanbiaos[i].total<<endl;
		cout<<"该进程的当前内存总占用量:"<<duanbiaos[i].now_occupy<<endl;
		if(duanbiaos[i].Isdiaoyong==0)
			cout<<"进程状态：未调用"<<endl;
		else
			cout<<"进程状态：已调用"<<endl;
		cout<<"____________________________________________"<<endl;
		cout<<"段 号    始址    内存始址    大小"<<endl;
		for(int j=0;j<duanbiaos[i].num;j++)
		{
			cout<<" "<<j+1<<setw(11)<<duanbiaos[i].duans[j].addr;
			if(duanbiaos[i].Isdiaoyong==0)
				cout<<"            ";
			else
				cout<<setw(11)<<duanbiaos[i].duans[j].realaddr;
			cout<<setw(8)<<duanbiaos[i].duans[j].capacity<<endl ;
		}
		cout<<"____________________________________________"<<endl;
}

void show()
{
	for(int i=0;i<duanbiaonum;i++)
	show1(i);
}

void taotai()
{
	char c;
	while (1)
	{
		cout << "***********************************************" << endl;
		cout << "  1:自动淘汰(a)   2:手动淘汰(s)     5:退  出(e)" << endl;
		cout << "***********************************************" << endl;
		cout << "请选择服务种类(a/s/e):";
		cin >> c;
		if (c == 'e')
		{
			cout << "服务完毕！" << endl;
			break;
		}
		else if (c == 'a')
		{
			int old_year = -1;
			int duanbiao_num = -1;
			int duan_num = -1;
			for (int i = 0; i < duanbiaonum; i++)
			{
				for (int j = 0; j < duanbiaos[i].num; j++)
				{
					if (duanbiaos[i].duans[j].inMemory && duanbiaos[i].duans[j].year > old_year)
					{
						old_year = duanbiaos[i].duans[j].year;
						duanbiao_num = i;
						duan_num = j;
					}
				}
			}
			if (-1 == duanbiao_num || -1 == duan_num)
			{
				cout << "未找到合适的可淘汰段！" << endl;
			}
			else
			{
				int num = duan_num;
				int n = ++duanbiao_num;
				cout << "自动被淘汰进程为：";
				cout << "进程p" << n << ":" << endl;
				cout << "名字:" << duanbiaos[n - 1].processname << endl;
				cout << "正在进行第" << num + 1 << "段的淘汰工作，请稍后......" << endl;
				free_biao(find_duan_num(duanbiaos[n - 1].duans[num].realaddr) - 1);
				duanbiaos[n - 1].duans[num].realaddr = -1;
				duanbiaos[n - 1].duans[num].inMemory = false;
				duanbiaos[n - 1].duans[num].year = 0;
				zhanyong += duanbiaos[n - 1].duans[num].capacity;
				duanbiaos[n - 1].now_occupy -= duanbiaos[n - 1].duans[num].capacity;
				zhanyong -= duanbiaos[n - 1].duans[num].capacity;
				cout << "第" << num + 1 << "段淘汰成功!" << endl;

				bool all__not_in_Memory = true;
				for (int j = 0; j < duanbiaos[duanbiaonum].num; j++)
				{
					if (-1 != duanbiaos[duanbiaonum].duans[j].realaddr)
						all__not_in_Memory = false;
				}
				if (all__not_in_Memory)
					duanbiaos[n - 1].Isdiaoyong = 0;
				duanbiaos[n - 1].Isdiaoyong = 1;
				cout << "淘汰后的结果是：" << endl;
				show1(n - 1);
				showtest();
			}

		}
		else if (c == 's')
		{
			cout << "您要淘汰的进程是：p";
			int n;
			cin >> n;
			if (n > duanbiaonum || n <= 0)
				cout << "段表不存在!" << endl;
			else
			{
				int num;
				show1(n - 1);
				cout << "要淘汰第几段？";
				cin >> num;
				num--;
				if (num > duanbiaos[n - 1].num || num < 0)
				{
					cout << "段号超出范围\n淘汰失败！" << endl;
				}
				else if (!duanbiaos[n - 1].duans[num].inMemory)
				{
					cout << "操作错误，该段不在内存内！" << endl;
				}
				else
				{
					cout << "正在进行第" << num + 1 << "段的淘汰工作，请稍后......" << endl;
					free_biao(find_duan_num(duanbiaos[n - 1].duans[num].realaddr) - 1);
					duanbiaos[n - 1].duans[num].realaddr = -1;
					duanbiaos[n - 1].duans[num].inMemory = false;
					duanbiaos[n - 1].duans[num].year = 0;
					zhanyong += duanbiaos[n - 1].duans[num].capacity;
					duanbiaos[n - 1].now_occupy -= duanbiaos[n - 1].duans[num].capacity;
					zhanyong -= duanbiaos[n - 1].duans[num].capacity;
					cout << "第" << num + 1 << "段淘汰成功!" << endl;
					bool all__not_in_Memory = true;
					for (int j = 0; j < duanbiaos[duanbiaonum].num; j++)
					{
						if (-1 != duanbiaos[duanbiaonum].duans[j].realaddr)
							all__not_in_Memory = false;
					}
					if (all__not_in_Memory)
						duanbiaos[n - 1].Isdiaoyong = 0;
					duanbiaos[n - 1].Isdiaoyong = 1;
					cout << "淘汰后的结果是：" << endl;
					show1(n - 1);
					showtest();
				}
			}
		}
		
		else
			cout << "error!请重新输入!" << endl;
	}


	
}

void showtest()
{
	int flag = 0;
	cout << "\n主存分配情况:\n";
	cout << "++++++++++++++++++++++++++++++++++++++++++++++\n\n";
	DuLNode* p = block_first->next;
	cout << "分区号\t起始地址\t分区大小\t状态\n\n";
	while (p)
	{
		cout << "  " << flag++ << "\t";
		cout << "  " << p->data.address << "\t\t";
		cout << " " << p->data.size << "KB\t\t";
		if (p->data.state == Free) cout << "空闲\n\n";
		else cout << "已分配\n\n";
		p = p->next;
	}
	cout << "++++++++++++++++++++++++++++++++++++++++++++++\n\n";
}

//首次适应算法
Status First_fit(int request)
{
	//为申请作业开辟新空间且初始化
	DuLinkList temp = (DuLinkList)malloc(sizeof(DuLNode));
	temp->data.size = request;
	temp->data.state = Busy;

	DuLNode* p = block_first->next;
	while (p)
	{
		if (p->data.state == Free && p->data.size == request)
		{//有大小恰好合适的空闲块

			p->data.state = Busy;
			free(temp);
			return p->data.address;
			//待补全

		}
		if (p->data.state == Free && p->data.size > request)
		{//有空闲块能满足需求且有剩余

			temp->prior = p->prior;
			temp->next = p;
			p->prior->next = temp;
			p->prior = temp;
			temp->data.address = p->data.address;
			p->data.address += request;
			p->data.size -= request;
			//待补全    
			return temp->data.address;
		}
		p = p->next;
	}
	return ERROR;
}

//最佳适应算法
Status Best_fit(int request)
{
	DuLinkList temp = (DuLinkList)malloc(sizeof(DuLNode));
	temp->data.size = request;
	temp->data.state = Busy;
	DuLNode* p = block_first->next;
	DuLNode* q = NULL; //记录最佳插入位置

	while (p) //初始化最小空间和最佳位置
	{
		if (p->data.state == Free && (p->data.size >= request))
		{
			if (q == NULL)
			{
				q = p;
				//待补全
			}
			else if (q->data.size > p->data.size)
			{
				q = p;
				//待补全
			}
		}
		p = p->next;
	}

	if (q == NULL) return ERROR;//没有找到空闲块
	else if (q->data.size == request)
	{
		q->data.state = Busy;
		free(temp);
		return q->data.address;
		//待补全
	}
	else
	{
		temp->prior = q->prior;
		temp->next = q;
		q->prior->next = temp;
		q->prior = temp;
		temp->data.address = q->data.address;
		q->data.address += request;
		q->data.size -= request;

		return temp->data.address;
		//待补全
	}
	return OK;
}

//最差适应算法
Status Worst_fit(int request)
{
	int ch; //记录最大剩余空间
	DuLinkList temp = (DuLinkList)malloc(sizeof(DuLNode));
	temp->data.size = request;
	temp->data.state = Busy;
	DuLNode* p = block_first->next;
	DuLNode* q = NULL; //记录最佳插入位置

	while (p) //初始化最大空间和最佳位置
	{
		if (p->data.state == Free && (p->data.size >= request))
		{
			if (q == NULL)
			{
				q = p;
				//待补全
			}
			else if (q->data.size < p->data.size)
			{
				q = p;
				//待补全
			}
		}
		p = p->next;
	}

	if (q == NULL) return ERROR;//没有找到空闲块
	else if (q->data.size == request)
	{
		q->data.state = Busy;
		free(temp);
		return q->data.address;
		//待补全
	}
	else
	{
		temp->prior = q->prior;
		temp->next = q;
		q->prior->next = temp;
		q->prior = temp;
		temp->data.address = q->data.address;
		q->data.address += request;
		q->data.size -= request;
		return q->data.address;
		//待补全
	}
	return OK;
}

int find_duan_num(long _realaddress)
{
	DuLNode* p = block_first->next;
	int flag = 0;
	while (p)
	{
		flag++;
		if (p->data.address == _realaddress)
			return flag;
		p = p->next;
	}
}

//主存回收
Status free_biao(int flag)
{
	DuLNode* p = block_first;
	DuLNode* killNode = NULL;
	for (int i = 0; i <= flag; i++)
		if (p != NULL)
			p = p->next;
		else
			return ERROR;

	if (p->data.state == Free)
	{
		cout << "该块本为空闲\n";
		return ERROR;
	}

	if (p == block_first->next && p == block_last)
	{
		p->data.state = Free;
		return OK;
	}
	if (p != block_first->next && p != block_last)
	{
		if (p->prior->data.state == Free && p->next->data.state == Free)
		{
			p->prior->data.size = p->prior->data.size + p->data.size + p->next->data.size;
			if (p->next == block_last)
			{
				killNode = p->next;
				block_last = block_last->prior;
				p->next = NULL;
				free(killNode);
				killNode = p;
				block_last = block_last->prior;
				p->prior->next = NULL;
				free(killNode);
				return OK;
			}
			else
			{
				killNode = p->next;
				p->next->next->prior = p;
				p->next = p->next->next;
				free(killNode);
				killNode = p;
				p->next->prior = p->prior;
				p->prior->next = p->next;
				free(killNode);
				return OK;
			}
		}
	}
	// p == block_first->next && p->next->data.state == Busy || 
	// p == block_last && p->prior->data.state == Busy || 
	// p != block_first->next && p != block_last && p->prior->data.state == Busy && p->next->data.state == Busy
	if (p == block_first->next && p->next->data.state == Busy || p == block_last && p->prior->data.state == Busy || p != block_first->next && p != block_last && p->prior->data.state == Busy && p->next->data.state == Busy)
	{
		p->data.state = Free;
		return OK;
	}
	if (p != block_last && p->next->data.state == Free)
	{

		killNode = p->next;
		p->data.size += p->next->data.size;
		p->data.state = Free;
		if (p->next == block_last)
		{
			block_last = block_last->prior;
			p->next = NULL;
		}
		else
		{
			p->next->next->prior = p;
			p->next = p->next->next;
		}
		free(killNode);
		return OK;
	}
	if (p != block_first->next && p->prior->data.state == Free)
	{
		killNode = p;
		p->prior->data.size += p->data.size;
		if (p == block_last)
		{
			block_last = block_last->prior;
			p->prior->next = NULL;
		}
		else
		{
			p->next->prior = p->prior;
			p->prior->next = p->next;
		}

		free(killNode);
		return OK;
	}

	cout << "\n未知错误！";
	return ERROR;
}

int main(int argc, char* argv[])
{
	int ch;//算法选择标记
	cout << "请输入所使用的内存分配算法：\n";
	cout << "(1)首次适应算法\n(2)最差适应算法\n(3)最佳适应算法\n";

	cin >> ch;
	while (ch < 1 || ch>3)
	{
		cout << "输入错误，请重新输入所使用的内存分配算法：\n";
		cin >> ch;
	}
	funsignal = ch;
	char c;
	cout<<"请输入内存的大小：";
	cin>>neicun;
	Initblock(neicun); //开创空间表
	while(1)
	{
		cout << "******************************************************************************************************************" << endl;
		cout << "  1:申请进程(a)   2:显示进程(s)     3:调度进程(d)    4.地址转换(z)    5.淘汰段块(t)    6.内存状态(m)   7:退  出(e)" << endl;
		cout << "******************************************************************************************************************" << endl;
		cout<<"请选择服务种类(a/s/d/z/t/m/e):";
		cin>>c;
		if (c == 'e')
		{
			cout << "服务完毕！" << endl;
			break;
		}
		else if (c == 'a')
			apply();
		else if (c == 's')
			show();
		else if (c == 'd')
			diaodu();
		else if (c == 'z')
			zhuanhuan();
		else if (c == 't')
			taotai();
		else if (c == 'm')
			showtest();
		else
			cout<<"error!请重新输入!"<<endl;
	}
	return 0;
}
