#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include "banco.h"

// --- Infraestrutura da Fila ---
Tarefa fila_tarefas[MAX_FILA]; //array de comandos brutos
int contador_tarefas = 0;
pthread_mutex_t mutex_fila = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_fila = PTHREAD_COND_INITIALIZER;

// Mutex para o Banco (Arquivo)
pthread_mutex_t trava_banco = PTHREAD_MUTEX_INITIALIZER;

//função que identifica o comando inserido (somente a palavra chave)
TipoOperacao identificar_comando(char* comando_bruto) {
    char cmd[20];
    if (sscanf(comando_bruto, "%s", cmd) <= 0) return OP_INVALIDA;

    if (strcmp(cmd, "INSERT") == 0) return OP_INSERT;
    if (strcmp(cmd, "DELETE") == 0) return OP_DELETE;
    if (strcmp(cmd, "UPDATE") == 0) return OP_UPDATE;
    if (strcmp(cmd, "SELECT") == 0) return OP_SELECT;
    
    return OP_INVALIDA;
}

void* worker_thread(void* arg) {
    while (1) {
        Tarefa tarefa_local;

        // 1. ESPERA POR TRABALHO (USANDO O MUTEX DA FILA)
        pthread_mutex_lock(&mutex_fila);
        while (contador_tarefas == 0) {
            pthread_cond_wait(&cond_fila, &mutex_fila);
        }

        // Retira da fila (sempre o primeiro da fila)
        tarefa_local = fila_tarefas[0];
        for (int i = 0; i < contador_tarefas - 1; i++) {
            fila_tarefas[i] = fila_tarefas[i + 1];
        }
        contador_tarefas--;

        pthread_mutex_unlock(&mutex_fila);

        printf("[N. %lu] recebeu um comando...\n", (unsigned long)pthread_self());

        // 2. PROCESSAMENTO
        TipoOperacao op = identificar_comando(tarefa_local.comando_bruto);
        char *saveptr;
        strtok_r(tarefa_local.comando_bruto, " ", &saveptr); // Pula o comando

        pthread_mutex_lock(&trava_banco); // Trava o arquivo
        
        switch (op) {
            case OP_INSERT: {
                char *id_s = strtok_r(NULL, " ", &saveptr);
                char *nome = strtok_r(NULL, " ", &saveptr);
                printf("[THREAD] Inserindo ID %s Nome %s\n", id_s, nome);
                // chamar_funcao_insert(atoi(id_s), nome);
                break;
            }
            case OP_DELETE: {
                char *id_s = strtok_r(NULL, " ", &saveptr);
                printf("[THREAD] Removendo ID %s\n", id_s);
                // chamar_funcao_delete(atoi(id_s), nome);
                break;
            }
            case OP_UPDATE: {
                char *id_s = strtok_r(NULL, " ", &saveptr);
                char *nome = strtok_r(NULL, " ", &saveptr);
                printf("[THREAD] Atualizando ID %s para Nome %s\n", id_s, nome);
                // chamar_funcao_update(atoi(id_s), nome);
                break;
            }
            case OP_SELECT: {
                char *param = strtok_r(NULL, " ", &saveptr);
                printf("[THREAD] Selecionando: %s\n", param);
                // chamar_funcao_select(atoi(id_s), nome);
                break;
            }
            default:
                printf("[THREAD] Comando inválido.\n");
        }
        sleep(1);
        pthread_mutex_unlock(&trava_banco); // Libera o arquivo
    }
    return NULL;
}

int main() {
    int fd;
    char buffer[BUFFER_SIZE];
    pthread_t pool[NUM_THREADS];

    mkfifo(PIPE_NAME, 0666);

    // 3. INICIALIZA A POOL DE THREADS
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&pool[i], NULL, worker_thread, NULL);
    }

    printf("=== SERVIDOR INICIALIZADO ===\n");

    while (1) {
        // O servidor trava aqui esperando alguém abrir o pipe para escrita
        fd = open(PIPE_NAME, O_RDONLY); 
        
        if (fd != -1) {
            ssize_t bytes_lidos;

            // Limpamos o buffer para evitar lixo de memória
            memset(buffer, 0, BUFFER_SIZE); 

            // Realiza UMA única leitura do que estiver disponível no pipe 
            bytes_lidos = read(fd, buffer, BUFFER_SIZE);

            if (bytes_lidos > 0) {
                int inicio = 0;
                for (int i = 0; i < bytes_lidos; i++) {
                    // Fatiamento baseado no caractere nulo enviado pelo cliente
                    if (buffer[i] == '\0') {
                        char* msg_atual = &buffer[inicio];
                    
                        // --- INSERÇÃO NA FILA ---
                        pthread_mutex_lock(&mutex_fila);
                        if (contador_tarefas < MAX_FILA) {
                            strncpy(fila_tarefas[contador_tarefas].comando_bruto, msg_atual, BUFFER_SIZE);
                            contador_tarefas++;
                            pthread_cond_signal(&cond_fila);
                        }
                        else {
                            printf("[FILA] Fila cheia! Comando: %s perdido\n", msg_atual);
                        }
                        pthread_mutex_unlock(&mutex_fila);

                        inicio = i + 1;
                    }
                }
            }
            // Fecha o descritor imediatamente após ler, voltando para o topo do while(1)
            close(fd); 
        }
    }

    return 0;
}