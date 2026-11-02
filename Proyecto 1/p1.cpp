#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <unordered_map>
#include <fstream>
#include <queue>
#include <limits>
#include <set>

using namespace std;

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

    /*MODIFICACIÓN DE LA IMPLEMENTACIÓN ORIGINAL:
      Agregamos este método para obtener la referencia de la matriz del grafo y operar directamente sobre ella en
      la clase RedTransporte
    */
    vector<vector<long long>>& obtenerMatrizReferencia() {
        return M;
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
            cout << "Error: Indices fuera de rango" << "\n";
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
            cout << "Error: El vertice con id " << id << " no existe" << "\n";
            return;
        }
        
    }

    //Eliminar la arista indicada por índices
    void eliminarArista(int u, int v) {
        if (0 <= u && u < n && 0 <= v && v < n && M[u][v] != INF) {
            M[u][v] = INF; // Restablecer a infinito
        }
        else {
            cout << "Error: La arista (" << u << ", " << v << ") no existe" << "\n";
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
        cout << "Matriz de Adyacencias:" << "\n";
        cout << "Nodos guardados: ";
        for (int i = 0; i < n; i++){
            cout << "[" << i << "]=" << dict[i] << " ";
        } 
        cout << "\n" << "\n";

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (M[i][j] == INF) cout << "INF\t";
                else cout << M[i][j] << "\t";
            }
            cout << "\n";
        }
    }
};

class GrafoLista {
private:
    //Diccionario para los vértices
    vector<string> dict;

    //Lista de adyacencias: para cada vértice i, almacena pares (destino_x, peso_y)
    vector<vector<pair<int, int>>> adj;

    int n; //Número de vértices (orden del grafo)

public:
    GrafoLista() : n(0) {}
    
    /*MODIFICACIÓN DE LA IMPLEMENTACIÓN ORIGINAL:
      Agregamos este método para obtener la referencia de la lista de adyacencia y operar en la clase RedTransporte
      más rápido
    */
    const vector<vector<pair<int, int>>>& obtenerListaReferencia() const {
        return adj;
    }

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
            cout << "Error: Indices de vertice fuera de rango" << "\n";
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
            cout << "Error: El vertice con id " << id << " no existe" << "\n";
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
                cout << "Error: La arista (" << u << ", " << v << ") no existe" << "\n";
                return;
            }
        }
        else {
            cout << "Error: La arista (" << u << ", " << v << ") no existe" << "\n";
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
        cout << "--- Lista de Adyacencia ---" << "\n";
        for (int i = 0; i < n; ++i) {
            cout << "[" << i << "] " << dict[i] << " -> ";
            for (const auto& arista : adj[i]) {
                cout << "(" << arista.first << " [" << dict[arista.first] 
                     << "], peso: " << arista.second << ") ";
            }
            cout << "\n";
        }
    }
};

class RedTransporte {
private:
    GrafoMatriz matriz;
    GrafoLista lista;
    
    //Mapeo String a ID
    unordered_map<string, int>  nombre_a_id;
    vector<string> id_a_nombre;
    
    vector<vector<int>> siguiente;
    vector<int> grado_in;
    vector<int> grado_out;
    int n, m;
    vector<vector<long long>> matriz_original;
    

public:
    void guardarMatrizOriginal() {
        vector<vector<long long>>& M = matriz.obtenerMatrizReferencia();
        for (int i = 0; i < n; i++) {
            M[i][i] = 0;
        }
        matriz_original = M;
    }
    
    RedTransporte(int num_estaciones, int num_tramos) {
        n = num_estaciones;
        m = num_tramos;
        
        grado_in.assign(n, 0);
        grado_out.assign(n, 0);
        siguiente.assign(n, vector<int>(n, -1)); // -1 significa que no hay camino conocido
                
    }

    bool existeEstacion(const string& nombre) const {
        return nombre_a_id.find(nombre) != nombre_a_id.end();
    }

    int calcularCantidadZonas() {
        vector<bool> asignado(n, false);
        int cantidad_zonas = 0;
        const vector<vector<long long>>& dist = matriz.obtenerMatrizReferencia();

        for (int i = 0; i < n; i++) {
            if (!asignado[i]) {
                cantidad_zonas++;
                asignado[i] = true;

                for (int j = i + 1; j < n; j++) {
                    if (!asignado[j] && dist[i][j] != INF && dist[j][i] != INF) {
                        asignado[j] = true;
                    }
                }
            }
        }
        return cantidad_zonas;
    }

    void registrarEstacion(const string& nombre) {
        int id = id_a_nombre.size();
        nombre_a_id[nombre] = id;
        id_a_nombre.push_back(nombre);
        
        matriz.agregarVertice(nombre);
        lista.agregarVertice(nombre);
    }

    void registrarTramo(const string& origen, const string& destino, long long tiempo) {
        int u = nombre_a_id[origen];
        int v = nombre_a_id[destino];

        matriz.agregarArista(u, v, tiempo);
        lista.agregarArista(u, v, tiempo);

        //Inicializar matriz de siguientes
        siguiente[u][v] = v;

        //Contadores para la consulta RESUMEN
        grado_out[u]++;
        grado_in[v]++;
    }

    void ejecutarFloydWarshall() {
        vector<vector<long long>>& dist = matriz.obtenerMatrizReferencia();

        for (int k = 0; k < n; k++) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if (dist[i][k] != INF && dist[k][j] != INF) {
                        
                        long long new_time = dist[i][k] + dist[k][j];
                        
                        if (new_time < dist[i][j]) {
                            dist[i][j] = new_time;
                            
                            //Actualizar matriz siguiente
                            siguiente[i][j] = siguiente[i][k]; 
                        }
                    }
                }
            }
        }
    }

    void consultaResumen() {
        //Encontrar la estación con mayor grado de salida
        int max_out = -1;
        int id_max_out = -1;
        for (int i = 0; i < n; i++) {
            if (grado_out[i] > max_out) {
                max_out = grado_out[i];
                id_max_out = i;
            }
        }

        //Encontrar la estación con mayor grado de entrada
        int max_in = -1;
        int id_max_in = -1;
        for (int i = 0; i < n; i++) {
            if (grado_in[i] > max_in) {
                max_in = grado_in[i];
                id_max_in = i;
            }
        }

        int zonas = calcularCantidadZonas();

        cout << "Estaciones: " << n << " Tramos: " << m << " Zonas: " << zonas << "\n";
        
        if (id_max_out != -1) {
            cout << "Mayor grado de salida: " << id_a_nombre[id_max_out] << " (" << max_out << ")\n";
        }
        
        if (id_max_in != -1) {
            cout << "Mayor grado de entrada: " << id_a_nombre[id_max_in] << " (" << max_in << ")\n";
        }
    }

    void consultaRuta(const string& origen, const string& destino) {
        int u = nombre_a_id[origen];
        int v = nombre_a_id[destino];
        
        long long t = matriz.obtenerMatrizReferencia()[u][v];

        if (u == v) {
            cout << origen << " (tiempo: 0 min)\n";
            return;
        }

        if (t == INF) {
            cout << "NO HAY RUTA\n";
            return;
        }

        int actual = u;
        cout << id_a_nombre[actual];
        
        while (actual != v) {
            actual = siguiente[actual][v];
            cout << " -> " << id_a_nombre[actual];
        }
        
        cout << " (tiempo: " << t << " min)\n";
    }

    void consultaSaltos(const string& origen, const string& destino) {
        int u = nombre_a_id[origen];
        int v = nombre_a_id[destino];

        if (u == v) {
            cout << origen << " (0 tramos)\n";
            return;
        }

        const vector<vector<pair<int, int>>>& adj = lista.obtenerListaReferencia();

        vector<bool> visitado(n, false);
        vector<int> pred(n, -1);
        vector<int> saltos(n, 0);
        queue<int> Q;

        Q.push(u);
        visitado[u] = true;

        bool encontrado = false;

        while (!Q.empty() && !encontrado) {
            int actual = Q.front();
            Q.pop();

            for (const pair<int,int>& arista : adj[actual]) {
                int vecino = arista.first;

                if (!visitado[vecino]) {
                    visitado[vecino] = true;
                    pred[vecino] = actual;
                    saltos[vecino] = saltos[actual] + 1;
                    
                    if (vecino == v) {
                        encontrado = true;
                        break;
                    }
                    Q.push(vecino);
                }
            }
        }

        if (!encontrado) {
            cout << "NO HAY RUTA\n";
            return;
        }

        //Reconstrucción de la ruta (de destino a origen)
        vector<int> camino;
        int temp = v;
        while (temp != -1) {
            camino.push_back(temp);
            temp = pred[temp];
        }

        //Imprimir la ruta al derecho
        for (int i = camino.size() - 1; i >= 0; i--) {
            cout << id_a_nombre[camino[i]];
            if (i > 0) cout << " -> ";
        }

        // Imprimir la gramática correcta para la cantidad de tramos
        int total_tramos = saltos[v];
        if (total_tramos == 1) {
            cout << " (1 tramo)\n";
        } else {
            cout << " (" << total_tramos << " tramos)\n";
        }
    }

    void consultaAlcanzables(const string& origen) {
        int u = nombre_a_id[origen];
        const auto& adj = lista.obtenerListaReferencia();
        
        vector<bool> visitado(n, false);
        queue<int> q;
        
        q.push(u);
        visitado[u] = true;
        
        set<string> alcanzables; // set mantiene el orden alfabético automáticamente
        
        while (!q.empty()) {
            int actual = q.front();
            q.pop();
            
            for (const auto& arista : adj[actual]) {
                int vecino = arista.first;
                if (!visitado[vecino]) {
                    visitado[vecino] = true;
                    q.push(vecino);
                    alcanzables.insert(id_a_nombre[vecino]);
                }
            }
        }
        
        cout << alcanzables.size() << ":";
        for (const string& est : alcanzables) {
            cout << " " << est;
        }
        cout << "\n";
        
        
    }

    void consultaZonas() {vector<bool> visitado(n, false);
        // Usamos un set de sets para mantener orden alfabético de estaciones y de zonas (por su primer elemento)
        set<set<string>> zonas;
        const vector<vector<long long>>& dist = matriz.obtenerMatrizReferencia();

        for (int i = 0; i < n; i++) {
            if (!visitado[i]) {
                set<string> zona_actual;
                zona_actual.insert(id_a_nombre[i]);
                visitado[i] = true;

                // Dos estaciones i y j están en la misma zona si hay camino mutuo
                for (int j = 0; j < n; j++) {
                    if (i != j && !visitado[j]) {
                        if (dist[i][j] != INF && dist[j][i] != INF) {
                            visitado[j] = true;
                            zona_actual.insert(id_a_nombre[j]);
                        }
                    }
                }
                zonas.insert(zona_actual);
            }
        }

        cout << zonas.size() << " zonas\n";
        int indice_zona = 1;
        for (const auto& zona : zonas) {
            cout << "Zona " << indice_zona++ << ":";
            for (const string& est : zona) {
                cout << " " << est;
            }
            cout << "\n";
        }
    }

    void consultaDiametro() {
        
        const vector<vector<long long>>& dist = matriz.obtenerMatrizReferencia();
        long long max_tiempo = -1;
        int best_u = -1, best_v = -1;

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i != j && dist[i][j] != INF) {
                    if (dist[i][j] > max_tiempo) {
                        max_tiempo = dist[i][j];
                        best_u = i;
                        best_v = j;
                    } else if (dist[i][j] == max_tiempo) {
                        // En caso de empate, priorizar menor índice de A y luego de B
                        if (best_u == -1 || i < best_u || (i == best_u && j < best_v)) {
                            best_u = i;
                            best_v = j;
                        }
                    }
                }
            }
        }

        if (best_u == -1 || max_tiempo == -1) {
            cout << "NO HAY RUTAS\n";
        } else {
            cout <<"Mayor tiempo minimo: " << id_a_nombre[best_u] << " -> " << id_a_nombre[best_v] << " (" << max_tiempo << " min)\n";
        }
    }
    
    void consultaCerrar(const string& estacion) {
        if (!existeEstacion(estacion)) {
            return;
        }
        int id = nombre_a_id[estacion];
        vector<vector<long long>> dist_viejo = matriz.obtenerMatrizReferencia();

        // Para recalcular sin la estación X, debemos partir de la MATRIZ ORIGINAL 
        // (pesos de los tramos directos), NO de la matriz de caminos mínimos actual.
        vector<vector<long long>> dist_nuevo = matriz_original;

        // Anular filas y columnas de la estación cerrada en la matriz temporal de tramos directos
        for (int i = 0; i < n; i++) {
            dist_nuevo[id][i] = INF;
            dist_nuevo[i][id] = INF;
        }
        dist_nuevo[id][id] = 0;

        // Recalcular caminos mínimos desde cero sobre la matriz temporal usando Floyd-Warshall
        for (int k = 0; k < n; k++) {
            if (k == id) continue; // Omitir la estación cerrada como nodo intermediario
            for (int i = 0; i < n; i++) {
                if (i == id) continue;
                for (int j = 0; j < n; j++) {
                    if (j == id) continue;
                    if (dist_nuevo[i][k] != INF && dist_nuevo[k][j] != INF) {
                        long long new_time = dist_nuevo[i][k] + dist_nuevo[k][j];
                        if (new_time < dist_nuevo[i][j]) {
                            dist_nuevo[i][j] = new_time;
                        }
                    }
                }
            }
        }

        int p = 0, q = 0;

        // 4. Contar p y q comparando la matriz real anterior (dist_viejo) con la nueva simulada (dist_nuevo)
        for (int i = 0; i < n; i++) {
            if (i == id) continue;
            for (int j = 0; j < n; j++) {
                if (j == id || i == j) continue;

                bool era_alcanzable = (dist_viejo[i][j] != INF);
                bool es_alcanzable = (dist_nuevo[i][j] != INF);

                // p = los que eran alcanzables y dejan de serlo
                if (era_alcanzable && !es_alcanzable) {
                    p++;
                }
                // q = los que siguen alcanzables pero con tiempo estrictamente mayor
                else if (era_alcanzable && es_alcanzable) {
                    if (dist_nuevo[i][j] > dist_viejo[i][j]) {
                        q++;
                    }
                }
            }
        }

        // Imprimir el resultado esperado
        cout << "Al cerrar " << estacion << ": " << p << " pares pierden conexion, " << q << " pares aumentan su tiempo\n";
    }
};



int main(int argc, char* argv[]) {
    //Verificar argumentos de consola
    if (argc != 3) {
        cout << "Uso: ./p1 <red.txt> <consultas.txt>" << "\n";
        return 1;
    }

    string archivoRed = argv[1];
    string archivoConsultas = argv[2];

    ifstream archivoEntrada(archivoRed);
    if (!archivoEntrada.is_open()) {
        cout << "Error al abrir el archivo de red: " << archivoRed << "\n";
        return 1;
    }

    int n, m;
    archivoEntrada >> n >> m;

    RedTransporte red(n, m);

    //Leer n estaciones
    string nombreEstacion;
    for (int i = 0; i < n; i++) {
        archivoEntrada >> nombreEstacion;
        red.registrarEstacion(nombreEstacion);
    }

    //Leer m tramos
    string origen, destino;
    long long tiempo;
    for (int i = 0; i < m; i++) {
        archivoEntrada >> origen >> destino >> tiempo;
        red.registrarTramo(origen, destino, tiempo);
    }
    archivoEntrada.close();

    red.guardarMatrizOriginal();
    //Calculo caminos mínimos
    red.ejecutarFloydWarshall();

    //Procesar el archivo de consultas
    ifstream archivoComandos(archivoConsultas);
    if (!archivoComandos.is_open()) {
        cout << "Error al abrir el archivo de consultas: " << archivoConsultas << "\n";
        return 1;
    }

    string comando;
    while (archivoComandos >> comando) {
        cout << "> " << comando;

        if (comando == "RESUMEN") {
            cout << "\n";
            red.consultaResumen();
        } 
        else if (comando == "ZONAS") {
            cout << "\n";
            red.consultaZonas();
        }
        else if (comando == "DIAMETRO") {
            cout << "\n";
            red.consultaDiametro();
        }
        else if (comando == "ALCANZABLES") {
            string estA;
            archivoComandos >> estA;
            cout << " " << estA << "\n";
            
            if (!red.existeEstacion(estA)) {
                cout << "ERROR: estacion inexistente: " << estA << "\n";
                continue;
            }
            
            red.consultaAlcanzables(estA);
        }
        else if (comando == "CERRAR") {
            string estA;
            archivoComandos >> estA;
            cout << " " << estA << "\n";
            
            if (!red.existeEstacion(estA)) {
                cout << "ERROR: estacion inexistente: " << estA << "\n";
                continue;
            }
            
            red.consultaCerrar(estA);
        }

        else if (comando == "RUTA") {
            string estA, estB;
            archivoComandos >> estA >> estB;
            cout << " " << estA << " " << estB << "\n";

            bool existeA = red.existeEstacion(estA);
            bool existeB = red.existeEstacion(estB);
            
            if (!existeA) {
                cout << "ERROR: estacion inexistente: " << estA << "\n";
                continue;
            }
            else if (!existeB) {
                cout << "ERROR: estacion inexistente: " << estB << "\n";
                continue;
            }

            red.consultaRuta(estA, estB);
        }
        else if (comando == "SALTOS") {
            string estA, estB;
            archivoComandos >> estA >> estB;
            cout << " " << estA << " " << estB << "\n";

            bool existeA = red.existeEstacion(estA);
            bool existeB = red.existeEstacion(estB);
            
            if (!existeA) {
                cout << "ERROR: estacion inexistente: " << estA << "\n";
                continue;
            }
            else if (!existeB) {
                cout << "ERROR: estacion inexistente: " << estB << "\n";
                continue;
            }

            red.consultaSaltos(estA, estB);
        }
    }
    archivoComandos.close();

    return 0;
}