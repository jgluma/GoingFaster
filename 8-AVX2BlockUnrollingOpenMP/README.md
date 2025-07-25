# 8-AVX2BlockUnrollingOpenMP - Implementación Completa con Todas las Optimizaciones

## Descripción

Esta es la implementación más avanzada que combina todas las técnicas de optimización: blocking jerárquico, instrucciones vectoriales AVX2, desenrollado de bucles y paralelización OpenMP. Representa el estado del arte en optimización manual de multiplicación de matrices, sin cambiar el algoritmo de multiplicación.

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

## Compilación 

```bash
# Compilación estándar
make

# Configuración personalizada
make BLOCK_SIZE=32 UNROLL=16 NTHREADS=4

# Ejecutar el código con profiling
./benchmark <size> -p
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
