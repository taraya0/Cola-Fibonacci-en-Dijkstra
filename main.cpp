#include <bits/stdc++.h>
#include <chrono>
#include <ctime>
#include <iostream>
#include <fstream>
using namespace std;
using namespace chrono;
#include "DijkstraHeap.h"
#include "DijkstraFib.h"
typedef pair<double, int> ii;
// <peso, nodo>

// Número de consultas
#define L 50

// Define un hash para pares de enteros
struct pair_hash {
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2>& pair) const {
        auto hash1 = std::hash<T1>{}(pair.first);
        auto hash2 = std::hash<T2>{}(pair.second);
        return hash1 ^ hash2;
    }
};

void printGrafo(vector<vector<ii>> grafo) {
	for (int i = 0; i < grafo.size(); ++i) {
		cout << i << ": ";
		for (int j = 0; j < grafo[i].size(); ++j) {
			cout << "(" << grafo[i][j].first << "," << grafo[i][j].second << ")";
		}
		cout << endl;
	}
}

ii generarArista(int j, int nodo, vector<vector<ii>>& grafo, unordered_set<pair<int, int>, pair_hash>& aristas) {
    // Generamos una arista del grafo aleatoria
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> peso(0, 1);
    uniform_int_distribution<int> dist(0, j-1);
    int res = dist(gen);
    while (res == nodo) {
        res = dist(gen);
    }

    // Aseguramos que no exista la arista
    while (aristas.count({min(nodo, res), max(nodo, res)})) {
        res = dist(gen);
        while (res == nodo) {
            res = dist(gen);
        }
    }

    // Añadimos la arista al conjunto
    aristas.insert({min(nodo, res), max(nodo, res)});

    return {peso(gen), res};
}

int generarNodo(int j) {
    // Generamos un nodo aleatorio
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0, j-1);
    return dist(gen);
}

// Cambiar función de multigrafo a grafo normal
// n es la cantidad de nodos y e de enlaces
void crearGrafo(vector<vector<ii>>& grafo, int i, int e) {
    // Se le asigna tamaño
    int v = 1 << i;
    grafo.resize(v);
    unordered_set<pair<int, int>, pair_hash> aristas; // Conjunto de aristas

    // Añadir arista random a 0
    ii aristaInicial = generarArista(v, 0, grafo, aristas);
    grafo[0].push_back(aristaInicial);
    grafo[aristaInicial.second].push_back({aristaInicial.first, 0});  // Añadimos la arista inversa

    // Rellenar los demás
    for (int k = 1; k < v; k++) {
        
		//caso especial donde todas las aristas apuntan al ultimo  nodo
		
		if(grafo[k].size() == v-1){
			k--;
			ii aristaRandom = generarArista(v, k, grafo, aristas);
			grafo[k].push_back(aristaRandom);
			grafo[aristaRandom.second].push_back({aristaRandom.first, k});
			break;
		}
		
		// Generar arista random para el nodo
        ii aristaRandom = generarArista(v, k, grafo, aristas);
        grafo[k].push_back(aristaRandom);
        grafo[aristaRandom.second].push_back({aristaRandom.first, k});
    }

    // Aquí rellenar los 2^e - v
    int h = (1 << e) - v;
    while (h > 0) {
        // Elegir el nodo
        int indice = generarNodo(v);
		//nos aseguramos de que un nodo no este lleno
		
		if (grafo[indice].size() == (v-1)){
			continue;
		}
        // Generar arista random
        ii aristaRandom = generarArista(v, indice, grafo, aristas);
        grafo[indice].push_back(aristaRandom);
        grafo[aristaRandom.second].push_back({aristaRandom.first, indice});  // Añadimos la arista inversa
        h--;
    }
}



int main(){
	cout << "------------------------- INICIO -------------------------" << endl;
	// archivo para guardar resultados
	ofstream archivo;
	cout << "Creando archivo de resultados" << endl;
	archivo.open("resultados.txt", fstream::out);
	archivo << "Resultados de la ejecución" << endl;
	archivo << endl;
	archivo.close();

	vector<double> averageTime_per_L_heap(L);
	vector<double> averageTime_per_L_fib(L);

	// ------------------- tests -------------------
	// grafo con v = 2i nodos, i ∈ {10, 12, 14}
	for(int i = 10; i <= 14; i+=2) {

		int v = pow(2, i);
		vector<double> times_heap(L);
        	vector<double> times_fib(L);

		// Calcular el máximo valor de j permitido para el número de nodos 2^i
        	int max_j = min(22, static_cast<int>(log2((1 << i) * (1 << i - 1) / 2)));

		// e = 2j aristas con pesos aleatorios y uniformes dentro del rango (0..1], j ∈ [16...22]
		for(int j = 16; j <= max_j; j++) {
			cout << "Creando grafo con v = 2^" << i << " nodos" << endl;
			cout << "Creando grafo con e = 2^" << j << " aristas" << endl;

			int e = pow(2, j);

			// crea grafo
			vector<vector<ii>> gr;
			crearGrafo(gr, i, j);
			cout << "Se creo el grafo " << endl;
			// printGrafo(gr);

			vector<int> previos; 
			vector<double> distancias;
			vector<int> previosFib; 
			vector<double> distanciasFib;

			ColaPrioridad heap;
			FibonacciHeap fib;

			int f = 0;
			while(f < L) {

				cout << "------------------------- HEAP -------------------------" << endl;
				cout << "Distancias usando Cola de Prioridad (Heap) para i = " << i << " y j = " << j << endl;

				auto begin_h = high_resolution_clock::now();
				caminoMasCorto(0, v, gr, heap, &distancias, &previos);
				auto end_h = high_resolution_clock::now();

				// Calcula el tiempo transcurrido y lo almacena en el vector
				double time_h = duration_cast<nanoseconds>(end_h - begin_h).count() / 1e9;
				times_heap[f] = time_h;
				cout << "Tiempo Heap: " << time_h << endl;
            			cout << endl;
					
				for(int k = 0; k < gr.size(); ++k) {
					cout << "Distancia Nodo " << k << ": " << distancias[k] << endl; 
					cout << "Previos Nodo " << k << ": " << previos[k] << endl; 
				}


				cout << "----------------------- FIBONACCI -----------------------" << endl;
				cout << "Distancias usando Cola de Prioridad (Fibonacci Heap) para i = " << i << " y j = " << j << endl;
					
				auto begin_f = high_resolution_clock::now();
				caminoMasCortoFib(0, v, gr, fib, &distanciasFib, &previosFib);
				auto end_f = high_resolution_clock::now();

				// Calcula el tiempo transcurrido y lo almacena en el vector
				double time_f = duration_cast<nanoseconds>(end_f - begin_f).count() / 1e9;
				times_fib[f] = time_f;
				cout << "Tiempo Fibonacci: " << time_f << endl;
            			cout << endl;
					
				for(int k = 0; k < gr.size(); ++k) {
					cout << "Distancia Nodo " << k << ": " << distanciasFib[k] << endl; 
					cout << "Previos Nodo " << k << ": " << previosFib[k] << endl; 
				}

				f++;

			}

			// Calcula tiempo promedio Heap y Fibonacci
			double averageTime_heap = accumulate(times_heap.begin(), times_heap.end(), 0.0) / L;
        		double averageTime_fib = accumulate(times_fib.begin(), times_fib.end(), 0.0) / L;

			// Guarda los resultados en un archivo
			archivo.open("resultados.txt", fstream::app);

			archivo << "v: 2^" << i << " = " << v << endl;
			archivo << "e: 2^" << j << " = " << e << endl;
			archivo << "Tiempo promedio cola de prioridad Heap: " << averageTime_heap << endl;
			archivo << "Tiempo promedio cola de Fibonacci: " << averageTime_fib << endl;
			archivo << endl;

			archivo.close();
		}

	}

	return 0;

}

// compile: g++ -std=c++11 main.cpp -o mainPenguin
