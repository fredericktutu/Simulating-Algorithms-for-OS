#include<stdlib.h>
#include<stdio.h>
int BDD[8][8]; //λʾͼ 
int enable;

typedef struct Block_{
	int next;
} Block;
Block FAT[64];   //�ļ������ 

typedef struct File_{
	int start;
	int length;
	int isFile;
}File; 
File FTB[64]; //��ļ��� 
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
		printf("�ñ���ļ��Ѿ�������Դ!\n"); 
		return;
	}
	else
	{
		if(length>enable)
		{
			printf("���̿ռ䲻�㣬����ʧ��!\n");
		}
		else
		{
			printf("���ڷ�����̿ռ�...<��ʽ��(���棬�ŵ�������)>\n");
			i=0; //������λʾͼ�в��ҿմ��̿� 
			
			while(BDD[i/8][i%8]) //Ѱ�ҵ�һ�����д��̿� 
			i++;
			length--;
			last=i;
			printf("(%d,%d,%d) ",i/8,(i%8)/4,(i%8)%4);
			FTB[num].isFile=1;  //�޸Ļ�ļ��� 
			FTB[num].start=i;
			FTB[num].length=length+1; 
			BDD[i/8][i%8]=1;  //�޸�λʾͼ 
			enable--;
            i++;
			while(length>0) //����ʣ�µĿմ��̿飬���޸�FAT��λʾͼ 
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
		printf("���ļ������ڣ��黹ʧ�ܣ�\n");
		return; 
	}
	else
	{
		printf("��ʼ�黹�ļ�%dռ�õĴ��̿�,��%d��..<��ʽ��(�ֽڣ�λ)>\n",num,FTB[num].length);
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
	printf("|    ��ļ���   |\n"); 
    printf("+--------+--------+\n");
    printf("|�ļ����|�ļ�����|\n");
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
	printf("|  λʾͼ |\n"); 
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
	printf("�����ʽ\n������̿ռ�:(0,�ļ����,�ļ���С)\n�黹���̿ռ�:(1,�ļ����,0)\n");
	printf("�鿴λʾͼ�� (2,0,0)\n�˳�����:    (3,0,0)\n");
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
		  printf("��ʽ����");fflush(stdin);continue; 
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
