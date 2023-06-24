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
} No;

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

  // A árvore será inclusa numa pilha. Essa abordagem de vetores faz com que
  // possamos fazer o mesmo na estrutura do nó, armazenando os filhos também
  // em vetores. Essa abordagem sem referência para o pai é mais custosa
  // computacionalmente falando, porém economiza espaço na memória.
  No *arvore[100], *atravessador, *novoNo, *atual, *aux;
  int caminhoPercorrido[100], altura = 0, direcao;
  atravessador = raiz;
  if (!raiz)
  {
    raiz = criaNo(id, quantidade, nomeProduto);
    raiz->cor = NEGRO;
    return;
  }

  arvore[altura] = raiz;
  caminhoPercorrido[altura++] = 0;

  // While que percorre a árvore até chegar numa folha.
  while (atravessador != NULL)
  {

    if (atravessador->id == id)
    {
      printf("Cada produto deve ter um ID único. \n");
      return;
    }

    if ((id - atravessador->id) > 0)
    {
      direcao = 1;
    }
    else
    {
      direcao = 0;
    }
    arvore[altura] = atravessador;
    atravessador = atravessador->filho[direcao];
    caminhoPercorrido[altura++] = direcao;
  }

  // Onde acontece a inserção de fato
  arvore[altura - 1]->filho[direcao] = novoNo = criaNo(id, quantidade, nomeProduto);

  // Fixups
  while ((altura >= 3) && (arvore[altura - 1]->cor == RUBRO))
  {
    if (caminhoPercorrido[altura - 2] == 0)
    {
      aux = arvore[altura - 2]->filho[1];

      // Caso 1: Tio rubro, apenas recoloração
      if (aux != NULL && aux->cor == RUBRO)
      {
        arvore[altura - 2]->cor = RUBRO;
        arvore[altura - 1]->cor = aux->cor = NEGRO;
        altura = altura - 2;
      }
      else
      {
        // Caso 2: Se o tio do nó atual é negro e o nó atual é um filho à direita

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

      // Caso 3: Tio rubro, recolore
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
        // Caso 4: Tio negro e nó atual é filho à esquerda
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

  // Raíz é sempre negra
  raiz->cor = NEGRO;
}

// Delete a no
void removeNo(int id)
{
  No *arvore[100], *atual, *aux;

  // O ponteiro atravessador é o que comumente vemos em aula como T.
  // É aquele ponteiro que vai atravessando e traçando o caminho percorrido pela árvore.
  No *atravessador;

  No *paiAtual, *filhoAtual, *irmaoAtual;

  // A variável caminhoPercorrido[] guarda o caminho percorrido, com
  // valores de 0 e 1. Se temos, por exemplo, o vetor caminhoPercorrido[1,0,1],
  // isso significa que o caminho feito foi direita, esquerda, direita.
  int caminhoPercorrido[100], altura = 0, direcao, i;
  enum cores cor;

  if (!raiz)
  {
    printf("Raíz nula.\n");
    return;
  }

  atravessador = raiz;

  // Laço while que irá encontrar o nó a ser excluído
  while (atravessador != NULL)
  {
    if ((id - atravessador->id) == 0)
      break;

    /*A array direcao[] vai indicar se iremos descer para a esquerda ou direita.
      drecao[0] significa descer à esquerda e direcao[1] significa descer à direita.*/
    if ((id - atravessador->id) > 0)
    {
      direcao = 1;
    }
    else
    {
      direcao = 0;
    }

    arvore[altura] = atravessador;               // Adiciona nó na árvore
    caminhoPercorrido[altura++] = direcao;       // Adiciona direção percorrida no caminhoPercorrido[]
    atravessador = atravessador->filho[direcao]; // Vai pro próximo nó
  }

  // Aqui serão tratados os casos quando o nó a ser deletado
  //*NÃO* tem um filho à direita.
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

    // Caso 1: Nó tem filho à direita
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

      // Acha o sucessor do nó a ser deletado
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

  // Caso 2: Nó é negro e tem filho rubro
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

        // Caso 3: Nó é negro, não tem filho rubro, e irmão é negro
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

        // Caso 4: Nó é negro, não tem filho rubro, irmão é negro, e sobrinhos são negros
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

      // Aqui acontecem a mesma coisa, são casos análogos aos anteriores.
      // A diferença é que o irmão está à esquerda.
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

// Função bônus implementada para facilitar saber se um nó existe na stack ou não.
// Se existir, retorna 1.
int existe(int id)
{

  No *atravessador;
  atravessador = raiz;
  int direcao;

  while (atravessador != NULL)
  {

    if (atravessador->id == id)
    {
      return 1;
    }

    if ((id - atravessador->id) > 0)
    {
      direcao = 1;
    }
    else
    {
      direcao = 0;
    }
    atravessador = atravessador->filho[direcao];
  }
}

// Função que altera o atributo quantidade de um nó.
void alteraQuantidade(int escolhaAlteracaoQtd, int id, int quantiaDeAlteracao)
{
  No *atravessador;
  atravessador = raiz;
  int direcao;

  while (atravessador != NULL)
  {

    if (atravessador->id == id)
    {
      // Se a escolhaAlteraçãoQtd for 1, adiciona. Se for 2, reduz.
      if (escolhaAlteracaoQtd == 1)
      {
        atravessador->quantidade += quantiaDeAlteracao;
        return;
      }
      else if (escolhaAlteracaoQtd == 2)
      {
        if ((atravessador->quantidade - quantiaDeAlteracao) < 0)
        {
          printf("Proibido reduzir a quantia de um produto para menos que 0.\n");
        }
        else
        {
          atravessador->quantidade -= quantiaDeAlteracao;
          return;
        }
      }
      else
      {
        printf("Opção não existente.");
        return;
      }
    }

    if ((id - atravessador->id) > 0)
    {
      direcao = 1;
    }
    else
    {
      direcao = 0;
    }
    atravessador = atravessador->filho[direcao];
  }
}

// Imprime todos os elementos da árvore
void imprimeEmEstoque(No *no)
{
  if (no != NULL)
  {
    imprimeEmEstoque(no->filho[0]);

    //A diferença dessa da outra função de imprimir a árvore
    //é que nós com quantidade 0 não serão impressos.
    if (no->quantidade > 0)
    {
      printf("%d: %s - %d unidades.\n", no->id, no->nomeProduto, no->quantidade);
    }
    imprimeEmEstoque(no->filho[1]);
  }
  return;
}


//Função que vai retornar a altura da árvore.
//Importante para a próxima função, de desenhar a árvore.
int getAltura(No *no)
{
  if (no == NULL){
    return 0;
  }

  //Aqui as alturas das sub árvores direita e esquerda serão calculadas recursivamente.
  int alturaEsquerda = getAltura(no->filho[0]);
  int alturaDireita = getAltura(no->filho[1]);

  //Essa expressão ternária vai decidir se vai retornar
  //a altura direita ou esquerda, vai retornar  a que
  //for maior. Aquele "1 + " é pra incluir a raíz.
  return 1 + (alturaEsquerda > alturaDireita ? alturaEsquerda : alturaDireita);
}


//Função que vai desenhar a Árvore no terminal.
void desenhaArvore(No *no, int nivel)
{
  if (no != NULL)
  {
    //Percorre até a direita, e volta recursivamente imprimindo
    //"da direita pra esquerda."
    desenhaArvore(no->filho[1], nivel + 1);
    for (int i = 0; i < nivel; i++)
    {
      printf("\t");
    }

    //Cor: 1 para negro e 0 para rubro.
    printf("%d | %d\n", no->id, no->cor);

    //Mesmo caso da linha 591, porém para a sub árvore esquerda.
    desenhaArvore(no->filho[0], nivel + 1);
  }
}

// Função main, onde a árvore é operada.
int main()
{
  int id, escolha, quantidade, existencia, altura;
  char nomeProduto[50];
  while (1)
  {
    printf("-------------------------------------------------------------------\n");
    printf("Olá, bem-vindo à UFES Import. Que operação deseja realizar?\n");
    printf("-------------------------------------------------------------------\n");
    printf("1) Cadastrar um novo Produto.\n");
    printf("2) Excluir um produto cadastrado.\n");
    printf("3) Atualizar a quantidade de um produto no estoque.\n");
    printf("4) Listar todos os produtos cadastrados.\n");
    printf("5) Listar todos os produtos em estoque.\n");
    printf("6) Imprimir Árvore Rubro-Negra.\n");
    printf("7) Sair.\n");
    printf("8) Consultar existência.\n");
    printf("9) Altura da árvore.\n\n");
    printf("-------------------------------------------------------------------\n");
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

      if (existencia = existe(id) == 1)
      {

        printf("Você deseja adicionar ou subtrair a quantidade deste produto?\n");
        printf("1) Adicionar.\n2) Subtrair.\n");
        int escolhaAlteracaoQtd, quantiaDeAlteracao;
        scanf("%d", &escolhaAlteracaoQtd);

        switch (escolhaAlteracaoQtd)
        {
        case 1:
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
      else
      {
        printf("Produto não cadastrado.\n");
      }
      break;

    case 4:
      imprimeArvore(raiz);
      printf("\n");
      break;

    case 5:
      imprimeEmEstoque(raiz);
      printf("\n");
      break;

    case 6:
      altura = getAltura(raiz);
      desenhaArvore(raiz, altura);
      break;

    case 7:
      printf("Até mais!\n");
      exit(0);

    // Case bônus
    case 8:
      printf("Qual o ID do produto que você gostaria de consultar a existência?\n");
      scanf("%d", &id);
      existencia = existe(id);

      if (existencia == 1)
      {
        printf("Existe.\n");
      }
      else
      {
        printf("Não existe.\n");
      }
      break;

    case 9:
      altura = getAltura(raiz);
      printf("Altura %d.\n\n", altura);
    break;

    default:
      printf("Opção não existente.\n");
      break;
    }
    printf("\n");
  }
  return 0;
}