#include <iostream>
#include <vector>

enum { BITSPERWORD = 32, SHIFT = 5, MASK = 0x1F };

class IntSetBitVec {
 private:
  int n, hi, *data;

 private:
  void set(int i) { data[i >> SHIFT] |= (1 << (i & MASK)); }

  void clr(int i) { data[i >> SHIFT] &= ~(1 << (i & MASK)); }

 public:
  IntSetBitVec(int max_elements, int max_val) {
    n = 0;
    hi = max_elements;
    data = new int[1 + max_elements / BITSPERWORD];
  }

  ~IntSetBitVec() { free(data); }

  int find(int i) { return data[i >> SHIFT] & (1 << (i & MASK)); }

  void insert(int i) {
    if (!find(i)) n++;
    set(i);
  }

  int size() { return n; }

  int* report() {
    int i = 0, j = 0, *out = new int[n];
    while (j < n) {
      if (find(i)) {
        out[j++] = i;
      }
      i++;
    }
    return out;
  }

  void remove(int i) { clr(i); }
};

struct BSTNode {
  int val;
  BSTNode* left;
  BSTNode* right;
};

class IntSetBST {
 private:
  int n;
  BSTNode* root;
  BSTNode* node_pool;

 public:
  IntSetBST(int size, int high) {
    n = 0;
    node_pool = new BSTNode[size + 1];
    root = &node_pool[0];
    node_pool += 1;
  }

  void insert(int val) {
    BSTNode* cur = root;
    while (true) {
      if (val == cur->val) break;

      if (val < cur->val) {
        if (cur->left == nullptr) {
          cur->left = &this->node_pool[this->n];
          cur->left->val = val;
          n++;
          return;
        } else {
          cur = cur->left;
        }
      } else if (val > cur->val) {
        if (cur->right == nullptr) {
          cur->right = &this->node_pool[this->n];
          cur->right->val = val;
          n++;
          return;
        } else {
          cur = cur->right;
        }
      }
    }
  }

  int size() { return n; }

  int* report() {
    int *out = new int[n], i = 0;
    BSTNode* node = root;
    std::vector<BSTNode*> stack;
    while (node != nullptr || stack.size() > 0) {
      while (node != nullptr) {
        stack.push_back(node);
        node = node->left;
      }

      node = stack.back();
      stack.pop_back();

      if (node != root) {
        out[i] = node->val;
        i++;
      }
      node = node->right;
    }
    return out;
  }

  ~IntSetBST() { delete[] root; }
};

int* bob_floyd(int size, int high) {
  int rand_num;
  IntSetBitVec int_set = IntSetBitVec(size, high);
  for (int i = high - size; i < high; i++) {
    rand_num = rand() % i;
    if (!int_set.find(rand_num)) {
      int_set.insert(rand_num);
    } else {
      int_set.insert(i);
    }
  }

  return int_set.report();
}

void test_intset_bit_vec() {
  auto start = std::chrono::high_resolution_clock::now();

  int* array = bob_floyd(10000000, 100000000);

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duration = end - start;
  std::cout << "Duration: " << std::endl << duration.count();
}

void test_intset_bst() {
  int n = 10000000;
  int m = 100000000;
  std::cout << "IntSetBST" << std::endl;
  std::cout << "Samples, MaxVal " << n << ", " << m << std::endl;
  auto start = std::chrono::high_resolution_clock::now();
  auto set = IntSetBST(n, m);
  while (set.size() < n) {
    set.insert(rand() % m);
  }
  auto end = std::chrono::high_resolution_clock::now();
  auto out = set.report();
  auto report = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double> duration = end - start;
  std::cout << "Generation Duration(ms): " << duration.count() * 1000
            << std::endl;
  duration = report - end;
  std::cout << "Report Duration(ms): " << duration.count() * 1000 << std::endl;
}

void test_intset_bitvec() {
  int n = 1000000;
  int m = n * 10;
  std::cout << "IntSetBitVec" << std::endl;
  std::cout << "Samples, MaxVal " << n << ", " << m << std::endl;
  auto start = std::chrono::high_resolution_clock::now();
  auto set = IntSetBitVec(n, m);
  while (set.size() < n) {
    set.insert(rand() % 100009);
  }
  auto end = std::chrono::high_resolution_clock::now();
  auto out = set.report();
  auto report = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double> duration = end - start;
  std::cout << "Generation Duration(ms): " << duration.count() * 100
            << std::endl;
  duration = report - end;
  std::cout << "Report Duration(ms): " << duration.count() * 100 << std::endl;
}

int main() { test_intset_bst(); }