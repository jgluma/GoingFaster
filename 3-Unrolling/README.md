# 3-Unrolling - Loop Unrolling (Desenrollado de Bucles)

## Descripción

Esta implementación utiliza la técnica de loop unrolling para reducir el overhead de control de bucles y mejorar el rendimiento mediante la ejecución de múltiples iteraciones en cada paso del bucle.

## Archivos

- **`sgemm.c`**: Implementación con bucles desenrollados
- **`benchmark.c`**: Programa de pruebas de rendimiento
- **`Makefile`**: Configuración de compilación
- **`output_files/`**: Directorio para resultados

## Opciones de Compilación

### Variables del Makefile
- **`CC`**: Compilador (gcc)
- **`OPT`**: Optimizaciones (`-O3`)
- **`CFLAGS`**: Flags de compilación con optimizaciones agresivas

### Flags de Optimización
```bash
# Optimización nivel 3
-O3

# Desenrollado automático adicional (opcional)
-funroll-loops

# Información de optimizaciones
-fopt-info-optimized
```

## Compilación

```bash
# Compilar con optimizaciones
make

# Compilar con desenrollado automático adicional
make OPT="-O3 -funroll-loops"

# Ver código desenrollado generado
make assembly
```

## Técnica de Loop Unrolling

### Concepto
El loop unrolling reduce el overhead del bucle ejecutando múltiples iteraciones por cada iteración real del bucle.

### Ejemplo de Transformación
```c
// Bucle original
for (int k = 0; k < n; k++) {
    C[i*n + j] += A[i*n + k] * B[k*n + j];
}

// Bucle desenrollado (factor 4)
for (int k = 0; k < n; k += 4) {
    C[i*n + j] += A[i*n + k] * B[k*n + j];
    C[i*n + j] += A[i*n + k+1] * B[(k+1)*n + j];
    C[i*n + j] += A[i*n + k+2] * B[(k+2)*n + j];
    C[i*n + j] += A[i*n + k+3] * B[(k+3)*n + j];
}
```

## Parámetros Configurables

### Factor de Desenrollado
- **Típico**: 2, 4, 8, 16
- **Consideraciones**: Balance entre tamaño de código y rendimiento

### Compilación con Factor Específico
```bash
# Compilar con factor específico (si está implementado)
make UNROLL_FACTOR=4
```

## Ventajas

1. **Reducción de overhead**: Menos operaciones de control de bucle
2. **Mejor utilización del pipeline**: Más instrucciones independientes
3. **Oportunidades de optimización**: Permite más optimizaciones del compilador
4. **Mejor localidad**: Potencial para mejor uso de registros

## Limitaciones

1. **Tamaño de código**: Incremento significativo del código generado
2. **Cache de instrucciones**: Posible presión adicional
3. **Complejidad**: Manejo de casos límite (cuando n no es múltiplo del factor)

## Rendimiento Esperado

- **Mejora típica**: 20-40% respecto a la versión naive
- **Factores**: Depende del factor de desenrollado y la arquitectura del procesador

## Análisis

```bash
# Comparar tamaño del código
size benchmark

# Analizar instrucciones generadas
objdump -d benchmark > unrolled_assembly.txt
```
