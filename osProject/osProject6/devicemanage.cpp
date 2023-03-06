#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<stdlib.h>
#include<string.h>

using namespace std;
//数据结构
typedef struct node
{
	char name[10];
	char devname[10];
	struct node *next;
}PCB;//进程
PCB * run;//执行队列
//
typedef struct Node
{
	char identify[10];//标识
	int state;//设备状态
	PCB *blocking;//阻塞队列指针
}CHCT;//通道控制表
CHCT * CH1, * CH2;//设置两个通道为ch1,ch2
//
typedef struct NOde
{
	char identify[10];//标识
	int state;//设备状态
	CHCT * chct;//CHCT
	PCB *blocking;//阻塞队列指针
	struct NOde *next;
}COCT;//控制器控制表
COCT * cohead;
typedef struct NODe
{
	char identify[10];//标识
	char type;//设备类型
	int state;//设备状态
	int times;//重复执行次数
	PCB *blocking;//阻塞队列指针
	COCT * coct;//COCT
}DCT;//设备控制表
typedef struct NODE
{
	char type;//类别
	char identify[10];//标识
	DCT * dct;//DCT设备控制表
	//驱动入口地址
	struct NODE *next;
}SDT;
//系统设备表
SDT * head;
SDT * checkdel;
void Init()
{
	PCB * ch1block,* ch2block,* co1block,* co2block,* co3block,* d1block,* d2block,* d3block,* d4block;//进程控制块
	COCT * CO1,* CO2,* CO3;
	DCT *d1,*d2,*d3,*d4;
	SDT *s1,*s2,*s3,*s4;
	//=================================================
	run=(PCB *)malloc(sizeof(PCB));
	run->next=NULL;
	CH1=(CHCT *)malloc(sizeof(CHCT));
	CH2=(CHCT *)malloc(sizeof(CHCT));
	strcpy(CH1->identify,"ch1");
	strcpy(CH2->identify,"ch2");
	CH1->state=0;
	CH2->state=0;
	ch1block=(PCB *)malloc(sizeof(PCB));
	ch1block->next=NULL;
	ch2block=(PCB *)malloc(sizeof(PCB));
	ch2block->next=NULL;
	CH1->blocking=ch1block;
	CH2->blocking=ch2block;
	cohead=(COCT *)malloc(sizeof(COCT));
	cohead->next=NULL;
	CO1=(COCT *)malloc(sizeof(COCT));
	cohead->next=CO1;
	CO1->next=NULL;
	CO2=(COCT *)malloc(sizeof(COCT));
	CO1->next=CO2;
	CO2->next=NULL;
	CO3=(COCT *)malloc(sizeof(COCT));
	CO2->next=CO3;
	CO3->next=NULL;
	CO1->state=0;
	CO2->state=0;
	CO3->state=0;
	co1block=(PCB *)malloc(sizeof(PCB));
	co1block->next=NULL;
	co2block=(PCB *)malloc(sizeof(PCB));
	co2block->next=NULL;
	co3block=(PCB *)malloc(sizeof(PCB));
	co3block->next=NULL;
	strcpy(CO1->identify,"co1");
	strcpy(CO2->identify,"co2");
	strcpy(CO3->identify,"co3");
	CO1->chct=CH1;
	CO2->chct=CH1;
	CO3->chct=CH2;
	CO1->blocking=co1block;
	CO2->blocking=co2block;
	CO3->blocking=co3block;
	//=====================================================
	d1block=(PCB *)malloc(sizeof(PCB));
	d2block=(PCB *)malloc(sizeof(PCB));
	d3block=(PCB *)malloc(sizeof(PCB));
	d4block=(PCB *)malloc(sizeof(PCB));
	d1block->next=NULL;
	d2block->next=NULL;
	d3block->next=NULL;
	d4block->next=NULL;
	d1=(DCT *)malloc(sizeof(DCT));
	strcpy(d1->identify,"P");//p表示打印机设备
	d2=(DCT *)malloc(sizeof(DCT));
	strcpy(d2->identify,"T");//t表示显示器设备
	d3=(DCT *)malloc(sizeof(DCT));
	strcpy(d3->identify,"K");//k表示键盘设备
	d4=(DCT *)malloc(sizeof(DCT));
	strcpy(d4->identify,"M");//m表示鼠标设备
	d1->coct=CO1;      //控制器
	d2->coct=CO2;   
	d3->coct=CO3;
	d4->coct=CO3;
	d1->state=0;//状态
	d2->state=0;
	d3->state=0;
	d4->state=0;
	d1->type='o';//o表示输出设备output
	d2->type='o';
	d3->type='i';//i表示输入设备
	d4->type='i';
	d1->blocking=d1block;
	d2->blocking=d2block;
	d3->blocking=d3block;
	d4->blocking=d4block;
	//================================
	head=(SDT *)malloc(sizeof(SDT));
	head->next=NULL;
	s1=(SDT *)malloc(sizeof(SDT));
	s2=(SDT *)malloc(sizeof(SDT));
	s3=(SDT *)malloc(sizeof(SDT));
	s4=(SDT *)malloc(sizeof(SDT));
	head->next=s1;
	s1->next=s2;
	s2->next=s3;
	s3->next=s4;
	s4->next=NULL;
	s1->dct=d1;
	s2->dct=d2;
	s3->dct=d3;
	s4->dct=d4;
	strcpy(s1->identify,"P");
	strcpy(s2->identify,"T");
	strcpy(s3->identify,"K");
	strcpy(s4->identify,"M");
	s1->type='o';
	s2->type='o';
	s3->type='i';
	s4->type='i';
	//test
	SDT *temp=head->next;
}
//============================================
//添加设备时候，添加新控制器，把控制器加到最后
void  Addcotrol (COCT *temp)//添加控制器函数
{
	COCT *cotemp=cohead;
	while(cotemp->next!=NULL)
	{
		cotemp=cotemp->next;
	}
	cotemp->next=temp;
	temp->next=NULL;
}
//查看所有控制器，选择所要连接的控制器
void Showallco()
{
	COCT *temp=cohead->next;
	while(temp!=NULL)
	{
		cout<<temp->identify<<"  ";
		temp=temp->next;
	}
	cout<<endl;
}
//查找要连接的控制器
COCT * Findco(char a[])
{
	COCT * temp=cohead->next;
	while(temp!=NULL)
	{
		if(!strcmp(temp->identify,a))
		{
			return temp;
		}
		temp=temp->next;
	}
	return temp;
}
//删除设备时候，判断是不是同时删除控制器,等于1删，0不删
int sf_deleteco(char a[],char b[])
{
	SDT *temp;
	temp=head->next;
	while(temp!=NULL)
	{
		if((strcmp(temp->identify,a))&&((!strcmp(temp->dct->coct->identify,b))))
		{
			return 0;
		}
		temp=temp->next;
	}
	return 1;
}
//删除设备的时候同时删除控制器
void Deletecotrol(COCT *te)
{
	COCT * temp=cohead;
	while(temp->next!=te)
	{
		temp=temp->next;
	}
	temp->next=te->next;
	delete(te);
}
//添加设备，查找设备是不是已经存在
int sf_exist(char a[])
{
	SDT *temp;
	if(head->next==NULL)
	{
		return 0;
	}
	else
	{
		temp=head->next;
		while(temp!=NULL)
		{
			if(!strcmp(a,temp->identify))
			{
				checkdel=temp;
				return 1;
			}
			temp=temp->next;
		}
		return 0;
	}
}
//申请设备时候，如果忙，将设备挂到等待队列
void Addwaitpcb(PCB * p1,PCB * p2)
{
	PCB *temp=p1;
	while(temp->next!=NULL)
	{
		temp=temp->next;
	}
	temp->next=p2;
	p2->next=NULL;
}
//回收设备时候，对PCB的操作
void Deletepcb(char a[])
{
	PCB * temp2=run->next,* temp=run;
	while(temp2!=NULL)
	{
		if(!strcmp(temp2->devname,a))
		{
			temp->next=temp2->next;
			delete(temp2);
			break;
		}
		temp=temp2;
		temp2=temp->next;
	}
}
//判断等待队列是不是空
int sf_pcbnull(PCB *temp)
{
	if(temp->next==NULL)
	{
		return 0;
	}
	else return 1;
}
//查看所有设备状态
void  Showmenu()
{
	PCB * ptemp=run->next;
	SDT * temp=head->next;
	DCT * dtemp;
	COCT * cotemp;
	CHCT * chtemp;
	cout<<"SDT"<<"    i/o"<<"      COCT"<<"            CHCT"<<endl;
	while(temp!=NULL)
	{
		dtemp=temp->dct;
		cotemp=dtemp->coct;
		chtemp=cotemp->chct;
		cout<<dtemp->identify<<"["<<dtemp->state<<"]"<<"\t"<<dtemp->type<<"\t"<<cotemp->identify<<"["<<cotemp->state<<"]"<<"     \t"<<chtemp->identify<<"["<<chtemp->state<<"]"<<endl;
		temp=temp->next;
	}
	while(ptemp!=NULL)
	{
		cout<<"进程"<<ptemp->name<<"申请了设备"<<ptemp->devname<<endl;
		ptemp=ptemp->next;
	}
	cout<<endl;
}
//设备独立性时候查找要查找的类型是不是存在
int sf_typeexist(char ch)
{
	SDT *temp;
	if(head->next==NULL)
	{
		return 0;
	}
	else
	{
		temp=head->next;
		while(temp!=NULL)
		{
			if(temp->type==ch)
			{
				return 1;
			}
			else temp=temp->next;
		}
	}
	return 0;
}
//增加设备
void Adddevice()
{
	cout<<"增加设备"<<endl;//写入设备名称
	SDT * stemp,* s2temp;
	COCT *cotemp;
	DCT *temp;
	char temptype;
	PCB *tempblock,* cotempblock;
	char choice;
	char eqary[10],coary[10];
	cin>>eqary;
	//此处查找SDT，看是否所要添加的设备已经存在
	if(sf_exist(eqary)==1)
	{//设备已经存在
		cout<<"设备已存在"<<endl;
	}
	else
	{//设备不存在
		tempblock=(PCB *)malloc(sizeof(PCB));
		tempblock->next=NULL;
		temp=(DCT *)malloc(sizeof(DCT));
		strcpy(temp->identify,eqary);
		temp->blocking=tempblock;
		temp->state=0;
		cout<<"输入设备类型"<<endl;
		cin>>temptype;
		temp->type=temptype;
		stemp=(SDT *)malloc(sizeof(SDT));
		stemp->next=NULL;
		stemp->dct=temp;
		stemp->type=temptype;
		strcpy(stemp->identify,eqary);
		s2temp=head;
		while(s2temp->next!=NULL)
		{
			s2temp=s2temp->next;
		}
		s2temp->next=stemp;
		stemp->next=NULL;
		cout<<"是否添加控制器?(y/n)"<<endl;
		cin>>choice;
		if((choice=='y')||(choice=='Y'))
		{	//添加新控制器
			cout<<"输入控制器名称："<<endl;
			cin>>coary;
			cotempblock=(PCB *)malloc(sizeof(PCB));
			cotempblock->next=NULL;
			cotemp=(COCT *)malloc(sizeof(COCT));
			cotemp->next=NULL;	
			strcpy(cotemp->identify,coary);
			cotemp->state=0;
			cotemp->blocking=cotempblock;
			Addcotrol(cotemp);
			temp->coct=cotemp;
			cout<<"请选择连接的通道:1/2"<<endl;//因为只定义了两个通道，选1就是ch1，选2就是ch2
			int i;
			cin>>i;
			if(i==1)
			{
				cotemp->chct=CH1;
			}
			else
			{
				cotemp->chct=CH2;
			}
		}
		else
		{//不添加控制器
			Showallco();
			cout<<"输入连接控制器的名称:"<<endl;
			cin>>coary;
			cotemp=Findco(coary);
			temp->coct=cotemp;
		}
		cout<<"设备"<<eqary<<"添加成功!"<<endl;
	}
}
//删除设备
void Deletedevice()
{//删除设备的时候同时删除pcb   Deletepcb
	cout<<"删除设备"<<endl;
	COCT * temp;
	SDT * stemp;
	char chary[10];
	char tempary[10];
	cin>>chary;
	if(sf_exist(chary)==0)
	{
		cout<<"删除设备不存在!"<<endl;
	}
	else if(checkdel->dct->state==1)
	{
		cout<<"设备正在使用无法删除"<<endl;
	}
	else
	{//sf_deleteco();
		Deletepcb(chary);
		strcpy(tempary,checkdel->dct->coct->identify);
		if(sf_deleteco(chary,tempary))
		{//删除控制器
			temp=checkdel->dct->coct;
			Deletecotrol(temp);
		}
		stemp=head;
		while(stemp->next!=checkdel)
		{
			stemp=stemp->next;
		}
		stemp->next=checkdel->next;
		delete(checkdel);
		cout<<"设备"<<chary<<"删除成功!"<<endl;
	}
}
//申请设备
void Applydevice()
{
	cout<<"申请设备"<<endl;
	PCB *ptemp;
	DCT *dtemp;
	COCT *cotemp;
	CHCT *chtemp;
	char pname[10],eqname[10];
	cout<<"输入申请设备的进程：";
	cin>>pname;
	cout<<"输入申请的设备名称：";
	cin>>eqname;
	if(sf_exist(eqname)==0)
	{
		cout<<"设备不存在!"<<endl;
	}
	else
	{//checkdel(对应要申请设备的指针)
		ptemp=(PCB *)malloc(sizeof(PCB));
		strcpy(ptemp->name,pname);
		ptemp->next=NULL;
		dtemp=checkdel->dct;
		//====================此处是否要考虑状态的改变========================
		if(dtemp->state==1)
		{//设备忙
			Addwaitpcb(dtemp->blocking,ptemp);
			cout<<"进程"<<ptemp->name<<"被挂在设备"<<dtemp->identify<<"的等待队列上!"<<endl;
		}
		else 
		{//设备不忙
			cotemp=dtemp->coct;
			if(cotemp->state==1)
			{//控制器忙
				Addwaitpcb(cotemp->blocking,ptemp);
				cout<<"进程"<<ptemp->name<<"被挂在控制器"<<cotemp->identify<<"的等待队列上!"<<endl;
			}
			else
			{//控制器不忙
				chtemp=cotemp->chct;
				if(chtemp->state==1)
				{//通道忙
					Addwaitpcb(chtemp->blocking,ptemp);
					cout<<"进程"<<ptemp->name<<"被挂在通道"<<chtemp->identify<<"的等待队列上!"<<endl;
				}
				else
				{//通道不忙
					Addwaitpcb(run,ptemp);
					dtemp->state=1;
					cotemp->state=1;
					chtemp->state=1;
					strcpy(ptemp->devname,eqname);
					cout<<"进程"<<pname<<"申请设备"<<eqname<<"成功!"<<endl;
				}
			}
		}
	}
}
//回收设备
void Recycledevice()
{
	cout<<"回收设备"<<endl;
//	char ch;
	char eqname[10];
	DCT *dtemp;
	COCT *cotemp;
	CHCT *chtemp;
	PCB *pctemp1,*pctemp2;
	cin>>eqname;
	if(sf_exist(eqname)==0)
	{//设备不存在
		cout<<"要回收的设备不存在!"<<endl;
	}
	else if(checkdel->dct->state==0)
	{//设备存在，但是不需要回收
		cout<<"设备处于闲状态，不需要回收!"<<endl;
	}
	else
	{//需要回收
		Deletepcb(eqname);//
		dtemp=checkdel->dct;
		cotemp=dtemp->coct;
		chtemp=cotemp->chct;
		dtemp->state=0;
		cotemp->state=0;
		chtemp->state=0;
		if(sf_pcbnull(chtemp->blocking))
		{//如果通道等待队列不空
			pctemp1=chtemp->blocking;
			pctemp2=pctemp1->next;
			pctemp1->next=pctemp2->next;
			pctemp2->next=NULL;
			strcpy(pctemp2->devname,eqname);
			Addwaitpcb(run,pctemp2);
			dtemp->state=1;
			cotemp->state=1;
			chtemp->state=1;
		}
		else
		{
			if(sf_pcbnull(cotemp->blocking))
			{//如果控制器的等待队列不空
				pctemp1=cotemp->blocking;
				pctemp2=pctemp1->next;
				pctemp1->next=pctemp2->next;
				pctemp2->next=NULL;
				strcpy(pctemp2->devname,eqname);
				Addwaitpcb(run,pctemp2);
				dtemp->state=1;
				cotemp->state=1;
				chtemp->state=1;
			}
			else
			{
				if(sf_pcbnull(dtemp->blocking))
				{//如果设备的等待队列不空
					pctemp1=dtemp->blocking;
					pctemp2=pctemp1->next;
					pctemp1->next=pctemp2->next;
					pctemp2->next=NULL;
					strcpy(pctemp2->devname,eqname);
					Addwaitpcb(run,pctemp2);
					dtemp->state=1;
					cotemp->state=1;
					chtemp->state=1;
				}
			}
		}
		cout<<"设备"<<eqname<<"回收成功!"<<endl;
	}
}
//设备独立性=======
void Independence()
{
	cout<<"设备独立性--申请设备:"<<endl;
	//cout<<"申请设备"<<endl;
	char type;
	char pname[10];
	SDT * temp,* temp2 = NULL;
	PCB *ptemp;
	DCT *dtemp;
	COCT *cotemp;
	CHCT *chtemp;
	cin>>pname>>type;
	if((type=='o')||(type=='i'))
	{//=========还得考虑要申请的类型是不是存在============================
		if(sf_typeexist(type)==0)
		{
			cout<<"要申请的该类设备不存在!"<<endl;
		}
		else
		{
			temp=head->next;
			while(temp!=NULL)
			{
				if((temp->type==type)&&(temp->dct->state==0))
				{//当设备类型相同，并且设备空闲
					temp2=temp;
					break;
				}
				else if((temp->type==type))
				{
					temp2=temp;
				}
				temp=temp->next;
			}
			sf_exist(temp2->identify);//=================================================
			ptemp=(PCB *)malloc(sizeof(PCB));
			strcpy(ptemp->name,pname);
			ptemp->next=NULL;
			dtemp=checkdel->dct;//要用了这个函数才可以==========================================================================
			if(temp2->dct->state==0)
			{//当设备不忙时候
				cotemp=dtemp->coct;
				if(cotemp->state==1)
				{//控制器忙
					Addwaitpcb(cotemp->blocking,ptemp);
					cout<<"进程"<<ptemp->name<<"被挂在控制器"<<cotemp->identify<<"的等待队列上!"<<endl;
				}
				else
				{//控制器不忙
					chtemp=cotemp->chct;
					if(chtemp->state==1)
					{//通道忙
						Addwaitpcb(chtemp->blocking,ptemp);
						cout<<"进程"<<ptemp->name<<"被挂在通道"<<chtemp->identify<<"的等待队列上!"<<endl;
					}
					else
					{//通道不忙
						Addwaitpcb(run,ptemp);
						dtemp->state=1;
						cotemp->state=1;
						chtemp->state=1;
						strcpy(ptemp->devname,temp2->identify);
						cout<<"进程"<<pname<<"申请设备"<<temp2->identify<<"成功!"<<endl;
					}
				}
			}
			else
			{//当设备忙时候
				Addwaitpcb(dtemp->blocking,ptemp);
				cout<<"进程"<<ptemp->name<<"被挂在设备"<<dtemp->identify<<"的等待队列上!"<<endl;
			}
		}
	}
	else
	{
		cout<<"输入设备的类型错误!"<<endl;
	}
}
//
void Show()
{
	cout<<"A :增加设备"<<endl;
	cout<<"D :删除设备"<<endl;
	cout<<"S :申请设备"<<endl;
	cout<<"H :回收设备"<<endl;
	cout<<"I :设备独立性"<<endl;
	cout<<"Q :退出"<<endl;
}
//
int main()
{  cout<<"                   设备管理实验          "<<endl;
   cout<<"                                           "<<endl;
   cout<<"相关指令："<<endl;
	Init();
	Show();
	char choice;
	while(true)
	{
		Showmenu();
		Show();
		cout<<"选择指令:"<<endl;
		cin>>choice;
		switch(choice)
		{
		case 'a':
		case 'A':
			Adddevice();
			break;
		case 'd':
		case 'D':
			Deletedevice();
			break;
		case 's':
		case 'S':
			Applydevice();
			break;
		case 'h':
		case 'H':
			Recycledevice();
			break;
		case 'q':
		case 'Q':
			exit(0);
		case 'i':
		case 'I':
			Independence();
			break;
		default :
			cout<<"指令错误"<<endl;
			break;
		}
	}
	return 0;
}
