#include <iostream>
#include <vector>
#include <string>
#include <limits>

using namespace std;

void fastio(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
}

//Definimos el valor infinito
const int INF = numeric_limits<int>::max();

class GrafoMatriz {
private:
    //Diccionario para los vértices
    vector<string> dict; 
    
    //Matriz de adyacencia nxn que almacena los pesos
    vector<vector<long long>> M; //MODIFICACIÓN DE LA IMPLEMENTACIÓN ORIGINAL: int puede causar desbordamiento
    
    int n; // Número de vértices

public:
    GrafoMatriz() : n(0) {}

    /*MODIFICACIÓN DE LA IMPLEMENTACIÓN ORIGINAL:
      Agregamos este metodo para iniciar la matriz con cierto tamaño y valor por defecto en sus casillas;
      es util para el problema A del lab 2, porque como nada mas recibimos los pesos de los arcos, ya que 
      los vertices estan indexados naturalmente, es innecesario agregar los vertices al diccionario
    */
    void setGrafo(int new_n, int new_empty){
        n = new_n;
        M.assign(new_n, vector<long long>(new_n, new_empty));
    }

    /*MODIFICACIÓN DE LA IMPLEMENTACIÓN ORIGINAL:
      Agregamos este metodo para obtener el peso de un arco existente en la matriz.
    */
    long long obtenerPeso(int u, int v){
        if(existeArco(u,v)){
            return M[u][v];
        }
        return INF;
    }

    //Agrega un vértice guardando su información en el diccionario
    void agregarVertice(string valor_nodo) {
        dict.push_back(valor_nodo);
        n++;
        
        //Redimensionar la matriz 
        for (int i = 0; i < n - 1; ++i) {
            M[i].push_back(INF); //Añadir columna a las filas existentes
        }
        M.push_back(vector<long long>(n, INF)); //Añadir nueva fila completa
    }

    //Agrega la arista (u, v) de peso w
    void agregarArista(int u, int v, long long w) {
        if (0 <= u && u < n && 0 <= v && v < n) {
            M[u][v] = w;
        } else {
            cout << "Error: Índices fuera de rango" << endl;
            return;
        }
    }

    //Retorna verdadero si existe un arco entre los índices u y v
    bool existeArco(int u, int v) const {
        if (0 <= u && u < n && 0 <= v && v < n) {
            return M[u][v] != INF;
        }
        return false;
    }

    //Eliminan el vértice (y sus aristas) por id
    void eliminarVertice(int id) {
        if (0 <= id && id < n){
            //Borramos del diccionario
            dict.erase(dict.begin() + id);
            
            //Borramos fila y columna de la matriz
            M.erase(M.begin() + id);
            for (int i = 0; i < n - 1; ++i) {
                M[i].erase(M[i].begin() + id);
            }
            n--;
        }
        else {
            cout << "Error: El vértice con id " << id << " no existe" << endl;
            return;
        }
        
    }

    //Eliminar la arista indicada por índices
    void eliminarArista(int u, int v) {
        if (0 <= u && u < n && 0 <= v && v < n && M[u][v] != INF) {
            M[u][v] = INF; // Restablecer a infinito
        }
        else {
            cout << "Error: La arista (" << u << ", " << v << ") no existe" << endl;
            return;
        }
    }

    //Retorna la lista de adyacentes del índice v
    vector<int> vecinos(int v) const {
        vector<int> res;
        if (0 <= v && v < n) {
            for (int j = 0; j < n; ++j) {
                if (M[v][j] != INF) {
                    res.push_back(j); //Retornamos los índices de los vecinos
                }
            }
        }
        return res;
    }

    //Retornan el orden
    int numVertices() const {
        return n;
    }

    //Imprime la representación
    void mostrar() const {
        cout << "Matriz de Adyacencias:" << endl;
        cout << "Nodos guardados: ";
        for (int i = 0; i < n; i++){
            cout << "[" << i << "]=" << dict[i] << " ";
        } 
        cout << endl << endl;

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (M[i][j] == INF) cout << "INF\t";
                else cout << M[i][j] << "\t";
            }
            cout << endl;
        }
    }
};