/* 1 - Menu:
   a. Incluir Pessoa (codigo, nome, altura)
   b. Mostrar Pessoas cadastradas
   c. Incluir Histórico de pesos (dia, kg)
   d. Mostrar histórico de IMCs (de uma pessoa)
2 - Os dados de pessoa devem ser armazenados em um arquivo CSV denominado pessoa.csv  (codigo; nome; altura)
3 - Os pesos devem ser armazenados em um arquivo CSV denominado pesos.csv (codigo; data; peso)
4 - Os arquivos devem ser criados, caso não existam, ou lidos caso existam. Os dados devem ser persistidos (gravados) a cada inclusão, e lidos diretamente do arquivo a cada operação Mostrar. */

// Bibliotecas usadas

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>

// Valores pre-compilados (altere por usa conta e risco!)
// Esses valores podem ser modificados de acordo conforme a conveniência

#define ARQ_PESSOA "pessoa.csv"         // Nome do arquivo que vai armazenar as pessoas, mantenha a extensão .csv no final
#define ARQ_PESO "pesos.csv"            // Nome do arquivo que vai armazenar os históricos, incluindo peso, mantenha a extensão .csv no final
#define QUANTIDADE_MAX_HISTORICO 100    // Quantidade máxima de histórico a ser armazenado
#define TAMANHO_MAX_NOME 50             // Quantidade máxima de carateres para o nome da pessoa
#define QUANTIDADE_MAX_PESSOAS 1000     // Quantidade máxima de pessoas que um arquivo armazenará
#define BUFFER_LEITURA_LINHA 256        // Tamanho máximo do buffer de leitura de uma linha

// Estrutura dos dados
// Historico inclui dia, peso e o IMC, pois precisa do peso e da altura
typedef struct {
    int dia;
    float peso;
    float IMC;
} Historico;

typedef struct {
    int codigo;
    char nome[TAMANHO_MAX_NOME];
    float altura;
    Historico history[QUANTIDADE_MAX_HISTORICO];
} Pessoa;

// Struct global das pessoas
Pessoa pessoas[QUANTIDADE_MAX_PESSOAS];

// Procedimentos para abrir e fechar arquivos com tratamento de erros
void abrirOuCriarArquivo(FILE **arq, const char *nomeArq){
    *arq = fopen(nomeArq, "r+");

    if(*arq == NULL){ // Verifica se existe, se não, cria o arquivo
        printf("Arquivo '%s' não existe!\n", nomeArq);
        printf("Criando arquivo...\n");

        *arq = fopen(nomeArq, "w");
        if(*arq == NULL){
            printf("ERRO ao criar o arquivo '%s'!\n", nomeArq);
            printf("Código errno: %d\n", errno);
            printf("Descrição: %s\n", strerror(errno));
            return;
        }

        printf("Arquivo criado com sucesso!\n");
    } else {
        printf("Abrindo '%s' para a leitura...\n", nomeArq);
    }
    fclose(*arq);
}

// Validação da entrada Pessoa
int validarPessoa(Pessoa *P){
    if(!P){
        printf("ERRO: Pessoa inválida (NULL)!\n");
        return 0;
    }

    if(P->codigo <= 0){
        printf("ERRO: Código inválido!\n");
        return 0;
    }
    
    FILE *arqtemp = fopen(ARQ_PESSOA, "r");
    char linha[BUFFER_LEITURA_LINHA];
    Pessoa temp;
    rewind(arqtemp);

    while(fgets(linha, sizeof(linha), arqtemp)){
        linha[strcspn(linha, "\n")] = 0;

        if(sscanf(linha, "%d,%49[^,],%f", &temp.codigo, temp.nome, &temp.altura) == 3) {
            if(P->codigo == temp.codigo){
                printf("ERRO: Código já atribuido a outra pessoa!\n");
                return 0;
            }
        }
    }

    fclose(arqtemp);

    if(strlen(P->nome) > TAMANHO_MAX_NOME){
        printf("ERRO: Nome maior que %d carateres!\n", TAMANHO_MAX_NOME);
        return 0;
    }

    if(P->altura <= 0.0f){
        printf("ERRO: Altura indefinida!\n");
        return 0;
    }

    return 1;
}

// Validação da entrada Peso
int validarHistorico(Historico *H){
    if(!H){
        printf("ERRO: Horário inválido! (NULL)\n");
        return 0;
    }

    if(H->dia <= 0 || H->dia > 31){
        printf("ERRO: Dia inválido!\n");
        return 0;
    }

    if(H->peso <= 0){
        printf("ERRO: Peso inválido!\n");
        return 0;
    }

    return 1;
}

// Procedimento para criar uma pessoa
void incluirPessoa(FILE **arq){
    *arq = fopen(ARQ_PESSOA, "a+"); // Abre em modo append (acréscimo)

    if(!arq){
        printf("ERRO ao abrir '%s'!\n", ARQ_PESSOA);
        return;
    }
    
    Pessoa P;

    printf("--- ADICIONANDO PESSOA ---\n");

    do{
        printf("Digite o código: ");
        scanf("%d", &P.codigo);
        getchar();

        printf("Digite o nome: ");
        fgets(P.nome, sizeof(P.nome), stdin);
        P.nome[strcspn(P.nome, "\n")] = '\0';

        printf("Digite a altura: ");
        scanf("%f", &P.altura);
        getchar();
    } while(!validarPessoa(&P));

    fprintf(*arq, "%d,%s,%.2f\n", P.codigo, P.nome, P.altura);

    printf("'%s' adicionado(a)!\n", P.nome);

    fclose(*arq);
}

// Procedimento para mostrar pessoas cadastradas
void mostrarPessoas(FILE **arq, Pessoa *pessoas, int *qtd){
    *arq = fopen(ARQ_PESSOA, "r");
    if(!arq){
        printf("ERRO: Falha ao abrir '%s'!\n", ARQ_PESSOA);
        return;
    }

    char linha[BUFFER_LEITURA_LINHA];
    *qtd = 0;

    printf("========================== PESSOAS ===========================\n");
    while(fgets(linha, sizeof(linha), *arq) && *qtd < QUANTIDADE_MAX_PESSOAS){
        linha[strcspn(linha, "\n")] = 0;  // remove o '\n' do final

        Pessoa P;
        if(sscanf(linha, "%d,%49[^,],%f", &P.codigo, P.nome, &P.altura) == 3) {
            pessoas[*qtd] = P;
            printf("| ID: %-6d | Nome: %-20s | Altura: %-6.2f |\n", P.codigo, P.nome, P.altura);
            (*qtd)++;
        }
    }
    printf("==============================================================\n");

    fclose(*arq);
}

// Procedimento para listar as pessoas cadastradas, apenas ID e nome
void listarPessoas(FILE **arq, Pessoa *pessoas, int *qtd){
    *arq = fopen(ARQ_PESSOA, "r");

    char linha[BUFFER_LEITURA_LINHA];
    *qtd = 0;

    printf("====================== LISTA DE PESSOAS ======================\n");
    while (fgets(linha, sizeof(linha), *arq) && *qtd < QUANTIDADE_MAX_PESSOAS) {
        linha[strcspn(linha, "\n")] = 0;  // remove o '\n' do final

        Pessoa p;
        if (sscanf(linha, "%d,%49[^,],%f", &p.codigo, p.nome, &p.altura) == 3) {
            pessoas[*qtd] = p;
            printf("| ID: %-15d Nome: %-45s |\n", p.codigo, p.nome);
            (*qtd)++;
        }
    }
    printf("==============================================================\n");

    fclose(*arq);
}

// Função para selecionar pessoa por ID
void selecionarPessoa(FILE **arq, Pessoa *escolhida){
    *arq = fopen(ARQ_PESSOA, "r");
    if(*arq == NULL) {
        printf("Erro ao abrir o arquivo '%s'!\n", ARQ_PESSOA);
        return;
    }

    int id;
    int ehValido = 0;
    char linha[BUFFER_LEITURA_LINHA];
    Pessoa temp;
    
    do{
        printf("Digite o ID da pessoa que deseja selecionar: ");
        scanf("%d", &id);

        while(fgets(linha, sizeof(linha), *arq)){
            linha[strcspn(linha, "\n")] = 0;

            if(sscanf(linha, "%d,%49[^,],%f", &temp.codigo, temp.nome, &temp.altura) == 3){
                if(temp.codigo == id){
                    *escolhida = temp;  // Copia a pessoa encontrada para um struct local
                    ehValido = 1;
                    break; // Sai do loop imediatamente se encontrou
                }
            }
        }
        
        if(!ehValido){
            printf("Pessoa com ID %d não encontrada!\n", id);
            printf("Tente digitar um id válido!\n");
        }
    } while(!ehValido);

    fclose(*arq);
}

// Procedimento para incluir histórico de pesos
void incluirHistorico(FILE **arq, Pessoa *P){
    *arq = fopen(ARQ_PESO, "a+");
    if(*arq == NULL){
        printf("ERRO ao abrir '%s'!", ARQ_PESO);
        perror("Erro");
        return;
    }
    
    Historico *H = P->history; // ponteiro para o array de históricos da pessoa
    int continuar = 1;

    // Procedimento para obter o último ID preenchido
    int idUltimoHist = 0;
    char linha[BUFFER_LEITURA_LINHA];

    while(fgets(linha, sizeof(linha), *arq)){
        int idPessoa, idHist;
        if(sscanf(linha, "%d,%d", &idPessoa, &idHist) == 2 && idPessoa == P->codigo){
            if(idHist > idUltimoHist){
                idUltimoHist = idHist;
            }
        }
    }

    printf("--- ADICIONANDO HISTÓRICO DE '%s' (ID %d) ---\n", P->nome, P->codigo);

    rewind(*arq);
    for(int i=0; i<QUANTIDADE_MAX_HISTORICO && continuar; i++){
        do{
            printf("Digite o dia: ");
            scanf("%d", &H[i].dia);
            getchar();

            printf("Digite o peso: ");
            scanf("%f", &H[i].peso);
            getchar();
        } while(!validarHistorico(&H[i]));

        // Cálculo de IMC
        H[i].IMC = (H[i].peso / (P->altura * P->altura));

        int idHistorico = ++idUltimoHist; // incrementa a partir do último encontrado

        fprintf(*arq, "%d,%d,%d,%.2f,%.2f\n", P->codigo, idUltimoHist, H[i].dia, H[i].peso, H[i].IMC);

        printf("Histórico %d adicionado para '%s'.\n", idHistorico, P->nome);

        do{
            printf("Adicionar mais um histórico? (0 = não, 1 = sim): ");
            if(scanf("%d", &continuar) != 1 || (continuar != 0 && continuar != 1)){
                printf("ERRO: digite apenas 0 ou 1.\n");
                while (getchar() != '\n'); // limpa o buffer
                continuar = -1;
            }
        } while(continuar != 0 && continuar != 1);
    }

    fclose(*arq);
}

// Procedimento para mostrar histórico
void mostrarHistorico(FILE **arq, Pessoa *P){
    *arq = fopen(ARQ_PESO, "r");
    if (*arq == NULL) {
        printf("ERRO: não foi possível abrir '%s'\n", ARQ_PESO);
        return;
    }

    char linha[BUFFER_LEITURA_LINHA];
    int qtd = 0;
    int codigoArquivo, idHistorico, dia;
    float peso, imc;

    printf("==============================================================\n");
    printf("| ID: %-10d | Nome: %-40s |\n", P->codigo, P->nome);
    printf("--------------------------------------------------------------\n");
    printf("| %-15s | %-10s | %-15s | %-10s |\n", "Histórico", "Dia", "Peso (kg)", "IMC");
    printf("--------------------------------------------------------------\n");

    while(fgets(linha, sizeof(linha), *arq) && qtd < QUANTIDADE_MAX_HISTORICO){
        linha[strcspn(linha, "\n")] = 0;  // remove o '\n'

        // Lê o arquivo de pesos
        if(sscanf(linha, "%d,%d,%d,%f,%f", &codigoArquivo, &idHistorico, &dia, &peso, &imc) == 5) {
            if(codigoArquivo == P->codigo) { // Verifica se o que foi lido é equivalente ao struct local da pessoa escolhida
                printf("| %-15d | %-10d | %-15.2f | %-10.2f |\n", idHistorico, dia, peso, imc);
                qtd++;
            }
        }
    }

    if(qtd == 0){
        printf("Nenhum histórico encontrado para %s.\n", P->nome);
    }

    printf("==============================================================\n");

    fclose(*arq);
}

int main(void){
    FILE *arqPessoa, *arqPeso;
    Pessoa escolhida;
    int qtd = 0;

    abrirOuCriarArquivo(&arqPessoa, ARQ_PESSOA);
    abrirOuCriarArquivo(&arqPeso, ARQ_PESO);

    char x;
    do{
        printf("============================ MENU ============================\n");
        printf("[a] ---> Incluir Pessoa (codigo, nome, altura)\n");
        printf("[b] ---> Mostrar Pessoas cadastradas\n");
        printf("[c] ---> Incluir Histórico de pesos para Pessoa (dia, kg)\n");
        printf("[d] ---> Mostrar histórico de IMCs de Pessoa\n");
        printf("[0] ---> Finalizar programa\n");
        printf("============================================================\n> ");
        scanf(" %c", &x);

        switch(x){
            case 'a':
                incluirPessoa(&arqPessoa);
                break;

            case 'b':
                mostrarPessoas(&arqPessoa, pessoas, &qtd);
                break;

            case 'c':
                listarPessoas(&arqPessoa, pessoas, &qtd);
                selecionarPessoa(&arqPessoa, &escolhida);
            
                incluirHistorico(&arqPeso, &escolhida);
                break;

            case 'd':
                listarPessoas(&arqPessoa, pessoas, &qtd);
                selecionarPessoa(&arqPessoa, &escolhida);

                mostrarHistorico(&arqPeso, &escolhida);
                break;

            case '0':
                printf("Finalizando programa...\n");
                break;
                break;

            default:
                printf("Opção inválida!\n");
                break;
        }
    } while(x != '0');

    printf("Programa finalizado.\n");

    return 0;
}