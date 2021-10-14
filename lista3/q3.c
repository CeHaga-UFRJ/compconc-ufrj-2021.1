#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

// Exclusao mutua de e, l & aux
sem_t mutexE, mutexL, mutexAux;

// Bloqueia leitores enquanto ha escritores
sem_t leitorBloq;

// Garante unicidade de escritores
sem_t bufferOcup;

int e, l, aux;

void entradaLeitor(){
  sem_wait(&mutexAux);
  sem_wait(&mutexE);
  // Confere se ha escritores
  if(e > 0){
    // Se sim, bloqueia e soma 1 na fila
    aux++;
    sem_post(&mutexE);
    sem_post(&mutexAux);
    sem_wait(&leitorBloq);
  }else{
    sem_post(&mutexE);
    sem_post(&mutexAux);
  }
  // Se chegou aqui pode ler
  // Adiciona um leitor e ocupa o buffer caso seja o primeiro
  sem_wait(&mutexL);
  l++;
  if(l == 1) sem_wait(&bufferOcup);
  sem_post(&mutexL); 
}

void saidaLeitor(){
  // Remove um leitor
  // Se for o ultimo libera o buffer tambem
  sem_wait(&mutexL);
  l--;
  if(l == 0) sem_post(&bufferOcup);
  sem_post(&mutexL);
}

void entradaEscritor(){
  // Adiciona um escritor
  sem_wait(&mutexE);
  e++;
  sem_post(&mutexE);

  // E ocupa o buffer
  sem_wait(&bufferOcup);
}

void saidaEscritor(){
  // Libera o buffer
  sem_post(&bufferOcup);

  sem_wait(&mutexAux);
  sem_wait(&mutexE);
  // Remove um escritor
  e--;
  // Se for o ultimo escritor libera cada leitor bloqueado
  if(e == 0 && aux > 0){
    while(aux--) sem_post(&leitorBloq);
    aux = 0;
  }
  sem_post(&mutexE);
  sem_post(&mutexAux);
}

void *leitor(void *args){
  entradaLeitor();
  // Realiza leitura
  saidaLeitor();
}

void *escritor(void *args){
  entradaEscritor();
  // Realiza escrita
  saidaEscritor();
}
