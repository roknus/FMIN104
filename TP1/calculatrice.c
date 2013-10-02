#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include"messages.h"

int main(){
  key_t key = ftok("./file",10);
  int id = msgget(key,IPC_CREAT|IPC_EXCL|0666);
  if(id == -1){
    printf("File de message deja existante");
    exit(0);
  }
  Request req;
  Response res;
  while(1){
    if(msgrcv(id,&req,sizeof(req),1,0) < 0){
      printf("Erreur reception message !");
      exit(0);
    }
    float result;
    switch(req.type)
      {
      case 0:
	result = req.op1+req.op2;
	break;
      case 1:
	result = req.op1-req.op2;
	break;
      case 2:
	result = req.op1*req.op2;
	break;
      case 3:
	result = req.op1/req.op2;
	break;
      }
    res.label = 2;
    res.res = result;
    msgsnd(id,&res,sizeof(res)-sizeof(long),0);
  }
}
