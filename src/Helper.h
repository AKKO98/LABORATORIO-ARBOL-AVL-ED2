#pragma once
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <filesystem>
#include <memory>
#include "Pelicula.h"

struct Helper
{
    static std::unique_ptr<Pelicula> buscarPeliculaPorTitulo( const std::string& path, const std::string& tituloBuscado) {
        std::ifstream archivo(path);

        if (!archivo.is_open()) {
            std::cerr << "No se pudo abrir el archivo: " << path << std::endl;
            return nullptr;
        }

        std::string linea;
        std::getline(archivo, linea);  // Leer la primera línea (encabezados) y descartarla

        // Convertir el título buscado a minúsculas
        std::string tituloBuscadoLower = tituloBuscado;
        std::transform(tituloBuscadoLower.begin(), tituloBuscadoLower.end(), tituloBuscadoLower.begin(), ::tolower);

        while (std::getline(archivo, linea)) {
            std::istringstream stream(linea);
            std::string title;
            std::string worldwideEarningsStr, domesticEarningsStr, domesticPercentEarningsStr, foreignEarningsStr, foreignPercentEarningsStr, yearStr;

            std::getline(stream, title, ',');
            std::getline(stream, worldwideEarningsStr, ',');
            std::getline(stream, domesticEarningsStr, ',');
            std::getline(stream, domesticPercentEarningsStr, ',');
            std::getline(stream, foreignEarningsStr, ',');
            std::getline(stream, foreignPercentEarningsStr, ',');
            std::getline(stream, yearStr, ',');

            // Convertir el título del CSV a minúsculas
            std::string titleLower = title;
            std::transform(titleLower.begin(), titleLower.end(), titleLower.begin(), ::tolower);

            if (titleLower == tituloBuscadoLower) {
                double WWE = std::stod(worldwideEarningsStr);
                double DE = std::stod(domesticEarningsStr);
                double FE = std::stod(foreignEarningsStr);
                double DPE = std::stod(domesticPercentEarningsStr);
                double FPE = std::stod(foreignPercentEarningsStr);
                int year = std::stoi(yearStr);

                archivo.close();
                return std::make_unique<Pelicula>(title, year, WWE, DE, FE, DPE, FPE);
            }
        }

        archivo.close();
        return nullptr; // No se encontró la película
    }

    static std::vector<Pelicula> csvPeliculasLoader(std::string& path) {
        std::vector<Pelicula> peliculas;
        std::ifstream archivo(path);

        if (!archivo.is_open()) {
            std::cerr << "No se pudo abrir el archivo: " << path << std::endl;
            return peliculas;
        }

        std::string linea;
        // Leer la primera línea (encabezados) y descartarla
        std::getline(archivo, linea);

        while (std::getline(archivo, linea)) {
            std::istringstream stream(linea);
            std::string title;
            std::string worldwideEarningsStr, domesticEarningsStr, domesticPercentEarningsStr, foreignEarningsStr, foreignPercentEarningsStr, yearStr;

            std::getline(stream, title, ',');
            std::getline(stream, worldwideEarningsStr, ',');
            std::getline(stream, domesticEarningsStr, ',');
            std::getline(stream, domesticPercentEarningsStr, ',');
            std::getline(stream, foreignEarningsStr, ',');
            std::getline(stream, foreignPercentEarningsStr, ',');
            std::getline(stream, yearStr, ',');

            double WWE = std::stod(worldwideEarningsStr);
            double DE = std::stod(domesticEarningsStr);
            double FE = std::stod(foreignEarningsStr);
            double DPE = std::stod(domesticPercentEarningsStr);
            double FPE = std::stod(foreignPercentEarningsStr);
            int year = std::stoi(yearStr);

            Pelicula pelicula(title, year, WWE, DE, FE, DPE, FPE);
            peliculas.push_back(pelicula);
        }

        archivo.close();
        return peliculas;
	}

    static AVLTreeNode<Pelicula>* findParentByTitle(AVLTree<Pelicula>& arbol, std::string title) {
        AVLTreeNode<Pelicula>* current = arbol.getRoot();
        AVLTreeNode<Pelicula>* parent = nullptr;


        while (current != nullptr) {
            if (title < current->value.getTitle()) {
                parent = current;
                current = current->left;
            }
            else if (title > current->value.getTitle()) {
                parent = current;
                current = current->right;
            }
            else {
                return parent;  // Se ha encontrado el nodo, se retorna el padre
            }
        }

        return nullptr; // Nodo no encontrado en el árbol o es la raíz (no tiene padre)
    }

    static AVLTreeNode<Pelicula>* findGrandParentByTitle(AVLTree<Pelicula>& arbol, std::string title) {
        AVLTreeNode<Pelicula>* parent = findParentByTitle(arbol, title);

        if (parent != nullptr)
        {
            return findParentByTitle(arbol, parent->value.getTitle());
        }

        return nullptr; // Nodo no encontrado en el árbol o es la raíz (no tiene padre)
    }

    static AVLTreeNode<Pelicula>* findUncleByTitle(AVLTree<Pelicula>& arbol, std::string title) {
        AVLTreeNode<Pelicula>* parent = findParentByTitle(arbol, title);
        AVLTreeNode<Pelicula>* grandParent = findGrandParentByTitle(arbol, title);

        if (grandParent == nullptr) 
            return nullptr; // No tiene abuelo, entonces no tiene tío

        if (grandParent->left == parent) {
            return grandParent->right;
        }
        else if (grandParent->right == parent) {
            return grandParent->left;
        }

        return nullptr; // Nodo no encontrado en el árbol o es la raíz (no tiene padre)
    }

    // Función para buscar y almacenar los nodos que cumplen con los criterios
    static std::vector<Pelicula> searchWithCriteria(AVLTree<Pelicula>& arbol, int year, double foreign_earnings) {
        std::vector<Pelicula> result;
        searchWithCriteria(arbol.getRoot(), year, foreign_earnings, result);
        return result;
    }

    static int getLevelByName(AVLTree<Pelicula>& arbol, std::string name) {
        int level = 0;
        AVLTreeNode<Pelicula>* current = arbol.getRoot();

        while (current != nullptr) {
            if (name < current->value.getTitle()) {
                current = current->left;
            }
            else if (name > current->value.getTitle()) {
                current = current->right;
            }
            else {
                return level;
            }
            level++;
        }

        return -1; // Nodo no encontrado en el árbol
    }

    static AVLTreeNode<Pelicula>* getNodeByName(AVLTree<Pelicula>& arbol, std::string name) {
        AVLTreeNode<Pelicula>* current = arbol.getRoot();

        while (current != nullptr) {
            if (name < current->value.getTitle()) {
                current = current->left;
            }
            else if (name > current->value.getTitle()) {
                current = current->right;
            }
            else {
                return current;
            }
            
        }

        return nullptr; // Nodo no encontrado en el árbol
    }


    static int getBalanceByName(AVLTree<Pelicula>& arbol, std::string name) {
        int balance = 0;
        AVLTreeNode<Pelicula>* current = arbol.getRoot();

        while (current != nullptr) {
            if (current->value.getTitle() == name)
            {
                return current->balanceFactor();
            }
            if (name < current->value.getTitle()) {
                current = current->left;
            }
            else if (name > current->value.getTitle()) {
                current = current->right;
            }
        }

        return -1; // Nodo no encontrado en el árbol
    }


private:

    static void searchWithCriteria(AVLTreeNode<Pelicula>* node, int year, double foreign_earnings, std::vector<Pelicula>& result) {
        if (node == nullptr) {
            return;
        }

        // Chequear si el nodo cumple con los criterios
        if (node->value.getYear() == year &&
            node->value.getDPE() < node->value.getFPE() &&
            node->value.getFE() >= foreign_earnings) {
            result.push_back(node->value);
        }

        // Buscar en el subárbol izquierdo y derecho
        searchWithCriteria(node->left, year, foreign_earnings, result);
        searchWithCriteria(node->right, year, foreign_earnings, result);
    }




};