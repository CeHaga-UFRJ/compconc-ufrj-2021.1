#include <stdio.h> 
#include <stdlib.h>
#include <pthread.h>
#include "../lib/timer.h"

#define ll long long int
#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)>(b)?(b):(a))

float *array;
int nthreads;
ll dim;

typedef struct{
  float maxValue;
  float minValue;
} minMax;

// Gera número aleatório
float randomDouble(){
  return 1000.0 * (float)rand() / (float)RAND_MAX;
}

// Inicializa array de tamanho dim com valores aleatórios
float *initArray(ll dim){
  float *array = (float *)malloc(sizeof(float) * dim);
  if(!array){
    fprintf(stderr, "Erro ao alocar vetor\n");
    exit(2);
  }
  for(ll i = 0; i < dim; i++){
    array[i] = randomDouble();
  }
  return array;
}

// Algoritmo sequencial
minMax sequentialTask(ll dim){
  minMax values;
  values.maxValue = array[0];
  values.minValue = array[0];
  for(ll i = 1; i < dim; i++){
    values.maxValue = max(values.maxValue, array[i]);
    values.minValue = min(values.minValue, array[i]);
  }
  return values;
}

// Algoritmo concorrente
void *task(void *args){
  ll id = (ll) args;
  minMax *values;
  values = (minMax *)malloc(sizeof(minMax));
  if(!values){
    fprintf(stderr, "Erro ao alocar minmax\n");
    exit(3);
  }
  values->maxValue = array[id];
  values->minValue = array[id];
  for(ll i = id; i < dim; i += nthreads){
    values->maxValue = max(values->maxValue, array[i]);
    values->minValue = min(values->minValue, array[i]);
  }
  pthread_exit((void *)values);
}

int main(int argc, char *argv[]){
  double start, end, delta;
  double totalConc = 0;
  double totalSeq = 0;
  int totalExec = 5;

  GET_TIME(start);
  // Leitura e verificacao dos parametros
  if(argc < 3){
    fprintf(stderr, "Entre com a dimensão do vetor e o número de threads\n");
    exit(1);
  }
  dim = atoll(argv[1]);
  nthreads = atoi(argv[2]);
  if(dim <= 0 || nthreads <= 0){
    fprintf(stderr, "Entre com números positivos\n");
    exit(1);
  }
  if(nthreads > dim) nthreads = dim;

  // Inicializacao das estruturas e variaveis
  pthread_t tid [nthreads];
  array = initArray(dim);
  minMax seqValues;
  minMax conValues;

  GET_TIME(end);
  delta = end - start;
  fprintf(stderr, "Inicialização levou %lf s\n", delta);
  GET_TIME(start);

  for(int t = 0; t < totalExec; t++){
    // Algoritmo sequencial
    seqValues = sequentialTask(dim);

    GET_TIME(end);
    delta = end - start;
    fprintf(stderr, "Sequencial %d levou %lf s\n", t, delta);
    totalSeq += delta;
    GET_TIME(start);

    // Algoritmo concorrente
    for(ll i = 0; i < nthreads; i++){
      if(pthread_create(tid+i, NULL, task, (void *) i)){
        fprintf(stderr, "Erro ao criar threads\n");
        exit(4);
      }
    }

    for(int i = 0; i < nthreads; i++){
      minMax *result;
      if(pthread_join(tid[i],(void**) &result)){
        fprintf(stderr, "Erro ao unir threads\n");
        exit(4);
      }
      if(i != 0){
        conValues.maxValue = max(conValues.maxValue, result->maxValue);
        conValues.minValue = min(conValues.minValue, result->minValue);
      }else{
        conValues.maxValue = result->maxValue;
        conValues.minValue = result->minValue;
      }
      free(result);
    }


    GET_TIME(end);
    delta = end - start;
    fprintf(stderr, "Concorrente %d levou %lf s\n", t, delta);
    totalConc += delta;
    GET_TIME(start);

    // Corretude
    if(seqValues.minValue == conValues.minValue && seqValues.maxValue == conValues.maxValue){
      fprintf(stderr, "Certo (%d)\n",t);
    }else{
      fprintf(stderr, "Errado (%d)\n",t);
    }
  }

  // Finalizacao
  free(array);

  totalConc /= totalExec;
  totalSeq /= totalExec;
  printf("Média Sequencial: %f\nMédia Concorrente: %f\nAceleração: %f\n",totalSeq, totalConc, totalSeq/totalConc);

  GET_TIME(end);
  delta = end - start;
  fprintf(stderr, "Finalização levou %lf s\n", delta);

  return 0;
}
