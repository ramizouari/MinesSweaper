/*#include "tree.h"

tree::tree(R a):left(nullptr),right(nullptr),root(this),val(a)
{

}

void tree::insert(R a)
{
    if(a<val)
    {
        if(left)
            left->insert(a);
        else
        {
            left = new tree(a);
            left->root=this->root;
        }
    }
    else if(val<a)
    {
        if(right)
            right->insert(a);
        else
        {
            right= new tree(a);
            right->root=this->root;
        }
    }
}

 void tree::change_root(ptrTree n)
 {
    if(left)
        left->change_root(n);
    if(right)
        right->change_root(n);
   root=n;
 }

 bool comparaison::operator<=(R a,R b)
 {
    if(a.first==b.first)
        return a.second<=b.second;
    return a.first<=b.first;
 }

 bool comparaison::operator<(R a,R b)
 {
     if(a.first==b.first)
         return a.second<b.second;
     return a.first<b.first;
 }

 void tree::insert(tree p)
 {
    if(p.left)
        insert(p.le);

 }
 void tree::insert(tree *ptr)
 {

 }
*/
