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
};
struct duanbiao
{
	duan duans[10];
	char processname[20]; // 进程的名字
	int Isdiaoyong;     //是否被调用
	int num;            //段的数目
	long total;         //该进程的总占用量
};

long neicun=0;
int duanbiaonum=0;   //段表的数目
long zhanyong=0;    // 占用的内存
long baseaddr=0;
duanbiao duanbiaos[10];

void show1(int i);
void show();


void diaodu()
{
	cout<<"您要调度的进程是：p";
	int n;
	cin>>n;
	if(n>duanbiaonum)
		cout<<"段表不存在!"<<endl;
	else if(duanbiaos[n-1].Isdiaoyong==1)
		cout<<"操作错误，该进程已经被调入到内存！"<<endl;
	else if(duanbiaos[n-1].total>neicun-zhanyong)
		cout<<"内存空间不足，调度不成功！"<<endl;
	else
	{

		cout<<"下面对进程"<<duanbiaos[n-1].processname<<"进行地址转化："<<endl;
		for (int j = 0; j < duanbiaos[n - 1].num; j++)
		{
			cout << "正在进行第" << j + 1 << "段的地址转换，请稍后......" << endl;
			duanbiaos[n - 1].duans[j].realaddr = duanbiaos[n - 1].duans[j].addr + zhanyong;

			cout << "第" << j + 1 << "段的地址转换成功!" << endl;
		}

		zhanyong += duanbiaos[n - 1].total;
		duanbiaos[n-1].Isdiaoyong =1;
		cout<<"调度后的结果是："<<endl;
		show1(n-1);
	}
}

void zhuanhuan()
{
    int n,i,j;
	cout<<"请输入进程：p";
	cin>>n;
    if(n>duanbiaonum)
		cout<<"进程不存在或没被调度!"<<endl;
	else if(duanbiaos[n-1].Isdiaoyong==1)
		  {
		  cout<<"请输入段号："<<endl;
		  cin>>i;
		  cout<<"输入偏移地址："<<endl;
		  cin>>j;
		  if(i>duanbiaos[n-1].num)
		  {
			  cout<<"段号超出范围"<<endl;
		  }
		  else if(j>duanbiaos[n-1].duans[i-1].capacity)
		  {
			  cout<<"段内偏移地址超出范围"<<endl;
		  }
		  else
		  {

			cout<<"转换后的地址为"<<endl;
		    cout<</*待补全*/duanbiaos[n - 1].duans[i - 1].realaddr + j << endl;
		  }
	}
	else cout<<"该进程没有调入内存"<<endl;
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
		cout<<"该进程所占用的存储空间:"<<duanbiaos[i].total<<endl;
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

int main(int argc, char* argv[])
{
	char c;
	cout<<"请输入内存的大小：";
	cin>>neicun;
	cout<<"*******************************************************************************"<<endl;
	cout<<"  1:申请进程(a)   2:显示进程(s)     3:调度进程(d)    4.地址转换(z)   5:退  出(e)"<<endl;
	cout<<"********************************************************************************"<<endl;
	while(1)
	{
	cout<<"请选择服务种类(a/s/d/z/e):";
		cin>>c;
		if(c=='e')
		{
			cout<<"服务完毕！"<<endl;
			break;
		}
		else if(c=='a')
			apply();
		else if(c=='s')
			show();
		else if(c=='d')
			diaodu();
		else if(c=='z')
			zhuanhuan();
		else
			cout<<"error!请重新输入!"<<endl;
	}
	return 0;
}
