#ifndef AVLTree_hpp
#define AVLTree_hpp

#include <algorithm>
#include "AVLTreeNode.hpp"
#include <fstream>
#include <iostream>
#include <locale>
#include <map>
#include "Pelicula.h"
#include <queue>
#include <stack>
#include <string>
#include <vector>

template <class T>
class AVLTree {
private:
    int _size;
    AVLTreeNode<T> *root;
    
    void balance(std::vector<AVLTreeNode<T>**> path);
    std::vector<AVLTreeNode<T>*> bfsRecursive(std::vector<AVLTreeNode<T>*>& result, std::queue<AVLTreeNode<T>*>& queue) const;

    
public:
    AVLTree();
    ~AVLTree();
    
    void insert(T& value);
    void erase(T value);
    
    AVLTreeNode<T>* getRoot() { return root; }

    void clear();
    bool empty() const;
    int size() const;
    
    int lower_bound(T value) const;
    int upper_bound(T value) const;

    int find(T value) const;

    int getLevel(AVLTreeNode<T>* node);
    const T& find_min() const;
    const T& find_max() const;
    AVLTreeNode<T>* operator[](std::size_t idx);
    

    void display(const std::string& filename);
    std::vector<AVLTreeNode<T>*> bfs() const;

    std::vector<AVLTreeNode<T>*> bfsRecursive() const;


};

#endif /* AVLTree_hpp */
