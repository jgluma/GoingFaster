# 5-OpenMP - Paralelización con OpenMP

## Descripción

Esta implementación utiliza OpenMP para paralelizar la multiplicación de matrices, distribuyendo el trabajo entre múltiples hilos de CPU para aprovechar sistemas multi-core.

## Archivos

- **`sgemm.c`**: Implementación paralelizada con OpenMP
- **`benchmark.c`**: Programa de pruebas de rendimiento
- **`Makefile`**: Configuración de compilación con OpenMP
- **`output_files/`**: Directorio para resultados

## Opciones de Compilación

### Variables del Makefile
- **`CC`**: Compilador (gcc)
- **`OPT`**: Optimizaciones + OpenMP (`-O3 -fopenmp`)
- **`CFLAGS`**: Flags de compilación + definiciones de parámetros
- **`LDLIBS`**: Librerías de enlace (`-fopenmp`)

### Parámetros Configurables
```bash
# Tamaño de bloque (default: 8)
BLOCKSIZE ?= 8

# Factor de desenrollado (default: 4)  
UNROLL ?= 4

# Número de hilos (default: 2)
NTHREADS ?= 2
```

## Compilación

```bash
# Compilar con valores por defecto
make

# Compilar con parámetros específicos
make BLOCKSIZE=16 UNROLL=8 NTHREADS=4

# Compilar para arquitectura específica
make OPT="-O3 -fopenmp -march=native"
```

## Configuración de OpenMP

### Variables de Entorno
```bash
# Número de hilos
export OMP_NUM_THREADS=4

# Política de scheduling
export OMP_SCHEDULE="static"

# Afinidad de hilos
export OMP_PROC_BIND=true

# Lugares de ejecución
export OMP_PLACES=cores
```

### Ejecución con Diferentes Configuraciones
```bash
# Ejecutar con 1 hilo (secuencial)
OMP_NUM_THREADS=1 ./benchmark

# Ejecutar con 4 hilos
OMP_NUM_THREADS=4 ./benchmark

# Ejecutar con todos los cores disponibles
OMP_NUM_THREADS=$(nproc) ./benchmark
```

## Directivas OpenMP Utilizadas

### Paralelización de Bucles
```c
#pragma omp parallel for
for (int i = 0; i < n; i++) {
    // Trabajo paralelizado
}

// Con scheduling específico
#pragma omp parallel for schedule(static)

// Con variables privadas
#pragma omp parallel for private(j, k)
```

### Regiones Paralelas
```c
#pragma omp parallel
{
    int thread_id = omp_get_thread_num();
    // Código ejecutado por cada hilo
}
```

## Parámetros de Optimización

### BLOCKSIZE
- **Función**: Tamaño de bloque para tiling de cache
- **Valores típicos**: 8, 16, 32, 64
- **Impacto**: Localidad de memoria

### UNROLL
- **Función**: Factor de desenrollado de bucles
- **Valores típicos**: 2, 4, 8
- **Impacto**: Reducción de overhead de bucle

### NTHREADS
- **Función**: Número de hilos de ejecución
- **Valores típicos**: Número de cores físicos
- **Impacto**: Paralelización

## Análisis de Rendimiento

### Escalabilidad
```bash
# Test de escalabilidad (1 a N cores)
for i in {1..8}; do
    echo "Hilos: $i"
    OMP_NUM_THREADS=$i ./benchmark
done
```

### Profiling con OpenMP
```bash
# Información de hilos
export OMP_DISPLAY_ENV=true
./benchmark

# Información de afinidad
export OMP_DISPLAY_AFFINITY=true
./benchmark
```

## Rendimiento Esperado

- **Speedup ideal**: Lineal con el número de cores
- **Speedup real**: 60-80% del ideal (debido a overhead y sincronización)
- **Eficiencia**: Depende del balance entre trabajo y comunicación

## Optimizaciones Adicionales

### Scheduling
```bash
# Estático (distribución uniforme)
export OMP_SCHEDULE="static"

# Dinámico (balanceado de carga)
export OMP_SCHEDULE="dynamic,16"

# Guiado (chunks adaptativos)
export OMP_SCHEDULE="guided"
```

### Afinidad de Hilos
```bash
# Afinidad por cores
export OMP_PROC_BIND=spread
export OMP_PLACES=cores

# Afinidad por sockets
export OMP_PLACES=sockets
```

## Debugging

```bash
# Información detallada de OpenMP
export OMP_DISPLAY_ENV=verbose
export OMP_DISPLAY_AFFINITY=format="%0.3E sec: %P"

# Verificar número de hilos usado
./benchmark 2>&1 | grep -i thread
```
