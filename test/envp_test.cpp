
/*
 * arg_test.cpp - Tests envp extractor that helps determine environmental 
 *                variables
 */
 
#include "test_suite.h"

/*
 * TestEnvpBasic() - Tests basic envp functionality
 *
 * Test this with the following envp:
 *   key1 -> value1
 *   key2 -> 2
 *   key3 -> asdfg
 */
void TestEnvpBasic() {
  _PrintTestName();
  
  // This should return "value1"
  std::string value1 = Envp::Get("key1");
  assert(value1 == "value1");
  
  // This should return empty string
  std::string value0 = Envp::Get("key0");
  assert(value0 == "");
  
  bool ret;
  unsigned long value = 12345;
  
  // Value should be 2 after this
  ret = Envp::GetValueAsUL("key2", &value);
  assert(value == 2);
  assert(ret == true);
  
  value = 12345;
  ret = Envp::GetValueAsUL("key0", &value);
  assert(value == 12345);
  assert(ret == true);
  
  value = 12345;
  ret = Envp::GetValueAsUL("key3", &value);
  assert(ret == false);
  
  return;
}

int main() {
  TestEnvpBasic();
  
  return 0; 
}
