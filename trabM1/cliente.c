#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "banco.h"

void enviar(char *msg) {
    int fd = open(PIPE_NAME, O_WRONLY);
    if (fd == -1) {
        perror("Erro ao abrir pipe. O servidor está ligado?");
        exit(1);
    }
    printf("[CLIENTE] Enviou: %s\n", msg);
    //envia a mensagem até o pipe correto, possui o tamanho da mensagem nele, funciona como FIFO
    write(fd, msg, strlen(msg) + 1);
    close(fd);
}

int main() {
    printf("=== INICIANDO CLIENTE ===\n");

    // 1. Simulação de Inserção
    enviar("INSERT 101 Alice");

    // 2. Simulação de Seleção (Busca)
    enviar("INSERT 102 Bob_Engenharia");

    // 3. Simulação de Atualização
    enviar("INSERT 103 Charlie_Lite");

    // 4. Simulação de Deleção
    enviar("INSERT 104 David_Proa");

    // 5. Simulação de Seleção (Busca)
    enviar("SELECT 101");

    // 6. Simulação de Inserção
    enviar("SELECT 102");

    // 7. Simulação de Inserção
    enviar("SELECT 103");

    // 8. Simulação de Inserção
    enviar("SELECT 104");

    // 9. Simulação de Inserção
    enviar("UPDATE 101 Joaquina");

    // 9. Simulação de Inserção
    enviar("UPDATE 102 Mariazinha");

    // 9. Simulação de Inserção
    enviar("UPDATE 103 Analinda");

    // 9. Simulação de Inserção
    enviar("UPDATE 104 Amandinha");

    // 5. Simulação de Seleção (Busca)
    enviar("DELETE 101");

    // 5. Simulação de Seleção (Busca)
    enviar("DELETE 102");

    // 5. Simulação de Seleção (Busca)
    enviar("DELETE 103");

    // 5. Simulação de Seleção (Busca)
    enviar("DELETE 104");

    printf("=== TESTE FINALIZADO COM SUCESSO ===\n");
    return 0;
}