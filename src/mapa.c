/**
 * @file mapa.c
 * @brief Implementação das operações de carregamento e visualização de mapas
 * 
 * @details Implementa as funções declaradas em mapa.h, incluindo:
 * - Leitura de ficheiros de mapa e conversão para grafos
 * - Representação matricial dos mapas
 * - Visualização de mapas com antenas e efeitos nefastos
 * - Deteção de posições com interferência entre antenas
 * 
 * @author Diogo Pereira
 * @date 18 Maio 2025
 * @version 1.0
 * 
 * @copyright Copyright (c) 2025
 * 
 * @course Licenciatura em Engenharia de Sistemas Informáticos
 * @institution EST-IPCA
 */

#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include "grafo.h"
#include "mapa.h"

/**
 * @brief Cria um ficheiro binário padrão com o mapa inicial
 * @return 0 em caso de sucesso, -1 em caso de erro
 * 
 * @details A função cria uma pasta "data" se não existir e gera um ficheiro binário
 * chamado "mapa.bin" com um mapa pré-definido (podemos alterar esse mapa)
 * 
 * O formato do ficheiro segue estritamente:
 * 1. Dois inteiros (4 bytes cada) para linhas e colunas
 * 2. Sequência de caracteres (1 byte cada) representando as linhas do mapa
 * 
 * @note Esta função é chamada automaticamente se o ficheiro de mapa não existir
 */
int criar_mapa_padrao() {
    const char* mapa[] = {
        "............",
        "............",
        "............",
        ".......0....",
        "....0.......",
        "......A.....",
        ".........0..",
        ".....0......",
        "........A...",
        "............",
        ".......A....",
        "............"
    };
    
    // Cria pasta se não existir
    _mkdir("data");
    
    FILE* file = fopen("data/mapa.bin", "wb");
    if (!file) return -1;
    
    // Escreve dimensões
    int linhas = 12, colunas = 12;
    fwrite(&linhas, sizeof(int), 1, file);
    fwrite(&colunas, sizeof(int), 1, file);
    
    // Escreve dados do mapa
    for (int i = 0; i < linhas; i++) {
        fwrite(mapa[i], sizeof(char), colunas, file);
    }
    
    fclose(file);
    return 0;
}

/**
 * @brief Implementação do carregamento de mapa a partir de ficheiro binário
 * @param ficheiro Caminho para o ficheiro binário contendo o mapa
 * @return Apontador para grafo criado ou NULL em caso de erro
 * 
 * @details A função:
 * 1. Abre o ficheiro em modo binário ("rb")
 * 2. Lê as dimensões (2 × sizeof(int))
 * 3. Aloca memória para o grafo e buffers temporários
 * 4. Processa cada linha do mapa, adicionando vértices para antenas
 * 5. Conecta arestas entre antenas da mesma frequência
 * 
 * @note Se o ficheiro não existir, chama criar_mapa_padrao() e tenta novamente
 */
Grafo* carregar_mapa(const char* ficheiro) {
    if (!ficheiro) return NULL;
    
    FILE* file = fopen(ficheiro, "rb");
    if (!file) {
        if (criar_mapa_padrao() != 0) return NULL;
        file = fopen(ficheiro, "rb");
        if (!file) return NULL;
    }
    
    int linhas, colunas;
    if (fread(&linhas, sizeof(int), 1, file) != 1 ||
        fread(&colunas, sizeof(int), 1, file) != 1) {
        fclose(file);
        return NULL;
    }
    
    Grafo* grafo = criar_grafo();
    if (!grafo) {
        fclose(file);
        return NULL;
    }
    
    char* linha = malloc(colunas + 1); // +1 para segurança
    if (!linha) {
        fclose(file);
        destruir_grafo(grafo);
        return NULL;
    }
    
    for (int y = 0; y < linhas; y++) {
        size_t elementos_lidos = fread(linha, sizeof(char), colunas, file);
        if (elementos_lidos != (size_t)colunas) {
            free(linha);
            fclose(file);
            destruir_grafo(grafo);
            return NULL;
        }
        
        for (int x = 0; x < colunas; x++) {
            if (linha[x] != '.') {
                adicionar_vertice(grafo, linha[x], x, y);
            }
        }
    }
    
    free(linha);
    fclose(file);
    
    // Conectar arestas
    Vertice* v = grafo->vertices;
    while (v != NULL) {
        Vertice* u = grafo->vertices;
        while (u != NULL) {
            if (v != u && v->frequencia == u->frequencia) {
                adicionar_aresta(v, u);
            }
            u = u->proximo;
        }
        v = v->proximo;
    }
    
    return grafo;
}

/**
 * @brief Imprime uma representação visual do mapa na consola
 * @param grafo Apontador para o grafo contendo as antenas
 * @param linhas Número de linhas do mapa
 * @param colunas Número de colunas do mapa
 * 
 * @details A representação usa:
 * - Caracteres das antenas para suas posições
 * - '#' para posições com efeito nefasto
 * - '.' para posições vazias
 * 
 * @note Calcula automaticamente as posições com efeito nefasto
 * considerando todas as combinações de antenas da mesma frequência
 */
void imprimir_mapa(Grafo* grafo, int linhas, int colunas) {
    Mapa* primeira_linha = NULL;
    Mapa* ultima_linha = NULL;
    
    // Criar todas as linhas do mapa
    for (int y = 0; y < linhas; y++) {
        Mapa* nova_linha = (Mapa*)malloc(sizeof(Mapa));
        nova_linha->dados = (char*)malloc(colunas * sizeof(char));
        
        // Inicializar com pontos
        for (int x = 0; x < colunas; x++) {
            nova_linha->dados[x] = '.';
        }
        
        nova_linha->prox = NULL;
        
        // Adicionar à lista
        if (primeira_linha == NULL) {
            primeira_linha = nova_linha;
            ultima_linha = nova_linha;
        } else {
            ultima_linha->prox = nova_linha;
            ultima_linha = nova_linha;
        }
    }

    // Marcar antenas no mapa
    Vertice* v = grafo->vertices;
    while (v != NULL) {
        // Encontrar a linha correta
        Mapa* linha_atual = primeira_linha;
        for (int y = 0; y < v->y; y++) {
            linha_atual = linha_atual->prox;
        }
        linha_atual->dados[v->x] = v->frequencia;
        v = v->proximo;
    }
    
    // Marcar efeitos nefastos
    v = grafo->vertices;
    while (v != NULL) {
        Vertice* u = grafo->vertices;
        while (u != NULL) {
            if (v != u && v->frequencia == u->frequencia) {
                int dx = u->x - v->x;
                int dy = u->y - v->y;
                
                // Verificar alinhamento para efeito nefasto
                if (dx == 0 || dy == 0 || abs(dx) == abs(dy) || 
                    (abs(dx) == 2*abs(dy)) || (2*abs(dx) == abs(dy)) ||
                    (abs(dx) == 3*abs(dy)) || (3*abs(dx) == abs(dy))) {
                    
                    // Pontos de efeito nefasto
                    int x1 = v->x - dx;
                    int y1 = v->y - dy;
                    int x2 = u->x + dx;
                    int y2 = u->y + dy;
                    
                    // Marcar no mapa se estiver dentro dos limites
                    if (x1 >= 0 && x1 < colunas && y1 >= 0 && y1 < linhas) {
                        Mapa* linha = primeira_linha;
                        for (int y = 0; y < y1; y++) {
                            linha = linha->prox;
                        }
                        if (linha->dados[x1] == '.') {
                            linha->dados[x1] = '#';
                        }
                    }
                    
                    if (x2 >= 0 && x2 < colunas && y2 >= 0 && y2 < linhas) {
                        Mapa* linha = primeira_linha;
                        for (int y = 0; y < y2; y++) {
                            linha = linha->prox;
                        }
                        if (linha->dados[x2] == '.') {
                            linha->dados[x2] = '#';
                        }
                    }
                }
            }
            u = u->proximo;
        }
        v = v->proximo;
    }
    
    // Imprimir mapa
    Mapa* linha = primeira_linha;
    while (linha != NULL) {
        printf("%.*s\n", colunas, linha->dados);
        linha = linha->prox;
    }
    
    // Liberar memória
    linha = primeira_linha;
    while (linha != NULL) {
        Mapa* temp = linha;
        linha = linha->prox;
        free(temp->dados);
        free(temp);
    }
}