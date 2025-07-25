CC = gcc
SRC = benchmark.c
TARGET = benchmark
ASM = benchmark.s

# Flags de optimizaci�n: opcion 1
#OPT = -O1 

# Flags de optimizaci�n: opcion 2
#OPT = -O1 -ftree-vectorize -fopt-info-vec-optimized

# Flags de optimizaci�n: opcion 3
#OPT = -O1 -march=native -ffast-math -ftree-vectorize -fopt-info-vec-optimized

# Flags de optimizaci�n: opcion 4: el compilador lo da todo para mejorar el rendimiento
OPT = -O3 -march=native -funroll-loops -ffast-math -ftree-vectorize -fopt-info-vec-optimized

CFLAGS = -Wall -std=gnu99 $(OPT)
LDFLAGS = 

.PHONY: all clean asm

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

asm: $(SRC)
	$(CC) $(CFLAGS) -S -o $(ASM) $(SRC)

clean:
	rm -f $(TARGET) $(ASM)
