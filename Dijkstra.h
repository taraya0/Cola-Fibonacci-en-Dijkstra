#include <bits/stdc++.h>
using namespace std;
#include "heap.h"
// Definimos los pares así al inicio para facilidad de su uso en la priority_queue
typedef pair<double, int> ii;

#define INF 1 << 23

/*  EJEMPLO DE GRAFO
*  0->[< 0, 1 >,< 0, 2 >]
*  1->[< 0, 2 >]  
*  2->[< 0 , 4>]
*  3->[< 0 , 1 >,< 0 , 4 >]
*  4->[< 0 , 1>]
*/

void caminoMasCorto(int s, int n, vector<vector<ii>> gr, ColaPrioridad cola,
                            vector<double>* pdistancia, vector<int>* pPrevios) { //n numero de nodos, s nodo inicial, gr es la lista de adyacencia del grafo que ahora tiene pares (distancia, nodo)
    //PASO 1:
    // Cambiamos el tamaño de distancia y previos
    (*pdistancia).resize(n);
    (*pPrevios).resize(n);
    // camino final es 0 4 2 1 : [-1, 2, x, 4, x, 0]

    //PASO 2
    // Iniciamos la pq con pares que guardaran (distancia,nodo)
    // y cambiamos su configuración para que nos de los más pequeños primero
    // ** esto se reemplaza en los casos de heap y cola de fibonacci
    //priority_queue<ii, vector<ii>, greater<ii>> Q;

    //PASO 3
    // Metemos la fuente a la cola y asignamos su distancia
    (*pdistancia)[s] = 0;
    (*pPrevios)[s] = -1;
    cola.push({0, s});
    
    //PASO 4: 
    //**** distancia ya esta inicializada con infinito
    //falta inicializar bien previos
    //se mete en la cola los nodos con sus distancias
    for(int i = 0; i < n; i++){
        if(i != s) {
            (*pdistancia)[i] = INF;
            (*pPrevios)[i] = -2;
            cola.push({INF,i}); // (distancia, nodo)
        }
    }
    //PASO 5:
    //heapifear
    cola.heapify(cola.heap, cola.heap.size(), 0);
    
    
    //PASO 6:
    // Mientras la cola no esté vacía, tenemos nodos que revisar
    int previo = -1;
    while(!cola.empty()) {
  
        // Sacamos el nodo que esté a menor distancia ahora mismo  
        //auto [nodo_d, nodo] = cola.top();
        //cola.pop();
        auto top = cola.top();
        double nodo_d = top.first;
        int nodo = top.second;


        // Revisamos sus vecinos: vecino_d es el peso entre nodo y vecino
        //for(auto [vecino_d, vecino]: gr[nodo]) {
        for (auto& vecino : gr[nodo]) {
            double vecino_d = vecino.first;
            int vecino_node = vecino.second;
            // Si la distancia guardada para el vecino es menor a la distancia de nodo + el peso
            if ((*pdistancia)[vecino_node] > nodo_d + vecino_d) {
                (*pdistancia)[vecino_node] = nodo_d + vecino_d; //se actualiza el valor de la distancia en el arreglo
                (*pPrevios)[vecino_node] = nodo;
                cola.decreaseKey(vecino_node , nodo_d + vecino_d); 
            }
        }
        
    }

    //PASO 7: 
    // Se retorna distancia y previos(ya se modificaron utilizando punteros)
}


/**/
int main(){
    vector<double> distancias; 
    vector<int> previos;
    //asjdnasjnads el tamaño del grafo
    int n = 5;
    //Creamos un grafo de ejemplo
    vector<vector<ii>> gr(n);
    gr[0].push_back({3, 1});
    gr[1].push_back({3, 0});

    gr[0].push_back({10, 4});
    gr[4].push_back({10, 0});
    
    gr[1].push_back({11, 3});
    gr[3].push_back({11, 1});
    
    gr[3].push_back({5, 2});
    gr[2].push_back({5, 3});
    
    gr[4].push_back({5, 3});
    gr[3].push_back({5, 4});
    
    //hacer cola de prioridad
    ColaPrioridad cola;
    //probar la weaita jajaja
    caminoMasCorto(2, n, gr, cola, &distancias, &previos);

    for(int i = 0; i < n; i++) {
        cout << "Distancia para " << i << " es: " << distancias[i] << endl; 
        cout << "Previos para " << i << " es: " << previos[i] << endl; 
    }

    return 0;
}

// how to compile: g++ -std=c++11 Dijkstra.cpp -o djPenguin