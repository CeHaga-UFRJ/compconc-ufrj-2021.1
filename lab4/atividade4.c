#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 4

int counter = 0;
pthread_mutex_t mutex;
pthread_cond_t cond1;
pthread_cond_t cond2;

void *A(void *t){
  pthread_mutex_lock(&mutex);
  if(counter == 0){
    pthread_cond_wait(&cond1, &mutex);
  }
  printf("Fique à vontade.\n");
  counter++;
  pthread_cond_signal(&cond2);
  pthread_mutex_unlock(&mutex);

  pthread_exit(NULL);
}

void *B(void *t){
  printf("Seja bem-vinde!\n");

  pthread_mutex_lock(&mutex);
  counter++;
  pthread_cond_broadcast(&cond1);
  pthread_mutex_unlock(&mutex);

  pthread_exit(NULL);
}

void *C(void *t){
  pthread_mutex_lock(&mutex);
  while(counter != 3){
    pthread_cond_wait(&cond2, &mutex);
  }
  printf("Volte sempre!\n");
  pthread_mutex_unlock(&mutex);

  pthread_exit(NULL);
}

void *D(void *t){
  pthread_mutex_lock(&mutex);
  if(counter == 0){
    pthread_cond_wait(&cond1, &mutex);
  }
  printf("Sente-se, por favor.\n");
  counter++;
  pthread_cond_signal(&cond2);
  pthread_mutex_unlock(&mutex);

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

  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond1, NULL);
  pthread_cond_init(&cond2, NULL);

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
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond1);
  pthread_cond_destroy(&cond2);

  return 0;
}
