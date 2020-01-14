#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>

struct PCB{
	char name;
	char state;
	char wait_reason;
	int bp;
}produce,consume,*run,*ready;
int s1=10,s2=0;
int PC;
int PA[5],SA[5],in=0,out=0;
char product=' ',B[10],X=' ';

void Init()
{
	int i;
	for(i=0;i<5;i++)
	{
		PA[i]=i;
		SA[i]=i;
	}
	for(i=0;i<10;i++)
	B[i]=' '; 
	produce.name='P';
	consume.name='C';
	produce.state='R';
	consume.state='R';
	produce.bp=0;
	consume.bp=0;
	PC=0;
}

void Schedule()
{
	if(produce.state=='R'&&consume.state=='R')
	{
		srand((unsigned)time(NULL));
		int a=rand()%2;
		if(a)
		{
			run=&produce;
			ready=&consume;
		}
		else
		{
			run=&consume;
			ready=&produce;
		}
	}
	else if(produce.state=='R'&&consume.state=='W')
	{
		run=&produce;
		ready=&consume;
	}
	else if(produce.state=='W'&&consume.state=='R')
	{
		run=&consume;
		ready=&produce;
	}
	PC=run->bp;
}

void Run()
{
	char temp;
	if(run->name=='P')
	{
		int j=PA[PC];
		PC++;
		switch(j)
		{
			case 0: //produce
			{
				printf("请输入字符:");
				temp=getchar();
				if(temp=='\n')
				temp=getchar();
				product=temp;
				break;	
			}
			case 1: //P(s1)
			{
				s1--;
				if(s1<0)
				{
					run->state='W';
					run->wait_reason='1';
					printf("生产者等待原因:s%c\n",run->wait_reason);
					break;
				}
				else
				{
					run->state='R';
					break;
				}
			}
			case 2: //PUT
			{
				B[in]=product;
				in=(in+1)%10;
				printf("缓冲器中的字符为:");
				for(int i=0;i<10;i++)
				{
					printf("%c ",B[i]);
				}
				printf("\n");
				break;
			}
			case 3: //V(s2)
			{
				s2++;
				if(s2<=0)
				ready->state='R';
				else
				run->state='R';
				break;
			}
			case 4:  //goto 0
			{
				PC=0;
				break;
			}
		}
	}
	else
	{
		int j=SA[PC];
		PC++;
		switch(j)
		{
			case 0: //P(s2)
			{
				s2--;
				if(s2<0)
				{
					run->state='W';
					run->wait_reason='2';
					printf("消费者等待原因:s%c\n",run->wait_reason);
			    }
			    break;
		    }
			case 1://GET
			{
				X=B[out];
				out=(out+1)%10;
				break;
			}
			case 2: //V(s1)
			{
				s1++;
				if(s1<=0)
				ready->state='R';
				else
				run->state='R';
				break;
			}
			case 3: //print
			{
				printf("执行消费者进程，打印输出字符:");
				 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15 | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_INTENSITY);  
				printf("[%c]\n",X);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); 
				break;
			}
			case 4: //goto 0
			{
				PC=0;
			}
		}
	}
	run->bp=PC;
}

int main()
{
	Init();
	for(int i=0;i<300;i++)
	{
		Schedule();
		Run();
	}
}










