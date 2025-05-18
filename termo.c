#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

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

int vezesJogadas = 0;
int fileSize = 0;
char palavraJogo[5];

int main()
{
    count();
    init_console_colors();
    seedRandom();

    escolherPalavra();
    char input[5];  
    printf("TERMO\n===============\n");

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

    else if(strcmp(input, palavraJogo) == 0) printf("\nVoce Ganhou!\n");

    else if(vezesJogadas == 5)
    {
        printf("\nVoce perdeu! A palavra era: %s", palavraJogo);
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