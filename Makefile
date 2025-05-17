CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -pedantic
LIBDIR = lib
SRCDIR = src
MAINDIR = main

all: projeto_edafase2.exe

$(LIBDIR)/grafo.lib: $(SRCDIR)/grafo.c include/grafo.h
	$(CC) $(CFLAGS) -c $< -o grafo.obj
	ar rcs $@ grafo.obj
	del grafo.obj

$(LIBDIR)/mapa.lib: $(SRCDIR)/mapa.c include/mapa.h
	$(CC) $(CFLAGS) -c $< -o mapa.obj
	ar rcs $@ mapa.obj
	del mapa.obj

projeto_edafase2.exe: $(MAINDIR)/main.c $(LIBDIR)/grafo.lib $(LIBDIR)/mapa.lib
	$(CC) $(CFLAGS) -L$(LIBDIR) $< -lgrafo -lmapa -o $@

clean:
	del $(LIBDIR)\*.lib projeto_edafase2.exe

.PHONY: all clean