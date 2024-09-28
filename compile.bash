gcc -c ./utils/utils.c -O3 -Wall \
&& gcc -c ./utils/array.c -O3 -Wall \
&& gcc -c ./utils/dmatrix.c -O3 -Wall \
&& gcc -c nj.c -Wall -O3 \
&& gcc -c main.c -Wall -O3 \
&& gcc  -O3 utils.o dmatrix.o array.o nj.o main.o -o nj.run -Wall
rm *.o

