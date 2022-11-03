#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>

#define CLAVE 100

int main(){
  int qid;
  
  qid = msgget(CLAVE, IPC_CREAT | IPC_EXCL | SHM_R | SHM_W);
  return 0;
}
