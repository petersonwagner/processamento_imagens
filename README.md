# processamento_imagens - Trabalho de Oficina de Computação
# Especificação

Para facilitar a leitura e escrita dos arquivos de imagem, neste projeto será adotado o formato de imagem Portable Gray Map (PGM), um formato de imagem em níveis de cinza bem simples e fácil de ler/escrever. Boa parte dos programas de tratamento de imagens reconhece o formato PGM.

Eis um exemplo de imagem no formato PGM:
```
P2
# this is a PGM image
24 7
15
0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
0  3  3  3  3  0  0  7  7  7  7  0  0 11 11 11 11  0  0 15 15 15 15  0
0  3  0  0  0  0  0  7  0  0  0  0  0 11  0  0  0  0  0 15  0  0 15  0
0  3  3  3  0  0  0  7  7  7  0  0  0 11 11 11  0  0  0 15 15 15 15  0
0  3  0  0  0  0  0  7  0  0  0  0  0 11  0  0  0  0  0 15  0  0  0  0
0  3  0  0  0  0  0  7  7  7  7  0  0 11 11 11 11  0  0 15  0  0  0  0
0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
```


## Filtros
Este projeto visa construir filtros simples para imagens digitais em níveis de cinza, no formato PGM (P2 e P5).

### Filtro negativo
O filtro negativo consiste em converter cada pixel da imagem em seu complemento. Sendo max o valor máximo para um pixel na imagem, o complemento de um pixel com valor v seria max-v.

Forma de chamada:

```
pgmnega -i input -o output
```

Exemplo de entrada e de saída:

![negative](nega.png?raw=true "negative")
 

### Filtro de rotação
O filtro de rotação gira uma imagem em 90° no sentido horário.

Forma de chamada:
```
pgmrotacao -i input -o output
```
Exemplo de entrada e de saída:

![rotation](rot.png?raw=true "rotation")
 

### Filtro de limiar
O filtro de limiar (threshold filter) converte uma imagem em tons de cinza em um imagem em preto-e-branco (2 cores). A forma mais simples de fazer isso é comparar o valor de cada pixel com um limiar pré-definido: se o valor do pixel for igual ou superior ao limiar, ele vira branco (v=max), caso contrário ele vira preto (v=0).

Forma de chamada:
```
pgmlimiar -l N -i input -o output
```
onde N é um limiar real entre 0.0 (0% do valor máximo) e 1.0 (100% do valor máximo). Caso o limiar não esteja definido, assume-se que seja 50%.

Exemplo de entrada e de saídas com limiar de 50% e 75%:

![binarization](bin.png?raw=true "binarization")
  

### Filtro de redução de ruído pela mediana
O filtro da mediana é uma melhoria do filtro da média, pois geralmente consegue reduzir o nível de ruído de uma imagem sem prejudicar sua nitidez. Ele consiste basicamente em substituir o valor de um pixel pelo valor da mediana de seus pixels vizinhos (9 pixels, incluindo ele mesmo).

Deve ser tomado cuidado especial ao tratar os pixels nas primeiras e últimas linhas ou colunas, pois eles não têm todos os vizinhos. Nesses casos, devem ser considerados no cálculo somente os vizinhos existentes.

Forma de chamada:
```
pgmmediana -i input -o output
```
Exemplo de entrada e de saída:

![median](median.png?raw=true "median")
  

### Atividade
Implementar os filtros acima definidos como arquivos e comandos separados. Exemplo: o filtro de limiar deve ser implementado em um arquivo pgmlimiar.c que gera um executável pgmlimiar.
Os filtros devem aceitar como entrada imagens no formato PGM (P2 e P5, plain e raw) e devem gerar como saída imagens no mesmo formato da entrada.
As rotinas comuns (leitura/escrita de arquivos, tratamento da linha de comando, etc) devem ser implementadas em arquivos separados, cujos cabeçalhos são incluídos nos arquivos de implementação dos filtros.
Sempre que possível, as informações da imagem necessárias às funções devem ser transferidas como parâmetros (por valor ou por referência, dependendo da situação). Minimizar o uso de variáveis globais.
Use alocação dinâmica de memória para as imagens, para poder processar as imagens maiores. Só aloque a memória após encontrar o tamanho da imagem.
Construir um Makefile para o projeto:
Ao menos os alvos all (default), clean e purge.
CFLAGS = -Wall
Compilar e ligar separadamente (gerar arquivos .o intermediários)
O que deve ser entregue ao professor:
arquivos .c e .h
arquivo Makefile
não enviar as imagens de teste
Uso de arquivos
A opção -i indica o nome do arquivo de entrada; se não for informado, deve-se usar a entrada padrão (stdin).
A opção -o indica o nome do arquivo de saída; se não for informado, deve-se usar a saída padrão (stdout).
Todas as mensagens de erro devem ser enviadas para a saída de erro (stderr).
Essas opções podem ser usadas em qualquer combinação, ou seja:
```
// entrada e saída em arquivos
pgmmediana -i inputfile.pgm -o outputfile.pgm
pgmmediana -o outputfile.pgm -i inputfile.pgm

// entrada em arquivo, saída em stdout, vice-versa ou ambos
pgmmediana -i inputfile.pgm > outputfile.pgm
pgmmediana -o outputfile.pgm < inputfile.pgm
pgmmediana < inputfile.pgm > outputfile.pgm

// as opções podem estar em qualquer ordem
pgmlimiar -l 0.3 -i inputfile.pgm -o outputfile.pgm
pgmlimiar -i inputfile.pgm -l 0.3 -o outputfile.pgm
pgmlimiar -o outputfile.pgm -i inputfile.pgm -l 0.3
```
