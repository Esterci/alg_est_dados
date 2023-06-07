#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declarando as estruturas e funções

typedef struct lista_duplamente_encadeada
{
  struct lista_duplamente_encadeada *prev;
  int destiny;
  int distance;
  struct lista_duplamente_encadeada *next;
} aresta;

typedef struct nos
{
  struct nos *prev;
  int key;
  int opens_at;
  int closes_at;
  int pred;
  int tempo;
  int final;
  aresta *adjacentes;
  struct nos *next;
} grafo;

// Declarando funcoes utilizadas

void aresta_insert(aresta **L, aresta *x);

void aresta_delete(aresta **L, aresta *x);

void print_aresta(aresta *L);

aresta *allocate_aresta(int dny, int dist);

void free_aresta(aresta *x);

grafo *graph_search(grafo *L, int k);

grafo *min_time_graph(grafo *L);

void node_insert(grafo **G, grafo *x);

void node_delete(grafo **G, grafo *x);

grafo *allocate_graph(int k, int o, int c);

void free_graph(grafo *x);

void print_graph(grafo *g);

void djikstra(grafo **G, grafo *init, int start_time);

void print_fastest_way(grafo *g);

void print_format_time(int t);

// Codigo principal

int main(void)
{
  int n_nos = 6;
  int n_arestas = 8;
  int no_origem = 1;
  int no_final = 6;
  int tempo_inicial = 7 * 60 + 30;
  grafo *head = NULL, *aux = NULL;
  aresta *aux2 = NULL;

  // Declarando nos

  int nos_matrix[7][3] = {
      {1, 8 * 60, 12 * 60 + 30},
      {2, 7 * 60, 14 * 60},
      {3, 14 * 60, 21 * 60},
      {4, 8 * 60, 10 * 60},
      {5, 0, 0},
      {6, 7 * 60, 19 * 60},
  };

  // Declarando arestas

  int aresta_matrix[13][3] = {
      {1, 2, 5},
      {1, 3, 15},
      {2, 4, 5},
      {2, 5, 5},
      {3, 4, 5},
      {3, 5, 5},
      {4, 6, 15},
      {5, 6, 5},
  };

  // Inserindo nos no grafo
  printf("Inserindo nos...\n");
  for (int i = 0; i < n_nos; i++)
  {
    node_insert(&head, allocate_graph(nos_matrix[i][0], nos_matrix[i][1], nos_matrix[i][2]));
    printf("\tNo %d inserido.\n", i + 1);
  };

  // Inserindo arestas no grafo
  printf("\n\nInserindo arestas...\n");
  for (int i = 0; i < n_arestas; i++)
  {
    aux = graph_search(head, aresta_matrix[i][0]);
    if (aux != NULL)
    {
      aresta_insert(&aux->adjacentes, allocate_aresta(aresta_matrix[i][1], aresta_matrix[i][2]));
      printf("\tAresta %d inserida.\n", i + 1);
    }
    else
    {
      printf("\nNo nao encontrado!\n");
    }
  };

  // aplicando algoritmo de djikstra

  djikstra(&head, graph_search(head, no_origem), tempo_inicial);

  // Mostrando grafo resultante

  printf("\n\nO grafo resultante é:\n");
  print_graph(head);

  // printf("\n\nO melhor caminho é:\n");
  // print_fastest_way(head);

  // limpando estruturas

  free_graph(head);
  free_graph(aux);
  free_aresta(aux2);
}

// Definindo funcoes

void aresta_insert(aresta **L, aresta *x)
{
  x->next = *L;
  if (*L != NULL)
    (*L)->prev = x;
  *L = x;
  x->prev = NULL;
}

void aresta_delete(aresta **L, aresta *x)
{
  if (x->prev != NULL)
    x->prev->next = x->next;
  else
    *L = x->next;
  if (x->next != NULL)
    x->next->prev = x->prev;
}

void print_aresta(aresta *L)
{
  aresta *x = L;
  while (x != NULL)
  {
    printf("%i -> ", x->destiny);
    x = x->next;
  }
  printf("\n");
}

aresta *allocate_aresta(int dny, int dist)
{
  aresta *x = (aresta *)malloc(sizeof(struct lista_duplamente_encadeada));
  x->destiny = dny;
  x->distance = dist;
  x->next = NULL;
  x->prev = NULL;
  return x;
}

void free_aresta(aresta *x)
{
  free(x);
}

grafo *graph_search(grafo *L, int k)
{
  grafo *x = L;
  while (x != NULL && x->key != k)
    x = x->next;
  return x;
}

grafo *min_time_graph(grafo *L)
{
  grafo *aux = L, *x = L;

  int min_time = x->tempo;

  while (aux != NULL && aux->tempo >= min_time)
  {
    aux = aux->next;

    if (aux != NULL && aux->tempo < min_time && aux->final == 0)
    {
      min_time = aux->tempo;
      x = aux;
    }
  }
  return x;
}

void node_insert(grafo **G, grafo *x)
{
  x->next = *G;
  if (*G != NULL)
    (*G)->prev = x;
  *G = x;
  x->prev = NULL;
}

void node_delete(grafo **G, grafo *x)
{
  if (x->prev != NULL)
    x->prev->next = x->next;
  else
    *G = x->next;
  if (x->next != NULL)
    x->next->prev = x->prev;
}

grafo *allocate_graph(int k, int o, int c)
{
  grafo *x = (grafo *)malloc(sizeof(struct nos));
  x->prev = NULL;
  x->key = k;
  x->opens_at = o;
  x->closes_at = c;
  x->pred = -1;
  x->tempo = 2147483647; // maxima representacao de um inteiro apox. infinito
  x->final = 0;
  x->adjacentes = NULL;
  x->next = NULL;
  return x;
}

void free_graph(grafo *x)
{
  aresta *aux = x->adjacentes;
  while (aux != NULL)
  {
    aresta_delete(&x->adjacentes, aux);
    free_aresta(aux);
    aux = x->adjacentes;
  }
  free(x);
}

void print_graph(grafo *g)
{
  grafo *auxg = g;
  while (auxg != NULL)
  {

    printf("No: %d\n\t Predecessor: %d", auxg->key, auxg->pred);
    printf("\n\t Horário de entrada: ");
    print_format_time(auxg->opens_at);
    printf("\n\t Horário de entrada: ");
    print_format_time(auxg->closes_at);
    printf("\n\t Tempo ate o no: ");
    print_format_time(auxg->tempo);
    printf("\n\t Arestas: ");

    aresta *auxl = auxg->adjacentes;
    while (auxl != NULL)
    {
      printf("%i -> ", auxl->destiny);
      auxl = auxl->next;
    }
    printf("\n");
    auxg = auxg->next;
  }
}

void djikstra(grafo **G, grafo *init, int start_time)
{
  grafo *aux_graph = NULL;
  aresta *aux_adj = NULL;

  if (init != NULL && init->final != 1)
  {
    int tempo_chegada = 0;
    aux_adj = init->adjacentes;

    // inicializando nó inicial análise

    if (init->next == NULL)
    {
      init->tempo = start_time;
    }

    init->final = 1;

    while (aux_adj != NULL)
    {
      aux_graph = graph_search((*G), aux_adj->destiny);

      tempo_chegada = aux_adj->distance * 60 + init->tempo;

      if (tempo_chegada < aux_graph->tempo)
      {
        aux_graph->tempo = tempo_chegada;
        aux_graph->pred = init->key;
      }

      aux_adj = aux_adj->next;
    }
    djikstra(G, min_time_graph(*G), start_time);
  }
}

void print_fastest_way(grafo *g)
{
  grafo *auxg = g;
  while (auxg != NULL)
  {
    printf("No %d\t- Horario de chegada: %d \n", auxg->key, auxg->tempo);
    printf("\t ^\n");
    printf("\t/ \\\n");
    printf("\t |\n");
    printf("\t |\n");
    auxg = graph_search(g, auxg->pred);
  }
  printf("\n\n");
}

void print_format_time(int t)
{
  // Checa se passaram dias

  int dias = (int)(t / (24 * 60));
  int horas = (int)(t % (24 * 60) / 60);
  int minutos = (int)(t % (24 * 60) % 60);

  if (dias > 0)
  {
    printf("%d dias, ", dias);
  }
  printf("%d horas e % d minutos", horas, minutos);
};