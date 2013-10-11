#include<stdio.h>
#include<stdlib.h>
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
  int semid = semget(key,1,IPC_CREAT|IPC_EXCL|0666);
  if( semid < 0 )
    {
      semid = semget(key,1,0666);
    }
  else
    {
      union senum init;
      init.val = 3;
      if(semctl(semid,0,SETVAL,init) == -1)
	{
	  printf("probleme init");
	}
    }
  struct sembuf P;
  struct sembuf Z;

  P.sem_num = 0;
  P.sem_op = -1;
  P.sem_flg = SEM_UNDO;

  Z.sem_num = 0;
  Z.sem_op = 0;
  Z.sem_flg = SEM_UNDO;

  semop(semid,&P,1);

  printf("Debut\n");

  semop(semid,&Z,1);

  printf("Fin\n");
  
  return EXIT_SUCCESS;
}
