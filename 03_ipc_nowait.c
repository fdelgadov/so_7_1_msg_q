#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>

#define CLAVE 100
#define TYPE 1

typedef struct {
  long type;
  int info;
} mensaje;

int main(){
  int qid;
  mensaje mg1, mg2, mg3;
  struct msqid_ds conf;
  
  qid = msgget(CLAVE, IPC_CREAT | IPC_EXCL | SHM_R | SHM_W);
  printf("Message queue creada\n");

  msgctl(qid, IPC_STAT, &conf);
  conf.msg_qbytes = sizeof(mensaje) - sizeof(long);
  msgctl(qid, IPC_SET, &conf);
  printf("Message queue configurada\n");

  mg1.type = TYPE;
  mg1.info = 10;
  msgsnd(qid, &mg1, sizeof(mensaje) - sizeof(long), 0);
  printf("Enviado msg1\n");

  mg2.type = TYPE;
  mg2.info = 15;
  msgsnd(qid, &mg2, sizeof(mensaje) - sizeof(long), 0);
  if(errno == EAGAIN)
    printf("errno: EAGAIN\n");
  else
    printf("Enviado msg2\n");

  msgctl(qid, IPC_RMID, NULL);
  printf("Message queue eliminada\n");

  return 0;
}