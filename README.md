# Simulador Centralizado de Redes y Enrutamiento Dinámico (C++)

Este repositorio contiene la solución e implementación de la **Práctica 4 de Laboratorio** para la asignatura **Informática II - Laboratorio** de la **Facultad de Ingeniería de la Universidad de Antioquia (UdeA)**.

---

## 🎯 Objetivo de la Actividad
El propósito de este proyecto es aplicar los fundamentos de la **Programación Orientada a Objetos (POO)** avanzada y el uso de los contenedores de la **STL** de C++ para resolver y simular un problema clásico en el área de las telecomunicaciones: el enrutamiento y la conectividad en redes de computadores.

El sistema modela de forma centralizada una red de enrutadores interconectados mediante enlaces con costos métricos variables (que representan factores como distancia, velocidad o retardo). Cada dispositivo es capaz de estructurar y converger de manera óptima su tabla de enrutamiento interna basándose en el cálculo de las mejores trayectorias hacia cualquier destino de la topología.

---

## 🚀 Logros Alcanzados en el Código
Hasta el momento, se ha consolidado un script de trabajo unificado en C++ que integra con éxito las siguientes capacidades técnicas:

* **Modelamiento de Capas (POO):** Diseño formal de las clases `Enrutador` y `Red`, garantizando el encapsulamiento y el aislamiento de la información local frente al control global de la topología.
* **Abstracción mediante la STL:** Uso analítico y eficiente de `std::map` para búsquedas, adiciones y eliminaciones logarítmicas de enlaces/nodos, y de `std::vector` para almacenar de forma contigua la secuencia exacta de los caminos optimizados.
* **Inteligencia Algorítmica:** Integración del **Algoritmo de Dijkstra** para la determinación y el cálculo en tiempo real de los menores costos acumulados y las rutas más cortas entre cualquier par de nodos de la red.
* **Control de Flujos y Archivos:** Implementación de un módulo de análisis sintáctico que configura automáticamente la topología leyendo un archivo externo (`.txt`), omitiendo comentarios (`#`) y procesando registros delimitados por comas (`Origen,Destino,Costo`).
* **Robustez e Interfaz de Usuario:** Diseño de un menú interactivo secuencial en consola que limpia de manera controlada los flujos de entrada (`std::cin`) mediante funciones de vaciado de buffer, blindando la ejecución ante ingresos alfanuméricos inválidos y asegurando una visualización limpia en entornos Unix/macOS (Xcode).

---

## ⚠️ Nota de Estado
> **Aviso de Desarrollo:** Este proyecto se encuentra actualmente en fase activa de auditoría, pruebas cuantitativas y validación de resultados. Por lo tanto, toda la estructura del código, funciones internas y lógica de enrutamiento están **a expensas de modificaciones y mejoras continuas** según se requiera para optimizar el rendimiento del sistema.

---
Developed as part of the Telecommunications Engineering laboratory curriculum.
