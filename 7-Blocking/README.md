# 7-Blocking - Técnica de Blocking/Tiling

## Descripción

Esta implementación utiliza la técnica de blocking (también conocida como tiling) para optimizar el acceso a memoria y mejorar la localidad de cache, reorganizando los cálculos en bloques que caben eficientemente en la jerarquía de memoria.

## Archivos

- **`sgemm.c`**: Implementación con algoritmo de blocking
- **`benchmark.c`**: Programa de pruebas de rendimiento
- **`Makefile`**: Configuración de compilación
- **`output_files/`**: Directorio para resultados

## Opciones de Compilación

### Variables del Makefile
- **`CC`**: Compilador (gcc)
- **`OPT`**: Optimizaciones (`-O3`)
- **`CFLAGS`**: Flags de compilación + definiciones de bloque

## Compilación

```bash
# Compilar con bloque por defecto
make

# Compilar con diferentes tamaños de bloque
make BLOCK_SIZE=16

# Ejecutar el código con profiling
./benchmark <size> -p
```

## Técnica de Blocking

### Concepto
El blocking divide las matrices grandes en submatrices más pequeñas que caben mejor en los niveles de cache, mejorando la localidad temporal y espacial.

### Transformación del Algoritmo
```c
// Algoritmo naive
for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
        for (k = 0; k < n; k++)
            C[i][j] += A[i][k] * B[k][j];

// Algoritmo con blocking
for (ii = 0; ii < n; ii += BLOCK_SIZE)
    for (jj = 0; jj < n; jj += BLOCK_SIZE)
        for (kk = 0; kk < n; kk += BLOCK_SIZE)
            // Bloque de BLOCK_SIZE x BLOCK_SIZE
            for (i = ii; i < min(ii+BLOCK_SIZE, n); i++)
                for (j = jj; j < min(jj+BLOCK_SIZE, n); j++)
                    for (k = kk; k < min(kk+BLOCK_SIZE, n); k++)
                        C[i][j] += A[i][k] * B[k][j];
```

## Selección del Tamaño de Bloque

### Factores a Considerar
1. **Tamaño de Cache L1**: Típicamente 32KB
2. **Tamaño de Cache L2**: Típicamente 256KB-1MB
3. **Tamaño de Cache L3**: Típicamente 8-32MB
4. **Tamaño de página**: 4KB en sistemas típicos

### Cálculo Teórico
```bash
# Para cache L1 (32KB) con 3 matrices
# Cada bloque: BLOCK_SIZE² * sizeof(float) * 3
# BLOCK_SIZE² * 4 * 3 ≤ 32768
# BLOCK_SIZE ≤ sqrt(32768/12) ≈ 52
# Valor práctico: 32-48
```

## Características del Algoritmo

### Ventajas
1. **Mejor localidad temporal**: Reutilización de datos en cache
2. **Mejor localidad espacial**: Acceso secuencial dentro de bloques
3. **Reducción de cache misses**: Especialmente en L1 y L2
4. **Escalabilidad**: Funciona bien con diferentes tamaños de matriz

### Limitaciones
1. **Complejidad adicional**: Manejo de bordes y bloques parciales
2. **Overhead**: Bucles adicionales de control
3. **Dependencia de arquitectura**: Tamaño óptimo varía por sistema

## Optimizaciones Adicionales

### Blocking Jerárquico
```c
// Multi-nivel blocking para L1, L2, L3
#define L1_BLOCK 32
#define L2_BLOCK 128
#define L3_BLOCK 512

// Blocking para L3
for (iii = 0; iii < n; iii += L3_BLOCK)
    // Blocking para L2
    for (ii = iii; ii < min(iii+L3_BLOCK, n); ii += L2_BLOCK)
        // Blocking para L1
        for (i = ii; i < min(ii+L2_BLOCK, n); i += L1_BLOCK)
            // Núcleo computacional
```

### Prefetching
```c
// Hints de prefetch para próximos bloques
__builtin_prefetch(&A[(i+BLOCK_SIZE)*n + k], 0, 3);
__builtin_prefetch(&B[k*n + (j+BLOCK_SIZE)], 0, 3);
```

## Tuning del Sistema

### Información de Cache
```bash
# Ver jerarquía de cache
lscpu | grep -i cache

# Información detallada
cat /sys/devices/system/cpu/cpu0/cache/index*/size
cat /sys/devices/system/cpu/cpu0/cache/index*/level
```

## Rendimiento Esperado

- **Mejora vs Naive**: 2-5x dependiendo del tamaño de matriz
- **Punto dulce**: Típicamente entre 32-64 para la mayoría de sistemas
- **Escalabilidad**: Mantiene rendimiento con matrices grandes