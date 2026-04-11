//Aqui é estruturado como é meu dado, que será
// recebido e interpretado pelo servidor

#ifndef BANCO_H
#define BANCO_H

//Configurações do sistema
#define PIPE_NAME "/tmp/bd_pipe"    // Caminho do Named Pipe (aquele que não vem de pai pra filho)
#define MAX_NOME 50                 // Tamanho máximo do campo nome
#define BUFFER_SIZE 256             // Tamanho do buffer de comunicação

//Estrutura de dados do banco Registro(id, nome[])
typedef struct {
    int id;
    char nome[MAX_NOME];
} Registro;

#endif
