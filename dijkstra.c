#include <stdio.h>
#include <stdlib.h>

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

aresta *aresta_search(aresta *L, int k);

void print_aresta(aresta *L);

aresta *allocate_aresta(int dny, int dist);

void free_object(aresta *x);

grafo *graph_search(grafo *L, int k);

void node_insert(grafo **G, grafo *x);

void node_delete(grafo **G, grafo *x);

grafo *allocate_graph(int k, int op, int cls);

void free_graph(grafo *x);

void print_graph(grafo *g);

// Codigo principal

int main(void)
{
  int opc, chave;
  int n_nos = 3;
  int n_arestas = 2;
  int no_origem = 1;
  int no_final = 6;
  grafo *head = NULL, *aux = NULL;
  aresta *aux2 = NULL;

  // Inserindo nos no grafo
  printf("Inserindo nos...\n");
  for (int i = n_nos; i >= 0; i--)
  {
    node_insert(&head, allocate_graph(i + 1,1,1));
    printf("\tNo %d inserido.\n", i + 1);
  };

  // Inserindo arestas no grafo
  printf("\nInserindo arestas...\n");
  for (int i = 0; i < n_arestas; i++)
  {
    aux = graph_search(head, 1);
    if (aux != NULL)
    {
      aresta_insert(&aux->adjacentes, allocate_aresta(1, i + 1));
      printf("\tAresta %d inserida.\n", i + 1);
    }
    else
    {
      printf("\nNo nao encontrado!\n");
    }
  };

  // Mostrando grafo resultante
  printf("O grafo resultante é:\n");
  print_graph(head);
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

aresta *aresta_search(aresta *L, int k)
{
  aresta *x = L;
  while (x != NULL && x->destiny != k)
    x = x->next;
  return x;
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

grafo *allocate_graph(int k, int op, int cls)
{
  grafo *x = (grafo *)malloc(sizeof(struct nos));
  x->prev = NULL;
  x->key = k;
  x->opens_at = op;
  x->closes_at = cls;
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
    printf("No: %i \n\t Arestas: ", auxg->key);

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