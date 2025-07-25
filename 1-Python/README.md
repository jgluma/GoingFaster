# 1-Python - Implementación en Python con NumPy

## Descripción

Esta implementación utiliza Python con la librería NumPy para realizar la multiplicación de matrices cuadradas de 512x512. Sirve como referencia de rendimiento utilizando librerías optimizadas y como comparación con las implementaciones en C.

## Archivos

- **`benchmark.py`**: Script principal que ejecuta las pruebas de rendimiento
- **`sgemm.py`**: Implementación de la multiplicación de matrices en Python

## Ejecución

```bash

# Asegúrate de tener NumPy instalado
pip3 install numpy

# Ejecutar el benchmark
python3 benchmark.py

```

## Características

- **Librería utilizada**: NumPy para crear las matrices
- **Lenguaje**: Python 3
- **Optimizaciones**: Prueba a modificar la función square_sgemm para que utilize el producto np.dot de Numpy, el cual incluye técnicas avanzadas como las que se incluyen en el resto de secciones

## Notas

- NumPy utiliza implementaciones optimizadas de BLAS que pueden incluir Intel MKL o OpenBLAS
- Esta implementación sirve como baseline para comparar el rendimiento de las optimizaciones manuales en C
- El rendimiento puede variar dependiendo de la instalación de NumPy y las librerías BLAS subyacentes
