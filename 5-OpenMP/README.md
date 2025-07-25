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

## Compilación

```bash
# Compilar con valores por defecto
make

# Compilar con parámetros específicos
make NTHREADS=4

# Ejecutar el código con profiling
./benchmark <size> -p
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

## Análisis de Rendimiento

### Escalabilidad
```bash
# Test de escalabilidad (1 a N cores)
for i in {1..8}; do
    echo "Hilos: $i"
    make NTHREADS=$i
    ./benchmark
done
```

## Rendimiento Esperado

- **Speedup ideal**: Lineal con el número de cores
- **Speedup real**: 60-80% del ideal (debido a overhead y sincronización)
- **Eficiencia**: Depende del balance entre trabajo y comunicación
