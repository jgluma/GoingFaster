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

## Compilación

```bash
# Compilar con optimizaciones completas
make

# Configuración personalizada
make UNROLL=16 NTHREADS=8

# Ejecutar el código con profiling
./benchmark <size> -p
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

## Rendimiento Esperado

### Mejoras Acumulativas
- **Vs Naive**: 10-20x mejora
- **Vs OpenMP solo**: 3-5x mejora adicional por vectorización
- **Vs AVX2 solo**: 2-4x mejora adicional por paralelización

### Factores Limitantes
1. **Ancho de banda de memoria**: Puede ser el cuello de botella final
2. **Contención de hilos**: Overhead en sistemas con muchos cores