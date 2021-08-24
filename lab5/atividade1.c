#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MIN_RANDOM 0
#define MAX_RANDOM 9
#define ll long long int

int nthreads;
int *array;
int blocked;
int n_iter;

pthread_mutex_t mutex;
pthread_cond_t cond;

void barrierEnd(int idB){
  printf("============ FIM DA ITERAÇÃO ============\n");
}

void barrier(int nthreads, ll id, int idB){
  pthread_mutex_lock(&mutex);

  printf("! Thread %lld chegou na barreira %d\n",id, idB);

  if(blocked == nthreads-1){
    printf("> Thread %lld liberou a barreira %d\n",id, idB);
    if(idB == 2) barrierEnd(idB);
    pthread_cond_broadcast(&cond);
    blocked = 0;
  }else{
    printf("X Thread %lld ficou bloqueada na barreira %d\n",id, idB);
    blocked++;
    pthread_cond_wait(&cond, &mutex);
  }

  pthread_mutex_unlock(&mutex);
}

int randomInt(int min, int max){
  return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

int sumArray(int *array, int n){
  int total = 0;
  for(int i = 0; i < n; i++){
    total += array[i];
  }
  return total;
}

void* task(void* arg){
  int *sum = (int *)malloc(sizeof(int));
  *sum = 0;
  ll id = (ll) arg; 

  for(int i = 0; i < n_iter; i++){
    printf("@ Começou thread %lld na iteração %d\n", id, i+1);
    *sum += sumArray(array, nthreads);
    barrier(nthreads, id, 1);

    array[id] = randomInt(MIN_RANDOM, MAX_RANDOM);
    barrier(nthreads, id, 2);
  }

  pthread_exit((void *)sum);
}

int *createArray(int n){
  int *array = (int *)malloc(sizeof(int) * n);
  for(int i = 0; i < n; i++){
    array[i] = randomInt(MIN_RANDOM, MAX_RANDOM);
  }
  return array;
}

int main(int argc, char *argv[]){
  // Inicializacao
  if(argc < 3){
    printf("Utilização do programa:\n%s <número de threads> <número de iterações>\n",argv[0]);
    exit(1);
  }

  nthreads = atoi(argv[1]);
  n_iter = atoi(argv[2]);
  array = createArray(nthreads);
  blocked = 0;
  pthread_t tid[nthreads];
  int result[nthreads];

  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond, NULL);

  srand(time(NULL));

  // Criacao das threads
  for(ll i = 0; i < nthreads; i++){
    pthread_create(tid+i, NULL, task, (void *)i);
  }

  // Retorno das threads 
  for(int i = 0; i < nthreads; i++){
    void *sum;
    pthread_join(tid[i], &sum);
    result[i] = *(int *)sum;
  }

  // Verificacao do resultado
  printf("A thread 0 obteve resultado %d\n",result[0]);

  for(int i = 1; i < nthreads; i++){
    printf("A thread %d obteve resultado %d\n",i,result[i]);
    if(result[i] != result[i-1]){
      printf("Erro no resultado!!!\nAs threads %d e %d obtiveram valores diferentes\n", i-1, i);
      exit(2);
    }
  }

  printf("Programa finalizado com sucesso! :)\n");

  free(array);
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond);

  return 0;
}
