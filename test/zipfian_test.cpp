
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

/*
 * DrawZipfianDistribution() - Draw a diagram on the distribution
 *
 * This function generates a workload first, and then output a distribution file
 * and draws a diagram using gnuplot
 */
void DrawZipfianDistribution(double theta) {
  // This is the name of the output file
  //static constexpr const char *file_name = "_zipfian_dist.txt";
  _PrintTestName();
  
  
  
  return;
}

int main() {
  TestZipfianTheta(0.0);
  TestZipfianTheta(50.0);
  TestZipfianTheta(0.99);
  
  return 0; 
}
