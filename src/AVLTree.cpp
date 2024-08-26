#include "AVLTree.hpp"
#define GVDLL
#include "graphviz/cgraph.h"
#include "graphviz/gvc.h"


template <class T>
AVLTree<T>::AVLTree(){
    root = nullptr;
    _size = 0;
}

template <class T>
AVLTree<T>::~AVLTree(){
    clear();
}

template <class T>
void AVLTree<T>::clear(){
    std::vector<AVLTreeNode<T>*> stack;
    
    if (root != nullptr)
        stack.push_back(root);
    
    while (!stack.empty()){
        AVLTreeNode<T> *node = stack.back();
        stack.pop_back();
        
        if (node->left != nullptr)
            stack.push_back(node->left);
        
        if (node->right != nullptr)
            stack.push_back(node->right);
        
        _size--;
        delete node;
    }
    
    root = nullptr;
}

template <class T>
void AVLTree<T>::insert(T& value){
    if (find(value) != -1)
    {
        return;
    }

    AVLTreeNode<T> **indirect = &root;  // Para generalizar la insercion con el nodo raiz
    std::vector<AVLTreeNode<T>**> path;  // Para actualizar la altura de los nodos
    
    while (*indirect != nullptr){
        path.push_back(indirect);
        
        if ((*indirect)->value < value)
            indirect = &((*indirect)->right);
        else if ((*indirect)->value > value)
            indirect = &((*indirect)->left);
    }
    
    *indirect = new AVLTreeNode<T>(value);
    path.push_back(indirect);
    
    balance(path);
    _size++;
}

template <class T>
void AVLTree<T>::erase(T value){
    AVLTreeNode<T> **indirect = &root;  // Para generalizar la insercion con el nodo raiz
    std::vector<AVLTreeNode<T>**> path;  // Para actualizar la altura de los nodos
    
    while (*indirect != nullptr and (*indirect)->value != value){
        path.push_back(indirect);
        
        if ((*indirect)->value > value)
            indirect = &((*indirect)->left);
        else
            indirect = &((*indirect)->right);
    }
    
    if (*indirect == nullptr) {  // El valor no existe en el sub arbol
        std::cout << "El nodo que trata de eliminar no existe" << std::endl;
        return;
    }
    else
        path.push_back(indirect);
    
    std::size_t index = path.size();
    
    if ((*indirect)->left == nullptr and (*indirect)->right == nullptr){  // El nodo es una hoja
        delete *indirect;  // Solo lo borra
        *indirect = nullptr;
        path.pop_back();  // pop al nodo borrado del Path
    }
    
    else if ((*indirect)->right == nullptr){  // Solo existe el hijo izquierdo del nodo
        AVLTreeNode<T> *toRemove = *indirect;
        
        (*indirect) = (*indirect)->left;
        delete toRemove;
        
        path.pop_back();
    }
    
    else{  // Existe el hijo derecho del nodo
        AVLTreeNode<T> **successor = &((*indirect)->right);
        
        while ((*successor)->left != nullptr){
            path.push_back(successor);
            successor = &((*successor)->left);
        }
        
        if (*successor == (*indirect)->right){
            (*successor)->left = (*indirect)->left;
            
            AVLTreeNode<T> *toRemove = *indirect;
            *indirect = *successor;
            delete toRemove;
        }
        
        else{
            AVLTreeNode<T> *tmp = *path.back(), *suc = *successor;
            
            tmp->left = (*successor)->right;
            suc->left = (*indirect)->left;
            suc->right = (*indirect)->right;
            
            delete *indirect;
            *indirect = suc;
            path[index] = &(suc->right);
        }
    }
    
    balance(path);
    _size--;
}

template <class T>
void AVLTree<T>::balance(std::vector<AVLTreeNode<T> **> path){  // Comenzando por la raiz
    std::reverse(path.begin(), path.end());
    
    for (auto indirect: path){
        (*indirect)->updateValues();
        
        //rotación simple a la derecha
        if ((*indirect)->balanceFactor() >= 2 and (*indirect)->left->balanceFactor() >= 0)   // left - left
            *indirect = (*indirect)->right_rotate();
        
        //rotación doble a la derecha
        else if ((*indirect)->balanceFactor() >= 2){  // left - right
            (*indirect)->left = (*indirect)->left->left_rotate();
            *indirect = (*indirect)->right_rotate();
        }

        //rotación simple a la izquierda
        else if ((*indirect)->balanceFactor() <= -2 and (*indirect)->right->balanceFactor() <= 0)  // right - right
            *indirect = (*indirect)->left_rotate();
        

        //rotación doble a la izquierda
        else if ((*indirect)->balanceFactor() <= -2){  // right - left
            (*indirect)->right = ((*indirect)->right)->right_rotate();
            *indirect = (*indirect)->left_rotate();
        }
    }
}



template <class T>
bool AVLTree<T>::empty() const{
    return _size == 0;
}

template <class T>
int AVLTree<T>::size() const{
    return _size;
}

template <class T>
int AVLTree<T>::find(T value) const{
    AVLTreeNode<T> *direct = root;
    int idx = 0;
    
    while (direct != nullptr and direct->value != value){
        if (direct->value > value)
            direct = direct->left;
        else{
            idx += (direct->left ? direct->left->count : 0) + 1;
            direct = direct->right;
        }
    }
    
    if (direct == nullptr)
        return -1;
    
    else
        return idx + (direct->left ? direct->left->count : 0);
}

// encuentra el primer elemento que es mayor que value
template <class T>
int AVLTree<T>::upper_bound(T value) const{
    AVLTreeNode<T> *direct = root;
    int idx = 0;
    
    while (direct != nullptr){
        if (direct->value > value)
            direct = direct->left;
        else{
            idx += (direct->left ? direct->left->count : 0) + 1;
            direct = direct->right;
        }
    }
    
    return idx;
}

// encuentra el primer elemento que es mayor que value
template <class T>
int AVLTree<T>::lower_bound(T value) const{
    AVLTreeNode<T> *direct = root;
    int idx = 0;
    
    while (direct != nullptr){
        if (direct->value >= value)
            direct = direct->left;
        else{
            idx += (direct->left ? direct->left->count : 0) + 1;
            direct = direct->right;
        }
    }
    
    return idx;
}

// encuentra y devuelve el valor mínimo en el árbol
template <class T>
const T& AVLTree<T>::find_min() const{
    AVLTreeNode<T> *cur = root;
    
    while (cur->left != nullptr)
        cur = cur->left;
    
    return cur->value;
}

// encuentra y devuelve el valor maximo en el árbol.
template <class T>
const T& AVLTree<T>::find_max() const{
    AVLTreeNode<T> *cur = root;
    
    while (cur->right != nullptr)
        cur = cur->right;
    
    return cur->value;
}

template <class T>
AVLTreeNode<T>* AVLTree<T>::operator[](std::size_t idx) {
    AVLTreeNode<T>* cur = root;
    int left = cur->left != nullptr ? cur->left->count : 0;

    while (left != idx) {
        if (left < idx) {
            idx -= left + 1;

            cur = cur->right;
            left = cur->left != nullptr ? cur->left->count : 0;
        }

        else {
            cur = cur->left;
            left = cur->left != nullptr ? cur->left->count : 0;
        }
    }

    return cur;
}


template <class T>
void AVLTree<T>::display(const std::string& filename) {
    // Obtener el vector BFS del árbol
    std::vector<AVLTreeNode<T>*> bfs_seq = bfs();

    if (bfs_seq.empty()) {
        std::cout << "El árbol está vacío." << std::endl;
        return;
    }

    // Crear un contexto de Graphviz
    GVC_t* gvc = gvContext();

    // Crear un grafo dirigido
    Agraph_t* g = agopen(_strdup("AVLTree"), Agdirected, nullptr);

    // Crear nodos en el grafo para cada nodo en el BFS
    std::map<AVLTreeNode<T>*, Agnode_t*> node_map;
    for (AVLTreeNode<T>* node : bfs_seq) {
        std::string label = node->to_string();
        node_map[node] = agnode(g, _strdup(label.c_str()), 1);
    }

    // Crear aristas en el grafo para representar las conexiones entre los nodos
    for (AVLTreeNode<T>* node : bfs_seq) {
        if (node->left != nullptr) {
            agedge(g, node_map[node], node_map[node->left], nullptr, 1);
        }
        if (node->right != nullptr) {
            agedge(g, node_map[node], node_map[node->right], nullptr, 1);
        }
    }

    // Realizar el layout y renderizar el grafo en formato PNG
    gvLayout(gvc, g, "dot");
    gvRenderFilename(gvc, g, "png", filename.c_str());

    // Liberar recursos
    gvFreeLayout(gvc, g);
    agclose(g);
    gvFreeContext(gvc);
}

template <class T>
int AVLTree<T>::getLevel(AVLTreeNode<T>* node) {
    int level = 0;
    AVLTreeNode<T>* current = root;

    while (current != nullptr) {
        if (node->value < current->value) {
            current = current->left;
        }
        else if (node->value > current->value) {
            current = current->right;
        }
        else {
            return level;
        }
        level++;
    }

    return -1; // Nodo no encontrado en el árbol
}


// Reccorido por anchura
template <class T>
std::vector<AVLTreeNode<T>*> AVLTree<T>::bfs() const {
    std::vector<AVLTreeNode<T>*> result;
    if (root == nullptr) {
        return result;
    }

    std::queue<AVLTreeNode<T>*> queue;
    queue.push(root);

    while (!queue.empty()) {
        AVLTreeNode<T>* node = queue.front();
        queue.pop();

        result.push_back(node);

        if (node->left != nullptr) {
            queue.push(node->left);
        }

        if (node->right != nullptr) {
            queue.push(node->right);
        }
    }

    return result;
}



template class AVLTree<short>;
template class AVLTree<long>;
template class AVLTree<long long>;
template class AVLTree<std::string>;
template class AVLTree<Pelicula>;
