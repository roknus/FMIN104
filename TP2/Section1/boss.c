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

  //Creation memoire partagé
  int shmid = shmget(key,sizeof(Message),IPC_CREAT|0666);

  Message* mess;
  while(1)
    {
      mess = (Message *)shmat(shmid, NULL, NULL);
      
      printf("Ecrivez votre message\n");
      scanf("%s",mess->mess);
      if(strcmp(mess->mess,"exit") == 0)
	{
	  break;
	}
      
      if( shmdt(mess) < 0 )
	{
	  printf("erreur liberation memoire partagé\n");
	  exit(0);
	}
      
      struct sembuf V;
      V.sem_num = 0;
      V.sem_op = +1;
      V.sem_flg = NULL;
      
      semop(semid,&V,1);
      
      struct sembuf op[] = 
	{
	  {(u_short)1,(short)0,NULL},
	  {(u_short)0,(short)-1,NULL},
	  {(u_short)1,(short)+2,NULL}
	};
      
      semop(semid,op,3);
      
    }

  shmctl(shmid,IPC_RMID,NULL);
  semctl(semid,0,IPC_RMID);
      
  return EXIT_SUCCESS;
}
