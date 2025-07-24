# Going Faster - Matrix Multiplication Optimization

Este proyecto implementa y compara diferentes optimizaciones para la multiplicación de matrices cuadradas (SGEMM - Single precision General Matrix Multiply) en C y Python. Está basado en la sección "Going Faster" del libro "Computer Organization and Design" de Patterson y Hennessy.

## Descripción

El proyecto explora múltiples técnicas de optimización para mejorar el rendimiento de la multiplicación de matrices:

- **Implementaciones naive**: Versiones básicas en Python y C
- **Optimizaciones vectoriales**: Uso de instrucciones AVX/AVX2
- **Loop unrolling**: Desenrollado de bucles para reducir overhead
- **Paralelización**: OpenMP para aprovechar múltiples cores
- **Blocking/Tiling**: Optimización de acceso a memoria cache

## Estructura del Proyecto

```
├── 1-Python/                          # Implementación en Python con NumPy
├── 2-A-Naive/                         # Implementación naive en C
├── 2-B-Naive-Instrumentalizado/       # Versión instrumentalizada para profiling
├── 2-C-Naive-AVX/                     # Versión naive con AVX
├── 3-Unrolling/                       # Loop unrolling
├── 4-AVX2/                            # Optimización con AVX2
├── 5-OpenMP/                          # Paralelización con OpenMP
├── 6-AVX2UnrollingOpenMP/             # Combinación AVX2 + Unrolling + OpenMP
├── 7-Blocking/                        # Técnica de blocking/tiling
├── 8-AVX2BlockUnrollingOpenMP/        # Todas las optimizaciones combinadas
├── clean_results.sh                   # Script para limpiar resultados
├── launch_tests.sh                    # Script para ejecutar pruebas
├── merge_results.sh                   # Script para combinar resultados
└── test_all.sh                        # Script para ejecutar todos los tests
```

## Compilación y Ejecución

### Requisitos
- GCC con soporte para AVX2
- OpenMP
- Python 3 con NumPy (para la versión Python)

### Compilar una implementación específica
```bash
cd <directorio-implementacion>
make
```

### Ejecutar benchmarks
```bash
# Ejecutar todos los tests
./test_all.sh

# Ejecutar tests específicos
./launch_tests.sh

# Limpiar archivos de resultados
./clean_results.sh

# Combinar resultados
./merge_results.sh
```

### Ejecutar una implementación individual
```bash
cd <directorio-implementacion>
./benchmark
```

## Optimizaciones Implementadas

1. **Naive**: Implementación básica con triple bucle anidado
2. **AVX/AVX2**: Uso de instrucciones vectoriales para procesar múltiples elementos simultáneamente
3. **Loop Unrolling**: Desenrollado de bucles para reducir el overhead de control
4. **OpenMP**: Paralelización para aprovechar múltiples cores del procesador
5. **Blocking/Tiling**: Optimización del acceso a memoria reorganizando los cálculos en bloques

## Resultados

Los resultados de benchmark se almacenan en los directorios `output_files/` de cada implementación.

## Autores

Mª Antonia Trenas Castro <matrenas@uma.es>

José Mª González Linares <jgl@uma.es>

## Notas

Faltaría añadir una documentación más detallada de cada técnica y de las opciones de compilación y ejecución