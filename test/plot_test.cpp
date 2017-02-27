
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

/*
 * RoundUpTest() - Tests whether rounding works
 */
void RoundUpTest() {
  _PrintTestName(); 
  
  double num = 5.92L;
  dbg_printf("Prev = %f; After = %f\n", num, BarChart::RoundUpToPoint5(num));
  
  num = 7.00L;
  dbg_printf("Prev = %f; After = %f\n", num, BarChart::RoundUpToPoint5(num));
  
  num = 9.15L;
  dbg_printf("Prev = %f; After = %f\n", num, BarChart::RoundUpToPoint5(num));
  
  num = 15.55L;
  dbg_printf("Prev = %f; After = %f\n", num, BarChart::RoundUpToPoint5(num));
  
  num = 27.41L;
  dbg_printf("Prev = %f; After = %f\n", num, BarChart::RoundUpToPoint5(num));
  
  return;
}

int main() {
  ColorTest();
  RoundUpTest();
  
  return 0; 
} 
