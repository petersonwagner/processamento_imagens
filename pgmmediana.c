#include <stdio.h>
#include <stdlib.h>
#include "pgmlib.h"
#include "commline.h"

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