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
#define SND 2
#define RCV 1
#define SIZE sizeof(char) * 9
#define NAME "OS"

char* TABLE;

typedef struct {
  long type;
  int info;
} mensaje;

void printTable(){
  for(int i = 0; i < 9; i++){
    printf("%d ", TABLE[i]);
  }
  printf("\n");
}

int main(){
  int fd;
  fd = shm_open(NAME, O_CREAT | O_RDWR, 0766);
  TABLE = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, fd, 0);

  int qid;

  qid = msgget(CLAVE, IPC_CREAT | IPC_EXCL | SHM_R | SHM_W);
  if (errno == EEXIST) {
    printf("Ya existe una cola de mensajes, accediendo...\n");
    qid = msgget(CLAVE, SHM_R | SHM_W);
  }

  mensaje msg;
  int turns = 2, idx;
  while(turns < 9){
    msgrcv(qid, &msg, sizeof(mensaje) - sizeof(long), RCV, 0);
    turns = msg.info;
    printTable();
    printf("Ingrese posicion (0-9): ");
    scanf("%d", &idx);
    TABLE[idx] = 'O';
    msg.info = turns++;
    msg.type = SND;
    msgsnd(qid, &msg, sizeof(mensaje) - sizeof(long), 0);
  }
}