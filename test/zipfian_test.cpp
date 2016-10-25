
/*
 * zipfian_test.cpp - Tests zipfian generator
 *
 * The Zipfian generator are adapted from:
 *   https://github.com/efficient/msls-eval/blob/master/zipf.h
 *   https://github.com/efficient/msls-eval/blob/master/util.h
 */

#include "../src/test_suite.h"

/*
 * TestZipfianTheta() - Tests zipfian dist with a given theta
 */
void TestZipfianTheta(double theta) {
  _PrintTestName();
  dbg_printf("Theta = %f\n", theta);
  
  // Set theta = 0 and this yields uniform distribution from 0 and wraps
  // back at n
  Zipfian zipf{30 * 1024 * 1024, theta, 0};
  
  for(int i = 0;i < 200;i++) {
    dbg_printf("i = %d; zipfian = %lu\n", i, zipf.Get()); 
  }
  
  return;
}

int main() {
  TestZipfianTheta(0.0);
  TestZipfianTheta(50.0);
  TestZipfianTheta(0.99);
  
  return 0; 
}
