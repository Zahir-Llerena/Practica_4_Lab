/*
 =======================================================================================
 PROYECTO: Practica 4 Laboratorio - Motor de Simulación Matemática (Desafío II)
 ESTUDIANTE: Arlington Zahir Llerena Martínez
 OBJETIVO ACADÉMICO: Comprender y aplicar Programación Orientada a Objetos (POO),
                     gestión manual de memoria dinámica (punteros), flujos de archivos (I/O)
                     y la implementación de Modela miento de red por enrutadores.
 =======================================================================================
*/

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits>
#include <algorithm>

// Valor numérico máximo para representar la ausencia de conexión (Infinito)
const int INF = std::numeric_limits<int>::max();

// ============================================================================
// BLOQUE 1: ESTRUCTURA ASOCIADA A ENRUTADOR.H
// ============================================================================
class Enrutador
{
private:
    std::string nombre; // Identificador único
    std::map<std::string, int> enlaces; // Contenedor elegido para enlaces directos y costos

public:
    // Constructores y destructor originales
    Enrutador() : nombre("") {}     // Constructor por defecto
    Enrutador(const std::string& nom) : nombre(nom) {} // Constructor parametrizado
    ~Enrutador() {}                 // Destructor

    // Getters propuestos al inicio
    const std::string &getNombre() const { return nombre; }  // Getter para el nombre
    const std::map<std::string, int> &getEnlaces() const { return enlaces; }   // Getter para los enlaces

    // Métodos de funcionamiento para gestionar los enlaces
    void agregarEnlace(const std::string &destino, int costo) {
        enlaces[destino] = costo;
    }
    
    void eliminarEnlace(const std::string &destino) {
        enlaces.erase(destino);
    }
    
    void actualizarCosto(const std::string &destino, int costo) {
        if (enlaces.find(destino) != enlaces.end()) {
            enlaces[destino] = costo;
        }
    }
};


// ============================================================================
// BLOQUE 2: ESTRUCTURA ASOCIADA A RED.H
// ============================================================================
class Red
{
private:
    std::map<std::string, Enrutador> enrutadores; // Contenedor para enrutadores de la red
    
    // Contenedor para guardar las tablas de enrutamiento de cada enrutador
    std::map<std::string, std::map<std::string, std::pair<int, std::vector<std::string>>>> tablasEnrutamiento;

public:
    // Constructores y destructor originales
    Red() {}  // Constructor por defecto
    ~Red() {} // Destructor

    // Agregar un enrutador a la red
    void agregarEnrutador(const std::string &nombre) {
        if (enrutadores.find(nombre) == enrutadores.end()) {
            enrutadores[nombre] = Enrutador(nombre);
        }
        actualizarTablas(); // Recalcular caminos
    }

    // Remover un enrutador de la red
    void removerEnrutador(const std::string &nombre) {
        if (enrutadores.find(nombre) == enrutadores.end()) return;
        
        // Purgar enlaces que apuntaban al nodo eliminado
        for (auto &par : enrutadores) {
            par.second.eliminarEnlace(nombre);
        }
        enrutadores.erase(nombre);
        actualizarTablas(); // Recalcular caminos
    }

    // Agregar un enlace entre dos enrutadores
    void agregarEnlace(const std::string &origen, const std::string &destino, int costo) {
        if (enrutadores.find(origen) != enrutadores.end() && enrutadores.find(destino) != enrutadores.end()) {
            enrutadores[origen].agregarEnlace(destino, costo);
            enrutadores[destino].agregarEnlace(origen, costo); // Enlaces simétricos
            actualizarTablas(); // Recalcular caminos
        }
    }

    // Eliminar un enlace entre dos enrutadores
    void eliminarEnlace(const std::string &origen, const std::string &destino) {
        if (enrutadores.find(origen) != enrutadores.end() && enrutadores.find(destino) != enrutadores.end()) {
            enrutadores[origen].eliminarEnlace(destino);
            enrutadores[destino].eliminarEnlace(origen);
            actualizarTablas(); // Recalcular caminos
        }
    }

    // Actualizar el costo de un enlace
    void actualizarCosto(const std::string &origen, const std::string &destino, int nuevoCosto) {
        if (enrutadores.find(origen) != enrutadores.end() && enrutadores.find(destino) != enrutadores.end()) {
            enrutadores[origen].actualizarCosto(destino, nuevoCosto);
            enrutadores[destino].actualizarCosto(origen, nuevoCosto);
            actualizarTablas(); // Recalcular caminos
        }
    }

    // Cargar la topología desde un archivo
    void cargarDesdeArchivo(const std::string &nombreArchivo) {
        std::ifstream archivo(nombreArchivo);
        if (!archivo.is_open()) return;

        enrutadores.clear();
        std::string linea;
        
        while (std::getline(archivo, linea)) {
            if (linea.empty() || linea[0] == '#') continue;
            
            std::stringstream ss(linea);
            std::string orig, dest, costStr;
            
            if (std::getline(ss, orig, ',') && std::getline(ss, dest, ',') && std::getline(ss, costStr, ',')) {
                if (enrutadores.find(orig) == enrutadores.end()) enrutadores[orig] = Enrutador(orig);
                if (enrutadores.find(dest) == enrutadores.end()) enrutadores[dest] = Enrutador(dest);
                
                enrutadores[orig].agregarEnlace(dest, std::stoi(costStr));
                enrutadores[dest].agregarEnlace(orig, std::stoi(costStr));
            }
        }
        archivo.close();
        actualizarTablas(); // Recalcular caminos
    }

    // Algoritmo de Dijkstra para calcular rutas y costos
    void dijkstra(const std::string &origen) {
        std::map<std::string, int> dists;
        std::map<std::string, std::string> padres;
        std::map<std::string, bool> visitados;

        for (const auto &p : enrutadores) dists[p.first] = INF;
        dists[origen] = 0;

        // Ejecucion del for
        for (size_t i = 0; i < enrutadores.size(); i++) {
            std::string u = "";
            int min_dist = INF;
            
            for (const auto& p : enrutadores) {
                if (!visitados[p.first] && dists[p.first] < min_dist) {
                    min_dist = dists[p.first];
                    u = p.first;
                }
            }
            if (u.empty() || min_dist == INF) break;
            visitados[u] = true;

            for (const auto &enlace : enrutadores[u].getEnlaces()) {
                std::string v = enlace.first;
                int peso = enlace.second;
                if (!visitados[v] && dists[u] + peso < dists[v]) {
                    dists[v] = dists[u] + peso;
                    padres[v] = u;
                }
            }
        }

        for (const auto &p : enrutadores) {
            std::string dest = p.first;
            if (dists[dest] == INF) {
                tablasEnrutamiento[origen][dest] = {INF, {}};
            } else {
                std::vector<std::string> cam; // Contenedor de la STL
                std::string act = dest;
                while (act != origen) {
                    cam.push_back(act);
                    act = padres[act];
                }
                cam.push_back(origen);
                std::reverse(cam.begin(), cam.end());
                tablasEnrutamiento[origen][dest] = {dists[dest], cam};
            }
        }
    }

    // Actualizar todas las tablas de enrutamiento
    void actualizarTablas() {
        tablasEnrutamiento.clear();
        for (const auto &p : enrutadores) dijkstra(p.first);
    }

    // Obtener el costo entre dos enrutadores
    int obtenerCosto(const std::string &origen, const std::string &destino) {
        if (tablasEnrutamiento.count(origen) && tablasEnrutamiento[origen].count(destino)) {
            return tablasEnrutamiento[origen][destino].first;
        }
        return INF;
    }

    // Obtener el camino eficiente entre dos enrutadores
    std::vector<std::string> obtenerCamino(const std::string &origen, const std::string &destino) {
        if (tablasEnrutamiento.count(origen) && tablasEnrutamiento[origen].count(destino)) {
            return tablasEnrutamiento[origen][destino].second;
        }
        return std::vector<std::string>();
    }

    // Mostrar la tabla de enrutamiento para un enrutador específico
    void mostrarTablaEnrutamiento(const std::string &nombre) {
        if (enrutadores.find(nombre) == enrutadores.end()) {
            std::cout << "El enrutador '" << nombre << "' no existe en la red.\n";
            return;
        }
        std::cout << "\nTABLA DE ENRUTAMIENTO: " << nombre << "\nDestino\tCosto\tCamino Eficiente\n";
        for (const auto &par : tablasEnrutamiento[nombre]) {
            std::cout << par.first << "\t" << (par.second.first == INF ? "INF" : std::to_string(par.second.first)) << "\t";
            // Ejecucion del for
            for (size_t i = 0; i < par.second.second.size(); i++)
                std::cout << par.second.second[i] << (i == par.second.second.size() - 1 ? "" : " -> ");
            std::cout << "\n";
        }
    }
};

