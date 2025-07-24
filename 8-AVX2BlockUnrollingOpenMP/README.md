# 8-AVX2BlockUnrollingOpenMP - Implementación Completa con Todas las Optimizaciones

## Descripción

Esta es la implementación más avanzada que combina todas las técnicas de optimización: blocking jerárquico, instrucciones vectoriales AVX2, desenrollado de bucles y paralelización OpenMP. Representa el estado del arte en optimización manual de multiplicación de matrices.

## Archivos

- **`sgemm.c`**: Implementación con todas las optimizaciones
- **`benchmark.c`**: Programa de pruebas de rendimiento
- **`Makefile`**: Configuración de compilación completa
- **`output_files/`**: Directorio para resultados detallados

## Opciones de Compilación

### Variables del Makefile
- **`CC`**: Compilador (gcc)
- **`OPT`**: Optimizaciones completas (`-mavx2 -mfma -O3 -fopenmp -march=native`)
- **`CFLAGS`**: Flags completos + parámetros ajustables
- **`LDLIBS`**: Librerías optimizadas (`-fopenmp`)

### Parámetros Configurables
```bash
# Blocking jerárquico
L1_BLOCK_SIZE ?= 32     # Optimizado para L1 cache
L2_BLOCK_SIZE ?= 128    # Optimizado para L2 cache
L3_BLOCK_SIZE ?= 512    # Optimizado para L3 cache

# Vectorización y desenrollado
VECTOR_UNROLL ?= 8      # Factor de desenrollado vectorial
SCALAR_UNROLL ?= 4      # Factor de desenrollado escalar

# Paralelización
NTHREADS ?= 8           # Número de hilos OpenMP
```

## Compilación Avanzada

```bash
# Compilación estándar
make

# Optimización máxima para arquitectura específica
make OPT="-mavx2 -mfma -O3 -fopenmp -march=native -mtune=native -flto"

# Configuración personalizada
make L1_BLOCK_SIZE=24 L2_BLOCK_SIZE=96 VECTOR_UNROLL=16 NTHREADS=16

# Compilación con información de optimización
make CFLAGS="-mavx2 -mfma -O3 -fopenmp -march=native -fopt-info-all"
```

## Arquitectura del Algoritmo

### Jerarquía de Optimización
```c
// Nivel 1: Blocking L3 + Paralelización OpenMP
#pragma omp parallel for schedule(static) collapse(2)
for (int iii = 0; iii < n; iii += L3_BLOCK_SIZE) {
    for (int jjj = 0; jjj < n; jjj += L3_BLOCK_SIZE) {
        
        // Nivel 2: Blocking L2
        for (int ii = iii; ii < min(iii+L3_BLOCK_SIZE, n); ii += L2_BLOCK_SIZE) {
            for (int jj = jjj; jj < min(jjj+L3_BLOCK_SIZE, n); jj += L2_BLOCK_SIZE) {
                
                // Nivel 3: Blocking L1 + Vectorización AVX2
                sgemm_kernel_avx2_blocked(ii, jj, kk, ...);
            }
        }
    }
}
```

### Kernel Optimizado
```c
void sgemm_kernel_avx2_blocked(int i_start, int j_start, int k_start, ...) {
    // Blocking L1 con vectorización AVX2 y desenrollado
    for (int i = i_start; i < i_start + L1_BLOCK_SIZE; i++) {
        for (int j = j_start; j < j_start + L1_BLOCK_SIZE; j += 8) { // AVX2: 8 floats
            
            __m256 c_vec = _mm256_load_ps(&C[i*n + j]);
            
            // Bucle K desenrollado y vectorizado
            for (int k = k_start; k < k_start + L1_BLOCK_SIZE; k += VECTOR_UNROLL) {
                // 8 operaciones FMA desenrolladas
                __m256 a_vec = _mm256_broadcast_ss(&A[i*n + k]);
                __m256 b_vec = _mm256_load_ps(&B[k*n + j]);
                c_vec = _mm256_fmadd_ps(a_vec, b_vec, c_vec);
                
                // Desenrollado manual para k+1, k+2, ..., k+7
                // ...más operaciones optimizadas
            }
            
            _mm256_store_ps(&C[i*n + j], c_vec);
        }
    }
}
```

## Configuración del Sistema

### Variables de Entorno OpenMP
```bash
# Configuración óptima
export OMP_NUM_THREADS=$(nproc)
export OMP_PROC_BIND=spread
export OMP_PLACES="cores"
export OMP_SCHEDULE="static"

# Para sistemas NUMA
export OMP_PLACES="sockets"
export OMP_PROC_BIND=close

# Ajuste de stack size para recursión profunda
export OMP_STACKSIZE=8M
```

### Optimización del Sistema
```bash
# Governor de CPU para máximo rendimiento
sudo cpupower frequency-set -g performance

# Deshabilitar Turbo Boost para benchmarks consistentes
echo 1 | sudo tee /sys/devices/system/cpu/intel_pstate/no_turbo

# Afinidad a cores físicos
taskset -c 0,2,4,6,8,10,12,14 ./benchmark
```

## Benchmarking Comprehensivo

### Test de Escalabilidad Completa
```bash
#!/bin/bash
# comprehensive_benchmark.sh

echo "=== Benchmark Completo ==="
echo "Sistema: $(uname -a)"
echo "CPU: $(lscpu | grep 'Model name')"
echo "Memoria: $(free -h | grep 'Mem:')"
echo

# Test de hilos
for threads in 1 2 4 8 16; do
    if [ $threads -le $(nproc) ]; then
        echo "=== $threads hilos ==="
        OMP_NUM_THREADS=$threads ./benchmark
    fi
done

# Test de tamaños de bloque
for l1_block in 16 24 32 48; do
    for l2_block in 64 96 128 192; do
        echo "=== L1:$l1_block L2:$l2_block ==="
        make clean > /dev/null
        make L1_BLOCK_SIZE=$l1_block L2_BLOCK_SIZE=$l2_block > /dev/null
        ./benchmark
    done
done
```

### Profiling Avanzado
```bash
# Análisis completo de rendimiento
perf stat -e cycles,instructions,cache-references,cache-misses,\
L1-dcache-loads,L1-dcache-load-misses,\
L1-icache-loads,L1-icache-load-misses,\
LLC-loads,LLC-load-misses,\
branch-loads,branch-load-misses \
./benchmark

# Análisis de vectorización
perf stat -e fp_arith_inst_retired.scalar_single,\
fp_arith_inst_retired.128b_packed_single,\
fp_arith_inst_retired.256b_packed_single \
./benchmark

# Profile de memoria detallado
valgrind --tool=cachegrind --I1=32768,8,64 --D1=32768,8,64 --LL=33554432,16,64 ./benchmark
```

## Tuning Específico por Arquitectura

### Intel Skylake/Ice Lake
```bash
make OPT="-mavx2 -mfma -march=skylake -mtune=skylake -O3 -fopenmp"
make L1_BLOCK_SIZE=32 L2_BLOCK_SIZE=128 L3_BLOCK_SIZE=512
```

### Intel Cascade Lake/Cooper Lake
```bash
make OPT="-mavx512f -mavx512dq -march=cascadelake -O3 -fopenmp"
make L1_BLOCK_SIZE=48 L2_BLOCK_SIZE=192 L3_BLOCK_SIZE=768
```

### AMD Zen 2/3
```bash
make OPT="-mavx2 -mfma -march=znver2 -mtune=znver2 -O3 -fopenmp"
make L1_BLOCK_SIZE=32 L2_BLOCK_SIZE=128 L3_BLOCK_SIZE=1024
```

## Análisis de Resultados

### Métricas de Rendimiento
```bash
# GFLOPS alcanzados
echo "GFLOPS: $(echo "$operations / $time_seconds / 1000000000" | bc -l)"

# Eficiencia vs pico teórico
echo "Eficiencia: $(echo "$actual_gflops / $theoretical_peak * 100" | bc -l)%"

# Análisis de memoria
echo "Ancho de banda: $(echo "$matrix_size * 3 * 4 / $time_seconds / 1000000000" | bc -l) GB/s"
```

### Comparación con Librerías Optimizadas
```bash
# Comparar con OpenBLAS
echo "Performance vs OpenBLAS:"
python3 compare_with_openblas.py

# Comparar con Intel MKL
echo "Performance vs Intel MKL:"
python3 compare_with_mkl.py
```

## Rendimiento Esperado

### Mejoras Acumulativas
- **Vs Naive**: 20-50x mejora
- **Vs Solo OpenMP**: 4-8x mejora adicional
- **Vs Solo AVX2**: 3-6x mejora adicional
- **Vs Solo Blocking**: 2-4x mejora adicional

### Límites Teóricos
- **Peak GFLOPS**: Frecuencia × Cores × Operaciones_por_ciclo
- **Ancho de banda**: Limitado por memoria DDR4/DDR5
- **Eficiencia típica**: 60-85% del pico teórico

## Debugging y Optimización

### Verificación de Optimizaciones
```bash
# Verificar instrucciones AVX2
objdump -d benchmark | grep -E "(vfmadd|vmul|vadd)" | wc -l

# Verificar paralelización
ldd benchmark | grep gomp

# Verificar alineación de memoria
gdb ./benchmark -ex "set confirm off" -ex "run" -ex "bt" -ex "quit"
```

### Análisis de Cuello de Botella
```bash
# Top-down analysis con Intel VTune (si disponible)
vtune -collect hotspots ./benchmark

# Análisis de contención de memoria
perf mem record ./benchmark
perf mem report
```
