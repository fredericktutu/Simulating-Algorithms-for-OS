#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
typedef struct Process_{
	int pid;
	struct Process_ *next;
	int need;
	int already;
	char state;
}Process; 

Process* Init(int pid,int need)
{
	Process* pt;
	if(need<=0)
	{
		printf("创建错误！\n提示：需要输入五个进程，且需要运行时间都大于0");
		exit(0); 
	}
	pt=(Process*)malloc(sizeof(Process));
	pt->pid=pid;
	pt->need=need;
	pt->next=NULL;
	pt->already=0;
	pt->state='R';
	return pt;
}
Process* Init1(int ps[])
{
	Process *head=Init(0,1);
	Process *pt=head;
	for(int i=0;i<5;i++)
	{
		pt->next=Init(i+1,ps[i]);
		pt=pt->next;
	}
	pt->next=head->next; //循环队列！ 
	
}

void Disp(Process* head)
{
	Process *p=head->next;
	printf("+------+--------+------+----+\n");
	printf("|进程号|要求运行|已运行|状态|\n");
	printf("+------+--------+------+----+\n");
	for(int i=0;i<5;i++)
	{
	  printf("|   %d  |   %2d   |  %2d  |  %c |\n",p->pid,p->need,p->already,p->state);
	  printf("+------+--------+------+----+\n");
	  Sleep(100);
	  p=p->next;
    }
	
}
void KillAll(Process* head)
{
	Process *pt,*p;
	p=head->next;
	for(int i;i<5;i++)
	{
		pt=p->next;
		free(p);
		p=pt;
	}
	free(head);
}
int ProcessRemain(Process* head)
{
	Process *p=head->next;
	int yes=0;
	for(int i=1;i<6;i++)
	{
		if(p->state=='R')
		{
			yes=1;
			break;
		}
		p=p->next;
	}
	return yes;
}
void DispR(Process* head)
{
	Process *p=head->next;
	int j=0,k,i;
	int a[6]={0};
	for(i=1;i<6;i++)
	{
		if(p->state=='R')
		{
			a[j]=i;
			j++;
		}
		p=p->next;
	}
	for(i=0;i<5;i++)
	{
		if(a[i]&&a[i+1])
		printf("%d>",a[i]);
		else if(a[i])
		printf("%d",a[i]);
	}
	
	switch(j){
	case 0:k=9;break;
    case 1:k=8;break;
    case 2:k=6;break;
    case 3:k=4;break;
    case 4:k=2;break;
    case 5:k=0;break;
    }
	for(;k>0;k--)
	{
		printf(" ");
	}
	printf("|\n");
}
int run(Process* head,int *cp,int *time,int clocktime)
{
	int i;
	Process *p=head->next;
	for(i=1;i<(*cp);i++)
	{
		p=p->next;  // 获取当前进程号 
	}
	if(p->state=='R')
	{
		if((p->need-p->already)<=clocktime)
		{
			p->state='E';
			printf("| %2d~%2d  |进程%d完成|    0   |   %2d   |  %2d  |  %c |",*time,*time+(p->need-p->already),p->pid,p->need,p->need,'E');
			DispR(head);
			printf("+------+-----------+--------+--------+------+----+---------+\n");
			p->already=p->need;
			*time=*time+p->need;
			(*cp)=(*cp==5)?1:(*cp+1);
			return 1;	
		}
		else
		{
			p->already=p->already+clocktime;
			printf("| %2d~%2d  |进程%d运行|   %2d   |   %2d   |  %2d  |  %c |",*time,*time+clocktime,p->pid,p->need-p->already,p->need,p->already,'R');
			DispR(head);
			printf("+------+-----------+--------+--------+------+----+---------+\n");
			*time=*time+clocktime;
			(*cp)=(*cp==5)?1:(*cp+1);
			return 1;
		}
	}
	else if(p->state=='E')
	{
		if(ProcessRemain(head))
		{
			while(p->state=='E')
			{
				p=p->next;
				(*cp)=(*cp==5)?1:(*cp+1);
			}
			if(p->need-p->already<=clocktime)
		    {
		    	p->state='E';
				printf("| %2d~%2d  |进程%d完成|    0   |   %2d   |  %2d  |  %c |",*time,*time+p->need-p->already,p->pid,p->need,p->need,'E');
				DispR(head);
				printf("+------+-----------+--------+--------+------+----+---------+\n");
                p->already=p->need;
				*time=*time+p->need;
				(*cp)=(*cp==5)?1:(*cp+1);
				return 1;
	 	    }
		    else
		    {
				p->already=p->already+clocktime;
				printf("| %2d~%2d  |进程%d运行|   %2d   |   %2d   |  %2d  |  %c |",*time,*time+clocktime,p->pid,p->need-p->already,p->need,p->already,'R');
				DispR(head);
				printf("+------+-----------+--------+--------+------+----+---------+\n");
				*time=*time+clocktime;
				(*cp)=(*cp==5)?1:(*cp+1);
				return 1;
		    }
		}
		else 
		{
			printf("所有进程都已完成！\n"); 
			return 0;
		}
	}
}

int main()
{    
    int ps[5];
    int cp;  //当前的进程号 
	int time=0; //当前的时间 
	int clocktime; 
	printf("请输入5个进程的需要运行时间，用逗号隔开：\n");
	scanf("%d,%d,%d,%d,%d",&ps[0],&ps[1],&ps[2],&ps[3],&ps[4]);	
	printf("创建进程中....\n");
	Process* head=Init1(ps);
	Disp(head);
	printf("创建进程成功！请输入时间片的长度：\n");
	scanf("%d",&clocktime);
	printf("现在开始运行\n");
    printf("+------+-----------+--------+--------+------+----+---------+\n");
    printf("|时 间 段|发生 事件|剩余时间|要求运行|已运行|状态|就绪 队列|\n");
    printf("+------+-----------+--------+--------+------+----+---------+\n");
	//开始运行 
	cp=1;
	while(run(head,&cp,&time,clocktime))
	{
		Sleep(300);
	}
	Disp(head);
	KillAll(head);
	system("pause");
	return 0;
}
