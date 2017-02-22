
/*
 * PermutationTest() - Tests whether permutation works as expected
 */

#include "test_suite.h"

/*
 * TestSimplePermutation() - Tests whether permutation works
 */
void TestSimplePermutation(int count, int start) {
  _PrintTestName();
   
  dbg_printf("Permute %d - %d\n", start, start + count - 1);
  
  Permutation<int> permute{(size_t)count, start};
  for(int i = 0;i < 20;i++) {
    printf("%d ", permute[i]); 
  }
  
  putchar('\n');
  
  return;
}

int main() {
  // 0 - 19
  TestSimplePermutation(20, 0);
  
  // 10 - 29
  TestSimplePermutation(20, 10);
  
  return 0;
}
