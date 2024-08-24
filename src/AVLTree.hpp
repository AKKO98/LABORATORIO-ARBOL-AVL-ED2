#ifndef AVLTree_hpp
#define AVLTree_hpp

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include "AVLTreeNode.hpp"
#include <queue>

template <class T>
class AVLTree {
    int _size;
    AVLTreeNode<T> *root;
    
    void balance(std::vector<AVLTreeNode<T>**> path);
    
public:
    AVLTree();
    ~AVLTree();
    
    void insert(T value);
    void erase(T value);
    
    AVLTreeNode<T>* getRoot() { return root; }

    void clear();
    bool empty() const;
    int size() const;
    
    int find(T value) const;
    int lower_bound(T value) const;
    int upper_bound(T value) const;

    const T& find_min() const;
    const T& find_max() const;
    const T& operator[](std::size_t idx) const;
    
    void display(const std::string& prefix, const AVLTreeNode<T>* node, bool isLeft, std::ofstream& file);

    void display();
    std::vector<AVLTreeNode<T>*> bfs() const;

    void graph_bfs(const std::string& filename);

};

#endif /* AVLTree_hpp */
