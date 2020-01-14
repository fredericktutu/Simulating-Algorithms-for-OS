#include<stdlib.h>
#include<stdio.h>
int BDD[8][8]; //位示图 
int enable;

typedef struct Block_{
	int next;
} Block;
Block FAT[64];   //文件分配表 

typedef struct File_{
	int start;
	int length;
	int isFile;
}File; 
File FTB[64]; //活动文件表 
void Init() 
{
	int i,j;
	for(i=0;i<8;i++)
	for(j=0;j<8;j++)
	BDD[i][j]=0;
	
	enable=64;
	
	for(i=0;i<64;i++)
	{
		FAT[i].next=-1;
		FTB[i].isFile=0;
	}
}
void Alloc(int num,int length)
{
	int i,j,k;
	int last; 
	if(FTB[num].isFile)
	{
		printf("该编号文件已经分配资源!\n"); 
		return;
	}
	else
	{
		if(length>enable)
		{
			printf("磁盘空间不足，分配失败!\n");
		}
		else
		{
			printf("正在分配磁盘空间...<格式：(柱面，磁道，扇区)>\n");
			i=0; //用于在位示图中查找空磁盘块 
			
			while(BDD[i/8][i%8]) //寻找第一个空闲磁盘块 
			i++;
			length--;
			last=i;
			printf("(%d,%d,%d) ",i/8,(i%8)/4,(i%8)%4);
			FTB[num].isFile=1;  //修改活动文件表 
			FTB[num].start=i;
			FTB[num].length=length+1; 
			BDD[i/8][i%8]=1;  //修改位示图 
			enable--;
            i++;
			while(length>0) //查找剩下的空磁盘块，并修改FAT，位示图 
			{
				while(BDD[i/8][i%8])
				i++;
				length--;
				FAT[last].next=i;
				last=i;
				printf("(%d,%d,%d) ",i/8,(i%8)/4,(i%8)%4);
				BDD[i/8][i%8]=1;
				enable--;
				i++;
			}
			printf("\n");
			FAT[last].next=-1;
			
		}
	}
}
void Release(int num)
{
	int start;
	int length;
	int i,j; 
	if(!FTB[num].isFile)
	{
		printf("该文件不存在，归还失败！\n");
		return; 
	}
	else
	{
		printf("开始归还文件%d占用的磁盘块,共%d块..<格式：(字节，位)>\n",num,FTB[num].length);
		start=FTB[num].start;
		FTB[num].isFile=0;
		i=start;
		while(i!=-1)
		{
		  printf("(%d,%d) ",i/8,i%8);
		  BDD[i/8][i%8]=0;
		  enable++;
		  j=FAT[i].next;
		  FAT[i].next=-1;
		  i=j;
		}	
		printf("\n");
	}
}
void DispF()
{
	int i;
	printf("+--------+--------+\n");
	printf("|    活动文件表   |\n"); 
    printf("+--------+--------+\n");
    printf("|文件编号|文件长度|\n");
    printf("+--------+--------+\n");
	for(i=0;i<64;i++)
	{
		if(FTB[i].isFile)
		{
		  printf("|   %2d   |   %2d   |\n",i,FTB[i].length);
		  printf("+--------+--------+\n");
	    }
	}
}
void DispB()
{
	int i,j;
	printf("+---------+\n");
	printf("|  位示图 |\n"); 
	for(i=0;i<8;i++)
	{
		printf("|");
		for(j=0;j<8;j++)
		{
		  printf("%d",BDD[i][j]);
		  if(j==3)printf(" ");
	    }
		printf("|\n");
	}
    printf("+---------+\n");
}

int main()
{
	int type,num,length;
	Init();
	DispB();
	printf("命令格式\n分配磁盘空间:(0,文件编号,文件大小)\n归还磁盘空间:(1,文件编号,0)\n");
	printf("查看位示图： (2,0,0)\n退出程序:    (3,0,0)\n");
	while(1)
	{
		scanf("(%d,%d,%d)",&type,&num,&length);
		if(type==2)
		{
			DispB();fflush(stdin);continue;
		}
		if(type==3)break;
		if(!(type==0||type==1)||!(num>=0&&num<64)||(type==1&&length!=0)||(type==0&&length<=0))
		{ 
		  printf("格式错误");fflush(stdin);continue; 
		} 
		if(type==0)
		{
			Alloc(num,length);
			DispF();
	    }else if(type==1)
	    {
	    	Release(num);
	    	DispF();
		}
		fflush(stdin);
	} 
	return 0;
}
