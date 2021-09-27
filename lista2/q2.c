#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_cond_t cond; // condicao de ser multiplo de 100
pthread_mutex_t mutex;
long long int contador = 0;

void FazAlgo(int x){ }

void *T1 (void * arg){
  while(1){
    FazAlgo(contador);
    pthread_mutex_lock(&mutex); // Trava para poder usar a variavel global 
    contador++;
    if(contador % 100 == 0){ // Se for multiplo libera a condicao e espera
      pthread_cond_signal(&cond);
      pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);
  }
}

void *T2(void *arg){
  while (1){
    pthread_mutex_lock(&mutex); // Trava para poder usar a variavel global
    if(contador % 100 == 0){ // Se for multiplo imprime e libera a condicao pra T1 poder somar
      printf("%lld\n",contador);
      pthread_cond_signal(&cond);
      if(contador == 2000) exit(1);
    }
    pthread_cond_wait(&cond, &mutex); // Espera a condicao estar liberada novamente
    pthread_mutex_unlock(&mutex);
  }
}

int main(){
  pthread_t t1, t2;
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond, NULL);
  pthread_create(&t1, NULL, T1, NULL);
  pthread_create(&t2, NULL, T2, NULL);
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond);
}