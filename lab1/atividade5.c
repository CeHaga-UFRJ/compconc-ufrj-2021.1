/*
* Atividade 5 para a disciplina de Computacao Concorrente
* Feito por Carlos Bravo, 2021.1
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 10000

typedef struct{
  int start;
  int end;
}targs;

int array[N];

// Inicializa o vetor
void initArray(){
  for(int i = 0; i < N; i++){
    array[i] = i;
  }
}

// Realiza uma repeticao para elevar ao quadrado
void *square(void *arg){
  targs args = *(targs *)arg;
  for(int i = args.start; i < args.end; i+=2){
    array[i] *= array[i];
  }
  free(arg);
  pthread_exit(NULL);
}

// Confere se o vetor esta certo
int check(){
  for(int i = 0; i < N; i++){
    // Se algum elemento nao for o quadrado, deu erro
    if(array[i] != i*i){
      return 0;
    }
  }
  return 1;
}

int main(){
  pthread_t tid[2]; // Um para cada thread

  initArray();

  for(int i = 0; i < 2; i++){
    // Define os argumentos
    targs *arg;
    arg = (targs *)malloc(sizeof(targs));
    if(!arg){
      printf("> Erro ao alocar memoria para arg, encerrando o programa\n");
      exit(1);
    }
    arg->start = i;
    arg->end = N;

    // E inicia as threads
    if(pthread_create(&tid[i], NULL, square, arg)){
      printf("> Erro ao criar a thread %d, encerrando o programa\n",i);
      exit(1);
    }else{
      printf("Thread %d criada com sucesso\n",i);
    }
  }

  // Espera as threads
  for(int i = 0; i < 2; i++){
    if(pthread_join(tid[i],NULL)){
      printf("> Erro ao unir a thread %d, encerrando o programa\n",i);
      exit(1);
    }else{
      printf("Thread %d finalizada com sucesso\n",i);
    }
  }

  int result = check();
  printf("\nPrograma finalizado, e o vetor esta %s\n",result?"certo":"errado");

  return 0;
}