#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* procura por string em argv, retorna  -1 se nao encontrar e a posicao em argv se encontrar */
int search_parameter (int argc, char **argv, char *string)
{
	for (int i = 1; i < argc; ++i)
		if (!strcmp (string, argv[i]) && i<argc-1)
			return i+1;
	return -1;
}

/* funcao que retorna a stream indicada pela linha de comando ou stdio/stdout se nao encontrar */
FILE* get_stream (int argc, char **argv, char *parameter, char *open_type)
{
	int ret;
	FILE* stream;

	ret = search_parameter (argc, argv, parameter);

	if (ret == -1)
	{
		if (open_type[0] == 'r')
			stream = stdin;
		else
			stream = stdout;
	}

	else
	{
		stream = fopen (argv[ret], open_type);
		if (stream == NULL)
		{
			perror ("Fail at opening file");
			abort();
		}
	}

	return stream;
}