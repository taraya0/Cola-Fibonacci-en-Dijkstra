#include <bits/stdc++.h>
using namespace std;
typedef pair<double, int> ii;

/*
Como estructura Q, se utiliza un Heap como cola de prioridad, este debe soportar las funciona-
lidades mencionadas en la descripción del algoritmo, y que se construya en tiempo lineal. En este
primer algoritmo, para la función decreaseKey se espera un tiempo logarítmico, en el que actualizar
un elemento de Q reordena la estructura del árbol
*/
struct ColaPrioridad {
    vector<ii> heap;
    unordered_map<int, double> posMap; // Map to store the position of nodes in the heap

    void push(ii p) {
        heap.push_back(p);
        int index = heap.size() - 1;
        posMap[p.second] = index;
    }

    // Devuelve el elemento con mayor prioridad (el mínimo en este caso)
    ii top() {
        if (heap.empty()) {
            throw runtime_error("Heap is empty");
        }
        return heap.front();
    }

    // Elimina el elemento con mayor prioridad (el mínimo en este caso)
    void pop() {
        int final = heap.size()-1;
        swap(heap[0], heap[final]);
        posMap[heap[0].second] = 0;
        posMap.erase(heap[final].second);
        heap.pop_back();
        heapify(heap, final, 0);
    }

    // Verifica si el heap está vacío
    bool empty() const {
        return heap.empty();
    }

    // Función para disminuir la clave (distancia) de un nodo específico
    void decreaseKey(int node, double newDist) {
        int index = posMap[node];
        heap[index].first = newDist;
        // Adjust the heap after decrease
        while (index != 0 && heap[parent(index)] > heap[index]) {
            swap(heap[index], heap[parent(index)]);
            posMap[heap[index].second] = index;
            posMap[heap[parent(index)].second] = parent(index);
            index = parent(index);
        }
    }

    void heapify(vector<ii>& array_in, int array_size, int subtree_root_index) {
        // 
        int largest_value = subtree_root_index;
        int left = 2*subtree_root_index + 1;
        int right = 2*subtree_root_index + 2;
            
        if (left < array_size && array_in[left] < array_in[largest_value]){
            largest_value = left;
        }

        if (right < array_size && array_in[right] < array_in[largest_value]){
            largest_value = right;
        }   

        if (largest_value != subtree_root_index ) {
            swap(array_in[subtree_root_index], array_in[largest_value]);
            // Cambiamos los indices en el map para que coincidan con los nuevos indices
            posMap[array_in[subtree_root_index].second] = subtree_root_index;
            posMap[array_in[largest_value].second] = largest_value;
            heapify(array_in, array_size, largest_value);
        }
    }
    
    // Imprime los elementos del heap (para depuración)
    void printHeap() const {
        for (const auto& element : heap) {
            cout << "(" << element.first << ", " << element.second << ") ";
        }
        cout << endl;
    }


private:
    // Helper functions
    int parent(int i) {
        return (i - 1) / 2;
    }

    int left(int i) {
        return 2 * i + 1;
    }

    int right(int i) {
        return 2 * i + 2;
    }
};