# 2-B-Naive-Instrumentalizado - Versión Instrumentalizada para Profiling

## Descripción

Esta versión es idéntica a la implementación naive pero compilada con flags adicionales para instrumentalización y profiling. Permite analizar el comportamiento del programa y identificar cuellos de botella.

## Archivos

- **`sgemm.c`**: Implementación naive instrumentalizada
- **`benchmark.c`**: Programa de pruebas con instrumentalización
- **`Makefile`**: Configuración de compilación con flags de profiling
- **`output_files/`**: Directorio para almacenar resultados de profiling

## Opciones de Compilación

### Variables del Makefile
- **`CC`**: Compilador (gcc)
- **`OPT`**: Optimizaciones + instrumentalización
- **`CFLAGS`**: Flags estándar + flags de profiling

### Flags de Instrumentalización Típicos
```bash
# Profiling con gprof
-pg

# Información de debug
-g

# Cobertura de código
-fprofile-arcs -ftest-coverage

# Información de vectorización
-fopt-info-vec-optimized -fopt-info-vec-missed
```

## Compilación y Profiling

```bash
# Compilar con instrumentalización
make

# Ejecutar para generar datos de profiling
./benchmark

# Analizar con gprof (si se compiló con -pg)
gprof ./benchmark gmon.out > analysis.txt

# Ver información de vectorización
gcc -O3 -fopt-info-vec-all -c sgemm.c
```

## Herramientas de Análisis

### gprof
```bash
# Generar reporte de profiling
gprof ./benchmark > profile_report.txt
```

### Valgrind
```bash
# Análisis de cache
valgrind --tool=cachegrind ./benchmark

# Análisis de memory leaks
valgrind --tool=memcheck ./benchmark
```

### perf
```bash
# Análisis de rendimiento
perf record ./benchmark
perf report
```

## Uso

Esta versión es especialmente útil para:
- **Identificar hotspots** en el código
- **Analizar patrones de acceso a memoria**
- **Medir efectividad de la vectorización automática**
- **Obtener métricas detalladas de rendimiento**

## Notas

- Los flags de instrumentalización pueden afectar el rendimiento
- Usar esta versión solo para análisis, no para benchmarks de rendimiento final
- Los resultados se almacenan en `output_files/` para posterior análisis
