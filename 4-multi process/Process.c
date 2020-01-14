#include<stdio.h>
#include<unistd.h>
void delay(int x)
{
    int i,j;
    for(i=0;i<x;i++)
    for(j=0;j<x;j++);

}

int main(){
  int pid;
  pid=fork();
  if(pid==0)
  {
    delay(2048);
    printf("ID%d(Parent%d):B\n",getpid(),getppid());
    return 0;
  }
  else 
  {
    pid=fork();
    if(pid==0)
    {
      delay(2048);
      printf("ID%d(Parent%d):C\n",getpid(),getppid()); return 0;
    }
    else
    {
      delay(2048);
      printf("ID%d:A\n",getpid());
      return 0;
    }
  }
  
   
}

