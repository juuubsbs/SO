//Aqui é a nossa cabeça pensante, é quem recebe o dado via pipe
//e executa o registro/deleção/atualização no nosso arquivinho .txt

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>  //sys/ -> bibliotecas do so
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include "banco.h"     //nossa biblioteca com os defines

// Mutex global para controlar o acesso ao "banco"
pthread_mutex_t trava_banco = PTHREAD_MUTEX_INITIALIZER;

// Função que cada thread executa, vai descobrir oq queremos fazer tbm (ex. INCLUDE)
void* tratar_requisicao(void* arg) {
    char* comando = (char*)arg;

    // --- ZONA CRÍTICA --- (chamam assim pq mexe com o banco direto)
    pthread_mutex_lock(&trava_banco);
    
    printf("\n[THREAD] Iniciando processamento...\n");
    printf("[THREAD] Comando recebido: %s\n", comando);

    // Comparando a string recebida
    if (strncmp(comando, "INSERT", 6) == 0) {
        printf("[THREAD] Executando lógica de INSERÇÃO...\n");
        //aqui vão ser postas as funções de cada coisinha
    } 
    else if (strncmp(comando, "DELETE", 6) == 0) {
        printf("[THREAD] Executando lógica de REMOÇÃO...\n");
    }
    else if (strncmp(comando, "SELECT", 6) == 0) {
        printf("[THREAD] Executando lógica de BUSCA...\n");
    }
    else {
        printf("[THREAD] Comando não reconhecido: %s\n", comando);
    }
    
    // Simulando um tempo de processamento no banco
    sleep(2); 
    
    printf("[THREAD] Processamento finalizado.\n");
    
    pthread_mutex_unlock(&trava_banco);
    // --- FIM DA ZONA CRÍTICA ---

    free(comando); // Libera a memória alocada no main
    return NULL;
}

int main() {
    int fd; //file descriptor: numero para a thread
    char buffer[BUFFER_SIZE];

    // 1. Cria o Named Pipe (FIFO)
    // Se o arquivo já existir, ele não dá erro por causa da verificação
    mkfifo(PIPE_NAME, 0666);

    printf("=== SERVIDOR DE BANCO DE DADOS INICIALIZADO ===\n");
    printf("Aguardando comandos via Pipe: %s\n", PIPE_NAME); //trecho fofo pra mostrar o caminho do pipe

    //while(1) deixa ele sempre rodando
    while (1) {
        // 2. Abre o pipe para leitura
        // O servidor para aqui até que um cliente escreva algo
        fd = open(PIPE_NAME, O_RDONLY);
        
        //se ele recebe algo, executa
        if (read(fd, buffer, BUFFER_SIZE) > 0) {

            printf("\n[SERVIDOR] Dados recebidos no Pipe! Criando thread...\n");

            // Remove o '\n' se houver
            buffer[strcspn(buffer, "\n")] = 0;

            // Aloca memória para passar a string para a thread com segurança
            char* comando_para_thread = strdup(buffer);

            // 3. Criação da Thread
            pthread_t tid;
            if (pthread_create(&tid, NULL, tratar_requisicao, (void*)comando_para_thread) != 0) {
                perror("Erro ao criar thread");
            } else {
                // Detach permite que a thread se limpe sozinha ao terminar
                pthread_detach(tid);
            }
        }

        close(fd); // Fecha o descritor para a próxima leitura
    }

    return 0;
}