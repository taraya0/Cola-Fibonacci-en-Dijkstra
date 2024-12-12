# Análisis de la cola de Fibonacci en Dijkstra
Se implementa el algoritmo de Dijkstra utilizando dos estructuras de datos distintas como cola de prioridad:
- Un heap. El costo teorico del algoritmo con esta estrucutura debiera ser O(e log v)
- Una cola de Fibonacci. El costo teorico del algoritmo con esta estrucutura debiera ser O(e + v log v)
El objetivo de este experimento es medir el rendimiento de ambos algoritmos y compararlo con el costo real. Para esto, se implementa un algoritmo que genera multigrafos aleatorios donde correr ambos algoritmos.
## Implementación de algoritmo de Dijkstra
Existe una implementación distinta del algoritmo de Dijkstra para cada cola de prioridad. En el archivo **DijsktraHeap.h** se encuentra la correspondiente para el heap, mientras que en **DijkstraFib.h** está la de la cola de Fibonacci.
## Implementación de estructuras de datos
Los archivos con la implementación de cada estructura de datos son autoreferentes: en **heap.h** se almacena el struct y las funciones que describen el comportamiento del heap, mientras que en **fibonacci.h** se encuentra la clase y sus métodos que implementan el comportamiento de esta cola.
# Archivo Main
El archivo main cuenta con funciones para generar un multigrafo dirigido con pesos aleatorizados, además del flujo principal de la experimentación que mide los costos utilizando la librería Ctime. 
# Archivo resultados
Al ejecutar el archivo main se genera un archivo de texto **resultados.txt** con las mediciones de los costos promedio de cada algoritmo, se utilizan multigrafos aleatorios con 2^10 vertices y una cantidad aristas pertenecientes al conjunto {2^16,2^17,2^18}
