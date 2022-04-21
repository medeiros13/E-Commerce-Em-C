#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//Definição dos registros
typedef struct
{
	int codigo;
	char nome[100];
	int quantidade;
	float preco;
} Produto;

//Declaração das variáveis globais
Produto *produtosEmEstoque = NULL, *produtosNoCarrinho = NULL;
int quantProdutosEstoque = 0, quantProdutosCarrinho = 0;

//Funções do módulo Estoque
void mostrarProdutosEmEstoque(bool modoVitrine)
{
	if(modoVitrine)
	{
		for(int i = 0; i < quantProdutosEstoque; i++)
		{
			printf("\n----------------------");
			printf("\nNome: %s",produtosEmEstoque[i].nome);
			printf("\nPreco: R$%.2f",produtosEmEstoque[i].preco);
		}
		return;
	}
	
	for(int i = 0; i < quantProdutosEstoque; i++)
	{
		printf("\n----------------------");
		printf("\nCodigo: %d",produtosEmEstoque[i].codigo);
		printf("\nQuantidade: %d",produtosEmEstoque[i].quantidade);
		printf("\nNome: %s",produtosEmEstoque[i].nome);
		printf("\nPreco: R$%.2f",produtosEmEstoque[i].preco);
	}
}

bool validarProdutoDigitado(int posicaoAtualPonteiro, int codDigitado, char nomeDigitado[100])
{
	for(int i = 0; i < posicaoAtualPonteiro; i++)
	{
		if (produtosEmEstoque[i].codigo == codDigitado)
		{
			printf("O código '%d' já existe em estoque!",codDigitado);
			return false;
		}
		if (strcmp(produtosEmEstoque[i].nome, nomeDigitado) == 0)
		{
			printf("O nome '%s' já existe em estoque!", nomeDigitado);
			return false;
		}
	}
	return true;
}

void leProdutosEstoque(int posicaoInicial, int posicaoFinal)
{
	int codDigitado, quantDigitada;
	char nomeDigitado[100];
	float precoDigitado;
	
	for (int i = posicaoInicial; i < posicaoFinal; i++)
	{
		printf("\n----------------------");
		printf("\nDigite o codigo do produto:");
		scanf("%d",&codDigitado);
		printf("\nDigite o nome do produto:");
		scanf("%s",nomeDigitado);
		while(!validarProdutoDigitado(i,codDigitado,nomeDigitado))
		{
			printf("\nDigite o codigo do produto:");
			scanf("%d",&codDigitado);
			printf("\nDigite o nome do produto:");
			scanf("%s",nomeDigitado);
		}
		printf("\nDigite a quantidade do produto:");
		scanf("%d",&quantDigitada);
		printf("\nDigite o preco do produto:");
		scanf("%f",&precoDigitado);
		produtosEmEstoque[i].codigo = codDigitado;
		strcpy(produtosEmEstoque[i].nome,nomeDigitado);
		produtosEmEstoque[i].preco = precoDigitado;
		produtosEmEstoque[i].quantidade = quantDigitada;
	}
}

void adicionarProdutosAoEstoque()
{
	int quantProdutosAdicionar;
	
	printf("\nDigite a quantidade de produtos a ser cadastrada no estoque: ");
	scanf("%d",&quantProdutosAdicionar);

	if (produtosEmEstoque == NULL)
	{
		quantProdutosEstoque = quantProdutosAdicionar;
		
		produtosEmEstoque = (Produto*) calloc(quantProdutosEstoque,sizeof(Produto));
		
		if (produtosEmEstoque == NULL)
		{
			printf("\nErro! Houve um erro na alocação de memoria.");
			return;
		}
		leProdutosEstoque(0,quantProdutosEstoque);
	}
	else
	{
		produtosEmEstoque = realloc(produtosEmEstoque, ((quantProdutosEstoque + quantProdutosAdicionar) * sizeof(Produto)));
		leProdutosEstoque((quantProdutosEstoque), (quantProdutosEstoque + quantProdutosAdicionar));
		quantProdutosEstoque += quantProdutosAdicionar;
	}

	printf("\nProdutos em estoque:");
	mostrarProdutosEmEstoque(false);
	printf("\n");
}

bool buscarProdutoNoEstoque(Produto *produtoCarrinho, char nomeDigitado[100], int quantDigitada)
{
	for(int i = 0; i < quantProdutosEstoque; i++)
	{
		if (strcmp(produtosEmEstoque[i].nome, nomeDigitado) == 0)
		{
			if(quantDigitada > produtosEmEstoque[i].quantidade)
			{
				printf("A quantidade a ser adicionada ao carrinho é superior a quantidade disponível em estoque!");
				return false;
			}
			
			produtoCarrinho->codigo = produtosEmEstoque[i].codigo;
			strcpy(produtoCarrinho->nome, produtosEmEstoque[i].nome);
			produtoCarrinho->preco = produtosEmEstoque[i].preco;
			produtoCarrinho->quantidade = quantDigitada;

			return true;
		}
	}
	printf("O produto '%s' não foi encontrado em estoque!", nomeDigitado);
	return false;
}

void retirarProdutoDoEstoque(int codRetirada, int quantRetirada)
{
	for (int i = 0; i < quantProdutosEstoque; i++)
	{
		if (produtosEmEstoque[i].codigo == codRetirada)
		{
			if (quantRetirada > produtosEmEstoque[i].quantidade)
			{
				printf("\nA quantidade a ser retirada do estoque é maior que a quantidade em estoque!");
				break;
			}
			produtosEmEstoque[i].quantidade -= quantRetirada;
			break;
		}
	}
}

void estoque()
{
	int opcao;
	while(opcao != 0)
	{
		printf("\n\n###########################");
		printf("\nEstoque");
		printf("\nOperações disponíveis:");
		printf("\n1 - Cadastrar produtos;");
		printf("\n2 - Visualizar produtos;");
		printf("\n0 - Sair do módulo.");
		printf("\nDigite a operação desejada:");
		scanf("%d",&opcao);
		switch(opcao)
		{
			case 1:
				adicionarProdutosAoEstoque();
			break;
			case 2:
				printf("\nProdutos em estoque:");
				mostrarProdutosEmEstoque(false);
			break;
			case 0:
			break;
			default:
			printf("Operação inválida!");
		}
	}
	
}

//Funções do módulo Carrinho de Compras
void mostrarCarrinho()
{
	printf("\nCarrinho de compras:");
	for(int i = 0; i < quantProdutosCarrinho; i++)
	{
		printf("\nProduto: %s | Quantidade: %d | Preço Unitário: R$%.2f | Preço Total: R$%.2f", produtosNoCarrinho[i].nome, produtosNoCarrinho[i].quantidade, produtosNoCarrinho[i].preco, (produtosNoCarrinho[i].preco * produtosNoCarrinho[i].quantidade));
	}
}

void leProdutosCarrinho(int posicaoInicial, int posicaoFinal)
{
	int quantDigitada;
	char nomeDigitado[100];
	
	for (int i = posicaoInicial; i < posicaoFinal; i++)
	{
		printf("\n----------------------");
		printf("\nDigite o nome do produto:");
		scanf("%s",nomeDigitado);
		printf("\nDigite a quantidade do produto:");
		scanf("%d",&quantDigitada);
		
		while(!buscarProdutoNoEstoque(&produtosNoCarrinho[i],nomeDigitado, quantDigitada))
		{
			printf("\nDigite o nome do produto:");
			scanf("%s",nomeDigitado);
			printf("\nDigite a quantidade do produto:");
			scanf("%d",&quantDigitada);
		}
	}
}

void adicionarProdutosAoCarrinho()
{
	int quantProdutosAdicionar;
	
	printf("\nInforme a quantidade de produtos que deseja adicionar ao carrinho:");
	scanf("%d",&quantProdutosAdicionar);
	
	if (produtosNoCarrinho == NULL)
	{
		quantProdutosCarrinho = quantProdutosAdicionar;
		produtosNoCarrinho = (Produto*) calloc(quantProdutosCarrinho,sizeof(Produto));
		
		if (produtosNoCarrinho == NULL)
		{
			printf("\nErro! Houve um erro na alocação de memoria.");
			return;
		}
		
		leProdutosCarrinho(0, quantProdutosCarrinho);
	}
	else
	{
		produtosNoCarrinho = realloc(produtosNoCarrinho, ((quantProdutosCarrinho + quantProdutosAdicionar) * sizeof(Produto)));
		
		leProdutosCarrinho((quantProdutosCarrinho), (quantProdutosCarrinho + quantProdutosAdicionar));
		quantProdutosEstoque += quantProdutosAdicionar;
	}
}

void carrinhoDeCompras()
{
	int opcao;
	while(opcao != 0)
	{
		printf("\n\n###########################");
		printf("\nCarrinho de Compras");
		printf("\n1 - Adicionar produtos ao carrinho de compras;");
		printf("\n2 - Visualizar carrinho de compras;");
		printf("\n0 - Sair do módulo.");
		printf("\nDigite a operação desejada:");
		scanf("%d",&opcao);
		switch(opcao)
		{
			case 1:
				adicionarProdutosAoCarrinho();
			break;
			case 2:
				mostrarCarrinho();
			break;
			case 0:
			break;
			default:
				printf("Operação inválida!");
		}
	}
}

//Funções do módulo Pedido
void somarTotalDoPedido()
{
	float totalPedido = 0;
	if (produtosNoCarrinho != NULL && quantProdutosCarrinho > 0)
	{
		for(int i = 0; i < quantProdutosCarrinho; i++)
		{
			totalPedido += (produtosNoCarrinho[i].quantidade * produtosNoCarrinho[i].preco);
		}
		printf("\nO valor total do seu pedido é de: R$%.2f",totalPedido);
	}
}

void finalizarPedido()
{
	mostrarCarrinho();
	somarTotalDoPedido();
	for (int i = 0; i < quantProdutosCarrinho; i++)
	{
		retirarProdutoDoEstoque(produtosNoCarrinho[i].codigo,produtosNoCarrinho[i].quantidade);
	}
}

void pedido()
{
	finalizarPedido();
}

//Funções do módulo Vitrine
void vitrine()
{
	printf("\n\n###########################");
	printf("\nVitrine");
	mostrarProdutosEmEstoque(true);
}

//Módulo principal (Main)
int main(void)
{
	int opcao;
	while(opcao != 0)
	{
		printf("\n\n###########################");
		printf("\nE-Commerce em C");
		printf("\nMódulos:");
		printf("\n1 - Estoque;");
		printf("\n2 - Vitrine;");
		printf("\n3 - Carrinho de Compras;");
		printf("\n4 - Pedido;");
		printf("\n0 - Encerrar a execução.");
		printf("\nDigite o módulo desejado:");
		scanf("%d",&opcao);
		switch (opcao)
		{
			case 1:
				estoque();
			break;
			case 2:
				vitrine();
			break;
			case 3:
				carrinhoDeCompras();
			break;
			case 4:
				pedido();
			break;
			case 0:
			break;
			default:
				printf("\nOpção inválida!");
		}
	}
	
	if(produtosNoCarrinho != NULL)
		free(produtosNoCarrinho);
	
	if(produtosEmEstoque != NULL)
		free(produtosEmEstoque);
	return 0;
}