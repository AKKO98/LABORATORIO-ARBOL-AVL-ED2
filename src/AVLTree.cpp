#include "AVLTreeNode.hpp"
#include "AVLTree.hpp"
#include <fstream>
#include <locale>
#include <stack>
#include <queue>
#define GVDLL
#include <graphviz/cgraph.h>
#include <graphviz/gvc.h>

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
void AVLTree<T>::insert(T value){
    if (find(value) != -1)
    {
        return;
    }

    AVLTreeNode<T> **indirect = &root;  // to generalize insertion
    std::vector<AVLTreeNode<T>**> path;  // to update height values
    
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
    AVLTreeNode<T> **indirect = &root;  // to generalize insertion
    std::vector<AVLTreeNode<T>**> path;  // to update height values
    
    while (*indirect != nullptr and (*indirect)->value != value){
        path.push_back(indirect);
        
        if ((*indirect)->value > value)
            indirect = &((*indirect)->left);
        else
            indirect = &((*indirect)->right);
    }
    
    if (*indirect == nullptr)  // the value does not exist in tree
        return;  // may be raising an Exception is more elegant
    
    else
        path.push_back(indirect);
    
    std::size_t index = path.size();
    
    if ((*indirect)->left == nullptr and (*indirect)->right == nullptr){  // the node is leaf
        delete *indirect;  // just delete node
        *indirect = nullptr;
        path.pop_back();  // pop the deleted node from path
    }
    
    else if ((*indirect)->right == nullptr){  // only left child exists
        AVLTreeNode<T> *toRemove = *indirect;
        
        (*indirect) = (*indirect)->left;
        delete toRemove;
        
        path.pop_back();
    }
    
    else{  // right child exists
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
void AVLTree<T>::balance(std::vector<AVLTreeNode<T> **> path){  // starting from root
    std::reverse(path.begin(), path.end());
    
    for (auto indirect: path){
        (*indirect)->updateValues();
        
        if ((*indirect)->balanceFactor() >= 2 and (*indirect)->left->balanceFactor() >= 0)   // left - left
            *indirect = (*indirect)->right_rotate();
        
        else if ((*indirect)->balanceFactor() >= 2){  // left - right
            (*indirect)->left = (*indirect)->left->left_rotate();
            *indirect = (*indirect)->right_rotate();
        }
        
        else if ((*indirect)->balanceFactor() <= -2 and (*indirect)->right->balanceFactor() <= 0)  // right - right
            *indirect = (*indirect)->left_rotate();
        
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

template <class T>
const T& AVLTree<T>::find_min() const{
    AVLTreeNode<T> *cur = root;
    
    while (cur->left != nullptr)
        cur = cur->left;
    
    return cur->value;
}

template <class T>
const T& AVLTree<T>::find_max() const{
    AVLTreeNode<T> *cur = root;
    
    while (cur->right != nullptr)
        cur = cur->right;
    
    return cur->value;
}

template <class T>
const T& AVLTree<T>::operator[](std::size_t idx) const{
    AVLTreeNode<T> *cur = root;
    int left = cur->left != nullptr ? cur->left->count : 0;
    
    while (left != idx){
        if (left < idx){
            idx -= left + 1;
            
            cur = cur->right;
            left = cur->left != nullptr ? cur->left->count : 0;
        }
        
        else{
            cur = cur->left;
            left = cur->left != nullptr ? cur->left->count : 0;
        }
    }
    
    return cur->value;
}



template <class T>
void AVLTree<T>::display(const std::string& prefix, const AVLTreeNode<T>* node, bool isLeft, std::ofstream& file) {
    if (node != nullptr) {
        // Imprimir el prefijo actual
        file << prefix;

        // Imprimir el borde izquierdo o derecho del nodo actual
        file << (isLeft ? "+-- " : "+-- ");

        // Imprimir el valor del nodo
        file << node->value << std::endl;

        // Entrar al siguiente nivel del árbol - ramas izquierda y derecha
        display(prefix + (isLeft ? "|   " : "    "), node->left, true, file);
        display(prefix + (isLeft ? "|   " : "    "), node->right, false, file);
    }
}

template <class T>
void AVLTree<T>::display() {
    std::ofstream file("arbol.txt");
    if (file.is_open()) {
        display("", root, false, file);
    }
    file.close();
}

template <class T>
void AVLTree<T>::graph_bfs(const std::string& filename) {
    // Obtener el vector BFS del árbol
    std::vector<AVLTreeNode<T>*> bfs_seq = bfs();

    // Crear un contexto de Graphviz
    GVC_t* gvc = gvContext();

    // Crear un grafo dirigido
    Agraph_t* g = agopen(_strdup("AVLTree"), Agdirected, nullptr);

    // Crear nodos en el grafo
    std::vector<Agnode_t*> nodes;
    for (AVLTreeNode<T>* value : bfs_seq)
    {
        std::string label = value->to_string();
        nodes.push_back(agnode(g, _strdup(label.c_str()), 1));
    }

    //for (size_t i = 0; i < bfs_seq.size(); ++i) {
    //    // Crear un nodo con el valor en bfs_seq[i]
    //    std::string label = bfs_seq[i].;
    //    nodes.push_back(agnode(g, _strdup(label.c_str()), 1));
    //}

    // Crear aristas (edges) en el grafo
    for (size_t i = 0; i < bfs_seq.size(); ++i) {
        size_t left_index = 2 * i + 1;
        size_t right_index = 2 * i + 2;

        if (left_index < bfs_seq.size()) {
            agedge(g, nodes[i], nodes[left_index], nullptr, 1);
        }

        if (right_index < bfs_seq.size()) {
            agedge(g, nodes[i], nodes[right_index], nullptr, 1);
        }
    }

    // Layout y renderizado del grafo
    gvLayout(gvc, g, "dot");
    gvRenderFilename(gvc, g, "png", filename.c_str());

    // Liberar recursos
    gvFreeLayout(gvc, g);
    agclose(g);
    gvFreeContext(gvc);
}

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

template class AVLTree<int>;
template class AVLTree<short>;
template class AVLTree<long>;
template class AVLTree<long long>;
template class AVLTree<std::string>;
