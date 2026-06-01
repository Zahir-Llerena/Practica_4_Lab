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
