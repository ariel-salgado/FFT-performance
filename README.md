# FFT Performance Comparison
Este proyecto implementa dos algoritmos de Transformada Rápida de Fourier (FFT, por sus siglas en inglés): una versión secuencial y otra que utiliza operaciones vectoriales para mejorar el rendimiento.

## Descripción
La FFT es un algoritmo fundamental en el procesamiento de señales y análisis de datos. En este proyecto, se desarrollarán dos versiones de la FFT con enfoques diferentes para la optimización del rendimiento:

- **FFT Secuencial:** Esta implementación de la FFT se desarrollará utilizando un enfoque secuencial y sencillo. El objetivo es proporcionar una implementación básica y fácil de entender de la FFT sin utilizar características avanzadas.

- **FFT Vectorial:** Esta versión se basará en el uso de instrucciones vectoriales, siendo más específico SSE (Streaming SIMD Extensions) para realizar cálculos simultáneos en múltiples elementos de datos. Esto permite procesar varios elementos de la FFT de manera eficiente y mejorar el rendimiento de la transformada.

El objetivo final de este proyecto es realizar una comparación del rendimiento entre ambas implementaciones. Se analizará la velocidad de ejecución, para determinar las ventajas de cada enfoque en diferentes escenarios y configuraciones.

## Uso
Para utilizar y ejecutar las implementaciones de la FFT, siga estas instrucciones:

- Compilar con `make`
- Ejecutar el código con `./main -n SIZE [-s] [-h]`

*Nota: Las flags entre corchetes son flags opcioniales.*

- `-n:` Corresponde al exponente en base 2. Ejemplo: `SIZE = 3`, entonces `n = 8`, ya que `2^3 = 8`.
- `-s:` Habilita el uso de operaciones vectoriales en el cálculo de la FFT. Por defecto es falso.
