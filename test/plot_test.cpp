
/*
 * plot_test.cpp - Tests plot wrapper
 */
 
#include "plot_suite.h" 
#include "test_suite.h"
 
/*
 * ColorTest() - Tests whether color works
 */
void ColorTest() {
  _PrintTestName();
  
  Color c1{"#123456"};
  Buffer buffer;
  
  c1.AppendToBuffer(&buffer);
  dbg_printf("Color 1: ");
  
  buffer.Append('\n');
  buffer.Append('\0');
  buffer.WriteToFile(stderr);
  buffer.Reset();
  
  Color c2{0x12, 0x34, 0x56};
  c2.AppendToBuffer(&buffer);
  dbg_printf("Color 2: ");
  
  buffer.Append('\n');
  buffer.Append('\0');
  buffer.WriteToFile(stderr);
  buffer.Reset();
  
  return;
}

int main() {
  ColorTest();
  
  return 0; 
} 
