#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct letra{
  char letra;
  struct letra *prox;
  struct palavra *arvoreBinaria;
};
typedef struct letra LETRA;

struct palavra{
  char palavra[30];
  int ocorrencias;
  struct palavra *esq;
  struct palavra *dir;
};

typedef struct palavra PALAVRA;

struct indice{
  struct letra *inicio;
  struct letra *fim;
};

typedef struct indice INDICE;


struct nodoLista{
  PALAVRA *raiz;
  struct nodoLista *prox;
  struct nodoLista *ant;
};

typedef struct nodoLista FILA;

void inicializa_tabela(INDICE tabela[]){
  for(int i = 0; i < 26; i++){
    tabela[i].inicio = NULL;
    tabela[i].fim = NULL;
  }
}


void insere_letras(INDICE tabela[]){
  for(int i = 97; i < 123; i++){
    LETRA *nodo = (LETRA *)malloc(sizeof(LETRA));
    nodo -> letra = i;
    nodo -> prox = NULL;
    nodo -> arvoreBinaria = NULL;
    

    int chave = i % 26;

    if(tabela[chave].inicio == NULL){
      tabela[chave].inicio = nodo;
      tabela[chave].fim = nodo;
    } else {
      tabela[chave].fim -> prox = nodo;
      tabela[chave].fim = nodo;
    }
  }
}

void escreve_tabela(INDICE tabela[]){
    int i;

    for(i=0; i<26; i++){
        printf("Resto %d\n", i);
        LETRA *aux = tabela[i].inicio;
        while ( aux != NULL ){
            printf("%c\n", aux-> letra);
            aux = aux->prox;
        }
        
    }
}


int buscaSequencial(int resto, INDICE *vetor){
  return resto % 26;
}
  
void insercao(PALAVRA **raiz, char *palavra){
  if(*raiz == NULL){
    PALAVRA *nodo = (PALAVRA*)malloc(sizeof(PALAVRA));
    strcpy(nodo -> palavra, palavra);
    nodo -> esq = NULL;
    nodo -> dir = NULL;
    nodo -> ocorrencias = 1;
    *raiz = nodo;
  } else if(strcmp((*raiz) -> palavra, palavra) == 0) {
    printf("Esse elemento já existe na árvore\n");
    (*raiz) -> ocorrencias++;
    return;
  } else if(strcmp(palavra, (*raiz) -> palavra) < 0){
    insercao(&(*raiz) -> esq, palavra);
  } else {
    insercao(&(*raiz) -> dir, palavra);
  }
}

void remocao(PALAVRA **raiz, char *palavra){ 
  if(*raiz == NULL){
    return;
  } else if(strcmp((*raiz) -> palavra, palavra) > 0) {
    remocao(&(*raiz) -> esq, palavra);
    }else if(strcmp((*raiz)-> palavra, palavra) < 0){
    remocao(&(*raiz) -> dir, palavra);
    }else if((*raiz) -> esq == NULL && (*raiz) -> dir == NULL){
    free(*raiz);
    *raiz = NULL;
  } else if((*raiz) -> esq == NULL){
    PALAVRA *aux = *raiz;
    *raiz = (*raiz) -> dir;
    free(aux);
  } else if((*raiz) -> dir == NULL){
    PALAVRA *aux = *raiz;
    *raiz = (*raiz) -> esq;
    free(aux);
  } else {
     PALAVRA *aux = (*raiz)->esq;
            while (aux->dir != NULL ){
                aux = aux->dir;
            }
            strcpy((*raiz) -> palavra, aux -> palavra);
            remocao(&(*raiz)->esq, aux->palavra);

  }
}

void inserirFila(FILA **inicio, FILA **fim, PALAVRA *raiz){
  FILA *nodo = (FILA*)malloc(sizeof(FILA));
  nodo -> raiz = raiz;
  nodo -> prox = NULL;
  nodo -> ant = NULL;
  
  if(*inicio == NULL){
    *inicio = nodo;
    *fim = nodo;
  } else {
    nodo -> ant = *fim;
    (*fim) -> prox = nodo;
    *fim = nodo;
  }
}

int removeFila(FILA **inicio, FILA **fim){
  int valor;
  if(*inicio == NULL){
    return 0;
  } else if(*inicio == *fim){
    FILA *aux = *inicio;
    *inicio = NULL;
    *fim = NULL;
    valor = aux -> raiz -> ocorrencias;
    free(aux);
    return valor;
  } else {
    FILA *aux = *inicio;
    (*inicio) -> prox -> ant = NULL;
    *inicio = (*inicio) -> prox;
    valor = aux -> raiz -> ocorrencias;
    return valor;
    free(aux);
  }
}

int percurso_em_nivel(PALAVRA *raiz){
      FILA *inicio = NULL, *fim = NULL;
      int aux1 = 0;
      int aux2 = 0;
  
      if(raiz == NULL){
        return 0;
      }
  
      inserirFila(&inicio, &fim, raiz);
      while(inicio != NULL){
      aux1 = removeFila(&inicio, &fim);
        if(aux1 > aux2){
          aux2 = aux1;
        }
        if(raiz -> esq != NULL){
          inserirFila(&inicio, &fim, raiz -> esq);
        }

        if(raiz -> dir != NULL){
          inserirFila(&inicio, &fim, raiz -> dir);
        }
      }
  
    return aux2;
  }

void em_ordem(PALAVRA *raiz){
  if(raiz != NULL){
    em_ordem(raiz -> esq);
    printf("%s\n", raiz -> palavra);
    em_ordem(raiz -> dir);
  }
}

void maior_ocorrencias(PALAVRA *raiz, int maior){
  if(raiz != NULL) {
    maior_ocorrencias(raiz -> esq, maior);
    if(raiz -> ocorrencias == maior){
      printf("%s\n", raiz -> palavra);
    }
    maior_ocorrencias(raiz -> dir, maior);
    }
}


void em_ordem_uma_ocorrencia(PALAVRA *raiz){
  if(raiz != NULL){
    em_ordem_uma_ocorrencia(raiz -> esq);
    if(raiz -> ocorrencias == 1){
      printf("%s\n", raiz -> palavra);
    }
    em_ordem_uma_ocorrencia(raiz -> dir);
    }
}

int quantidade_nos(PALAVRA *raiz){
    if(raiz == NULL){
      return 0;
    } else{
      return 1 + quantidade_nos(raiz -> esq) + quantidade_nos(raiz -> dir);
    }
}
  
int somar(INDICE tabela[26]){
  int soma = 0;
  for(int i = 0; i < 26; i++){
    soma += quantidade_nos(tabela[i].inicio -> arvoreBinaria);
  }
  return soma;
  }

int somar_ocorrencias(INDICE tabela[26]){
  int soma = 0;
  for(int i = 0; i < 26; i++){
    
    if(tabela[i].inicio -> arvoreBinaria == NULL){
      continue;
    }
    soma+=tabela[i].inicio -> arvoreBinaria -> ocorrencias;
  }

  return soma;
  }
  
void menu(){
  
  printf("\n");
  printf("===================\n");
  printf("0.Sair\n");
  printf("1.Inserir\n");
  printf("2.Remover palavra\n");
  printf("3.Ocorrências\n");
  printf("4.Contar palavras\n");
  printf("5.Contar número de ocorrencias\n");
  printf("6.Mostrar palavras em ordem alfabetica\n");
  printf("7.Mostrar todas as palavras de uma letra\n");
  printf("8.Mostrar palavras com o maior numero de ocorrencias\n");
  printf("9.Mostrar palavras com apenas 1 ocorrencia\n");
  printf("===================\n");
  printf("\n");
}

int main(void) {

  INDICE tabela[26];
  
  inicializa_tabela(tabela);

  insere_letras(tabela);


  char *result;
  
  int i;

  int soma = 0;

  int maior = 0;
  
  char buscar;

  char *pt;

  char aux[30];

  char linha[100];
  
  char op = '.';
  char palavras[100];
  FILE *arq;
  while(op != '0'){
    
    
    menu();
    scanf(" %c", &op);
    fflush(stdin);
    
    switch(op){
      case '1':
        
        scanf(" %100[^\n]", palavras);
        printf("\n");
        
        pt = strtok(palavras, " ");
        while(pt){
           i = buscaSequencial((int)pt[0], tabela);
           insercao(&tabela[i].inicio -> arvoreBinaria, pt);
           pt = strtok(NULL, " ");
        }
        
        break;

      case '2':

        scanf("%s", aux);
        i = buscaSequencial((int)aux[0], tabela);

        remocao(&tabela[i].inicio -> arvoreBinaria, aux);
                     
      break;
    
    case '3':
        scanf("%s", aux);
        i = buscaSequencial((int)aux[0], tabela);
       
        if(tabela[i].inicio -> arvoreBinaria == NULL){
          printf("%d\n", 0);
        } else {
          printf("%s - %d\n", aux ,tabela[i].inicio -> arvoreBinaria -> ocorrencias);  
        }
        
      break;

      case '4':
        soma = somar(tabela);

        printf("O número total de palavras é: %d\n", soma);
        
        break;

      case '5':

        soma = somar_ocorrencias(tabela);

        printf("A soma total das ocorrencias é: %d\n", soma);
        
      break;

      case '6':
        for(int i = 97; i < 123; i++){
          int chave = i % 26;
          em_ordem(tabela[chave].inicio -> arvoreBinaria);
        }

      break;

      case '7':
        scanf(" %c", &buscar);
        int chave = (int)buscar % 26;
        em_ordem(tabela[chave].inicio -> arvoreBinaria);
        
      break;

      case '8':
        for(int i = 97; i < 123; i++){
          chave = i % 26;
          if(percurso_em_nivel(tabela[chave].inicio -> arvoreBinaria) > maior){
            maior = percurso_em_nivel(tabela[chave].inicio -> arvoreBinaria);
          }
        }

      for(int i = 0; i < 26; i++){
        maior_ocorrencias(tabela[i].inicio -> arvoreBinaria, maior);
      }
        
      break;

      case '9':
        for(int i = 97; i < 123; i++){
          chave = i % 26;
          em_ordem_uma_ocorrencia(tabela[chave].inicio -> arvoreBinaria);
        }
      break;
      
      }
  }
  return 0;
}