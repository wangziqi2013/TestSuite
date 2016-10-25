
/*
 * zipfian_test.cpp - Tests zipfian generator
 *
 * The Zipfian generator are adapted from:
 *   https://github.com/efficient/msls-eval/blob/master/zipf.h
 *   https://github.com/efficient/msls-eval/blob/master/util.h
 */
 
void TestZipfianTheta0() {
  // Set theta = 0 and this yields uniform distribution from 0 and wraps
  // back at n
  Zipfian zipf{100, 0.0, 0};
  
  
}

int main() {
  return 0; 
}
