#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM 5
#define NAVIOS 3

typedef struct {
    char tabuleiro[TAM][TAM];
    int navios_restantes;
} Jogador;

void inicializaTabuleiro(Jogador *jogador) {
    for (int i = 0; i < TAM; i++)
        for (int j = 0; j < TAM; j++)
            jogador->tabuleiro[i][j] = '~';  // Água
    jogador->navios_restantes = NAVIOS;
}

void posicionaNavios(Jogador *jogador, int automatico) {
    int x, y, navios_posicionados = 0;

    while (navios_posicionados < NAVIOS) {
        if (automatico) {
            x = rand() % TAM;
            y = rand() % TAM;
        } else {
            printf("Informe as coordenadas para o navio %d (linha e coluna de 0 a %d): ", navios_posicionados + 1, TAM - 1);
            scanf("%d %d", &x, &y);
        }

        if (jogador->tabuleiro[x][y] != 'N') {
            jogador->tabuleiro[x][y] = 'N';  // Navio
            navios_posicionados++;
        } else if (!automatico) {
            printf("Já existe um navio nessa posição!\n");
        }
    }
}

void exibeTabuleiro(Jogador *jogador, int mostrarNavios) {
    printf("  ");
    for (int i = 0; i < TAM; i++)
        printf("%d ", i);
    printf("\n");

    for (int i = 0; i < TAM; i++) {
        printf("%d ", i);
        for (int j = 0; j < TAM; j++) {
            char c = jogador->tabuleiro[i][j];
            if (!mostrarNavios && c == 'N')
                printf("~ ");
            else
                printf("%c ", c);
        }
        printf("\n");
    }
}

int atacar(Jogador *atacado, int x, int y) {
    if (atacado->tabuleiro[x][y] == 'N') {
        atacado->tabuleiro[x][y] = 'X';  // Acertou
        atacado->navios_restantes--;
        printf("Acertou um navio!\n");
        return 1;
    } else if (atacado->tabuleiro[x][y] == '~') {
        atacado->tabuleiro[x][y] = 'o';  // Água
        printf("Água!\n");
    } else {
        printf("Você já atirou aqui.\n");
    }
    return 0;
}

void salvarJogo(Jogador *jogador1, Jogador *jogador2) {
    FILE *f = fopen("batalha_salvo.bin", "wb");
    if (!f) {
        printf("Erro ao salvar o jogo!\n");
        return;
    }
    fwrite(jogador1, sizeof(Jogador), 1, f);
    fwrite(jogador2, sizeof(Jogador), 1, f);
    fclose(f);
    printf("Jogo salvo com sucesso!\n");
}

void carregarJogo(Jogador *jogador1, Jogador *jogador2) {
    FILE *f = fopen("batalha_salvo.bin", "rb");
    if (!f) {
        printf("Nenhum jogo salvo encontrado.\n");
        return;
    }
    fread(jogador1, sizeof(Jogador), 1, f);
    fread(jogador2, sizeof(Jogador), 1, f);
    fclose(f);
    printf("Jogo carregado com sucesso!\n");
}

int main() {
    Jogador player, computador;
    int x, y, opcao;
    srand(time(NULL));

    printf("1 - Novo Jogo\n2 - Carregar Jogo\nEscolha: ");
    scanf("%d", &opcao);

    if (opcao == 1) {
        inicializaTabuleiro(&player);
        inicializaTabuleiro(&computador);
        printf("Posicione seus navios:\n");
        posicionaNavios(&player, 0);
        posicionaNavios(&computador, 1);
    } else {
        carregarJogo(&player, &computador);
    }

    while (player.navios_restantes > 0 && computador.navios_restantes > 0) {
        printf("\nSeu Tabuleiro:\n");
        exibeTabuleiro(&player, 1);
        printf("Tabuleiro do Computador:\n");
        exibeTabuleiro(&computador, 0);

        printf("Informe as coordenadas para atacar (linha e coluna): ");
        scanf("%d %d", &x, &y);
        atacar(&computador, x, y);

        if (computador.navios_restantes == 0)
            break;

        x = rand() % TAM;
        y = rand() % TAM;
        printf("O computador atacou a posição (%d, %d)\n", x, y);
        atacar(&player, x, y);

        printf("Deseja salvar o jogo? (1-Sim / 0-Não): ");
        scanf("%d", &opcao);
        if (opcao == 1)
            salvarJogo(&player, &computador);
    }

    if (player.navios_restantes == 0)
        printf("Você perdeu!\n");
    else
        printf("Parabéns! Você venceu!\n");

    return 0;
}

