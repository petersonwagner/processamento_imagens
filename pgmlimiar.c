#include <stdio.h>
#include <stdlib.h>
#include "pgmlib.h"
#include "commline.h"

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


int main(int argc, char *argv[])
{
	float threshold;
	PGM_t pgm;
	FILE *stream_input, *stream_output;
	
	stream_input  = get_stream (argc, argv, "-i", "r");
	stream_output = get_stream (argc, argv, "-o", "w");

	readpgm (&pgm, stream_input);

	threshold = get_threshold (argc, argv);

	apply_threshold (&pgm, threshold);

	writepgm (&pgm, stream_output);

	fclose (stream_input);
	fclose (stream_output);

	return 0;
}
