#include <stdio.h>
#include <stdlib.h>
#include "pgmlib.h"
#include "commline.h"


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

int main(int argc, char *argv[])
{
	PGM_t pgm, pgm_result;
	FILE *stream_input, *stream_output;

	stream_input  = get_stream (argc, argv, "-i", "r");
	stream_output = get_stream (argc, argv, "-o", "w");

	readpgm (&pgm, stream_input);
	pgm_result = pgm;
	allocate_matrix_pgm (&pgm_result);

	apply_median (&pgm, &pgm_result);

	writepgm(&pgm_result, stream_output);

	fclose (stream_input);
	fclose (stream_output);
	
	return 0;
}
