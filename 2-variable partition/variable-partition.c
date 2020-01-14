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
}JTB;    //���ҵ�� 
struct FTB{
	Free *firstfree;
}FTB;    //���п��� 

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
		printf("����ʧ��\n");
	}else
	{
		printf("��ҵ%d����ɹ�,����λ�ã�����%d\n",num,f1->start);
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
		printf("�����в����ڸñ�ŵ���ҵ��\n");
	}
	else
	{
		printf("�����ڴ�ɹ�\n");
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
		if(f1==NULL) //���п�����û�п��п飬�������׼�һ���� 
		{
			printf("���п����գ��¼���\n"); 
			newfree=(Free*)malloc(sizeof(Free));
			newfree->length=length;
			newfree->start=start;
			newfree->next=NULL;
			FTB.firstfree=newfree;
			return;
		}else if(f1->start>start) //Ӧ�÷��ڵ�һ�� 
		{
			if(start+length==f1->start) //���ڽ� 
			{
				printf("���п�������ף��Һͺ�һ���ڽӣ��ͺ�һ���ϲ�\n");
				f1->start=start;
				f1->length+=length;
				return;
			}else    //û�����ڽ� 
			{
				printf("���п�������ף��Ҳ��ͺ�һ���ڽ�\n");
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
		if(f1==NULL) //λ����β 
		{
			if(f2->start+f2->length==start) //���ڽ� 
			{
				printf("���п������β���Һ�ǰһ���ڽӣ���ǰһ���ϲ�\n"); 
				f2->length+=length;
				return;
			}else  //û�����ڽ� 
			{
                printf("���п������β���Ҳ���ǰһ���ڽ�\n");
				newfree=(Free*)malloc(sizeof(Free));
			    newfree->length=length;
			    newfree->start=start;
			    newfree->next=NULL;
			    f2->next=newfree;
			    return;
			}
		}else   //λ�����в���һ��λ�� 
		{
			if(f2->start+f2->length==start &&start+length==f1->start) //�����ڽ�
			{
				printf("���п�������У���ǰ���ڽ�,��ǰ��ϲ�\n");
			    f2->length+=length+f1->length;
				f2->next=f1->next;
				free(f1); 
				return;
		    }
			else if(f2->start+f2->length==start)// ���ڽ�
			{
				printf("���п���ڷ������У��Һ�ǰһ���ڽӣ���ǰһ���ϲ�\n");
			    f2->length+=length; 
			    return;
			}
            else if(start+length==f1->start) //���ڽ�
			{
				printf("���п�������У��Һͺ�һ���ڽӣ��ͺ�һ���ϲ�\n");
				f1->length+=length;
				f1->start=start;
				return;
			} 
			else    //��û�� 
			{
				printf("���п�������У�û���κ��ڽ�\n");
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

void Disp() //չʾ������� 
{
	int count=0; 
	int num=0; 
	Job *j=JTB.firstjob;
	Free *f=FTB.firstfree;
	        printf("+------+---------------+--------+----+\n"); 
	        printf("|����|���п��/��ҵ��|��ʼ��ַ|����|\n");
	        printf("+------+---------------+--------+----+\n");
	while(j!=NULL&& f!=NULL)
	{
		if(j->start<f->start)
		{
			count++;
			printf("| %3d  |     ��ҵ%3d   |  %3d   | %3d|\n",count,j->num,j->start,j->length);
			printf("+------+---------------+--------+----+\n");
			j=j->next;
		}else
		{
			count++;
			num++;
			printf("| %3d  |    ���п�%3d  |  %3d   | %3d|\n",count,num,f->start,f->length);
			printf("+------+---------------+--------+----+\n");
			f=f->next;
		}
	}
	if(f==NULL)
	{
		while(j!=NULL)
		{
			count++;
			printf("| %3d  |     ��ҵ%3d   |  %3d   | %3d|\n",count,j->num,j->start,j->length);
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
			printf("| %3d  |    ���п�%3d  |  %3d   | %3d|\n",count,num,f->start,f->length);
			printf("+------+---------------+--------+----+\n");
			f=f->next;
		}
	}
}
int main(){
	int num,length,type;
	Init();
	Disp(); 
	printf("ָ���ʽ:\n���䣺(0,��ҵ��,��ҵ����) \n���գ�(1,��ҵ��,0)\n�˳�����:(3,��������,��������)\n����������\n");
	while(1){
	  scanf("(%d,%d,%d)",&type,&num,&length);
	  printf("\n");
	  if(type==3)
	  {
	  	printf("������");
	  	break;
	  }
	  if(num<=0 ||!(type==0||type==1)||(length<=0&&type==0)||(type==1 && length!=0))
	  {
	  printf("��ʽ����\n");
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


