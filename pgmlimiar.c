#include <stdio.h>
#include <stdlib.h>
#include "pgmlib.h"
#include "commline.h"

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