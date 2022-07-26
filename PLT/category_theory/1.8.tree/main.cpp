#include <functional>
#include <memory>
#include <iostream>

template <typename T>
class Tree
{
public:
    virtual ~Tree(){};
};

template <typename T>
class Leaf : public Tree<T>
{
public:
    T _label;
    Leaf(T val) : _label{val} {}
};

template <typename T>
class Node : public Tree<T>
{
public:
    std::unique_ptr<Tree<T>> _left;
    std::unique_ptr<Tree<T>> _right;
    Node(
        std::unique_ptr<Tree<T>> l,
        std::unique_ptr<Tree<T>> r) 
        : _left{std::move(l)}, _right{std::move(r)} {}
};

template <typename A, typename B>
std::unique_ptr<Tree<B>> fmap(
    std::function<B(A)> f,
    Tree<A> *t)
{
    auto leaf = dynamic_cast<Leaf<A> *>(t);
    if (leaf)
        return std::make_unique<Leaf<B>>(f(leaf->_label));

    auto node = dynamic_cast<Node<A> *>(t);
    if (node)
    {
        return std::make_unique<Node<B>>(
            // calls fmap recursively on the children of the current node
            fmap<A, B>(f, node->_left.get()),
            fmap<A, B>(f, node->_right.get()));
    }

    return {};
}

float conv(int a)
{
    std::cout << a << '\n';
    return a;
}

int main()
{
    auto r = std::make_unique<Node<int>>(
        std::make_unique<Leaf<int>>(0),
        std::make_unique<Leaf<int>>(1)
    );

    auto ret = fmap<int, float>(conv, r.get());

    return 0;
}