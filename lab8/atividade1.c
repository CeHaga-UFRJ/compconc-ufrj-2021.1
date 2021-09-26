#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS 4

sem_t condA;
sem_t condB;
sem_t condD;

void *A(void *t){
  sem_wait(&condB);
  printf("Fique à vontade.\n");
  sem_post(&condA);
  pthread_exit(NULL);
}

void *B(void *t){
  printf("Seja bem-vinde!\n");
  sem_post(&condB);
  sem_post(&condB);
  pthread_exit(NULL);
}

void *C(void *t){
  sem_wait(&condD);
  sem_wait(&condA);
  printf("Volte sempre!\n");
  pthread_exit(NULL);
}

void *D(void *t){
  sem_wait(&condB);
  printf("Sente-se, por favor.\n");
  sem_post(&condD);
  pthread_exit(NULL);
}

pthread_t* createTid() {
  pthread_t* tid = (pthread_t *) malloc(sizeof(pthread_t) * NTHREADS);
  if (tid == NULL) {
    printf("ERRO--createTid");
    exit(1);
  }
  return tid;
}

pthread_t* init(){
  pthread_t *tid;
  tid = createTid();

  sem_init(&condA,0,0);
  sem_init(&condB,0,0);
  sem_init(&condD,0,0);

  return tid;
}

void taskStart(pthread_t *tid){
  pthread_create(tid, NULL, A, NULL);
  pthread_create(tid+1, NULL, B, NULL);
  pthread_create(tid+2, NULL, C, NULL);
  pthread_create(tid+3, NULL, D, NULL);

  for(int i = 0; i < NTHREADS; i++){
    pthread_join(tid[i], NULL);
  }
}

int main(){
  pthread_t *tid;

  tid = init();
  taskStart(tid);

  free(tid);
  sem_destroy(&condA);
  sem_destroy(&condB);
  sem_destroy(&condD);

  return 0;
}
