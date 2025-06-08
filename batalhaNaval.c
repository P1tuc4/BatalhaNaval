#include <stdio.h>
#include <stdlib.h> // Para a função abs()

#define TAMANHO_TABULEIRO 10 // Tamanho do tabuleiro principal
#define TAMANHO_NAVIO 3      // Tamanho fixo dos navios

// Definições para os valores no tabuleiro
#define AGUA 0
#define NAVIO 3
#define HABILIDADE_AFETADA 5 // Novo valor para áreas afetadas por habilidades

// Tamanho das matrizes de habilidade
#define TAMANHO_HABILIDADE_CONE 7
#define TAMANHO_HABILIDADE_CRUZ 5
#define TAMANHO_HABILIDADE_OCTAEDRO 5

// Enum para facilitar a leitura das orientações dos navios
typedef enum {
    HORIZONTAL,
    VERTICAL,
    DIAGONAL_PRINCIPAL, // linha++, coluna++
    DIAGONAL_SECUNDARIA // linha++, coluna--
} OrientacaoNavio;

// --- Funções de Manipulação do Tabuleiro e Navios (do nível anterior) ---

// Função para inicializar o tabuleiro com água
void inicializarTabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            tabuleiro[i][j] = AGUA;
        }
    }
}

// Função para verificar se um navio pode ser posicionado sem sair dos limites
// e sem sobrepor outros navios. Retorna 1 se puder posicionar, 0 caso contrário.
int podePosicionarNavio(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int linhaInicial, int colunaInicial, OrientacaoNavio orientacao) {
    if (linhaInicial < 0 || linhaInicial >= TAMANHO_TABULEIRO ||
        colunaInicial < 0 || colunaInicial >= TAMANHO_TABULEIRO) {
        return 0; // Coordenada inicial fora dos limites
    }

    for (int k = 0; k < TAMANHO_NAVIO; k++) {
        int linhaAtual = linhaInicial;
        int colunaAtual = colunaInicial;

        switch (orientacao) {
            case HORIZONTAL:
                colunaAtual = colunaInicial + k;
                break;
            case VERTICAL:
                linhaAtual = linhaInicial + k;
                break;
            case DIAGONAL_PRINCIPAL: // linha++, coluna++
                linhaAtual = linhaInicial + k;
                colunaAtual = colunaInicial + k;
                break;
            case DIAGONAL_SECUNDARIA: // linha++, coluna--
                linhaAtual = linhaInicial + k;
                colunaAtual = colunaInicial - k;
                break;
        }

        // Verifica se a posição atual do navio está dentro dos limites do tabuleiro
        if (linhaAtual < 0 || linhaAtual >= TAMANHO_TABULEIRO ||
            colunaAtual < 0 || colunaAtual >= TAMANHO_TABULEIRO) {
            return 0; // Saiu dos limites
        }

        // Verifica sobreposição com outro navio
        if (tabuleiro[linhaAtual][colunaAtual] == NAVIO) {
            return 0; // Sobreposição
        }
    }
    return 1; // Pode posicionar
}

// Função para posicionar um navio no tabuleiro
void posicionarNavio(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int linhaInicial, int colunaInicial, OrientacaoNavio orientacao) {
    for (int k = 0; k < TAMANHO_NAVIO; k++) {
        int linhaAtual = linhaInicial;
        int colunaAtual = colunaInicial;

        switch (orientacao) {
            case HORIZONTAL:
                colunaAtual = colunaInicial + k;
                break;
            case VERTICAL:
                linhaAtual = linhaInicial + k;
                break;
            case DIAGONAL_PRINCIPAL: // linha++, coluna++
                linhaAtual = linhaInicial + k;
                colunaAtual = colunaInicial + k;
                break;
            case DIAGONAL_SECUNDARIA: // linha++, coluna--
                linhaAtual = linhaInicial + k;
                colunaAtual = colunaInicial - k;
                break;
        }
        tabuleiro[linhaAtual][colunaAtual] = NAVIO;
    }
}

// --- Novas Funções para Habilidades ---

// Função para criar a matriz da habilidade CONE
// Um cone apontando para baixo, com o ponto de origem no topo
void criarHabilidadeCone(int cone[TAMANHO_HABILIDADE_CONE][TAMANHO_HABILIDADE_CONE]) {
    int centro_coluna = TAMANHO_HABILIDADE_CONE / 2;

    for (int i = 0; i < TAMANHO_HABILIDADE_CONE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE_CONE; j++) {
            cone[i][j] = 0; // Inicializa com 0 (não afetado)
        }
    }

    // Lógica para preencher o cone
    for (int i = 0; i < TAMANHO_HABILIDADE_CONE; i++) {
        // Largura do cone aumenta com a linha
        int largura_atual = (i * 2) + 1;
        // Calcula o início e fim da parte afetada na coluna
        int inicio_col = centro_coluna - (largura_atual / 2);
        int fim_col = centro_coluna + (largura_atual / 2);

        for (int j = 0; j < TAMANHO_HABILIDADE_CONE; j++) {
            if (j >= inicio_col && j <= fim_col) {
                cone[i][j] = 1; // Afetado
            }
        }
    }
}

// Função para criar a matriz da habilidade CRUZ
// Ponto de origem no centro
void criarHabilidadeCruz(int cruz[TAMANHO_HABILIDADE_CRUZ][TAMANHO_HABILIDADE_CRUZ]) {
    int centro = TAMANHO_HABILIDADE_CRUZ / 2;

    for (int i = 0; i < TAMANHO_HABILIDADE_CRUZ; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE_CRUZ; j++) {
            cruz[i][j] = 0; // Inicializa com 0
        }
    }

    // Linha central
    for (int j = 0; j < TAMANHO_HABILIDADE_CRUZ; j++) {
        cruz[centro][j] = 1;
    }
    // Coluna central
    for (int i = 0; i < TAMANHO_HABILIDADE_CRUZ; i++) {
        cruz[i][centro] = 1;
    }
}

// Função para criar a matriz da habilidade OCTAEDRO (formato de losango/diamante)
// Ponto de origem no centro
void criarHabilidadeOctaedro(int octaedro[TAMANHO_HABILIDADE_OCTAEDRO][TAMANHO_HABILIDADE_OCTAEDRO]) {
    int centro = TAMANHO_HABILIDADE_OCTAEDRO / 2;

    for (int i = 0; i < TAMANHO_HABILIDADE_OCTAEDRO; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE_OCTAEDRO; j++) {
            octaedro[i][j] = 0; // Inicializa com 0
        }
    }

    // Lógica para preencher o losango
    // A condição para estar no losango é |linha - centro| + |coluna - centro| <= centro
    for (int i = 0; i < TAMANHO_HABILIDADE_OCTAEDRO; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE_OCTAEDRO; j++) {
            if (abs(i - centro) + abs(j - centro) <= centro) {
                octaedro[i][j] = 1; // Afetado
            }
        }
    }
}

// Função para sobrepor uma habilidade no tabuleiro principal
// skill_size: tamanho da matriz da habilidade (ex: TAMANHO_HABILIDADE_CONE)
// skill_matrix: a matriz da habilidade (cone, cruz, octaedro)
// origem_linha, origem_coluna: ponto central da habilidade no tabuleiro principal
void sobreporHabilidadeNoTabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO],
                                   int skill_matrix_size,
                                   int skill_matrix[skill_matrix_size][skill_matrix_size],
                                   int origem_linha, int origem_coluna) {

    // Calcula o deslocamento para centralizar a matriz da habilidade
    int offset = skill_matrix_size / 2;

    for (int i = 0; i < skill_matrix_size; i++) {
        for (int j = 0; j < skill_matrix_size; j++) {
            // Calcula a posição correspondente no tabuleiro principal
            int tabuleiro_linha = origem_linha + (i - offset);
            int tabuleiro_coluna = origem_coluna + (j - offset);

            // Verifica se a posição está dentro dos limites do tabuleiro
            if (tabuleiro_linha >= 0 && tabuleiro_linha < TAMANHO_TABULEIRO &&
                tabuleiro_coluna >= 0 && tabuleiro_coluna < TAMANHO_TABULEIRO) {
                // Se a posição na matriz de habilidade for 1 e não for um navio,
                // marca como HABILIDADE_AFETADA
                if (skill_matrix[i][j] == 1 && tabuleiro[tabuleiro_linha][tabuleiro_coluna] != NAVIO) {
                    tabuleiro[tabuleiro_linha][tabuleiro_coluna] = HABILIDADE_AFETADA;
                }
            }
        }
    }
}

// Função para exibir o tabuleiro com diferentes caracteres
void exibirTabuleiroFinal(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    printf("   "); // Espaço para alinhar os números das colunas
    for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
        printf("%2d ", j); // Imprime os números das colunas com 2 dígitos e espaço
    }
    printf("\n");
    printf("  +-------------------------------\n"); // Separador visual

    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        printf("%2d|", i); // Imprime os números das linhas com 2 dígitos e barra
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            switch (tabuleiro[i][j]) {
                case AGUA:
                    printf(" ~ "); // Água
                    break;
                case NAVIO:
                    printf(" N "); // Navio
                    break;
                case HABILIDADE_AFETADA:
                    printf(" X "); // Área afetada pela habilidade
                    break;
                default:
                    printf(" ? "); // Qualquer outro valor inesperado
                    break;
            }
        }
        printf("\n");
    }
}

// --- Função Principal ---
int main() {
    int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO];

    // Inicializa o tabuleiro com água
    inicializarTabuleiro(tabuleiro);

    // --- Posicionamento dos Navios (do nível anterior) ---
    // Você pode alterar as coordenadas para testar a sobreposição com as habilidades

    // Navio 1: Horizontal
    int navio1_linha = 1;
    int navio1_coluna = 1;
    OrientacaoNavio navio1_orientacao = HORIZONTAL;
    if (podePosicionarNavio(tabuleiro, navio1_linha, navio1_coluna, navio1_orientacao)) {
        posicionarNavio(tabuleiro, navio1_linha, navio1_coluna, navio1_orientacao);
        printf("Navio 1 (HORIZONTAL) posicionado em (%d,%d).\n", navio1_linha, navio1_coluna);
    } else {
        printf("ERRO: Nao foi possivel posicionar Navio 1 (HORIZONTAL) em (%d,%d).\n", navio1_linha, navio1_coluna);
    }

    // Navio 2: Vertical
    int navio2_linha = 0;
    int navio2_coluna = 7;
    OrientacaoNavio navio2_orientacao = VERTICAL;
    if (podePosicionarNavio(tabuleiro, navio2_linha, navio2_coluna, navio2_orientacao)) {
        posicionarNavio(tabuleiro, navio2_linha, navio2_coluna, navio2_orientacao);
        printf("Navio 2 (VERTICAL) posicionado em (%d,%d).\n", navio2_linha, navio2_coluna);
    } else {
        printf("ERRO: Nao foi possivel posicionar Navio 2 (VERTICAL) em (%d,%d).\n", navio2_linha, navio2_coluna);
    }

    // Navio 3: Diagonal Principal (linha++, coluna++)
    int navio3_linha = 4;
    int navio3_coluna = 4;
    OrientacaoNavio navio3_orientacao = DIAGONAL_PRINCIPAL;
    if (podePosicionarNavio(tabuleiro, navio3_linha, navio3_coluna, navio3_orientacao)) {
        posicionarNavio(tabuleiro, navio3_linha, navio3_coluna, navio3_orientacao);
        printf("Navio 3 (DIAGONAL PRINCIPAL) posicionado em (%d,%d).\n", navio3_linha, navio3_coluna);
    } else {
        printf("ERRO: Nao foi possivel posicionar Navio 3 (DIAGONAL PRINCIPAL) em (%d,%d).\n", navio3_linha, navio3_coluna);
    }

    // Navio 4: Diagonal Secundária (linha++, coluna--)
    int navio4_linha = 0;
    int navio4_coluna = 9;
    OrientacaoNavio navio4_orientacao = DIAGONAL_SECUNDARIA;
    if (podePosicionarNavio(tabuleiro, navio4_linha, navio4_coluna, navio4_orientacao)) {
        posicionarNavio(tabuleiro, navio4_linha, navio4_coluna, navio4_orientacao);
        printf("Navio 4 (DIAGONAL SECUNDARIA) posicionado em (%d,%d).\n", navio4_linha, navio4_coluna);
    } else {
        printf("ERRO: Nao foi possivel posicionar Navio 4 (DIAGONAL SECUNDARIA) em (%d,%d).\n", navio4_linha, navio4_coluna);
    }

    printf("\n--- Tabuleiro com Navios Posicionados ---\n");
    exibirTabuleiroFinal(tabuleiro); // Exibe o tabuleiro apenas com navios

    // --- Criação e Posicionamento das Habilidades ---

    // 1. Habilidade CONE
    int cone_matriz[TAMANHO_HABILIDADE_CONE][TAMANHO_HABILIDADE_CONE];
    criarHabilidadeCone(cone_matriz);
    printf("\n--- Matriz Habilidade CONE (%dx%d) ---\n", TAMANHO_HABILIDADE_CONE, TAMANHO_HABILIDADE_CONE);
    for (int i = 0; i < TAMANHO_HABILIDADE_CONE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE_CONE; j++) {
            printf("%d ", cone_matriz[i][j]);
        }
        printf("\n");
    }
    int cone_origem_linha = 3; // Ponto de origem do cone no tabuleiro principal
    int cone_origem_coluna = 3;
    printf("Aplicando Habilidade CONE no tabuleiro em (%d,%d)...\n", cone_origem_linha, cone_origem_coluna);
    sobreporHabilidadeNoTabuleiro(tabuleiro, TAMANHO_HABILIDADE_CONE, cone_matriz, cone_origem_linha, cone_origem_coluna);

    // 2. Habilidade CRUZ
    int cruz_matriz[TAMANHO_HABILIDADE_CRUZ][TAMANHO_HABILIDADE_CRUZ];
    criarHabilidadeCruz(cruz_matriz);
    printf("\n--- Matriz Habilidade CRUZ (%dx%d) ---\n", TAMANHO_HABILIDADE_CRUZ, TAMANHO_HABILIDADE_CRUZ);
    for (int i = 0; i < TAMANHO_HABILIDADE_CRUZ; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE_CRUZ; j++) {
            printf("%d ", cruz_matriz[i][j]);
        }
        printf("\n");
    }
    int cruz_origem_linha = 7; // Ponto de origem da cruz no tabuleiro principal
    int cruz_origem_coluna = 2;
    printf("Aplicando Habilidade CRUZ no tabuleiro em (%d,%d)...\n", cruz_origem_linha, cruz_origem_coluna);
    sobreporHabilidadeNoTabuleiro(tabuleiro, TAMANHO_HABILIDADE_CRUZ, cruz_matriz, cruz_origem_linha, cruz_origem_coluna);

    // 3. Habilidade OCTAEDRO
    int octaedro_matriz[TAMANHO_HABILIDADE_OCTAEDRO][TAMANHO_HABILIDADE_OCTAEDRO];
    criarHabilidadeOctaedro(octaedro_matriz);
    printf("\n--- Matriz Habilidade OCTAEDRO (%dx%d) ---\n", TAMANHO_HABILIDADE_OCTAEDRO, TAMANHO_HABILIDADE_OCTAEDRO);
    for (int i = 0; i < TAMANHO_HABILIDADE_OCTAEDRO; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE_OCTAEDRO; j++) {
            printf("%d ", octaedro_matriz[i][j]);
        }
        printf("\n");
    }
    int octaedro_origem_linha = 1; // Ponto de origem do octaedro no tabuleiro principal
    int octaedro_origem_coluna = 8;
    printf("Aplicando Habilidade OCTAEDRO no tabuleiro em (%d,%d)...\n", octaedro_origem_linha, octaedro_origem_coluna);
    sobreporHabilidadeNoTabuleiro(tabuleiro, TAMANHO_HABILIDADE_OCTAEDRO, octaedro_matriz, octaedro_origem_linha, octaedro_origem_coluna);


    // Exibe o tabuleiro final com navios e áreas de habilidade
    printf("\n--- Tabuleiro Final com Navios e Habilidades ---\n");
    exibirTabuleiroFinal(tabuleiro);

    return 0;
}