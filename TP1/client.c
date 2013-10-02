#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>

#include"messages.h"

int main(){
  key_t key = ftok("./file",10);
  int id = msgget(key,IPC_CREAT|IPC_EXCL|0666);
  if(id > 0){
    msgctl(id,IPC_RMID,NULL);
    exit(0);
  }
  id = msgget(key,0666);

  Request req;
  Response res;

  printf("Operande n°1 : ");
  scanf("%d",&req.op1);
  printf("(+:0,-:1,*:2,/:3) : ");
  scanf("%d",&req.type);
  printf("Operande n°2 : ");
  scanf("%d",&req.op2);
  req.label = 1;

  if(msgsnd(id,&req,sizeof(req)-sizeof(long),0) < 0){
    printf("Erreur envoie message !");
    exit(0);
  }
  if(msgrcv(id,&res,sizeof(res),2,0) < 0){
    printf("Erreur reception message !");
    exit(0);
  }
  printf("\nLe résultat est : %d\n", res.res);  
}
