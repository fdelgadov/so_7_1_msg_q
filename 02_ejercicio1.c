#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define CLAVE_MSG 2000
#define OK 0
#define ERROR -1
#define INFO 150
#define TIPO 7

typedef struct {
  long tipo;
  int info;
} MENSAJE;

int main() {
  int qid, opcion;
  MENSAJE msg, msg2;
  int lector = 0;
  
  qid = msgget(CLAVE_MSG, IPC_CREAT | IPC_EXCL | SHM_R | SHM_W);
  if (qid == ERROR) {
    if (errno == EEXIST) {
      printf("Ya existe una cola de mensajes, accediendo...\n");
      qid = msgget(CLAVE_MSG, SHM_R | SHM_W);
      if (qid == ERROR) {
        perror("msgget:");
        exit(errno);
      }
    }
  }
  printf("Cola de mensajes creada...\n");
  
  msg.tipo = TIPO; // pid del destinatario
  msg.info = INFO; // informacion a transmitir
  printf("Enviando mensaje...\n");
  if (msgsnd(qid, &msg, sizeof(MENSAJE) - sizeof(long), 0) == ERROR) {
    perror("msgsnd:");
    exit(errno);
  }
  printf("Mensaje enviado.\n");
  
  if(fork() == 0){
    printf("Leyendo el primer mensaje de la cola...\n");
    if (msgrcv(qid, &msg2, sizeof(MENSAJE) - sizeof(long), 0, 0) == ERROR) {
      perror("msgrcv:");
      exit(errno);
    }
    printf("Mensaje recibido de tipo = %ld con info = %d\n", msg2.tipo,
    msg2.info);
    
    if (msgctl(qid, IPC_RMID, NULL) == ERROR) {
      perror("msgctl:");
      exit(errno);
    }
    printf("Cola de mensajes eliminada\n");

    return 0;
  }
  return 0;
}
