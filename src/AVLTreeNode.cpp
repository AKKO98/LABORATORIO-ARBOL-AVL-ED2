#include "AVLTreeNode.hpp"
#include <sstream>

//Constructor
template <class T>
AVLTreeNode<T>::AVLTreeNode(T value) : value(value), left(nullptr), right(nullptr), count(1), height(1) {}

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
    
    this->updateValues();  // EL ORDEN ES IMPORTANTE
    R->updateValues();
    
    return R;
}

template <class T>
AVLTreeNode<T>* AVLTreeNode<T>::right_rotate(){
    AVLTreeNode* L = left;
    left = left->right;
    L->right = this;
    
    this->updateValues();  // EL ORDEN ES IMPORTANTE WAAaaAAaaaAAaaaAaaAA
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


template<class T>
bool AVLTreeNode<T>::operator==(const T& other) const
{
    return this->value == other;
}


template struct AVLTreeNode<int>;
template struct AVLTreeNode<short>;
template struct AVLTreeNode<long>;
template struct AVLTreeNode<long long>;
template struct AVLTreeNode<std::string>;
template struct AVLTreeNode<Pelicula>;
