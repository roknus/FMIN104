#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>


void *monThread(void *par)
{
  int *valeur = (int *) par;
  pthread_t moi = pthread_self();
  int i =0;
  printf("%d",*valeur);
  while(i < 1000)
    {
      printf("0");
      i++;
    }
  pthread_exit(NULL);
}

int main()
{
  pthread_t idTh;
  int valeur = 2;
  if(pthread_create(&idTh, NULL, monThread, &valeur) != 0)
    {
      perror("Erreur creation thread");
    }
  int j=0;
  while(j < 1000)
    {
      printf("1");
      j++;
    }
  pthread_join(idTh,NULL);

  return EXIT_SUCCESS;
}
