#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define ll long long int
#define N 10

sem_t fullSlots, emptySlots;
sem_t mutexProd, mutexCons;

int buffer[N];
int in;
int P, C;

void insert(int element, int id){
  printf("? Produtor %d querendo produzir\n", id);
  sem_wait(&mutexProd);
  sem_wait(&emptySlots);

  printf("! Produtor %d produzindo na posição %d\n", id, in);
  buffer[in] = element;
  in = (in + 1) % N;

  sem_post(&fullSlots);
  sem_post(&mutexProd);
}

int* pop(int id){
  int *out = (int *)malloc(sizeof(int) * N);
  printf("> Consumidor %d querendo consumir\n", id);
  sem_wait(&mutexCons);
  for(int i = 0; i < N; i++){
    sem_wait(&fullSlots);
    out[i] = buffer[i];
    sem_post(&emptySlots);
  }
  printf("< Consumidor %d consumiu tudo\n", id);
  sem_post(&mutexCons);
  return out;
}

void *producer(void *arg){
  int id = (ll)arg;
  int element;
  while(1){
    printf("# Produtor %d criando coisas\n", id);
    element = id;
    insert(element, id);
  }
  pthread_exit(NULL);  
}

void *consumer(void *arg){
  int id = (ll)arg;
  int *elements;
  while(1){
    elements = pop(id);
    printf("$ Consumidor %d usando coisas\n", id);
  }
  pthread_exit(NULL);
}

pthread_t* createTid(int n) { 
  pthread_t* tid = (pthread_t *) malloc(sizeof(pthread_t) * n);
  if (tid == NULL) {
    printf("ERRO--createTid");
    exit(1);
  }
  return tid;
}

void createThreads(pthread_t* tid, int n, void *task) {
  for (ll i = 0; i < n; i++) {
    if (pthread_create(tid+i, NULL, task, (void *)i)) {
      printf("ERRO--createThreads\n");
      exit(2);
    }
  }
}

void waitThreads(pthread_t* tid, int n) {
  for (int i = 0; i < n; i++) {
    if (pthread_join(tid[i], NULL)) {
      printf("ERRO--pthread_join");
      exit(3);
    }
  }
}

void initArgs(int argc, char *argv[]){
  if(argc < 3){
    printf("Entre com %s <Num produtores> <Num consumidores>\n",argv[0]);
    exit(4);
  }

  P = atoi(argv[1]);
  C = atoi(argv[2]);

  in = 0;

  sem_init(&fullSlots, 0, 0);
  sem_init(&emptySlots, 0, N);

  sem_init(&mutexCons, 0, 1);
  sem_init(&mutexProd, 0, 1);
}

void run() {
  pthread_t* prod  = createTid(P);
  pthread_t* cons = createTid(C); 

  createThreads(prod, P, producer);
  createThreads(cons, C, consumer);
  
  waitThreads(prod, P);
  waitThreads(cons, C);
}

void endArgs(){
  sem_destroy(&fullSlots);
  sem_destroy(&emptySlots);
  sem_destroy(&mutexCons);
  sem_destroy(&mutexProd);
}

int main(int argc, char *argv[]){
  initArgs(argc, argv);
  run();
  endArgs();
  return 0;
}
