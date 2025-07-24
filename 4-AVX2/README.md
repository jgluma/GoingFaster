# 4-AVX2 - Optimización con Instrucciones AVX2

## Descripción

Esta implementación utiliza instrucciones AVX2 para vectorización explícita, procesando múltiples elementos simultáneamente mediante instrucciones SIMD de 256 bits. Proporciona control directo sobre la vectorización.

## Archivos

- **`sgemm.c`**: Implementación con intrínsecos AVX2
- **`benchmark.c`**: Programa de pruebas de rendimiento
- **`Makefile`**: Configuración de compilación con flags AVX2
- **`output_files/`**: Directorio para resultados

## Opciones de Compilación

### Variables del Makefile
- **`CC`**: Compilador (gcc)
- **`OPT`**: Optimizaciones + flags AVX2 (`-mavx2 -mfma -O3`)
- **`CFLAGS`**: Flags de compilación vectorial

### Flags de AVX2
```bash
# Habilitar instrucciones AVX2 (256-bit)
-mavx2

# Habilitar FMA (Fused Multiply-Add)
-mfma

# Optimización nivel 3
-O3

# Información de vectorización
-fopt-info-vec-all
```

## Compilación

```bash
# Compilar con soporte AVX2
make

# Verificar instrucciones AVX2 generadas
objdump -d benchmark | grep -E "(vmul|vadd|vfma)"

# Compilar con información detallada
make CFLAGS="-mavx2 -mfma -O3 -fopt-info-vec-all"
```

## Características de AVX2

### Capacidades
- **Ancho de vector**: 256 bits
- **Elementos float**: 8 elementos de 32 bits simultáneamente
- **Instrucciones FMA**: Multiply-Add fusionadas (a*b+c en una instrucción)
- **Throughput**: Hasta 2x mejor que AVX original

### Intrínsecos Típicos Utilizados
```c
// Cargar 8 floats
__m256 va = _mm256_load_ps(&A[i]);

// Multiplicación vectorial
__m256 result = _mm256_mul_ps(va, vb);

// FMA (Fused Multiply-Add)
__m256 result = _mm256_fmadd_ps(va, vb, vc);

// Almacenar resultado
_mm256_store_ps(&C[i], result);
```

## Verificación de Soporte

```bash
# Verificar soporte AVX2 en el CPU
cat /proc/cpuinfo | grep avx2

# Verificar soporte FMA
cat /proc/cpuinfo | grep fma

# Verificar uso en el binario
objdump -d benchmark | grep ymm | head -10
```

## Optimizaciones Implementadas

1. **Vectorización explícita**: Control directo sobre instrucciones SIMD
2. **FMA**: Operaciones fusionadas multiply-add
3. **Alineación de memoria**: Optimización de accesos vectoriales
4. **Desenrollado**: Combinado con vectorización

## Rendimiento Esperado

- **Mejora teórica**: Hasta 8x en operaciones vectorizables
- **Mejora práctica**: 3-5x respecto a la versión naive
- **Limitaciones**: Acceso a memoria puede seguir siendo el cuello de botella

## Requisitos del Sistema

### CPU Soportados
- **Intel**: Haswell (2013) y posteriores
- **AMD**: Excavator (2015) y posteriores

### Verificación de Compatibilidad
```bash
# Verificar familia de CPU
lscpu | grep "Model name"

# Test de instrucciones AVX2
./benchmark  # Fallará si no hay soporte AVX2
```

## Notas de Desarrollo

- **Alineación**: Los datos deben estar alineados a 32 bytes para máximo rendimiento
- **Manejo de bordes**: Casos donde el tamaño no es múltiplo de 8
- **Portabilidad**: Código específico para arquitecturas con AVX2
