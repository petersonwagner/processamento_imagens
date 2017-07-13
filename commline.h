#ifndef __COMMLINE__
#define __COMMLINE__

/* procura por string em argv, retorna  -1 se nao encontrar e a posicao em argv se encontrar */
int search_parameter (int argc, char **argv, char *string);

/* funcao que retorna a stream indicada pela linha de comando ou stdio/stdout se nao encontrar */
FILE* get_stream (int argc, char **argv, char *parameter, char *open_type);

#endif