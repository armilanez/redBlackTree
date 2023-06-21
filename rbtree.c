// Arthur Araujo Rios Milanez
// Yan Bruno do Vale Gomes

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum cores
{
  RUBRO,
  NEGRO
};

typedef struct no
{
  int id, quantidade, cor;
  char nomeProduto[50];
  struct no *filho[2];
  // Os filhos serão armazenados num array de 2 espaços.
  // O filho à esquerda é o filho[0] e o filho à direita é o filho[1].
}No;

No *raiz = NULL;

// Create a RUBRO-NEGRO tree
No *criaNo(int id, int quantidade, char *nomeProduto)
{
  No *novoNo;
  novoNo = (No *)malloc(sizeof(No));

  novoNo->cor = RUBRO;
  novoNo->id = id;
  novoNo->quantidade = quantidade;
  strcpy(novoNo->nomeProduto, nomeProduto);
  
  novoNo->filho[0] = NULL;
  novoNo->filho[1] = NULL;
  
  return novoNo;
}

// Função responsável por inserir um nó na árvore.
void insereNo(int id, int quantidade, char *nomeProduto)
{
  No *arvore[98], *atravessador, *novoNo, *atual, *aux;
  int caminhoPercorrido[98], altura = 0, index;
  atravessador = raiz;
  if (!raiz)
  {
    raiz = criaNo(id, quantidade, nomeProduto);
    return;
  }

  arvore[altura] = raiz;
  caminhoPercorrido[altura++] = 0;
  while (atravessador != NULL)
  {

    if(atravessador->id == id) {
      printf("Cada produto deve ter um ID único. \n");
      return;
    }
    
    if((id - atravessador->id) > 0){
      index = 1;
    }
    else {
      index = 0;
    }
    arvore[altura] = atravessador;
    atravessador = atravessador->filho[index];
    caminhoPercorrido[altura++] = index;
  }
  arvore[altura - 1]->filho[index] = novoNo = criaNo(id, quantidade, nomeProduto);
  while ((altura >= 3) && (arvore[altura - 1]->cor == RUBRO))
  {
    if (caminhoPercorrido[altura - 2] == 0)
    {
      aux = arvore[altura - 2]->filho[1];
      if (aux != NULL && aux->cor == RUBRO)
      {
        arvore[altura - 2]->cor = RUBRO;
        arvore[altura - 1]->cor = aux->cor = NEGRO;
        altura = altura - 2;
      }
      else
      {
        if (caminhoPercorrido[altura - 1] == 0)
        {
          aux = arvore[altura - 1];
        }
        else
        {
          atual = arvore[altura - 1];
          aux = atual->filho[1];
          atual->filho[1] = aux->filho[0];
          aux->filho[0] = atual;
          arvore[altura - 2]->filho[0] = aux;
        }
        atual = arvore[altura - 2];
        atual->cor = RUBRO;
        aux->cor = NEGRO;
        atual->filho[0] = aux->filho[1];
        aux->filho[1] = atual;
        if (atual == raiz)
        {
          raiz = aux;
        }
        else
        {
          arvore[altura - 3]->filho[caminhoPercorrido[altura - 3]] = aux;
        }
        break;
      }
    }
    else
    {
      aux = arvore[altura - 2]->filho[0];
      if ((aux != NULL) && (aux->cor == RUBRO))
      {
        arvore[altura - 2]->cor = RUBRO;
        arvore[altura - 1]->cor = aux->cor = NEGRO;
        altura = altura - 2;
      }
      else
      {
        if (caminhoPercorrido[altura - 1] == 1)
        {
          aux = arvore[altura - 1];
        }
        else
        {
          atual = arvore[altura - 1];
          aux = atual->filho[0];
          atual->filho[0] = aux->filho[1];
          aux->filho[1] = atual;
          arvore[altura - 2]->filho[1] = aux;
        }
        atual = arvore[altura - 2];
        aux->cor = NEGRO;
        atual->cor = RUBRO;
        atual->filho[1] = aux->filho[0];
        aux->filho[0] = atual;
        if (atual == raiz)
        {
          raiz = aux;
        }
        else
        {
          arvore[altura - 3]->filho[caminhoPercorrido[altura - 3]] = aux;
        }
        break;
      }
    }
  }
  raiz->cor = NEGRO;
}

// Delete a no
void removeNo(int id)
{
  No *arvore[98], *atual, *aux;

  //O ponteiro atravessador é o que comumente vemos em aula como T.
  //É aquele ponteiro que vai atravessando e traçando o caminho percorrido pela árvore.
  No *atravessador;

  No *paiAtual, *filhoAtual, *irmaoAtual;
  int caminhoPercorrido[98], altura = 0, direcao, i;
  enum cores cor;

  if (!raiz)
  {
    printf("Raíz nula.\n");
    return;
  }

  atravessador = raiz;
  while (atravessador != NULL)
  {
    if ((id - atravessador->id) == 0)
      break;

    /*A array direcao[] vai indicar se iremos descer para a esquerda ou direita.
      drecao[0] significa descer à esquerda e direcao[1] significa descer à direita.*/
    if((id - atravessador->id) > 0) {
      direcao = 1;
    }
    else {
      direcao = 0;
    }
    
    arvore[altura] = atravessador;
    caminhoPercorrido[altura++] = direcao;
    atravessador = atravessador->filho[direcao];
  }

  if (atravessador->filho[1] == NULL)
  {
    if ((atravessador == raiz) && (atravessador->filho[0] == NULL))
    {
      free(atravessador);
      raiz = NULL;
    }
    else if (atravessador == raiz)
    {
      raiz = atravessador->filho[0];
      free(atravessador);
    }
    else
    {
      arvore[altura - 1]->filho[caminhoPercorrido[altura - 1]] = atravessador->filho[0];
    }
  }
  else
  {
    atual = atravessador->filho[1];
    if (atual->filho[0] == NULL)
    {
      atual->filho[0] = atravessador->filho[0];
      cor = atual->cor;
      atual->cor = atravessador->cor;
      atravessador->cor = cor;

      if (atravessador == raiz)
      {
        raiz = atual;
      }
      else
      {
        arvore[altura - 1]->filho[caminhoPercorrido[altura - 1]] = atual;
      }

      caminhoPercorrido[altura] = 1;
      arvore[altura++] = atual;
    }
    else
    {
      i = altura++;
      while (1)
      {
        caminhoPercorrido[altura] = 0;
        arvore[altura++] = atual;
        aux = atual->filho[0];
        if (!aux->filho[0])
          break;
        atual = aux;
      }

      caminhoPercorrido[i] = 1;
      arvore[i] = aux;
      if (i > 0)
        arvore[i - 1]->filho[caminhoPercorrido[i - 1]] = aux;

      aux->filho[0] = atravessador->filho[0];

      atual->filho[0] = aux->filho[1];
      aux->filho[1] = atravessador->filho[1];

      if (atravessador == raiz)
      {
        raiz = aux;
      }

      cor = aux->cor;
      aux->cor = atravessador->cor;
      atravessador->cor = cor;
    }
  }

  if (altura < 1)
    return;

  if (atravessador->cor == NEGRO)
  {
    while (1)
    {
      paiAtual = arvore[altura - 1]->filho[caminhoPercorrido[altura - 1]];
      if (paiAtual != NULL && paiAtual->cor == RUBRO)
      {
        paiAtual->cor = NEGRO;
        break;
      }

      if (altura < 2)
        break;

      if (caminhoPercorrido[altura - 2] == 0)
      {
        irmaoAtual = arvore[altura - 1]->filho[1];

        if (!irmaoAtual)
          break;

        if (irmaoAtual->cor == RUBRO)
        {
          arvore[altura - 1]->cor = RUBRO;
          irmaoAtual->cor = NEGRO;
          arvore[altura - 1]->filho[1] = irmaoAtual->filho[0];
          irmaoAtual->filho[0] = arvore[altura - 1];

          if (arvore[altura - 1] == raiz)
          {
            raiz = irmaoAtual;
          }
          else
          {
            arvore[altura - 2]->filho[caminhoPercorrido[altura - 2]] = irmaoAtual;
          }
          caminhoPercorrido[altura] = 0;
          arvore[altura] = arvore[altura - 1];
          arvore[altura - 1] = irmaoAtual;
          altura++;

          irmaoAtual = arvore[altura - 1]->filho[1];
        }

        if ((!irmaoAtual->filho[0] || irmaoAtual->filho[0]->cor == NEGRO) &&
            (!irmaoAtual->filho[1] || irmaoAtual->filho[1]->cor == NEGRO))
        {
          irmaoAtual->cor = RUBRO;
        }
        else
        {
          if (!irmaoAtual->filho[1] || irmaoAtual->filho[1]->cor == NEGRO)
          {
            filhoAtual = irmaoAtual->filho[0];
            irmaoAtual->cor = RUBRO;
            filhoAtual->cor = NEGRO;
            irmaoAtual->filho[0] = filhoAtual->filho[1];
            filhoAtual->filho[1] = irmaoAtual;
            irmaoAtual = arvore[altura - 1]->filho[1] = filhoAtual;
          }
          irmaoAtual->cor = arvore[altura - 1]->cor;
          arvore[altura - 1]->cor = NEGRO;
          irmaoAtual->filho[1]->cor = NEGRO;
          arvore[altura - 1]->filho[1] = irmaoAtual->filho[0];
          irmaoAtual->filho[0] = arvore[altura - 1];
          if (arvore[altura - 1] == raiz)
          {
            raiz = irmaoAtual;
          }
          else
          {
            arvore[altura - 2]->filho[caminhoPercorrido[altura - 2]] = irmaoAtual;
          }
          break;
        }
      }
      else
      {
        irmaoAtual = arvore[altura - 1]->filho[0];
        if (!irmaoAtual)
          break;

        if (irmaoAtual->cor == RUBRO)
        {
          arvore[altura - 1]->cor = RUBRO;
          irmaoAtual->cor = NEGRO;
          arvore[altura - 1]->filho[0] = irmaoAtual->filho[1];
          irmaoAtual->filho[1] = arvore[altura - 1];

          if (arvore[altura - 1] == raiz)
          {
            raiz = irmaoAtual;
          }
          else
          {
            arvore[altura - 2]->filho[caminhoPercorrido[altura - 2]] = irmaoAtual;
          }
          caminhoPercorrido[altura] = 1;
          arvore[altura] = arvore[altura - 1];
          arvore[altura - 1] = irmaoAtual;
          altura++;

          irmaoAtual = arvore[altura - 1]->filho[0];
        }
        if ((!irmaoAtual->filho[0] || irmaoAtual->filho[0]->cor == NEGRO) &&
            (!irmaoAtual->filho[1] || irmaoAtual->filho[1]->cor == NEGRO))
        {
          irmaoAtual->cor = RUBRO;
        }
        else
        {
          if (!irmaoAtual->filho[0] || irmaoAtual->filho[0]->cor == NEGRO)
          {
            filhoAtual = irmaoAtual->filho[1];
            irmaoAtual->cor = RUBRO;
            filhoAtual->cor = NEGRO;
            irmaoAtual->filho[1] = filhoAtual->filho[0];
            filhoAtual->filho[0] = irmaoAtual;
            irmaoAtual = arvore[altura - 1]->filho[0] = filhoAtual;
          }
          irmaoAtual->cor = arvore[altura - 1]->cor;
          arvore[altura - 1]->cor = NEGRO;
          irmaoAtual->filho[0]->cor = NEGRO;
          arvore[altura - 1]->filho[0] = irmaoAtual->filho[1];
          irmaoAtual->filho[1] = arvore[altura - 1];
          if (arvore[altura - 1] == raiz)
          {
            raiz = irmaoAtual;
          }
          else
          {
            arvore[altura - 2]->filho[caminhoPercorrido[altura - 2]] = irmaoAtual;
          }
          break;
        }
      }
      altura--;
    }
  }
}

// Percorre toda a árvore imprimindo seus valores.
void imprimeArvore(No *no)
{
  if (no != NULL)
  {
    imprimeArvore(no->filho[0]);
    printf("%d: %s - %d unidades.\n", no->id, no->nomeProduto, no->quantidade);
    imprimeArvore(no->filho[1]);
  }
  return;
}

int existe(int id) {

  No *atravessador;
  atravessador = raiz;
  int index;

  while (atravessador != NULL)
  {

    if(atravessador->id == id) {
      return 1;
    }
    
    if((id - atravessador->id) > 0){
      index = 1;
    }
    else {
      index = 0;
    }
    atravessador = atravessador->filho[index];
  }
}


void alteraQuantidade(int escolhaAlteracaoQtd, int id, int quantiaDeAlteracao)
{
  No *atravessador;
  atravessador = raiz;
  int index;

  while (atravessador != NULL)
  {

    if(atravessador->id == id) {
      if (escolhaAlteracaoQtd == 1) {
        atravessador->quantidade += quantiaDeAlteracao;
        return;
      }
      else if (escolhaAlteracaoQtd == 2){
        if ((atravessador->quantidade - quantiaDeAlteracao) < 0) {
          printf("Proibido reduzir a quantia de um produto para menos que 0.\n");
        }
        else {
          atravessador->quantidade -= quantiaDeAlteracao;
          return;
        }
  
      }
      else {
        printf("Opção não existente.");
        return;
      }
    }
    
    if((id - atravessador->id) > 0){
      index = 1;
    }
    else {
      index = 0;
    }
    atravessador = atravessador->filho[index];
  }
}

// Função main, onde a árvore é operada.
int main()
{
  int id, escolha, quantidade, existencia;
  char nomeProduto[50];
  while (1)
  {
    printf("-------------------------------------------------------\n");
    printf("Olá. Que operação deseja realizar?\n\n");
    printf("1) Cadastrar um novo Produto.\n");
    printf("2) Excluir um produto cadastrado.\n");
    printf("3) Atualizar a quantidade de um produto no estoque.\n");
    printf("6) Imprimir a Árvore Rubro-Negra.\n");
    printf("7) Sair.\n");
    printf("8) Consultar existência.\n\n");
    printf("Escolha: ");
    scanf("%d", &escolha);
    switch (escolha)
    {
    case 1:
      printf("Insira o ID do novo produto: ");
      scanf("%d", &id);
      printf("Insira o nome do produto: ");
      scanf("%s", nomeProduto);
      printf("Insira a quantidade desejada: ");
      scanf("%d", &quantidade);
      insereNo(id, quantidade, nomeProduto);
      printf("\n");
      break;
    case 2:
      printf("Insira o ID do elemento que deseja excluir: ");
      scanf("%d", &id);
      removeNo(id);
      printf("\n");
      break;
    case 3:
      printf("Escolha o ID do produto que deseja alterar a quantidade:\n\n");
      imprimeArvore(raiz);
      scanf("%d", &id);

      if (existencia = existe(id) == 1) {

        printf("Você deseja adicionar ou subtrair a quantidade deste produto?\n");
        printf("1) Adicionar.\n2) Subtrair.\n");
        int escolhaAlteracaoQtd, quantiaDeAlteracao;
        scanf("%d", &escolhaAlteracaoQtd);

        switch(escolhaAlteracaoQtd){
          case 1: //Adição
            printf("Em quantas unidades?\n");
            scanf("%d", &quantiaDeAlteracao);
            alteraQuantidade(escolhaAlteracaoQtd, id, quantiaDeAlteracao);
            break;

          case 2:
            printf("Em quantas unidades?\n");
            scanf("%d", &quantiaDeAlteracao);
            alteraQuantidade(escolhaAlteracaoQtd, id, quantiaDeAlteracao);
            break;

          default:
            printf("Opção não existente.\n");
            break;

        }  
      }
      else {
        printf("Produto não cadastrado.\n");
      }
      break;
    case 6:
      imprimeArvore(raiz);
      printf("\n");
      break;
    case 7:
      printf("Até mais!\n");
      exit(0);

    //Case bônus
    case 8:
      printf("Qual o ID do produto que você gostaria de consultar a existência?\n");
      scanf("%d", &id);
      existencia = existe(id);

      if (existencia == 1) {
        printf("Existe.\n");
      }
      else {
        printf("Não existe.\n");
      }
      break;

    default:
      printf("Opção não existente.\n");
      break;
    }
    printf("\n");
  }
  return 0;
}