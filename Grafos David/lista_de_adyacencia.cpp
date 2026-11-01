#include <iostream>
#include <vector>
#include <string>
#include <utility>

using namespace std;

class GrafoLista {
private:
    //Diccionario para los vértices
    vector<string> dict;

    //Lista de adyacencias: para cada vértice i, almacena pares (destino_x, peso_y)
    vector<vector<pair<int, int>>> adj;

    int n; //Número de vértices (orden del grafo)

public:
    GrafoLista() : n(0) {}

    //Agrega un vértice al grafo registrándolo en el diccionario
    void agregarVertice(string v) {
        dict.push_back(v);
        adj.push_back({}); //Inicializa la lista de adyacentes vacía para este nuevo vértice
        n++;
    }

    //Agrega la arista (u, v) con peso w usando los índices dados
    void agregarArista(int u, int v, int w) {
        if (0 <= u && u < n && 0 <= v && v < n) {
            adj[u].push_back({v, w});
        } else {
            cout << "Error: Índices de vértice fuera de rango" << endl;
            return;
        }
    }

    //Retorna verdadero si existe un arco dirigido u -> v
    bool existeArco(int u, int v) const {
        if (0 <= u && u < n && 0 <= v && v < n) {
            for (const auto& arista : adj[u]) {
                if (arista.first == v) {
                    return true;
                }
            }
        }
        return false;
    }

    //Elimina el vértice por índice y reajusta todas las aristas afectadas
    void eliminarVertice(int id) {
        if (id < 0 || id >= n) {
            cout << "Error: El vértice con id " << id << " no existe" << endl;
            return;
        };

        //Eliminar el vértice del diccionario y de la lista de adyacencias
        dict.erase(dict.begin() + id);
        adj.erase(adj.begin() + id);
        n--;

        //Actualizar las conexiones de los vértices restantes
        for (int i = 0; i < n; ++i) {
            for (auto it = adj[i].begin(); it != adj[i].end();) {
                if (it->first == id) {
                    //Si el arco apuntaba al nodo eliminado, se remueve
                    it = adj[i].erase(it);
                } else {
                    //Si apuntaba a un índice posterior al eliminado, se decrementa
                    if (it->first > id) {
                        it->first--;
                    }
                    ++it;
                }
            }
        }
    }

    //Elimina la arista indicada entre u y v
    void eliminarArista(int u, int v) {
        if (0 <= u && u < n && 0 <= v && v < n) {
            bool encontrado = false;
            for (auto it = adj[u].begin(); it != adj[u].end(); ++it) {
                if (it->first == v) {
                    adj[u].erase(it);
                    encontrado = true;
                    break;
                }
            }
            if (!encontrado) {
                cout << "Error: La arista (" << u << ", " << v << ") no existe" << endl;
                return;
            }
        }
        else {
            cout << "Error: La arista (" << u << ", " << v << ") no existe" << endl;
            return;
        }
    }

    //Retorna los índices de los vértices adyacentes a v
    vector<int> vecinos(int v) const {
        vector<int> resultado;
        if (0 <= v && v < n) {
            for (const auto& arista : adj[v]) {
                resultado.push_back(arista.first);
            }
        }
        return resultado;
    }

    //Retorna el orden del grafo
    int numVertices() const {
        return n;
    }

    //Imprime la representación de la lista
    void mostrar() const {
        cout << "--- Lista de Adyacencia ---" << endl;
        for (int i = 0; i < n; ++i) {
            cout << "[" << i << "] " << dict[i] << " -> ";
            for (const auto& arista : adj[i]) {
                cout << "(" << arista.first << " [" << dict[arista.first] 
                     << "], peso: " << arista.second << ") ";
            }
            cout << endl;
        }
    }
};