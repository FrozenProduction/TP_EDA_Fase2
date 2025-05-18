/**
 * @file main.c
 * @brief Programa principal para demonstração da Fase 2 do projeto EDA
 * 
 * @details Implementa o fluxo principal do programa que:
 * - Carrega um mapa de antenas a partir de ficheiro
 * - Demonstra as funcionalidades implementadas na Fase 2
 * - Inclui exemplos de uso de todas as operações principais
 * - Liberta todos os recursos no final
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
#include "grafo.h"
#include "mapa.h"

/**
 * @brief Função principal do programa
 * @return 0 se executado com sucesso, 1 em caso de erro
 * 
 * @details O fluxo do programa é:
 * 1. Carregar o mapa a partir de ficheiro
 * 2. Mostrar representação do grafo e do mapa
 * 3. Executar procura em profundidade (DFS)
 * 4. Executar procura em largura (BFS)
 * 5. Encontrar caminhos entre antenas
 * 6. Detetar intersecções entre frequências
 * 7. Libertar todos os recursos
 * 
 * @note O programa assume que o ficheiro do mapa está em "data/mapa.txt"
 * e contém antenas nas posições esperadas para as demonstrações.
 */
int main() {
    // 1. Carregar mapa
    Grafo* grafo = carregar_mapa("data/mapa.txt");
    if (!grafo) {
        printf("Erro ao carregar mapa\n");
        return 1;
    }

    // 2. Mostrar grafo e mapa
    printf("\n=== Grafo ===");
    imprimir_grafo(grafo);
    
    printf("\n=== Mapa ===\n");
    imprimir_mapa(grafo, 12, 12);

    // 3. Procura em Profundidade
    Vertice* inicio_dfs = encontrar_vertice(grafo, 5, 7);
    if (inicio_dfs != NULL) {
        printf("\n=== Procura em Profundidade ===\n");
        procura_profundidade(grafo, inicio_dfs);
    }

    // 4. Procura em Largura
    Vertice* inicio_bfs = encontrar_vertice(grafo, 8, 8);
    if (inicio_bfs != NULL) {
        printf("\n=== Procura em Largura ===\n");
        procura_largura(grafo, inicio_bfs);
    }

    // 5. Caminhos entre antenas
    Vertice* origem = encontrar_vertice(grafo, 4, 4);
    Vertice* destino = encontrar_vertice(grafo, 7, 3);
    encontrar_caminhos(grafo, origem, destino);

    // 6. Intersecções entre frequências
    int count = intersecoes_frequencias(grafo, 'A', '0');
    if (count == 0) {
        printf("\n=== Intersecoes entre frequencias de A e 0 ===\n");
        printf("Nenhuma intersecao encontrada entre as frequencias A e 0\n");
    }

    // 7. Liberar recursos
    destruir_grafo(grafo);
    return 0;
}