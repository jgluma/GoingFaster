# 2-A-Naive - Implementación Naive en C

## Descripción

Implementación básica de multiplicación de matrices en C usando el algoritmo estándar de triple bucle anidado. Esta versión sirve como línea base para comparar el rendimiento de las optimizaciones posteriores.

## Archivos

- **`sgemm.c`**: Implementación naive de SGEMM con triple bucle anidado
- **`benchmark.c`**: Programa de pruebas de rendimiento
- **`Makefile`**: Configuración de compilación

## Opciones de Compilación

### Variables del Makefile
- **`CC`**: Compilador (gcc)
- **`OPT`**: Optimizaciones del compilador (`-O3`)
- **`CFLAGS`**: Flags de compilación (`-Wall -Wno-unused-result -std=gnu99`)

### Compilación
```bash
# Compilar con optimización -O3
make

# Limpiar archivos objeto
make clean

# Ver código ensamblador generado
make assembly
```

## Ejecución

```bash
./benchmark
```

## Características del Algoritmo

- **Complejidad**: O(n³)
- **Acceso a memoria**: No optimizado para cache
- **Paralelización**: No implementada
- **Vectorización**: Dependiente del auto-vectorizador del compilador

## Código Principal

```c
void square_sgemm (int n, float* A, float* B, float* C)
{
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j) 
      for( int k = 0; k < n; k++ )
        C[OFF(i,j)] += A[OFF(i,k)] * B[OFF(k,j)];
}
```

## Notas

- Esta implementación es la más simple y menos eficiente
- Sirve como referencia para medir las mejoras de las optimizaciones
- El compilador con `-O3` puede aplicar algunas optimizaciones automáticas
