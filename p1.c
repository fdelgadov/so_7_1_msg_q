#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define CLAVE_MSG 2000
#define OK 0
#define ERROR -1
#define INFO 150
#define TIPO 7

typedef struct{
  int numero;
  char nombre[20];
  float sueldo;
} DATA;

typedef struct {
  long tipo;
  DATA info;
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
  
  msg.tipo = 1; // pid del destinatario
  DATA dt = {10, "Franco", 10.5};
  msg.info = dt; // informacion a transmitir
  printf("dt: %d %s %f\n", dt.numero, dt.nombre, dt.sueldo);
  printf("Enviando mensaje...\n");
  if (msgsnd(qid, &msg, sizeof(MENSAJE) - sizeof(long), 0) == ERROR) {
    perror("msgsnd:");
    exit(errno);
  }
  printf("Mensaje enviado.\n");

  exit(OK);
}
