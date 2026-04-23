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
    enviar("INSERT 10 Aluno_Univali");

    // 2. Simulação de Seleção (Busca)
    enviar("SELECT 10");

    // 3. Simulação de Atualização
    enviar("UPDATE 10 Novo_Nome_Editado");

    // 4. Simulação de Deleção
    enviar("DELETE 10");

    // 5. Simulação de Inserção
    enviar("INSERT 10 Aluno_Univali");

    // 6. Simulação de Seleção (Busca)
    enviar("SELECT 10");

    // 7. Simulação de Atualização
    enviar("UPDATE 10 Novo_Nome_Editado");

    // 8. Simulação de Deleção
    enviar("DELETE 10");

    // 9. Simulação de Inserção
    enviar("INSERT 10 Aluno_Univali");

    // 10. Simulação de Seleção (Busca)
    enviar("SELECT 10");

    // 11. Simulação de Atualização
    enviar("UPDATE 10 Novo_Nome_Editado");

    // 12. Simulação de Deleção
    enviar("DELETE 10");

    // 13. Simulação de Inserção
    enviar("INSERT 10 Aluno_Univali");

    // 14. Simulação de Seleção (Busca)
    enviar("SELECT 10");

    // 15. Simulação de Atualização
    enviar("UPDATE 10 Novo_Nome_Editado");

    // 16. Simulação de Deleção
    enviar("DELETE 10");

    printf("=== TESTE FINALIZADO COM SUCESSO ===\n");
    return 0;
}