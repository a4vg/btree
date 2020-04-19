#include <iostream>
#include <vector> 

template <typename T>
class SS_Traits{
public:
  typedef T  value_t;
  typedef std::vector<value_t> container_t;
  typedef std::vector<value_t*> pcontainer_t;
  typedef typename container_t::iterator iterator_t;
  
  class simple_search{
  public:
    bool operator() (container_t a, value_t v){
      return false;
    }
  };

  class post_order_print{
  public:
    void operator() (void){
      std::cout << "post order" << std::endl;
    }
  };

  typedef simple_search functor_t;
  typedef post_order_print print_t;
};

template <typename T>
class BS_Traits{
public:
  typedef T  value_t;
  typedef std::vector<value_t> container_t;
  typedef std::vector<value_t*> pcontainer_t;
  typedef typename container_t::iterator iterator_t;
  
  class binary_search{
  public:
    bool operator() (container_t a, value_t v){
      return false;
    }
  };

  class pre_order_print{
  public:
    void operator() (void){
      std::cout << "pre order" << std::endl;
    }
  };

  typedef binary_search functor_t;
  typedef pre_order_print print_t;
};

template <typename T, int S> 
class BNode {
public:
  typedef typename T::value_t value_t;
  typedef typename T::container_t container_t;
  typedef typename T::pcontainer_t pcontainer_t;

  container_t keys;
  pcontainer_t ptrs;
  std::size_t order;

  BNode(void):order(S){
    keys=container_t(order,0);
    ptrs=pcontainer_t(order+1,NULL);
  }

  ~BNode(void){}
};

template <typename T, int S>
class BTree {
public: 
  typedef typename T::value_t value_t;
  typedef typename T::functor_t functor_t;
  typedef typename T::print_t print_t;

  BNode<T,S>* root;
  print_t print;
  functor_t search;

private:
  value_t createChildren(BNode<T,S>* poverf_node, BNode<T,S>& child1_node, BNode<T,S>& child2){
    /*
      Creates two children from overflowed node and returns the extra key (middle one)
    */

    size_t mid_i = poverf_node->keys.size()/2;
    size_t overfnode_i = 0, i;
    // Copy overflowed node contents to first child
    for (i=0; overfnode_i<mid_i; ++i, ++overfnode_i){
      child1_node.keys[i] = poverf_node->keys[overfnode_i];
      child1_node.ptrs[i] = poverf_node->ptrs[overfnode_i];
    }

    ++overfnode_i; // skip middle

    // Copy overflowed node contents to second child
    for (i=0; overfnode_i<poverf_node->keys.size(); ++i, ++overfnode_i){
      child2.keys[i] = poverf_node->keys[overfnode_i];
      child2.ptrs[i] = poverf_node->ptrs[overfnode_i];
    }
    child2.ptrs[i+1] = poverf_node->ptrs[overfnode_i+1]; // last ptr

    return poverf_node->keys[mid_i];
  }

  void split(BNode<T,S>* pparent_node, size_t target_i){
    BNode<T,S> child1_node, child2_node;
    value_t mid_key = createChildren(*(pparent_node->ptrs[target_i]), split1_node, split2_node);

    // Assign children nodes to parent
    pparent_node->ptrs[target_i] = &child1_node;
    pparent_node->ptrs[target_i+1] = &child2_node;

    insert(pparent_node, mid_key);
  }

  void insertInLeaf(BNode<T,S> &leaf_node, const value_t& val){
    // Displace elements
    value_t last_key = leaf_node.keys.back();
    BNode<T,S>* plast_ptr = leaf_node.ptrs.back();
    for (size_t i=leaf_node.eys.size()-1; i>=target_i; --i){
      leaf_node.keys[i] = leaf_node.keys[i-1];
      leaf_node.ptrs[i+1] = leaf_node.ptrs[i];
    }

    // Insert
    leaf_node.keys[target_i] = val;
    leaf_node.ptrs[target_i+1] = nullptr;

    // Append elements if node was full (creates overflow)
    if (last_key){
      leaf_node.keys.push_back(last_key);
      leaf_node.ptrs.push_back(plast_ptr);
    }
  }

  bool insert(BNode<T,S>* pcur_node, const value_t& val){
    /*
      Returns true if node is overflowing.
    */

    size_t target_i=0;
    for (; target_i<pcur_node->keys || pcur_node->keys[target_i]<val; ++target_i);
    if (pcur_node->ptrs[i])
      if (insert(pcur_node->ptrs[target_i], val)){ // overflow
        split(pcur_node, target_i);
        return false;
      }

    // Current node is a leaf
    insertInLeaf(*pcur_node, val);

    // Check overflow
    return pcur_node->keys.size()>pcur_node->order;
  }

public:
  BTree(void):root(NULL){
  }

  ~BTree(void){}

  void insert(const value_t& val = 0){
    if (empty()) root = new BNode<T,S>;
    if (insert(root, val)){  // root overflow
      BNode<T,S> new_root, child1_node, child2_node;
      value_t mid_key = createChildren(root, child1_node, child2_node);
      new_root.keys[0]= mid_key;
      new_root.ptrs[0] = &child1_node;
      new_root.ptrs[1] = &child2_node;
      root = &new_root;
    }
  }

  bool find(const value_t = 0) const{
    // TODO :: SEARCH
    // search(x); inside each page
    return false;
  }

  bool empty(){
    return !root;
  }

  template <typename _T, int _S>
  friend std::ostream& operator<<(std::ostream& out, BTree<_T,_S> tree){
    tree.print();// (out)
    // IN PRE POST LEVEL ORDER
    return out;
  }

};

int main() {
  typedef BS_Traits<int> btrait_t;
  BTree<btrait_t,4> tree;
  tree.find(10);
  std::cout<<tree<< std::endl;


  typedef SS_Traits<float> strait_t;
  BTree<strait_t,10> stree; 
  std::cout<<stree<< std::endl;
}