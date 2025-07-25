# 2-B-Naive-Instrumentalizado - Versión Instrumentalizada para Profiling

## Descripción

Esta versión es idéntica a la implementación naive pero compilada con flags adicionales para instrumentalización y profiling. Permite analizar el comportamiento del programa y identificar cuellos de botella.

## Archivos

- **`sgemm.c`**: Implementación naive instrumentalizada
- **`benchmark.c`**: Programa de pruebas con instrumentalización
- **`Makefile`**: Configuración de compilación con flags de profiling
- **`output_files/`**: Directorio para almacenar resultados de profiling

## Instrumentalización

Las rutinas de instrumentalización se encargan de obtener los valores de 6 métricas de rendimiento durante la ejecución de una sección del código:

- Instrucciones retiradas (es decir, totalmente ejecutadas)
- Ciclos de CPU
- Referencias a memoria caché
- Fallos de memoria caché
- Instrucciones de salto
- Fallos de predicción de salto

Se pueden obtener otras métricas (```perf list``` proporciona un listado con las métricas disponibles), pero no es conveniente leer muchas métricas al mismo tiempo ya que los contadores HW que las almacenan están limitados y tendrían que multiplexarse.

## Compilación y Profiling

```bash
# Compilar con instrumentalización
make

# Ejecutar para generar datos de profiling
./benchmark
```

## Herramientas de Análisis

Hay otras herramientas que se pueden utilizar para analizar el rendimiento de un código, como gprof, valgrind o perf, pero realizan el análisis sobre todo el programa.

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

# Obtención del listado de métricas disponible en ese procesador
perf list
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
