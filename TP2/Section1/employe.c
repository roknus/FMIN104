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

typedef struct message
{
  char mess[255];
} Message;

int main()
{
  key_t key = ftok("./id",10);

  //Creation semaphore
  int semid = semget(key,2,IPC_CREAT|IPC_EXCL|0666);
  if( semid < 0 )
    {
      semid = semget(key,2,0666);
    }
  else
    {
      union senum init;
      //Semaphore pour le boss
      init.val = 0;
      if(semctl(semid,0,SETVAL,init) == -1)
	{
	  printf("probleme init");
	}
      //Semaphore pour les employers
      init.val = 2;
      if(semctl(semid,1,SETVAL,init) == -1)
	{
	  printf("probleme init");
	}
    }
  int shmid = shmget(key,sizeof(Message),IPC_CREAT|0666);
  
  struct sembuf P;
  P.sem_num = 0;
  P.sem_op = -1;
  P.sem_flg = NULL;

  semop(semid,&P,1);

  Message* mess = (Message *)shmat(shmid, NULL, NULL);

  printf("%s\n",mess->mess);

  struct sembuf V;
  V.sem_num = 0;
  V.sem_op = +1;
  V.sem_flg = NULL;

  semop(semid,&V,1);

  P.sem_num = 1;

  semop(semid,&P,1);

  return EXIT_SUCCESS;
}
