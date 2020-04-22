#include <iostream>
#include <vector> 
#include <limits>
#include <time.h>
#include <stdio.h>
#include "traits.h"

template <typename T, int S> 
class BNode {
public:
  typedef typename T::value_t value_t;
  typedef typename T::container_t container_t;
  typedef std::vector< BNode<T,S>* > pcontainer_t;

  value_t INF = std::numeric_limits<value_t>::max();
  container_t keys;
  pcontainer_t ptrs;
  std::size_t order;

  BNode(void):order(S){
    keys=container_t(order-1,INF);
    ptrs=pcontainer_t(order,NULL);
  }

  ~BNode(void){}

  template <typename _T, int _S>
  friend std::ostream& operator<<(std::ostream& out, BNode<_T,_S> node){

    std::cout << "[ ";
    for (const value_t& key: node.keys)
      if (key!=node.INF) std::cout << key << " ";
      else std::cout << "- ";

    std::cout << "]";
    return out;
  }
};

template <typename T, int S>
class BTree {
public: 
  typedef typename T::value_t value_t;
  typedef typename T::container_t container_t;
  typedef typename T::functor_t functor_t;
  typedef typename T::print_t print_t;

  const int order = S;
  value_t INF = std::numeric_limits<value_t>::max();
  BNode<T,S>* root;
  print_t printPost;
  functor_t search;

private:
  value_t createChildren(BNode<T,S>& overf_node, BNode<T,S>& child1_node, BNode<T,S>& child2_node){
    /*
      Creates two children from overflowed node and returns the extra key (middle one)
    */

    bool right_bias = true;
    size_t mid_i = overf_node.keys.size()/2-right_bias;
    size_t overf_i = 0, i;
    // Copy overflowed node contents to first child
    for (i=0; overf_i<mid_i; ++i, ++overf_i){
      child1_node.keys[i] = overf_node.keys[overf_i];
      child1_node.ptrs[i] = overf_node.ptrs[overf_i];
    }

    child1_node.ptrs[i] = overf_node.ptrs[overf_i]; // skip middle key
    ++overf_i;

    // Copy overflowed node contents to second child
    for (i=0; overf_i<overf_node.keys.size(); ++i, ++overf_i){
      child2_node.keys[i] = overf_node.keys[overf_i];
      child2_node.ptrs[i] = overf_node.ptrs[overf_i];
    }
    child2_node.ptrs[i] = overf_node.ptrs[overf_i]; // last ptr

    return overf_node.keys[mid_i];
  }

  void split(BNode<T,S>* parent_pnode, size_t target_i){
    BNode<T,S>* child1_node = new BNode<T,S>;
    BNode<T,S>* child2_node= new BNode<T,S>;
    value_t mid_key = createChildren(*(parent_pnode->ptrs[target_i]), *child1_node, *child2_node);

    insertInNode(*parent_pnode, target_i, mid_key);

    // Assign children nodes to parent
    parent_pnode->ptrs[target_i] = child1_node;
    parent_pnode->ptrs[target_i+1] = child2_node;
  }

  void insertInNode(BNode<T,S> &leaf_node, size_t target_i, const value_t& val){
    // Displace elements
    value_t last_key = leaf_node.keys.back();
    BNode<T,S>* last_ptr = leaf_node.ptrs.back();
    for (size_t i=leaf_node.keys.size()-1; i>target_i; --i){
      leaf_node.keys[i] = leaf_node.keys[i-1];
      leaf_node.ptrs[i+1] = leaf_node.ptrs[i];
    }

    // Append elements if node was full (creates overflow)
    if (last_key!=INF){
      leaf_node.keys.push_back(last_key);
      leaf_node.ptrs.push_back(last_ptr);
    }

    // Insert
    leaf_node.keys[target_i] = val;
    leaf_node.ptrs[target_i+1] = nullptr;
  }

  bool insert(BNode<T,S>* cur_pnode, const value_t& val){
    /*
      Returns true if node is overflowing.
    */

    size_t target_i = search(cur_pnode->keys, val);
    target_i += 1*(target_i<cur_pnode->keys.size() &&  cur_pnode->keys[target_i]<val);

    if (cur_pnode->ptrs[target_i]){
      if (insert(cur_pnode->ptrs[target_i], val)) // overflow
        split(cur_pnode, target_i);
    }
    else{
      // Current node is a leaf or root
      insertInNode(*cur_pnode, target_i, val);
    }

    // Check overflow
    return cur_pnode->keys.size()==cur_pnode->order;
  }

  void postorderPrint(std::ostream& out, BNode<T,S>* cur_pnode, int space=0){
    if (!cur_pnode) return;

    for (auto& ptr: cur_pnode->ptrs)
      if (ptr) postorderPrint(out, ptr, space+2);

    out << "\n";
    for (int i=0; i<space; ++i) out << " ";
    out << *cur_pnode;
    }

  void preorderPrint(std::ostream& out, BNode<T,S>* cur_pnode, int space=0){
    if (!cur_pnode) return;
    out << "\n";
    
    for (int i=0; i<space; ++i) out << " ";
    out << *cur_pnode;
    
    for (auto& ptr: cur_pnode->ptrs)
      if (ptr) preorderPrint(out, ptr, space+2);
  }

  bool find(BNode<T,S>* cur_pnode, const value_t& val){
    size_t target_i = search(cur_pnode->keys, val);
    target_i += 1*(target_i<cur_pnode->keys.size() &&  cur_pnode->keys[target_i]<val);

    if (cur_pnode->keys[target_i]==val)
      return true; // found
    else if (cur_pnode->ptrs[target_i])
      return find(cur_pnode->ptrs[target_i], val); // go down
    else
      return false;
  }

public:
  BTree(void):root(NULL){
  }

  ~BTree(void){}

  void insert(const value_t& val = 0){
    if (empty()) root = new BNode<T,S>;
    if (insert(root, val)){  // root overflow
      BNode<T,S>* new_root = new BNode<T,S>;
      BNode<T,S>* child1 = new BNode<T,S>;
      BNode<T,S>* child2 = new BNode<T,S>;
      value_t mid_key = createChildren(*root, *child1, *child2);
      new_root->keys[0]= mid_key;
      new_root->ptrs[0] = child1;
      new_root->ptrs[1] = child2;
      root = new_root;
    }
  }

  bool find(const value_t& val= 0){
    if (empty()) return false;
    return find(root, val);
  }

  bool empty(){
    return !root;
  }

  template <typename _T, int _S>
  friend std::ostream& operator<<(std::ostream& out, BTree<_T,_S> tree){
    bool post = tree.printPost();
    if (post){
      out << "\nPost order print";
      tree.postorderPrint(out, tree.root);
    }
    else{
      out << "\nPre order print";
      tree.preorderPrint(out, tree.root);
    }
    return out;
  }

};