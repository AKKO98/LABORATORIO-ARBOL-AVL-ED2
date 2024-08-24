#include <iostream>
#include "AVLTree.hpp"
#include <random>  // Para la generación de números aleatorios
#include <ctime>   // Para usar el tiempo como semilla

int main(int argc, const char* argv[]) {
    AVLTree<int> tree2;
    AVLTreeNode<int>* dummy = new AVLTreeNode<int>;
    std::vector<int> numeros(25);




    std::random_device rd;                         // Obtiene una semilla de un generador de números aleatorios
    std::mt19937 gen(rd());                        // Inicializa el generador con la semilla
    std::uniform_int_distribution<> dis(4, 150);  // Define el rango [min_value, max_value]

    for (int& number : numeros) {
        number = dis(gen);  // Genera un número aleatorio en el rango definido
    }

    for (auto n : numeros)
    {
        tree2.insert(n);
    }
    
    //tree2.display();
    tree2.graph_bfs("Felipe.png");

    //std::cout << tree2.lower_bound(4) << std::endl;
    //std::cout << tree2.find_max() << std::endl;
    //std::cout << tree2.size() << std::endl;
    //std::cout << tree2.empty() << std::endl;
    //tree2.clear();
    //std::cout << tree2.empty() << std::endl;

    return 0;
}