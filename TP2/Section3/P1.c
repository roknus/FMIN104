#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>

union senum
{
  int val;
  struct semid_ds *buf;
  unsigned short *array;
  struct seminfo *__buf;
};

int main()
{
  key_t key = ftok("./id",10);
  int semid = semget(key,3,IPC_CREAT|IPC_EXCL|0666);
  if( semid < 0 )
    {
      semid = semget(key,3,0666);
    }
  else
    {
      union senum init;
      init.val = 0;
      int numSem;
      for(numSem = 0; numSem < 3; numSem++)
	{
	  if(semctl(semid,numSem,SETVAL,init) == -1)
	    {
	      perror("probleme init");
	    }
	}
    }
  struct sembuf semoi;
  semoi.sem_flg = NULL;
  semoi.sem_op = +1;
  int i;
  for(i = 0; i < 3 ; i++)
    {
      semoi.sem_num = i;
      printf("Travail sur le segment %d\n",i);
      sleep(5);
      printf("Fin du travail sur le segment %d\n",i);
      semop(semid,&semoi,1);
    }
    return EXIT_SUCCESS;
}
