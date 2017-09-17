#include <stdio.h>
#include <stdlib.h>
#include "pgmlib.h"
#include "commline.h"


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

int main(int argc, char *argv[])
{
	PGM_t pgm, pgm_result;
	FILE *stream_input, *stream_output;

	stream_input  = get_stream (argc, argv, "-i", "r");
	stream_output = get_stream (argc, argv, "-o", "w");

	readpgm (&pgm, stream_input);
	pgm_result = pgm;
	allocate_matrix_pgm (&pgm_result);

	apply_average (&pgm, &pgm_result);

	writepgm(&pgm_result, stream_output);

	fclose (stream_input);
	fclose (stream_output);

	return 0;
}	
