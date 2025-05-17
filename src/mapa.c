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
 * @date Maio 2025
 * @version 1.0
 * 
 * @copyright Copyright (c) 2025
 * 
 * @course Licenciatura em Engenharia de Sistemas Informáticos
 * @institution EST-IPCA
 */

#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"
#include "mapa.h"

/**
 * @brief Carrega um mapa a partir de ficheiro e converte para estrutura de grafo
 * @param ficheiro Nome do ficheiro contendo o mapa
 * @return Apontador para o grafo criado ou NULL em caso de erro
 * 
 * @details O formato do ficheiro deve conter:
 * - Dimensões do mapa (linhas colunas)
 * - Matriz de caracteres onde:
 *   - '.' representa posições vazias
 *   - Caracteres alfanuméricos representam antenas
 * 
 * @note Cria arestas entre todas as antenas da mesma frequência
 */
Grafo* carregar_mapa(const char* ficheiro) {
    if (!ficheiro) return NULL;
    
    FILE* file = fopen(ficheiro, "r");
    if (file == NULL) return NULL;
    
    int linhas, colunas;
    if (fscanf(file, "%d %d", &linhas, &colunas) != 2) {
        fclose(file);
        return NULL;
    }
    
    Grafo* grafo = criar_grafo();
    if (grafo == NULL) {
        fclose(file);
        return NULL;
    }
    
    // Ler mapa linha por linha
    for (int y = 0; y < linhas; y++) {
        for (int x = 0; x < colunas; x++) {
            char c = fgetc(file);
            if (c == '\n' || c == '\r') {
                x--; // Ignorar quebras de linha
                continue;
            }
            if (c != '.') {
                adicionar_vertice(grafo, c, x, y);
            }
        }
    }
    
    fclose(file);
    
    // Conectar arestas entre antenas da mesma frequência
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