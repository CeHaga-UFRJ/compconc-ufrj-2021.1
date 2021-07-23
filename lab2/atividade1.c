#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"
// Usando a formula A.B=C
float *A;
float *B;
float *C;

int dim;
int nthreads;

// Inicializa matrizes com valores definidos
float *startMatrix(int value){
  float *matriz = (float *)malloc(sizeof(float)*dim*dim);
  if(!matriz){
    printf("Erro ao alocar matriz\n");
    exit(2);
  }
  for(int i = 0; i < dim; i++){
    for(int j = 0; j < dim; j++){
      matriz[dim*i + j] = value;
    }
  }
  return matriz;
}

// Retorna o resultado de uma celula x,y
float multiplyCell(int x, int y){
  float res = 0;
  for(int i = 0; i < dim; i++){
    res += A[x*dim + i] * B[i*dim + y];
  }
  return res;
}

// Percorre a matriz realizando a multiplicacao
void *task(void *arg){
  int id = *(int *)arg;
  for(int i = id; i < dim; i+= nthreads){
    for(int j = 0; j < dim; j++){
      C[i*dim + j] = multiplyCell(i,j);
    }
  }
  free(arg);
}

// Confere o resultado da matriz
int check(){
  // Como os valores sao sempre os mesmos, o resultado deve ser o mesmo
  for(int i = 0; i < dim; i++){
    for(int j = 0; j < dim; j++){
      if(C[i*dim + j] != dim) return 0;
    }
  }
  return 1;
}

int main(int argc, char **argv){
  double start, end, delta;

  GET_TIME(start);
  // Le argumentos da entrada
  if(argc < 3){
    printf("Entre com dimensão da matriz e número de threads\n");
    exit(1);
  }
  dim = atoi(argv[1]);
  nthreads = atoi(argv[2]);

  // Realiza verificacoes
  if(dim <= 0 || nthreads <= 0){
    printf("Entre com números positivos\n");
    exit(1);
  }
  if(nthreads > dim){
    nthreads = dim;
  }

  // Inicializa ids das threads
  pthread_t *tid = (pthread_t *)malloc(sizeof(pthread_t) * dim);
  if(!tid){
    printf("Erro ao alocar ids\n");
    exit(2);
  }

  // Inicializa matrizes
  A = startMatrix(1);
  B = startMatrix(1);
  C = startMatrix(0);

  GET_TIME(end);
  delta = end - start;
  printf("Inicializacao levou %f ms\n",delta);
  GET_TIME(start);

  // Inicializa threads
  for(int i = 0; i < nthreads; i++){
    int *arg = (int *)malloc(sizeof(int));
    if(!arg){
      printf("Erro ao alocar argumento\n");
      exit(2);
    }
    *arg = i;
    if(pthread_create(tid+i, NULL, task, arg)){
      printf("Erro ao criar threads\n");
      exit(3);
    }
  }

  // Espera as threads
  for(int i = 0; i < nthreads; i++){
    if(pthread_join(tid[i], NULL)){
      printf("Erro au juntar threads\n");
      exit(3);
    }
  }


  GET_TIME(end);
  delta = end - start;
  printf("Multiplicacao levou %f ms\n",delta);
  GET_TIME(start);

  printf("O resultado esta %s\n",check()?"certo":"errado");

  // Liberacao de memoria
  free(A);
  free(B);
  free(C);
  free(tid);

  GET_TIME(end);
  delta = end - start;
  printf("Finalizacao levou %f ms\n",delta);

  return 0;
}