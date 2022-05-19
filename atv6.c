#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>

#define true 1

sem_t mutex, vazio, cheio;

int *buffer = NULL;
int limite = 0, tamBuffer = 0;
int entrada = 0, saida = 0;
int x = 0;

void *producer(void *arg);
void *consumer(void *arg);
int produce();
int consume();

int main() {
        int tamProd, tamCons;
        scanf("%d %d %d %d", &tamProd, &tamCons, &limite, &tamBuffer);

        sem_init(&mutex, 0, 1);
        sem_init(&vazio, 0, tamBuffer);
        sem_init(&cheio, 0, 0);

        buffer = calloc(tamBuffer, sizeof(int));

        pthread_t produtores[tamProd];
		pthread_t consumidores[tamCons];

        int prodId[tamProd];
        for (int i = 0; i < tamProd; i++)
                prodId[i] = i;

        int consId[tamCons];
        for (int i = 0; i < tamCons; i++)
                consId[i] = i;

        for (int i = 0; i < tamProd; i++) {
                if(pthread_create(&produtores[i], NULL, producer, &prodId[i]))
                    printf("ERRO");
        }

        for (int i = 0; i < tamCons; i++) {
                if(pthread_create(&consumidores[i], NULL, consumer, &consId[i]))
                    printf("ERRO");
        }

        for (int i = 0; i < tamProd; i++)
                pthread_join(produtores[i], NULL);

        for (int i = 0; i < tamCons; i++)
                pthread_join(consumidores[i], NULL);
        
        free(buffer);
        return 0;
}

void *producer(void *arg) {
        int *prodId = arg;

        while (true) {
                int produzido = produce();
                x = (x+1)%(limite+1);

                sem_wait(&vazio);
                sem_wait(&mutex);

                buffer[entrada] = produzido;
                printf("Produtor %d produzindo %d na posicao %d\n", *prodId, produzido, entrada);

                entrada = (entrada + 1)%tamBuffer;

                sem_post(&cheio);
                sem_post(&mutex);
        }  
}

void *consumer(void *arg) {
        int *consId = arg;

        while (true) {
                sem_wait(&cheio);
                sem_wait(&mutex);

                int consumido = consume();
                printf("Consumidor %d consumindo %d da posicao %d\n", *consId, consumido, saida);
                saida = (saida + 1)%tamBuffer;

                sem_post(&vazio);
                sem_post(&mutex);
        }
}

int produce() {
        return (2 * x) + 1;
}

int consume() {
        int consumido = buffer[saida];
        buffer[saida] = NULL;

        return consumido;
}
