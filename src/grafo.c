/**
 * @file grafo.c
 * @brief Implementação das operações para gestão de grafos de antenas
 * 
 * @details Implementa todas as funções declaradas em grafo.h, incluindo:
 * - Criação e destruição de grafos
 * - Operações básicas com vértices e arestas
 * - Algoritmos de travessia (DFS e BFS)
 * - Cálculo de caminhos entre vértices
 * - Deteção de intersecções entre frequências
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
#include <math.h>
#include "grafo.h"

static int contador_caminhos; ///< Contador auxiliar para número de caminhos encontrados

/**
 * @brief Cria um novo grafo vazio
 * @return Apontador para o grafo criado ou NULL em caso de erro
 * 
 * @details Aloca memória para a estrutura do grafo e inicializa:
 * - Lista de vértices como NULL
 * - Contador de vértices como 0
 */
Grafo* criar_grafo() {
    Grafo* grafo = (Grafo*)malloc(sizeof(Grafo));
    grafo->vertices = NULL;
    grafo->num_vertices = 0;
    return grafo;
}

/**
 * @brief Liberta toda a memória associada ao grafo
 * @param grafo Apontador para o grafo a destruir
 * @return 0 em caso de sucesso, -1 em caso de erro
 * 
 * @details Percorre e liberta:
 * - Todos os vértices (antenas)
 * - Todas as arestas (conexões)
 * - A própria estrutura do grafo
 */
int destruir_grafo(Grafo* grafo) {
    if (!grafo) return -1;
    
    Vertice* v = grafo->vertices;
    while (v != NULL) {
        Vertice* temp_v = v;
        v = v->proximo;
        
        Aresta* a = temp_v->arestas;
        while (a != NULL) {
            Aresta* temp_a = a;
            a = a->proxima;
            free(temp_a);
        }
        
        free(temp_v);
    }
    free(grafo);
    return 0;
}

/**
 * @brief Adiciona um novo vértice (antena) ao grafo
 * @param grafo Apontador para o grafo
 * @param freq Frequência da antena (caracter)
 * @param x Coordenada x (coluna) da antena
 * @param y Coordenada y (linha) da antena
 * 
 * @details A nova antena é inserida no início da lista de vértices
 */
Vertice* adicionar_vertice(Grafo* grafo, char freq, int x, int y) {
    if (!grafo) return NULL;
    
    Vertice* novo = (Vertice*)malloc(sizeof(Vertice));
    if (!novo) return NULL;  
    novo->frequencia = freq;
    novo->x = x;
    novo->y = y;
    novo->visitado = false;
    novo->arestas = NULL;
    novo->proximo = grafo->vertices;
    grafo->vertices = novo;
    grafo->num_vertices++;
    return novo;
}

/**
 * @brief Adiciona uma aresta entre dois vértices (antenas da mesma frequência)
 * @param origem Vértice de origem
 * @param destino Vértice de destino
 * @return 0 em caso de sucesso, -1 em caso de erro, -2 se frequências diferentes
 * 
 * @details A aresta é bidirecional (grafo não direcionado) e não são permitidas:
 * - Arestas entre vértices com frequências diferentes
 * - Arestas duplicadas entre o mesmo par de vértices
 */
int adicionar_aresta(Vertice* origem, Vertice* destino) {
    if (!origem || !destino) return -1;
    if (origem->frequencia != destino->frequencia) return -2;
    
    // Verificar se aresta já existe
    Aresta* a = origem->arestas;
    while (a != NULL) {
        if (a->destino == destino) return 0;
        a = a->proxima;
    }
    
    // Adicionar aresta origem->destino
    Aresta* nova = (Aresta*)malloc(sizeof(Aresta));
    if (!nova) return -3;
    nova->destino = destino;
    nova->proxima = origem->arestas;
    origem->arestas = nova;
    
    // Adicionar aresta destino->origem (grafo não direcionado)
    nova = (Aresta*)malloc(sizeof(Aresta));
    if (!nova) return -4;
    nova->destino = origem;
    nova->proxima = destino->arestas;
    destino->arestas = nova;
    
    return 0;
}

/**
 * @brief Função recursiva auxiliar para procura em profundidade (DFS)
 * @param v Vértice atual a visitar
 * @return 0 em caso de sucesso, -1 em caso de erro
 * 
 * @details Implementa DFS recursivo, marcando vértices como visitados
 * e imprimindo as coordenadas à medida que visita
 */
int procura_profundidade_rec(Vertice* v) {
    if (v == NULL) return -1;
    if (v->visitado) return 0;
    
    printf("Visitando: %c (%d,%d)\n", v->frequencia, v->x, v->y);
    v->visitado = true;
    
    Aresta* a = v->arestas;
    while (a != NULL) {
        procura_profundidade_rec(a->destino);
        a = a->proxima;
    }
    return 0;
}

/**
 * @brief Executa uma procura em profundidade (DFS) a partir de um vértice
 * @param grafo Apontador para o grafo
 * @param inicio Vértice de início da procura
 * @return 0 em caso de sucesso, -1 em caso de erro
 * 
 * @details Prepara a DFS reiniciando os marcadores de visita
 * antes de iniciar a procura recursiva
 */
int procura_profundidade(Grafo* grafo, Vertice* inicio) {
    if (!grafo || !inicio) return -1;
    reiniciar_visitados(grafo);
    return procura_profundidade_rec(inicio);
}

/**
 * @brief Executa uma procura em largura (BFS) a partir de um vértice
 * @param grafo Apontador para o grafo
 * @param inicio Vértice de início da procura
 * @return 0 em caso de sucesso, -1 em caso de erro
 * 
 * @details Implementa BFS usando uma fila para visitar os vértices
 * por níveis de proximidade ao vértice inicial
 */
int procura_largura(Grafo* grafo, Vertice* inicio) {
    if (!grafo || !inicio) return -1;
    reiniciar_visitados(grafo);
    
    NodeFila* inicio_fila = NULL;
    NodeFila* fim_fila = NULL;
    
    // Adicionar início na fila
    NodeFila* novo = (NodeFila*)malloc(sizeof(NodeFila));
    if (!novo) return -2;
    novo->vertice = inicio;
    novo->prox = NULL;
    inicio_fila = fim_fila = novo;
    inicio->visitado = true;
    
    while (inicio_fila != NULL) {
        Vertice* atual = inicio_fila->vertice;
        printf("Visitando: %c (%d,%d)\n", atual->frequencia, atual->x, atual->y);
        
        // Remover da fila
        NodeFila* temp = inicio_fila;
        inicio_fila = inicio_fila->prox;
        if (inicio_fila == NULL) fim_fila = NULL;
        
        // Adicionar vizinhos
        Aresta* a = atual->arestas;
        while (a != NULL) {
            if (!a->destino->visitado) {
                a->destino->visitado = true;
                NodeFila* novo_vizinho = (NodeFila*)malloc(sizeof(NodeFila));
                if (!novo_vizinho) return -3;
                novo_vizinho->vertice = a->destino;
                novo_vizinho->prox = NULL;
                
                if (fim_fila == NULL) {
                    inicio_fila = fim_fila = novo_vizinho;
                } else {
                    fim_fila->prox = novo_vizinho;
                    fim_fila = novo_vizinho;
                }
            }
            a = a->proxima;
        }
        free(temp);
    }
    return 0;
}

/**
 * @brief Imprime um caminho na ordem inversa (auxiliar para encontrar_caminhos_rec)
 * @param caminho Apontador para o nó do caminho a imprimir
 * 
 * @details Usa recursão para imprimir o caminho desde o fim até ao início
 */
void imprimir_caminho_inverso(CaminhoNode* caminho) {
    if (caminho == NULL) return;
    imprimir_caminho_inverso(caminho->prox);
    printf("%c(%d,%d) ", caminho->vertice->frequencia, caminho->vertice->x, caminho->vertice->y);
}

/**
 * @brief Função recursiva auxiliar para encontrar todos os caminhos entre dois vértices
 * @param atual Vértice atual na procura
 * @param destino Vértice de destino
 * @param caminho_atual Caminho percorrido até ao momento
 * @return 0 em caso de sucesso, -1 em caso de erro
 * 
 * @details Implementa DFS modificado para encontrar todos os caminhos possíveis,
 * usando backtracking e marcadores de visita
 */
int encontrar_caminhos_rec(Vertice* atual, Vertice* destino, CaminhoNode* caminho_atual) {
    if (!atual || !destino) return -1;
    
    // Adicionar vértice atual ao caminho
    CaminhoNode* novo_no = (CaminhoNode*)malloc(sizeof(CaminhoNode));
    if (!novo_no) return -2;
    novo_no->vertice = atual;
    novo_no->prox = caminho_atual;
    
    if (atual == destino) {
        contador_caminhos++;
        printf("Caminho %d: ", contador_caminhos);
        imprimir_caminho_inverso(novo_no);
        printf("\n");
    } else {
        atual->visitado = true;
        Aresta* a = atual->arestas;
        while (a != NULL) {
            if (!a->destino->visitado) {
                encontrar_caminhos_rec(a->destino, destino, novo_no);
            }
            a = a->proxima;
        }
        atual->visitado = false;
    }
    
    free(novo_no);
    return 0;
}

/**
 * @brief Encontra e imprime todos os caminhos entre duas antenas
 * @param grafo Apontador para o grafo
 * @param origem Vértice de origem
 * @param destino Vértice de destino
 * @return 0 em caso de sucesso, -1 em caso de erro
 * 
 * @details Verifica previamente:
 * - Existência dos vértices
 * - Se têm a mesma frequência
 * - Prepara o contador de caminhos e marcadores de visita
 */
int encontrar_caminhos(Grafo* grafo, Vertice* origem, Vertice* destino) {
    if (!grafo) return -1;
    
    if (origem == NULL || destino == NULL) {
        printf("\nNao foi possivel encontrar caminhos:\n");
        if (origem == NULL && destino == NULL) {
            printf("- Ambas as antenas nao existem no mapa\n");
        } else if (origem == NULL) {
            printf("- Antena de origem nao existe no mapa\n");
        } else {
            printf("- Antena de destino nao existe no mapa\n");
        }
        return -2;
    }
    
    if (origem->frequencia != destino->frequencia) {
        printf("\nNao existem caminhos entre %c(%d,%d) e %c(%d,%d)\n", 
               origem->frequencia, origem->x, origem->y,
               destino->frequencia, destino->x, destino->y);
        printf("- As antenas tem frequencias diferentes (%c e %c)\n", 
               origem->frequencia, destino->frequencia);
        return -3;
    }
    
    contador_caminhos = 0;
    reiniciar_visitados(grafo);
    
    printf("\n=== Caminhos entre %c(%d,%d) e %c(%d,%d) ===\n",
           origem->frequencia, origem->x, origem->y,
           destino->frequencia, destino->x, destino->y);
           
    int resultado = encontrar_caminhos_rec(origem, destino, NULL);
    
    if (contador_caminhos == 0) {
        printf("Nenhum caminho encontrado entre as antenas\n");
    } else {
        printf("Total de caminhos encontrados: %d\n", contador_caminhos);
    }
    
    return resultado;
}

/**
 * @brief Calcula o ponto de intersecção entre duas linhas definidas por pares de pontos
 * @param p1, p2 Pontos da primeira linha (freqA)
 * @param p3, p4 Pontos da segunda linha (freqB)
 * @param[out] x, y Coordenadas da interseção (se existir)
 * @return true se as linhas se intersectam, false caso contrário
 * 
 * @details Implementa o algoritmo de interseção de segmentos de linha
 * usando álgebra linear para determinar o ponto de interseção
 */
bool calcular_intersecao(Vertice* p1, Vertice* p2, Vertice* p3, Vertice* p4, int* x, int* y) {
    // Calcula os denominadores
    int denom = (p4->y - p3->y) * (p2->x - p1->x) - (p4->x - p3->x) * (p2->y - p1->y);
    
    // Linhas paralelas
    if (denom == 0) return false;
    
    // Calcula os parâmetros
    float ua = ((p4->x - p3->x) * (p1->y - p3->y) - (p4->y - p3->y) * (p1->x - p3->x)) / (float)denom;
    float ub = ((p2->x - p1->x) * (p1->y - p3->y) - (p2->y - p1->y) * (p1->x - p3->x)) / (float)denom;
    
    // Verifica se a interseção está dentro dos segmentos
    if (ua < 0 || ua > 1 || ub < 0 || ub > 1) return false;
    
    // Calcula o ponto de interseção
    *x = p1->x + ua * (p2->x - p1->x);
    *y = p1->y + ua * (p2->y - p1->y);
    
    return true;
}

/**
 * @brief Encontra e imprime todas as intersecções entre pares de antenas de duas frequências
 * @param grafo Apontador para o grafo
 * @param freqA Primeira frequência a considerar
 * @param freqB Segunda frequência a considerar
 * @return Número de intersecções encontradas
 * 
 * @details Compara todos os pares de arestas das duas frequências
 * e verifica se as linhas que os representam se intersectam
 */
int intersecoes_frequencias(Grafo* grafo, char freqA, char freqB) {
    Intersecao* intersecoes = NULL;
    int count = 0;
    
    Vertice* a1 = grafo->vertices;
    while (a1 != NULL) {
        if (a1->frequencia == freqA) {
            Aresta* arestaA = a1->arestas;
            while (arestaA != NULL) {
                Vertice* a2 = arestaA->destino;
                if (a1 < a2) {
                    Vertice* b1 = grafo->vertices;
                    while (b1 != NULL) {
                        if (b1->frequencia == freqB) {
                            Aresta* arestaB = b1->arestas;
                            while (arestaB != NULL) {
                                Vertice* b2 = arestaB->destino;
                                if (b1 < b2) {
                                    int x, y;
                                    if (calcular_intersecao(a1, a2, b1, b2, &x, &y)) {
                                        bool existe = false;
                                        Intersecao* atual = intersecoes;
                                        while (atual != NULL) {
                                            if (atual->x == x && atual->y == y) {
                                                existe = true;
                                                break;
                                            }
                                            atual = atual->prox;
                                        }
                                        
                                        if (!existe) {
                                            Intersecao* nova = malloc(sizeof(Intersecao));
                                            nova->x = x;
                                            nova->y = y;
                                            nova->prox = intersecoes;
                                            intersecoes = nova;
                                            count++;
                                            
                                            if (count == 1) {
                                                printf("\n=== Intersecoes entre frequencias de %c e %c ===\n", freqA, freqB);
                                            }
                                            
                                            printf("Linha %c(%d,%d)-%c(%d,%d) com ", 
                                                   freqA, a1->x, a1->y, freqA, a2->x, a2->y);
                                            printf("%c(%d,%d)-%c(%d,%d) em (%d,%d)\n",
                                                   freqB, b1->x, b1->y, freqB, b2->x, b2->y, x, y);
                                        }
                                    }
                                }
                                arestaB = arestaB->proxima;
                            }
                        }
                        b1 = b1->proximo;
                    }
                }
                arestaA = arestaA->proxima;
            }
        }
        a1 = a1->proximo;
    }
    
    while (intersecoes != NULL) {
        Intersecao* temp = intersecoes;
        intersecoes = intersecoes->prox;
        free(temp);
    }
    
    return count;
}

/**
 * @brief Encontra um vértice no grafo pelas suas coordenadas
 * @param grafo Apontador para o grafo
 * @param x Coordenada x (coluna) do vértice
 * @param y Coordenada y (linha) do vértice
 * 
 * @details Percorre a lista de vértices até encontrar um com as coordenadas especificadas
 */
Vertice* encontrar_vertice(Grafo* grafo, int x, int y) {
    Vertice* v = grafo->vertices;
    while (v != NULL) {
        if (v->x == x && v->y == y) return v;
        v = v->proximo;
    }
    return NULL;
}

/**
 * @brief Reinicia o estado 'visitado' de todos os vértices do grafo
 * @param grafo Apontador para o grafo
 * @return 0 em caso de sucesso, -1 em caso de erro
 * 
 * @details Pré-requisito para algoritmos de travessia do grafo
 */
int reiniciar_visitados(Grafo* grafo) {
    if (!grafo) return -1;
    
    Vertice* v = grafo->vertices;
    while (v != NULL) {
        v->visitado = false;
        v = v->proximo;
    }
    return 0;
}

/**
 * @brief Imprime a representação do grafo na consola
 * @param grafo Apontador para o grafo a imprimir
 * 
 * @details Mostra cada vértice e as suas conexões num formato legível
 */
void imprimir_grafo(Grafo* grafo) {
    if (grafo == NULL) {
        printf("Grafo inválido (NULL)\n");
        return;
    }
    
    printf("\nGrafo (%d antenas):\n", grafo->num_vertices);
    
    Vertice* v = grafo->vertices;
    while (v != NULL) {
        printf("Antena %c (%d,%d) -> ", v->frequencia, v->x, v->y);
        Aresta* a = v->arestas;
        
        if (a == NULL) {
            printf("Sem conexões");
        } else {
            while (a != NULL) {
                printf("%c(%d,%d)", a->destino->frequencia, a->destino->x, a->destino->y);
                a = a->proxima;
                if (a != NULL) printf("  ");
            }
        }
        printf("\n");
        v = v->proximo;
    }
}