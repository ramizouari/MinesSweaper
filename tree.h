#ifndef TREE_H
#define TREE_H
#include <utility>

namespace  comparaison{
     using R = std::pair<std::size_t,std::size_t>;
    bool operator<=(R a,R b);
     bool operator<(R a,R b);
}


class tree
{
public:
    using R = comparaison::R;
    tree(R a);
    void insert(R a);
    void insert(tree p);
    void insert(tree *ptr);
private:
    using ptrTree =tree*;
    void change_root(ptrTree n);
    ptrTree left,right;
    R val;
    ptrTree root;

};

#endif // TREE_H
