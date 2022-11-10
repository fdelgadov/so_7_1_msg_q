#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#define CLAVE 100
#define SND 1
#define RCV 2
#define SIZE sizeof(char) * 9
#define NAME "OS"

char* TABLE;

typedef struct {
  long type;
  int info;
} mensaje;

void printTable(){
  for(int i = 0; i < 3; i++){
    printf("%c ", TABLE[3 * i]);
    for(int j = 1; j < 3; j++){
      printf("| %c", TABLE[3 * i + j]);
    }
    printf("\n");
  }
  printf("\n");
}

int main(){
  int fd;
  fd = shm_open(NAME, O_CREAT | O_RDWR, 0766);
  ftruncate(fd, SIZE);
  TABLE = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, fd, 0);
  sprintf(TABLE, "%s", "        ");

  int qid;

  qid = msgget(CLAVE, IPC_CREAT | IPC_EXCL | SHM_R | SHM_W);
  if (errno == EEXIST) {
    printf("Ya existe una cola de mensajes, accediendo...\n");
    qid = msgget(CLAVE, SHM_R | SHM_W);
  }

  struct msqid_ds conf;
  msgctl(qid, IPC_STAT, &conf);
  conf.msg_qbytes = sizeof(mensaje) - sizeof(long);
  msgctl(qid, IPC_SET, &conf);

  mensaje msg;
  msg.type = RCV;
  msg.info = 1;

  msgsnd(qid, &msg, sizeof(mensaje) - sizeof(long), 0);

  int turns = -1, idx;
  while(turns < 9){
    msgrcv(qid, &msg, sizeof(mensaje) - sizeof(long), RCV, 0);
    printf("%d\n", turns);
    turns = msg.info;
    printTable();
    printf("Ingrese posicion (0-9): ");
    scanf("%d", &idx);
    TABLE[idx] = 'X';
    msg.info = turns + 1;
    msg.type = SND;
    msgsnd(qid, &msg, sizeof(mensaje) - sizeof(long), 0);
  }

  printTable();
  shm_unlink(NAME);
  msgctl(qid, IPC_RMID, NULL);
  printf("Cola de mensajes eliminada...\n");
}