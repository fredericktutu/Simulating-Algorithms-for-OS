#include<stdio.h>
#include<stdlib.h>

typedef struct Job_{
	int num;
	int start;
	int length;
	struct Job_ *next;
}Job; 
typedef struct Free_{
	int start;
	int length;
	struct Free_ *next;
}Free;
struct JTB{
	Job *firstjob;
}JTB;    //活动作业链 
struct FTB{
	Free *firstfree;
}FTB;    //空闲块链 

void Init()
{
	Free *f1;
	JTB.firstjob=NULL;

	f1=(Free*)malloc(sizeof(Free));
	f1->length=150;
	f1->start=0;
	f1->next=NULL;
	FTB.firstfree=f1;
}

void AddJob(int num,int length,int start)
{   
    Job *j,*pj;
    Job *newjob;
    j=JTB.firstjob;
	if(j==NULL|| start<j->start )
	{
		newjob=(Job*)malloc(sizeof(Job));
		newjob->num=num;
		newjob->length=length;
		newjob->start=start;
		newjob->next=j;
		JTB.firstjob=newjob;
		return;
	}else
	{
		pj=j;
		j=j->next;
		while(j!=NULL && j->start<start)
		{
			pj=j;
			j=j->next;
		}
		newjob=(Job*)malloc(sizeof(Job));
		newjob->num=num;
		newjob->length=length;
		newjob->start=start;
		newjob->next=j;
		pj->next=newjob;
		return;
	}
	
	
}

void Alloc(int num,int length)
{
	Free *f1,*f2;
	int count=1;
	f1=FTB.firstfree;
	f2=f1;
	while(f1!=NULL && f1->length<length)
	{
		if(f1!=FTB.firstfree)
		f2=f2->next;
		f1=f1->next;
		count++;
	
	}
	if(f1==NULL)
	{
		printf("分配失败\n");
	}else
	{
		printf("作业%d分配成功,分配位置：主存%d\n",num,f1->start);
		AddJob(num,length,f1->start);
		if(f1==FTB.firstfree)
		{
			if(f1->length==length)
			{
				FTB.firstfree=f1->next;
				free(f1);
				return;
			}
			else
			{
				f1->start+=length;
				f1->length-=length;
				return;
			}
		}
		else
		{
			if(f1->length==length)
			{
				f2->next=f1->next;
				free(f1);
				return;
			}else
			{
				f1->start+=length;
				f1->length-=length;
				return;
			}
		}
    } 
}

void Release(int num)
{
	Job *j=JTB.firstjob,*pj=j;
	Free *f1,*f2,*newfree;
	int start,length;
	while(j!=NULL && j->num!=num)
	{
		if(j!=JTB.firstjob)
		pj=pj->next;
		j=j->next;
	}
	if(j==NULL)
	{
		printf("主存中不存在该编号的作业！\n");
	}
	else
	{
		printf("回收内存成功\n");
		start=j->start;
		length=j->length;
	    if(j==JTB.firstjob)
	    {
	    	JTB.firstjob=j->next;
			free(j); 
		}
		else
		{
			pj->next=j->next;
			free(j);
		}
		f1=FTB.firstfree;
		if(f1==NULL) //空闲块链中没有空闲块，则在链首加一个块 
		{
			printf("空闲块链空，新加入\n"); 
			newfree=(Free*)malloc(sizeof(Free));
			newfree->length=length;
			newfree->start=start;
			newfree->next=NULL;
			FTB.firstfree=newfree;
			return;
		}else if(f1->start>start) //应该放在第一个 
		{
			if(start+length==f1->start) //上邻接 
			{
				printf("空闲块放在链首，且和后一个邻接，和后一个合并\n");
				f1->start=start;
				f1->length+=length;
				return;
			}else    //没有上邻接 
			{
				printf("空闲块放在链首，且不和后一个邻接\n");
				newfree=(Free*)malloc(sizeof(Free));
			    newfree->length=length;
			    newfree->start=start;
			    newfree->next=FTB.firstfree;
				FTB.firstfree=newfree; 
				return;
			}
		}
		f2=f1;
		f1=f1->next;
		while(f1!=NULL && f1->start<start)
		{
			f2=f1;
			f1=f1->next;
		}
		if(f1==NULL) //位于链尾 
		{
			if(f2->start+f2->length==start) //下邻接 
			{
				printf("空闲块放在链尾，且和前一个邻接，和前一个合并\n"); 
				f2->length+=length;
				return;
			}else  //没有下邻接 
			{
                printf("空闲块放在链尾，且不和前一个邻接\n");
				newfree=(Free*)malloc(sizeof(Free));
			    newfree->length=length;
			    newfree->start=start;
			    newfree->next=NULL;
			    f2->next=newfree;
			    return;
			}
		}else   //位于链中部的一个位置 
		{
			if(f2->start+f2->length==start &&start+length==f1->start) //上下邻接
			{
				printf("空闲块放在链中，且前后邻接,和前后合并\n");
			    f2->length+=length+f1->length;
				f2->next=f1->next;
				free(f1); 
				return;
		    }
			else if(f2->start+f2->length==start)// 下邻接
			{
				printf("空闲块放在放在链中，且和前一个邻接，和前一个合并\n");
			    f2->length+=length; 
			    return;
			}
            else if(start+length==f1->start) //上邻接
			{
				printf("空闲块放在链中，且和后一个邻接，和后一个合并\n");
				f1->length+=length;
				f1->start=start;
				return;
			} 
			else    //都没有 
			{
				printf("空闲块放在链中，没有任何邻接\n");
				newfree=(Free*)malloc(sizeof(Free));
				newfree->start=start;
				newfree->length=length;
				f2->next=newfree;
				newfree->next=f1;
				return;
			}
		}
	}
}

void Disp() //展示分配情况 
{
	int count=0; 
	int num=0; 
	Job *j=JTB.firstjob;
	Free *f=FTB.firstfree;
	        printf("+------+---------------+--------+----+\n"); 
	        printf("|块编号|空闲块号/作业号|起始地址|长度|\n");
	        printf("+------+---------------+--------+----+\n");
	while(j!=NULL&& f!=NULL)
	{
		if(j->start<f->start)
		{
			count++;
			printf("| %3d  |     作业%3d   |  %3d   | %3d|\n",count,j->num,j->start,j->length);
			printf("+------+---------------+--------+----+\n");
			j=j->next;
		}else
		{
			count++;
			num++;
			printf("| %3d  |    空闲块%3d  |  %3d   | %3d|\n",count,num,f->start,f->length);
			printf("+------+---------------+--------+----+\n");
			f=f->next;
		}
	}
	if(f==NULL)
	{
		while(j!=NULL)
		{
			count++;
			printf("| %3d  |     作业%3d   |  %3d   | %3d|\n",count,j->num,j->start,j->length);
			printf("+------+---------------+--------+----+\n");
			j=j->next;
		}
	}
	if(j==NULL)
	{
		while(f!=NULL)
		{
			count++;
			num++;
			printf("| %3d  |    空闲块%3d  |  %3d   | %3d|\n",count,num,f->start,f->length);
			printf("+------+---------------+--------+----+\n");
			f=f->next;
		}
	}
}
int main(){
	int num,length,type;
	Init();
	Disp(); 
	printf("指令格式:\n分配：(0,作业号,作业长度) \n回收：(1,作业号,0)\n退出程序:(3,任意数字,任意数字)\n请输入命令\n");
	while(1){
	  scanf("(%d,%d,%d)",&type,&num,&length);
	  printf("\n");
	  if(type==3)
	  {
	  	printf("结束！");
	  	break;
	  }
	  if(num<=0 ||!(type==0||type==1)||(length<=0&&type==0)||(type==1 && length!=0))
	  {
	  printf("格式不对\n");
	  fflush(stdin);
	  continue;
      }
	  if(type==0)
	  {
		Alloc(num,length);
		Disp();
	  } 
	  else
	  {
	  	Release(num);
	  	Disp();
	  }
	  fflush(stdin);
    }
    system("pause");
    return 0;
}


