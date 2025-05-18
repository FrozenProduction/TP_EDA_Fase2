/**
 * @file mapa.h
 * @brief Utilitários para carregamento e manipulação de mapas de antenas
 * 
 * @details Implementa as operações de:
 * - Carregamento de mapas a partir de ficheiros de texto
 * - Representação de mapas na memória
 * - Conversão entre mapas e grafos de antenas
 * - Visualização de mapas com antenas e efeitos nefastos
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

#ifndef MAPA_H
#define MAPA_H
  
#include "grafo.h"

/**
 * @struct Dimensoes
 * @brief Armazena as dimensões de um mapa (número de linhas e colunas)
 */
typedef struct Dimensoes Dimensoes;

/**
 * @struct Mapa
 * @brief Representa uma linha do mapa para visualização
 */
typedef struct Mapa Mapa;

/**
 * @struct Dimensoes
 * @brief Contém as dimensões de um mapa de antenas
 */
struct Dimensoes {
    int linhas;     ///< Número de linhas do mapa
    int colunas;    ///< Número de colunas do mapa
};

/**
 * @struct Mapa
 * @brief Estrutura para armazenar linhas do mapa durante a visualização
 */
struct Mapa {
    char* dados;    ///< Array de caracteres representando uma linha do mapa
    Mapa* prox;     ///< Apontador para a próxima linha do mapa
};
  
/**
 * @brief Carrega um mapa a partir de um ficheiro e converte para grafo
 * @param ficheiro Nome do ficheiro contendo o mapa
 * 
 * @details O formato do ficheiro deve ser:
 * - Primeira linha: número de linhas e colunas
 * - Linhas seguintes: representação do mapa com:
 *   - '.' para posições vazias
 *   - Caracteres alfanuméricos para representar antenas
 */
Grafo* carregar_mapa(const char* ficheiro);

/**
 * @brief Imprime um mapa na consola com as antenas e efeitos nefastos
 * @param grafo Apontador para o grafo contendo as antenas
 * @param linhas Número de linhas do mapa
 * @param colunas Número de colunas do mapa
 * 
 * @details A representação visual usa:
 * - Caracteres das antenas para as suas posições
 * - '#' para posições com efeito nefasto
 * - '.' para posições vazias
 */
void imprimir_mapa(Grafo* grafo, int linhas, int colunas);
  
#endif // MAPA_H