#include<stdio.h>
#include<gtthread.h>

void printHello(int a)
{
   int i = 0;
   while(i < 9999999)
  {	
  	fprintf(stderr, "\nThread 1 %d ", a);
	i++;
  }
 
}

int main()
{

  int a = 5;	
  void* returnVal;	  
  gtthread_t *thread;
  
  gtthread_init(50000);
  
  thread = (gtthread_t*)malloc(sizeof(gtthread_t));
	gtthread_create(thread, &printHello, 32);

  //gtthread_join(*thread, &returnVal);

 	
  while(a < 10000)
  {  
  	fprintf(stderr, "\nLOG:Main exiting");
	a++;
  }

  return 0;
}

