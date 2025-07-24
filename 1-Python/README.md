# 1-Python - Implementación en Python con NumPy

## Descripción

Esta implementación utiliza Python con la librería NumPy para realizar la multiplicación de matrices. Sirve como referencia de rendimiento utilizando librerías optimizadas y como comparación con las implementaciones en C.

## Archivos

- **`benchmark.py`**: Script principal que ejecuta las pruebas de rendimiento
- **`sgemm.py`**: Implementación de la multiplicación de matrices usando NumPy

## Ejecución

```bash
# Ejecutar el benchmark
python3 benchmark.py

# Asegúrate de tener NumPy instalado
pip3 install numpy
```

## Características

- **Librería utilizada**: NumPy (altamente optimizada con BLAS/LAPACK)
- **Lenguaje**: Python 3
- **Optimizaciones**: Las proporcionadas por NumPy (vectorización, SIMD, multithreading)

## Notas

- NumPy utiliza implementaciones optimizadas de BLAS que pueden incluir Intel MKL o OpenBLAS
- Esta implementación sirve como baseline para comparar el rendimiento de las optimizaciones manuales en C
- El rendimiento puede variar dependiendo de la instalación de NumPy y las librerías BLAS subyacentes
