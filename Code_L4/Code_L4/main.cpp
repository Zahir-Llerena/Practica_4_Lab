/*
 =======================================================================================
 PROYECTO: Practica 4 Laboratorio - Motor de Simulación Matemática (Desafío II)
 ESTUDIANTE: Arlington Zahir Llerena
 OBJETIVO ACADÉMICO: Comprender y aplicar Programación Orientada a Objetos (POO),
                     gestión manual de memoria dinámica (punteros), flujos de
                     archivos (I/O) y la implementación de Modela miento de red por
                     enrutadores.
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


const int INF = std::numeric_limits<int>::max();

// ============================================================================
// BLOQUE 1: ESTRUCTURA ASOCIADA A ENRUTADOR.H
// ============================================================================
class Enrutador
{
private:
    std::string nombre;
    std::map<std::string, int> enlaces;

public:
    Enrutador() : nombre("") {}
    Enrutador(const std::string& nom) : nombre(nom) {}
    ~Enrutador() {}

    
    const std::string &getNombre() const { return nombre; }
    const std::map<std::string, int> &getEnlaces() const { return enlaces; }

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
    std::map<std::string, Enrutador> enrutadores;
    
    std::map<std::string, std::map<std::string, std::pair<int, std::vector<std::string>>>> tablasEnrutamiento;

public:
     Red() {}
    ~Red() {}

    void agregarEnrutador(const std::string &nombre) {
        if (enrutadores.find(nombre) == enrutadores.end()) {
            enrutadores[nombre] = Enrutador(nombre);
        }
        actualizarTablas();
    }

    
    void removerEnrutador(const std::string &nombre) {
        if (enrutadores.find(nombre) == enrutadores.end()) return;
        
        
        for (auto &par : enrutadores) {
            par.second.eliminarEnlace(nombre);
        }
        enrutadores.erase(nombre);
        actualizarTablas();
    }

    
    void agregarEnlace(const std::string &origen, const std::string &destino, int costo) {
        if (enrutadores.find(origen) != enrutadores.end() && enrutadores.find(destino) != enrutadores.end()) {
            enrutadores[origen].agregarEnlace(destino, costo);
            enrutadores[destino].agregarEnlace(origen, costo);
            actualizarTablas();
        }
    }

    // Eliminar un enlace entre dos enrutadores
    void eliminarEnlace(const std::string &origen, const std::string &destino) {
        if (enrutadores.find(origen) != enrutadores.end() && enrutadores.find(destino) != enrutadores.end()) {
            enrutadores[origen].eliminarEnlace(destino);
            enrutadores[destino].eliminarEnlace(origen);
            actualizarTablas();
        }
    }

    
    void actualizarCosto(const std::string &origen, const std::string &destino, int nuevoCosto) {
        if (enrutadores.find(origen) != enrutadores.end() && enrutadores.find(destino) != enrutadores.end()) {
            enrutadores[origen].actualizarCosto(destino, nuevoCosto);
            enrutadores[destino].actualizarCosto(origen, nuevoCosto);
            actualizarTablas();
        }
    }

    
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
        actualizarTablas();
    }

    
    void dijkstra(const std::string &origen) {
        std::map<std::string, int> dists;
        std::map<std::string, std::string> padres;
        std::map<std::string, bool> visitados;

        for (const auto &p : enrutadores) dists[p.first] = INF;
        dists[origen] = 0;

        
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
                std::vector<std::string> cam;
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

    
    void actualizarTablas() {
        tablasEnrutamiento.clear();
        for (const auto &p : enrutadores) dijkstra(p.first);
    }

    
    int obtenerCosto(const std::string &origen, const std::string &destino) {
        if (tablasEnrutamiento.count(origen) && tablasEnrutamiento[origen].count(destino)) {
            return tablasEnrutamiento[origen][destino].first;
        }
        return INF;
    }

    
    std::vector<std::string> obtenerCamino(const std::string &origen, const std::string &destino) {
        if (tablasEnrutamiento.count(origen) && tablasEnrutamiento[origen].count(destino)) {
            return tablasEnrutamiento[origen][destino].second;
        }
        return std::vector<std::string>();
    }

    
    void mostrarTablaEnrutamiento(const std::string &nombre) {
        if (enrutadores.find(nombre) == enrutadores.end()) {
            std::cout << "El enrutador '" << nombre << "' no existe en la red.\n";
            return;
        }
        std::cout << "\nTABLA DE ENRUTAMIENTO: " << nombre << "\nDestino\tCosto\tCamino Eficiente\n";
        for (const auto &par : tablasEnrutamiento[nombre]) {
            std::cout << par.first << "\t" << (par.second.first == INF ? "INF" : std::to_string(par.second.first)) << "\t";
            
            for (size_t i = 0; i < par.second.second.size(); i++)
                std::cout << par.second.second[i] << (i == par.second.second.size() - 1 ? "" : " -> ");
            std::cout << "\n";
        }
    }
};


// ============================================================================
// BLOQUE 3: FUNCIONES DE INTERFAZ Y MANEJO DEL BUFFER (MAIN.CPP)
// ============================================================================

void limpiarBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void esperarTecla() {
    std::cout << "\nPresione Enter para continuar...";
    std::cin.get();
#ifdef _WIN32
    std::system("cls"); // Limpia la pantalla en Windows
#else
    std::cout << "\n\n\n\n\n\n\n";
#endif
}

// Invocacion del main
int main() {
    Red red;
    std::string archivoInicial;
    bool archivoCargado = false;

    std::cout << "==================================================\n";
    std::cout << " BIENVENIDO AL SIMULADOR CENTRALIZADO DE REDES   \n";
    std::cout << "==================================================\n";
    
    while (!archivoCargado) {
        std::cout << "Por favor, ingrese la ruta del archivo de topologia (.txt):\n-> ";
        std::getline(std::cin, archivoInicial);
        
        std::ifstream test(archivoInicial);
        if (test.is_open()) {
            test.close();
            red.cargarDesdeArchivo(archivoInicial);
            std::cout << "\n[OK]: Topologia inicializada correctamente.\n";
            archivoCargado = true;
            esperarTecla();
        } else {
            std::cout << "\n[ERROR]: No se pudo abrir el archivo. Verifique la ruta.\n\n";
        }
    }

    int opcion;
    do {
        std::cout << "\n--- MENU SIMULADOR RED DE ENRUTADORES ---\n"
                  << "1. Agregar enrutador\n"
                  << "2. Remover enrutador\n"
                  << "3. Agregar enlace\n"
                  << "4. Eliminar enlace\n"
                  << "5. Actualizar costo de enlace\n"
                  << "6. Cargar topologia desde archivo\n"
                  << "7. Mostrar tabla de enrutamiento\n"
                  << "8. Obtener costo entre dos enrutadores\n"
                  << "9. Obtener camino eficiente entre dos enrutadores\n"
                  << "0. Salir\n"
                  << "Seleccione una opcion: ";

        if (!(std::cin >> opcion)) {
            std::cout << "Error: Ingrese un valor numerico.\n";
            limpiarBuffer();
            esperarTecla();
            opcion = -1;
            continue;
        }
        
        limpiarBuffer();

        std::string nombre, origen, destino, archivo;
        int costo;

        switch (opcion) {
            case 1:
                std::cout << "Nombre del enrutador: ";
                std::getline(std::cin, nombre);
                red.agregarEnrutador(nombre);
                std::cout << "Operacion realizada.\n";
                esperarTecla();
                break;
                
            case 2:
                std::cout << "Nombre del enrutador a remover: ";
                std::getline(std::cin, nombre);
                red.removerEnrutador(nombre);
                std::cout << "Operacion realizada.\n";
                esperarTecla();
                break;
                
            case 3:
                std::cout << "Origen: "; std::getline(std::cin, origen);
                std::cout << "Destino: "; std::getline(std::cin, destino);
                std::cout << "Costo: "; std::cin >> costo;
                red.agregarEnlace(origen, destino, costo);
                std::cout << "Operacion realizada.\n";
                limpiarBuffer();
                esperarTecla();
                break;
                
            case 4:
                std::cout << "Origen: "; std::getline(std::cin, origen);
                std::cout << "Destino: "; std::getline(std::cin, destino);
                red.eliminarEnlace(origen, destino);
                std::cout << "Operacion realizada.\n";
                esperarTecla();
                break;
                
            case 5:
                std::cout << "Origen: "; std::getline(std::cin, origen);
                std::cout << "Destino: "; std::getline(std::cin, destino);
                std::cout << "Nuevo Costo: "; std::cin >> costo;
                red.actualizarCosto(origen, destino, costo);
                std::cout << "Operacion realizada.\n";
                limpiarBuffer();
                esperarTecla();
                break;
                
            case 6:
                std::cout << "Ruta completa (.txt): ";
                std::getline(std::cin, archivo);
                red.cargarDesdeArchivo(archivo);
                std::cout << "Operacion realizada.\n";
                esperarTecla();
                break;
                
            case 7:
                std::cout << "Enrutador a consultar: ";
                std::getline(std::cin, nombre);
                red.mostrarTablaEnrutamiento(nombre);
                esperarTecla();
                break;
                
            case 8:
                std::cout << "Origen: "; std::getline(std::cin, origen);
                std::cout << "Destino: "; std::getline(std::cin, destino);
                costo = red.obtenerCosto(origen, destino);
                std::cout << "Costo: " << (costo == INF ? "INFINITO" : std::to_string(costo)) << "\n";
                esperarTecla();
                break;
                
            case 9:
                std::cout << "Origen: "; std::getline(std::cin, origen);
                std::cout << "Destino: "; std::getline(std::cin, destino);
                {
                    auto camino = red.obtenerCamino(origen, destino);
                    if (camino.empty()) {
                        std::cout << "Ruta no disponible.\n";
                    } else {
                        std::cout << "Camino: ";
                        // Ejecucion del for
                        for (size_t i = 0; i < camino.size(); i++)
                            std::cout << camino[i] << (i == camino.size() - 1 ? "" : " -> ");
                        std::cout << "\nCosto: " << red.obtenerCosto(origen, destino) << "\n";
                    }
                }
                esperarTecla();
                break;
                
            case 0:
                std::cout << "Saliendo...\n";
                break;
                
            default:
                std::cout << "Opcion no valida.\n";
                esperarTecla();
                break;
        }
    } while (opcion != 0);

    return 0;
}



// Hasta el momento se da a conocer el trabajo realizado. Estamos en un proceso de auditoria
// Att: Zahir Llerena

// Ruta de archivo .txt de la topologia de ruta:
// /Users/zahir_llerena/My Drive/UdeA/Semestre_3/Informatica_II/Laboratorio/Practica_4_Lab/Code_L4/red_enrutadores.txt

