#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

void  ClientProcess(int SharedMem[]);

void deposit(int* bankAccount){
  
  int localBalance = *bankAccount;
  
  int amount = rand() % 101;
 
  if (amount%2 == 0){
    
    localBalance+= amount;
    printf("Dear Old Dad: Deposited $%d / Balance = $%d\n", amount, localBalance);
    *bankAccount = localBalance;
  
  }else{
    printf("Dear Old Dad: Doesn't have any money to give\n");
  }
}

void withdraw(int* bankAccount){
  
  int localBalance = *bankAccount;
  
  int needs = rand() % 51;
  
  printf("Poor Student needs $%d\n", needs);
  if (needs <= localBalance){
    localBalance-=needs;
   
    printf("Poor Student: Withdrew $%d / Balance = $%d\n", needs, localBalance);
    *bankAccount = localBalance;
  
  }else{
    ("Poor Student: Not Enough Cash ($%d)\n", localBalance);
  }
}

int  main(int  argc, char *argv[])
{
     int    ShmID;
     int    *ShmPTR;
     pid_t  pid;
     int    status;
     int forward;
     int none_0 = 0;
     int *c;
     int i;
     sem_t *sPoint;

     forward= open("log.txt",O_RDWR|O_CREAT,S_IRWXU);
     write(forward,&none_0,sizeof(int));
     c = mmap(NULL,sizeof(int),PROT_READ |PROT_WRITE,MAP_SHARED,forward,0);
     close(forward);
     if ((sPoint = sem_open("semaphore", O_CREAT, 0644, 1)) == SEM_FAILED) {
       perror("semaphore initilization");
       exit(1);
     }
     
     pid = fork();
     
     if (pid > 0) {
        
      for (i = 0; i>-1; i++){
            sleep(rand()%6);
           
          printf("Dear Old Dad: Attempting to Check Balance\n");\
            sem_wait(sPoint);
            
          int randomNumber = rand()%101;
            
          if (randomNumber % 2 == 0){
              if (*c<100){
                deposit(c);
              } else {
                printf("Dear Old Dad: Thinks Student has enough Cash ($%d)\n", *c);
              }
            
            }else{
              printf("Dear Old Dad: Last Checking Balance = $%d\n", *c);
            }
            sem_post(sPoint);              
        }
     exit(1);
     }
     else if (pid == 0){
       for (i = 0; i>-1; i++){
            sleep(rand()%6);
            printf("Poor Student: Attempting to Check Balance\n");  
            sem_wait(sPoint);
            int randomNumber = rand();
            if (randomNumber%2 == 0){
              withdraw(c);
            }else{
              printf("Poor Student: Last Checking Balance = $%d\n", *c);
            }
            sem_post(sPoint);
            
        }
        printf("   Client is about to exit\n");
        exit(0);
     }
     wait(&status);
     printf("Server has detected the completion of its child...\n");
     shmdt((void *) ShmPTR);
     printf("Server has detached its shared memory...\n");
     shmctl(ShmID, IPC_RMID, NULL);
     printf("Server has removed its shared memory...\n");
     printf("Server exits...\n");
     exit(0);
}

void  ClientProcess(int  SharedMem[])
{
     printf("   Client process started\n");
     printf("   Client found %d %d %d %d in shared memory\n",
                SharedMem[0], SharedMem[1], SharedMem[2], SharedMem[3]);
     printf("   Client is about to exit\n");
}