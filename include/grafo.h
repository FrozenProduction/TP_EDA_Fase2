/**
 * @file grafo.h
 * @brief Definição de estruturas e operações para gestão de antenas usando grafos
 * 
 * @details Implementa a Fase 2 do Projeto de Estruturas de Dados Avançadas (EDA), com:
 * - Representação de antenas como vértices num grafo
 * - Conexões entre antenas da mesma frequência como arestas
 * - Algoritmos de procura em grafos (DFS e BFS)
 * - Cálculo de caminhos entre antenas
 * - Deteção de intersecções entre frequências diferentes
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

#ifndef GRAFO_H
#define GRAFO_H

#include <stdbool.h>

/**
 * @brief Estrutura que representa um vértice do grafo (antena)
 */
typedef struct Vertice Vertice;

/**
 * @brief Estrutura que representa uma aresta do grafo (conexão entre antenas)
 */
typedef struct Aresta Aresta;

/**
 * @brief Estrutura principal que representa o grafo completo
 */
typedef struct Grafo Grafo;

/**
 * @brief Estrutura auxiliar para representação de caminhos no grafo
 */
typedef struct CaminhoNode CaminhoNode;

/**
 * @brief Estrutura auxiliar para implementação de filas (usada em BFS)
 */
typedef struct NodeFila NodeFila;

/**
 * @brief Estrutura para representação de intersecções entre frequências
 */
typedef struct Intersecao Intersecao;

/**
 * @struct Vertice
 * @brief Representa uma antena no grafo
 */
struct Vertice {
    char frequencia;        ///< Caracter que representa a frequência da antena
    int x;                  ///< Coordenada x (coluna) da antena no mapa
    int y;                  ///< Coordenada y (linha) da antena no mapa
    bool visitado;          ///< Flag para marcar se o vértice foi visitado em algoritmos de procura
    Aresta* arestas;        ///< Lista de arestas que partem deste vértice
    Vertice* proximo;       ///< Próximo vértice na lista de vértices do grafo
};

/**
 * @struct Aresta
 * @brief Representa uma conexão entre duas antenas da mesma frequência
 */
struct Aresta {
    Vertice* destino;       ///< Vértice de destino da aresta
    Aresta* proxima;        ///< Próxima aresta na lista de arestas do vértice
};

/**
 * @struct Grafo
 * @brief Estrutura principal que contém todos os vértices e arestas
 */
struct Grafo {
    Vertice* vertices;      ///< Lista de vértices (antenas) do grafo
    int num_vertices;       ///< Contador do número total de vértices no grafo
};

/**
 * @struct NodeFila
 * @brief Nó auxiliar para implementação de filas (usada em BFS)
 */
struct NodeFila {
    Vertice* vertice;       ///< Vértice armazenado no nó
    NodeFila* prox;         ///< Próximo nó na fila
};

/**
 * @struct CaminhoNode
 * @brief Nó auxiliar para armazenar caminhos entre antenas
 */
struct CaminhoNode {
    Vertice* vertice;       ///< Vértice atual no caminho
    CaminhoNode* prox;      ///< Próximo vértice no caminho
};

/**
 * @struct Intersecao
 * @brief Representa um ponto de intersecção entre linhas de frequências diferentes
 */
struct Intersecao {
    int x, y;               ///< Coordenadas do ponto de intersecção
    Vertice* a1, *a2;       ///< Par de antenas da frequência A que causam a intersecção
    Vertice* b1, *b2;       ///< Par de antenas da frequência B que causam a intersecção
    Intersecao* prox;       ///< Próxima intersecção na lista
};

/**
 * @brief Cria um novo grafo vazio
 */
Grafo* criar_grafo();

/**
 * @brief Liberta toda a memória associada ao grafo
 * @param grafo Apontador para o grafo a destruir
 */
int destruir_grafo(Grafo* grafo);

/**
 * @brief Adiciona um novo vértice (antena) ao grafo
 * @param grafo Apontador para o grafo
 * @param freq Frequência da antena (caracter)
 * @param x Coordenada x (coluna) da antena
 * @param y Coordenada y (linha) da antena
 */
Vertice* adicionar_vertice(Grafo* grafo, char freq, int x, int y);

/**
 * @brief Adiciona uma aresta entre dois vértices (antenas da mesma frequência)
 * @param origem Vértice de origem
 * @param destino Vértice de destino
 */
int adicionar_aresta(Vertice* origem, Vertice* destino);

/**
 * @brief Função recursiva auxiliar para procura em profundidade (DFS)
 * @param v Vértice atual a visitar
 */
int procura_profundidade_rec(Vertice* v);

/**
 * @brief Executa uma procura em profundidade (DFS) a partir de um vértice
 * @param grafo Apontador para o grafo
 * @param inicio Vértice de início da procura
 */
int procura_profundidade(Grafo* grafo, Vertice* inicio);

/**
 * @brief Executa uma procura em largura (BFS) a partir de um vértice
 * @param grafo Apontador para o grafo
 * @param inicio Vértice de início da procura
 */
int procura_largura(Grafo* grafo, Vertice* inicio);

/**
 * @brief Imprime um caminho na ordem inversa (auxiliar para encontrar_caminhos_rec)
 * @param caminho Apontador para o nó do caminho a imprimir
 */
void imprimir_caminho_inverso(CaminhoNode* caminho);

/**
 * @brief Função recursiva auxiliar para encontrar todos os caminhos entre dois vértices
 * @param atual Vértice atual na procura
 * @param destino Vértice de destino
 * @param caminho_atual Caminho percorrido até ao momento
 */
int encontrar_caminhos_rec(Vertice* atual, Vertice* destino, CaminhoNode* caminho_atual);

/**
 * @brief Encontra e imprime todos os caminhos entre duas antenas
 * @param grafo Apontador para o grafo
 * @param origem Vértice de origem
 * @param destino Vértice de destino
 */
int encontrar_caminhos(Grafo* grafo, Vertice* origem, Vertice* destino);

/**
 * @brief Calcula o ponto de intersecção entre duas linhas definidas por pares de pontos
 * @param p1 Primeiro ponto da primeira linha (frequência A)
 * @param p2 Segundo ponto da primeira linha (frequência A)
 * @param p3 Primeiro ponto da segunda linha (frequência B)
 * @param p4 Segundo ponto da segunda linha (frequência B)
 * @param[out] x Coordenada x da intersecção (se existir)
 * @param[out] y Coordenada y da intersecção (se existir)
 */
bool calcular_intersecao(Vertice* p1, Vertice* p2, Vertice* p3, Vertice* p4, int* x, int* y);

/**
 * @brief Encontra e imprime todas as intersecções entre pares de antenas de duas frequências
 * @param grafo Apontador para o grafo
 * @param freqA Primeira frequência a considerar
 * @param freqB Segunda frequência a considerar
 * @return Número de intersecções encontradas
 */
int intersecoes_frequencias(Grafo* grafo, char freqA, char freqB);

/**
 * @brief Encontra um vértice no grafo pelas suas coordenadas
 * @param grafo Apontador para o grafo
 * @param x Coordenada x (coluna) do vértice
 * @param y Coordenada y (linha) do vértice
 */
Vertice* encontrar_vertice(Grafo* grafo, int x, int y);

/**
 * @brief Reinicia o estado 'visitado' de todos os vértices do grafo
 * @param grafo Apontador para o grafo
 */
int reiniciar_visitados(Grafo* grafo);

/**
 * @brief Imprime a representação do grafo na consola
 * @param grafo Apontador para o grafo a imprimir
 */
void imprimir_grafo(Grafo* grafo);

#endif