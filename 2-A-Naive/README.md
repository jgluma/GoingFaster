# 2-A-Naive - Implementación Naive en C

## Descripción
Este directorio contiene la implementación naive (línea base) de la multiplicación general de matrices de precisión simple (SGEMM), donde se calcula `C:=C+A×B` para matrices cuadradas de dimensión `n×n` almacenadas en formato row-major (por filas).

Esta versión es intencionalmente la más simple y menos eficiente. Su propósito fundamental es establecer una referencia de rendimiento inicial sobre la cual se medirán las ganancias obtenidas por las técnicas de optimización avanzadas (como unrolling, vectorización AVX y blocking), tal como se explora en el proyecto "Going Faster".

### Algoritmo Principal: Estructura de Bucle Naive (i-j-k)
El núcleo de la implementación se encuentra en la función `square_sgemm` dentro de `sgemm.c`, la cual utiliza la estructura canónica de triple bucle anidado (i−j−k):
```
void square_sgemm (int n, float* A, float* B, float* C)
{
    /* Para cada fila i de C */
    for (int i = 0; i < n; ++i)
        /* Para cada columna j de C */
        for (int j = 0; j < n; ++j)
        {
            /* Cálculo del elemento C[i][j] */
            for( int k = 0; k < n; k++ )
                C[OFF(i,j)] += A[OFF(i,k)] * B[OFF(k,j)]; /* C[i][j] += A[i][k]*B[k][j] */
        }
}
```
Donde `OFF(row, col)` es una macro que realiza la indexación `row×n+col` para acceder a los elementos en el almacenamiento row-major.

### Análisis Arquitectónico y de Rendimiento
Esta implementación naive presenta limitaciones críticas en la jerarquía de memoria y la explotación del paralelismo de datos (DLP), convirtiéndola en un caso de estudio ideal para optimización:

| Característica | Detalle |
| --- | --- |
| **Complejidad Asintótica** | $O(n^3)$. El costo operacional es el estándar para la multiplicación de matrices, lo que define el número total de operaciones de punto flotante (FLOPs). |
| **Acceso a Memoria (Caché)** | No optimizado para caché. El orden de bucle i−j−k resulta en una pésima localidad espacial para la matriz B. Mientras que los accesos a `A[i][k]` y `C[i][j]` son contiguos (o tienen un stride predecible), el acceso a `B[k][j]` en formato row-major requiere saltos grandes (stride de `n` elementos) por cada incremento en `k`, maximizando los fallos de caché. |
| **Paralelización** | No implementada. El código es estrictamente secuencial. Las versiones optimizadas requerirán el uso de directivas como OpenMP para explotar el paralelismo a nivel de thread (TLP). |
| **Vectorización (SIMD)** | Depende completamente del auto-vectorizador del compilador. Aunque se utiliza el flag -O3, la estructura de bucle i−j−k y la falta de localidad de `B` dificultan que el compilador vectorize efectivamente el bucle más interno `k`. |

### Configuración de Compilación
La compilación se realiza utilizando el compilador GCC con un conjunto de flags que buscan exprimir el máximo rendimiento posible del código, incluso en su versión no optimizada:
* **`Compilador (CC)`**: `gcc`.
* **`Optimización (OPT)`**: `-O3`. Este es el nivel más alto de optimización y fuerza al compilador a aplicar agresivamente técnicas como *loop transformations* y, crucialmente, la auto-vectorización de instrucciones SIMD (como SSE/AVX).
* **`Flags de Compilación (CFLAGS)`**: `-Wall -Wno-unused-result -std=gnu99 $(OPT)`. Fija el nivel de *warning* al conjunto completo de avisos pero quita el aviso de "no se usa un valor devuelto por una función". También fija la especificación de C a GNU99 para poder declarar variables dentro de declaraciones de bucle.

Esta configuración permite al estudiante analizar el límite de lo que se puede lograr simplemente confiando en la inteligencia del compilador, antes de implementar optimizaciones manuales que mejoren la gestión de la jerarquía de memoria (p. ej., blocking o reordenamiento de bucles).

### Archivos del Módulo
* **`sgemm.c`**: Contiene la implementación de la función square_sgemm.
* **`benchmark.c`**: Programa de prueba que mide el tiempo de ejecución y realiza una verificación de la exactitud del resultado (check_correctness).
* **`Makefile`**: Define las reglas de compilación, utilizando -O3.

### Instrucciones de Compilación y Ejecución

#### Compilar el programa con optimización -O3
```make```

#### Ejecutar el benchmark (por defecto usa matrices 512x512)
```./benchmark```

#### Ejecutar con un tamaño específico (e.g., 1024x1024)
```./benchmark 1024```

#### Generar el código ensamblador (para analizar la auto-vectorización)
```make ensambla```

#### Limpiar archivos objeto
```make clean```

### Verificación de la corrección
El código incluye una función, `check_correctness`, cuyo objetivo es contrastar el resultado obtenido por la función implementada (`square_sgemm`) contra un resultado de referencia generado por una implementación canónica y probadamente correcta (`ref_square_sgemm`). 

Esta verificación es crítica, ya que la aplicación de optimizaciones agresivas (como el desenrollado de bucles, la vectorización AVX o el *tiling*) puede introducir errores sutiles de programación que alteren la lógica de la multiplicación matricial. Una vez que se tiene la matriz `C` (resultado de la función bajo prueba) y `Cref` (resultado de la referencia), se comparan elemento a elemento para cuantificar el error. Para ello se calcula una diferencia absoluta acumulada (`dif`) iterando sobre los `n×n` elementos. 

$$ dif= \sum_{x=0}^{n^2−1} \|C\[x\]−Cref\[x\]\| $$
 
Debido a la inherente inexactitud y acumulación de errores de redondeo en las operaciones de punto flotante (precisión simple, float), se establece una tolerancia máxima, `max_error`, fijada en 0.00001.
- Si la diferencia acumulada es mayor que la tolerancia (`dif > max_error`), se imprime un mensaje de fallo (`Bad multiplication routine, absolute error: %f`)
- Si la diferencia es aceptable, se confirma la corrección de la rutina (`Multiplication routine works correctly`). La verificación de corrección solo debe hacerse al modificar la rutina `square_sgemm`, pero nunca cuando se quieran medir tiempos.

#### Ejecutar el benchmark verificando la corrección
```./benchmark -c```
