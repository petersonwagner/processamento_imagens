#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commline.h"

typedef struct PGM_t
{
	char type[3];
	int rows, cols, maxval;
	int **matrix;
} PGM_t;

//funcao para alocar matriz em PGM_t
void allocate_matrix_pgm (PGM_t *pgm)
{
	/*----------  allocating 2-dimensional vector  ----------*/
	(*pgm).matrix = malloc (sizeof(int *) * (*pgm).rows);
	for (int i = 0; i < (*pgm).rows; ++i)
		(*pgm).matrix[i] = malloc (sizeof(int) * (*pgm).cols);

	//se ocorrer algum erro
	if ((*pgm).matrix == NULL)
	{
		perror ("Error at allocating memory");
		abort();
	}
}

//verifica se tem algum comentario
//para nao arruinar completamente a leitura
//funcao 'privada'
void get_comment (FILE *stream)
{
	char test;

	fscanf(stream, "%c", &test);
	
	//se encontrar um #, jogar toda a linha fora
	if (test == '#')
	{
		do
		{
			fscanf(stream, "%c", &test);
		} while (test != '\n');
	}
	else //se nao for comentario, volta uma posicao na leitura
		fseek (stream, -1, SEEK_CUR);
}

//funcao para ler pgm em uma stream
void readpgm (PGM_t *pgm, FILE *stream)
{
	/*----------  reading initial part  ----------*/
	get_comment (stream);
	fscanf(stream, "%s\n", (*pgm).type);
	get_comment (stream);
	fscanf(stream, "%d %d", &(*pgm).cols, &(*pgm).rows);
	get_comment (stream);
	fscanf(stream, "%d", &(*pgm).maxval);
	get_comment (stream);

	allocate_matrix_pgm (pgm);

	/*----------  reading pgm pixels  ----------*/
	//se for do tipo P2, ler em ASCII
	if (!strcmp ((*pgm).type, "P2"))
		for (int i = 0; i < (*pgm).rows; ++i)
			for (int j = 0; j < (*pgm).cols; ++j)
				fscanf (stream, "%d", &(*pgm).matrix[i][j]);

	//se for do tipo P5, ler em binario
	else if (!strcmp ((*pgm).type, "P5"))
		for (int i = 0; i < (*pgm).rows; ++i)
			for (int j = 0; j < (*pgm).cols; ++j)
				fread (&(*pgm).matrix[i][j], 1, 1, stream);

	//tipo nao aceito
	else
	{
		perror ("PGM type not accepted");
		abort();
	}
}

//funcao para escrever pgm em uma stream
void writepgm (PGM_t *pgm, FILE *stream)
{
	/*----------  writing initial part  ----------*/
	fprintf(stream, "P2\n");
	fprintf(stream, "%d %d\n", (*pgm).cols, (*pgm).rows);
	fprintf(stream, "%d\n", (*pgm).maxval);

	/*----------  writing PGM image  ----------*/
	for (int i = 0; i < (*pgm).rows; ++i)
	{
		for (int j = 0; j < (*pgm).cols; ++j)
			fprintf(stream, "%d  ", (*pgm).matrix[i][j]);
		fprintf(stream, "\n");
	}
}


//funcao que aplica o filtro negativo
//substituindo cada valor da matriz por
//Valor Maximo - Valor do Pixel
void apply_negative (PGM_t *pgm)
{
	for (int i = 0; i < (*pgm).rows; ++i)
		for (int j = 0; j < (*pgm).cols; ++j)
			(*pgm).matrix[i][j] = (*pgm).maxval - (*pgm).matrix[i][j];
}


//le o limiar na linha de comando
float get_threshold (int argc, char **argv)
{
	int ret;

	ret = search_parameter (argc, argv, "-l"); //procura por ocorrencia de -l na linha de comando

	if (ret == -1) //se nao achar o parametro
		return 0.5;
	else
		return (atof (argv[ret])); //se nao, retorna a string encontrada transformada em float
}

//aplica o filtro de limiar
//se o valor do pixel for maior ou igual ao do limiar,
//	ele eh substituido por Valor Maximo
//se não,
//  ele eh substituido por zero
void apply_threshold (PGM_t *pgm, float threshold)
{
	for (int i = 0; i < (*pgm).rows; ++i)
	{
		for (int j = 0; j < (*pgm).cols; ++j)
		{
			//se o pixel tem valor maior ou igual ao do limiar
			if ((*pgm).matrix[i][j] >= threshold * (*pgm).maxval)
				(*pgm).matrix[i][j] = (*pgm).maxval;
			else
				(*pgm).matrix[i][j] = 0;
		}
	}
}


//aplica o filtro da media
//substituindo cada pixel pela media dos seus vizinhos e ele mesmo
void apply_average (PGM_t *pgm, PGM_t *pgm_result)
{
	int i, j, sum;

	//linhas de cima e baixo
	for (j = 1; j < (*pgm).cols-1; ++j)
	{
		sum  = (*pgm).matrix[0][j-1];
		sum += (*pgm).matrix[0][j];
		sum += (*pgm).matrix[0][j+1];
		sum += (*pgm).matrix[1][j-1];
		sum += (*pgm).matrix[1][j];
		sum += (*pgm).matrix[1][j+1];
		(*pgm_result).matrix[0][j] = sum / 6;

		sum  = (*pgm).matrix[(*pgm).rows-1][j-1];
		sum += (*pgm).matrix[(*pgm).rows-1][j];
		sum += (*pgm).matrix[(*pgm).rows-1][j+1];
		sum += (*pgm).matrix[(*pgm).rows-2][j-1];
		sum += (*pgm).matrix[(*pgm).rows-2][j];
		sum += (*pgm).matrix[(*pgm).rows-2][j+1];
		(*pgm_result).matrix[(*pgm).rows-1][j] = sum / 6;
	}


	//colunas das laterais
	for (i = 1; i < (*pgm).rows-1; ++i)
	{
		sum  = (*pgm).matrix[i-1][0];
		sum += (*pgm).matrix[i][0];
		sum += (*pgm).matrix[i+1][0];
		sum += (*pgm).matrix[i-1][1];
		sum += (*pgm).matrix[i][1];
		sum += (*pgm).matrix[i+1][1];
		(*pgm_result).matrix[i][0] = sum / 6;

		sum  = (*pgm).matrix[i-1][(*pgm).cols-1];
		sum += (*pgm).matrix[i][(*pgm).cols-1];
		sum += (*pgm).matrix[i+1][(*pgm).cols-1];
		sum += (*pgm).matrix[i-1][(*pgm).cols-2];
		sum += (*pgm).matrix[i][(*pgm).cols-2];
		sum += (*pgm).matrix[i+1][(*pgm).cols-2];
		(*pgm_result).matrix[i][(*pgm).cols-1] = sum / 6;
	}


	//cantos
	sum  = (*pgm).matrix[0][0];
	sum += (*pgm).matrix[0][1];
	sum += (*pgm).matrix[1][0];
	sum += (*pgm).matrix[1][1];
	(*pgm_result).matrix[0][0] = sum / 4;
	
	sum  = (*pgm).matrix[(*pgm).rows-1][0];
	sum += (*pgm).matrix[(*pgm).rows-1][1];
	sum += (*pgm).matrix[(*pgm).rows-2][0];
	sum += (*pgm).matrix[(*pgm).rows-2][1];
	(*pgm_result).matrix[(*pgm).rows-1][0] = sum / 4;

	sum = (*pgm).matrix[0][(*pgm).cols-1];
	sum += (*pgm).matrix[0][(*pgm).cols-2];
	sum += (*pgm).matrix[1][(*pgm).cols-1];
	sum += (*pgm).matrix[1][(*pgm).cols-2];
	(*pgm_result).matrix[0][(*pgm).cols-1] = sum / 4;

	sum  = (*pgm).matrix[(*pgm).rows-1][(*pgm).cols-1];
	sum += (*pgm).matrix[(*pgm).rows-1][(*pgm).cols-2];
	sum += (*pgm).matrix[(*pgm).rows-2][(*pgm).cols-1];
	sum += (*pgm).matrix[(*pgm).rows-2][(*pgm).cols-2];
	(*pgm_result).matrix[(*pgm).rows-1][(*pgm).cols-1] = sum / 4;


	//interior da matriz
	for (i = 1; i < (*pgm).rows-1; ++i)
	{
		for (j = 1; j < (*pgm).cols-1; ++j)
		{
			sum  = (*pgm).matrix[i-1][j-1];
			sum += (*pgm).matrix[i-1][j];
			sum += (*pgm).matrix[i-1][j+1];
			sum += (*pgm).matrix  [i][j-1];
			sum += (*pgm).matrix  [i][j];
			sum += (*pgm).matrix  [i][j+1];
			sum += (*pgm).matrix[i+1][j-1];
			sum += (*pgm).matrix[i+1][j];
			sum += (*pgm).matrix[i+1][j+1];
			(*pgm_result).matrix[i][j] = sum / 9;
		}
	}
}



//bubble sort para o calculo da mediana
void bubble_sort (int v[], int n)
{
	int aux, flag;

	for (int k = 1; k < n; ++k)
	{
		flag = 0;
		for (int i = 0; i < n-k; ++i)
		{
			if (v[i] > v[i+1])
			{
				aux = v[i];
				v[i] = v[i+1];
				v[i+1] = aux;
				flag = 1;
			}
		}

		//se nao ocorreu nenhuma troca
		if (flag == 0)
			break;
	}
}

//calculo da mediana
int median (int v[], int n)
{
	bubble_sort (v, n);	

	//quando n eh impar, retorna o valor do meio
	if (n % 2 == 1)
		return v[n/2];
	else //quando n eh par, retorna media dos dois valores do meio
		return (v[n/2] + v[(n/2)-1])/2;
}

//aplica o filtro da media
//substituindo cada pixel pela mediana dos seus vizinhos e ele mesmo
void apply_median (PGM_t *pgm, PGM_t *pgm_result)
{
	int v[9], i, j;
	
	//linhas de cima e baixo
	for (j = 1; j < (*pgm).cols-1; ++j)
	{
		v[0] = (*pgm).matrix[0][j-1];
		v[1] = (*pgm).matrix[0][j];
		v[2] = (*pgm).matrix[0][j+1];
		v[3] = (*pgm).matrix[1][j-1];
		v[4] = (*pgm).matrix[1][j];
		v[5] = (*pgm).matrix[1][j+1];
		(*pgm_result).matrix[0][j] = median (v, 6);

		v[0] = (*pgm).matrix[(*pgm).rows-1][j-1];
		v[1] = (*pgm).matrix[(*pgm).rows-1][j];
		v[2] = (*pgm).matrix[(*pgm).rows-1][j+1];
		v[3] = (*pgm).matrix[(*pgm).rows-2][j-1];
		v[4] = (*pgm).matrix[(*pgm).rows-2][j];
		v[5] = (*pgm).matrix[(*pgm).rows-2][j+1];
		(*pgm_result).matrix[(*pgm).rows-1][j] = median (v, 6);
	}


	//colunas das laterais
	for (i = 1; i < (*pgm).rows-1; ++i)
	{
		v[0] = (*pgm).matrix[i-1][0];
		v[1] = (*pgm).matrix[i][0];
		v[2] = (*pgm).matrix[i+1][0];
		v[3] = (*pgm).matrix[i-1][1];
		v[4] = (*pgm).matrix[i][1];
		v[5] = (*pgm).matrix[i+1][1];
		(*pgm_result).matrix[i][0] = median (v, 6);

		v[0] = (*pgm).matrix[i-1][(*pgm).cols-1];
		v[1] = (*pgm).matrix[i][(*pgm).cols-1];
		v[2] = (*pgm).matrix[i+1][(*pgm).cols-1];
		v[3] = (*pgm).matrix[i-1][(*pgm).cols-2];
		v[4] = (*pgm).matrix[i][(*pgm).cols-2];
		v[5] = (*pgm).matrix[i+1][(*pgm).cols-2];
		(*pgm_result).matrix[i][(*pgm).cols-1] = median (v, 6);
	}


	//cantos
	v[0] = (*pgm).matrix[0][0];
	v[1] = (*pgm).matrix[0][1];
	v[2] = (*pgm).matrix[1][0];
	v[3] = (*pgm).matrix[1][1];
	(*pgm_result).matrix[0][0] = median (v, 4);

	v[0] = (*pgm).matrix[(*pgm).rows-1][0];
	v[1] = (*pgm).matrix[(*pgm).rows-1][1];
	v[2] = (*pgm).matrix[(*pgm).rows-2][0];
	v[3] = (*pgm).matrix[(*pgm).rows-2][1];
	(*pgm_result).matrix[(*pgm).rows-1][0] = median (v, 4);

	v[0] = (*pgm).matrix[0][(*pgm).cols-1];
	v[1] = (*pgm).matrix[0][(*pgm).cols-2];
	v[2] = (*pgm).matrix[1][(*pgm).cols-1];
	v[3] = (*pgm).matrix[1][(*pgm).cols-2];
	(*pgm_result).matrix[0][(*pgm).cols-1] = median (v, 4);

	v[0] = (*pgm).matrix[(*pgm).rows-1][(*pgm).cols-1];
	v[1] = (*pgm).matrix[(*pgm).rows-1][(*pgm).cols-2];
	v[2] = (*pgm).matrix[(*pgm).rows-2][(*pgm).cols-1];
	v[3] = (*pgm).matrix[(*pgm).rows-2][(*pgm).cols-2];
	(*pgm_result).matrix[(*pgm).rows-1][(*pgm).cols-1] = median (v, 4);


	//interior da matriz
	for (i = 1; i < (*pgm).rows-1; ++i)
	{
		for (j = 1; j < (*pgm).cols-1; ++j)
		{
			v[0] = (*pgm).matrix[i-1][j-1];
			v[1] = (*pgm).matrix[i-1][j];
			v[2] = (*pgm).matrix[i-1][j+1];
			v[3] = (*pgm).matrix  [i][j-1];
			v[4] = (*pgm).matrix  [i][j];
			v[5] = (*pgm).matrix  [i][j+1];
			v[6] = (*pgm).matrix[i+1][j-1];
			v[7] = (*pgm).matrix[i+1][j];
			v[8] = (*pgm).matrix[i+1][j+1];
			(*pgm_result).matrix[i][j] = median (v, 9);
		}
	}
}