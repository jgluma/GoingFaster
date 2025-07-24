# 6-AVX2UnrollingOpenMP - Combinación AVX2 + Unrolling + OpenMP

## Descripción

Esta implementación combina múltiples técnicas de optimización: instrucciones vectoriales AVX2, desenrollado de bucles y paralelización con OpenMP para maximizar el rendimiento en sistemas multi-core.

## Archivos

- **`sgemm.c`**: Implementación con todas las optimizaciones combinadas
- **`benchmark.c`**: Programa de pruebas de rendimiento
- **`Makefile`**: Configuración de compilación optimizada
- **`output_files/`**: Directorio para resultados

## Opciones de Compilación

### Variables del Makefile
- **`CC`**: Compilador (gcc)
- **`OPT`**: Optimizaciones combinadas (`-mavx2 -mfma -O3 -fopenmp`)
- **`CFLAGS`**: Flags completos + parámetros configurables
- **`LDLIBS`**: Librerías OpenMP (`-fopenmp`)

### Parámetros Configurables
```bash
# Tamaño de bloque
BLOCKSIZE ?= 16

# Factor de desenrollado vectorial
UNROLL ?= 8

# Número de hilos
NTHREADS ?= 4
```

## Compilación

```bash
# Compilar con optimizaciones completas
make

# Optimización para arquitectura específica
make OPT="-mavx2 -mfma -O3 -fopenmp -march=native -mtune=native"

# Configuración personalizada
make BLOCKSIZE=32 UNROLL=16 NTHREADS=8
```

## Técnicas Combinadas

### 1. Vectorización AVX2
- **Procesamiento**: 8 floats simultáneamente
- **Instrucciones**: FMA para multiply-add fusionadas
- **Alineación**: Datos alineados a 32 bytes

### 2. Loop Unrolling
- **Factor típico**: 8-16 iteraciones por bucle
- **Beneficio**: Reducción de overhead + mejor pipeline
- **Combinación**: Con vectorización para máximo throughput

### 3. Paralelización OpenMP
- **Distribución**: Bucles externos entre hilos
- **Escalabilidad**: Aprovecha todos los cores disponibles
- **Balanceo**: Distribución equilibrada del trabajo

## Configuración Avanzada

### Variables de Entorno OpenMP
```bash
# Configuración óptima para rendimiento
export OMP_NUM_THREADS=$(nproc)
export OMP_PROC_BIND=spread
export OMP_PLACES=cores
export OMP_SCHEDULE="static"

# Para sistemas NUMA
export OMP_PLACES=sockets
export OMP_PROC_BIND=close
```

### Compilación Optimizada por Arquitectura
```bash
# Intel Haswell/Broadwell
make OPT="-mavx2 -mfma -march=haswell -O3 -fopenmp"

# Intel Skylake+
make OPT="-mavx2 -mfma -march=skylake -O3 -fopenmp"

# AMD Zen2+
make OPT="-mavx2 -mfma -march=znver2 -O3 -fopenmp"
```

## Estructura del Algoritmo

### Jerarquía de Optimización
```c
#pragma omp parallel for schedule(static)
for (int i = 0; i < n; i += BLOCK_SIZE) {
    for (int j = 0; j < n; j += BLOCK_SIZE) {
        for (int k = 0; k < n; k += BLOCK_SIZE) {
            // Bloque optimizado con AVX2 + unrolling
            sgemm_block_avx2_unrolled(i, j, k, ...);
        }
    }
}
```

### Bloque Vectorizado
```c
void sgemm_block_avx2_unrolled(...) {
    // Unrolling + vectorización AVX2
    for (int ii = i; ii < i + BLOCK_SIZE; ii++) {
        for (int jj = j; jj < j + BLOCK_SIZE; jj += 8) { // AVX2: 8 elementos
            __m256 sum = _mm256_setzero_ps();
            
            // Unrolled loop
            for (int kk = k; kk < k + BLOCK_SIZE; kk += UNROLL) {
                // 8 operaciones FMA desenrolladas
                __m256 a = _mm256_broadcast_ss(&A[ii*n + kk]);
                __m256 b = _mm256_load_ps(&B[kk*n + jj]);
                sum = _mm256_fmadd_ps(a, b, sum);
                // ... más operaciones desenrolladas
            }
            
            _mm256_store_ps(&C[ii*n + jj], sum);
        }
    }
}
```

## Benchmarking Completo

### Test de Escalabilidad
```bash
# Test con diferentes números de hilos
for threads in 1 2 4 8 16; do
    echo "=== $threads hilos ==="
    OMP_NUM_THREADS=$threads ./benchmark
done
```

### Test de Parámetros
```bash
# Test de diferentes tamaños de bloque
for bs in 8 16 32 64; do
    echo "=== BLOCKSIZE=$bs ==="
    make clean && make BLOCKSIZE=$bs && ./benchmark
done
```

### Comparación de Versiones
```bash
# Script para comparar todas las versiones
cd ../
./test_all.sh
./merge_results.sh
```

## Rendimiento Esperado

### Mejoras Acumulativas
- **Vs Naive**: 10-20x mejora
- **Vs OpenMP solo**: 3-5x mejora adicional por vectorización
- **Vs AVX2 solo**: 2-4x mejora adicional por paralelización

### Factores Limitantes
1. **Ancho de banda de memoria**: Puede ser el cuello de botella final
2. **Latencia de cache**: Importancia del BLOCKSIZE adecuado
3. **Contención de hilos**: Overhead en sistemas con muchos cores

## Optimización de Sistema

### Para Máximo Rendimiento
```bash
# Deshabilitar CPU frequency scaling
sudo cpupower frequency-set -g performance

# Aumentar prioridad del proceso
nice -n -20 ./benchmark

# Afinidad a cores físicos específicos
taskset -c 0,2,4,6 ./benchmark
```

## Análisis de Resultados

```bash
# Información del sistema
lscpu
cat /proc/meminfo | grep MemTotal

# Análisis de instrucciones
perf stat -e cycles,instructions,cache-misses ./benchmark

# Profile de memoria
valgrind --tool=cachegrind ./benchmark
```
