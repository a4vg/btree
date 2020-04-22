#include <time.h>
#include <stdio.h>
#include "btree.h"

int main() {
  srand(time(NULL));
  typedef BS_Traits<int> btrait_t;
  typedef SS_Traits<float> strait_t;

  std::cout << "******** Binary Search BTree ********\n";
  BTree<btrait_t,4> btree;
  std::vector<int> i_values;
  std::cout << "Order: " << btree.order;

  for (int i=0; i<20; ++i){
    int val = rand() % 100;
    i_values.push_back(val);
    btree.insert(val);
  }
  std::cout << "\nElements inserted: ";
  for (auto& ival: i_values) std::cout << ival << ", ";
  std::cout << btree << "\n";
  int to_findi = rand() % 100;
  std::cout << "\nfind(" << to_findi << ") : " << btree.find(to_findi);
  std::cout << "\nfind(" << i_values[0] << ") : " << btree.find(i_values[0]);

  std::cout << "\n\n******** Sequential Search BTree ********\n";
  BTree<strait_t,10> stree;
  std::vector<float> f_values;
  std::cout << "Order: " << stree.order;

  for (int i=0; i<200; ++i){
    float val = (float)(rand() % 10000)/100;
    f_values.push_back(val);
    stree.insert(val);
  }
  
  std::cout << "\nElements inserted: ";
  for (auto& fval: f_values) std::cout << fval << ", ";
  std::cout << stree << "\n";
  
  int to_findf = (rand() % 10000)/100;
  std::cout << "\nfind(" << to_findf << ") : " << stree.find(to_findf);
  std::cout << "\nfind(" << f_values[0] << "): " << stree.find(f_values[0]);
}