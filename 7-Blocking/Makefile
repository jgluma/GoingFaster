# Podéis usar este Makefile como referencia. Vuestro codigo debe poderse 
# compilar con GCC usando los flags que ya estan especificados en CFLAGS.
# Podéis experimentar con la variable OPT para añadir opciones adicionales.

CC = gcc 
OPT = -O3

CFLAGS = -Wall -Wno-unused-result -std=gnu99 $(OPT)
# Añadir definiciones a CFLAGS
BLOCKSIZE ?= 8
UNROLL ?= 4
NTHREADS ?= 2
CFLAGS += -DBLOCKSIZE=$(BLOCKSIZE) -DUNROLL=$(UNROLL) -DNTHREADS=$(NTHREADS)

LDFLAGS = -Wall
LDLIBS = 

targets = benchmark
objects = benchmark.o sgemm.o
ass_obj = benchmark.s sgemm.s

.PHONY : default
default : all

.PHONY : all
all : clean $(objects) benchmark

benchmark : benchmark.o sgemm.o
	$(CC) -o $@ $^ $(LDLIBS)

%.o : %.c
	$(CC) -c $(CFLAGS) $<

.PHONY : ensambla
ensambla: clean $(ass_obj) all

%.s : %.c
	$(CC) -S $(CFLAGS) $<

.PHONY : clean
clean:
	rm -f $(targets) $(objects) $(ass_obj)

