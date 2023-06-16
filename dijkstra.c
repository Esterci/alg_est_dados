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
  int block_start;
  int block_end;
  int pred;
  int tempo;
  int final;
  aresta *adjacentes;
  struct nos *next;
} grafo;

// Declarando funcoes utilizadas

void aresta_insert(aresta **L, aresta *x);

void aresta_delete(aresta **L, aresta *x);

aresta *allocate_aresta(int dny, int dist);

void free_aresta(aresta *x);

grafo *graph_search(grafo *L, int k);

grafo *min_time_graph(grafo *L);

void node_insert(grafo **G, grafo *x);

grafo *allocate_graph(int k, int o, int c);

void free_graph(grafo *x);

void print_graph(grafo *g);

void djikstra(grafo **G, grafo *init, int start_time, int no_final);

void print_fastest_way(grafo *g, int chegada, int partida);

void print_format_time(int t);

int conv_str_to_minutes(char *t);

// Codigo principal

int main(void)
{
  FILE *arquivo;
  int i = 0;
  int i_n, i_a, j;

  int n_nos, n_arestas, no_origem, no_final;
  int tempo_inicial;
  int chave_no;
  char tempo1[5];
  char tempo2[5];
  int block_start;
  int block_end;
  int aresta_start_node;
  int aresta_end_node;
  int aresta_weight;
  grafo *head = NULL, *aux = NULL;
  aresta *aux2 = NULL;

  // Abrindo o arquivo
  arquivo = fopen("dados.txt", "r");
  if (arquivo == NULL)
  {
    printf("Erro ao abrir o arquivo.\n");
    return 1;
  }

  fscanf(arquivo, "%d", &n_nos);
  fscanf(arquivo, "%d", &n_arestas);
  fscanf(arquivo, "%d", &no_origem);
  fscanf(arquivo, "%d", &no_final);
  fscanf(arquivo, "%s", tempo1);

  // Convertendo tempo inicial
  tempo_inicial = conv_str_to_minutes(tempo1);

  // Lendo os dados dos nos no arquivo e insere nos no grafo
  printf("Inserindo nos...\n");
  for (i_n = 0; i_n < n_nos; i_n++)
  {
    fscanf(arquivo, "%d %s %s", &chave_no, tempo1, tempo2);

    // Convertendo inicio do bloqueio para minutos

    block_start = conv_str_to_minutes(tempo1);
    block_end = conv_str_to_minutes(tempo2);

    // Criando no
    node_insert(&head, allocate_graph(chave_no, block_start, block_end));
    printf("\tNo %d inserido.\n", i + 1);
  }

  // Lendo os dados das arestas no arquivo e inserindo arestas no grafo
  printf("\n\nInserindo arestas...\n");
  for (i_a = 0; i_a < n_arestas; i_a++)
  {
    fscanf(arquivo, "%d %d %d", &aresta_start_node, &aresta_end_node, &aresta_weight);

    aux = graph_search(head, aresta_start_node);
    if (aux != NULL)
    {
      aresta_insert(&aux->adjacentes, allocate_aresta(aresta_end_node, aresta_weight));
      printf("\tAresta %d inserida.\n", i + 1);
    }
    else
    {
      printf("\nNo nao encontrado!\n");
    }
  }

  // Fechando o arquivo
  fclose(arquivo);

  // aplicando algoritmo de djikstra

  djikstra(&head, graph_search(head, no_origem), tempo_inicial, no_final);

  // Mostrando grafo resultante

  printf("\n\nO grafo resultante é:\n");
  print_graph(head);

  printf("\n\nO melhor caminho é:\n");
  print_fastest_way(head, no_origem, no_final);

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

  while (aux != NULL)
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

grafo *allocate_graph(int k, int o, int c)
{
  grafo *x = (grafo *)malloc(sizeof(struct nos));
  x->prev = NULL;
  x->key = k;
  x->block_start = o;
  x->block_end = c;
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
    print_format_time(auxg->block_start);
    printf("\n\t Horário de entrada: ");
    print_format_time(auxg->block_end);
    printf("\n\t Tempo ate o no: ");
    print_format_time(auxg->tempo);
    printf("\n\t Arestas: ");

    aresta *auxl = auxg->adjacentes;
    while (auxl != NULL)
    {
      printf("%i, %d -> ", auxl->destiny, auxl->distance);
      auxl = auxl->next;
    }
    printf("\n");
    auxg = auxg->next;
  }
}

void djikstra(grafo **G, grafo *init, int start_time, int no_final)
{
  grafo *aux_graph = NULL;
  aresta *aux_adj = NULL;

  if (init != NULL && init->key != no_final)
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

      // Checar se posso sair da cidade de partida
      if (init->block_end != init->block_start)
      {
        if (init->block_start > init->block_end)
        {
          if (init->tempo % (24 * 60) > init->block_start && init->tempo % (24 * 60) < (init->block_start + 24 * 60))
            tempo_chegada += init->block_start + 24 * 60 - init->tempo % (24 * 60);
        }
        else
        {
          if (init->tempo % (24 * 60) > init->block_start && init->tempo % (24 * 60) < init->block_end)
            tempo_chegada += init->block_end - init->tempo % (24 * 60);
        }
      }


      // Checando horário de saída na chegada
      if (aux_graph->block_end != aux_graph->block_start)
      {
        if (aux_graph->block_start > aux_graph->block_end)
        {
          if (tempo_chegada % (24 * 60) > aux_graph->block_start && tempo_chegada % (24 * 60) < (aux_graph->block_start + 24 * 60))
            tempo_chegada += aux_graph->block_start + 24 * 60 - tempo_chegada % (24 * 60);
        }
        else
        {
          if (tempo_chegada % (24 * 60) > aux_graph->block_start && tempo_chegada % (24 * 60) < aux_graph->block_end)
            tempo_chegada += aux_graph->block_end - tempo_chegada % (24 * 60);
        }
      }

      if (tempo_chegada < aux_graph->tempo)
      {
        aux_graph->tempo = tempo_chegada;
        aux_graph->pred = init->key;
      }

      aux_adj = aux_adj->next;
    }
    djikstra(G, min_time_graph(*G), start_time, no_final);
  }
}

void print_fastest_way(grafo *g, int inicio, int final)
{
  int partida, chegada;
  grafo *auxg = g;
  while (auxg != NULL)
  {
    if (auxg->key == final)
    {
      chegada = auxg->tempo;
      printf("No %d\t- Horario de partida: ", auxg->key);
    }
    else
      printf("No %d\t- Horario de chegada: ", auxg->key);
    if (auxg->key == inicio)
      partida = auxg->tempo;
    print_format_time(auxg->tempo);
    printf("\n\t ^\n");
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
  printf("%d horas e % d minutos : %d", horas, minutos,t);
};

int conv_str_to_minutes(char *t)
{
  char t_hora[3], t_min[3];
  int t_vh, t_vm;

  // Dividindo as strings em duas partes iguais

  strncpy(t_hora, t, 2);
  t_hora[2] = '\0';
  strncpy(t_min, t + 2, 2);
  t_min[2] = '\0';

  t_vh = atoi(t_hora);
  t_vm = atoi(t_min);

  // Convertendo as partes divididas em minutos e as unindo

  return t_vh * 60 + t_vm;
}