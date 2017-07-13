#ifndef __PGMLIB__
#define __PGMLIB__
 
typedef struct PGM_t
{
	char type[3];
	unsigned int rows, cols, maxval;
	int **matrix;
} PGM_t;

//funcao para alocar matriz em PGM_t
void allocate_matrix_pgm (PGM_t *pgm);

//funcao para ler pgm em uma stream
void readpgm (PGM_t *pgm, FILE *stream);

//funcao para escrever pgm em uma stream
void writepgm (PGM_t *pgm, FILE *stream);

//funcao que aplica o filtro negativo
//substituindo cada valor da matriz por
//Valor Maximo - Valor do Pixel
void apply_negative (PGM_t *pgm);

//le o limiar na linha de comando
float get_threshold (int argc, char **argv);

//aplica o filtro de limiar
//se o valor do pixel for maior ou igual ao do limiar,
//	ele eh substituido por Valor Maximo
//se não,
//  ele eh substituido por zero
void apply_threshold (PGM_t *pgm, float threshold);

//aplica o filtro da media
//substituindo cada pixel pela media dos seus vizinhos e ele mesmo
void apply_average (PGM_t *pgm, PGM_t *pgm_result);

//aplica o filtro da media
//substituindo cada pixel pela mediana dos seus vizinhos e ele mesmo
void apply_median (PGM_t *pgm, PGM_t *pgm_result);

#endif