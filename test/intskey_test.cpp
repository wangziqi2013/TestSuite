
/*
 * intskey_test.cpp - Unit test for IntsKey module
 */
 
#include "ints_key.h"

/*
 * TestIntsKeySetAndGet() - Tests 
 */
void TestIntsKeySetAndGet() {
  _PrintTestName();
  
  int a1 = -1;
  unsigned char b1 = 123;
  long c1 = -85412;
  
  int a2 = -1;
  unsigned char b2 = 254;
  long c2 = 1;
  
  size_t a_offset = 0;
  size_t b_offset = a_offset + sizeof(a1);
  size_t c_offset = b_offset + sizeof(b1);
  
  IntsKey<4> k1, k2;
  k1.AddInteger(a1, a_offset);
  k1.AddUnsignedInteger(b1, b_offset);
  k1.AddInteger(c1, c_offset);
  
  k2.AddInteger(a2, a_offset);
  k2.AddUnsignedInteger(b2, b_offset);
  k2.AddInteger(c2, c_offset);
  
  assert(k1.GetInteger<decltype(a1)>(a_offset) == a1);
  assert(k1.GetUnsignedInteger<decltype(b1)>(b_offset) == b1);
  assert(k1.GetInteger<decltype(c1)>(c_offset) == c1);
  
  assert(k2.GetInteger<decltype(a2)>(a_offset) == a2);
  assert(k2.GetUnsignedInteger<decltype(b2)>(b_offset) == b2);
  assert(k2.GetInteger<decltype(c2)>(c_offset) == c2);

  assert(decltype(k1)::LessThan(k1, k2) == true);
  assert(decltype(k1)::Equals(k1, k2) == false);

  k1.PrintRawData();
  k2.PrintRawData();

  return;
}

int main() {
  TestIntsKeySetAndGet();
  
  return 0;
} 
