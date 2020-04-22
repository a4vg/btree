#include <iostream>

template <typename T>
class SS_Traits{
public:
  typedef T  value_t;
  typedef std::vector<value_t> container_t;
  typedef typename container_t::iterator iterator_t;
  
  class simple_search{
  public:
    int operator() (container_t a, value_t v){
      int i=0;
      while (i<a.size() && a[i]<v)
        ++i;
      return i;
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
  typedef typename container_t::iterator iterator_t;
  
  class binary_search{
  public:
    int operator() (container_t a, value_t v){
      int limits[2] = {0, (int)a.size()-1};
      int mid_i;
      while (limits[0]<=limits[1]){
        mid_i = (limits[0] + limits[1])/2;
        if (a[mid_i]==v) return mid_i;
        if (a[mid_i]>v) limits[1] = mid_i-1; // first half
        else limits[0] = mid_i+1; // second half
      }
      return mid_i;
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