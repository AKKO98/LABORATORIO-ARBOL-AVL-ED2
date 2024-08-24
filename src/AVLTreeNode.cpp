#include "AVLTreeNode.hpp"
#include <sstream>

//Constructor
template <class T>
AVLTreeNode<T>::AVLTreeNode(T value) : value(value), left(nullptr), right(nullptr), count(1), height(1) {}

template <class T>
AVLTreeNode<T>::AVLTreeNode() : value(NULL) {
    count = 1;
    height = 1;

    left = nullptr;
    right = nullptr;
}

template <class T>
void AVLTreeNode<T>::updateValues(){
    count = (left != nullptr ? left->count : 0) + (right != nullptr ? right->count : 0) + 1;
    
    height = std::max(left != nullptr ? left->height : 0,
                      right != nullptr ? right->height : 0) + 1;
}

template <class T>
int AVLTreeNode<T>::balanceFactor(){
    return (left != nullptr ? left->height : 0) - (right != nullptr ? right->height: 0);
}

template <class T>
AVLTreeNode<T>* AVLTreeNode<T>::left_rotate(){
    AVLTreeNode* R = right;
    right = right->left;
    R->left = this;
    
    this->updateValues();  // the order is important
    R->updateValues();
    
    return R;
}

template <class T>
AVLTreeNode<T>* AVLTreeNode<T>::right_rotate(){
    AVLTreeNode* L = left;
    left = left->right;
    L->right = this;
    
    this->updateValues();  // the order is important
    L->updateValues();
    
    return L;
}


// Implementación de la función to_string()
template <class T>
std::string AVLTreeNode<T>::to_string() const {
    if constexpr (std::is_same<T, int>::value || std::is_same<T, float>::value || std::is_same<T, double>::value) {
        return std::to_string(value);  // Usar std::to_string para tipos primitivos
    }
    else {
        std::ostringstream oss;
        oss << value;  // Usar std::ostringstream para otros tipos
        return oss.str();
    }
}

template struct AVLTreeNode<int>;
template struct AVLTreeNode<short>;
template struct AVLTreeNode<long>;
template struct AVLTreeNode<long long>;
template struct AVLTreeNode<std::string>;
