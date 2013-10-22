#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
 
pthread_mutex_t verrou = PTHREAD_MUTEX_INITIALIZER;

void incrementeMatricule(char matricule[])
{
  if(matricule[2] < '9')
    {
      matricule[2]++;
    }
  else
    {
      matricule[2] = '0';
      if(matricule[1] < '9')
	{
	  matricule[1]++;
	}
      else
	{
	  matricule[1] = '0';
	  if(matricule[0] < '9')
	    {
	      matricule[0]++;
	    }
	  else
	    {
	      matricule[0] = '0';
	      if(matricule[5] < 'Z')
		{
		  matricule[5]++;
		}
	      else
		{
		  matricule[5] = 'A';
		  if(matricule[4] < 'Z')
		    {
		      matricule[4]++;
		    }
		  else
		    {
		      matricule[4] = 'A';
		      if(matricule[3] < 'Z')
			{
			  matricule[3]++;
			}
		      else
			{
			  
			}
		    }
		}
	    }
	}
    }
}

void *thread (void *arg) {
  pthread_t p = pthread_self();
  char* matricule = (char *) arg; 
  int i = 0;
  while(i < 10000000 )
    {
      pthread_mutex_lock(&verrou);
      printf("%s - %d\n",matricule,(int)p);
      incrementeMatricule(matricule);
      pthread_mutex_unlock(&verrou);
      i++;
    }
  pthread_exit(NULL); 
}
 
int main() {
 
  char matricule[8] = "000AAA34";
 
  pthread_t tid[3];
  
  int i;
  for(i = 0; i < 3; i++)
    {
      pthread_create(&tid[i], NULL, thread, matricule);
    }

  printf ("Attente de la terminaison du thread\n");
  pthread_join(tid[0],NULL);
  pthread_join(tid[1],NULL);
  pthread_join(tid[2],NULL);
  
  return 0;
  
}
