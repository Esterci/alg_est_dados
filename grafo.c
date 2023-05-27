#include <stdio.h>
#include <stdlib.h>

typedef struct lista_duplamente_encadeada
{
  struct lista_duplamente_encadeada *prev;
  int key;
  struct lista_duplamente_encadeada *next;
} list;

typedef struct nos
{
  struct nos *prev;
  int key;
  list *arestas;
  struct nos *next;
} grafo;

void list_insert(list **L, list *x)
{
  x->next = *L;
  if (*L != NULL)
    (*L)->prev = x;
  *L = x;
  x->prev = NULL;
}

void list_delete(list **L, list *x)
{
  if (x->prev != NULL)
    x->prev->next = x->next;
  else
    *L = x->next;
  if (x->next != NULL)
    x->next->prev = x->prev;
}

list *list_search(list *L, int k)
{
  list *x = L;
  while (x != NULL && x->key != k)
    x = x->next;
  return x;
}

void print_list(list *L)
{
  list *x = L;
  while (x != NULL)
  {
    printf("%i -> ", x->key);
    x = x->next;
  }
  printf("\n");
}

list *allocate_object(int k)
{
  list *x = (list *)malloc(sizeof(struct lista_duplamente_encadeada));
  x->key = k;
  x->next = NULL;
  x->prev = NULL;
  return x;
}

void free_object(list *x)
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

grafo *allocate_graph(int k)
{
  grafo *x = (grafo *)malloc(sizeof(struct nos));
  x->key = k;
  x->arestas = NULL;
  x->next = NULL;
  x->prev = NULL;
  return x;
}

void free_graph(grafo *x)
{
  list *aux = x->arestas;
  while (aux != NULL)
  {
    list_delete(&x->arestas, aux);
    free_object(aux);
    aux = x->arestas;
  }
  free(x);
}

void print_graph(grafo *g)
{
  grafo *auxg = g;
  while (auxg != NULL)
  {
    printf("No: %i \n\t Arestas: ", auxg->key);

    list *auxl = auxg->arestas;
    while (auxl != NULL)
    {
      printf("%i -> ", auxl->key);
      auxl = auxl->next;
    }
    printf("\n");
    auxg = auxg->next;
  }
}

int main(void)
{
  int opc, chave;
  grafo *head = NULL, *aux = NULL;
  list *aux2 = NULL;
  do
  {
    printf("1 - Insere No; 2 - Remove No; 3 - Procura No; 4 - Imprime Grafo; 5 - Insere Aresta; 6 - Remove Aresta; 7 - Procura Aresta ; 8 - Fim\n");
    scanf("%i", &opc);
    switch (opc)
    {
    case 1: // 1 - Insere No;
      printf("Numero do No: ");
      scanf("%i", &chave);
      node_insert(&head, allocate_graph(chave));
      printf("\nInserido!\n");

      break;
    case 2: // 2 - Remove No;
      printf("Valor: ");
      scanf("%i", &chave);
      aux = graph_search(head, chave);
      
      if (aux != NULL)
      {
        node_delete(&head, aux);
        free_graph(aux);
        printf("\nRemovido!\n");
      }
      else
        printf("\nValor nao encontrado!\n");
      break;
    case 3: // 3 - Procura No;
      printf("Valor: ");
      scanf("%i", &chave);
      aux = graph_search(head, chave);
      if (aux != NULL)
        printf("\nValor encontrado!\n");
      else
        printf("\nValor nao encontrado!\n");
      break;
    case 4: // 4 - Imprime Grafo;
      print_graph(head);
      break;
    case 5:  // 5 - Insere Aresta;
      printf("Numero do No: ");
      scanf("%i", &chave);
      aux = graph_search(head, chave);
      if (aux != NULL)
      {
        printf("Numero da aresta: ");
        scanf("%i", &chave);
        list_insert(&aux->arestas, allocate_object(chave));
        printf("\nInserido!\n");
      }
      else
      {
        printf("\nNo nao encontrado!\n");
      }
      break;
    case 6: // 6 - Remove Aresta;
      printf("Numero do No: ");
      scanf("%i", &chave);
      aux = graph_search(head, chave);
      if (aux != NULL)
      {
        printf("Numero da aresta: ");
        scanf("%i", &chave);
        aux2 = list_search(aux->arestas, chave);
        if (aux2 != NULL)
        {
          list_delete(&aux->arestas, aux2);
          free_object(aux2);
          printf("\nRemovido!\n");
        }
        else
        {
          printf("\nAresta nao encontrada!\n");
        }
      }
      else
        printf("\nNo nao encontrado!\n");
      break;
    case 7:  // 7 - Procura Aresta ;
      printf("Numero do No: ");
      scanf("%i", &chave);
      aux = graph_search(head, chave);
      if (aux != NULL)
      {
        printf("Numero da aresta: ");
        scanf("%i", &chave);
        aux2 = list_search(aux->arestas, chave);
        if (aux2 != NULL)
        {
          printf("\nAresta encontrada!\n");
        }
        else
        {
          printf("\nAresta nao encontrada!\n");
        }
      }
      else
        printf("\nNo nao encontrado!\n");
      break;

    case 8: // 8 - Fim;
      printf("\nFim!\n\n");
      break;
    default:
      printf("\nOpcao Invalida!\n");
      break;
    }
  } while (opc != 8);
}
