#ifndef AVLTreeNode_hpp
#define AVLTreeNode_hpp
#include "Pelicula.h"

#include <string>
#include <algorithm>

template <class T>
struct AVLTreeNode {
    AVLTreeNode *left;
    AVLTreeNode *right;
    
    const T value;
    int count;  // Cuantos nodos hay en total de este sub arbol
    int height;
    

    AVLTreeNode(T value);
    void updateValues();
    int balanceFactor();

    
    AVLTreeNode* left_rotate();
    AVLTreeNode* right_rotate();

    std::string to_string() const;
    bool operator==(const T& other) const;

};

#endif /* AVLTreeNode_hpp */
