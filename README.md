# Se mudou grafo.c:
gcc -c src/grafo.c -Iinclude -o grafo.obj
ar rcs lib/grafo.lib grafo.obj
del grafo.obj

# Se mudou mapa.c:
gcc -c src/mapa.c -Iinclude -o mapa.obj
ar rcs lib/mapa.lib mapa.obj
del mapa.obj


gcc -Iinclude -Llib main.c -lgrafo -lmapa -o projeto_edafase2.exe
.\projeto_edafase2.exe

ou

make clean
make
./projeto_edafase2.exe