#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <ctype.h>

HANDLE hConsole;

void palavraInput(char palavra[]);
void verificarPos(char input[], char palavraJogo[]);
void escolherPalavra();
void count();
void yellow(char x);
void green(char x);
void reset();
void seedRandom();
void init_console_colors();
void winHistory(int resultado);
void printHistory();

int vezesJogadas = 0;
int fileSize = 0;
char *vitorias[7] = {0};
char palavraJogo[5];

int main()
{
    count();
    init_console_colors();
    seedRandom();
    
    escolherPalavra();
    char input[5];  
    printf("TERMO\n===============\nDigite 1 para ver historico\nou\n");
    
    printf("Digite sua palavra: ");
    palavraInput(input);
    verificarPos(input, palavraJogo);
    
    printf("\nPressione Enter para sair...");
    while (getchar() != '\n');
    getchar();
}

void palavraInput(char palavra[])
{
    int valido = 0;
    while(!valido){
        do{
            scanf("%s", palavra);

            if(strcmp(palavra, "1") == 0){
                printHistory(); 
                exit(1);
            }    

            while(getchar() != '\n');
            if(strlen(palavra) != 5) printf("A palavra precisa conter 5 letras\n-> ");
        }
        while(strlen(palavra) != 5);

        FILE *arquivo = fopen("palavras.txt", "r");
        char buffer[256];
        int encontrou = 0;

        while(fgets(buffer, sizeof(buffer), arquivo) != NULL)
        {
            buffer[strcspn(buffer, "\n")] = '\0';
            if(strcmp(buffer, palavra) == 0)
            {
                encontrou = 1;
                break;
            }
        }

        fclose(arquivo);

        if(encontrou){
            valido = 1;
        }
        else printf("Palavra invalida. Tente novamente:\n-> ");
    }
}


void verificarPos(char input[], char palavraJogo[])
{
    char temp[5] = {0};
    for(int i = 0; i < 5; i++)
    {
        if(input[i] == palavraJogo[i])
        {
            green(input[i]);
        }

        else
        {
            int ver = 1;
            for(int j = 0; j < 5 && ver; j++)
            {
                if(input[i] == palavraJogo[j] && temp[j] == 0 && input[j] != palavraJogo[j])
                {
                    yellow(input[i]);
                    temp[j] = 1;
                    ver = 0;
                }
            }
            if(ver) printf("%c", input[i]);
        }
    }

    if(strcmp(input, palavraJogo) != 0 && vezesJogadas < 5)
    {
        vezesJogadas++;
        printf("\n");
        palavraInput(input);
        verificarPos(input, palavraJogo);
    }

    else if(strcmp(input, palavraJogo) == 0){
        printf("\nVoce Ganhou!\n"); 
        winHistory(vezesJogadas);
        
    }

    else if(vezesJogadas == 5){
        printf("\nVoce perdeu! A palavra era: %s", palavraJogo);
        winHistory(6);
    }

}

void escolherPalavra()
{
    FILE *arquivo = fopen("palavras.txt", "r");
    if(arquivo == NULL)
    {
        printf("Nao foi possivel abrir o arquivo\n");
    }
    char linha[256];
    int palavraEscolhida = rand() % fileSize;
    int palavraAtual = 1;

    while(fgets(linha, sizeof(linha), arquivo) != NULL)
    {
        if(palavraAtual == palavraEscolhida)
        {
            linha[strcspn(linha, "\n")] = '\0';
            strcpy(palavraJogo, linha);
            palavraJogo[0] = tolower(palavraJogo[0]);
            break;
        }
        palavraAtual++;
    }
}

void count()
{
    FILE *arquivo = fopen("palavras.txt", "r");
    char buffer[256];
    while(fgets(buffer, sizeof(buffer), arquivo) != NULL)
    {
        fileSize++;
    }
    fclose(arquivo);
}

void winHistory(int resultado)
{
    FILE* arquivo = fopen("vitorias.txt", "r");
    
    if(arquivo != NULL)
    {
        for(int i = 0; i < 7; i++) {
            fscanf(arquivo, "%d", &vitorias[i]);
        }
        fclose(arquivo);

        vitorias[resultado] += 1;

        arquivo = fopen("vitorias.txt", "w");
        for(int i = 0; i < 7; i++) {
            fprintf(arquivo, "%d\n", vitorias[i]);
        }
        fclose(arquivo);
    }
    else
    {
        arquivo = fopen("vitorias.txt", "w");
        for(int i = 0; i < 7; i++) {
            vitorias[i] = 0;
            fprintf(arquivo, "0\n");
        }
        fclose(arquivo);
        winHistory(resultado);
    }
}

void printHistory()
{
    FILE *arquivo = fopen("vitorias.txt", "r");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char linha[100];

    for (int i = 0; i < 7; i++)
    {
        if (fgets(linha, sizeof(linha), arquivo) == NULL)
        {
            printf("Erro ao ler linha %d do arquivo.\n", i + 1);
            fclose(arquivo);
            return;
        }

        vitorias[i] = malloc(10);
        if (vitorias[i] == NULL)
        {
            printf("Erro de alocaco de memória.\n");
            fclose(arquivo);
            return;
        }

        strcpy(vitorias[i], linha);
        vitorias[i][strcspn(vitorias[i], "\n")] = '\0';
    }

    fclose(arquivo);

    printf("\n\nHISTORICO\n______________________________\n");
    for (int i = 0; i < 7; i++)
    {
        int win = atoi(vitorias[i]);
        (i != 6) ? printf("%d. ", i + 1) : printf("#. ");
        for (int j = 0; j < win; j++)
        {
            printf("=");
        }

        for(int j = 0; j < 20 - win; j++)
        {
            printf(" ");
        }
        printf("%d", win);
        printf("\n");
        free(vitorias[i]);
        vitorias[i] = NULL;
    }
}

void init_console_colors() {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

void yellow(char x) {
    SetConsoleTextAttribute(hConsole, 14);
    printf("%c", x);
    reset();
}

void green(char x) {
    SetConsoleTextAttribute(hConsole, 10);
    printf("%c", x);
    reset();
}

void reset() {
    SetConsoleTextAttribute(hConsole, 7);
}

void seedRandom() {
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);

    unsigned int seed = (unsigned int)(time(NULL) ^ (counter.LowPart ^ counter.HighPart));
    srand(seed);
}
