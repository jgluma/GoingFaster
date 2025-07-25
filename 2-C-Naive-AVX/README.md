# 2-C-Naive-AVX - Implementación Naive con AVX

## Descripción

Esta implementación mantiene el algoritmo naive de triple bucle pero está compilada con soporte para instrucciones AVX. Permite al compilador utilizar vectorización automática con instrucciones SIMD. El objetivo de este código es comprobar el rendimiento máximo que se puede obtener de forma automática sin intervención del programador.

## Archivos

- **`benchmark.c`**: Programa de pruebas, incluye la rutina de multiplicación de matrices
- **`Makefile`**: Configuración de compilación con flags AVX

## Opciones de Compilación

### Variables del Makefile
- **`CC`**: Compilador (gcc)
- **`OPT`**: Optimizaciones + flags AVX (descomentar las distintas opciones)

## Compilación

```bash
# Compilar con soporte AVX
make

# Ejecutar el código
./benchmark

# Verificar uso de instrucciones AVX en ensamblador
objdump -d benchmark | grep -i vax
```

## Características

- **Instrucciones SIMD**: AVX (256-bit, procesa 8 floats simultáneamente)
- **Vectorización**: Automática por el compilador
- **Algoritmo**: Mantiene el triple bucle naive
- **Compatibilidad**: Requiere CPU con soporte AVX (Intel Sandy Bridge+, AMD Bulldozer+)

## Verificación de Soporte AVX

```bash
# Verificar soporte AVX en el CPU
cat /proc/cpuinfo | grep avx

# Verificar que el binario usa AVX
objdump -d benchmark | grep ymm
```

## Rendimiento Esperado

- **Mejora teórica**: Hasta 8x en operaciones vectorizables
- **Mejora real**: Depende de la efectividad del auto-vectorizador
- **Limitaciones**: Acceso a memoria sigue sin optimizar

## Notas

- El compilador debe identificar automáticamente los bucles vectorizables
- La mejora de rendimiento depende de la alineación de datos en memoria
- Puede no mostrar mejoras significativas debido al acceso no optimizado a memoria del algoritmo naive
